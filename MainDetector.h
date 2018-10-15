//
// Created by sergey on 01.10.18.
//

#ifndef WEBPRJ_MAINDETECTOR_H
#define WEBPRJ_MAINDETECTOR_H

#define OPENCV
#include <yolo_v2_class.hpp>
#include "PartDetector.h"
#include <thread>
#include <mutex>

class MainDetector {

private:
    float car_thresh = 0.2;
    float man_thresh = 0.5;
    Detector detector;
    PartDetector partDetector;

public:
    MainDetector(std::string car_cfg, std::string car_weight,
                 std::string man_cfg, std::string man_weight,
                 int gpu_id = 0, int obj_size = 20,
                 float man_thresh = 0.2, float car_thresh = 0.5);
    std::vector<bbox_t> detect(cv::Mat image);
};


#endif //WEBPRJ_MAINDETECTOR_H
