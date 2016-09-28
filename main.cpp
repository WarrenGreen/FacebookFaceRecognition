#include <iostream>
#include "FaceDetector.h"
#include "FaceRecog.h"


int main() {
  FaceDetector fD;
  fD.loadCSV("./index.csv");
  fD.detect("./images/full/", "./images/test/");

  FaceRecog fR;
  fR.setTrainingImagesDirectory("images/test/");
  fR.train();

  Mat img = imread("test/image.bmp", CV_8UC1);
  bool recog = fR.recognize(img);

  return 0;
}