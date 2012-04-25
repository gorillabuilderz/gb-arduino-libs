#include "Rectangle.h"

Rectangle::Rectangle(GB4DLcdDriver &lcd, int16_t x, int16_t y, int16_t x2, int16_t y2, bool fill, int16_t color)
	: Widget(&lcd, x, y, color), _x2(x2), _y2(y2), _fill(fill), _backgroundColor(SGC_COLORS.BLACK) {
}

void Rectangle::clear() {
	_lcd->setPenSize(_fill);
	_lcd->drawRectangle(_x, _y, _x2, _y2, _backgroundColor);
}

void Rectangle::draw() {
	_lcd->setPenSize(_fill);
	_lcd->drawRectangle(_x, _y, _x2, _y2, _color);
}
