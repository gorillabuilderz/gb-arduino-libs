#include <GBStatus.h>
#include <Transport.h>
#include <GB4DLcdDriver.h>
#include <Console.h>
#include <SPI.h>
#include <GBIMAC.h>
#include <SD.h>

// Required for automated GB build verification, you won't need this in your sketch, the IDE takes care of it
#include "GB4DLcd1Verify.h"

int SD_CARD_CHIP_SELECT = 3;
boolean sdInitialised = false;
GB4DSerialLcdDriver serialLcd;
GB4DSPILcdDriver spiLcd;

Console console(&serialLcd);

GBIMAC mac(4);
byte macAddress[MAC_LENGTH];;

File testFile;
String fileContents = "Testing";

void welcome(const char *mode) {
  console.println("GorillaBuilderz");
  console.println("4D LCD Shield 1 Verification");  
  console.print("Test Mode: ");
  console.println(SGC_COLORS.GREEN, mode);  
}

void testIMAC() {
  console.println("");
  console.println("GB IMAC: Reading...");

  mac.read(macAddress);
  console.println(SGC_COLORS.GREEN, "GB IMAC: ", mac.format(macAddress));  
}

void testSD() {
  doTestSD();
  // Always clean up the file
  SD.remove("test.txt");    
}

void doTestSD() {
  pinMode(SD_CARD_CHIP_SELECT, OUTPUT);
  console.println("");
  console.print("SD Card: Initialising Pin: ");
  char chipSelectBuffer[3];
  String(SD_CARD_CHIP_SELECT, DEC).toCharArray(chipSelectBuffer, 3);
  console.println(chipSelectBuffer);
  
  // If we're not already initialised and SD fails to initialise
  if (!sdInitialised && !SD.begin(SD_CARD_CHIP_SELECT)) {
    console.println(SGC_COLORS.RED, "SD Card: ", "Initialisation Failed");
    return;
  }
  
  // SD is initialised
  sdInitialised = true;
  
  // Remove the test file first
  SD.remove("test.txt");
  testFile = SD.open("test.txt", FILE_WRITE);
  
  if(testFile) {
    console.println("SD Card: Writing to file");
    testFile.println(fileContents);
    // close the file:
    testFile.close();
    
    console.println(SGC_COLORS.GREEN, "SD Card: ", "Write success");
  }
  else {
    console.println(SGC_COLORS.RED, "SD Card: ", "Failed to open file for write");
  }
  
  testFile = SD.open("test.txt");
  
  if(testFile) {
    console.println("SD Card: Reading file");
    
    char readByte;
    int index = 0;
    
    while(index < fileContents.length() && testFile.available()) {
        readByte = testFile.read();
        if(fileContents[index++] != readByte) {
          console.println(SGC_COLORS.RED, "SD Card: ", "File Corrupt");
          return;
        }
    }
    
    // If there's more bytes available (greater than null terminators etc), something is not quite right
    if(testFile.available() > 2) {
      console.println(SGC_COLORS.RED, "SD Card: ", "Additional bytes avail in file");                
    }
    // close the file:
    testFile.close();
    
    console.println(SGC_COLORS.GREEN, "SD Card: ", "Read success");
  }
  else {
    console.println(SGC_COLORS.RED, "SD Card: ", "Failed to open file for read");
  }
  
}

void testSpiLcd() {
  console.println("");
  console.println("SPI LCD: Test Ready");
  console.println("SPI LCD: Flick the switch!! ");  
  console.println("SPI LCD: Will begin in 3 seconds... ");
  delay(3000);
}

void runTest(GB4DLcdDriver lcd, const char *mode) {
  lcd.initialise();
  console.setLcd(&lcd);
  
  welcome(mode);
  
  testIMAC();
  testSD();
}

void setup() {
  delay(3000);
  
  runTest(serialLcd, "Serial LCD");
  testSpiLcd();

  runTest(spiLcd, "SPI LCD");  
  
  console.println("");    
  console.println("Finished");  
}

void loop() {
  Status.idle();
}
