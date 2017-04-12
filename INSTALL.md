## Installation
- [Prerequisites](#prerequisites)
- [CMake Options](#cmake-options)
- [Linux](#linux)
- [Windows](#windows)
  - [MinGW](#mingw)
  - [MSVC](#msvc)


### Prerequisites

  - CMake 2.8.11 or newer
  - [libtoxcore](https://github.com/irungentoo/toxcore) installed (just tox.h header file would do)


### CMake Options

Name | Description | Default
---|---|---
|BUILD_STATIC | Build a static library instead. | OFF
|BUILD_TEST | Build test application. | OFF
|TOXCORE_INCLUDE_DIR_PATH | Path to toxcore include directory. | "NONE"
|CMAKE_INSTALL_PREFIX | Path to where everything should be installed. | Platform dependent

(Usage: `cmake -DBUILD_STATIC=ON ...` or `cmake -DTOXCORE_INCLUDE_DIR_PATH="C:/toxcore/prefix/include/tox/" ...`)

Note that options are cached in CMakeCache.txt. Delete CMakeCache.txt if you want to re-run cmake with different set of options.

### Linux

Navigate into the root directory of this repository and run:

```sh
mkdir build && cd build
cmake ..
make
make install
```

### Windows

#### MinGW

Run `cmd.exe` and navigate into the root directory of this repository.

Make sure you have CMake and MinGW in your PATH environment variable. If you don't, you can just run:

```cmd
set PATH=%PATH%;C:\path\to\where\mingw\is\installed\bin\;C:\path\to\where\cmake\is\installed\bin\
```

This will add them into the PATH environment variable of the current `cmd.exe` session.

After that, run:

```cmd
mkdir build && cd build
cmake .. -G "MinGW Makefiles" -DTOXCORE_INCLUDE_DIR_PATH="C:/path/to/directory/containing/tox.h/" -DCMAKE_INSTALL_PREFIX="prefix"
mingw32-make install
cd prefix
explorer .
```

If everything goes successfully, you should have the "prefix" directory, the place where the library is said to be installed, open in Explorer.

If something goes wrong, read the output of cmake or the compiler carefully and try to fix the problem.

#### MSVC

Run `cmd.exe` and navigate into the root directory of this repository.

Make sure you have CMake in your PATH environment variable. If you don't you can just run:

```cmd
set PATH=%PATH%;C:\path\to\where\cmake\is\installed\bin\
```

This will add cmake into the PATH environment variable of the current `cmd.exe` session.

Run the appropriate `vcvarsall.bat` script for your version of MSVC. For example, for MSVC 12.0 (Microsoft Visual Studio 2013):
```cmd
"C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" x86
```

You can get a list of all valid script arguments by running:
```cmd
"C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" help
```

After that, run:

```cmd
mkdir build && cd build
cmake .. -G "NMake Makefiles" -DTOXCORE_INCLUDE_DIR_PATH="C:/path/to/directory/containing/tox.h/" -DCMAKE_INSTALL_PREFIX="prefix"
nmake install
cd prefix
explorer .
```

If everything goes successfully, you should have the "prefix" directory, the place where the library is said to be installed, open in Explorer.

If something goes wrong, read the output of cmake or the compiler carefully and try to fix the problem.
