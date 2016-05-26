COMPILER = g++
FLAGS = -std=c++14 -O3

MSGPACK = `pkg-config --libs --cflags msgpack` 
AMQPCLIENT = `pkg-config --libs --cflags libSimpleAmqpClient`
OPENCV = -I/usr/local/include/opencv -I/usr/local/include -L/usr/local/lib -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_photo -lopencv_imgproc -lopencv_core
BOOST = -lboost_system
AVAHI = `pkg-config --libs --cflags avahi-client`
OTHERS = -lpthread
LIBS := $(MSGPACK) $(AMQPCLIENT) $(OPENCV) $(BOOST) $(AVAHI) $(OTHERS)

OBJS := $(wildcard ../lib/*.o)
SRCS := $(wildcard *.cpp)
EXECS := $(SRCS:.cpp=)

all: $(EXECS)

clean:
	rm -f $(EXECS)

%: %.cpp
	$(COMPILER) $@.cpp -o $@ $(OBJS) $(FLAGS) $(LIBS) 
