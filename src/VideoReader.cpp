#include <iostream>
#include "VideoReader.h"


using std::cout;
using std::endl;

#define __STDC_CONSTANT_MACROS
extern "C"
{
    #include <libavutil/imgutils.h>
    #include <libavutil/samplefmt.h>
    #include <libavutil/timestamp.h>
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
}

VideoReader::VideoReader(const char* _src_file, const char* _video_output_file, const char* _audio_output_file)
{
    src_file = _src_file;
    video_output_file = _video_output_file;
    audio_output_file = _audio_output_file;

    // Format context
    fmt_ctx = NULL;

    // Codec context (decoder)
    vid_dec_ctx = NULL;
    audio_dec_ctx = NULL;

    // Stream index
    video_stream_idx = -1;
    audio_stream_idx = -1;
    // Streams
    video_stream = NULL;
    audio_stream = NULL;

    video_dst_file = NULL;
    audio_dst_file = NULL;

    width = 0;
    height = 0;

    video_dst_data[4] = { NULL };
    video_dst_bufsize = -1;

    frame = NULL;

    video_frame_count = 0;
    audio_frame_count = 0;
}


VideoReader::~VideoReader()
{
    cout << "Deallocating memory" << endl;
    // Deallocate memory
    av_frame_free(&frame);

    avcodec_free_context(&vid_dec_ctx);
    avcodec_free_context(&audio_dec_ctx);

    avformat_close_input(&fmt_ctx);
}


void VideoReader::SaveFrame(AVFrame *pFrame)
{
    FILE *pFile;
    // Open file
    pFile = fopen("/home/ubuntu/Documents/Github/LazyEdit/resource/out/image", "wb");
    if(pFile == NULL)
        return;
    
    // Write header
    fprintf(pFile, "P6\n%d %d\n255\n", width, height);
    
    // Write pixel data
    n_frames++;
    for(int y = 0; y < height; y++) {
        fwrite(pFrame->data[0]+y*pFrame->linesize[0], 1, width, pFile);
    }
        
    if(n_frames == 200)
        exit(0);
    
    // Close file
    fclose(pFile);
}


int VideoReader::decode_packet(AVCodecContext* dec, const AVPacket* pkt)
{
    int status = 0;

    // Submit the packet to the decoder
    status = avcodec_send_packet(dec, pkt);
    if (status < 0) {
        cout << "Could not submit packet for decoding\n";
        return status;
    }
    
    // Get all frames from decoder
    while (status >= 0) {
        status = avcodec_receive_frame(dec, frame);
        if (status < 0) {
            if (status == AVERROR_EOF ||status == AVERROR(EAGAIN))
                return 0;
            
            cout << "Error during decoding\n";
            return status;
        }
        
        if(dec->codec->type == AVMEDIA_TYPE_VIDEO)
            SaveFrame(frame);
        
        av_frame_unref(frame);
    }
    return 0;
}

int VideoReader::open_codec_context(int &stream_index,
            AVCodecContext** dec_ctx, enum AVMediaType type)
{
    /*
        Summary
        (1) Find stream
            (a) Use: av_find_best_stream
        (2) Find stream decoder
            (a) Use: avcodec_find_decoder

        Rule of thumb:
            Never use the current AVCodecContext for any stream directly!
        
        (3) Allocate a AVCodecContext for decoder
            (a) Use: avcodec_alloc_context3
        (4) Copy codec context of input stream to output
            (a) Use: avcodec_parameters_to_context
        (5) Open decoders
            (a) Use: avcodec_open2
    */
    int idx, status;
    const char* str_type = av_get_media_type_string(type);
    AVStream* stream;
    AVCodec* dec = NULL;

    idx = av_find_best_stream(fmt_ctx, type, -1, -1, NULL, 0);

    if (idx < 0) {
        cout << "Could not find " << str_type << " stream in source file\n";
        return idx;
    }
    else {
        stream = fmt_ctx->streams[idx];

        // Find the decoder for the stream
        dec = avcodec_find_decoder(stream->codecpar->codec_id);
        if (!dec) {
            cout << "Cannot find " << str_type << " codec\n";
            return AVERROR_DECODER_NOT_FOUND;
        }

        // Allocate a AVCodecContext for decoder
        *dec_ctx = avcodec_alloc_context3(dec);
        if (!*dec_ctx) {
            cout << "Could not allocate AVCodecContext for " << str_type << " decoder\n";
            return -1;
        }

        // Copy input codec context to output codec context
        if ((status = avcodec_parameters_to_context(*dec_ctx, stream->codecpar)) < 0) {
            cout << "Could not copy input to output codec context\n";
            return status;
        }

        // Open decoders
        if ((status = avcodec_open2(*dec_ctx, dec, NULL)) < 0)  {
            cout << "Could not open " << str_type << " codec\n";
            return status;
        }
        stream_index = idx;
    }
    return 0;
}


