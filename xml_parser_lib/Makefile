########################################
# Simple Root Makefile for xml_parser_lib
# with a variable EXAMPLE for choosing the example,
# and a default set of examples otherwise
########################################

CXX        := g++
CXXFLAGS   := -std=c++17 -Wall -Wextra -O2 -fPIC

SRC_DIR    := src
INC_DIR    := include
BUILD_DIR  := build
LIB_DIR    := lib
LIB_NAME   := libxml_parser_lib.a

# Gather library .cpp in src/
LIB_SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
LIB_OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(LIB_SOURCES))

# DEFAULT_EXAMPLES: which examples to build if EXAMPLE is not set
DEFAULT_EXAMPLES := sample_app network_influencer_app

.PHONY: all clean library apps help

###########################
# Default: library + apps
###########################
all: library apps
	@echo "----------------------------------------------------"
	@echo ">>> All targets have been built successfully. <<<"
	@echo "----------------------------------------------------"

###########################################
# 1) Build the static library
###########################################
library: $(LIB_DIR)/$(LIB_NAME)

$(LIB_DIR)/$(LIB_NAME): $(LIB_OBJECTS)
	@mkdir -p $(LIB_DIR)
	ar rcs $@ $^
	@echo "----------------------------------------------------------"
	@echo ">>> Built library $(LIB_NAME) successfully. <<<"
	@echo "----------------------------------------------------------"

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(INC_DIR)/*.hpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

###########################################
# 2) Build examples (either a single EXAMPLE or defaults)
###########################################
apps:
	@if [ -z "$(EXAMPLE)" ]; then \
	   echo "No EXAMPLE specified; building default examples: $(DEFAULT_EXAMPLES)."; \
	   for exdir in $(DEFAULT_EXAMPLES); do \
	      if [ -d examples/$$exdir ] && [ -f examples/$$exdir/Makefile ]; then \
	         echo "=> Building $$exdir"; \
	         $(MAKE) -C examples/$$exdir; \
			 echo "----------------------------------------------------"; \
	         echo ">>> Built $$exdir successfully. <<<"; \
			 echo "----------------------------------------------------"; \
	      else \
	         echo "Skipping $$exdir: no Makefile found."; \
	      fi; \
	   done; \
	else \
	   echo "EXAMPLE=$(EXAMPLE) => building that single subdir."; \
	   if [ -d examples/$(EXAMPLE) ] && [ -f examples/$(EXAMPLE)/Makefile ]; then \
	      echo "=> Building $(EXAMPLE)"; \
	      $(MAKE) -C examples/$(EXAMPLE); \
		  echo "----------------------------------------------------"; \
	      echo ">>> Built $(EXAMPLE) successfully. <<<"; \
		  echo "----------------------------------------------------"; \
	   else \
	      echo "Skipping $(EXAMPLE): subdir or Makefile not found."; \
	   fi; \
	fi
	@echo ">>> Done building apps."

###########################################
# 3) Clean library + each example
###########################################
clean:
	rm -rf $(BUILD_DIR) $(LIB_DIR)
	@echo "Removed library build artifacts."
	@for d in $(shell ls -d examples/*/ 2>/dev/null); do \
	   if [ -f $$d/Makefile ]; then \
	      echo "=> Cleaning $$d"; \
	      $(MAKE) -C $$d clean; \
	   fi; \
	 done
	@echo "----------------------------------------------------"
	@echo ">>> Cleaned everything successfully. <<<"
	@echo "----------------------------------------------------"
########
# HELP #
########
help:
	@echo "================================================="
	@echo "   XML Parser Lib - Root Makefile (EXAMPLE var)  "
	@echo "================================================="
	@echo "Targets:"
	@echo "  all       : Build library + apps. If EXAMPLE is unset, builds default examples."
	@echo "  library   : Build only the static library ($(LIB_NAME))."
	@echo "  apps      : Build either the single EXAMPLE subdir or default set."
	@echo "  clean     : Remove library artifacts + call 'clean' in each example subdir."
	@echo "  help      : Show this help info."
	@echo
	@echo "Variables:"
	@echo "  EXAMPLE   : If set to a subdir name (e.g. 'sample_app'), build only that subdir."
	@echo "              If unset, build the default set: $(DEFAULT_EXAMPLES)."
	@echo
	@echo "Usage Examples:"
	@echo "  make                   # library + default examples ($(DEFAULT_EXAMPLES))"
	@echo "  make EXAMPLE=util_app  # library + only 'util_app'"
	@echo "  make clean             # remove library + examples' build artifacts"
	@echo "  make help              # show this help"
	@echo "================================================="
