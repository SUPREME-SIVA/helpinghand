# Makefile for C++14 Features Guide

CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -pthread
TARGET = cpp14_features
SOURCES = cpp14_features.cpp cpp14_basic_features.cpp cpp14_intermediate_features.cpp

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ $^

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
