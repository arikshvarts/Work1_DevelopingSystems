# Compiler and flags
CC = g++
CFLAGS = -Wall -g -Iinclude  # Include the 'include' directory for headers

# Directories
SRCDIR = src
INCDIR = include
OBJDIR = bin

# Source files and object files
SRCS = $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(INCDIR)/*.cpp)  # All .cpp files in 'src' and 'include'
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCS))  # Corresponding .o files
TARGET = program  # Output executable

# Default rule
all: $(TARGET)

# Linking the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# Rule for building object files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(OBJDIR)  # Ensure 'bin' directory exists
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(INCDIR)/%.cpp
	mkdir -p $(OBJDIR)  # Ensure 'bin' directory exists
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -rf $(OBJDIR) $(TARGET)

# Phony targets
.PHONY: all clean
