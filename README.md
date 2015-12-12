# README

An empty C++ Open GL ES 2.0 scratch project providing drawing context for projections.

## Folders

```
root dir, run (e.g.) `make` here  .
                                  ├── bin
vendor code                       ├── include
                                  │   └── projection
platform specific code code       ├── platform
                                  │   ├── rpi
                                  │   │   └── projection
                                  │   └── weston
                                  │       └── projection
the source for the projection     └── src
                                      └── scratch

```

* Vendor code which is not install as an operating system dependency
* The platform directory lists routines for establishing a OpenGL ES 2.0 contexts
* The source directory is the place to implement your projection

## Platforms

The build system targets raspberry pi as the end platform and a debian VM running Wayland (weston) as a development environment.

There is a single platform-specific entry point: `projection::play(drawing)`.
In the platform folder, `play.cpp` implement this methodm the build system selects the correct platform.

# Acknowledgments

Based on the simple-egl example from weston source code:
https://github.com/krh/weston/blob/0eab05d2a7cf610e4d049dfd81b0100d4219e305/clients/simple-egl.c
