#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace cv;
using namespace std;

class MyObject {
public:
     int maxC;
     vector<Point> pos;

     Point centre() {
          Point sump = std::accumulate(pos.begin(), pos.end(), Point(0, 0));
          sump.x /= (double)pos.size();
          sump.y /= (double)pos.size();
          return sump;
     }

     void addP(Point p) {
          pos.push_back(p);
          if (pos.size() > maxC)
          {
            //vector withot first element
               pos = vector<Point>(pos.begin() + 1, pos.end());
          }
     }

     void addEmpty() {
          if (pos.size() > 0)
          {
               pos = vector<Point>(pos.begin() + 1, pos.end());
          }
     }
};

void drawPictureOnPict(const Mat srcMat, const Mat imgToPut, Mat &f3, const vector<Point> contour)
{
	Mat dstMat(Size(srcMat.cols, srcMat.rows), CV_8UC3);
	Mat cutter(Size(imgToPut.cols, imgToPut.rows), imgToPut.type());

	cutter = Scalar(0);
	bitwise_not(cutter, cutter);
	vector<Point2f> src = {{0, 0}, {(float)dstMat.cols, 0}, {(float)dstMat.cols, (float)dstMat.rows}, {0, (float)dstMat.rows}};
	vector<Point2f> dst;
	for (auto p : contour)
		dst.push_back(Point2f(p.x, p.y));
	auto wrap_mtx = getPerspectiveTransform(src, dst);
	warpPerspective(imgToPut, dstMat, wrap_mtx, Size(dstMat.cols, dstMat.rows));

	warpPerspective(cutter, cutter, wrap_mtx, Size(dstMat.cols, dstMat.rows));
	bitwise_not(cutter, cutter);
	Mat dstMatClone;
	srcMat.copyTo(dstMatClone, cutter);

	dstMat = dstMat + dstMatClone;
	f3 = dstMat;
}

int main() {
     // int loRange[3] = {0,100,110};
     // int hiRange[3] = {0,255,255};
     namedWindow("f1", WINDOW_AUTOSIZE);
     int H_low = 0, H_high = 255;
     int S_low = 0 ,S_high = 255;
     int V_low = 0, V_high = 255;
     createTrackbar("H_low", "f1", &H_low, 255);
     createTrackbar("S_low", "f1", &S_low, 255);
     createTrackbar("V_low", "f1", &V_low, 255);
     createTrackbar("H_high", "f1", &H_high, 255);
     createTrackbar("S_high", "f1", &S_high, 255);
     createTrackbar("V_high", "f1", &V_high, 255);

     VideoCapture camera(0);
     Mat imgtoshow;
	imgtoshow = imread("flag.png", IMREAD_COLOR);

     MyObject obj1, obj2;
     obj1.maxC = 10;
     obj2.maxC = 10;
    
     while ( waitKey(1) != 27  ) {
          Mat f, f1;

          camera >> f;
          flip(f, f, 1);
          
          cvtColor(f, f1, COLOR_BGR2HSV);
          inRange(f1, Scalar(H_low, S_low, V_low), Scalar(H_high, S_high, V_high), f1);
          imshow("f1", f1);

          vector<vector<Point>> contours;
          findContours(f1, contours, cv::RETR_LIST, cv::CHAIN_APPROX_TC89_KCOS);

          sort(contours.begin(), contours.end(),
               [](auto &a, auto &b) {
                    return contourArea(a, false) > contourArea(b, false);
               });

          for(int i = 0;i < contours.size(); i++) {
               approxPolyDP(contours.at(i),contours.at(i),10,true);
               if(i == 0 || i == 1)
                    drawContours(f,contours,i,{0,0,255,255},3);
          }

          if (contours.size() >= 2) {
               Point avg;
               Rect r = boundingRect(contours.at(0));
               avg.x = r.x + r.width / 2;
               avg.y = r.y + r.height / 2;
               obj1.addP(avg);

               Point avg1;
               Rect r1 = boundingRect(contours.at(1));
               avg1.x = r1.x + r1.width / 2;
               avg1.y = r1.y + r1.height / 2;
               obj2.addP(avg1);
          }
          else {
               obj1.addEmpty();
               obj2.addEmpty();
          }

          if (obj1.pos.size() > 1) {
               putText(f, "C", obj1.centre(), cv::FONT_HERSHEY_PLAIN, 2, {0, 0, 255, 255});
          }
          
          if (obj2.pos.size() > 1) {
               putText(f, "C", obj2.centre(), cv::FONT_HERSHEY_PLAIN, 2, {0, 0, 255, 255});
          }

          int centre = abs(obj1.centre().y - obj2.centre().y);

          if(centre < 33) {
               //drawPictureOnPict(f, imgtoshow, f3, contours.at(0));
               //imshow("f3", f3);
               line(f,obj1.centre(),obj2.centre(),{255,0,0},5);
          }
          
          imshow("f", f);

     }
     return 0;
}
