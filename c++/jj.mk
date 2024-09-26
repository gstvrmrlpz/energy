ALL = bvector string

STD = c++17
CXXFLAGS = -flto -march=native -O3 -std=$(STD) -Wall $(XTR) $(XT2)

