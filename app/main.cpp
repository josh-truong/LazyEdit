#include <iostream>
#include "VideoReader.h"


using std::cout;
using std::endl;



int main(int argc, const char** argv)
{
    if(argc != 4) {
        cout << "Source path not given" << endl;
        cout << "Video output path not given" << endl;
        cout << "Audio output path not given" << endl;
        return -1;
    }

    const char* SRC_FILE = argv[1];
    const char* VIDEO_OUTPUT_PATH = argv[2];
    const char* AUDIO_OUTPUT_PATH = argv[3];

    VideoReader demux(SRC_FILE, VIDEO_OUTPUT_PATH, AUDIO_OUTPUT_PATH);
    demux.demux();
    return 0;
}