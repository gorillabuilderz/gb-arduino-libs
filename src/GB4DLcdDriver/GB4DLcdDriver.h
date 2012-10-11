#ifndef GB4DLcdDriver_h
#define GB4DLcdDriver_h

#include <Arduino.h>
#include <GBStatus.h>
#include <Transport.h>
#include <SerialTransport.h>
#include <SpiTransport.h>
#include <SC16SpiTransport.h>

struct SGC_COMMANDS_STRUCT {
	static const int	 	INIT_DELAY					= 2000;
	static const uint8_t 	LCD_PIN 					= 0x07;

	// Flow control
	static const uint8_t 	AUTOBAUD 					= 0x55;
	static const uint8_t 	SETBAUD 					= 0x51;
	static const uint8_t 	DISPLAY_ACK					= 0x06;
	static const uint8_t 	DISPLAY_NACK 				= 0x15;
	static const uint8_t 	STRING_TERMINATOR			= 0x00;

	// Utilities
	static const uint8_t 	VERSION		 				= 0x56;
	static const uint8_t 	CLEAR_SCREEN 				= 0x45;
	static const uint8_t 	DISPLAY_FUNCTIONS			= 0x59;
	static const uint8_t 	TOUCH_COORDINATES			= 0x6F;

	// Drawing
	static const uint8_t 	DRAW_CIRCLE 				= 0x43;
	static const uint8_t 	DRAW_TRIANGLE 				= 0x47;
	static const uint8_t 	DRAW_LINE	 				= 0x4C;
	static const uint8_t 	DRAW_POLYGON 				= 0x67;
	static const uint8_t 	DRAW_RECTANGLE 				= 0x72;
	static const uint8_t 	DRAW_ELIPSE 				= 0x65;
	static const uint8_t 	DRAW_PIXEL	 				= 0x50;
	static const uint8_t 	COPY_PASTE	 				= 0x63;
	static const uint8_t 	REPLACE_BACKGROUND_COLOR	= 0x42;
	static const uint8_t 	SET_BACKGROUND_COLOR		= 0x4B;
	static const uint8_t 	SET_PEN_SIZE				= 0x70;


	static const uint8_t 	FONT_SIZE					= 0x46;
	static const uint8_t 	DRAW_STRING					= 0x73;
	static const uint8_t 	DRAW_GRAPHICS_STRING		= 0x53;
};

struct DISPLAY_FUNCTIONS_MODE_STRUCT {
	static const uint8_t BACKLIGHT_CONTROL	= 0x00;
	static const uint8_t TOUCH_CONTROL		= 0x05;
};

extern DISPLAY_FUNCTIONS_MODE_STRUCT DISPLAY_FUNCTIONS_MODE;

struct SGC_COLORS_STRUCT {
	static const int16_t WHITE 	= 0xFFFF;
	static const int16_t BLACK 	= 0x0000;
	static const int16_t RED 	= 0xF800;
	static const int16_t BLUE 	= 0x001F;
	static const int16_t GREEN 	= 0x03E0;
	static const int16_t CYAN   = 0x07FF;
	static const int16_t YELLOW = 0xFFE0;
	static const int16_t MAGENTA = 0xF81F;
};

struct SGC_FONT {
	uint8_t id;
	uint8_t width;
	uint8_t height;
};

struct SGC_FONT_SIZE_STRUCT {
	static const SGC_FONT SMALL;
	static const SGC_FONT MEDIUM;
	static const SGC_FONT LARGE;
	static const SGC_FONT LARGEST;
};


struct SGC_BAUD_RATE_STRUCT {
	static const uint8_t B9600		= 0x06;
	static const uint8_t B115200	= 0x0D;
};

extern SGC_COLORS_STRUCT SGC_COLORS;
extern SGC_FONT_SIZE_STRUCT SGC_FONT_SIZE;
extern SGC_BAUD_RATE_STRUCT SGC_BAUD_RATE;





class GB4DLcdDriver
{
  public:
    GB4DLcdDriver(Transport *transport);

    // Utility methods
    virtual bool initialise();
    void reset();
    void version(boolean showOnScreen);
    bool isAck(uint8_t reply);
    bool isNack(uint8_t reply);
    uint8_t getLcdResetPin();
    int16_t getHorizontalResolution();
    int16_t getVerticalResolution();

