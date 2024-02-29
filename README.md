# gugu::Engine 0.7.1
[![License: Zlib](https://img.shields.io/badge/License-Zlib-lightgrey.svg)](https://opensource.org/licenses/Zlib)

A minimalist 2D game engine, written in C++, mainly based on SFML, ImGui, and PugiXml.

Author : Legulysse  
Contributors : Diccy, Amitsouko

## Dependencies

- SFML 2.6.1 : used by the Engine.  
- PugiXml 1.13 : used by the Engine.  
- ImGui 1.87 (docking) : used by the Editor.  
- ImGui-SFML 2.6 : used for the ImGui integration.  
- Python 3.x : used for build and utility scripts.  
- Premake 5 alpha 16 : used by build scripts.  

## Docs

- [Changelog](/Docs/ProjectHistory.md) (French)  
- [Console Commands](/Docs/Commands.md)  

## Quick Overview

### Setup

The project makefiles are generated through Premake.  

You can use /Tools/Build.py to spawn a little prompt with a menu to select your target (visual studio, gnu makefile, etc).  
Alternatively, you can use /Tools/Build_vs2019.bat to generate the visual2019 build files directly, without using python.  
The generated project files will be available in /Dev/Build.  

### Engine

You can find the engine code itself in /Dev/SourcesEngine.  
It's mostly made of c++ classes, with an additionnal python script (DatasheetBindingTool) used to generate xml binding code for the datasheet system.  

### Demos

You can find several little demos in /Dev/SourcesDemos.  
Those demos are very small unit cases, showing individual engine systems at work.  
Once compiled, most of them will run in /Version/DemoTests.  

The demo Game is a bit bigger, and is aimed to become a sample of a fully functional mini roguelike, to show all of the engine systems working together.  
This demo will run in /Version/DemoGame.  

![Demo Game](/Docs/Pictures/DemoGame_03.png)

### Editor

You can find the editor in /Tools/GuguEditor.  
Its sources can be found in /Dev/SourcesEditor and /Dev/SourcesEditorApp.  
The editor allows the edition of gameplay datasheets, imagesets, animsets, and particle effects.  

![Demo Game](/Docs/Pictures/Editor_02.png)
