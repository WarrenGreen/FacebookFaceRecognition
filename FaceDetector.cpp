//
// Created by Warren Green on 9/24/16.
//

#include "FaceDetector.h"

String face_cascade_name = "./haarcascade_frontalface_alt.xml";
CascadeClassifier face_cascade;

map<string, Point> faceLocs;

void FaceDetector::loadCSV(string indexFile) {
  ifstream file(indexFile);
  string str;
  while (getline(file, str))
  {
    vector<string> strSplit;
    split(strSplit, str, is_any_of(","));
    trim(strSplit[0]);
    faceLocs[strSplit[0]] = Point(stoi(strSplit[1]), stoi(strSplit[2]));
  }
}

double distance(int matchX, int matchY, int faceX, int faceY) {
  return sqrt(pow(faceX - matchX,2) + pow(faceY - matchY, 2));
}

void FaceDetector::detect(string inputDirectory, string outputDirectory) {

  if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading face cascade\n"); return; };

  DIR *dir = NULL;
  struct dirent *ent;
  dir = opendir(inputDirectory.c_str());
  while ((ent = readdir (dir)) != NULL) {
    Mat img;
    string imgPath(inputDirectory + ent->d_name);
    img = imread(imgPath, CV_LOAD_IMAGE_COLOR);
    if (img.empty()) { continue; }

    std::vector<Rect> faces;
    Mat frame_gray;
    cvtColor( img, frame_gray, COLOR_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );
    //-- Detect faces
    face_cascade.detectMultiScale( frame_gray, faces);
    Rect matchFace;
    string fileName = ent->d_name;
    trim_right_if(fileName, is_any_of(".jpg"));
    Point actual = faceLocs[fileName];
    actual.x = actual.x * img.cols / 100;
    actual.y = actual.y * img.rows / 100;
    if (faces.size() <= 0 )
      continue;

    for ( size_t i = 0; i < faces.size(); i++ )
    {
      if ((matchFace.x == 0 && matchFace.y == 0) || distance(matchFace.x, matchFace.y, actual.x, actual.y) > distance(faces[i].x, faces[i].y, actual.x, actual.y) )
        matchFace = faces[i];
    }

    Mat finalFace = frame_gray(matchFace);
    //-- Show what you got
    if(!finalFace.empty())
      imwrite( outputDirectory+fileName+".bmp", finalFace );
  }
}