int VideoReader::demux()
{
    /* open input file, and allocate format context */
    if (avformat_open_input(&fmt_ctx, src_file, NULL, NULL) < 0) {
        cout << "[ERROR] Source file could not be opened!\n";
        return -1;
    }

    /*
        Retrieve stream information
        Populates fmt_ctx->stream
    */
   if (avformat_find_stream_info(fmt_ctx, NULL) < 0) {
       cout << "[ERROR] Stream information could not be found\n";
       return -1;
   }

    if (open_codec_context(video_stream_idx, &vid_dec_ctx, AVMEDIA_TYPE_VIDEO) >= 0) {
        video_stream = fmt_ctx->streams[video_stream_idx];

        video_dst_file = fopen(video_output_file, "wb");
        if (!video_dst_file) {
            cout << "Could not open video destination file\n";
            return -1;
        }

        /* Allocate image where the decoded image will be put */
        width = vid_dec_ctx->width;
        height = vid_dec_ctx->height;
        pix_fmt = vid_dec_ctx->pix_fmt;

        video_dst_bufsize = av_image_alloc(video_dst_data, video_dst_linesize, width, height, pix_fmt, 1);
        if (video_dst_bufsize < 0) {
            cout << "Could not allocate raw video buffer\n";
            return -1;
        }
    }

    if (open_codec_context(audio_stream_idx, &audio_dec_ctx, AVMEDIA_TYPE_AUDIO) >= 0) {
        audio_stream = fmt_ctx->streams[audio_stream_idx];

        audio_dst_file = fopen(audio_output_file, "wb");
        if (!audio_dst_file) {
            cout << "Could not open audio destination file\n";
            return -1;
        }
    }

    av_dump_format(fmt_ctx, 0, src_file, 0);

    if (!video_stream && !audio_stream) {
        cout << "Could not find audio/video in the input\n";
        return -1;
    }

    frame = av_frame_alloc();
    if (!frame) {
        cout << "Could not allocate frame\n";
        return -1;
    }

    // Initialize packet, set data to NULL, set demuxer
    av_init_packet(&pkt);
    pkt.data = NULL;
    pkt.size = 0;

    if (video_stream)
        cout << "Demuxing video\n";
    if (audio_stream)
        cout << "Demuxing audio\n";

    // Read frames to file
    int decoder_status = -1;
    while (av_read_frame(fmt_ctx, &pkt) == 0) {
        // Check if the packet belongs to the stream of interest
        if (pkt.stream_index == video_stream_idx)
            decoder_status = decode_packet(vid_dec_ctx, &pkt);
        else if (pkt.stream_index == audio_stream_idx)
            decoder_status = decode_packet(audio_dec_ctx, &pkt);
        
        av_packet_unref(&pkt);
        if(decoder_status < 0)
            break;
    }

    // Flush the decoder
    if (vid_dec_ctx)
        decode_packet(vid_dec_ctx, NULL);
    if (audio_dec_ctx)
        decode_packet(audio_dec_ctx, NULL);
    
    cout << "Demuxing succeeded\n";


    return 0;
}