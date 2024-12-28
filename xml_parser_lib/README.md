# XML Parser Library (`xml_parser_lib`)

A lightweight C++17 library that parses XML **files** or **strings**, letting you navigate and manipulate the resulting XML tree. This guide focuses on **how** to use the library in different **practical scenarios**.

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
6. [Sample XML Files](#sample-xml-files)
7. [Additional Notes](#additional-notes)

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
│   │   └── main.cpp
│   └── mutual_followers_app/
│       └── main.cpp
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

## Sample XML Files

- **`tests/network_arabic_english.xml`**: Arabic girls’ names in English transliteration.
- **`tests/mutual_followers.xml`**: Demonstrates an overlap of followers for multiple IDs.
- **`tests/sample.xml`**: A simple book library example.
