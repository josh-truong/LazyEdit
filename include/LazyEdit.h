#ifndef LAZYEDIT_H
#define LAZYEDIT_H

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#define __STDC_CONSTANT_MACROS
extern "C"
{
    #include <libavutil/imgutils.h>
    #include <libavutil/samplefmt.h>
    #include <libavutil/timestamp.h>
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
}

#include "LazyEdit.h"
#include "Shader.h"

#include <cstdio>

class LazyEdit
{
    private:
        AVFormatContext *fmt_ctx = NULL;
        AVCodecContext *video_dec_ctx = NULL, *audio_dec_ctx;
        int width, height;
        enum AVPixelFormat pix_fmt;
        AVStream *video_stream = NULL, *audio_stream = NULL;
        FILE *video_dst_file = NULL;
        FILE *audio_dst_file = NULL;

        uint8_t *video_dst_data[4] = {NULL};
        int      video_dst_linesize[4];
        int video_dst_bufsize;

        int video_stream_idx = -1, audio_stream_idx = -1;
        AVFrame *frame = NULL;
        AVPacket *pkt = NULL;
        int video_frame_count = 0;
        int audio_frame_count = 0;

        SwsContext *sws_cxt;

        GLFWwindow *window;
        Shader shader;
        unsigned int VBO, VAO, EBO;
        unsigned int texture;

    public:
        const char *src_filename = NULL;
        const char *video_dst_filename = NULL;
        const char *audio_dst_filename = NULL;

        LazyEdit();
        ~LazyEdit();

        int output_video_frame(AVFrame *frame);
        int output_audio_frame(AVFrame *frame);
        int sws_scale_video_frame(AVFrame *frame, uint8_t *frame_buffer);
        void render_frame(uint8_t *frame_data);
        int decode_packet(AVCodecContext *dec, const AVPacket *pkt);
        int open_codec_context(int *stream_idx, AVCodecContext **dec_ctx, AVFormatContext *fmt_ctx, enum AVMediaType type);
        int get_format_from_sample_fmt(const char **fmt, enum AVSampleFormat sample_fmt);
        int init_renderer();
        int decode();
};

#endif