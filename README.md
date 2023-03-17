# cschc

## In a nutshell

C implementation of Static Context Header Compression (SCHC), RFC 8724

## License

MIT License, Copyright (c) 2022 Orange, by Quentin Lampin

## Important Status: Development in progress

This library is Work in Progress, do not use it in production.

## Setting the dev environment using conda

```bash
conda env create -f dev/conda.yml
```

## Build variants

- Build for debubbing & testing:

```bash
cmake -S . -B build-test -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=On;
cmake --build build-test --verbose;
ctest --test-dir build-test;
```

- Build for release with debugging symbols:

```bash
cmake -S. -B build-release-debug -DCMAKE_BUILD_TYPE=RelWithDebInfo; 
cmake --build build-release-debug --verbose;
```

- Build for release:

```bash
cmake -S. -B build-release -DCMAKE_BUILD_TYPE=Release; 
cmake --build build-release --verbose;
```

## Note for Darwin users (macOS)

For debugging and testing, it is recommended to build using the LLVM toolchain provided by Homebrew 
as Apple LLVM does not include sanitizers for debugging memory leaks.

To do so:

- Install Homebrew:

```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

- Install LLVM:

```bash
brew install llvm
```

- configure build using the homebrew cmake toolchain:

```bash
cmake -S . -B build-test -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE="darwin/homebrew-llvm.cmake" -DBUILD_TESTING=On;
```

- build as usual:

```bash
cmake --build build-test --verbose;
```

- test using ASAN and LSAN options

```bash
ASAN_OPTIONS=detect_leaks=1 LSAN_OPTIONS="suppressions=../darwin/darwin.lsan" ctest --test-dir ./build-test/ --output-on-failure
```
