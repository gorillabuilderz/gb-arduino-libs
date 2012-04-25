#include "Radial.h"

Radial::Radial(int16_t radius, double min, double max)
	: _radius(radius), _min(min), _max(max), _startRadians(0), _endRadians(2*PI)  {
}

Radial::Radial(int16_t radius, double min, double max, int16_t startDegrees, int16_t endDegrees)
	: _radius(radius), _min(min), _max(max), _startRadians(startDegrees*PI/180), _endRadians(endDegrees*PI/180) {
}

double Radial::getMin() {
	return _min;
}

double Radial::getMax() {
	return _max;
}

int16_t Radial::getX(double value) {
	return cos(getValueRadians(value))*_radius;
}

int16_t Radial::getY(double value) {
	return sin(getValueRadians(value))*_radius;
}

int16_t Radial::getXEnd(Widget *widget, double value) {
	return widget->getX() + getX(value);
}

int16_t Radial::getYEnd(Widget *widget, double value) {
	return widget->getY() + getY(value);
}

double Radial::getValueRadians(double value) {
	return ((value-_min)/(_max-_min))*(_endRadians-_startRadians)-PI/2 + _startRadians;
}
