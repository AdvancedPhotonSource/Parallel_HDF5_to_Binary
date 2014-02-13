CC = g++

CXXFLAGS = -Wall -g -fPIC

BOOST_BASE = /local/fkhan/local/boost_1_53_0-shared/

INCLUDES = -I${BOOST_BASE}/INCLUDE

LFLAGS = -L${BOOST_BASE}/lib

LIBS = -lboost_thread -lboost_system

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
