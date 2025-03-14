CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic

all: basic intermediate advanced

basic: cpp17_basic_features
intermediate: cpp17_intermediate_features
advanced: cpp17_advanced_features

cpp17_basic_features: cpp17_basic_features.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

cpp17_intermediate_features: cpp17_intermediate_features.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

cpp17_advanced_features: cpp17_advanced_features.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f cpp17_basic_features cpp17_intermediate_features cpp17_advanced_features

.PHONY: all basic intermediate advanced clean
