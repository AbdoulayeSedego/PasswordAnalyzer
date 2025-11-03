# Makefile for Password Analyzer Project
CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra

# Directories
CLI_DIR = cli
GUI_DIR = gui
COMMON_DIR = common

# Source files
CLI_SRC = $(CLI_DIR)/main.cpp
COMMON_SRC = $(COMMON_DIR)/PasswordAnalyzer.cpp

# Output executables
CLI_TARGET = password_cli

.PHONY: all clean cli

all: cli

cli: $(CLI_TARGET)

$(CLI_TARGET): $(CLI_SRC) $(COMMON_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(CLI_TARGET)

# Run the CLI application
run: cli
	./$(CLI_TARGET)
