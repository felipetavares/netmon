#!/bin/bash

# Compile

g++ -g -std=c++11 main.cpp -o netmon -I ../nanogui/ext/eigen/ -I ../nanogui/ext/glfw/include/ -I ../nanogui/ext/nanovg/src/ -lnanogui

# Run

LD_LIBRARY_PATH=/usr/local/lib/ $1 ./netmon