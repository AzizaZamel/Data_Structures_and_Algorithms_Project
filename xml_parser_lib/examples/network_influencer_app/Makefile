CXX       := g++
CXXFLAGS  := -std=c++17 -Wall -Wextra -O2

INC_DIR   := ../../include
LIB_DIR   := ../../lib
BUILD_DIR := ../../build
BIN_DIR   := ../../bin

LIB_NAME  := libxml_parser_lib.a
LIB_PATH  := $(LIB_DIR)/$(LIB_NAME)

SRC := main.cpp
OBJ := $(BUILD_DIR)/network_influencer_main.o
TARGET := $(BIN_DIR)/network_influencer_app

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ) $(LIB_PATH)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJ): $(SRC)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)/network_influencer_main.o $(TARGET)
