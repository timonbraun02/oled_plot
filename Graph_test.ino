#include "Graph_v2.h"

graph Graph1(2, 50); // create graph (num signals, num data points in graph)
graph Graph2(1,50); // default constructor (1, 50) 1 signal, 50 data points per plot

void setup() {

  init_graph_v2();

  Graph1.drawAxis(15, 128, 1, 28); // init graph with dimensions (x1, x2, y1, y2)
  Graph2.drawAxis(40, 80, 34, 62); // init graph with dimensions
}

void loop() {
  float t = micros() / 1e6;
  int signal_1[2];
  int signal_2[1];

  signal_1[0] = 12 + 10 * sin(2 * PI * t * 0.05);
  signal_1[1] = 12 + 5 * sin(2 * PI * t * 0.1);

  signal_2[0] = 10 + 10 * sin(2 * PI * t * 0.2);

  display.clearDisplay();

  Graph1.addDataPoint(signal_1); // create data point with val
  Graph1.drawDataLine(); // connect all data poits with line

  Graph2.addDataPoint(signal_2); // create data point with val
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
