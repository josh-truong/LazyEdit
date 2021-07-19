#include "LazyEdit.h"

int main (int argc, char **argv)
{
    if (argc != 4) {
        fprintf(stderr, "usage: %s  input_file video_output_file audio_output_file\n"
                "API example program to show how to read frames from an input file.\n"
                "This program reads frames from a file, decodes them, and writes decoded\n"
                "video frames to a rawvideo file named video_output_file, and decoded\n"
                "audio frames to a rawaudio file named audio_output_file.\n",
                argv[0]);
        exit(1);
    }

    LazyEdit edit;
    edit.src_filename = argv[1];
    edit.video_dst_filename = argv[2];
    edit.audio_dst_filename = argv[3];
    edit.decode();

    return 0;
}