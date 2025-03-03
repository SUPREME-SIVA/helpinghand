# Executables Directory

This directory contains all the compiled executables for the HelpingHand C and C++ project.

## Contents

This directory is populated automatically by running `make install` from the project root.

## Executables

The directory contains:
- C program executables (compiled from .c files)
- C++ program executables (compiled from .cpp files)

## Usage

You can run any executable directly from this directory:

```bash
# Run from project root
./bin/addition          # C program
./bin/shared_ptr_demo   # C++ program

# Or navigate to bin directory first
cd bin
./addition
./shared_ptr_demo
```

## Maintenance

- To build and install all executables: `make install`
- To build and install only C executables: `make c-execs && make install`
- To build and install only C++ executables: `make cxx-execs && make install`
- To clean this directory: `make clean`

Note: This directory and its contents are excluded from Git version control.
