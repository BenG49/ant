#!/usr/bin/env sh
g++ -c main.cpp
g++ main.o -lsfml-graphics -lsfml-window -lsfml-system -O2
./a.out $@
