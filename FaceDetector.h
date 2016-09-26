//
// Created by Warren Green on 9/24/16.
//

#ifndef FBFACERECOGNITION_FACEDETECTOR_H
#define FBFACERECOGNITION_FACEDETECTOR_H

#include "opencv2/objdetect.hpp"
#include "opencv2/video.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <boost/algorithm/string.hpp>

#include <dirent.h>
#include <map>
#include <fstream>
#include <math.h>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;
using namespace boost::algorithm;

class FaceDetector {
public:
    void loadCSV(string indexFile);
    Mat detect(Mat img);
    int detect();
    void detect(string inputDirectory, string outputDirectory);

};


#endif //FBFACERECOGNITION_FACEDETECTOR_H
