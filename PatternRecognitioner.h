//
// Created by root on 24.06.16.
//
#ifndef OPENCVTEST_PATTERNRECOGNITIONER_H
#define OPENCVTEST_PATTERNRECOGNITIONER_H

#include <opencv2/opencv.hpp>

#include "helper.h"
#include "util.h"

using namespace std;
using namespace cv;

class PatternRecognitioner {
public:
    PatternRecognitioner();

    //Learning
    void addToBrain(string _id, Mat _img, int minObjSize = 5);

    //Function
    string recognize(Mat _img, int minObjSize = 5);
    vector<PR::Object> segmentImage(Mat _img, int minObjSize = 5);

    vector<PR::Pattern>& patterns();
private:
    string checkObject(PR::Object object);
    int comparePattern(cv::Mat objImg, PR::Pattern pattern);

    vector<PR::Pattern> m_Patterns;
};


#endif //OPENCVTEST_PATTERNRECOGNITIONER_H
