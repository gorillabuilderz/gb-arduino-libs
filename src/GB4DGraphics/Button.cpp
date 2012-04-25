#include "Button.h"

Button::Button(GB4DLcdDriver &lcd, int16_t x, int16_t y, char* text)
	: Widget(&lcd, x, y, SGC_COLORS.GREEN)/*, _label(new Label(lcd, x,
	y, text))*//*, _rectangle(new Rectangle(lcd, x, y, SGC_COLORS.GREEN))*/ {

	SGC_FONT fontSize = SGC_FONT_SIZE.LARGEST;
	unsigned int textLength = strlen(text);
	//unsigned int textLength = 1;
	int8_t padding = 20;

	_label = new Label(lcd, x+padding/2, y+padding/2, fontSize, SGC_COLORS.WHITE, SGC_COLORS.BLUE, text);

	// Width of the rectangle is x + Total width of text (text length * width of character) + spaces (text length-1)
	_rectangle = new Rectangle(lcd, x, y,
			x+(textLength*fontSize.width)+(textLength-1)*2+padding,
			y+fontSize.height+padding-1,
			true,
			SGC_COLORS.GREEN);
}

Button::Button(GB4DLcdDriver &lcd, int16_t x, int16_t y, int16_t width, char* text)
		: Widget(&lcd, x, y, SGC_COLORS.GREEN) {

	SGC_FONT fontSize = SGC_FONT_SIZE.LARGEST;
	int8_t padding = 20;

	_label = new Label(lcd, x, y+padding/2, fontSize, SGC_COLORS.WHITE, SGC_COLORS.BLUE, width, Label::CENTER, text);

	// Width of the rectangle is x + Total width of text (text length * width of character) + spaces (text length-1)
	_rectangle = new Rectangle(lcd, x, y,
			x+width,
			y+fontSize.height+padding-1,
			true,
			SGC_COLORS.GREEN);
}

Button::Button(GB4DLcdDriver &lcd, Rectangle& rectangle, Label& label)
	: Widget(&lcd, rectangle.getX(), rectangle.getY(), SGC_COLORS.GREEN) {
	_rectangle = &rectangle;
	_label = &label;
}

void Button::setText(char* text) {
	_label->setText(text);
}

void Button::on() {
	_rectangle->setColor(SGC_COLORS.BLUE);
	draw();
}

void Button::off() {
	_rectangle->setColor(SGC_COLORS.GREEN);
	draw();
}

void Button::draw() {
	_rectangle->draw();
	_label->draw();
}
