# Console Commands

## Overview

Console commands are available at runtime by using the ² key.  
A prompt will appear at the top of the window, where you can enter the desired command and its arguments.  

The engine provide several built-in commands, but other commands can be implemented in the Application. Any command entered in the console will be forwarded to the application for custom computations.

## Built-in Console Commands

### > fps
Display a basic fps count on the screen.

### > stats
Display rendering stats with curves/histograms on the screen.

### > bounds
Display all Elements bounds.

### > ruler [int:size]
Display vertical and horizontal lines centered on cursor, with graduations every [size] pixels (default : ruler 100).

### > trace [int:nbFrames]
Trace performances for the next [nbFrames] frames (default : trace 10).  
Generates a json file readable in chrome://tracing/ (chrome) or about://tracing/ (opera).

### > speed [float:multiplier]
Set the engine loop speed multiplier at [multiplier] (default speed is 1, minimum is 0).

### > pause
Pause the engine loop (can be stacked on top of a modified speed).

### > time [int:value]
Inject [value] milliseconds into the engine loop (can only be used when the engine loop is paused).

### > imgui
Toggle ImGui rendering and execution.
