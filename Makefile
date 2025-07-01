# Variables
BUILD_DIR := build
CMAKE := cmake
NINJA := ninja

# Source files to format
FORMAT_SOURCES := $(shell find src include -name '*.cpp' -o -name '*.h')

# Default target
.PHONY: all
all: debug

# Initialize Ninja build directory (generate build files)
.PHONY: init
init:
	$(CMAKE) -S . -B $(BUILD_DIR) -G Ninja

# Build Debug (with debug symbols, no optimization)
.PHONY: debug
debug: init
	$(CMAKE) -S . -B $(BUILD_DIR) -G Ninja -DCMAKE_BUILD_TYPE=Debug
	$(NINJA) -C $(BUILD_DIR)

# Build Release (optimized)
.PHONY: release
release: init
	$(CMAKE) -S . -B $(BUILD_DIR) -G Ninja -DCMAKE_BUILD_TYPE=Release
	$(NINJA) -C $(BUILD_DIR)

# Clean build directory
.PHONY: clean
clean:
	$(NINJA) -C $(BUILD_DIR) -t clean
	rm -rf $(BUILD_DIR)/CMakeCache.txt $(BUILD_DIR)/CMakeFiles

# Format source files with clang-format
.PHONY: format
format:
	clang-format -i $(FORMAT_SOURCES)

# Show usage help
.PHONY: help
help:
	@echo "Makefile commands:"
	@echo "  init    - Generate Ninja build files"
	@echo "  debug   - Build Debug version"
	@echo "  release - Build Release version"
	@echo "  clean   - Clean build files"
	@echo "  format  - Format source files with clang-format"
