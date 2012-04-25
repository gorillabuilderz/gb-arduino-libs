#include "RadialTicks.h"

RadialTicks::RadialTicks(GB4DLcdDriver &lcd, int16_t x, int16_t y, int16_t radius, double min, double max, double interval, int16_t tickRadius, int16_t color)
	: Line(lcd, x, y, 0, 0, color), _interval(interval), _tickRadius(tickRadius), _radial(new Radial(radius, min, max)) {

}

RadialTicks::RadialTicks(GB4DLcdDriver &lcd, int16_t x, int16_t y, int16_t radius, double min, double max, double interval, int16_t tickRadius, int16_t startDegrees, int16_t endDegrees, int16_t color)
	: Line(lcd, x, y, 0, 0, color), _interval(interval), _tickRadius(tickRadius), _radial(new Radial(radius, min, max, startDegrees, endDegrees)) {

}

void RadialTicks::clear() {
	_lcd->setPenSize(true);
	for(int index = _radial->getMin(); index < _radial->getMax(); index += _interval) {
		_lcd->drawCircle(_radial->getXEnd(this, index), _radial->getYEnd(this, index), _tickRadius, _backgroundColor);
	}
}

void RadialTicks::draw() {
	_lcd->setPenSize(true);
	for(int index = _radial->getMin(); index <= _radial->getMax(); index += _interval) {
		_lcd->drawCircle(_radial->getXEnd(this, index), _radial->getYEnd(this, index), _tickRadius, _color);
	}
}
