# ===========================
# Compiler and Flags
# ===========================
CC = g++
CFLAGS = -Iinclude -Wall -Wextra -std=c++17

# Debug and Release Flags
DEBUG_FLAGS = -g -DDEBUG
RELEASE_FLAGS = -O2 -DNDEBUG

# ===========================
# Directories
# ===========================
SRCDIR = src
INCDIR = include
OBJDIR = obj
BINDIR = bin
LIBDIR = lib
TESTDIR = tests

# ===========================
# Targets
# ===========================
TARGET = $(BINDIR)/xml_checker
STATIC_LIB = $(LIBDIR)/libxmlchecker.a
DYNAMIC_LIB = $(LIBDIR)/libxmlchecker.so
TEST_TARGET = $(BINDIR)/test_xml_checker

# ===========================
# Source and Object Files
# ===========================
SRC = $(wildcard $(SRCDIR)/*.cpp)
# Exclude main.cpp from SRC for test builds
SRC_NO_MAIN = $(filter-out $(SRCDIR)/main.cpp, $(SRC))
OBJ = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC))
OBJ_NO_MAIN = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC_NO_MAIN))

# Test Source and Object Files
TEST_SRC = $(wildcard $(TESTDIR)/*.cpp)
TEST_OBJ = $(patsubst $(TESTDIR)/%.cpp, $(OBJDIR)/%.o, $(TEST_SRC))

# ===========================
# Build Type
# ===========================
# Default build type is Release
BUILD_TYPE ?= release

ifeq ($(BUILD_TYPE), debug)
    BUILD_FLAGS = $(DEBUG_FLAGS)
    LIB_BUILD_TYPE = debug
else ifeq ($(BUILD_TYPE), release)
    BUILD_FLAGS = $(RELEASE_FLAGS)
    LIB_BUILD_TYPE = release
else
    $(error "Invalid BUILD_TYPE. Use 'debug' or 'release'.")
endif

# ===========================
# Phony Targets
# ===========================
.PHONY: all clean help static dynamic test run_tests

# ===========================
# Default Target
# ===========================
all: $(TARGET)

# ===========================
# Linking the Executable
# ===========================
$(TARGET): $(OBJ)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $(BUILD_FLAGS) $^ -o $@
	@echo "Built executable: $@"

# ===========================
# Compiling Object Files
# ===========================
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) $(BUILD_FLAGS) -c $< -o $@
	@echo "Compiled: $< -> $@"

# ===========================
# Building Static Library
# ===========================
static: $(STATIC_LIB)

$(STATIC_LIB): $(OBJ_NO_MAIN)
	@mkdir -p $(LIBDIR)
	ar rcs $@ $^
	@echo "Built static library: $@"

# ===========================
# Building Dynamic Library
# ===========================
dynamic: $(DYNAMIC_LIB)

$(DYNAMIC_LIB): $(OBJ_NO_MAIN)
	@mkdir -p $(LIBDIR)
	$(CC) -shared -fPIC $(CFLAGS) $(BUILD_FLAGS) -o $@ $^
	@echo "Built dynamic library: $@"

# ===========================
# Compiling Test Object Files
# ===========================
$(OBJDIR)/%.o: $(TESTDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) $(BUILD_FLAGS) -c $< -o $@
	@echo "Compiled test: $< -> $@"

# ===========================
# Linking Test Executable
# ===========================
test: $(TEST_TARGET)

$(TEST_TARGET): $(OBJ_NO_MAIN) $(TEST_OBJ)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $(BUILD_FLAGS) $^ -o $@
	@echo "Built test executable: $@"

# ===========================
# Running Tests
# ===========================
run_tests: test
	@echo "Running tests..."
	@$(BINDIR)/test_xml_checker
	@echo "All tests completed."

# ===========================
# Cleaning Build Artifacts
# ===========================
clean:
	@rm -rf $(OBJDIR) $(BINDIR) $(LIBDIR)
	@echo "Cleaned build artifacts."

# ===========================
# Help Target
# ===========================
help:
	@echo "=========================================="
	@echo "          XML Consistency Checker         "
	@echo "=========================================="
	@echo "Available Targets:"
	@echo "  all               - Build the executable."
	@echo "  static            - Build the static library."
	@echo "  dynamic           - Build the dynamic library."
	@echo "  test              - Build the test executable."
	@echo "  run_tests         - Build and run tests."
	@echo "  clean             - Remove all build artifacts."
	@echo "  help              - Show this help message."
	@echo ""
	@echo "Build Types:"
	@echo "  release           - Optimize the build (default)."
	@echo "  debug             - Include debug information."
	@echo ""
	@echo "Usage Examples:"
	@echo "  make all                      # Builds the executable (Release)."
	@echo "  make all BUILD_TYPE=debug     # Builds the executable (Debug)."
	@echo "  make static                   # Builds the static library (Release)."
	@echo "  make static BUILD_TYPE=debug  # Builds the static library (Debug)."
	@echo "  make dynamic                  # Builds the dynamic library (Release)."
	@echo "  make dynamic BUILD_TYPE=debug # Builds the dynamic library (Debug)."
	@echo "  make test                     # Builds the test executable."
	@echo "  make run_tests                # Builds and runs tests."
	@echo "  make clean                    # Cleans all build files."
	@echo "  make help                     # Displays this help message."
	@echo "=========================================="
