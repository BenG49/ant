#!/usr/bin/env sh

g++ -c main.cpp -Wall
g++ main.o -lsfml-graphics -lsfml-window -lsfml-system -O2 -Wall
