//
// Created by daniel on 24.06.16.
//

#include "PatternRecognitioner.h"


PatternRecognitioner::PatternRecognitioner(){
}


/*
 * Creates a pattern out of a image or adds it to existing
 */
void PatternRecognitioner::addToBrain(string _id, Mat _img, int minObjSize) {

    PR::Pattern *pattern = NULL;

    // Check if pattern is already in database
    for(int i = 0; i < m_Patterns.size(); i++){
        if(m_Patterns[i].getID() == _id){
            pattern = &m_Patterns[i];
            break;
        }
    }

    // If not, create new pattern
    if(pattern == NULL){
        m_Patterns.push_back(PR::Pattern(_id));
        pattern = &m_Patterns.back();
    }

    // Create Object out of image
    vector<PR::Object> objects = segmentImage(_img, minObjSize);

    // Scale to standart
    Mat scaled;
    cv::resize(objects[0].img(),scaled,cv::Size(100,100),0,0,INTER_LINEAR);

    // Add to pointvalue
    for(int i = 0; i < scaled.cols; i++){
        for(int j = 0; j < scaled.rows; j++){
            if(scaled.at<uchar>(cv::Point(j,i)) != 255){
                pattern->getPointValues()[i][j]++;
            }
        }
    }
}


string PatternRecognitioner::recognize(Mat _img, int minObjSize) {

    vector<PR::Object> objects = segmentImage(_img, minObjSize);

    string outString = "";

    //Check with existing patterns
    for(int i = 0; i < objects.size(); i++){
        outString += checkObject(objects[i]);
        imwrite("Object"+to_string(i)+".png",objects[i].img());
    }

    return outString;
}

/*
TODO: takes some time
 */
void checkNeighbours(Mat &_img, vector<vector<bool>> &objectMatrix, vector<PR::Point> &objectPoints,
vector<PR::Point> &toCheck, int it){

    for(int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            if (i == 0 && j == 0)
                continue;

            // Exclude outer borders
            if (toCheck[it].y + i < 0 || toCheck[it].x + j < 0)
                continue;
            if (toCheck[it].y + i >= _img.rows || toCheck[it].x + j >= _img.cols)
                continue;

            uchar inP = _img.at<uchar>(cv::Point(toCheck[it].x + j, toCheck[it].y + i));
            if (inP == 255)
                continue;

            if (objectMatrix[toCheck[it].x + j][toCheck[it].y + i])
                continue;

            objectMatrix[toCheck[it].x + j][toCheck[it].y + i] = true;

            objectPoints.push_back(PR::Point(toCheck[it].x+j,toCheck[it].y+i));

            toCheck.push_back(PR::Point(toCheck[it].x+j, toCheck[it].y+i));
        }
    }
}

/*
 * TODO: Maybe move toCheck in own function
 */
