#include "GB4DLcdDriver.h"

#include <Arduino.h>
#include <GBStatus.h>
#include <Transport.h>

SGC_COMMANDS_STRUCT SGC_COMMANDS;
SGC_EXT_COMMANDS_STRUCT SGC_EXT_COMMANDS;

// 0 : 5x7 internal font
// 1 : 8x8 internal font
// 2 : 8x12 internal font
// 3 : 12x16 internal font

// These values have been slightly adjusted
const SGC_FONT SGC_FONT_SIZE_STRUCT::SMALL		= {0x00, 5, 7};
const SGC_FONT SGC_FONT_SIZE_STRUCT::MEDIUM		= {0x01, 8, 8};
const SGC_FONT SGC_FONT_SIZE_STRUCT::LARGE		= {0x02, 8, 12};
const SGC_FONT SGC_FONT_SIZE_STRUCT::LARGEST	= {0x03, 10, 14};


GB4DLcdDriver::GB4DLcdDriver(Transport *transport) {
	_transport = transport;
}

bool GB4DLcdDriver::isAck(uint8_t reply) {
	return reply == SGC_COMMANDS.DISPLAY_ACK;
}

bool GB4DLcdDriver::isNack(uint8_t reply) {
	return reply == SGC_COMMANDS.DISPLAY_NACK;
}

/**
 * Seems like the LCD needs a bit of a delay after the CS deslect
 */
void GB4DLcdDriver::transportDeselect() {
	_transport->deselect();
	delay(10);
}

bool GB4DLcdDriver::initialise() {
	if(!_transport->initialise()) {
		return false;
	}

	reset();

  	// Wait > 500ms (+ more for memory cards)
  	// Large memory cards can take up to 3 seconds
  	delay(SGC_COMMANDS.INIT_DELAY);

  	_transport->select();
  	_transport->prepareWrite();
  	_transport->write(SGC_COMMANDS.AUTOBAUD);
  	transportDeselect();

  	return isAck(readReply());
}

void GB4DLcdDriver::reset() {
	pinMode(SGC_COMMANDS.LCD_PIN, OUTPUT);

	// Reset and initialise the display
  	digitalWrite(SGC_COMMANDS.LCD_PIN, LOW);
  	delay(10);
  	digitalWrite(SGC_COMMANDS.LCD_PIN, HIGH);
}

uint8_t GB4DLcdDriver::getLcdResetPin() {
	return SGC_COMMANDS.LCD_PIN;
}

void GB4DLcdDriver::version(boolean showOnScreen) {
	_transport->select();
	_transport->prepareWrite();
	_transport->write(SGC_COMMANDS.VERSION);
	_transport->write((byte)showOnScreen);
	transportDeselect();

	_transport->select();
	_transport->waitForData();
	transportDeselect();

	_transport->select();
	_transport->prepareRead();
	_transport->read();	// Device Type
	_transport->read();	// Hardware rev
	_transport->read();	// firmware rev
	_horizontalResolution = translateByteToHorizontalResolution(_transport->read());	// hor res
	_verticalResolution = translateByteToVerticalResolution(_transport->read());	// vert res
	transportDeselect();
}

bool GB4DLcdDriver::setBaudRate(uint8_t baudRate) {
	_transport->select();
	_transport->prepareWrite();
	_transport->write(SGC_COMMANDS.SETBAUD);

	switch(baudRate) {
		case SGC_BAUD_RATE_STRUCT::B9600:
			_transport->write(baudRate);
			break;
		case SGC_BAUD_RATE_STRUCT::B115200:
			_transport->write(baudRate);
			break;
	}

	transportDeselect();

	return true;
}

uint8_t GB4DLcdDriver::clearScreen() {
	_transport->select();
	_transport->prepareWrite();
  	_transport->write(SGC_COMMANDS.CLEAR_SCREEN);
  	transportDeselect();

  	return readReply();
}

uint8_t GB4DLcdDriver::readByte() {
	_transport->select();
	// Wait for a response
	_transport->waitForData();
	transportDeselect();

	_transport->select();
	_transport->prepareRead();
    uint8_t readByte = _transport->read();
    transportDeselect();

//    Serial.print("Read:");
//    Serial.println(readByte, HEX);

    return readByte;
}

uint8_t GB4DLcdDriver::readReply() {
	_transport->select();
	// Wait for a response
	_transport->waitForData();
	transportDeselect();

	_transport->select();
	_transport->prepareRead();
    uint8_t readByte = _transport->read();
    transportDeselect();

//    Serial.print("Read:");
//    Serial.println(readByte, HEX);

    if(isAck(readByte)) {
    	Status.success();
    }
    else if(isNack(readByte)) {
      Status.error();
    }
    else {
    	Status.failure();
	}

	return readByte;
}

uint8_t GB4DLcdDriver::setBackgroundColor(int16_t color) {
	_transport->select();
	_transport->prepareWrite();
	_transport->write(SGC_COMMANDS.SET_BACKGROUND_COLOR);
	writeInt(color);
	transportDeselect();

	return readReply();
}

