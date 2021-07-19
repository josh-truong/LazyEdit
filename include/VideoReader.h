#ifndef VIDEOREADER_H
#define VIDEOREADER_H

#include <iostream>

#define __STDC_CONSTANT_MACROS
extern "C"
{
    #include <libavutil/imgutils.h>
    #include <libavutil/samplefmt.h>
    #include <libavutil/timestamp.h>
    #include <libavformat/avformat.h>
}

class VideoReader
{
    private:
        int n_frames=0;

        const char *src_file, *video_output_file, *audio_output_file;

        AVFormatContext *fmt_ctx;
        AVCodecContext *vid_dec_ctx, *audio_dec_ctx;

        int video_stream_idx, audio_stream_idx;
        AVStream *video_stream, *audio_stream;

        FILE *video_dst_file, *audio_dst_file;

        int width, height;
        enum AVPixelFormat pix_fmt;

        uint8_t* video_dst_data[4];
        int video_dst_linesize[4];
        int video_dst_bufsize;

        AVFrame *frame;

        AVPacket pkt;
        int video_frame_count, audio_frame_count;
    public:

        VideoReader(const char* _src_file, const char* _video_output_file, const char* _audio_output_file);
        ~VideoReader();

        void SaveFrame(AVFrame *pFrame);
        void ShowFrame(AVFrame *pFrame);

        int DecodePacket(AVCodecContext* dec, const AVPacket* pkt);
        void Frame2File();
        int OpenCodecContext(int &stream_index,
            AVCodecContext** dec_ctx, enum AVMediaType type);

        int demux();


};

#endif