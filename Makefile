# Compiler settings
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -I/usr/local/include -I.
LDFLAGS = -L/usr/local/lib -lraylib -lGL -lm -lpthread -ldl -lX11 -lXrandr -lXinerama -lXcursor

# Directories
SRC_FILES = main.cpp headers/raylib-tileson.cpp
OBJ_FILES = $(patsubst %.cpp,build/%.o,$(SRC_FILES))

# Target executable
TARGET = chaos_craze

# Default target
all: $(TARGET)

# Compile source files to object files
build/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -Iheaders -c $< -o $@

# Link object files
$(TARGET): $(OBJ_FILES)
	$(CXX) $(OBJ_FILES) -o $@ $(LDFLAGS)

# Clean build files
clean:
	rm -rf build $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run 