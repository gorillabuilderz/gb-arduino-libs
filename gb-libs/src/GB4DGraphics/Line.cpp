#include "Line.h"

Line::Line(GB4DLcdDriver &lcd, int16_t x, int16_t y, int16_t x2, int16_t y2, int16_t color)
	: Widget(&lcd, x, y, color), _x2(x2), _y2(y2), _backgroundColor(SGC_COLORS.BLACK) {
}

void Line::clear() {
	_lcd->drawLine(_x, _y, _x2, _y2, _backgroundColor);
}

void Line::draw() {
	_lcd->drawLine(_x, _y, _x2, _y2, _color);
}
