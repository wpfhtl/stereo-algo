#include <iostream>
#include <cstdio>
#include <iomanip>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <ctime>
#include "FiveRegionStereo.h"
#include "st_util.h"

using std::cout;
using std::endl;
using std::cerr;

int main(int argc, char **argv) {

    cv::Mat img_left_g;
    cv::cvtColor(cv::imread(argv[1]), img_left_g, CV_BGR2GRAY);
    cv::Mat img_right_g;
    cv::cvtColor(cv::imread(argv[2]), img_right_g, CV_BGR2GRAY);
    int max_disp = atoi(argv[3]);
    int region_size = atoi(argv[4]);

    clock_t begin = clock();
    FiveRegionStereo frs = FiveRegionStereo(0, max_disp, region_size, region_size, 25, 6, 0.0);
    cv::Mat disparity = frs.compute_disparity(img_left_g, img_right_g);
    
    // func(img_left_g, img_right_g, 0, max_disp, region_size, region_size, 25, 6, 0.2);
    cerr << double(clock()-begin) / CLOCKS_PER_SEC << endl;
    su::print_mat_float(disparity);
    return 0;
}