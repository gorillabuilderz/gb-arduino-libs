/*
 * Line.h
 *
 *  Created on: 14/03/2012
 *      Author: andre
 */

#ifndef RADIAL_H_
#define RADIAL_H_

#include <Arduino.h>
#include "Widget.h"

class Radial
{
  public:
	Radial(int16_t radius, double min, double max);
	Radial(int16_t radius, double min, double max, int16_t startDegrees, int16_t endDegrees);

	double getMin();
	double getMax();
    int16_t getX(double value);
    int16_t getY(double value);
    int16_t getXEnd(Widget *widget, double value);
    int16_t getYEnd(Widget *widget, double value);
    double getValueRadians(double value);

  private:
  	int16_t _radius;

    double _min;
    double _max;
    double _interval;
    double _startRadians;
    double _endRadians;

};


#endif /* LINE_H_ */
