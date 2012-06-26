/*
 * Circle.h
 *
 *  Created on: 14/03/2012
 *      Author: andre
 */

#ifndef RADIAL_GAUGE_H_
#define RADIAL_GAUGE_H_

#include <Arduino.h>
#include <GB4DLcdDriver.h>
#include "utility/Widget.h"
#include "RadialLine.h"

class RadialGauge : public Widget
{
  public:
	// TODO implement fill
	RadialGauge(GB4DLcdDriver &lcd, int16_t x, int16_t y, int16_t radius, double min, double max, int16_t color);
	RadialGauge(GB4DLcdDriver &lcd, int16_t x, int16_t y, int16_t radius, double min, double max, int16_t startDegrees, int16_t endDegrees, int16_t color);

    void clear();

    void draw();

    void setValue(double value);

  private:
    int16_t _radius;
    int16_t _backgroundColor;

    RadialLine *_line;
    // Larger tick with text label
    int16_t _majorTick;
    // Medium tick with no text label
    int16_t _mediumTix;
    // Smaller tick with no text label
    int16_t _minorTix;
};


#endif /* CIRCLE_H_ */
