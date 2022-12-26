#include <chrono>
#include <unistd.h>
#include <iostream>
#include "./utils/cvutils.h"

using namespace std;
using namespace std::chrono;

#define FPS 30
#define SLEEP_TIME 1000000/FPS
high_resolution_clock::time_point start;
high_resolution_clock::time_point start_move;
#define NOW high_resolution_clock::now()
#define TIME duration_cast<duration<double>>(NOW - start).count()
#define TIME_MOVE duration_cast<duration<double>>(NOW - start_move).count()

#define H 25
#define H_G 20
#define W 10
#define ROCK_SIZE 4

cv::Vec3b color = cv::Vec3b(255,255,0);

int level = 0; 
int highest = 1;
int tetris[H+1];
int wall = 1 |(1 << (W+1));
int walls[H+1];

int rocks[5][4] = {
    {std::stoi("1111", 0, 2),0,0,0},
    {std::stoi("010", 0, 2),std::stoi("111", 0, 2),std::stoi("010", 0, 2),0},
    {std::stoi("11", 0, 2),std::stoi("01", 0, 2),std::stoi("01", 0, 2),std::stoi("01", 0, 2)},
    {std::stoi("1", 0, 2),std::stoi("1", 0, 2),std::stoi("1", 0, 2),std::stoi("1", 0, 2)},
    {std::stoi("11", 0, 2),std::stoi("11", 0, 2),0,0},

};
int rocks_s[5] = {(W+2)/2 - 2,(W+2)/2 - 1, (W+2)/2 - 1, (W+2)/2, (W+2)/2};
int rocks_h[5] = {1,3,4,4,2};

bool testRock();
void runTetris();
void showMap();
bool isFree(int x,int y);
bool test(int v , int n);
void updateBackground(){
    //update background
    for(int i=0;i < H+1;i++){
        for(int j=0;j < W+2;j++){
            if(isFree(j,i))continue;
            setBackgroundPixel(j,i,cv::Vec3b(0,0,0));
        }  
    }
}
int move_c = 2;
int idx = -1;
int top = 1;
int rock[4];
bool godownfast = false;

void gameTurn(){
    if(move_c > 0){
        
        move_c = 0;
        if(idx>-1){
            highest = std::max(top + rocks_h[idx],highest);
        }
        idx = (idx+1) % 5;
        top = H_G;
        // spawn rock
        for(int i = 0 ; i < 4 ; i++ ){
            rock[i] = rocks[idx][i]<< rocks_s[idx];           
        }

    }

    while(move_c<1){

        top--;
        for(int i = 0 ; i < 4 ; i++ ){
            if(!testRock()){
                godownfast = false;
                top++;
                for(int i = 0 ; i < ROCK_SIZE ; i++){
                    for(int j = 0; j < W+2; j++){
                        if(test(rock[i],j)){
                            tetris[top+i] |= 1 << j;
                        }
                    }
                }
                updateBackground();
                move_c+=1;
                break;
            }
        }
        break;
    }
}


void LaunchGame(){
    newBackgroundFrame(cv::Size(W+2,H+1),cv::Scalar(255,255,255));
    start_move = NOW;
    for(int i = 0;i < H+1;i++){
        walls[i] = wall;
    } 
    walls[0] = (1 << (W+2))-1;

    updateBackground();
    showMap();
    
    move_c = 2;

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
    if(test(tetris[H-y],x))return false;
    return true;
}

bool testRock(){
    for(int i = 0 ; i < ROCK_SIZE ; i++){
        for(int j = 0; j < W+2; j++){
            if(test(rock[i],j)){
                if(!isFree(j,H-top-i))return false;
            }
        }
    }
    return true;
}

void showMap(){

    for(int i = 0 ; i < ROCK_SIZE ; i++){
        for(int j = 0; j < W; j++){
            if(test(rock[i],j)){
                setPixel(j,H-top-i,color);
            }
        }
    }
    showResize("Tetris",1,40);
}
extern int keyboard;
void runTetris(){
    newFrame(cv::Size(W+2,H+1), cv::Scalar(255,255,255));
    
    switch(keyboard){
        case '<':case 'q':
            color = cv::Vec3b(100,0,100);
            break;
        case '>':case 'd':
            color = cv::Vec3b(0,100,100);
            break;
        case 's':
            godownfast = true;
        default:
            break;
    }
    if(TIME_MOVE > pow(.95,level) || godownfast){
        gameTurn();
        start_move = NOW;
    }
    showMap();
}