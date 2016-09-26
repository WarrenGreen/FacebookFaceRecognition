// Created by Warren Green on 9/24/16.
//

#include "FaceRecog.h"

Ptr<FaceRecognizer> _model = createLBPHFaceRecognizer();
string _trainingImagesDir;

void FaceRecog::setTrainingImagesDirectory(string trainingFile) {
  _trainingImagesDir = trainingFile;
}

void FaceRecog::train() {
  vector<Mat> images;
  vector<int> labels;

  DIR *dir = NULL;
  struct dirent *ent;
  dir = opendir(_trainingImagesDir.c_str());
  while ((ent = readdir (dir)) != NULL) {
    string imgPath(_trainingImagesDir + ent->d_name);
    Mat img = imread(imgPath, CV_8UC1);
    if(img.empty()) { continue;}

    images.push_back(img);
    labels.push_back(1);
  }
  _model->train(images, labels);

}

bool FaceRecog::recognize(Mat img) {
  int pl;
  double confidence;
  _model->set("threshold", 30.0);
  _model->predict(img, pl, confidence);
  return pl == 1;
}
