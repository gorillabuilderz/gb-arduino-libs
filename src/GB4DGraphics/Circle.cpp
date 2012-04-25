#include "Circle.h"

Circle::Circle(GB4DLcdDriver &lcd, int16_t x, int16_t y, int16_t radius, bool fill, int16_t color)
	: Widget(&lcd, x, y, color), _radius(radius), _fill(fill), _backgroundColor(SGC_COLORS.BLACK) {
}

void Circle::clear() {
	_lcd->setPenSize(_fill);
	_lcd->drawCircle(_x, _y, _radius, _backgroundColor);
}

void Circle::draw() {
	_lcd->setPenSize(_fill);
	_lcd->drawCircle(_x, _y, _radius, _color);
}
