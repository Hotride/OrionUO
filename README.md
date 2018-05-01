# OrionUO Client

[![Travis Build Status](https://travis-ci.org/Hotride/OrionUO.svg?branch=master)](https://travis-ci.org/Hotride/OrionUO)
[![Build status](https://ci.appveyor.com/api/projects/status/qmd3795itrkiwnr3?svg=true)](https://ci.appveyor.com/project/Hotride/orionuo)


* OrionUO Client - is an alternative, open source Ultima Online graphic client.

* Platforms: Windows and Linux (in progress)

* Rendering: OpenGL 2.0 and higher

## Orion Community

[Forum](http://forum.orion-client.online)

[Discord](https://discord.gg/UcVKWzB)

[WIKI](https://github.com/Hotride/OrionUO/wiki)

[Our Mantis bugtracking system](http://bt.orion-client.online) or [github issue tracking](https://github.com/Hotride/OrionUO/issues)

## Download

* Download [Orion Launcher](https://orion-client.online/index.php?pages/downloads/) to set everything up and play right away!

### Other Orion Projects

* [Orion.dll](https://github.com/Hotride/OrionDLL) optional protocol cryptography
* [Orion Launcher](https://github.com/Hotride/OrionLauncher)

--------------

## Building OrionUO

This project requires CMake 3.7 to build. It currently supports Visual Studio 2015, 2017, GCC 7.0 and Clang 6.

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

  > You'll need a `client.cfg` in the working directory. Inside `client.cfg` you can add a line `CustomPath=/path/to/uo/data`, so Orion will be able to find your original client data files.

--------------

## Contributing

See the project planning [here](https://github.com/Hotride/OrionUO/projects) to find tasks on which you can help.

  > More detailed contribution documentation soon

## Contributors

[Hotride](https://github.com/Hotride/) (Author)

[AimedNuu](https://github.com/AimedNuu)

and [Others](https://github.com/Hotride/OrionUO/graphs/contributors)

--------------
## Support this project, make a donation!

[PayPal](https://www.paypal.me/Hotride)

WebMoney: R644829964694 Z983232789532 E400319624386
