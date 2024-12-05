# OpenGL Examples Project

This project will contain code I've written as I learn more
OpenGL by following the excellent [learnopengl.com](https://learnopengl.com/)
tutorial, which I can't recommend enough
It has a convenient CMake build system adapted from
[gloom](https://github.com/aleksaro/gloom). The programs in `src/examples`
are small adaptations of the _learnopengl.com_ examples.

The main project is in `src/model_viewer`.
So far it shows a textured cube in the center of the window, that rotates at a constant rate.
I plan to add the ability to load arbitrary textured models
and use keyboard input to move and rotate the displayed model.
And later I'll experiment with other effects, like lighting and shadow.
It uses an example model and texture and some utility code from
_learnopengl.com_, along with some of our own utility code.
