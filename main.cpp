#include <iostream>
#include "FaceDetector.h"
#include "FaceRecog.h"


int main() {
  FaceDetector fD;
  fD.loadCSV("/Users/wsgreen/ClionProjects/FacebookFaceRecognition/index.csv");
  fD.detect("/Users/wsgreen/ClionProjects/FacebookFaceRecognition/images/full/", "/Users/wsgreen/ClionProjects/FacebookFaceRecognition/images/faceROI/");

  FaceRecog fR;
  fR.setTrainingImagesDirectory("/Users/wsgreen/ClionProjects/FacebookFaceRecognition/images/faceROI/");
  fR.train();

  Mat img = imread("/Users/wsgreen/ClionProjects/FacebookFaceRecognition/images/faceROI/498848063618208.bmp", CV_8UC1);
  bool recog = fR.recognize(img);

  cout << recog << endl;

  img = imread("/Users/wsgreen/ClionProjects/FacebookFaceRecognition/images/Falses/415053601934485.bmp", CV_8UC1);
  recog = fR.recognize(img);

  cout << recog << endl;
  return 0;
}