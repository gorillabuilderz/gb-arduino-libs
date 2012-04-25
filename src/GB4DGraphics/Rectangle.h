#ifndef Rectangle_h
#define Rectangle_h

#include <Arduino.h>
#include <GB4DLcdDriver.h>
#include <utility/Widget.h>

class Rectangle : public Widget
{
  public:
	Rectangle(GB4DLcdDriver &lcd, int16_t x, int16_t y, int16_t x2, int16_t y2, bool fill, int16_t color);

    void clear();

    void draw();

  private:
    bool _fill;

  	int16_t _x2;
	int16_t _y2;

    int16_t _backgroundColor;
};

#endif
