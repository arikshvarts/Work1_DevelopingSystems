# Compiler and flags
CC = g++
CFLAGS = -Wall -g -Iinclude -std=c++11

# Directories
SRCDIR = src
INCDIR = include
BINDIR = bin

# Source and object files
SOURCES = $(wildcard $(SRCDIR)/*.cpp)  # take all files with .cpp ending
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(BINDIR)/%.o, $(SOURCES))  # Object files in bin/
TARGET = $(BINDIR)/main  # Output executable

# Default rule
all: $(TARGET)

# Build executable
$(TARGET): $(OBJECTS)
	mkdir -p $(BINDIR)  # Ensure 'bin' directory exists
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

# Build object files
$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(BINDIR)  # Ensure 'bin' directory exists
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(BINDIR)/*.o $(TARGET)

# Phony targets
.PHONY: all clean

run: all
	./$(TARGET)run: all
	./$(TARGET)