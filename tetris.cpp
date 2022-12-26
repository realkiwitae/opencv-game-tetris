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

#define H 20
#define W 10
#define ROCK_SIZE 4

cv::Vec3b color = cv::Vec3b(255,255,0);

int level = 0; 
int highest = 1;
int tetris[H];
int wall = 1 |(1 << (W+1));
int walls[H+1];
char pending_move = '0';

int rocks[5][4] = {
    {std::stoi("1111", 0, 2),0,0,0},
    {std::stoi("010", 0, 2),std::stoi("111", 0, 2),std::stoi("010", 0, 2),0},
    {std::stoi("111", 0, 2),std::stoi("001", 0, 2),std::stoi("001", 0, 2),0},
    {std::stoi("1", 0, 2),std::stoi("1", 0, 2),std::stoi("1", 0, 2),std::stoi("1", 0, 2)},
    {std::stoi("11", 0, 2),std::stoi("11", 0, 2),0,0},

};
int rocks_s[5] = {W/2 - 2,W/2 - 1, W/2 - 1, W/2, W/2};
int rocks_h[5] = {1,3,3,4,2};

bool testRock();
void runTetris();
void showMap();
bool isFree(int x,int y);
bool test(int v , int n);
int move_c = 2;
int idx = -1;
int top = 1;
int rock[4];

void gameTurn(){
    if(move_c > 0){
        move_c = 0;
        if(idx>-1){
            highest = std::max(top + rocks_h[idx],highest);
        }
        idx = (idx+1) % 5;
        top = H + 4;
        // spawn rock
        for(int i = 0 ; i < 4 ; i++ ){
            rock[i] = rocks[idx][i]<< rocks_s[idx];           
        }

    }

    while(move_c<1){

        top--;
        for(int i = 0 ; i < 4 ; i++ ){
            if(!testRock()){
                move_c+=1;
                top++;
                break;
            }
        }
        break;
        if(move_c < 1){

            
            //  std::cout << "move "<<c << std::endl;          
            switch(pending_move){
                case '<':
                    {
                        int srock[4];
                        bool b = true;
                        for(int i = 0 ; i < 4 ; i++ ){
                            srock[i] = rock[i];
                            rock[i] = rock[i] << 1;
                            if( ((tetris[top+i] ^ rock[i]) & rock[i]) != rock[i]){
                                b = false;
                            }
                        }

                        for(int i = 0 ; i < 4 ; i++ ){
                            if(!b)rock[i]=srock[i];
                            tetris[top+i] |= rock[i] ;
                        }                              

                    }
                    break;
                case '>':
                    {
                        int srock[4];
                        bool b = true;
                        for(int i = 0 ; i < 4 ; i++ ){
                            tetris[top+i] ^= rock[i];
                            srock[i] = rock[i];
                            rock[i] = rock[i] >> 1;
                            if( ((tetris[top+i] ^ rock[i]) & rock[i]) != rock[i]){
                                b = false;
                            }
                        }

                        for(int i = 0 ; i < 4 ; i++ ){
                            if(!b)rock[i]=srock[i];
                            tetris[top+i] |= rock[i] ;
                        }                              

                    }
                    break;
                default:
                break;

            }
        }
    }
}


void LaunchGame(){

    newFrame(cv::Size(W+2,H+1),cv::Scalar(255,255,255));
    start_move = NOW;
    for(int i = 0;i < H+1;i++){
        walls[i] = wall;
    } 
    walls[0] = (1 << (W+2))-1;
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
   // if(test(tetris[y-1],x-1))return false;
    return true;
}

bool testRock(){
    return true;
}

void showMap(){
    for(int i=0;i < H+1;i++){
        for(int j=0;j < W+2;j++){
            if(isFree(j,i))continue;
            setPixel(j,i,cv::Vec3b(0,0,0));
        }  
    }

    for(int i = 0 ; i < ROCK_SIZE ; i++){
        for(int j = 0; j < W; j++){
            if(test(rock[i],j)){
                setPixel(j+1,H-top-i,color);
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
        default:
            break;
    }
    if(TIME_MOVE > pow(.95,level)){
        gameTurn();
        start_move = NOW;
    }
    showMap();
}