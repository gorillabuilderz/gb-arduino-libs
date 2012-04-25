#include "Widget.h"

Widget::Widget(GB4DLcdDriver *lcd, int16_t x, int16_t y, int16_t color)
	: _lcd(lcd), _x(x), _y(y), _color(color) {
}
void Widget::setColor(int16_t color){
	_color = color;
}

int16_t Widget::getColor() {
	return _color;
}
int16_t Widget::getX() {
	return _x;
}

int16_t Widget::getY() {
	return _y;
}
/*

int Widget::getWidth() {return 0;}
int Widget::getHeight() {return 0;}

*/
void Widget::clear() {}
void Widget::draw() {}
