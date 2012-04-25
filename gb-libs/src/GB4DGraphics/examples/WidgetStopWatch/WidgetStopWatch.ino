#include <GBStatus.h>
#include <Transport.h>
#include <GB4DLcdDriver.h>
#include <SPI.h>
#include <GB4DGraphics.h>
#include <GString.h>

#include <stdio.h>

// Initialise the SPI driver using the default chip select. 
// NOTE: Remember to have the UART/SPI switch on SPI when uploading this sketch and running it
//       using the SPI driver!
GB4DSPILcdDriver lcd;

// Uncomment the following line and comment above to use the serial driver
// NOTE: If using serial driver, remember to have the UART/SPI switch on SPI when uploading 
//       this sketch but switched to UART when running this sketch.
// GB4DSerialLcdDriver lcd;

Label timeLabel(lcd, 0, 25, SGC_FONT_SIZE.LARGEST, SGC_COLORS.WHITE, SGC_COLORS.BLACK, 190, Label::CENTER, "00:00:00");

Button start(lcd, 10, 70, 80, "Start");
Button stop(lcd, 100, 70, 80, "Stop");
Rectangle border(lcd,1,1,190,115, false, SGC_COLORS.RED);

unsigned long startTime;
unsigned long previousSeconds;

static const long DAY = 86400000; // 86400000 milliseconds in a day
static const long HOUR = 3600000; // 3600000 milliseconds in an hour
static const long MINUTE = 60000; // 60000 milliseconds in a minute
static const long SECOND =  1000; // 1000 milliseconds in a second

Label touchLabel(lcd, 0, 200, "No Touch");

void setup() {
  delay(3000);
  lcd.initialise();

  border.draw();
  timeLabel.draw();
  start.draw();
  stop.draw();
  
  if(lcd.isAck(lcd.enableTouch(true))) {
    touchLabel.draw();
  }
  else {
    touchLabel.setText("Failed Initialise Touch");
  }
  
  startTime = millis();
}

void loop() {
  Status.idle();
  
  unsigned long milliSeconds = millis() - startTime;
  
  int seconds = (((milliSeconds % DAY) % HOUR) % MINUTE)/SECOND;
  
  if(seconds != previousSeconds) {  
    previousSeconds = seconds;
    int days = milliSeconds/DAY ;                                //number of days
    int hours = (milliSeconds % DAY)/HOUR;                       //the remainder from days division (in milliseconds) divided by hours, this gives the full hours
    int minutes = ((milliSeconds % DAY) % HOUR)/MINUTE;         //and so on...
  
  
    char* string = "00:00:32";
  
    sprintf(string, "%02i:%02i:%02i",
  		hours,
  		minutes,
  		seconds);
    //timeLabel.setText(string);
  }

  delay(500);
  uint8_t response = lcd.getTouchActivity();
  if(response) {
    String responseString = "I've been touched:";
    responseString += response;
    touchLabel.setText(responseString);
  }
  else {
    String responseString = "No touch:";
    responseString += response;
    touchLabel.setText(responseString);
  }
}

