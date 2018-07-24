# gugu::Engine 0.5

A minimalist 2D game engine, written in C++, mainly based on SFML and PugiXml.

Author : Legulysse  
Contributor : Diccy

## Dependencies

SFML 2.5.0 : used by the Engine.  
PugiXml 1.2 : used by the Engine.  

Python 3.x : used for build and utility scripts.  
Premake 5 alpha 12 : used by build scripts.  

Qt 5.7.0 : used by the Editor.  
Qt Creator 4.1.0 : used for the Editor.  

## Setup

The project makefiles are generated through Premake.  

You can use /Tools/Build.py to spawn a little prompt with a menu to select your target (visual studio, gnu makefile, etc).  
The generated project files will be available in /Dev/Build.  

## Quick Start

### Engine

You can find the engine code itself in /Dev/SourcesEngine.  
It's mostly made of c++ classes, with an additionnal python script (DatasheetBindingTool) used to generate xml binding code for the datasheet system.  

### Demos

You can find several little demos in /Dev/SourcesDemos.  
Those demos are very small unit cases, showing individual engine systems at work.  
Once compiled, most of them will run in /Version/DemoTests.  

The demo Game is a bit bigger, and is aimed to become a sample of a fully functional mini roguelike, to show all of the engine systems working together.  
This demo will run in /Version/DemoGame.  

![Demo Game](/Docs/Pictures/DemoGame.png)

### Editor

You can find an early draft of the editor in /Tools/GuguEditor.  
Its sources can be found in /Dev/Editor.  
This tool can help create imagesets and animsets from existing textures.  
Please note that it's a very early draft.  

![Demo Game](/Docs/Pictures/Editor.png)
