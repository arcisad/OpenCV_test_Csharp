#include <opencv2\core.hpp>
#include <opencv2\opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include <mmsystem.h>

using namespace cv;
using namespace std;

void DetectAndDisplay(Mat frame);
void PlayOnMove(Mat frame, Point Center);
void ShowOnMove(Mat frame, Point Center);

//Global variables
int still_in = 0;
Point out_cent;
String face_cascade_name = "haarcascade_frontalface_default.xml";
//String face_cascade_name = "haarcascade_frontalface_alt.xml";
//String face_cascade_name = "lbpcascade_frontalface.xml";
CascadeClassifier face_cascade;
CascadeClassifier eye_cascade;

int main(int, char**)
{

	//load classifiers
	if (!face_cascade.load(face_cascade_name)) { printf("Error loading face classifier\n"); return -1; }
	
	VideoCapture cap(0); // open the default camera
	if (!cap.isOpened())  // check if we succeeded
		return -1;

	Mat edges;
	namedWindow("edges", 1);

	for (;;)
	{
		Mat frame;
		cap >> frame; // get a new frame from camera
		if (!frame.empty())
		{
			DetectAndDisplay(frame);
		}
		else
		{
			printf("No image available ... \n");
			break;
		}
		imshow("edges", frame);
		int wt = waitKey(3);
		if ((char)wt == 'q') break;		
	}
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}

void DetectAndDisplay(Mat frame)
{
	vector<Rect> faces;
	//Detect faces
	face_cascade.detectMultiScale(frame, faces);
	if (faces.size() != 0) {
		for (size_t i = 0; i < faces.size(); i++)
		{
			Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
			out_cent = center;
			ellipse(frame, center, Size(faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);
		}
		ShowOnMove(frame, out_cent);
		PlayOnMove(frame, out_cent);
	}
}

void PlayOnMove(Mat frame, Point Center)
{
	Size s = frame.size();
	int row = s.width;
	int col = s.height;
	if (still_in == 0) {
		if (Center.x > (row - 200))
		{
			PlaySoundW(TEXT("Hasan.wav"), NULL, SND_FILENAME | SND_ASYNC);
			still_in = 1;
		}
		else if (Center.x < 200) {
			PlaySoundW(TEXT("Ziarat.wav"), NULL, SND_FILENAME | SND_ASYNC);
			still_in = 1;
		}
	}
	else {
		if ((Center.x < (row - 200)) && (Center.x > 200))
		{
			PlaySoundW(NULL, 0, 0);
			still_in = 0;
		}
	}
}

void ShowOnMove(Mat frame, Point Center)
{
	Size s = frame.size();
	int col = s.width;

	Point corrected_center;
	corrected_center.x = Center.x - 75;
	corrected_center.y = Center.y - 75;

	Mat right = imread("Hasan.jpg");
	Mat left = imread("Abbas.jpg");

	Rect right_rect(corrected_center, right.size());
	Rect left_rect(corrected_center, left.size());

	if (Center.x > (col - 200))
	{
		right.copyTo(frame(right_rect));
	}
	else if (Center.x < 200) {
		left.copyTo(frame(left_rect));
	}
}
