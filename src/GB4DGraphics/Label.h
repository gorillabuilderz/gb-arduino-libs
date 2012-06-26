#ifndef Label_h
#define Label_h

#include <Arduino.h>
#include <GB4DLcdDriver.h>
#include "utility/Widget.h"
#include <GString.h>

class Label : public Widget
{
  public:
	enum ALIGNMENT { LEFT, RIGHT, CENTER };

    Label(GB4DLcdDriver &lcd, int16_t x, int16_t y, char* text);
    // Define static width and horizontal text alignment
    Label(GB4DLcdDriver &lcd, int16_t x, int16_t y, int16_t width, ALIGNMENT alignment, char* text);
    Label(GB4DLcdDriver &lcd, int16_t x, int16_t y, SGC_FONT fontSize, int16_t color, int16_t backgroundColor, char* text);
    Label(GB4DLcdDriver &lcd, int16_t x, int16_t y, SGC_FONT fontSize, int16_t color, int16_t backgroundColor, int16_t width, ALIGNMENT alignment, char* text);

    char* getText();
    void setText(char* text);
    void setText(String text);
    void clear();

    void draw();

    int16_t getTextPixelLength();

  private:
    ALIGNMENT _alignment;
    int16_t _width;
    int16_t _backgroundColor;
    SGC_FONT _fontSize;

    GString _text;

    /**
     * Returns the x offset with respect to width and alignment
     */
    int16_t getXOffset();
};

#endif
