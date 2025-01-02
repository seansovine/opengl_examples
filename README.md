# OpenGL Examples Project

This project will contain code I've written as I learn more
OpenGL by following the excellent [learnopengl.com](https://learnopengl.com/)
tutorial, which I can't recommend enough.
It has a convenient CMake build system adapted from
[gloom](https://github.com/aleksaro/gloom). The programs in `src/examples`
are small adaptations of the _learnopengl.com_ examples.

## Model viewer

The main project is in [src/model_viewer](src/model_viewer).
So far it shows a model loaded from a given file in the center of the window, and
allows the user to click and drag with the mouse to rotate the object around
the x- and y-axes and zoom in and out using the scroll wheel.
I have added the ability to load arbitrary textured models using Assimp, following
the examples in _learnopengl.com_. 
This uses some code from _learnopengl.com_, like the Shader class and
the main outline of the Model and Mesh classes,
along with some of our own infrastructure we're slowly
building up for working with OpenGL graphics as we work through the tutorial.

Next, I plan to experiment with some lighting more sophisticated texture
techniques.
