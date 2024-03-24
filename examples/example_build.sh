#!/usr/bin/env bash

# This should be run from the *source file directory*.

set -e # Stop on first failed command.

if ! [ -f "$1.cpp" ]; then
	echo "Source file '$1.cpp' does not exist. Please provide basename of a source file in the current directory."
	exit 1
fi

echo "Building '$1.cpp'..."

# NOTE: This needs to be updated if you clone repo
#       to a different location.
PROJECT_DIR="/home/sean/Code/opengl_examples"

# Link to the glfw installed through apt.
use_system_glfw=1

linked_libs="-lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl"

include_paths="-I$PROJECT_DIR/glad/include"
local_lib_path=""

glad_source="$PROJECT_DIR/glad/src/glad.c"

link_info="$linked_libs"

if [ $use_system_glfw -eq 0 ]; then
	include_paths+=" -I$PROJECT_DIR/glfw_src/include"
	local_lib_path+="$PROJECT_DIR/glfw_build/lib"
	link_info="-L$local_lib_path $link_info -Wl,-rpath=$local_lib_path"
fi

g++ $glad_source $1.cpp -o build/$1 $include_paths $link_info

echo "Binary placed at 'build/$1'..."
