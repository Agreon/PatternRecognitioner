//
// Created by root on 10.07.16.
//

#include "Recognizer.h"

Recognizer::Recognizer() {
    m_PatternRecognitioner = PatternRecognitioner();
}


/*
 * FileStructure:
 * + ID:p.x,p.y,value;p.x,p.y,value
  */
bool Recognizer::loadPatterns(string _file){


    vector<string> lines = loadFileContent(_file);

    if(lines.size() == 0){
        cout << "Could not load pattern file" << endl;
        return false;
    }

    vector<string> elems, points;
    PR::Pattern pattern;

    for(int i = 0; i < lines.size(); i++){

        elems = split(lines[i],':');

        pattern = PR::Pattern(elems[0]);

        elems = split(elems[1],';');

        for(int j = 0; j < elems.size(); j++){
            points = split(elems[j],',');
            pattern.getPointValues()[atoi(points[0].c_str())][atoi(points[1].c_str())] = atoi(points[2].c_str());
        }

        m_PatternRecognitioner.patterns().push_back(pattern);
    }

    cout << "Loaded Pattern file!" << endl;

    return true;
}

/*
 * Saves Patterns to a file
 */
bool Recognizer::savePatterns(string _file) {

    ofstream out(_file);

    if(!out.is_open()){
        return false;
    }

    vector<PR::Pattern> &patterns = m_PatternRecognitioner.patterns();

    for(int i = 0; i < patterns.size(); i++){
        out << patterns[i].getID() << ":";
        for(int x = 0; x < patterns[i].getPointValues().size(); x++){
            for(int y = 0; y < patterns[i].getPointValues()[x].size(); y++){
                if(!(x == 0 && y == 0)){
                    out << ";";
                }
                out << x << "," << y << "," << patterns[i].getPointValues()[x][y];
            }
        }
        out << endl;
    }

    out.close();

    return true;
}

/*
 * Draws every pattern in library with values
 */
bool Recognizer::visualisePatterns() {

    int max;
    vector<vector<int>> values;
    Mat out;

    vector<PR::Pattern> &patterns = m_PatternRecognitioner.patterns();

    for(int i = 0; i < patterns.size(); i++){
        max = patterns[i].getMaxValue();
        values = patterns[i].getPointValues();

        out = cv::Mat(cv::Size(100,100),CV_8UC1);
        out = cv::Scalar(255);

        //Draw grey value
        for(int x = 0; x < values.size(); x++){
            for(int y = 0;  y < values[x].size(); y++){
                out.at<uchar>(cv::Point(y,x)) = (uchar)(255 - ((double)values[x][y] / (double)max) * 255);
            }
        }
        imwrite(patterns[i].getID()+"_vis.png",out);
    }
}

/*
 * Loads a .cfg file with Key-Image data to learn
 * Structure: Key;fileName
 */
bool Recognizer::learnWithFile(string _file) {

    vector<string> lines = loadFileContent(_file);

    if(lines.size() == 0){
        cerr << "Could not load cfg file" << endl;
        return false;
    }

    vector<string> images;
    Mat img;

    for(int i = 0; i < lines.size(); i++){
        images = split(lines[i],';');

        img = imread(images[1], 1);

        if(!img.data){
            cerr << "could not read img " << images[1]  << " to learn pattern " << images[0] << endl;
            continue;
        }

        cout << "Adding img " << images[1]  << " to pattern " << images[0] << endl;

        m_PatternRecognitioner.addToBrain(images[0], img);
    }

    cout << "Loaded cfg file" << endl;

    return true;
}

vector<string> Recognizer::loadFileContent(string _file) {
    vector<string> lines;
    string line;

    ifstream in(_file.c_str());

    if(!in.is_open()){
        cerr << "Could not open File " << _file << endl;
        return lines;
    }

    while(getline(in,line)){
        lines.push_back(line);
    }

    return lines;
}

string Recognizer::recognize(string _file, int minObjSize) {

    Mat img = imread(_file, 1);

    if(!img.data){
        cerr << "could not read img " << _file << endl;
        return "";
    }

    return m_PatternRecognitioner.recognize(img,minObjSize);
}
