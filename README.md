# README

An empty C++ Open GL ES 2.0 scratch project providing drawing context for projections.


## Platforms

The build system targets raspberry pi as the end platform and a debian VM running Wayland (weston) as a development environment.

There is a single platform-specific entry point: `projection::play(drawing)`.
In the platform folder, `play.cpp` implement this methodm the build system selects the correct platform.

# Acknowledgments

Based on the simple-egl example from weston source code:
https://github.com/krh/weston/blob/0eab05d2a7cf610e4d049dfd81b0100d4219e305/clients/simple-egl.c
