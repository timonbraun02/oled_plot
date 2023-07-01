#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/**
 * Note:
 * - Version 3
 * - for educational purposes
 * - expect bugs
 * - have fun
 * - thanks :)
 */

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void init_graph()
{
  Serial.begin(9600);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }
  display.display();
  display.clearDisplay();
  display.setTextSize(1);      // set text size
  display.setTextColor(WHITE); // set text color
}

class graph
{

public:
  int num_signals;
  int data_len;
  graph() : num_signals(1), data_len(50) {}
  graph(int num, int len = 50)
  {
    num_signals = num;
    data_len = len;
  }
  bool autoZoom = true;
  const static int max_num_signals = 6;
  const static int max_samples = 255;
  int X_startVal, X_endVal, Y_startVal, Y_endVal;
  int data[max_num_signals][max_samples];
  int Coords[max_num_signals][max_samples][2];
  int Y_max[max_num_signals];
  int manual_Y_max = 0, auto_Y_max = 0;
  int Y_min[max_num_signals];
  int points[max_num_signals] = {0, 0, 0, 0, 0, 0};
  int avr[max_num_signals];

  void drawAxis(int X0, int X1, int Y0, int Y1)
  {
    X_startVal = X0;
    X_endVal = X1;
    Y_startVal = Y0;
    Y_endVal = Y1;

    display.drawLine(X_startVal, Y_endVal, X_endVal, Y_endVal, WHITE);     // horizontal
    display.drawLine(X_startVal, Y_startVal, X_startVal, Y_endVal, WHITE); // vertical

    for (byte i = 0; i < (((Y_endVal - Y_startVal) * 100) / 500); i++)
    {
      display.drawLine(X_startVal - 1, Y_endVal - (i + 1) * 5, X_startVal + 1, Y_endVal - (i + 1) * 5, WHITE);
    }

    for (byte i = 0; i < (((X_endVal - X_startVal) * 100) / 500); i++)
    {
      display.drawLine(X_startVal + (i + 1) * 5, Y_endVal - 1, X_startVal + (i + 1) * 5, Y_endVal + 1, WHITE);
    }
  }

  void addDataPoint(int val[])
  {
    auto_Y_max = 0;
    for (byte signal_index = 0; signal_index < num_signals; signal_index++)
    {
      Y_max[signal_index] = 0;
      Y_min[signal_index] = 0xFFFF;
      if (points[signal_index] < data_len)
      {
        data[signal_index][points[signal_index]] = val[signal_index];
        points[signal_index]++;
      }
      else
      {
        for (int i = 0; i < data_len - 1; i++)
        {
          data[signal_index][i] = data[signal_index][i + 1];
        }
        data[signal_index][data_len - 1] = val[signal_index];
      }
      avr[signal_index] = 0;
      for (byte i = 0; i < points[signal_index]; i++)
      {

        if (data[signal_index][i] > Y_max[signal_index])
        {
          Y_max[signal_index] = data[signal_index][i];
          if (Y_max[signal_index] > auto_Y_max)
          {
            auto_Y_max = Y_max[signal_index];
          }
        }
        if (data[signal_index][i] < Y_min[signal_index])
        {
          Y_min[signal_index] = data[signal_index][i];
        }

        avr[signal_index] += data[signal_index][i];
      }
      avr[signal_index] = avr[signal_index] / points[signal_index];
    }
  }

  int getAverage(int signal = 0)
  {
    if (signal > num_signals)
    {
      return 0;
    }
    return avr[signal];
  }

  int getMax(int signal = 0)
  {
    return Y_max[signal];
  }

  int getMin(int signal = 0)
  {
    return Y_min[signal];
  }


  void drawDataPoints()
  {
    for (byte signal_index = 0; signal_index < num_signals; signal_index++)
    {
      drawAxis(X_startVal, X_endVal, Y_startVal, Y_endVal);
      for (byte data_point_index = 0; data_point_index < points[signal_index]; data_point_index++)
      {
        int X_coord = ((((data_point_index + 1) * 100 / points[signal_index]) * (X_endVal - X_startVal)) / 100) + X_startVal;
        int point = data[signal_index][data_point_index];
        int Y_coord = 0;
        if (point > 0)
        {
          if (autoZoom)
          {
            Y_coord = ((point * 100 / auto_Y_max) * Y_endVal) / 100;
            manual_Y_max = auto_Y_max;
          }
          else
          {
            Y_coord = ((point * 100 / manual_Y_max) * Y_endVal) / 100;
          }
        }
        if (Y_coord <= Y_endVal)
        {
          Y_coord = map(Y_coord, 0, Y_endVal, Y_endVal, Y_startVal);
          display.drawPixel(X_coord, Y_coord, WHITE);
        }
      }
    }
  }

  void drawDataLine()
  {
    for (byte signal_index = 0; signal_index < num_signals; signal_index++)
    {
      drawAxis(X_startVal, X_endVal, Y_startVal, Y_endVal);
      for (byte data_point_index = 0; data_point_index < points[signal_index]; data_point_index++)
      {
        int X_coord = ((((data_point_index + 1) * 100 / points[signal_index]) * (X_endVal - X_startVal)) / 100) + X_startVal;
        int point = data[signal_index][data_point_index];
        int Y_coord = 0;
        if (point > 0)
        {
          if (autoZoom)
          {
            Y_coord = ((point * 100 / auto_Y_max) * Y_endVal) / 100;
            manual_Y_max = auto_Y_max;
          }
          else
          {
            Y_coord = ((point * 100 / manual_Y_max) * Y_endVal) / 100;
          }
          if (Y_coord > Y_endVal)
          {
            Y_coord = Y_endVal;
          }
        }
        Coords[signal_index][data_point_index][0] = X_coord;
        Coords[signal_index][data_point_index][1] = map(Y_coord, 0, Y_endVal, Y_endVal, Y_startVal);
        if (data_point_index > 0)
        {
          display.drawLine(Coords[signal_index][data_point_index - 1][0], Coords[signal_index][data_point_index - 1][1], Coords[signal_index][data_point_index][0], Coords[signal_index][data_point_index][1], WHITE);
        }
      }
    }
  }

  void set_Y_Zoom(int maxVal)
  {
    if (maxVal <= 0)
    {
      maxVal = 1; // prevent divison by zero error
    }
    manual_Y_max = maxVal;
  }
};
