# Mutual Followers Project

This project demonstrates how to **use** the [`xml_parser_lib`](https://github.com/AzizaZamel/Data_Structures_and_Algorithms_Project/tree/main/xml_parser_lib) static library to parse a **network** XML file and find mutual followers among specified user IDs. It accepts a command-line structure like:

```
xml_editor mutual -i <input_file.xml> -ids <comma_separated_ids>
```

to parse the file, gather each user's followers, and output the user IDs (and names) who follow **all** of the specified users.

---

## Project Structure

```
my_mutual_project/
├── include/
│   ├── xml_node.hpp
│   └── xml_parser.hpp
├── lib/
│   └── libxml_parser_lib.a
├── src/
│   └── main.cpp
├── tests/
│   └── sample_network.xml
├── Makefile
└── README.md
```

- **`include/`**: Contains the **headers** (`xml_node.hpp`, `xml_parser.hpp`) copied from the `xml_parser_lib` project.
- **`lib/`**: Contains the **static library** (`libxml_parser_lib.a`) compiled from `xml_parser_lib`.
- **`src/`**: Holds the source code for our mutual-follower application (`main.cpp`).
- **`tests/`**: Stores a sample network XML file (e.g., `sample_network.xml`) for testing.
- **`Makefile`**: Builds the project, linking against `libxml_parser_lib.a`.

---

## Requirements

- **C++17** or later.
- A compatible compiler (e.g., **GCC** or **Clang**).
- The static library **`libxml_parser_lib.a`** (already built in your `xml_parser_lib` project).
- Copied **`xml_parser.hpp`** and **`xml_node.hpp`** headers into `include/`.

---

## Building

From the project root (`my_mutual_project/`), run:

```bash
make
```

The Makefile:

1. Compiles `src/main.cpp` to object files.
2. Links them with `libxml_parser_lib.a`.
3. Produces an executable in `bin/mutual_followers_app`.

---

## Usage

After building, run:

```bash
./bin/mutual_followers_app mutual -i tests/sample_network.xml -ids 1,2,3
```

**Explanation**:

1. **`mutual`**: The command specifying we want mutual followers mode.
2. **`-i tests/sample_network.xml`**: The path to your network XML file.
3. **`-ids 1,2,3`**: A comma-separated list of user IDs. The application finds which user(s) follow **all** of these IDs.

### Sample Output

```
Mutual followers of users 1,2,3:
  Sara (ID=3)
  Aisha (ID=4)
```

_(Varies depending on your sample XML content.)_

---

## XML Format

Each `<user>` element has:

- An `id` attribute (unique integer).
- A `name` attribute (user’s name).
- A child `<followers>` block containing multiple `<follower id="X"/>` elements, indicating user **X** follows this user.

Example:

```xml
<network>
  <user id="1" name="Layla">
    <followers>
      <follower id="2"/>
      <follower id="3"/>
    </followers>
  </user>
  <!-- More <user> elements... -->
</network>
```

---

## Code Explanation

In **`main.cpp`**:

1. **Parse Command Line**: Looks for `mutual`, `-i <file>`, and `-ids <IDs>`.
2. **Split IDs**: Divides the comma list (e.g., `1,2,3`) into integers.
3. **Parse XML**: Uses `xml_parser_lib::xml_parser` from the static library.
4. **Build Map**: Maps each user ID to a list of follower IDs.
5. **Intersect**: Finds the set intersection of all followers’ sets for the specified user IDs.
6. **Print**: Outputs matching user IDs/names or says “No mutual followers found.”

---

## Makefile Reference

```makefile
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
```

**Key Points**:

- **`-I$(INC_DIR)`**: Points to the headers (e.g., `xml_parser.hpp`, `xml_node.hpp`).
- **`-L$(LIB_DIR)`** and **`-lxml_parser_lib`**: Link to `libxml_parser_lib.a`.
- Produces **`mutual_followers_app`** under `bin/`.

---

## Additional Notes

- **Extend the Parser**: If your XML includes more advanced features (comments, CDATA, etc.), consider customizing `xml_parser.cpp` in the `xml_parser_lib` project and rebuilding `libxml_parser_lib.a`.
- **Thread Safety**: The current parser is not thread-safe by default. If you need concurrent parsing, wrap calls in synchronization mechanisms.
- **Error Handling**: If an XML is malformed or missing attributes, the parser throws exceptions or logs warnings. Ensure you handle them gracefully.
- **Performance**: For extremely large networks, you might want streaming or partial loading strategies.
