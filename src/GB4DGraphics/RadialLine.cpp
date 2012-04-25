#include "RadialLine.h"

RadialLine::RadialLine(GB4DLcdDriver &lcd, int16_t x, int16_t y, int16_t radius, double min, double max, int16_t color)
	: Line(lcd, x, y, 0, 0, color), _radial(new Radial(radius, min, max)), _value(0) {

}

RadialLine::RadialLine(GB4DLcdDriver &lcd, int16_t x, int16_t y, int16_t radius, double min, double max, int16_t startDegrees, int16_t endDegrees, int16_t color)
	: Line(lcd, x, y, 0, 0, color), _radial(new Radial(radius, min, max, startDegrees, endDegrees)), _value(0) {

}

void RadialLine::setValue(double value) {
	clear();

	_value = value;

	draw();
}

void RadialLine::clear() {
	_lcd->drawLine(_x, _y,
			_radial->getXEnd(this, _value),
			_radial->getYEnd(this, _value),
			_backgroundColor);
}

void RadialLine::draw() {
	_lcd->drawLine(_x, _y,
			_radial->getXEnd(this, _value),
			_radial->getYEnd(this, _value),
			_color);
}
