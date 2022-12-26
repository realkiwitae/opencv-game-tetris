#include <iostream>
#include <opencv2/opencv.hpp>
#include <chrono>
using namespace std;
using namespace std::chrono;

#include "./tetris.cpp"

high_resolution_clock::time_point start;
#define NOW high_resolution_clock::now()
#define TIME duration_cast<duration<double>>(NOW - start).count()

int main()
{
  start = NOW;
  std::cout << "-------" << std::endl;
  runTetris();
  std::cout << "-------" << std::endl;
  std::cout << TIME << std::endl;
}