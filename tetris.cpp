

#include <chrono>
#include <unistd.h>

using namespace std;
using namespace std::chrono;

#define FPS 30
#define SLEEP_TIME 1000000/FPS
high_resolution_clock::time_point start;
#define NOW high_resolution_clock::now()
#define TIME duration_cast<duration<double>>(NOW - start).count()


#define H 20
#define W 10


bool bInit = false;

long highest = 1;
int tetris[H];
int wall = 1 |(1 << (W+1));
int walls[H+1];

int rocks[5][4] = {
    {std::stoi("000111100", 0, 2),0,0,0},
    {std::stoi("000010000", 0, 2),std::stoi("000111000", 0, 2),std::stoi("000010000", 0, 2),0},
    {std::stoi("000111000", 0, 2),std::stoi("000001000", 0, 2),std::stoi("000001000", 0, 2),0},
    {std::stoi("000100000", 0, 2),std::stoi("000100000", 0, 2),std::stoi("000100000", 0, 2),std::stoi("000100000", 0, 2)},
    {std::stoi("000110000", 0, 2),std::stoi("000110000", 0, 2),0,0},

};
int rocks_h[5] = {1,3,3,4,2};

void runTetris();
void showMap();
bool test(int v , int n);

void LaunchGame(){
    newFrame(cv::Size(W+2,H+1),cv::Scalar(255,255,255));
    
    for(int i = 0;i < H+1;i++){
        walls[i] = wall;
    } 
    walls[0] = (1 << (W+2))-1;
    showMap();

    while (true)
    {
        start = NOW;
        
        runTetris();
        int us = SLEEP_TIME - TIME*1e6;
        usleep(us);
    }
}

bool test(int v , int n){
    return v & (1 << n);
}

bool isFree(int x,int y){
    if(test(walls[H-y],x))return false;
   // if(test(tetris[y-1],x-1))return false;
    return true;
}

void showMap(){
    for(int i=0;i < H+1;i++){
        for(int j=0;j < W+2;j++){
            if(isFree(j,i))continue;
            setPixel(j,i,cv::Vec3b(0,0,0));
        }  
    }

    showResize("Tetris",1,40);
}

void runTetris(){
   
    newFrame(cv::Size(W+2,H+1), cv::Scalar(255,255,255));
    showMap();
}
