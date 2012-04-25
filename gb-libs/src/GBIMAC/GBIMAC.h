#ifndef GBIMAC_h
#define GBIMAC_h

#include <Arduino.h>

static const uint8_t MAC_LENGTH = 6;
static const int DEFAULT_MAC_CHIP_SELECT = A2;

class GBIMAC
{
  public:
  	// Default contructor uses the default chip select A2
    GBIMAC();	
    // Use this to set a custom chip select
    GBIMAC(int chipSelect);	
    void read(byte macAddressArray[]);
    int getChipSelect();
    
    // Returns a string of the formatted mac address: "00:00:00:00:00:00"
    char *format(uint8_t macAddress[]);
  private:
    int _chipSelect;
    void initSPI();
};

#endif