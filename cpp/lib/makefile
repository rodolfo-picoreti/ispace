COMPILER = g++
FLAGS = -std=c++14 -O3

SRCS := $(wildcard *.cpp)
SRCS := $(SRCS:.cpp=)

all: $(SRCS)

$(SRCS):
	$(COMPILER) -c $@.cpp $(FLAGS) -o $@.o 
