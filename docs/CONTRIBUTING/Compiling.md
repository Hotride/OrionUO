# Compiling

This project requires CMake 3.7 to build. It currently supports Visual Studio 2015, 2017, GCC 7.0 and Clang 7.

### Windows

On Windows, to generate the Visual Studio solution, go inside your cloned OrionUO folder and do:

```bat
md build
cd build
cmake -G "Visual Studio 2017" ..
```

  > This will build a 32bits executable. If you do want 64bits, you can do `cmake -G "Visual Studio 2017 Win64" ..` instead. Note that plugins are still unsupported in the 64bits client.

Or, you can use CMake-GUI to configure and generate the solution.

* If you want to help converting OrionUO to SDL, you can disable `ORION_WISP` option in CMake. This option enables the original client implementation in pure win32 API and is kept as a reference implementation to use during the client conversion to SDL.

### Linux (in progress)

```bash
$ mkdir build
$ cd build
$ cmake -G Ninja .. -DCMAKE_BUILD_TYPE=Release
$ ninja OrionUO_unity -j8
$ ./OrionUO/unity/OrionUO
```

  > You'll need a `Client.cuo` and a `OrionUO.cfg` in the working directory. Inside `OrionUO.cfg` you can add a line `CustomPath=/path/to/uo/data`, so OrionUO will be able to find your original client data files.
