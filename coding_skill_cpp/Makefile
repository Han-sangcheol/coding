# Makefile for C++ Design Patterns (Linux/Mac)

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
SOURCES = $(wildcard *.cpp)
TARGETS = $(SOURCES:.cpp=)

.PHONY: all clean

all: $(TARGETS)

%: %.cpp
	@echo "Compiling $<..."
	@$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	@echo "Cleaning..."
	@rm -f $(TARGETS)

run: all
	@for target in $(TARGETS); do \
		echo ""; \
		echo "==========================================="; \
		echo "Running $$target"; \
		echo "==========================================="; \
		./$$target; \
	done

.PHONY: help
help:
	@echo "Usage:"
	@echo "  make         - Compile all patterns"
	@echo "  make clean   - Remove executables"
	@echo "  make run     - Compile and run all"
