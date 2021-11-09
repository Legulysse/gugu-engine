# gugu::Engine 0.6
[![License: Zlib](https://img.shields.io/badge/License-Zlib-lightgrey.svg)](https://opensource.org/licenses/Zlib)

A minimalist 2D game engine, written in C++, mainly based on SFML, ImGui, and PugiXml.

Author : Legulysse  
Contributors : Diccy, Amitsouko

## Dependencies

SFML 2.5.1 : used by the Engine.  
PugiXml 1.11.4 : used by the Engine.  

Python 3.x : used for build and utility scripts.  
Premake 5 alpha 16 : used by build scripts.  

ImGui 1.84.1 (docking) : used by the new Editor.  
ImGui-SFML 2.3 : used for the ImGui integration.  

Qt 5.7.0 : used by the old Editor.  
Qt Creator 4.1.0 : used for the old Editor.  

## Setup

The project makefiles are generated through Premake.  

You can use /Tools/Build.py to spawn a little prompt with a menu to select your target (visual studio, gnu makefile, etc).  
Alternatively, you can use /Tools/Build_vs.bat to generate the visual2019 build files directly, without using python.  
The generated project files will be available in /Dev/Build.  

## Quick Overview

### Engine

You can find the engine code itself in /Dev/SourcesEngine.  
It's mostly made of c++ classes, with an additionnal python script (DatasheetBindingTool) used to generate xml binding code for the datasheet system.  

### Demos

You can find several little demos in /Dev/SourcesDemos.  
Those demos are very small unit cases, showing individual engine systems at work.  
Once compiled, most of them will run in /Version/DemoTests.  

The demo Game is a bit bigger, and is aimed to become a sample of a fully functional mini roguelike, to show all of the engine systems working together.  
This demo will run in /Version/DemoGame.  

![Demo Game](/Docs/Pictures/DemoGame_02.png)

### Editor

You can find an early draft of the editor in /Tools/GuguEditorOld, and a remake in /Tools/GuguEditor.  
Their sources can be found in /Dev/EditorOld and /Dev/SourcesEditor.  
The old version can help create imagesets and animsets from existing textures.  
The new version allows the edition of gameplay datasheets, and will later include the old imageset and animset edition tools.  
Please note that those are very early drafts.  

![Demo Game](/Docs/Pictures/EditorOld.png)
![Demo Game](/Docs/Pictures/Editor_01.png)