uint8_t GB4DLcdDriver::replaceBackgroundColor(int16_t color) {
	_transport->select();
	_transport->prepareWrite();
	_transport->write(SGC_COMMANDS.REPLACE_BACKGROUND_COLOR);
	writeInt(color);
	transportDeselect();

	return readReply();
}

uint8_t GB4DLcdDriver::setPenSize(bool solid) {
	_transport->select();
	_transport->prepareWrite();
	_transport->write(SGC_COMMANDS.SET_PEN_SIZE);
	_transport->write(!solid);
	transportDeselect();

	return readReply();
}

uint8_t GB4DLcdDriver::enableBacklight(bool enable) {
	_transport->select();
	_transport->prepareWrite();
	_transport->write(SGC_COMMANDS.DISPLAY_FUNCTIONS);
	_transport->write(DISPLAY_FUNCTIONS_MODE.BACKLIGHT_CONTROL);
	_transport->write(enable);
	transportDeselect();

	return readReply();
}

uint8_t GB4DLcdDriver::enableTouch(bool enable) {
	_transport->select();
	_transport->prepareWrite();
	_transport->write(SGC_COMMANDS.DISPLAY_FUNCTIONS);
	_transport->write(DISPLAY_FUNCTIONS_MODE.TOUCH_CONTROL);
	_transport->write(!enable);
	transportDeselect();

	uint8_t response = readReply();

	if(isAck(response) && enable) {
		_transport->select();
		_transport->prepareWrite();
		_transport->write(SGC_COMMANDS.DISPLAY_FUNCTIONS);
		_transport->write(DISPLAY_FUNCTIONS_MODE.TOUCH_CONTROL);
		_transport->write(0x02);
		transportDeselect();

		return readReply();
	}

	return response;
}

uint8_t GB4DLcdDriver::setImageFormat(uint8_t format) {
	_transport->select();
	_transport->prepareWrite();
	_transport->write(SGC_COMMANDS.DISPLAY_FUNCTIONS);
	_transport->write(DISPLAY_FUNCTIONS_MODE.IMAGE_FORMAT);
	_transport->write(format);
	transportDeselect();

	return readReply();
}

uint8_t GB4DLcdDriver::getTouchActivity() {
	uint8_t cnt=0,tBuff[6];
	_transport->select();
	_transport->prepareWrite();
	_transport->write(0x6f);
	_transport->write((uint8_t)0x04);
	transportDeselect();

	_transport->select();
	_transport->prepareRead();
	while(cnt<5){
	tBuff[cnt++] = _transport->read();
	}

	transportDeselect();

	if( (tBuff[1]>=1)&&(tBuff[1]<=3))
		return tBuff[1];
	else
		return false;
}

uint8_t GB4DLcdDriver::getTouchXYCoord(uint16_t &x, uint16_t &y) {
	uint8_t cnt=0,tBuff[6];
	_transport->select();
	_transport->prepareWrite();
	_transport->write(0x6f);
	_transport->write((uint8_t)0x05);
	transportDeselect();

	_transport->select();
	_transport->prepareRead();
	while(cnt<5){
	tBuff[cnt++] = _transport->read();
	}

	transportDeselect();
	x = (uint16_t)(tBuff[0] << 8) + tBuff[1];
	y = (uint16_t)(tBuff[2] << 8) + tBuff[3];
}

uint8_t GB4DLcdDriver::detectTouchRegion(uint16_t xTL, uint16_t yTL, uint16_t xBR, uint16_t yBR) {
	uint8_t cnt=0,tBuff[6];
	_transport->select();
	_transport->prepareWrite();
	_transport->write(0x75);

	_transport->write((byte)(xTL >> 8));
	_transport->write((byte)(xTL & 0xff));
	_transport->write((byte)(yTL >> 8));
	_transport->write((byte)(yTL & 0xff));
	_transport->write((byte)(xBR >> 8));
	_transport->write((byte)(xBR & 0xff));
	_transport->write((byte)(yBR >> 8));
	_transport->write((byte)(yBR & 0xff));
	transportDeselect();

	_transport->select();
	_transport->prepareRead();

	tBuff[0] = _transport->read();
	tBuff[1] = _transport->read();
	transportDeselect();
	return tBuff[0];
}

uint8_t GB4DLcdDriver::drawString(uint8_t column, uint8_t row, SGC_FONT font, int16_t color, const char *string) {
	_transport->select();
	_transport->prepareWrite();
	_transport->write(SGC_COMMANDS.DRAW_STRING);
  	_transport->write(column);
  	_transport->write(row);
  	_transport->write(font.id);
  	writeInt(color);
  	_transport->write(string);
  	_transport->write(SGC_COMMANDS.STRING_TERMINATOR);
	transportDeselect();

  	return readReply();
}

uint8_t GB4DLcdDriver::drawString(uint8_t column, uint8_t row, SGC_FONT font, int16_t color, const char character) {
	_transport->select();
	_transport->prepareWrite();
	_transport->write(SGC_COMMANDS.DRAW_STRING);
  	_transport->write(column);
  	_transport->write(row);
  	_transport->write(font.id);
  	writeInt(color);
  	_transport->write(character);
  	_transport->write(SGC_COMMANDS.STRING_TERMINATOR);
	transportDeselect();

  	return readReply();
}

