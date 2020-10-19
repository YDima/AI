#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/highgui.hpp>
#include <iostream>

int main( int argc, char** argv ) {
     
     using namespace cv;
     
     using namespace std;
     
     bool capturing = true;
     
     VideoCapture cap(0);
     
     if ( !cap.isOpened() ) {
          cerr << "error opening frames source" << endl;
          return -1;
     }
     
     cout << "Video size: " << cap.get( CAP_PROP_FRAME_WIDTH )
     << "x" << cap.get( CAP_PROP_FRAME_HEIGHT ) << endl;
     
     do {
          Mat frame, frameRevers;
          
          if ( cap.read( frame ) ) {
               
               // flipcode: 1 - flip around y-axis     0 - flip around x-axis     -1 -flip both axes
               flip(frame, frameRevers, 1);
               imshow( "Normal view", frame );
               imshow( "Flip view", frameRevers );
               
          } else {
               capturing = false;
          }
     
          if( (waitKey( 1000.0/60.0 )&0x0ff) == 27 )
               capturing = false;
          
     } while( capturing );
     
     return 0;
     
}
