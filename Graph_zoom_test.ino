#include "Graph_v4.h"
// library imports all of the oled libs
// if you use a 32x128 pixel oled, you need to change the #define SCREEN_HEIGHT to 32 in "Graph_v2.h", otherwise it will try to plot outside the visable screen

  /**
   * Note:
   * - for educational purposes
   * - expect bugs
   * - have fun
   * - thanks :)
   */

#define poti 4 // poti at analog pin 4 (ESP32)
int Y_max = 1;

graph Graph1(2, 255); // create graph (num signals -> max. 6, num data points in graph -> max. 255)

void setup() {

  init_graph(); // set up the library

  Graph1.drawAxis(15, 128, 1, 62); // init graph with dimensions (x1, x2, y1, y2)
  Graph1.autoZoom = false; // disble auto zoom
}

void loop() {

  float t = micros() / 1e6; // time var

  int graph1_signal[2];

  //two diffent signals for Graph1
  graph1_signal[0] = 12 + 10 * sin(2 * PI * t * 0.1) + 2 *  sin(2 * PI * t * 150);
  graph1_signal[1] = 12 + 5 * sin(2 * PI * t * 0.5);

  Y_max = analogRead(poti); // read poti
  Y_max = map(Y_max, 0, 4095, 1, 50); // map analog value to your needs

  display.clearDisplay(); // clear current screen

  Graph1.addDataPoint(graph1_signal); // create data point with signal_1 ( must be int var_name[] type! )
  Graph1.set_Y_Zoom(Y_max); // set zoom first, since coordinates on the screen are calculated in .drawDataLine()
  Graph1.drawDataLine(); // then draw to display

  // display avr, max, min and current zoom
  display.setCursor(0, 0);
  display.print(Graph1.getAverage());
  display.setCursor(0, 10);
  display.print(Graph1.getMax());
  display.setCursor(0, 20);
  display.print(Graph1.getMin());
  display.setCursor(0, 35);
  display.print(Graph1.manual_Y_max);

  display.display(); // display everything
  delay(10);
}