    // Display Functions
    uint8_t clearScreen();
    uint8_t setBackgroundColor(int16_t color);
    uint8_t replaceBackgroundColor(int16_t color);
    uint8_t setPenSize(bool solid);
    uint8_t enableBacklight(bool enable);
    uint8_t enableTouch(bool enable);
    uint8_t getTouchActivity();
    char* getTouchCoordinates();


    uint8_t drawString(uint8_t column, uint8_t row, SGC_FONT font, int16_t color, const char *string);
    uint8_t drawString(uint8_t column, uint8_t row, SGC_FONT font, int16_t color, const char character);
    uint8_t drawGraphicsString(int16_t x, int16_t y, SGC_FONT font, int16_t color, const char *string);

    uint8_t drawCircle(int16_t x, int16_t y, int16_t radius, int16_t color);
    uint8_t drawTriangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, int16_t color);
    uint8_t drawRectangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t color);
    uint8_t drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t color);
    uint8_t drawElipse(int16_t x, int16_t y, int16_t rx, int16_t ry, int16_t color);
    uint8_t drawPixel(int16_t x, int16_t y, int16_t color);
	//void drawPolygon();
    uint8_t screenCopyPaste(int16_t xs, int16_t ys, int16_t xd, int16_t yd, int16_t width, int16_t height);

	Transport *getTransport();

  private:
	int16_t _horizontalResolution;
    int16_t _verticalResolution;

	// Utility methods
    uint8_t commandFourIntsColor(uint8_t command, int16_t value1, int16_t value2, int16_t value3, int16_t value4, int16_t color);
	void writeInt(int16_t value);
	int16_t translateByteToHorizontalResolution(uint8_t value);
	int16_t translateByteToVerticalResolution(uint8_t value);

  protected:
	Transport *_transport;
    bool setBaudRate(uint8_t baudRate);
	uint8_t readByte();
	uint8_t readReply();
};

class GB4DSerialLcdDriver : public GB4DLcdDriver
{
	public:
		GB4DSerialLcdDriver() : GB4DLcdDriver(&_serialTransport) {}
	private:
		SerialTransport _serialTransport;
};

class GB4DSPILcdDriver : public GB4DLcdDriver
{
	public:
  		static const int DEFAULT_CHIPSELECT = 2;

		GB4DSPILcdDriver(int chipSelect = DEFAULT_CHIPSELECT) : GB4DLcdDriver(&_spiTransport) {
			_spiTransport.setChipSelect(chipSelect);
		}

	    bool setBaudRate(uint8_t baudRate) {
	        if(!GB4DLcdDriver::setBaudRate(baudRate)) {
	    		return false;
	    	}
	    	switch(baudRate) {
				case SGC_BAUD_RATE_STRUCT::B9600:
					_spiTransport.setBaudRate(SC16IS740_BAUDRATE.B9600);
					break;
				case SGC_BAUD_RATE_STRUCT::B115200:
					_spiTransport.setBaudRate(SC16IS740_BAUDRATE.B115200);
					break;

	    	}
	    	// Re initialise the spi bridge with new board
	    	_spiTransport.initialise();

	    	// Read the reply from the LCD
	    	return isAck(SGC_COMMANDS_STRUCT::DISPLAY_ACK);
	    }

	    bool initialise() {
	    	if(!GB4DLcdDriver::initialise()) {
	    		return false;
	    	}

	    	if(!setBaudRate(SGC_BAUD_RATE.B115200)) {
	    		return false;
	    	}

			// Delay for things to settle down
			delay(20);

	    	return true;
	    }

	private:
		SC16SpiTransport _spiTransport;
};

class GB4DMOTGLcdDriver : public GB4DLcdDriver
{
	public:
  		static const int DEFAULT_CHIPSELECT = 7;

		GB4DMOTGLcdDriver(int chipSelect = DEFAULT_CHIPSELECT) : GB4DLcdDriver(&_spiTransport) {
			_spiTransport.setChipSelect(chipSelect);
		}

		/**
		 * Nothing really to do in reset other than clear the screen. Would be nice to have
		 * some type of reset control line.
		 */
		bool initialise() {
			clearScreen();
			return true;
		}

	private:
		SpiTransport _spiTransport;
};

#endif
