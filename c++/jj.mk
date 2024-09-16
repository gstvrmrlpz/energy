include ./makefile

CXXFLAGS = -flto -march=native -O3 -std=$(STD) -Wall $(XTR) $(XT2)
STD = -std=c++17
