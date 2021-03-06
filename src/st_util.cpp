#include "st_util.h"

#include <string>
#include <sstream>
#include <stdexcept>
#include <cstdio>
#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/contrib/contrib.hpp>

void su::require(bool condition, const std::string &msg) {
    if (!condition) {
        throw std::invalid_argument(msg);
    }
}

template <typename T>
std::string su::str(T t) {
    std::stringstream ss;
    ss << t;
    return ss.str();
}

// Explicit instantiation for su::str function
template std::string su::str<int>(int t);

template <typename T>
void su::print_mat(const cv::Mat &m) {
    for (int i = 0; i < m.rows; i++) {
        for (int j = 0; j < m.cols; j++) {
            std::cout << m.at<T>(i,j) << ' ';
        }
        std::cout << std::endl;
    }
}

// Explicit instantiation for su::print_mat
template void su::print_mat<float>(const cv::Mat &m);
template void su::print_mat<short>(const cv::Mat &m);
template void su::print_mat<int>(const cv::Mat &m);

void su::convert_to_disparity_visualize(const cv::Mat &source, cv::Mat &dest,
                                        double minv, double maxv, bool color) {
    dest = (source - minv) * 255.0 / (maxv - minv);
    dest.convertTo(dest, CV_8UC1);
    if (color) {
        cv::applyColorMap(dest, dest, cv::COLORMAP_JET);
    }
}

void su::convert_to_disparity_visualize(const cv::Mat &source, cv::Mat &dest, bool color) {
    double minv, maxv;
    cv::minMaxLoc(source, &minv, &maxv);
    su::convert_to_disparity_visualize(source, dest, minv, maxv, color);
}

void su::draw_horiz_lines(cv::Mat &img, int step, int thickness, const cv::Scalar &color) {
    for (int y = step; y < img.rows; y += step) {
        cv::line(img, cv::Point2f(0, y), cv::Point2f(img.cols, y), color, thickness);
    }
}

void su::print_mat_float(const cv::Mat &m) {
    for (int i = 0; i < m.rows; i++) {
        for (int j = 0; j < m.cols; j++) {
            printf("%.1f ", m.at<float>(i, j));
        }
        printf("\n");
    }
}

void su::count_disparities(const cv::Mat &disparity, std::vector<int> &counts, int min_d, int max_d) {
    if (counts.size() != max_d - min_d) {
        counts = std::vector<int>(max_d - min_d, 0);
    }
    for (int row = 0; row < disparity.rows; row++) {
        for (int col = 0; col < disparity.cols; col++) {
            int d = disparity.at<int>(row, col);
            counts[d-min_d]++;
        }
    }
}

void su::print_counts(const std::vector<int> &counts, int min_d, int max_d) {
    for (int i = min_d; i < max_d; i++) {
        printf("%4d %5d\n", i, counts[i-min_d]);
    }
}

uint64_t su::rdtsc() {
    unsigned int lo,hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}
