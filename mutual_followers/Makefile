CXX        := g++
CXXFLAGS   := -std=c++17 -Wall -Wextra -O2
INC_DIR    := include
LIB_DIR    := lib
BIN_DIR    := bin
LIB_NAME   := libxml_parser_lib.a

SRCS       := src/main.cpp
OBJS       := $(SRCS:.cpp=.o)
TARGET     := $(BIN_DIR)/mutual_followers_app

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS) $(LIB_DIR)/$(LIB_NAME)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OBJS) -I$(INC_DIR) -L$(LIB_DIR) -lxml_parser_lib -o $@
	@echo ">>> Built $(TARGET) successfully!"

$(OBJS): %.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

$(LIB_DIR)/$(LIB_NAME):
	@echo "Error: Static library not found in lib/."
	@exit 1

clean:
	rm -f $(OBJS) $(TARGET)
	@echo ">>> Cleaned build artifacts."
