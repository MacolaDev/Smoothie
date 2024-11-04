# About 

**Smoothie** is a open source physically-based real time render engine, written in C++ and built with OpenGL. 
It can be easly integreted within any game/game engine for rendering your game.

Another feature of Smoothie is its unique [Blender](https://www.blender.org/) add-on that allows artists and users to build entire scenes and models of their games in Blender and then render them in Smoothie render engine. 

As of now, it only supports Windows, but I plan Linux version at some point in the future too.

---

# Dependencies:

Make sure you have installed:
- [cmake](https://cmake.org/)
- [Python 3.11.7 ](https://www.python.org/downloads/release/python-3117/) 

before attempting to build this project.

---

# How to build:

Clone the repository first:

``git clone https://github.com/MacolaDev/Smoothie.git --recurse-submodules``

Then go to folder where you cloned this repository and run: 

``cmake .``

Cmake will then generate Visual Studio project files and solutions.

>Note: cmake will generate ZERO_CHECK and ALL_BUILD visual studio projects. You dont need them so its save to remove them from your solution.

# Getting started:
This repository countains 3 projects: 
- SmoothieEngine
- Demo
- Exporter

SmoothieEngine is core application, it builds as static library.
To use SmoothieEngine in your project, simply `#include "Smoothie.h"` in your code and link `SmoothieEngine.lib` with your project.
Everything that Smoothie needs to run is inside namespeces Smoothie, SmoothieMath and class SmoothieCore. 

Demo contains most basic app that you can build with SmoothieEngine.
It contains demo scene with all of its models, textures and shaders to run. 
You can move throught scene with pressing W, A, S and D keys.
It's mostly there to test how engine and its features work.

Exporter contains python code used in Blender add-on and source code for python extention module, SmoothieExporter that scripts uses to process data fast.
> Note: Exporter project must be build in Release mode, because (to my knowledge) Blender's python interpreter only works with standard and not debug version of python (python_d.exe). Otherwise SmoothieExporter wont be loaded into scripts and you would get an error.

To use exporter, simply open a Blender, go to Scripting tab, open `__init__.py` and run the script.
> Note: Unlike standard Python interpreter, Blender's interpreter wont know where are other scripts that `__init__.py` uses. To fix that we need to tell Blender explicitly where is our working folder.
The simplest way to do that is to copy-paste this line of code in Blender's python interpreter before running the script: `import sys; sys.path.append('C:\\path\\to\\your\\folder')`

---

### One more thing

If you have any questions, recommendation, or you would like to contribute to this project, feel free to contact me here or even better on [ my discord server](https://discord.gg/Mqh4Jmdh) (im more active there).
