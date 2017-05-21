#!/bin/bash

# Compile

g++ -g -std=c++11 main.cpp conf.cpp network.cpp -o netmon -I ../nanogui/ext/eigen/ -I ../nanogui/ext/glfw/include/ -I ../nanogui/ext/nanovg/src/ -lnanogui -pthread

# Run

LD_LIBRARY_PATH=/usr/local/lib/ $1 ./netmon
