#include <GBStatus.h>
#include <Transport.h>
#include <GB4DLcdDriver.h>
#include <SPI.h>
#include <GB4DGraphics.h>
#include <GString.h>


int CHIP_SELECT_PIN  = 2;

SC16SpiTransport transport(CHIP_SELECT_PIN);


// Initialise the SPI driver using the default chip select. 
// NOTE: Remember to have the UART/SPI switch on SPI when uploading this sketch and running it
//       using the SPI driver!
GB4DSPILcdDriver lcd(CHIP_SELECT_PIN);


void setup() {
  delay(3000);
  
  Serial.begin(9600);
  Serial.println("\n\r\n\rGorillaBuilderz LCDSerial Diagnose Tool");
  Serial.println();
  Serial.print("Chip Select for SPI operations: ");
  Serial.println(CHIP_SELECT_PIN);
  Serial.println();
  Serial.println("Initialising transport to SPI/UART Brige....");
  if(transport.initialise()) {
    Serial.println("Success!");
  }
  else {
    Serial.println("FAIL");
    Serial.println("Halting tests");
    return;
  }

  Serial.println();
  Serial.println("Initialising LCD....");
  
  if(lcd.initialise()) {
    Serial.println("Success!");
  }
  else {
    Serial.println("FAIL");
    Serial.println("Halting tests");
    return;
  }
  
  Serial.println();
  Serial.println("Drawing text on LCD");
  Label label(lcd, 5, 1, "GorillaBuilderz!!!");
  label.draw();
  Serial.println("You shoul see text");  
}

void loop() {
  Status.idle();
}
