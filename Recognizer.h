//
// Created by root on 10.07.16.
//

#ifndef OPENCVTEST_RECOGNIZER_H
#define OPENCVTEST_RECOGNIZER_H

#include "PatternRecognitioner.h"

class Recognizer {
public:
    Recognizer();

    // File-op
    bool loadPatterns(string _file);
    bool savePatterns(string _file);

    bool visualisePatterns();

    bool learnWithFile(string _file);

    string recognize(string _file, int minObjSize = 5);
private:
    vector<string> loadFileContent(string _file);

    PatternRecognitioner m_PatternRecognitioner;
};


#endif //OPENCVTEST_RECOGNIZER_H
