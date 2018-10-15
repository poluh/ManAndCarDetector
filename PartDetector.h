//
// Created by sergey on 10.09.18.
//

#ifndef WEBPRJ_PARTDETECTOR_H
#define WEBPRJ_PARTDETECTOR_H

#define OPENCV
#include <opencv2/opencv.hpp>
#include "yolo_v2_class.hpp"


class PartDetector {
private:
    Detector detector;
    int net_w;
    int net_h;
    int obj_size;

public:
    PartDetector(std::string cfg_filename, std::string weight_filename, int gpu_id = 0, int obj_size = 20);
    std::vector<bbox_t> detect(cv::Mat image, float thresh);

private:
    void thread_process(cv::Mat img, float thresh, int x, int y, int h, int w, std::vector<bbox_t> *res);
};



#endif //WEBPRJ_PARTDETECTOR_H
