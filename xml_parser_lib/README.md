# XML Parser Library (`xml_parser_lib`)

A lightweight C++17 library that parses XML **files** or **strings**, letting you navigate and manipulate the resulting XML tree. This guide focuses on **how** to use the library in different **practical scenarios**—from parsing basic XML content, to analyzing social network data, to integrating the library as a **static library** in your own project.

---

## Table of Contents

1. [Overview](#overview)
2. [Scenarios and Usage](#scenarios-and-usage)
   - [1. Parsing an XML File](#1-parsing-an-xml-file)
   - [2. Parsing an XML String](#2-parsing-an-xml-string)
   - [3. Finding the Most Influential User](#3-finding-the-most-influential-user)
   - [4. Finding Mutual Followers Among N Users](#4-finding-mutual-followers-among-n-users)
3. [API Summary](#api-summary)
4. [Project Structure](#project-structure)
5. [Build and Integration](#build-and-integration)
   - [Root Makefile](#root-makefile)
   - [Using `xml_parser_lib` as a Static Library in Your Project](#using-xml_parser_lib-as-a-static-library-in-your-project)
     1. [Building the Static Library](#1-building-the-static-library)
     2. [Integrating the Library into Your Project](#2-integrating-the-library-into-your-project)
     3. [Copy the Required Files](#3-copy-the-required-files)
     4. [Sample `main.cpp` (Your Project Source)](#4-sample-maincpp-your-project-source)
     5. [Compiling and Linking with CLI (Direct Command Line)](#5-compiling-and-linking-with-cli-direct-command-line)
     6. [Writing a Makefile for Your Project (Automated Build)](#6-writing-a-makefile-for-your-project-automated-build)
     7. [Building and Running Your Project](#7-building-and-running-your-project)
     8. [Testing the Static Library Integration](#8-testing-the-static-library-integration)
     9. [Benefits of Using Static Libraries](#9-benefits-of-using-static-libraries)
     10. [Additional Notes](#10-additional-notes)
6. [Sample XML Files](#sample-xml-files)
7. [Additional Notes](#additional-notes-1)

---

## Overview

**Key Points**:

- **Simple**: Minimal classes, straightforward methods.
- **Flexible**: Parse from strings or disk files.
- **Extensible**: Example applications show how to adapt for specialized tasks like analyzing a social network or printing a library’s book data.

---

## Scenarios and Usage

### 1. Parsing an XML File

If you have an XML on **disk** (e.g., `tests/sample.xml`) and want to load its root node:

```cpp
#include <iostream>
#include <memory>
#include "xml_parser.hpp"
#include "xml_node.hpp"

int main() {
    xml_parser_lib::xml_parser parser;
    try {
        // parse_from_file returns the root xml_node
        auto root = parser.parse_from_file("tests/sample.xml");
        if (root) {
            std::cout << "XML parsed successfully!\n";
            root->print(); // display the structure
        } else {
            std::cerr << "Failed to parse XML.\n";
        }
    } catch (const std::exception &ex) {
        std::cerr << "Error: " << ex.what() << "\n";
    }
    return 0;
}
```

**Scenario**:

- Config files
- Book library data
- Any typical XML file usage

**Result**: You get a **root** `xml_node`, from which you can call `get_name()`, `get_children()`, etc.

---

### 2. Parsing an XML String

If the XML content is **in-memory** rather than on disk, or you need to parse quickly from a network request:

```cpp
#include <iostream>
#include <memory>
#include "xml_parser.hpp"
#include "xml_node.hpp"

int main() {
    std::string xml_content = R"(
        <root>
            <greeting>Hello, world!</greeting>
            <number value="42" />
        </root>
    )";

    xml_parser_lib::xml_parser parser;
    try {
        auto root = parser.parse_from_string(xml_content);
        if (root) {
            std::cout << "Parsed from string!\n";
            root->print();
        }
    } catch (const std::exception &ex) {
        std::cerr << "Error: " << ex.what() << "\n";
    }
    return 0;
}
```

**Scenario**:

- Data arrives from an **API** response as XML text.
- Configuration stored in a string for quick parse.

---

### 3. Finding the Most Influential User

**Command**:

```
xml_editor most_influencer -i input_file.xml
```

**Scenario**: In a social network, a user’s “influence” might be the number of followers. We parse `<network>` → `<user>` elements, each with `<followers>` listing `<follower id="X"/>`. The user with the **largest** follower count is the “influencer.”

```cpp
// Pseudocode snippet
int main(int argc, char* argv[]) {
    // Expect: argv[1] = "most_influencer", argv[2] = "-i", argv[3] = "file.xml"
    // Parse file -> build user_map -> find user with max followers.
    // Print ID, name, follower count.
}
```

**Result**:

```
Most Influencer User:
  ID: 3
  Name: Malak
  Followers Count: 5
```

---

### 4. Finding Mutual Followers Among N Users

**Command**:

```
xml_editor mutual -i input_file.xml -ids 1,2,3
```

**Scenario**: We have multiple user IDs. Each user has a list of followers. We want to find the **intersection** of these lists—i.e., which user(s) follow all of the listed IDs.

**Approach**:

1. Parse `<network>`.
2. For each user ID in `1,2,3`, gather its followers into a `std::set<int>`.
3. Compute **set intersection** across them all.
4. Print the resulting IDs and names.

**Result**:

```
Mutual followers of users 1,2,3:
  Noor (ID=2)
  Aisha (ID=4)
```

---

## API Summary

### `xml_node`

- **Methods**:
  - `get_name()`, `get_text()`, `get_attributes()`, `get_children()`
  - `set_name(...)`, `set_text(...)`, `add_attribute(...)`, `add_child(...)`
  - `print(...)`

**Use**:

- After parsing, a `<network>` or `<library>` etc. is your **root** node.
- Recursively navigate child nodes to find `<user>`, `<followers>`, `<book>`, etc.

### `xml_parser`

- **Methods**:
  - `parse_from_file(filename) -> root_node`
  - `parse_from_string(xml_content) -> root_node`

**Use**:

- Quickly parse either from **disk** or from **memory**.
- On success, you get a shared pointer to the top-level node.

---

## Project Structure

```
xml_parser_lib/
├── include/
│   ├── xml_node.hpp
│   └── xml_parser.hpp
├── src/
│   ├── xml_node.cpp
│   └── xml_parser.cpp
├── lib/
│   └── libxml_parser_lib.a
├── bin/
│   ├── network_influencer_app
│   └── mutual_followers_app
├── examples/
│   ├── network_influencer_app/
│   │   ├── main.cpp
│   │   └── Makefile
│   └── mutual_followers_app/
│       ├── main.cpp
│       └── Makefile
├── tests/
│   ├── network_arabic_english.xml
│   └── mutual_followers.xml
├── Makefile
└── README.md
```

---

## Build and Integration

1. **Root Makefile** builds the library + example apps:

   ```bash
   make
   ```

   Produces **`lib/libxml_parser_lib.a`** and executables in `bin/`.

2. **Local Makefiles** in each `examples/<app>/` directory link against the library.
3. To **integrate** into your own project, copy `include/` + `src/` or just `libxml_parser_lib.a` and `include/` if you want to link the static library.

---

### Root Makefile

Below is the main Makefile located at the root of the project (`xml_parser_lib/Makefile`), used to build the core library and all example applications:

```makefile
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
```

---

### Using `xml_parser_lib` as a Static Library in Your Project

You can easily integrate `xml_parser_lib` into your C++ projects as a **static library**. This section covers how to build, link, and use the library to simplify XML parsing in your own applications.

---

#### 1. Building the Static Library

1. **Navigate to the Project Directory**:

```bash
cd path/to/xml_parser_lib
```

2. **Build the Static Library**:

```bash
make library
```

3. **Result**:

- The compiled static library will be located at:

```
lib/libxml_parser_lib.a
```

- Headers needed for integration are in the `include/` directory.

---

#### 2. Integrating the Library into Your Project

**Project Structure Example**:

```
my_project/
├── include/
│   ├── xml_node.hpp
│   └── xml_parser.hpp
├── lib/
│   └── libxml_parser_lib.a
├── src/
│   └── main.cpp
├── Makefile
└── ...
```

---

#### 3. Copy the Required Files

- **Headers**: Copy `include/xml_parser.hpp` and `include/xml_node.hpp` to your project’s `include/` directory.
- **Static Library**: Copy `lib/libxml_parser_lib.a` to your project’s `lib/` directory.

---

#### 4. Sample `main.cpp` (Your Project Source)

```cpp
#include <iostream>
#include <memory>
#include "xml_parser.hpp"
#include "xml_node.hpp"

int main() {
    xml_parser_lib::xml_parser parser;

    try {
        auto root = parser.parse_from_file("tests/sample.xml");
        if (root) {
            std::cout << "XML parsed successfully!\n";
            root->print();
        } else {
            std::cerr << "Failed to parse XML.\n";
        }
    } catch (const std::exception &ex) {
        std::cerr << "Error: " << ex.what() << "\n";
    }
    return 0;
}
```

---

#### 5. Compiling and Linking with CLI (Direct Command Line)

Instead of writing a Makefile, you can **compile and link directly** using the CLI:

```bash
g++ -std=c++17 -Wall -Wextra -Iinclude src/main.cpp -Llib -lxml_parser_lib -o bin/xml_demo
```

**Explanation**:

- **`-std=c++17`** – Use C++17 standard.
- **`-Iinclude`** – Include path for headers.
- **`src/main.cpp`** – The source file to compile.
- **`-Llib`** – Path to the directory containing the static library.
- **`-lxml_parser_lib`** – Link the static library (`libxml_parser_lib.a`).
- **`-o bin/xml_demo`** – Output executable.

---

#### 6. Writing a Makefile for Your Project (Automated Build)

Below is a sample **`Makefile`** that automates the build process:

```makefile
# Project Makefile (my_project)

CXX       := g++
CXXFLAGS  := -std=c++17 -Wall -Wextra -O2
INC_DIR   := include
LIB_DIR   := lib
LIB_NAME  := libxml_parser_lib.a
BIN_DIR   := bin

SRCS      := src/main.cpp
OBJS      := $(SRCS:.cpp=.o)
TARGET    := $(BIN_DIR)/xml_demo

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
	@echo "Error: Static library not found. Please ensure libxml_parser_lib.a exists in lib/"
	@exit 1

clean:
	rm -f $(OBJS) $(TARGET)
	@echo ">>> Cleaned build artifacts."
```

---

#### 7. Building and Running Your Project

1. **Build**:

```bash
make
```

2. **Run**:

```bash
./bin/xml_demo
```

---

#### 8. Testing the Static Library Integration

1. Create a test XML file (`tests/sample.xml`):

```xml
<root>
    <message>Hello, XML!</message>
</root>
```

2. Re-run the application to ensure it parses the file successfully:

```bash
./bin/xml_demo
```

**Expected Output**:

```
XML parsed successfully!
<root>
  <message>Hello, XML!</message>
</root>
```

---

#### 9. Benefits of Using Static Libraries

- **Portability**: Link the static library once, and distribute the binary without requiring dynamic dependencies.
- **Performance**: Faster execution since the entire codebase is linked at compile time.
- **Security**: Reduces dependency-related runtime errors.

---

#### 10. Additional Notes

- **Updating the Library**:  
   If the core library is updated, simply rebuild `libxml_parser_lib.a` by running:
  ```bash
  make library
  ```
- **Customizing the Parser**: Extend `xml_parser` or `xml_node` by modifying the `src/` files and rebuilding the static library.
- **Thread Safety**: Currently, the library is not thread-safe. If needed, wrap parsing logic in mutexes for concurrent parsing.

---

## Sample XML Files

### `tests/network_arabic_english.xml`

```xml
<!-- File: tests/network_arabic_english.xml -->

<network>
    <!-- Huda (id=1) -->
    <user id="1" name="Huda">
        <followers>
            <!-- Aya (2), Malak (3) follow Huda (1) -->
            <follower id="2" />
            <follower id="3" />
        </followers>
    </user>

    <!-- Aya (id=2) -->
    <user id="2" name="Aya">
        <followers>
            <!-- Malak (3), Reem (5) follow Aya (2) -->
            <follower id="3" />
            <follower id="5" />
        </followers>
    </user>

    <!-- Malak (id=3) -->
    <user id="3" name="Malak">
        <followers>
            <!-- Huda (1), Fatima (4) follow Malak (3) -->
            <follower id="1" />
            <follower id="4" />
        </followers>
    </user>

    <!-- Fatima (id=4) -->
    <user id="4" name="Fatima">
        <followers>
            <!-- Aya (2) follows Fatima (4) -->
            <follower id="2" />
        </followers>
    </user>

    <!-- Reem (id=5) -->
    <user id="5" name="Reem">
        <followers>
            <!-- Malak (3), Fatima (4), Rana (6) follow Reem (5) -->
            <follower id="3" />
            <follower id="4" />
            <follower id="6" />
        </followers>
    </user>

    <!-- Rana (id=6) -->
    <user id="6" name="Rana">
        <followers>
            <!-- Huda (1) and Reem (5) follow Rana (6) -->
            <follower id="1" />
            <follower id="5" />
        </followers>
    </user>
</network>
```

### `tests/mutual_followers.xml`

```xml
<!-- File: tests/mutual_followers.xml -->

<network>
    <!-- Layla (id=1) -->
    <user id="1" name="Layla">
        <followers>
            <follower id="2" />
            <follower id="3" />
            <follower id="4" />
        </followers>
    </user>

    <!-- Noor (id=2) -->
    <user id="2" name="Noor">
        <followers>
            <follower id="1" />
            <follower id="3" />
            <follower id="5" />
        </followers>
    </user>

    <!-- Sara (id=3) -->
    <user id="3" name="Sarah">
        <followers>
            <follower id="1" />
            <follower id="2" />
            <follower id="4" />
        </followers>
    </user>

    <!-- Aisha (id=4) -->
    <user id="4" name="Aisha">
        <followers>
            <follower id="2" />
            <follower id="3" />
        </followers>
    </user>

    <!-- Dina (id=5) -->
    <user id="5" name="Dina">
        <followers>
            <follower id="1" />
            <follower id="3" />
        </followers>
    </user>
</network>
```

### `tests/sample.xml`

```xml
<!-- File: tests/sample.xml -->

<library>
    <book id="1" title="The C++ Programming Language">
        <author>Bjarne Stroustrup</author>
        <year>2013</year>
        <genres>
            <genre>Programming</genre>
            <genre>Software Engineering</genre>
        </genres>
    </book>
    <book id="2" title="Effective Modern C++">
        <author>Scott Meyers</author>
        <year>2014</year>
        <genres>
            <genre>Programming</genre>
        </genres>
    </book>
    <book id="3" title="Clean Code">
        <author>Robert C. Martin</author>
        <year>2008</year>
        <genres>
            <genre>Programming</genre>
            <genre>Software Engineering</genre>
        </genres>
    </book>
</library>
```

---

## Additional Notes

- **Error Handling**:  
  If you encounter **`Error: Expected tag name at position X`**, it might be due to a **BOM** or unexpected leading characters in your XML file. Ensure your XML files are properly formatted without hidden characters or that you skip leading whitespace.

- **Extending the Parser**:  
  The parser currently handles basic XML elements, attributes, and text. You can extend `xml_parser` to handle comments, CDATA sections, or processing instructions if your use case demands it.

- **Performance Considerations**:  
  For extremely large XML files, consider streaming or partial parsing techniques to avoid excessive memory usage.

- **Thread Safety**:  
  The library is not thread-safe by default. If needed, wrap parsing logic in appropriate synchronization mechanisms.

- **Integration**:
  By following the instructions above, you can seamlessly include `xml_parser_lib` in any C++17 project, either by referencing the static library or by copying relevant source/include files.
