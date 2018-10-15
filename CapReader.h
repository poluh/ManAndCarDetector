//
// Created by sergey on 10.09.18.
//

#ifndef WEBPRJ_CAPREADER_H
#define WEBPRJ_CAPREADER_H

#include <mutex>
#include <thread>
#include "opencv2/opencv.hpp"
#include <time.h>

class CapReader {

public:
    CapReader(cv::VideoCapture cap);
    ~CapReader();
    cv::Mat get_frame();
    bool get_frame_ready();

private:
    cv::Mat frame;
    cv::VideoCapture cap;
    std::mutex frame_mutex;
    bool frame_ready = false;
    void cap_reader();
    std::thread* thread;

};


#endif //WEBPRJ_CAPREADER_H
