# Shakujo - A portable relational database languages compiler

## Requirements

* CMake `>= 3.5`
* C++ Compiler `>= C++17`
* Java Runtime `>= 8.0`
* and see *Dockerfile* section

```sh
# retrieve third party modules
git submodule update --init --recursive
```

#### Dockerfile

```dockerfile
FROM ubuntu:18.04

RUN apt update -y && apt install -y git build-essential cmake ninja-build openjdk-8-jdk pkg-config uuid-dev doxygen
```

## How to build

```sh
mkdir build
cd build
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..
ninja
```

available options:

* `-DBUILD_TESTS=OFF` - don't build test programs
* `-DBUILD_DOCUMENTS=OFF` - don't build documents by doxygen
* `-DBUILD_EXAMPLES=OFF` - don't build nor test example programs
* `-DINSTALL_EXAMPLES=ON` - also install example programs
* `-DFORCE_INSTALL_RPATH=ON` - force set RPATH for non-default library paths

### install

```sh
ninja install
```

### run tests

```sh
ctest
```

### generate documents

```sh
ninja doxygen
```

## License

[Apache License, Version 2.0](http://www.apache.org/licenses/LICENSE-2.0)
