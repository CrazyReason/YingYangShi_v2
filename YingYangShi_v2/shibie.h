#pragma once

#include<iostream>  
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>

cv::Point ShiBie(char * img_name, char * templ_name, int match_method);

cv::Point ShiBie(HBITMAP Img_Bitmap, char * templ_name, int match_method);

cv::Point ShiBie(cv::Mat templ, int match_method);

double ShiBie_LunKuo(std::vector<std::vector<cv::Point>> tmp);

cv::Point ShiBie3(HBITMAP Img_Bitmap, char * templ_name, int match_method);
