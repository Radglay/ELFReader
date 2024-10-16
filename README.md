# ELFParser
![example workflow](https://github.com/Radglay/ELFReader/actions/workflows/ci.yml/badge.svg)

## Downloading
1) `git clone https://github.com/Radglay/ELFParser`
2) `cd ELFParser`
3) `git submodule init`
4) `git submodule update`

## Building & running executable
### DEBUG version
1) `cmake -S . -B build -DCMAKE_BUILD_TYPE=DEBUG`
2) `cmake --build build`
### RELEASE version
1) `cmake -S . -B build -DCMAKE_BUILD_TYPE=RELEASE`
2) `cmake --build build`

### Running
3) `./build/App/main <elf_file_name>`

## Building tests & running tests
### Building
1) `cmake -S . -B build -DBUILD_TESTS=ON`
2) `cmake --build build`

### Running
3) simple summary: `ctest --test-dir build`
4) colored verbose summary: `GTEST_COLOR = 1 ctest --test-dir build -V`
