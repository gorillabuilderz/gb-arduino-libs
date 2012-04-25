#include <GBStatus.h>
#include <GB4DLcdDriver.h>
#include <SPI.h>


GB4DSPILcdDriver lcd;

int _slaveSelect = 6;

void clearFIFO() {
	digitalWrite(_slaveSelect, LOW);
	SPI.transfer((0x02 << 3) & 0x7F); // Write command
	SPI.transfer(0x07);
	digitalWrite(_slaveSelect, HIGH);
}

byte read(byte spiRegister) {
	digitalWrite(_slaveSelect, LOW);
	SPI.transfer(spiRegister | 0x80); // register address is sent
	byte data = SPI.transfer(0); // dummy data is sent for spi read
	digitalWrite(_slaveSelect, HIGH);
	return data; // receive the read data    
}

void version() {
    Serial.println("Version");
//        clearFIFO();
      digitalWrite(_slaveSelect, LOW);
    SPI.transfer(0x00 & 0x7F); 
    SPI.transfer(0x56);
    SPI.transfer(0x01);    
    digitalWrite(_slaveSelect, HIGH);
    
    waitForData();

    digitalWrite(_slaveSelect, LOW);
//    Serial.println(read(0));
//    Serial.println(read(0));
//    Serial.println(read(0));
//    Serial.println(read(0));
//    Serial.println(read(0));    
    SPI.transfer(0x80);    
    Serial.println(SPI.transfer(0x00), HEX);
    Serial.println(SPI.transfer(0x00), HEX);
    Serial.println(SPI.transfer(0x00), HEX);
    Serial.println(SPI.transfer(0x00), HEX);    
    Serial.println(SPI.transfer(0x00), HEX);    
    digitalWrite(_slaveSelect, HIGH);    
}

void waitForData() {
    // Wait for data available
    while(!available()) {
      delay(20);
//      Serial.println("No Data");
    }
}

boolean available() {
      return read(0x05 << 3) & 0x01;
}

void background() {
    Serial.println("Background");  
//    clearFIFO();
   digitalWrite(_slaveSelect, LOW);

    SPI.transfer(0x00 & 0x7F); 
    SPI.transfer(0x42);
    SPI.transfer(0xFF);    
    SPI.transfer(0xFF);
    digitalWrite(_slaveSelect, HIGH);    
    
    waitForData();
    
    digitalWrite(_slaveSelect, LOW);
    SPI.transfer(0x80);    
    Serial.println(SPI.transfer(0x00), HEX);

    digitalWrite(_slaveSelect, HIGH);    
}

void setup() {
  delay(3000);
    Serial.begin(9600);
    Serial.println("\n\r\n\rGB-LCD Routine");
    
  if(lcd.initialise()) {
    Serial.println("Bridge initialized successfully!"); 
    
    version();
    
//    delay(500);

    background();    
  }
  else {
    Serial.println("Could not initialize bridge, locking up.\n\r"); 
  }
}

void loop() {
  Status.idle();
}
