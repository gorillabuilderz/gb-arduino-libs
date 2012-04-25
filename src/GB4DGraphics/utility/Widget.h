#ifndef Widget_h
#define Widget_h

#include <Arduino.h>
#include <GB4DLcdDriver.h>

class Widget
{
  public:
	Widget(GB4DLcdDriver *lcd, int16_t x, int16_t y, int16_t color);

    void setColor(int16_t color);
    int16_t getColor();

    virtual void clear();

    int16_t getX();
    int16_t getY();

    virtual void draw();

  protected:
    GB4DLcdDriver *_lcd;

  	int16_t _x;
	int16_t _y;

    int16_t _color;

  private:
};

#endif
