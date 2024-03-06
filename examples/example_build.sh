#!/usr/bin/env bash

# This should be run from the *source file directory*.

set -e # Stop on first failed command.

echo "Building '$1.cpp'..."

# Link to the glfw installed through apt.
use_system_glfw=1

linked_libs="-lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl"

include_paths="-I../../glad/include"
local_lib_path=""

link_info="$linked_libs"

if [ $use_system_glfw -eq 0 ]
then
	include_paths+=" -I../../glfw_src/include"
	local_lib_path+="../../glfw_build/lib"
	link_info="-L$local_lib_path $link_info -Wl,-rpath=$local_lib_path"
fi

g++ $1.cpp -o build/$1 $include_paths $link_info

echo "Binary placed at 'build/$1'..."
