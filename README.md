# OpenGL and GLFW Examples

We're following Joey De Vries' tutorial, here: [learnopengl.com](https://learnopengl.com/Getting-started/Hello-Window)

We have added our own adaptation of his first "hello triangle" example, and in
the process we've started adding our own library of C++ tools for general use.

## CMake build system

We've now added a CMake build system based on [gloom](https://github.com/aleksaro/gloom).
And, following gloom, we have added dependencies glad, glfw, glm, and stb
as submodules and are building glfw as a subproject and linking it 
statically (I think) to the examples.

We will want to modify the CMake setup more for our purposes, especially
as we add more examples and build out the existing code. But this
is a big improvement on what we had before and is heading towards a
nice, professional setup.

## To Do Next

We'll want to think about whether shaders should be in a folder for each example.
Probably so. Images are probably fine in their own folder, which could have
subfolders within it.

> We will want to remove the globbing for more fine grained dependency specification.

Also, figure out why the glfw window example is seg faulting.
