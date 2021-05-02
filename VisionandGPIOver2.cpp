#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <chrono>
#include <thread>
#include <signal.h>
#include <JetsonGPIO.h>



using namespace std;
using namespace cv;





int output_pin = 18; // BOARD pin 12, BCM pin 18

bool end_this_program = false;

inline void delay(int s){
        this_thread::sleep_for(chrono::seconds(s));
}

void signalHandler (int s){
        end_this_program = true;
}







std::string gstreamer_pipeline (int capture_width, int capture_height, int display_width, int display_height, int framerate, int flip_method) {
    return "nvarguscamerasrc ! video/x-raw(memory:NVMM), width=(int)" + std::to_string(capture_width) + ", height=(int)" +
           std::to_string(capture_height) + ", format=(string)NV12, framerate=(fraction)" + std::to_string(framerate) +
           "/1 ! nvvidconv flip-method=" + std::to_string(flip_method) + " ! video/x-raw, width=(int)" + std::to_string(display_width) + ", height=(int)" +
           std::to_string(display_height) + ", format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
}

int main()
{

// When CTRL+C pressed, signalHandler will be called
        signal(SIGINT, signalHandler);

        // Pin Setup. 
        GPIO::setmode(GPIO::BCM);
        // set pin as an output pin with optional initial state of HIGH

     // set pin as an output pin with optional initial state of HIGH
        GPIO::setup(output_pin, GPIO::OUT, GPIO::HIGH);

        cout << "Strating demo now! Press CTRL+C to exit" << endl;
        int curr_value = GPIO::HIGH;

        //////////////////////////////////////////////////////////////////////////////


    int capture_width = 1280 ;
    int capture_height = 720 ;
    int display_width = 1280 ;
    int display_height = 720 ;
    int framerate = 60 ;
    int flip_method = 2 ;/// orignally was 0  i changed to 2 should rotate camera image by 180 
                         //degrees 

    std::string pipeline = gstreamer_pipeline(capture_width,
        capture_height,
        display_width,
        display_height,
        framerate,
        flip_method);
    std::cout << "Using pipeline: \n\t" << pipeline << "\n";

    cv::VideoCapture cap(pipeline, cv::CAP_GSTREAMER);
    if(!cap.isOpened()) {
        std::cout<<"Failed to open camera."<<std::endl;
        return (-1);
    }

    cv::namedWindow("CSI Camera", cv::WINDOW_AUTOSIZE);
    cv::Mat img;



    cv::namedWindow("Detected BottleCap", cv::WINDOW_AUTOSIZE);
    Mat image;

    std::cout << "Hit ESC to exit" << "\n" ;
    while(true)
    {

        if (!cap.read(img)) {
                std::cout<<"Capture read error"<<std::endl;
                break;
        }



       cap >> image;  // thats me or try cap.read(image) as in original;
       if (image.empty())



                             
{
cerr<<"Error for object detection mat capture"<<endl;// it is me again ,leave break and remowe info and brackets if it isnotwor
       break;
}
     /////////////////////////////////////////////////
       CascadeClassifier cap_cascade;
       cap_cascade.load( "cascadefalserate02.xml" ); // I have change file name and path as well
       if(!cap_cascade.load("cascadefalserate02.xml"))
       {
         cerr<<"Error Loading XML file"<<endl;
         return 0;
       }
 
      // Detect green cap
      std::vector<Rect> caps;
      cap_cascade.detectMultiScale( image, caps, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(50,50)); //changeto CASCADE_SCALE_IM
                                                                                                    
      // Draw circles on the detected green caps
      for( int i = 0; i < caps.size(); i++ )
      {
        Point center( caps[i].x + caps[i].width*0.5, caps[i].y + caps[i].height*0.5 );
        ellipse( image, center, Size( caps[i].width*0.5, caps[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );

        if(curr_value==0){
        curr_value=1; //will set PIN HIGH
        }



        cout<<"WE HAVE IT!!!!! SEND SIGNAL AND PAUSE FOR REQUIRED TIME to REMOVE Reject\n"<<endl;
        cout << "Outputting " << curr_value << " to pin ";
                cout << output_pin << endl;
                GPIO::output(output_pin, curr_value);

       delay(0.5); //reduce the speed for PIC operations,code should be rewritten for professional application
          
   
          
      }

      if(caps.size()==0){
        cout<<"No Object for Rejection\n"<<endl;
        curr_value=0;//OFF No SIGNAL      

        //curr_value ^= GPIO::HIGH;
      GPIO::output(output_pin, curr_value);
      cout << "Outputting " << curr_value << " to pin ";
     
      }



        cv::imshow( "Detected BottleCap", image );

        cv::imshow("CSI Camera",img);
        int keycode = cv::waitKey(30) & 0xff ;
        if (keycode == 27) break ;

    }
    
      

    GPIO::cleanup();
    cap.release();
    cv::destroyAllWindows() ;
    return 0;
}
                                                     
                           

