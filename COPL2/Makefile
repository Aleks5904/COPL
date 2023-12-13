CC=g++
RM=rm -f
CXXFLAGS=-std=c++11 -Wall
LDFLAGS=
LDLIBS=

SRCS=main.cpp AST.cpp
OBJS=$(subst .cpp,.o,$(SRCS))
TARGET=CPL2

.PHONY: all clean debug release
release: CFLAGS += -O3 -DNDEBUG
release: all
debug: CFLAGS += -O0 -DDEBUG -ggdb3
debug: all
all: $(TARGET)
clean:
	rm -f $(OBJECTS) $(DEPS) $(TARGET)
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $^
-include $(DEPS)
%.o: %.cc
	$(CC) $(CFLAGS) $(CPPFLAGS) -MMD -o $@ -c $<
