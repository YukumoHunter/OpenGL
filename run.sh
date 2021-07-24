#!/bin/sh

if gcc ./src/hello_world.c ./include/GLAD/glad.c -Iinclude -o hello_world -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lm; then

echo "Compiled :D"

./hello_world

else

echo "Compilation Error :("

fi