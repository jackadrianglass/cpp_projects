# Assignment 2
cmake target : assignment2

usage
  * WASD and up down left right for movement
  * Left mouse click for rotation
  * R for resetting

No bonus attempted

Used msvc toolchain for an x64 target on windows with the Ninja generator

Tried building on wsl. Compiles but doesn't link for missing symbol issues for glew
(which I didn't feel like fixing at the time and is odd because I'm using glad...)

I used CLion for my project and build. I've run into issue with the FreeType
library on other machines. Let me know if that occurs for you.