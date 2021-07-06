#!/bin/sh

gcc ./src/hello_world.c ./include/GLAD/glad.c -o hello_world -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lm
echo Compiled :D
./hello_world