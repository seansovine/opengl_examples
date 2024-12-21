# OpenGL Examples Project

This project will contain code I've written as I learn more
OpenGL by following the excellent [learnopengl.com](https://learnopengl.com/)
tutorial, which I can't recommend enough.
It has a convenient CMake build system adapted from
[gloom](https://github.com/aleksaro/gloom). The programs in `src/examples`
are small adaptations of the _learnopengl.com_ examples.

The main project is in [src/model_viewer](src/model_viewer).
So far it shows a textured cube in the center of the window, and
you can click and drag with the mouse to rotate the object around
the x- and y-axes.
Next I plan to add the ability to load arbitrary textured models,
and later I'll experiment with some effects like lighting and shadow.
It's currently using the cube model and some utility code from
_learnopengl.com_, along with some of the infrastructure we're slowly
building up for working with OpenGL graphics.
