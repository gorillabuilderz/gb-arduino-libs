#ifndef Console_h
#define Console_h

#include <Arduino.h>
#include <GB4DLcdDriver.h>

class Console
{
  public:
    Console(GB4DLcdDriver *lcd);
    
    // Utility methods
    bool initialise();
    void clear();
    
    void setColor(int16_t color);
    int16_t getColor();
    void setFont(SGC_FONT font);
    SGC_FONT getFont();

	void println(const char *string);
	void println(const uint8_t byte);
	void println(int color, const char *string);
	void println(int color, const uint8_t byte);
	void println(int color, const char *title, const char *string);
	void print(int color, const char *string);
	void print(int color, const uint8_t byte);	
	void print(const char *string);
	void print(const uint8_t byte);
		
	GB4DLcdDriver *getLcd();
	void setLcd(GB4DLcdDriver *lcd);
	
  private:
  	int16_t _row;
	int16_t _column;
  
	int16_t _columns;
    int16_t _rows;

    int16_t _color;
    SGC_FONT _font;
	
	GB4DLcdDriver *_lcd;
};

#endif
