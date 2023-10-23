#!/usr/bin/env bash

# This should be run from the *source file directory*.

set -e # Stop on first failed command.

echo "Building '$1.cpp'..."

include_paths="-I../../glad/include -I../../glfw_src/include"

local_lib_path="../../glfw_build/lib"
linked_libs="-lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl"
link_info="-L$local_lib_path $linked_libs -Wl,-rpath=$local_lib_path"

g++ $1.cpp -o build/$1 $include_paths $link_info

echo "Binary placed at 'build/$1'..."
