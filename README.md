# Graph_v2
An Arduino compatible library for plotting data to an OLED-Display.

You also need the following libs:
- Adafruit_GFX.h
- Adafruit_SSD1306.h

Have fun :)

# Quick start guide

- Import the lib

```cpp
#include "Graph_v3.h"
```

- Create and name a graph object
- Optional: at creation you are able to set ne number of signals you want to plot in the graph as well as the number of data points that should be plottet in each frame
```cpp
graph Graph1(2, 100); // create graph (num signals -> max. 6, num data points in graph -> max. 255)
graph Graph1; // default constructor (1, 50) -> 1 signal, 50 data points per plot
```

- Init the lib in your set up and define the dimensions of your graph (display coordinates)
- If needed, disable the auto zoom feature in the setup as well
```cpp
void setup() {
  init_graph(); // set up the library
  Graph1.drawAxis(15, 128, 1, 62); // init graph with dimensions (x1, x2, y1, y2)
  Graph1.autoZoom = false; // disble auto zoom
}
```

- In your loop, clear the display if you want to plot a new data point, pass the value to the graph and display everything again
```cpp
void loop() {
  // some code
  graph1_signal = analogRead(sensor); // for example
  display.clearDisplay(); // clear current screen
  Graph1.addDataPoint(graph1_signal); // create data point with signal_1 ( must be int var_name[] type! )
  Graph1.drawDataLine(); // then draw to display
  display.display(); // display everything
  }
```

- Try out the Graph_test.ino and the Graph_zoom_test.ino to test all the features

# Manual zoom

- By default, the lib automaticly adjusts the y-axis according to the max of the plotted data points
- You can disable this feature
```cpp
Graph1.autoZoom = false; // disble auto zoom
```
- Then set your zoom manualy (via potentiometer, buttons, ...)
```cpp
  Y_max = analogRead(poti); // read poti
  Y_max = map(Y_max, 0, 4095, 1, 50); // map analog value to your needs
  Graph1.set_Y_Zoom(Y_max); // set zoom before plotting, since coordinates on the screen are calculated in .drawDataLine()
```
