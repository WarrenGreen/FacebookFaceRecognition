//
// Created by Warren Green on 9/24/16.
//

#include "FaceDetector.h"

String face_cascade_name = "/Users/wsgreen/ClionProjects/FacebookFaceRecognition/haarcascade_frontalface_alt.xml";
String eyes_cascade_name = "/Users/wsgreen/ClionProjects/FacebookFaceRecognition/haarcascade_eye_tree_eyeglasses.xml";
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;

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
  if( !eyes_cascade.load( eyes_cascade_name ) ){ printf("--(!)Error loading eyes cascade\n"); return; };

  DIR *dir = NULL;
  struct dirent *ent;
  dir = opendir(inputDirectory.c_str());
  while ((ent = readdir (dir)) != NULL) {
    Mat img;
    string imgPath(inputDirectory + ent->d_name);
    img = imread(imgPath, CV_LOAD_IMAGE_COLOR);
    if (img.empty()) { continue; }
    //imshow("h", img);
    //waitKey(0);

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
    cout<< imgPath << " - " <<  faces.size() <<endl;
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

void detectAndDisplay( Mat frame )
{
  std::vector<Rect> faces;
  Mat frame_gray;
  cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
  equalizeHist( frame_gray, frame_gray );
  //-- Detect faces
  face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30) );
  Mat faceROI;
  for ( size_t i = 0; i < faces.size(); i++ )
  {
    rectangle(frame,
              Rect(Point(faces[i].x, faces[i].y), Point(faces[i].x+faces[i].width, faces[i].y+faces[i].height)),
              Scalar(0,255,0), 4);
    faceROI = frame_gray( faces[i] );
    std::vector<Rect> eyes;
    //-- In each face, detect eyes
    eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CASCADE_SCALE_IMAGE, Size(30, 30) );
    for ( size_t j = 0; j < eyes.size(); j++ )
    {
      rectangle( frame,
                 Rect(Point(faces[i].x + eyes[j].x , faces[i].y + eyes[j].y),
                      Point(faces[i].x + eyes[j].x + eyes[j].width , faces[i].y + eyes[j].y + eyes[j].height)),
                 Scalar( 255, 0, 0 ), 4);
    }
  }
  //-- Show what you got
  if(!faceROI.empty())
    imshow( "t", frame );
}

int FaceDetector::detect() {
  VideoCapture capture;
  Mat frame;
  //-- 1. Load the cascades
  if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading face cascade\n"); return -1; };
  if( !eyes_cascade.load( eyes_cascade_name ) ){ printf("--(!)Error loading eyes cascade\n"); return -1; };
  //-- 2. Read the video stream
  capture.open( -1 );
  int count =0;
  if ( ! capture.isOpened() ) { printf("--(!)Error opening video capture\n"); return -1; }
  while ( capture.read(frame) )
  {
    if( frame.empty() )
    {
      printf(" --(!) No captured frame -- Break!");
      break;
    }
    //-- 3. Apply the classifier to the frame
    detectAndDisplay( frame );
    int c = waitKey(10);
    if( (char)c == 27 ) { break; } // escape
  }
  return 0;
}
