# OpenGL and GLFW Examples

We're following Joey De Vries' tutorial here: [learnopengl.com](https://learnopengl.com/Getting-started/Hello-Window)

We initially used the instructions [here](https://www.glfw.org/docs/3.3/compile.html) to build and _not install_ GLFW.
But now we are just using the system GLFW library. (See the build script for details.)

Following the tutorial suggestion we used GLAD to configure OpenGL. See here: [GLAD](https://glad.dav1d.de/).

We've made a build script, `examples/example_build.sh`, to compile our examples, assuming the
GLAD header in the root directory is configured appropriately for the system, following
De Vries' instructions [here](https://learnopengl.com/Getting-started/Creating-a-window).

The build script is set up to be run from the directory containing the example source file.
There is a script `set_build_alias.rc` in the project root directory that you can
use to create an alias for the build script for the current shell session. To use it run:

```shell
source ./set_build_alias.sh
```

Then you'll be able to run

```shell
example_build <SOURCE_BASENAME>
```

from the directory where the source file you're building is located.

## CMake build system

We've now added a CMake build system based on [gloom](https://github.com/aleksaro/gloom).

We will want to modify it more for our purposes, especially
as we add more examples and build out existing code. But this
is a big improvement on what we had before.
