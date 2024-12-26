# XML Consistency Checker

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Project Structure](#project-structure)
- [Installation and Setup](#installation-and-setup)
  - [Clone the Repository](#clone-the-repository)
  - [Build the Project](#build-the-project)
- [Usage](#usage)
  - [Basic XML Validation](#basic-xml-validation)
  - [Validate and Auto-Fix XML](#validate-and-auto-fix-xml)
- [Example Workflow](#example-workflow)
- [How It Works](#how-it-works)
  - [Core Components](#core-components)
- [Testing](#testing)
- [Contributing](#contributing)
- [License](#license)

---

## Overview

The **XML Consistency Checker** is a C++ application designed to validate and repair XML files. It detects common XML syntax errors such as:

- Mismatched tags
- Missing closing tags
- Unexpected tags  
  Additionally, the tool can automatically fix these issues and generate a corrected version of the XML file.

---

## Features

- **Consistency Check**:  
  Detects and reports XML syntax errors with precise line and column numbers.
- **Auto-fix Capability**:  
  Fixes common XML errors and outputs a corrected version.
- **Command Line Interface**:  
  Simple and flexible command-line usage.

---

## Project Structure

```
.
├── examples/                              # XML examples for testing
│   ├── edge_case_only_comments.xml        # Example with only comments
│   ├── invalid_mismatched_tags.xml        # Example with mismatched tags
│   ├── invalid_missing_closing_tag.xml    # Example with missing closing tag
│   ├── invalid_multiple_errors.xml        # Example with multiple issues
│   ├── invalid_unexpected_closing_tag.xml # Example with unexpected closing tag
│   └── valid_example.xml                  # Valid XML example
├── include/
│   └── xml_checker.hpp                    # XmlChecker class header
├── src/
│   ├── main.cpp                           # Main entry point
│   └── xml_checker.cpp                    # XmlChecker implementation
├── tests/
│   └── test_xml_checker.cpp               # Unit tests for XML checking
├── Makefile                               # Build instructions
├── output.xml                             # Sample output of fixed XML
└── README.md                              # Project documentation
```

---

## Installation and Setup

### Clone the Repository

```bash
git clone <repository-url>
cd <repository-name>
```

### Build the Project

```bash
make
```

---

## Usage

### Basic XML Validation

To check the consistency of an XML file:

```bash
./xml_checker <path-to-xml-file>
```

**Example**:

```bash
./xml_checker examples/invalid_mismatched_tags.xml
```

### Validate and Auto-Fix XML

To validate and fix the XML, use the `-f` (fix) flag and specify an output path with `-o`:

```bash
./xml_checker <path-to-xml-file> -f -o <output-path>
```

**Example**:

```bash
./xml_checker examples/invalid_missing_closing_tag.xml -f -o fixed_output.xml
```

**Output**:

```
Fixed XML written to: fixed_output.xml
```

---

## Example Workflow

### Input (Invalid XML)

```xml
<root>
    <child>
    <child2>
</root>
```

### Output (Validation)

```
Error (Line 3, Column 5): Unclosed tag <child2>
Error (Line 3, Column 1): Unclosed tag <child>
invalid
Total Errors: 2
```

### Output (Fixed XML)

```xml
<root>
    <child>
    <child2></child2>
    </child>
</root>
```

---

## How It Works

### Core Components

- **`xml_checker.hpp`**:  
  Defines the `XmlChecker` class and its core functions:

  - `checkConsistency()`: Validates XML syntax and reports errors.
  - `fixXml()`: Fixes detected errors and generates corrected XML.

- **`xml_checker.cpp`**:  
  Implements the logic for XML validation and correction.

- **`main.cpp`**:  
  Parses command-line arguments, triggers XML validation, and optionally fixes XML if requested.

---

## Testing

Run unit tests to ensure the tool works correctly:

```bash
make test
./test_xml_checker
```

---

## Contributing

1. **Fork the repository**.
2. **Create a new branch** (`feature/fix-issue`).
3. **Commit your changes**.
4. **Submit a pull request**.

---

## License

This project is licensed under the [MIT License](LICENSE).
