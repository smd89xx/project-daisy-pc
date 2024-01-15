Project Daisy (PC Edition)
==========================

It's a PC game thing.

## Important Note

This branch is unmaintained and only exists to preserve the original codebase.

Use the [main](https://github.com/TheWindowsPro98/project-daisy-pc/tree/main) branch instead.

## Dependencies

 - [SFML](https://www.sfml-dev.org/)
 
 - Clang++/G++ (Linux)
 
 - Microsoft Visual Studio 2022 (Windows)
 
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

### On Windows

#### Important Note

Windows building does not currently work. Will fix later, and also may shove SFML into a submodule.

1. Download SFML 2.6.0 for your target architecture and Visual Studio version, and extract the `SFML-2.6.0` directory to the root of your C: drive.

2. Copy all DLL files from `SFML-2.6.0\bin` to the root of the repository.

3. Open the solution.

The solution should now be ready to build and run.
