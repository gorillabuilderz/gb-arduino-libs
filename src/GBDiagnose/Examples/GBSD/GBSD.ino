#include <SPI.h>
#include <SD.h>

// Required for automated GB build verification, you won't need this in your sketch, the IDE takes care of it
#include "GBSD.h"

int SD_CARD_CHIP_SELECT = 4;

void setup() {
  delay(3000);

  Serial.begin(115200);
  Serial.println("\n\r\n\rGorillaBuilderz SD Diagnose Tool");
  Serial.println();

  doTestSD();
}

void doTestSD() {
  File testFile;
  String fileContents = "Testing";

  Serial.println("");
  Serial.print("SD Card: Initialising Pin: ");
  Serial.println(SD_CARD_CHIP_SELECT);

  pinMode(SD_CARD_CHIP_SELECT, OUTPUT);
  // If we're not already initialised and SD fails to initialise
  if (!SD.begin(SD_CARD_CHIP_SELECT)) {
    Serial.println("SD Card: Initialisation Failed");
    return;
  }

  // Remove the test file first
  SD.remove("test.txt");
  testFile = SD.open("test.txt", FILE_WRITE);
  
  if(testFile) {
    Serial.println("SD Card: Writing to file");
    testFile.println(fileContents);
    // close the file:
    testFile.close();
    
    Serial.println("SD Card: Write success");
  }
  else {
    Serial.println("SD Card: Failed to open file for write");
  }
  
  testFile = SD.open("test.txt");

  if(testFile) {
    Serial.println("SD Card: Reading file");
    
    char readByte;
    int index = 0;
    
    while(index < fileContents.length() && testFile.available()) {
        readByte = testFile.read();
        if(fileContents[index++] != readByte) {
          Serial.println("SD Card: File Corrupt");
          return;
        }
    }
    
    // If there's more bytes available (greater than null terminators etc), something is not quite right
    if(testFile.available() > 2) {
      Serial.println("SD Card: Additional bytes avail in file");                
    }

    // close the file:
    testFile.close();
    
    Serial.println("SD Card: Read success");
  }
  else {
    Serial.println("SD Card: Failed to open file for read");
  }
}

void loop() {
}
