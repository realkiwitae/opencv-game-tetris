#include "cvutils.h"

cv::Mat img;
cv::Mat background;
bool bGotBackground = false;
int keyboard;
void drawCircle( double x, double y,double r, cv::Scalar c, std::string msg){
    circle(img, cv::Point(x,y),r, c,cv::FILLED, 8,0);
    putText(img, msg, cv::Point(x - .75*r, y + r/2),
     cv::FONT_HERSHEY_SIMPLEX, 1 , cv::Scalar() ,1, cv::LINE_AA);
}

void setPixel(int x,int y,cv::Vec3b c){
    
    img.at<cv::Vec3b>(cv::Point(x,y)) = c;
}
void setBackgroundPixel(int x,int y,cv::Vec3b c){
    background.at<cv::Vec3b>(cv::Point(x,y)) = c;
}

void show(std::string title,int i){
    cv::imshow(title, img);
    cv::waitKey(i);
}

void  showResize(std::string title,int i,int x){
    cv::Mat out;
    cv::resize(img, out, cv::Size(), x, x,cv::INTER_AREA);
    cv::imshow(title, out);
    keyboard = cv::waitKey(i);
}

void newFrame(cv::Size s,cv::Scalar c){
    if(!bGotBackground)img = cv::Mat(s, CV_8UC3, c);
    else background.copyTo(img);
}
void newBackgroundFrame(cv::Size s,cv::Scalar c){
    newFrame(s,c);
    background = cv::Mat(s, CV_8UC3, c);
    bGotBackground = true;
}

void newFrameBW(cv::Size s){
    img = cv::Mat(s, CV_8UC1, cv::Scalar(0));
}

void drawLine( std::vector<cv::Point> pts , cv::Scalar c, int thickness, std::string msg){
    
    for(int i = 0; i < pts.size()-1 ; i++){
        line(img, pts[i], pts[i+1], c,thickness , cv::LineTypes::LINE_8);
        putText(img, msg, (pts[i]+pts[i+1])/2,
        cv::FONT_HERSHEY_SIMPLEX, 1 , c,1, cv::LINE_AA);
    }

}