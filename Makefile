CXX := g++
CXXFLAGS := -std=c++20 -O2
LDFLAGS :=

TOP_DIR := ./cppshell
COMMANDS_DIR := $(TOP_DIR)/cmds
BIN_DIR := $(TOP_DIR)/bin
MAIN := $(TOP_DIR)/shell

# Get all commands files and binaries
COMMAND_FILES := $(wildcard $(COMMANDS_DIR)/*.cpp)
COMMAND_BIN := $(patsubst $(COMMANDS_DIR)/%.cpp,$(BIN_DIR)/%,$(COMMAND_FILES))

.PHONY: all cmds clean main shell

all: cmds main

cmds: $(COMMAND_BIN)

$(BIN_DIR)/%: $(COMMANDS_DIR)/%.cpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

main: cmds
	$(CXX) $(CXXFLAGS) $(MAIN)/$@.cpp -o $(BIN_DIR)/$@ $(LDFLAGS)

shell: main
	$(BIN_DIR)/main

clean:
	rm -rf $(BIN_DIR)
