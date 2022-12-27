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
#define NB_ROCK 5

cv::Vec3b color = cv::Vec3b(255,255,0);

int level = 1; 
int score = 0;
int highest = 1;
int tetris[H];
int wall = 1 |(1 << (W+1));
int walls[H];
int fullrow = (1 << (W+1)) - 2;
std::vector<int> rows2delete = {};
int move_offset = 0;

int rocks[NB_ROCK][4][4] = {
    {
        {std::stoi("1111", 0, 2),0,0,0},
        {std::stoi("1", 0, 2),std::stoi("1", 0, 2),std::stoi("1", 0, 2),std::stoi("1", 0, 2)},
        {std::stoi("1111", 0, 2),0,0,0},
        {std::stoi("1", 0, 2),std::stoi("1", 0, 2),std::stoi("1", 0, 2),std::stoi("1", 0, 2)}
    },
    {
        {std::stoi("111", 0, 2),std::stoi("010", 0, 2),0,0},
        {std::stoi("10", 0, 2),std::stoi("11", 0, 2),std::stoi("10", 0, 2),0},
        {std::stoi("010", 0, 2),std::stoi("111", 0, 2),0,0},
        {std::stoi("01", 0, 2),std::stoi("11", 0, 2),std::stoi("01", 0, 2),0}
    },
    {
        {std::stoi("11", 0, 2),std::stoi("01", 0, 2),std::stoi("01", 0, 2),0},
        {std::stoi("111", 0, 2),std::stoi("100", 0, 2),0,0},
        {std::stoi("10", 0, 2),std::stoi("10", 0, 2),std::stoi("11", 0, 2),0},
        {std::stoi("001", 0, 2),std::stoi("111", 0, 2),0,0}
    },
    {
        {std::stoi("11", 0, 2),std::stoi("11", 0, 2),0,0},
        {std::stoi("11", 0, 2),std::stoi("11", 0, 2),0,0},
        {std::stoi("11", 0, 2),std::stoi("11", 0, 2),0,0},
        {std::stoi("11", 0, 2),std::stoi("11", 0, 2),0,0}
    },
    {
        {std::stoi("011", 0, 2),std::stoi("110", 0, 2),0,0},
        {std::stoi("10", 0, 2),std::stoi("11", 0, 2),std::stoi("01", 0, 2),0},
        {std::stoi("110", 0, 2),std::stoi("011", 0, 2),0,0},
        {std::stoi("01", 0, 2),std::stoi("11", 0, 2),std::stoi("10", 0, 2),0}
    }
};


int rocks_s[2][5] = {
                        {4,3,2,2,3},
                        {1,2,3,2,2}
                    };

bool testRock();
void runTetris();
void showMap();
bool isFree(int x,int y);
bool test(int v , int n);
bool tryMoveLeft();
bool tryMoveRight();
bool tryRotate();

void updateBackground(){
    //update background
    for(int i=0;i < H;i++){
        for(int j=0;j < W+2;j++){
            if(isFree(j,i))setBackgroundPixel(j,i,cv::Vec3b(255,255,255));
            else setBackgroundPixel(j,i,cv::Vec3b(0,0,0));
        }  
    }
}
int move_c = 2;
int idx = -1;
int top = 1;
int rock[4];
bool godownfast = false;
int idx_rot = 0;
bool bGameIsOver = false;
bool bJustSpawned = false;
void gameTurn(){
    bJustSpawned = false;
    if(move_c > 0 && rows2delete.size()<1){
        //pick new piece only if rows2delete empty
        move_offset = 0;
        bJustSpawned = true;
        move_c = 0;
        if(idx>-1){
            highest = std::max(top + rocks_s[(idx_rot+1)%2][idx],highest);
        }
        idx = (idx+1) % NB_ROCK;
        top = H_G;
        // spawn rock
        idx_rot = rand() % 4;
        for(int i = 0 ; i < 4 ; i++ ){
            rock[i] = rocks[idx][idx_rot][i]<< (W+2-rocks_s[(idx_rot)%2][idx])/2;           
        }

    }

    if(rows2delete.size()>0){
        int r = rows2delete.back();
        rows2delete.pop_back();
        for(int i = r;i < H;i++){
            tetris[i]=tetris[i+1];
        }
        updateBackground();
    }

    while(move_c<1){

        top--;
        if(!testRock()){
            godownfast = false;
            top++;
            int s = 0;
            for(int i = 0 ; i < ROCK_SIZE ; i++){
                if(move_offset == 0)tetris[top+i] |= rock[i];
                else if(move_offset > 0)tetris[top+i] |= (rock[i] >> move_offset);
                else tetris[top+i] |= (rock[i] << -move_offset);
                if(tetris[top+i] == fullrow){
                    tetris[top+i] = 0;
                    rock[i] = 0;
                    rows2delete.push_back(top+i);
                    score += pow(2,s);
                    s++;
                    level = 1 + score/10;
                }
                rock[i] = 0;
            }
            move_c+=1;
            updateBackground();
            if(s==0 && bJustSpawned)bGameIsOver = true;
            break;
        }
        break;
    }
}


