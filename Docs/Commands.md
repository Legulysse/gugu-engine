# Built-in Console Commands

### > fps
Display a basic fps count on the screen.

### > stats
Display rendering stats with curves/histograms on the screen.

### > bounds
Display all Elements bounds.

### > ruler
Display vertical and horizontal lines centered on cursor, with graduations every 100 pixels.

### > trace [int:nbFrames]
Trace performances for the next [nbFrames] frames (default : trace 10).  
Generates a json file readable in chrome://tracing/ (chrome) or about://tracing/ (opera).

### > speed [float:multiplier]
Set the engine loop speed multiplier at [multiplier] (default speed is 1, minimum is 0).

### > pause
Pauses the engine loop (can be stacked on top of a modified speed).

### > time [int:value]
Injects [value] milliseconds into the engine loop (can only be used when the engine loop is paused).
