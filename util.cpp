//
// Created by root on 24.06.16.
//
#include "util.h"

const int COLOR_TYPE = CV_8UC1;

/*
 *
 */
int getMin(vector<uchar> input){
    int min = input[0];

    for(int i = 1; i < input.size(); i++){
        if(input[i] < min)
            min = input[i];
    }

    return min;
}

/*
 *
 */
int getMax(vector<uchar> input){
    int max = input[0];

    for(int i = 1; i < input.size(); i++){
        if(input[i] > max)
            max = input[i];
    }

    return max;
}

vector<uchar> get3x3(cv::Mat input, int x, int y, int constraint = -1){
    vector<uchar> output;

    for(int i = -1; i < 2; i++){
        for(int j = -1; j < 2; j++){
            if(i == 0 && j == 0) continue;
            if(y+i < 0 || x+j < 0) continue;
            if(y+i >= input.rows || x+j >= input.cols) continue;

            if(constraint != -1 && input.at<uchar>(cv::Point(x+j,y+i)) == constraint)
                continue;

            output.push_back(input.at<uchar>(cv::Point(x+j,y+i)));
        }
    }

    return output;
}

/*
 * Deletes the pixel if any of its neightbours is White (Reversed in this case)
 */
cv::Mat erode(cv::Mat input){
    cv::Mat output(input.size(),COLOR_TYPE);

    output = cv::Scalar(255);

    for(int y = 0; y < input.rows; y++) {
        for (int x = 0; x < input.cols; x++) {
            uchar inP = input.at<uchar>(cv::Point(x,y));
            if(inP == 255){
                continue;
            }
            vector<uchar> x3 = get3x3(input,x,y);
            int maxVal = getMax(x3);

            if(maxVal != 255){
                output.at<uchar>(y,x) = inP;
            }
        }
    }

    return output;
}

/*
 * Adds Pixel if any of its neightbours is black
 * TODO: Check performance
 * TODO: Is border ignored or not?
 */
cv::Mat dilate(cv::Mat input){
    cv::Mat output(input.size(),COLOR_TYPE);

    output = cv::Scalar(255);

    for(int y = 0; y < input.rows; y++) {
        for (int x = 0; x < input.cols; x++) {
            uchar inP = input.at<uchar>(cv::Point(x,y));
            if(inP == 0) {
                output.at<uchar>(cv::Point(x,y)) = 0;
                continue;
            }

            vector<uchar> x3 = get3x3(input,x,y);
            int minVal = getMin(x3);
            if(minVal == 0){
                output.at<uchar>(cv::Point(x,y)) = 0;
            }else{
                output.at<uchar>(cv::Point(x,y)) = inP;
            }
        }
    }

    return output;
}

/*
 * Erode + Dilate
 */
cv::Mat open(cv::Mat input){
    cv::Mat output(input.size(),COLOR_TYPE);

    output = erode(input);
    output = dilate(output);

    return output;
}

/*
 * Dilate + Erode
 */
cv::Mat close(cv::Mat input){
    cv::Mat output(input.size(),COLOR_TYPE);

    output = dilate(input);
    output = erode(output);

    return output;
}

cv::Mat binarise(cv::Mat input, int val){
    cv::Mat output(input.size(),COLOR_TYPE);

    for(int y = 0; y < input.rows; y++){
        for(int x = 0; x < input.cols; x++){
            if(input.at<uchar>(cv::Point(x,y)) < val){
                output.at<uchar>(cv::Point(x,y)) = 0;
            }else{
                output.at<uchar>(cv::Point(x,y)) = 255;
            }
        }
    }

    return output;
}

cv::Mat laplace(cv::Mat input, bool offset = true){
    cv::Mat output(input.size(),COLOR_TYPE);

    int newP;

    for(int y = 0; y < input.rows; y++){
        for(int x = 0; x < input.cols; x++){

            //MAX: 4*255
            //MIN: -4*255
            if(offset)
                newP = 4*255;
            else
                newP = 0;

            newP += -4*input.at<uchar>(cv::Point(x,y));
            newP += input.at<uchar>(cv::Point(x-1,y));
            newP += input.at<uchar>(cv::Point(x,y-1));
            newP += input.at<uchar>(cv::Point(x+1,y));
            newP += input.at<uchar>(cv::Point(x,y+1));

            newP /= 8;

            output.at<uchar>(cv::Point(x,y)) = newP;
        }
    }

    return output;
}


cv::Mat invert(cv:: Mat input){
    cv::Mat output(input.size(),COLOR_TYPE);

    for(int y = 0; y < input.rows; y++){
        for(int x = 0; x < input.cols; x++) {
            output.at<uchar>(cv::Point(x,y)) = 255 - input.at<uchar>(cv::Point(x,y));
        }
    }

    return output;
}

vector<string> &split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}