vector<PR::Object> PatternRecognitioner::segmentImage(Mat _img, int minObjSize) {

    vector<PR::Object> segments;

    // Get grey values
    cvtColor(_img, _img, CV_BGR2GRAY);

    // Binarise
    _img = binarise(_img,128);

    // Open to remove distortions
    _img = open(_img);


    //Prepare object matrix
    vector<vector<bool>> objectMatrix;
    for(int i = 0; i < _img.cols; i++){
        vector<bool> v;
        objectMatrix.push_back(v);
        for(int j = 0; j < _img.rows; j++){
            objectMatrix[i].push_back(false);
        }
    }

    vector<PR::Point> objectPoints;
    vector<PR::Point> toCheck;

    for(int y = 0; y < _img.rows; y++) {
        for (int x = 0; x < _img.cols; x++) {
            uchar inP = _img.at<uchar>(cv::Point(x, y));

            if(inP == 255)
                continue;

            if(objectMatrix[x][y])
                continue;

            objectMatrix[x][y] = true;
            objectPoints.clear();
            objectPoints.push_back(PR::Point(x,y));
            toCheck.clear();
            toCheck.push_back(PR::Point(x,y));

            for(int a = 0; a < toCheck.size(); a++){

                checkNeighbours(_img,objectMatrix,objectPoints,toCheck,a);

                /*for(int i = -1; i < 2; i++) {
                    for (int j = -1; j < 2; j++) {
                        if (i == 0 && j == 0)
                            continue;
                        if (toCheck[a].y + i < 0 || toCheck[a].x + j < 0)
                            continue;
                        if (toCheck[a].y + i >= _img.rows || toCheck[a].x + j >= _img.cols)
                            continue;

                        uchar inP = _img.at<uchar>(cv::Point(toCheck[a].x + j, toCheck[a].y + i));
                        if (inP == 255)
                            continue;

                        if (objectMatrix[toCheck[a].x + j][toCheck[a].y + i])
                            continue;

                        objectMatrix[toCheck[a].x + j][toCheck[a].y + i] = true;

                        objectPoints.push_back(PR::Point(toCheck[a].x+j,toCheck[a].y+i));

                        toCheck.push_back(PR::Point(toCheck[a].x+j, toCheck[a].y+i));
                    }
                }*/
            }

            if(objectPoints.size() < 1)
                continue;

            PR::Object obj = PR::Object(objectPoints);
            cout << "Added Object " << obj.getWidth() << "x" << obj.getHeight() << endl;
            segments.push_back(obj);

        }
    }

    vector<PR::Object> objects;

    // Filter out small objects
    for(int i = 0; i < segments.size(); i++){
        if(segments[i].getWidth() < _img.rows/(100/minObjSize) && segments[i].getHeight() < _img.cols/(100/minObjSize)){
            continue;
        }
        objects.push_back(segments[i]);
    }

    cout << "Found " << objects.size() << " objects!" << endl;

    // Order Objects by Y-position
    bool swapped = true;
    while(swapped){
        swapped = false;
        for(int j = 0; j < objects.size()-1; j++){
            PR::Object current = objects[j];
            if(objects[j].leftStart() > objects[j+1].leftStart()){
                objects[j] = objects[j+1];
                objects[j+1] = current;
                swapped = true;
            }
        }
    }

    segments.clear();

    int i;
    int del = 0;
    //Combine splitted elements of letters //Caution: So only one-line texts work //TODO: you can do that better
    for(i = 0; i < objects.size() - 1; i++){

        if(objects[i+1].leftStart() < objects[i].leftStart() + objects[i].getWidth() ){
            for(int z = 0; z < objects[i+1].points().size(); z++){
                objects[i].points().push_back(objects[i+1].points()[z]);
            }
            del = i+1;
            segments.push_back(objects[i]);
            i++;
            continue;
        }
        segments.push_back(objects[i]);
    }
    if(del != objects.size() - 1 || objects.size() == 1)
        segments.push_back(objects.back());

    cout << "Combined to " << segments.size() << " Objects!" << endl;

    return segments;
}

/*
 * Scales a Object to standart size and compares it to all patterns in database.
 */
string PatternRecognitioner::checkObject(PR::Object _object) {
    string output = "";
    int max = 0, value = 0;


    Mat objImg;
    cv::resize(_object.img(),objImg,cv::Size(100,100),0,0,INTER_LINEAR);

    for(int i = 0; i < m_Patterns.size(); i++){
        value = comparePattern(objImg,m_Patterns[i]);

        if(value > max){
            max = value;
            output = m_Patterns[i].getID();
        }
    }

    return output;
}

/*
 * Compares an Object with a pattern by its pixel values.
 */
int PatternRecognitioner::comparePattern(cv::Mat _objectImg, PR::Pattern pattern) {

    int value = 0;

    vector<vector<int>> patternPoints = pattern.getPointValues();
    int maxValue = pattern.getMaxValue();

    for(int i = 0; i < _objectImg.cols; i++){
        for(int j = 0; j < _objectImg.rows; j++){
            if(_objectImg.at<uchar>(cv::Point(j,i)) != 255 && patternPoints[i][j] != 0){
                // Add relative value so comparision is fair
                value += ((double)patternPoints[i][j] / (double)maxValue) * 255;
            }
        }
    }

    return value;
}

vector<PR::Pattern> &PatternRecognitioner::patterns() {
    return m_Patterns;
}
