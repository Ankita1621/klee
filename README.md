KLEE Symbolic Execution Engine
==============================

[![Build Status](https://github.com/klee/klee/workflows/CI/badge.svg)](https://github.com/klee/klee/actions?query=workflow%3ACI)
[![Build Status](https://api.cirrus-ci.com/github/klee/klee.svg)](https://cirrus-ci.com/github/klee/klee)
[![Coverage](https://codecov.io/gh/klee/klee/branch/master/graph/badge.svg)](https://codecov.io/gh/klee/klee)

KLEE is a symbolic execution engine built on top of the LLVM compiler infrastructure. It is capable of automatically generating test cases for C programs that achieve high code coverage.

This guide includes:

- Steps to install KLEE
- How to run code with KLEE
- Types of C programs KLEE can test
- Limitations on what cannot be tested

---

## 🛠 Installation Steps

### 1. Clone and run the install script

Create a script file named `install.sh`:

```bash
chmod +x install.sh
./install.sh

```
### Paste the following in install.sh:

```bash
#!/bin/bash

set -e

# Update and install dependencies
sudo apt update
sudo apt install -y build-essential cmake python3 zlib1g-dev \
  llvm-14 llvm-14-dev llvm-14-tools clang-14 libclang-14-dev \
  libcap-dev git curl

# Clone and build STP
git clone https://github.com/stp/stp.git
cd stp
mkdir build && cd build
cmake .. && make -j$(nproc) && sudo make install
cd ../..
rm -rf stp

# Clone and build klee-uclibc
git clone https://github.com/klee/klee-uclibc.git
cd klee-uclibc
./configure --make-llvm-lib
make -j$(nproc)
cd ..

# Optional: Install Z3 Solver
sudo apt install -y z3 libz3-dev

# Clone and build KLEE
git clone https://github.com/klee/klee.git
cd klee
mkdir build && cd build

cmake .. -DENABLE_SOLVER_STP=ON \
         -DENABLE_POSIX_RUNTIME=ON \
         -DKLEE_UCLIBC_PATH=../../klee-uclibc \
         -DLLVM_CONFIG_BINARY=/usr/bin/llvm-config-14 \
         -DENABLE_KLEE_ASSERTS=ON

make -j$(nproc)

# Verify installation
./bin/klee --version
```
▶️ How to Run Code with KLEE

1. Write a C program using KLEE's symbolic functions:
 ```bash
#include <klee/klee.h>
#include <stdio.h>

int main() {
  int x;
  klee_make_symbolic(&x, sizeof(x), "x");
  if (x == 42) {
    printf("Hit 42!\n");
    klee_assert(0);
  }
  return 0;
}
```
2.Compile the C program to LLVM bitcode using clang:
```bash
clang-14 -I /path/to/klee/include -emit-llvm -c -g your_file.c -o your_file.bc
```
3.Run KLEE on the bitcode file:
```bash
/path/to/klee/build/bin/klee your_file.bc
```
4.View output files in the klee-out-* directory:
```bash
ktest-tool klee-out-0/test000001.ktest
```
Use ktest-tool klee-out-0/test000001.ktest to inspect symbolic inputs

✅ C Programs You Can Test with KLEE

KLEE works best with simple, deterministic C programs. It supports:

-Integer and pointer operations

-Branching (if, switch, etc.)

-Loops (if bounded or symbolic input controlled)

-Assertion testing (klee_assert) and path exploration

-Programs with symbolic inputs (klee_make_symbolic)

-Assumptions using klee_assume

✔ Example: Test Case Generation
```bash
#include <klee/klee.h>

int main() {
    int x;
    klee_make_symbolic(&x, sizeof(x), "x");
    if (x > 10)
        klee_assert(0);
    return 0;
}
```
❌ What KLEE Cannot Test (Limitations)

-Dynamic memory allocation beyond simple malloc patterns

-System calls (e.g., file I/O, threads, sockets) are unsupported unless using KLEE POSIX runtime

-Floating point arithmetic (limited support)

-Concurrency or multithreading programs

-Programs that rely on external libraries or environment-specific behavior

⚠️ Example (NOT Recommended):
```bash
int main() {
    double x = 0.1;  // floating-point
    if (x == 0.1) {
        // unpredictable due to precision
    }
    return 0;
}
```


🧠 Tips

-Use klee_assume() to reduce path explosion

-Always check .stderr and .info files for analysis

-Use small programs to validate symbolic behavior before scaling



For further information, see the [webpage](https://klee-se.org/).
