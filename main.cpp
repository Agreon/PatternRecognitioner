#include "Recognizer.h"


/*
 * TODO: Cmd line arguments
 *  + recognize a file
 *  + load patterns
 *  + visualize patterns
 *  TODO: What if binarise errors?
 *  => Done, but only one-line texts available
 *
 */
int main(int argc, char** argv )
{
    Recognizer *recognizer = new Recognizer();

    recognizer->loadPatterns("input.pt");

    recognizer->learnWithFile("loadPatterns.cfg");

    bool save = recognizer->savePatterns("output.pt");

    //m_PatternRecognitioner.visualisePatterns();
    recognizer->visualisePatterns();

    cout << recognizer->recognize("learn/test4.png") << endl;

    waitKey(0);

    return 0;

   /* std::string img_in = "written.png";

    if ( argc == 2 )
    {
        img_in = argv[1];
    }
    namedWindow("Input Image", WINDOW_AUTOSIZE
    imshow("Closed Image", closed);

    waitKey(0);

    return 0;*/
}