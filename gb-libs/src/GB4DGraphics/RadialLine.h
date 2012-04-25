/*
 * Line.h
 *
 *  Created on: 14/03/2012
 *      Author: andre
 */

#ifndef RADIAL_LINE_H_
#define RADIAL_LINE_H_

#include <Arduino.h>
#include <GB4DLcdDriver.h>
#include <utility/Widget.h>
#include <Line.h>
#include "utility/Radial.h"

class RadialLine : public Line
{
  public:
	RadialLine(GB4DLcdDriver &lcd, int16_t x, int16_t y, int16_t radius, double min, double max, int16_t color);
	RadialLine(GB4DLcdDriver &lcd, int16_t x, int16_t y, int16_t radius, double min, double max, int16_t startDegrees, int16_t endDegrees, int16_t color);

    void clear();

    void draw();

    void setValue(double value);

  private:
  	double _value;

  	Radial *_radial;
};


#endif /* LINE_H_ */
