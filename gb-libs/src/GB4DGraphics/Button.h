#ifndef Button_h
#define Button_h

#include <Arduino.h>
#include <GB4DLcdDriver.h>
#include "utility/Widget.h"
#include "Label.h"
#include "Rectangle.h"

class Button : public Widget
{
  public:
    Button(GB4DLcdDriver &lcd, int16_t x, int16_t y, char* text);
    // Pass width onto label with center horizontal alignment
    Button(GB4DLcdDriver &lcd, int16_t x, int16_t y, int16_t width, char* text);
    Button(GB4DLcdDriver &lcd, Rectangle& rectangle, Label& label);

    Label getLabel();
    void setLabel(Label *label);

    void setText(char *text);

    void on();
    void off();

    void draw();

  private:
    Rectangle *_rectangle;
    Label *_label;
};

#endif
