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
cmake -S. -Bbuild -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=On;
cmake --build build --verbose;
ctest --test-dir ./build 
```

- Build for release:
```bash
cmake -S. -Brelease -DCMAKE_BUILD_TYPE=Release; cmake --build release --verbose
```

