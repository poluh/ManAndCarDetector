//
// Created by sergey on 01.10.18.
//

#include <thread>
#include "MainDetector.h"

MainDetector::MainDetector(std::string car_cfg, std::string car_weight, std::string man_cfg, std::string man_weight,
                           int gpu_id, int obj_size, float man_thresh, float car_thresh)
: detector(car_cfg, car_weight, gpu_id), partDetector(man_cfg, man_weight){
    this->car_thresh = car_thresh;
    this->man_thresh = man_thresh;
}

std::vector<bbox_t> MainDetector::detect(cv::Mat image) {
    std::vector<bbox_t> bbox;
    std::vector<bbox_t> part_bbox;
    bbox = detector.detect(image, car_thresh);
    part_bbox = partDetector.detect(image, man_thresh);

    for (auto& item : part_bbox) {
        bbox.push_back(item);
    }
    return bbox;
}