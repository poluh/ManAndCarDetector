//
// Created by sergey on 10.09.18.
//

#include <thread>
#include "PartDetector.h"


PartDetector::PartDetector(std::string cfg_filename, std::string weight_filename, int gpu_id = 0, int obj_size = 20)
: detector(cfg_filename, weight_filename, gpu_id) {
    net_w = detector.get_net_width();
    net_h = detector.get_net_height();
    this->obj_size = obj_size;
};

std::vector<bbox_t> PartDetector::detect(cv::Mat img, float thresh = 0.2) {

    std::vector<bbox_t> res;
    cv::Size img_size = img.size();
//        net_w = img_size.width / 2;
//        net_h = img_size.height / 2;
    int w = net_w;
    int h = net_h;
    for (int x = 0; x < img_size.width; x += w) {
        w = x + w + obj_size > img_size.width ? w - (x + w - img_size.width) : net_w + obj_size;
        for (int y = 0; y < img_size.height; y += h) {
            //thread_process(img, thresh, x, y, h, w, &res);
            //auto img_size = img.size();
            h = y + h + obj_size > img_size.height ? h - (y + h - img_size.height) : net_h + obj_size;
            int right_border = net_w + obj_size - w;
            int bottom_border = net_h + obj_size - h;

            cv::RNG rng(12345);
            cv::Scalar value = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255) );

            cv::Mat buff_img(net_w, net_h, 0);
            cv::Mat cropped_img = img(cv::Rect(x, y, w, h));

            cv::copyMakeBorder(cropped_img, buff_img, 0, bottom_border, 0, right_border, cv::BORDER_CONSTANT, value);

            std::vector<bbox_t> buff_vector = detector.detect(buff_img, thresh);
            printf("%lu detect",  buff_vector.size());
            for (auto& bbox : buff_vector) {
                bbox.x = bbox.x + x;
                bbox.y = bbox.y + y;
                res.push_back(bbox);
            }

            buff_vector.clear();
            cropped_img.release();
            buff_img.release();
        }
    }


    return res;
}

void PartDetector::thread_process(cv::Mat img, float thresh, int x, int y, int h, int w, std::vector<bbox_t> *res) {
    auto img_size = img.size();
    h = y + h + obj_size > img_size.height ? h - (y + h - img_size.height) : net_h + obj_size;
    int right_border = net_w + obj_size - w;
    int bottom_border = net_h + obj_size - h;

    cv::RNG rng(12345);
    cv::Scalar value = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255) );

    cv::Mat buff_img(net_w, net_h, 0);
    cv::Mat cropped_img = img(cv::Rect(x, y, w, h));

    cv::copyMakeBorder(cropped_img, buff_img, 0, bottom_border, 0, right_border, cv::BORDER_CONSTANT, value);

    std::vector<bbox_t> buff_vector = detector.detect(img, thresh);
    for (auto& bbox : buff_vector) {
        bbox.x = bbox.x + x;
        bbox.y = bbox.y + y;
        res->push_back(bbox);
    }
    cropped_img.release();
    buff_img.release();
}