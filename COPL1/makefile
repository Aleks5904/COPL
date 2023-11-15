<<<<<<< HEAD
INC_DIR = include
SRC_DIR = src
SOURCES = $(shell find $(SRC_DIR)/ -name '*.cc')
OBJECTS = $(SOURCES:.cc=.o)
DEPS = $(OBJECTS:.o=.d)
TARGET = COPL1
CC = g++
CFLAGS = -march=native
CPPFLAGS = $(addprefix -I, $(INC_DIR)) -Wall -Wextra -pedantic
=======
CC=g++
RM=rm -f
CXXFLAGS=-std=c++11 -Wall -Wextra
LDFLAGS=
LDLIBS=

SRCS=main.cc parser.cc
OBJS=$(subst .cpp,.o,$(SRCS))
TARGET=program
>>>>>>> 7f27be1d42885576a8faeaaf0bf88efc338b4e3b

.PHONY: all clean debug release
release: CFLAGS += -O3 -DNDEBUG
release: all
debug: CFLAGS += -O0 -DDEBUG -ggdb3
debug: all
all: $(TARGET)
<<<<<<< HEAD
=======

$(TARGET): $(OBJS)
	$(CC) $(CXXFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LDLIBS)

parser.o: parser.cc parser.h
main.o: main.cc parser.h token.h

>>>>>>> 7f27be1d42885576a8faeaaf0bf88efc338b4e3b
clean:
	rm -f $(OBJECTS) $(DEPS) $(TARGET)
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $^
-include $(DEPS)
%.o: %.cc
	$(CC) $(CFLAGS) $(CPPFLAGS) -MMD -o $@ -c $<
