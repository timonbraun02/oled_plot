#include "Graph_v4.h"

graph myGraph; // default constructor: 1 signal, 50 data points per plot

#define Y_axis_max 30 // define a fixed max value for the Y-axis
int some_signal[1]; // define input var

void setup() {

  init_graph();
  myGraph.drawAxis(2, 128, 20, 62); // init graph with dimensions (x1, x2, y1, y2)
  myGraph.autoZoom = false; // disble auto zoom
  myGraph.set_Y_Zoom(Y_axis_max); // set manual zoom for y-axis

}

void loop() {
  // to show a signal on the oled, I'm creating diffenten values with a sin function
  float t = micros() / 1e6; // time var for artificial input signal (sine wave)
  some_signal[0] = 15 + 10 * sin(2 * PI * t * 1); // sample a value from the input signal and assign the value to the variable
  // some_signal[0] = analogRead(POTENTIOMETER_PIN); // this would be the code for reading(and plotting) an analog sensor value 

  display.clearDisplay(); // clear current screen

  myGraph.addDataPoint(some_signal); // pass value of data point(sample from sine wave in this case) to object
  myGraph.drawDataLine(); // draw all recorded data points and connect them with a line (50 points in case of default class constructor)
  //myGraph.drawDataPoints(); // draw all data points without connecting them

  // just print some value to the display (optional)
  display.setCursor(0, 0);
  display.print("Avr:");
  display.print(myGraph.getAverage()); // get the average of all displayed data points in the current frame
  display.setCursor(45, 0);
  display.print("Max:");
  display.print(myGraph.getMax()); // get the max of all displayed data points in the current frame
  display.setCursor(90, 0);
  display.print("Min:");
  display.print(myGraph.getMin()); // get the min of all displayed data points in the current frame

  display.display(); // display everything
  delay(10); // just add some delay (optional)
}
