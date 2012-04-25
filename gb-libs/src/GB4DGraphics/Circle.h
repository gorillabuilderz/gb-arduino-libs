/*
 * Circle.h
 *
 *  Created on: 14/03/2012
 *      Author: andre
 */

#ifndef CIRCLE_H_
#define CIRCLE_H_

#include <Arduino.h>
#include <GB4DLcdDriver.h>
#include <utility/Widget.h>

class Circle : public Widget
{
  public:
	// TODO implement fill
	Circle(GB4DLcdDriver &lcd, int16_t x, int16_t y, int16_t radius, bool fill, int16_t color);

    void clear();

    void draw();

  private:
    bool _fill;

    int16_t _backgroundColor;
    int16_t _radius;
};


#endif /* CIRCLE_H_ */
