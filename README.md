# ELFReader ![workflow status](https://github.com/Radglay/ELFReader/actions/workflows/ci.yml/badge.svg?event=pull_request)
ELFReader is an desktop application created with C++ and Qt, used to view the structure of files of the Executable And Linkable format (ELF). The application provides user with high level
descriptions of particular fields of ELF headers and sections. An user can navigate through the file's object code manually or via the naviagation menu.

## Build
To create the application on the Linux platform:

##### Install build tools:
* g++ version 11.4.0
* Qt version 6.2.4
* CMake version 3.10

##### Download git repository with submodules: 
* Clone project's code from the git repository main branch
```
git clone https://github.com/Radglay/ELFReader
```
* Download project's submodules (GTest and plog libraries)  
```
cd ELFReader
git submodule update --init
```

##### Build the executable:
From the project top level directory:
```
cmake -S . -B build
```
```
cmake --build build
```
To run the application:
```
./build/App/main
```

## Tests
From the project top level directory:
#### Building
```
cmake -S . -B build -DBUILD_TESTS=ON
```
```
cmake --build build
```

#### Running
* simple summary:
```
ctest --test-dir build
```
* colored verbose summary:
```
GTEST_COLOR=1 ctest --test-dir build -V
```
