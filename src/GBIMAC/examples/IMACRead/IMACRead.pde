#include <SPI.h>
#include <GBIMAC.h>

/*
  If you look under the board there will be solder link options labeled nI. 
  Look for the one with a solder blob across it and read off the 
  corresponding data line for its CS.
*/
GBIMAC mac(A2);

byte macAddress[MAC_LENGTH];

void setup() {
  delay(5000);
  Serial.begin(9600);
  
  // Sample usage of the API
  
  // Read back and display the current chip select value
  int chipSelect = mac.getChipSelect();
  // Read the mac address
  mac.read(macAddress);
  
  Serial.println("\n\r\n\rGorillaBuilderz IMACReader");  
  Serial.print("Chip Select: ");  
  Serial.println(chipSelect, DEC);
  Serial.print("This ID is: ");
  Serial.println(mac.format(macAddress));
}

void loop() {
}
