CC = g++
CFLAGS = -Wall -std=c++11

# List of source files
SRCS = AST.cpp main.cpp

# List of object files (automatically generated from source files)
OBJS = $(SRCS:.cpp=.o)

# The main target, which will be the name of the executable
TARGET = myprogram

# The build rule
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rules to compile individual source files into object files
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule to remove object files and the executable
clean:
	rm -f $(OBJS) $(TARGET)
