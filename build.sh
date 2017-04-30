#!/bin/sh
c++ -std=c++11 -g -lstdc++ -lfltk -lfltk_images -I/usr/local/include -L/usr/local/lib -o minesweeper *.cpp
