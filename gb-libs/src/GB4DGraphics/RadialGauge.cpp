#include "RadialGauge.h"

RadialGauge::RadialGauge(GB4DLcdDriver &lcd, int16_t x, int16_t y, int16_t radius, double min, double max, int16_t color)
	: Widget(&lcd, x, y, color), _radius(radius), _backgroundColor(SGC_COLORS.BLACK),
	  _line(new RadialLine(lcd, x, y, radius-2, min, max, color)) {
}

RadialGauge::RadialGauge(GB4DLcdDriver &lcd, int16_t x, int16_t y, int16_t radius, double min, double max, int16_t startDegrees, int16_t endDegrees, int16_t color)
	: Widget(&lcd, x, y, color), _radius(radius), _backgroundColor(SGC_COLORS.BLACK),
	  _line(new RadialLine(lcd, x, y, radius-2, min, max, startDegrees, endDegrees, color)) {
}

void RadialGauge::clear() {
	_lcd->setPenSize(false);
	_lcd->drawCircle(_x, _y, _radius, _backgroundColor);
	_line->clear();
}

void RadialGauge::setValue(double value) {
	_line->setValue(value);
}


void RadialGauge::draw() {
	_lcd->setPenSize(false);
	_lcd->drawCircle(_x, _y, _radius, _color);

	// Draw major ticks
	// Draw medium ticks
	// Draw minor ticks

	// Draw line
	_line->draw();
}
