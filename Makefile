# Standoff 2 External Tool - Makefile
# Supports both MinGW-w64 and MSVC

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -mwindows
LDFLAGS = -static-libgcc -static-libstdc++

# Directories
SRC_DIR = src
BUILD_DIR = build
OUTPUT_DIR = Lorer

# Core DLL sources
CORE_SRCS = $(SRC_DIR)/core/MemoryManager.cpp
CORE_OBJS = $(CORE_SRCS:.cpp=.o)
CORE_TARGET = $(OUTPUT_DIR)/Standoff2Core.dll

# Loader sources
LOADER_SRCS = $(SRC_DIR)/loader/Loader.cpp
LOADER_OBJS = $(LOADER_SRCS:.cpp=.o)
LOADER_TARGET = $(OUTPUT_DIR)/lorer.exe

# Default target
all: directories $(LOADER_TARGET) $(CORE_TARGET)

# Create directories
directories:
    @if not exist $(OUTPUT_DIR) mkdir $(OUTPUT_DIR)

# Build Loader
$(LOADER_TARGET): $(LOADER_OBJS)
    $(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) -ladvapi32

# Build Core DLL
$(CORE_TARGET): $(CORE_OBJS)
    $(CXX) $(CXXFLAGS) -shared -o $@ $^ $(LDFLAGS) -lpsapi

# Compile source files
%.o: %.cpp
    $(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
    @if exist $(OUTPUT_DIR) rmdir /s /q $(OUTPUT_DIR)
    @if exist $(SRC_DIR)\\core\\*.o del /q $(SRC_DIR)\\core\\*.o
    @if exist $(SRC_DIR)\\loader\\*.o del /q $(SRC_DIR)\\loader\\*.o

# Install dependencies (for MSYS2/MinGW)
install-deps:
    @echo Installing MinGW dependencies...
    @pacman -S --needed mingw-w64-x86_64-gcc mingw-w64-x86_64-make mingw-w64-x86_64-toolchain

.PHONY: all clean directories install-deps