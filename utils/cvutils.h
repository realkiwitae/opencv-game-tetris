#ifndef CVUTILS_H
#define CVUTILS_H

#pragma once

#include "opencv2/opencv.hpp"

extern void show(std::string title,int i);
void resize(int x);
void  showResize(std::string title,int i);
extern void setPixel(int x,int y,cv::Vec3b c);
extern void setBackgroundPixel(int x,int y,cv::Vec3b c);
extern void newFrame(cv::Size size = cv::Size(1000,1000),cv::Scalar c = cv::Scalar(0,0,0));
extern void newBackgroundFrame(cv::Size size = cv::Size(1000,1000),cv::Scalar c = cv::Scalar(0,0,0));
extern void newFrameBW(cv::Size size = cv::Size(1000,1000));
extern void drawText(double x,double y , cv::Scalar c , std::string msg,int s);
extern void drawCircle( double x, double y,double r, cv::Scalar c, std::string msg = "");
extern void drawLine( std::vector<cv::Point> pts , cv::Scalar c, int thickness, std::string msg = "");
#endif