#include "Label.h"

Label::Label(GB4DLcdDriver &lcd, int16_t x, int16_t y, char* text)
	: Widget(&lcd, x, y, SGC_COLORS.WHITE), _text(text), _alignment(Label::LEFT), _backgroundColor(SGC_COLORS.BLACK), _fontSize(SGC_FONT_SIZE.SMALL) {
}

Label::Label(GB4DLcdDriver &lcd, int16_t x, int16_t y, int16_t width, ALIGNMENT alignment, char* text)
	: Widget(&lcd, x, y, SGC_COLORS.WHITE), _width(width), _alignment(alignment), _text(text), _backgroundColor(SGC_COLORS.BLACK), _fontSize(SGC_FONT_SIZE.SMALL) {
}

Label::Label(GB4DLcdDriver &lcd, int16_t x, int16_t y, SGC_FONT fontSize, int16_t color, int16_t backgroundColor, char* text)
	: Widget(&lcd, x, y, color), _text(text), _alignment(Label::LEFT), _backgroundColor(backgroundColor), _fontSize(fontSize) {
}

Label::Label(GB4DLcdDriver &lcd, int16_t x, int16_t y, SGC_FONT fontSize, int16_t color, int16_t backgroundColor, int16_t width, ALIGNMENT alignment, char* text)
	: Widget(&lcd, x, y, color), _text(text), _backgroundColor(backgroundColor), _fontSize(fontSize), _width(width), _alignment(alignment) {

}

void Label::setText(char* text) {
	// Clear the previous text
	if(_text != NULL) {
		_lcd->drawGraphicsString(getXOffset(),_y, _fontSize, _backgroundColor, _text.getBuffer());
	}

	// Will convert to a GString and create a copy of the string
	_text = text;
	draw();
}

void Label::setText(String text) {
	GString newText = text;
	setText(newText.getBuffer());
}

void Label::clear() {
	setText("");
}

void Label::draw() {
	_lcd->drawGraphicsString(getXOffset(),_y, _fontSize, _color, _text.getBuffer());
}

int16_t Label::getXOffset() {
	switch(_alignment) {
	case CENTER:
		return (_width-getTextPixelLength())/2 + _x;
	case RIGHT:
		return _x+_width-getTextPixelLength();
	default:
		// Left is default
		return _x;
	}
}

int16_t Label::getTextPixelLength() {
	unsigned int textLength = _text.length();

	return (textLength*_fontSize.width)+(textLength-1);
}
