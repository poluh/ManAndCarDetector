//
// Created by sergey on 10.09.18.
//

#include "CapReader.h"

CapReader::CapReader(cv::VideoCapture cap) {
    this->cap = cap;
    frame_ready = false;
//    thread = new std::thread(&CapReader::cap_reader, this);

    thread = new std::thread([this]() {
        this->cap_reader();
    });
}
CapReader::~CapReader()
{
    delete thread;
    thread = nullptr;
}


cv::Mat CapReader::get_frame() {
    frame_mutex.lock();
    if (frame_ready) {
        frame_ready = false;
        cv::Mat result = frame.clone();
        frame_mutex.unlock();
        return result;
    }
    frame_mutex.unlock();
    return cv::Mat();
}

void CapReader::cap_reader() {
    struct timespec tw = {0,1};
    struct timespec tr;
    while (1) {
        nanosleep(&tw, &tr);
        frame_mutex.lock();
        if (!frame_ready) {
            cap >> frame;
            frame_ready = true;
        }
        frame_mutex.unlock();
    }
}

bool CapReader::get_frame_ready() {
    frame_mutex.lock();
    bool buff = frame_ready;
    frame_mutex.unlock();
    return buff;
}






