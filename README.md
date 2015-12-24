# README

An empty C++ Open GL ES 2.0 scratch project providing drawing context for projections.  
Visit [http://projectionist.array.bz/projections](http://projectionist.array.bz/projections) for some example projections based on this project.

## Usage

The scratch project is developed to be built on a Debian VM or a Rapsberry PI as detail on the [tools page](http://projectionist.array.bz/tools) page of the project website.

* check out the project by cloning the git repository
* `cd` in to the project folder
* run `make clean`. It is a good idea to run these when switching between different branches of the same repository.
* run `make`
* run `bin/main`. you should see a full screen, white trapezium on a black background.

## Folder Structure

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

* Vendor code is where we put depedencies which are not installed with the OS package manager
* The platform directory lists routines for establishing a OpenGL ES 2.0 contexts
* The source directory is the place to implement your projection

## Additional Platforms

The build system targets raspberry pi as the end platform and a debian VM running Wayland (weston) as a development environment.

There is a single entry point: `projection::play(drawing)` for each platform.  
Platform-specifics are decided in the implementation files in `platform/*`.  
In the folder corresponding to each platform, `play.cpp` implements this method.  
The build system selects the correct implementation.

# Also see

http://projectionist.array.bz/tools

# Acknowledgments

Based on the simple-egl example from weston source code:
https://github.com/krh/weston/blob/0eab05d2a7cf610e4d049dfd81b0100d4219e305/clients/simple-egl.c
