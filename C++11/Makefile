# Makefile for C++11 Features Guide

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pthread
TARGET = cpp11_features

.PHONY: all clean run

all: $(TARGET)

$(TARGET): cpp11_features.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
