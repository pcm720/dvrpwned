# DVRPwned Test Application

A test application for validating DVRPwned functionality.  
Tests custom CFW functions and serves as an example for using CFW devctl operations in your homebrew.

## Overview

Tests:
- LBA48 support for accessing large hard disk drives (>137GB)
- Custom devctl commands for extended functionality, including memory peek/poke operations

## Building

To build the test application, you'll need to install PS2SDK and run
```bash
make
```

## Usage

Load the ELF file on a PSX with DVRPwned installed.
To get debug logs via IOP console, run it via PS2Link.
