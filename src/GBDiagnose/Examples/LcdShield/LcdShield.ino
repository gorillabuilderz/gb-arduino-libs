#include <GBStatus.h>
#include <Transport.h>
#include <GB4DLcdDriver.h>
#include <SPI.h>
#include <GB4DGraphics.h>
#include <GString.h>

// Update this to change from default
static const int CHIP_SELECT_PIN  = 2;

// This is usage to specify the chip select
SC16SpiTransport transport(CHIP_SELECT_PIN);
// This is usage to use default chip select
// Comment above and uncomment this line to initialise using defaults
//SC16SpiTransport transport;


// Initialise the SPI driver using the default chip select. 
// NOTE: Remember to have the UART/SPI switch on SPI when uploading this sketch and running it
//       using the SPI driver!
GB4DSPILcdDriver lcd(CHIP_SELECT_PIN);
// This is usage to use default chip select
// Comment above and uncomment this line to initialise using defaults
//GB4DSPILcdDriver lcd;

void setup() {
  delay(3000);
  
  Serial.begin(9600);
  Serial.println("\n\r\n\rGorillaBuilderz LCDSerial Diagnose Tool");
  Serial.println();
  Serial.print("Chip Select for SPI operations: ");
  Serial.println(CHIP_SELECT_PIN);
  Serial.println();
  Serial.print("Initialising transport to SPI/UART Bridge using chip select: ");
  Serial.println(transport.getChipSelect());
  Serial.println("...");
  if(transport.initialise()) {
    Serial.println("Success!");
  }
  else {
    Serial.println("FAIL");
    Serial.println("Halting tests");
    return;
  }

  Serial.println();
  Serial.print("Resetting LCD using pin: ");
  Serial.println(lcd.getLcdResetPin());
  Serial.println("Resetting in 2 seconds. You should see the LCD reset, the backlight will flick");
  delay(2000);
  lcd.reset();
  Serial.println("Done. Continuing in 6 seconds...");
  for(int counter = 6; counter > 0; counter--) {
    Serial.println(counter);
    delay(1000);
  }

  Serial.println();
  Serial.print("Initialising LCD using chip select: ");
  Serial.println(((SC16SpiTransport*)lcd.getTransport())->getChipSelect());
  Serial.println("...");
  
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
  Serial.println("You should see text");  
}

void loop() {
  Status.idle();
}
