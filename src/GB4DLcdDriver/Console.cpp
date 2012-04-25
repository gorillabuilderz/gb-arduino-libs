#include "Console.h"

#include <Arduino.h>

Console::Console(GB4DLcdDriver *lcd) {
	_row = 0;
	_column = 0;
	_lcd = lcd;
	_font = SGC_FONT_SIZE.SMALL;
	_color = SGC_COLORS.WHITE;
}

void Console::setLcd(GB4DLcdDriver *lcd) {
	_lcd = lcd;
}

void Console::setColor(int16_t color) {
	_color = color;
}

int16_t Console::getColor() {
	return _color;
}

void Console::setFont(SGC_FONT font) {
	_font = font;
}

SGC_FONT Console::getFont() {
	return _font;
}

bool Console::initialise() {
	if(!_lcd->initialise()) {
		return false;
	}
	
	_lcd->clearScreen();
	return true;
}

void Console::println(const char *string) {
  println(_color, string);
}
void Console::print(const char *string) {
  print(_color, string);
}

void Console::println(int color, const char *string) {
  _lcd->drawString(_column,_row++,_font, color,string);
  _column = 0;
}

void Console::print(int color, const char *string) {
  _lcd->drawString(_column,_row,_font, color,string);
  _column += String(string).length();
}

void Console::print(int color, uint8_t byte) {
  _lcd->drawString(_column,_row, _font, color, byte);
  _column += 1;
}

void Console::print(uint8_t byte) {
  print(_color, byte);
}

void Console::println(int color, const char *title, const char *string) {
  print(title);
  println(color, string);  
}
