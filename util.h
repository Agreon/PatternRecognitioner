//
// Created by root on 24.06.16.
//

#ifndef OPENCVTEST_UTIL_H
#define OPENCVTEST_UTIL_H

#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>

#include <opencv2/opencv.hpp>

using namespace std;

extern const int COLOR_TYPE;

/*
 *
 */
extern int getMin(vector<uchar> input);
/*
 *
 */
extern int getMax(vector<uchar> input);

extern vector<uchar> get3x3(cv::Mat input, int x, int y, int constraint);

/*
 * Deletes the pixel if any of its neightbours is White
 */
extern   cv::Mat erode(cv::Mat input);

    /*
     * Adds Pixel if any of its neightbours is black
     */
    extern   cv::Mat dilate(cv::Mat input);

/*
 * Erode + Dilate
 */
extern  cv::Mat open(cv::Mat input);

/*
 * Dilate + Erode
 */
extern cv::Mat close(cv::Mat input);

extern cv::Mat binarise(cv::Mat input, int val);

extern cv::Mat laplace(cv::Mat input, bool offset);

extern cv::Mat invert(cv:: Mat input);

extern vector<string> &split(const string &s, char delim, vector<string> &elems);

extern  vector<string> split(const string &s, char delim);

#endif //OPENCVTEST_UTIL_H
