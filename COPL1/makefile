CC=g++
RM=rm -f
CXXFLAGS=-std=c++11 -Wall
LDFLAGS=
LDLIBS=

SRCS=main.cpp parser.cpp
OBJS=$(subst .cpp,.o,$(SRCS))
TARGET=CPL1

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CXXFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LDLIBS)

main.o: main.cpp parser.h token.h
AST.o: parser.cpp token.h

clean:
	$(RM) $(OBJS) $(TARGET)

distclean: clean

build: $(TARGET)
	@echo "Finished building $(TARGET)."

run: build
	./$(TARGET) positives.txt
