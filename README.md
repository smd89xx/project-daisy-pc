Project Daisy (PC Edition)
==========================

It's a PC game thing.

## Dependencies

 - [SFML](https://www.sfml-dev.org/)
 
 - A working C++ compiler
 
  - I personally use Clang++, but g++ should work too.
 
 - [CMake](https://cmake.org/)

## How 2 build thing

### Important Note

If you use a seperate build directory, you must make sure to copy the binary to the root of the folder, or copy the /res folder to your custom build directory, or the game will not start due to not being able to find its assets.

### On Linux

    cmake .

or

    cmake ..

depending on whether or not you use a custom build directory.

Once either command is finished, run

    cmake --build .

and you will have an executable binary.
