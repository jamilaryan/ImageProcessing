//
//  cartoonifyImage.cpp
//  Cartoonifyme
//
//  Created by Jamil on 2016-02-12.
//  Copyright © 2016 Jamil. All rights reserved.
//

#include "cartoonifyImage.hpp"
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;


Mat cartoonifyImage(Mat srcColor)
{
    /* 1. convert to gray, apply medianBlur */
    Mat gray;
    cvtColor(srcColor, gray, CV_BGR2GRAY); // convert image to grayscale for use in medianBlur
    const int MEDIAN_BLUR_FILTER_SIZE = 15;
    medianBlur(gray, gray, MEDIAN_BLUR_FILTER_SIZE);  // filter to smooth image, lower noise
    
    /* get 3 kinds of EVIL mode */
    Mat edges1, edges2;
    Scharr(gray, edges1, CV_8U, 1, 0); // 3x3 Scharr gradient filter along x,y to get a scary look
    Scharr(gray, edges2, CV_8U, 1,0,-1);
    edges1 += edges2;  // combine x & y edges together
    const int EVIL_EDGE_THRESHOLD = 12;
    Mat mask1;
    threshold(edges1, mask1, EVIL_EDGE_THRESHOLD, 255, THRESH_BINARY_INV); // binary threshold with low cut off
    medianBlur(mask1, mask1, 3); // 3x3 median blur
    return mask1;
    
    /* 2. apply laplacian filter for edge detection */
    Mat edges;
    const int LAPLACIAN_FILTER_SIZE = 5;
    Laplacian(gray, edges, CV_8U,LAPLACIAN_FILTER_SIZE);  // edge detection filter
    //return edges;
    
    /* 3. get binary threshold */
    Mat mask;
    const int EDGES_THRESHOLD = 80;
    threshold(edges, mask, EDGES_THRESHOLD, 255, THRESH_BINARY_INV);  // mask to remove brightness, make edges look like a sketch
    //return mask;
    
    /* 4. bilateralFilter as an edge preserving filter which is slow, so resize original image */
    Size size = srcColor.size();
    Size smallSize;
    smallSize.width = size.width/2;
    smallSize.height = size.height/2;
    Mat smallImg = Mat(smallSize,CV_8UC3);
    resize(srcColor, smallImg, smallSize,0,0,INTER_LINEAR); // resize original image by a factor of 4
    
    /* 5. apply bilateralFilter */
    Mat tmp = Mat(smallSize,CV_8UC3);
    int repititions = 7;  // repititions for strong cartoon effects
    for (int i = 0;i<repititions; i++) {
        int ksize = 9; // filter size, has a large effect on speed
        double sigmaColor = 9;  // filter color strength
        double sigmaSpace = 7;  // Spatial strength, affects speed
        bilateralFilter(smallImg, tmp, ksize, sigmaColor, sigmaSpace);
        bilateralFilter(tmp, smallImg, ksize, sigmaColor, sigmaSpace);
    }
    
    /* 6. expand the shrunk image back to original size */
    Mat bigImg,dst;
    resize(smallImg, bigImg, size, 0,0,INTER_LINEAR);
    dst.setTo(0);
    bigImg.copyTo(dst,mask);
    //return dst;
    
    
}