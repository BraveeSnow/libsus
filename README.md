# Snow's Ultimate Shaders

An open source library for OpenGL. Is it sus? Maybe.

## Installation

Start by running cmake to generate a build directory. If you wish to generate
HTML documentation, either add the `-DSUS_GEN_DOCS=ON` argument for cmake or
run `doxygen` in the root directory.

`cmake -B build -DCMAKE_INSTALL_PREFIX=/usr`

Once finished, change into the new directory, make all targets, then install.

`make all && make install`
