COMPILER = g++
FLAGS = -std=c++11 -O3
LIBS = `pkg-config --libs --cflags msgpack` `pkg-config --libs --cflags libSimpleAmqpClient`

all: publisher subscriber

publisher: publisher.cpp
	g++ publisher.cpp $(FLAGS) $(LIBS) -o $@

subscriber: subscriber.cpp
	g++ subscriber.cpp $(FLAGS) $(LIBS) -o $@