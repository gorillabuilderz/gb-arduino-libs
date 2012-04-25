#include <GBStatus.h>
#include <GB4DLcdDriver.h>
#include <SPI.h>

//GB4DSerialLcdDriver lcd;
GB4DSPILcdDriver lcd;

void setup() {
  delay(3000);
    Serial.begin(9600);
    Serial.println("\n\r\n\rGB-LCD Routine");
    
  if(lcd.initialise()) {
    Serial.println("Bridge initialized successfully!"); 
      lcd.drawString(0,0, "GorillaBuilderz!!!");
//    lcd.version(true);
      // Different ways to set the background color
//  delay(1000);
//  Serial.println("Background!"); 
//  lcd.replaceBackgroundColor(SGC_COLORS.WHITE);
//  Serial.println("Set!");   
  }
  else {
    Serial.println("Could not initialize bridge, locking up.\n\r"); 
  }
}

void loop() {
  Status.idle();
}
