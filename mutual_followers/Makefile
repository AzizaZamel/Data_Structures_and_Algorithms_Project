CXX        := g++
CXXFLAGS   := -std=c++17 -Wall -Wextra -O2
INC_DIR    := include
LIB_DIR    := lib
BIN_DIR    := bin
BUILD_DIR  := build
LIB_NAME   := libxml_parser_lib.a

SRCS       := $(wildcard src/*.cpp)
OBJS       := $(patsubst src/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))
TARGET     := $(BIN_DIR)/mutual_followers_app

.PHONY: all clean lib rebuild_lib run help

all: $(TARGET)

# Build target executable
$(TARGET): $(OBJS) $(LIB_DIR)/$(LIB_NAME)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OBJS) -I$(INC_DIR) -L$(LIB_DIR) -lxml_parser_lib -o $@
	@echo "------------------------------------------------------"
	@echo ">>> Built $(TARGET) successfully! <<<"
	@echo "------------------------------------------------------"

# Compile all .cpp files into object files
$(BUILD_DIR)/%.o: src/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@
	@echo ">>> Compiled $< -> $@"

# Clean build artifacts
clean:
	rm -f $(OBJS) $(TARGET)
	rm -rf $(BUILD_DIR)
	@echo "----------------------------------"
	@echo ">>> Cleaned build artifacts. <<<"
	@echo "----------------------------------"

# Help target to display available commands
help:
	@echo "Available targets in this Makefile:"
	@echo "  all        - Build the project and generate the executable"
	@echo "  clean      - Remove build artifacts and compiled files"
	@echo "  help       - Display this help message"
	@echo ""
	@echo "Usage:"
	@echo "  make [target]"
	@echo "Example:"
	@echo "  make all       (Build the project)"
	@echo "  make clean     (Clean build artifacts)"
