COMPILER = g++
FLAGS = -std=c++14 -O3

MSGPACK = `pkg-config --libs --cflags msgpack` 
AMQPCLIENT = `pkg-config --libs --cflags libSimpleAmqpClient`
OPENCV = -I/usr/local/include/opencv -I/usr/local/include -L/usr/local/lib -lopencv_stitching -lopencv_superres -lopencv_videostab -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_dnn -lopencv_dpm -lopencv_fuzzy -lopencv_hdf -lopencv_line_descriptor -lopencv_optflow -lopencv_plot -lopencv_reg -lopencv_saliency -lopencv_stereo -lopencv_structured_light -lopencv_rgbd -lopencv_surface_matching -lopencv_tracking -lopencv_datasets -lopencv_text -lopencv_face -lopencv_xfeatures2d -lopencv_shape -lopencv_video -lopencv_ximgproc -lopencv_calib3d -lopencv_features2d -lopencv_flann -lopencv_xobjdetect -lopencv_objdetect -lopencv_ml -lopencv_xphoto -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_photo -lopencv_imgproc -lopencv_core
BOOST = -lboost_coroutine -lboost_system -lboost_thread -lboost_context -lboost_atomic -lboost_chrono -lboost_filesystem -lboost_timer -lboost_signals
OTHERS = -lpthread
LIBS := $(MSGPACK) $(AMQPCLIENT) $(OPENCV) $(OTHERS)

OBJS := $(wildcard ../lib/*.o)
SRCS := $(wildcard *.cpp)
SRCS := $(SRCS:.cpp=)

all: $(SRCS)

$(SRCS):
	$(COMPILER) $@.cpp -o $@ $(OBJS) $(FLAGS) $(LIBS) 
