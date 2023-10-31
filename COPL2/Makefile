CC=g++
RM=rm -f
CXXFLAGS=-std=c++11 -Wall
LDFLAGS=
LDLIBS=

SRCS=main.cpp AST.cpp
OBJS=$(subst .cpp,.o,$(SRCS))
TARGET=program

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CXXFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LDLIBS)

main.o: main.cpp AST.h token.h
AST.o: AST.cpp AST.h

clean:
	$(RM) $(OBJS) $(TARGET)

distclean: clean

build: $(TARGET)
	@echo "Finished building $(TARGET)."

run: build
	./$(TARGET) positives.txt
