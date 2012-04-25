#include <GBStatus.h>
#include <GB4DLcdDriver.h>
#include <SPI.h>
#include <Console.h>

GB4DSPILcdDriver lcd;
Console console(&lcd);

SerialTransport transport(115200, 5000);

void setup() {
  delay(3000);
  console.initialise();
  console.println("GorillaBuilderz Wifi Test");
  console.println("");
  
  transport.initialise();
  transport.write("AT+WPAPSK=Testing,Password\n");
}

String readLine() {
  String line = "";
  byte readByte = 0;
  
  while(readByte != '\n' && readByte != '\r') {
    readByte = transport.read();
    line += String(readByte);
  }
  
  return line;
}

void loop() {
  String line = readLine();
  char lineArray[line.length()+1];
  line.toCharArray(lineArray, line.length());
  
  console.println(lineArray);
  Status.idle();
}
