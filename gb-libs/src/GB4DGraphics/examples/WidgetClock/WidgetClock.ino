#include <GBStatus.h>
#include <Transport.h>
#include <GB4DLcdDriver.h>
#include <SPI.h>
#include <GB4DGraphics.h>
#include <GString.h>

// Initialise the SPI driver using the default chip select. 
// NOTE: Remember to have the UART/SPI switch on SPI when uploading this sketch and running it
//       using the SPI driver!
GB4DSPILcdDriver lcd;

// Uncomment the following line and comment above to use the serial driver
// NOTE: If using serial driver, remember to have the UART/SPI switch on SPI when uploading 
//       this sketch but switched to UART when running this sketch.
// GB4DSerialLcdDriver lcd;

RadialLine minuteLine(lcd, 100, 100, 85, 0, 60, SGC_COLORS.BLUE);
RadialLine hourLine(lcd, 100, 100, 45, 0, 12, SGC_COLORS.BLUE);
RadialLine secondLine(lcd, 100, 100, 87, 0, 60, SGC_COLORS.BLUE);
RadialTicks quarterTicks(lcd, 100, 100, 90, 0, 60, 15, 2, SGC_COLORS.WHITE);
RadialTicks fifthTicks(lcd, 100, 100, 90, 0, 60, 5, 1, SGC_COLORS.WHITE);

unsigned long startTime;
unsigned long previousSeconds;

static const long DAY = 86400000; // 86400000 milliseconds in a day
static const long HOUR = 3600000; // 3600000 milliseconds in an hour
static const long MINUTE = 60000; // 60000 milliseconds in a minute
static const long SECOND =  1000; // 1000 milliseconds in a second

void setup() {
  delay(3000);
  lcd.initialise();

  minuteLine.draw();
  hourLine.draw();
  secondLine.draw();
  fifthTicks.draw();  
  quarterTicks.draw();
  
  startTime = millis();
}
int count = 0;
void loop() {
  Status.idle();

  unsigned long milliSeconds = millis() - startTime;
  
  int seconds = (((milliSeconds % DAY) % HOUR) % MINUTE)/SECOND;
  
  if(seconds != previousSeconds) {  
    previousSeconds = seconds;
    int hours = (milliSeconds % DAY)/HOUR;                       //the remainder from days division (in milliseconds) divided by hours, this gives the full hours
    int minutes = ((milliSeconds % DAY) % HOUR)/MINUTE;         //and so on...

    secondLine.setValue(seconds);
    minuteLine.setValue(minutes);
    hourLine.setValue(hours);
  }
}
