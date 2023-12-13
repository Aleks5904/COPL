INC_DIR = include
SRC_DIR = src
SOURCES = $(shell find $(SRC_DIR)/ -name '*.cc')
OBJECTS = $(SOURCES:.cc=.o)
DEPS = $(OBJECTS:.o=.d)
TARGET = COPL2
CC = g++
CFLAGS = -march=native
CPPFLAGS = $(addprefix -I, $(INC_DIR)) -Wall -Wextra -pedantic

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
