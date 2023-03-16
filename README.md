# cschc

## In a nutshell

C implementation of Static Context Header Compression (SCHC), RFC 8724

## License

MIT License, Copyright (c) 2022 Orange, by Quentin Lampin

## Important Status: Development in progress

This library is Work in Progress, do not use it in production.

## Setting the dev environment

```bash
conda env create -f dev-environment.yml
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


