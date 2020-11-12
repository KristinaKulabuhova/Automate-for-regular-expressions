#!/bin/bash
clear && g++ -std=c++20 main.cpp automate.cpp automate_builder.cpp -g $1 -o main && ./main && dot -Tpng dump.dot > dump.png && feh dump.png