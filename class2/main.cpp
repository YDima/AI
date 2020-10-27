#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

int main(int argc, char **argv) {
     
     int a, b;
     
	VideoCapture cap(0);

	if (!cap.isOpened())
		return -1;

	namedWindow("hsv", WINDOW_AUTOSIZE);
     int H_low = 0, H_high = 255;
     int S_low = 0 ,S_high = 255;
     int V_low = 0, V_high = 255;
     createTrackbar("H_low", "hsv", &H_low, 255);
     createTrackbar("S_low", "hsv", &S_low, 255);
     createTrackbar("V_low", "hsv", &V_low, 255);
     createTrackbar("H_high", "hsv", &H_high, 255);
     createTrackbar("S_high", "hsv", &S_high, 255);
     createTrackbar("V_high", "hsv", &V_high, 255);
     
     if (argc == 1) {
          a = 320;
          b = 200;
     } else if (argc == 2) {
          a = atoi(argv[1]);
          b = a;
     } else if(argc == 3) {
          a = atoi(argv[1]);
          b = atoi(argv[2]);
     } else {
          cout<<"Wow too many numbers!";
     }
     
	while (true) {
          
		Mat f1, gaussian, hsv;
          
		cap >> f1;
          resize(f1, f1, {a, b});
          imshow("f1", f1);
          GaussianBlur(f1, gaussian, Size(3, 3), 0);
          imshow("Gaussian", gaussian);
          cvtColor(gaussian, hsv, COLOR_BGR2HSV);
          inRange(hsv, Scalar(H_low, S_low, V_low), Scalar(H_high, S_high, V_high), hsv);
          imshow("hsv", hsv);
		
		char k = waitKey(1);
          
		if (k == 27)
			break;
          
		if (k == 'x') {
			auto r = selectROI("hsv", hsv);
			Mat roi = f1(r);
               imwrite("photo.bmp", roi);
		}
	}
	return 0;
}
