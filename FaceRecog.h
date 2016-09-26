//
// Created by Warren Green on 9/24/16.
//

#ifndef FBFACERECOGNITION_FACERECOGNIZER_H
#define FBFACERECOGNITION_FACERECOGNIZER_H

#include <iostream>
#include <vector>
#include <fstream>
#include <dirent.h>
#include <map>
#include <iostream>
#include <stdio.h>

#include <boost/algorithm/string.hpp>

#include <dirent.h>
#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace boost::algorithm;

using std::cout;
using std::endl;
using std::string;


class FaceRecog {
public:
    void setTrainingImagesDirectory(string trainingFile);
    void train();
    bool recognize(Mat img);

};


#endif //FBFACERECOGNITION_FACERECOGNIZER_H