void LaunchGame(){
    newBackgroundFrame(cv::Size(W+2,H),cv::Scalar(255,255,255));
    start_move = NOW;
    for(int i = 0;i < H;i++){
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
    if(test(walls[H-y-1],x))return false;
    if(test(tetris[H-y-1],x))return false;
    return true;
}

bool testRock(){
    for(int i = 0 ; i < ROCK_SIZE ; i++){
        for(int j = 0; j < W+2; j++){
            int a = rock[i];
            if(move_offset > 0)a= (rock[i] >> move_offset);
            else a= (rock[i] << -move_offset);
            if(test(a,j)){
                if(!isFree(j,H-1-top-i))return false;
            }
        }
    }
    return true;
}

void showMap(){

    for(int i = 0 ; i < ROCK_SIZE ; i++){
        for(int j = 0; j < W+2; j++){
            int a = rock[i];
            if(move_offset > 0)a= (rock[i] >> move_offset);
            else a= (rock[i] << -move_offset);
            if(test(a,j)){
                setPixel(j,H-1-top-i,color);
            }
        }
    }

    //draw UI

    resize(40);
    drawText(40 ,25*40-10, cv::Scalar(0,40,200) , "Score: "+std::to_string(score),1);
    drawText(8*40 ,25*40-10, cv::Scalar(0,40,200) , "Level: "+std::to_string(level),1);
    if(bGameIsOver)drawText(2*40-10 ,10*40, cv::Scalar(200,10,200) , "GAME OVER",2);
    showResize("Tetris",1);
}

bool tryMoveLeft(){
    for(int i = 0 ; i < ROCK_SIZE ; i++){
        for(int j = 0; j < W+2; j++){
            int a = rock[i];
            if(move_offset > 0)a= (rock[i] >> move_offset);
            else a= (rock[i] << -move_offset);
            if(test(a>>1,j)){
                if(!isFree(j,H-1-top-i))return false;
            }
        }
    }

    move_offset++;
    return true;
}
bool tryMoveRight(){
    for(int i = 0 ; i < ROCK_SIZE ; i++){
        for(int j = 0; j < W+2; j++){
            int a = rock[i];
            if(move_offset > 0)a= (rock[i] >> move_offset);
            else a= (rock[i] << -move_offset);
            if(test(a<<1,j)){
                if(!isFree(j,H-1-top-i))return false;
            }
        }
    }

    move_offset--;
    return true;
}

bool tryRotate(){
    int r[4];
    for(int i = 0 ; i < ROCK_SIZE ; i++ ){
        r[i]= rocks[idx][(idx_rot+1)%4][i] << (W+2-rocks_s[(idx_rot+1)%2][idx])/2;
        for(int j = 0; j < W+2; j++){
            int a = r[i];
            if(move_offset > 0)a= (r[i] >> move_offset);
            else a= (r[i] << -move_offset);
            if(test(a,j)){
                if(!isFree(j,H-1-top-i))return false;
            }
        } 
    }
    for(int i = 0;i < ROCK_SIZE; i++){
        rock[i] = r[i];
    }
    idx_rot++;

    return true;
}

extern int keyboard;
void runTetris(){
    newFrame(cv::Size(W+2,H), cv::Scalar(255,255,255));
    switch(keyboard){
        case '<':case 'q':
            color = cv::Vec3b(100,0,100);
            tryMoveLeft();
            break;
        case '>':case 'd':
            color = cv::Vec3b(0,100,100);
            tryMoveRight();
            break;
        case 'z':
            tryRotate();
            break;    
        case 's':
            if(move_c<1)godownfast = true;
        default:
            break;
    }
    if(TIME_MOVE > pow(.95,level-1) || godownfast || rows2delete.size()>0){
        gameTurn();
        start_move = NOW;
    }
    showMap();
}