/*
 * $Id: fv.c
 * $Desp: draw jpeg to framebuffer
 * $Author: rockins
 * $Date: Wed Jan  3 20:15:49 CST 2007
*/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <cstring>
#include <iostream>
#include <ctime>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#define    FB_DEV  "/dev/fb0"

//using namespace cv;
int main(int argc, char **argv) {
       cv::Mat left_image = cv::imread("depth0004.png", cv::IMREAD_COLOR);
       cv::namedWindow("left_image", cv::WINDOW_NORMAL);
       cv::imshow("left_image", left_image);
       cv::waitKey(0);
       return 0;
}
