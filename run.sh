#!/bin/bash

g++ main.cpp -I /home/varga/dependencies/OCLPack/include -I /home/varga/dependencies/OGLPack/include -lGL -lGLU -lglut -lm -lGLEW -lOpenCL 
./a.out