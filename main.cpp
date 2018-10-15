#include <iostream>
#define OPENCV


#include "PartDetector.h"
#include "opencv2/opencv.hpp"
#include "CapReader.h"
#include <string>
#include "MainDetector.h"

int find_argument(char *argv[], int argc, char *arg) {
    for (int i = 0; i < argc; ++i) if (strcmp(argv[i], arg) == 0) return i;
    return -1;
}

void draw(cv::Mat &img, std::vector<bbox_t> bboxes, cv::Scalar color = cv::Scalar(0, 255, 0)) {
    if (bboxes.size() != 0) {
        for (auto& bbox : bboxes) {
            cv::Rect rect(bbox.x, bbox.y, bbox.w, bbox.h);
            cv::rectangle(img, rect, color, 4);
        }
    }
}

int main(int argc, char *argv[]) {

    //MainDetector md(argv[1], argv[2], argv[3], argv[4], 0, 20, 0.5, 0.7);

    Detector car_detector(argv[1], argv[2]);
    PartDetector man_detector(argv[3], argv[4]);

    bool use_webcam = find_argument(argv, argc, (char *) "-w") != -1;
    bool skiping_frame = find_argument(argv, argc, (char *) "-s") != -1;
    bool set_thresh = find_argument(argv, argc, (char *) "-t") != -1;
    int  skip_frame = 0;
    float thresh = 0.2;
    int delay = skip_frame;

    if (skiping_frame) skip_frame = std::stoi(std::string(argv[find_argument(argv, argc, (char *) "-s") + 1]));
    if (set_thresh) thresh = std::stof(std::string(argv[find_argument(argv, argc, (char *) "-t") + 1]));

    printf("Use skiping frames (%d frames)\nUse threshold %f", skip_frame, thresh);
    std::cout << argv[find_argument(argv, argc, (char *) "-s") + 1] << std::endl;

    cv::VideoCapture cap;

    if (use_webcam) {
        cap = cv::VideoCapture(0);
    } else {
        cap = cv::VideoCapture(argv[5]);
    }

    CapReader capReader(cap);
    while(1) {
        if (!cap.isOpened()) {
            std::cout << "Error in opening" << std::endl;
            break;
        }

        cv::Mat frame;
        while (!capReader.get_frame_ready()) {}
        //printf("%s", "FRAME READY");

        frame = capReader.get_frame();
        //cap >> frame;

        if  (frame.empty()) {
            std::cout << "Empty frame" << std::endl;
            break;
        }

        if (delay <= 0) {
            delay = skip_frame;

            draw(frame, car_detector.detect(frame, 0.5));
            draw(frame, man_detector.detect(frame, 0.65), cv::Scalar(0, 0, 255));
            cv::Size size = frame.size();
            cv::namedWindow("Demo", cv::WINDOW_FREERATIO);
            cv::resizeWindow("Demo", size.width, size.height);
            cv::imshow("Demo", frame);
        }
        --delay;

        frame.release();

        // Pause
        if (cv::waitKey(30) == 32) {
            while (1) {
                if (cv::waitKey(30) == 32) break;
            }
        }

        // Exit
        if (cv::waitKey(30) == 27) {
            capReader.~CapReader();
            break;
        }
    }

    return 0;
}