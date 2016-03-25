//
//  main.cpp
//  Cartoonifyme
//
//  Created by Jamil on 2016-02-05.
//  Copyright © 2016 Jamil. All rights reserved.
//

#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "cartoonifyImage.hpp"


using namespace::cv;
using namespace std;

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "OpenCv version: \n"<< CV_VERSION;
    
    
    
    int cameraNumber = 0;
    
    if (argc>1) {
        cameraNumber = atoi(argv[1]);
    }
    
    //get access to the camera
    cv::VideoCapture camera;
    camera.open(cameraNumber);
    if (!camera.isOpened()) {
        std::cerr << "Error: Count access camera" <<std::endl;
        exit(1);
        
    }
    
    //trying to set camera resolution
    camera.set(cv::WINDOW_FREERATIO, 160);
    camera.set(cv::WINDOW_FREERATIO, 120);
    
    while (true) {
        // grab next frame
        cv::Mat cameraFrame;
        camera >> cameraFrame;
        
        if (cameraFrame.empty()) {
            std::cerr << "Error: Couldnt grab frame"
            <<std::endl;
        }
        //creat a blank output
        //cv::Mat displayedFrame;
        
        /*
        Mat gray;
        cvtColor(cameraFrame, gray, CV_BGR2GRAY);
        const int MEDIAN_BLUR_FILTER_SIZE = 15;
        medianBlur(gray, gray, MEDIAN_BLUR_FILTER_SIZE);
        Mat edges;
        const int LAPLACIAN_FILTER_SIZE = 5;
        Laplacian(gray, edges, CV_8U,LAPLACIAN_FILTER_SIZE);
        
        Mat mask;
        const int EDGES_THRESHOLD = 80;
        threshold(edges, mask, EDGES_THRESHOLD, 255, THRESH_BINARY_INV);
         
         cv::imshow("Cartoonifier1", mask);
         */
        
        
        Mat processedFrame = cartoonifyImage(cameraFrame);
        
        //display the processed image
        
        cv::imshow("Cartoonifier", cameraFrame);
        
        cv::imshow("Cartoonifier1", processedFrame);
        
        char keypress = cv::waitKey(20);
        if (keypress==27) {
            break;
        }
    }
    
    
    
    return 0;
}


