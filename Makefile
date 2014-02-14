CC = g++

CXXFLAGS = -Wall -g -fPIC

BOOST_BASE = /local/fkhan/local/boost_1_53_0-shared

H5_BASE = /local/fkhan/local/hdf5-1.8.12

INCLUDES = -I${BOOST_BASE}/include -I${H5_BASE}/include

LFLAGS = -L${BOOST_BASE}/lib -L${H5_BASE}/lib

LIBS = -lboost_thread-mt -lboost_thread -lboost_system -lhdf5

SRCS = $(wildcard *.cpp)

OBJS = $(SRCS:.cpp=.o)

OUT = h52imm

.PHONY: depend clean

all: $(OUT)

$(OUT): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(OUT) $(OBJS) $(LFLAGS) $(LIBS)

.cpp.o:
	$(CC) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) *.o $(OUT)

depend: $(SRCS)
	makedepend $(INCLUDES) $^