uint8_t GB4DLcdDriver::drawGraphicsString(int16_t x, int16_t y, SGC_FONT font, int16_t color, const char *string) {
	_transport->select();
	_transport->prepareWrite();
	_transport->write(SGC_COMMANDS.DRAW_GRAPHICS_STRING);
	writeInt(x);
	writeInt(y);
  	_transport->write(font.id);
  	writeInt(color);
  	_transport->write(1);
  	_transport->write(1);
  	_transport->write(string);
  	_transport->write(SGC_COMMANDS.STRING_TERMINATOR);
	transportDeselect();

  	return readReply();
}

uint8_t GB4DLcdDriver::drawCircle(int16_t x, int16_t y, int16_t radius, int16_t color) {
	_transport->select();
	_transport->prepareWrite();
  	_transport->write(SGC_COMMANDS.DRAW_CIRCLE);
  	writeInt(x);
  	writeInt(y);
	writeInt(radius);
	writeInt(color);
	transportDeselect();

  	return readReply();
}

uint8_t GB4DLcdDriver::drawTriangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, int16_t color) {
	_transport->select();
	_transport->prepareWrite();
  	_transport->write(SGC_COMMANDS.DRAW_TRIANGLE);
  	writeInt(x1);
  	writeInt(y1);
  	writeInt(x2);
  	writeInt(y2);
  	writeInt(x3);
	writeInt(y3);
	writeInt(color);
	transportDeselect();

  	return readReply();
}

uint8_t GB4DLcdDriver::drawPixel(int16_t x, int16_t y, int16_t color) {
	_transport->select();
	_transport->prepareWrite();
  	_transport->write(SGC_COMMANDS.DRAW_PIXEL);
  	writeInt(x);
  	writeInt(y);
	writeInt(color);
	transportDeselect();

  	return readReply();
}

uint8_t GB4DLcdDriver::drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t color) {
	return commandFourIntsColor(SGC_COMMANDS.DRAW_LINE, x1, y1, x2, y2, color);
}

uint8_t GB4DLcdDriver::drawRectangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t color) {
	return commandFourIntsColor(SGC_COMMANDS.DRAW_RECTANGLE, x1, y1, x2, y2, color);
}

uint8_t GB4DLcdDriver::drawElipse(int16_t x, int16_t y, int16_t rx, int16_t ry, int16_t color) {
	return commandFourIntsColor(SGC_COMMANDS.DRAW_ELIPSE, x, y, rx, ry, color);
}

uint8_t GB4DLcdDriver::commandFourIntsColor(uint8_t command, int16_t int1, int16_t int2, int16_t int3, int16_t int4, int16_t color) {
	_transport->select();
	_transport->prepareWrite();
  	_transport->write(command);
  	writeInt(int1);
  	writeInt(int2);
  	writeInt(int3);
	writeInt(int4);
	writeInt(color);
	transportDeselect();

  	return readReply();
}

uint8_t GB4DLcdDriver::screenCopyPaste(int16_t xs, int16_t ys, int16_t xd, int16_t yd, int16_t width, int16_t height) {
	_transport->select();
	_transport->prepareWrite();
  	_transport->write(SGC_COMMANDS.COPY_PASTE);
  	writeInt(xs);
  	writeInt(ys);
  	writeInt(xd);
	writeInt(yd);
	writeInt(width);
	writeInt(height);
	transportDeselect();

  	return readReply();
}

uint8_t GB4DLcdDriver::SDDisplayIcon(uint16_t xs, int16_t ys, uint32_t sector) {
	_transport->select();
	_transport->prepareWrite();
  	_transport->write(SGC_EXT_COMMANDS.EXT_COMMAND_ID);
  	_transport->write(SGC_EXT_COMMANDS.SD_DISPLAY_IMAGE_ICON);
  	writeInt(xs);
  	writeInt(ys);
  	_transport->write((uint8_t)(sector>>16));
	writeInt((uint16_t)sector);
	transportDeselect();

  	return readReply();
}

void GB4DLcdDriver::writeInt(int16_t value) {
  _transport->write(value >> 8 & 0xFF);
  _transport->write(value & 0xFF);
}

int16_t GB4DLcdDriver::getHorizontalResolution() {
	return _horizontalResolution;
}

int16_t GB4DLcdDriver::getVerticalResolution() {
	return _verticalResolution;
}

int16_t GB4DLcdDriver::translateByteToHorizontalResolution(uint8_t value) {
	switch(value) {
		case 0x22: return 220;
		case 0x28: return 128;
		case 0x32: return 320;
		case 0x60: return 160;
		case 0x64: return 64;
		case 0x76: return 176;
		case 0x96: return 96;
		default: return 0;
	}
}

int16_t GB4DLcdDriver::translateByteToVerticalResolution(uint8_t value) {
	return translateByteToHorizontalResolution(value);
}

Transport *GB4DLcdDriver::getTransport() {
	return _transport;
}
