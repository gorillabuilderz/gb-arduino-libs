/*
 * Line.h
 *
 *  Created on: 14/03/2012
 *      Author: andre
 */

#ifndef RADIAL_TICKS_H_
#define RADIAL_TICKS_H_

#include <Arduino.h>
#include <GB4DLcdDriver.h>
#include "utility/Widget.h"
#include "Line.h"
#include "utility/Radial.h"

class RadialTicks : public Line
{
  public:
	RadialTicks(GB4DLcdDriver &lcd, int16_t x, int16_t y, int16_t radius, double min, double max, double interval, int16_t tickRadius, int16_t color);
	RadialTicks(GB4DLcdDriver &lcd, int16_t x, int16_t y, int16_t radius, double min, double max, double interval, int16_t tickRadius, int16_t startDegrees, int16_t endDegrees, int16_t color);

    void clear();

    void draw();

  private:
  	int16_t _radius;
  	int16_t _tickRadius;

    double _interval;

    Radial *_radial;
};


#endif /* LINE_H_ */
