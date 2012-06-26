/*
 * Line.h
 *
 *  Created on: 14/03/2012
 *      Author: andre
 */

#ifndef LINE_H_
#define LINE_H_

#include <Arduino.h>
#include <GB4DLcdDriver.h>
#include "utility/Widget.h"

class Line : public Widget
{
  public:
	Line(GB4DLcdDriver &lcd, int16_t x, int16_t y, int16_t x2, int16_t y2, int16_t color);

    void clear();

    void draw();

  protected:
  	int16_t _x2;
	int16_t _y2;

    int16_t _backgroundColor;
    uint8_t _fontSize;
};


#endif /* LINE_H_ */
