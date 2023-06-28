#include "Graph_v2.h"
// library imports all of the oled libs
// if you use a 32x128 pixel oled, you need to change the #define SCREEN_HEIGHT to 32 in "Graph_v2.h", otherwise it will try to plot outside the visable screen

  /**
   * Note:
   * - for educational purposes
   * - expect bugs
   * - have fun
   * - thanks :)
   */

graph Graph1(2, 100); // create graph (num signals -> max. 6, num data points in graph -> max. 255)
graph Graph2; // default constructor (1, 50) 1 signal, 50 data points per plot

void setup() {

  init_graph_v2(); // set up the library

  Graph1.drawAxis(15, 128, 1, 28); // init graph with dimensions (x1, x2, y1, y2)
  Graph2.drawAxis(40, 80, 34, 62); // init smaller graph
}

void loop() {
  float t = micros() / 1e6;
  int graph1_signal[2];
  int graph2_signal[1];

  //two diffent signals for Graph1
  graph1_signal[0] = 12 + 10 * sin(2 * PI * t * 0.05);
  graph1_signal[1] = 12 + 5 * sin(2 * PI * t * 0.1);

  graph2_signal[0] = 10 + 10 * sin(2 * PI * t * 0.2);

  display.clearDisplay();

  // just plot the signal, not showing the avr, max and min vals
  Graph1.addDataPoint(graph1_signal); // create data point with signal_1 ( must be int var_name[] type! )
  Graph1.drawDataLine(); // connect all data poits with line

  // plotting data and showing the avr, max and min vals of the currently plotted data
  Graph2.addDataPoint(graph2_signal); // create data point with signal_2 ( must be int var_name[] type! )
  Graph2.drawDataLine(); // connect all data poits with line
  display.setCursor(25, 35);
  display.print(Graph2.getAverage(0));
  display.setCursor(25, 45);
  display.print(Graph2.getMax());
  display.setCursor(25, 55);
  display.print(Graph2.getMin());

  display.display();
  delay(200);
}
