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

## Project Structure

We're currently putting images, textures, maybe model files under the `resources`
subdirectory of project root.

We're grouping shaders in a `shaders` subfolder of each example, but keeping
a `src/shaders` folder in case there are shaders that can be shared across
example programs.

The `src/learnopengl` folder contains De Vries' utility programs from his tutorial,
with some of our modifications.

## To Do Next

Look into the libraries we use (mostly the ones from De Vries code). E.g., `stb_image`.
See what we can learn about how they're working and so on.

Keep studying De Vries' code and reading his tutorial, and start writing some
of our own code based on what we learn and ideas we get from other places
(like [Physically Based Rendering](https://www.pbr-book.org/4ed/contents)).

Maybe create a similar project to this for WebGL. It seems to use a version of
GLSL, so there should be overlap with regular OpenGL programming.

## Question

Understand the `dl` library here:

```cmake
  if(NOT WIN32)
    set(GLAD_LIBRARIES dl)
  endif()
```

This bit was added by gloom, and I don't see that we're building a `dl` target.
So I'm guessing this is a library provided by the Linux environment.

[This](https://stackoverflow.com/questions/33678965/need-to-link-cmake-project-to-dl-library)
Stack Overflow post suggests that it might be "dlopen". And 
[this](https://stackoverflow.com/questions/63306734/when-to-actually-use-dlopen-does-dlopen-means-dynamic-loading)
post suggests it might be used when you want to dynamically load a library,
as in a plugin architecture.
