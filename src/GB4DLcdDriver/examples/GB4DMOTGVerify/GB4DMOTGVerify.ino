#include <GBStatus.h>
#include <Transport.h>
#include <GB4DLcdDriver.h>
#include <SPI.h>
#include <GBIMAC.h>
#include <SD.h>

// Required for automated GB build verification, you won't need this in your sketch, the IDE takes care of it
#include "GB4DMOTGVerify.h"

// MOTG1
int SD_CARD_CHIP_SELECT = 3;
// MOTG2
GB4DMOTGLcdDriver motgLcd(7);

GB4DLcdDriver *lcd = &motgLcd;

int row = 0;
int column = 0;

GBIMAC mac(A2);
byte macAddress[MAC_LENGTH];;

File testFile;
String fileContents = "Testing";

void println(const char *string) {
  println(SGC_COLORS.WHITE, string);
}
void print(const char *string) {
  print(SGC_COLORS.WHITE, string);
}

void println(int color, const char *string) {
  lcd->drawString(column,row++,SGC_FONT_SIZE.SMALL,color,string);
  column = 0;
}

void print(int color, const char *string) {
  lcd->drawString(column,row,SGC_FONT_SIZE.SMALL,color,string);
  column += String(string).length();
}

void println(int color, const char *title, const char *string) {
  print(title);
  println(color, string);  
}


void welcome(const char *mode) {
  println("GorillaBuilderz");
  println("4D MOTG Verify");  
  print("Test Mode: ");
  println(SGC_COLORS.GREEN, mode);  
}

void testIMAC() {
  println("");
  println("GB IMAC: Reading");

  mac.read(macAddress);
  println(SGC_COLORS.GREEN, "", mac.format(macAddress));  
}

void testSD() {
  doTestSD();
  // Always clean up the file
  SD.remove("test.txt");    
}

void doTestSD() {
  pinMode(SD_CARD_CHIP_SELECT, OUTPUT);
  println("");
  print("SD Card: Initialising Pin: ");
  char chipSelectBuffer[3];
  String(SD_CARD_CHIP_SELECT, DEC).toCharArray(chipSelectBuffer, 3);
  println(chipSelectBuffer);
  
  if (!SD.begin(SD_CARD_CHIP_SELECT)) {
    println(SGC_COLORS.RED, "SD Card: ", "Initialisation Failed");
    return;
  }
  
  // Remove the test file first
  SD.remove("test.txt");
  testFile = SD.open("test.txt", FILE_WRITE);
  
  if(testFile) {
    println("SD Card: Writing to file");
    testFile.println(fileContents);
    // close the file:
    testFile.close();
    
    println(SGC_COLORS.GREEN, "SD Card: ", "Write success");
  }
  else {
    println(SGC_COLORS.RED, "SD Card: ", "Failed to open file for write");
  }
  
  testFile = SD.open("test.txt");
  
  if(testFile) {
    println("SD Card: Reading file");
    
    char readByte;
    int index = 0;
    
    while(index < fileContents.length() && testFile.available()) {
        readByte = testFile.read();
        if(fileContents[index++] != readByte) {
          println(SGC_COLORS.RED, "SD Card: ", "File Corrupt");
          return;
        }
    }
    
    // If there's more bytes available (greater than null terminators etc), something is not quite right
    if(testFile.available() > 2) {
      println(SGC_COLORS.RED, "SD Card: ", "Additional bytes avail in file");                
    }
    // close the file:
    testFile.close();
    
    println(SGC_COLORS.GREEN, "SD Card: ", "Read success");
  }
  else {
    println(SGC_COLORS.RED, "SD Card: ", "Failed to open file for read");
  }
  
}

void testSpiLcd() {
  println("");
  println("SPI LCD: Test Ready");
  println("SPI LCD: Flick the switch!! ");  
  println("SPI LCD: Will begin in 3 seconds... ");
  delay(3000);
}

void runTest(const char *mode) {
  lcd->initialise();
  
  welcome(mode);
  
  testIMAC();
//  testSD();
}

void setup() {
  delay(3000);
  Serial.begin(115200);
  Serial.println("\n\r\n\rGB MOTG Sanity");  
}

void loop() {
  runTest("MOTG");  
  
  println("");    
  println("Finished");  
  delay(3000);
  Status.idle();
  lcd->clearScreen();
//  delay(2000);
}
