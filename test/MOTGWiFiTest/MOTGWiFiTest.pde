#include <GBStatus.h>
#include <SPI.h>
#include <SerialTransport.h>


SerialTransport transport(115200, 5000);
/*  
  Status.isState();
  Status.success();
  Status.command();
  Status.error();
  Status.failure();
  Status.idle();
*/  

void readByte() {
  if(transport.read() == 'O');
  transport.read();  
}

void setup() {
  transport.initialise();
  delay(5000);
  Status.command();
  while(Status.isState()) {
    Status.idle();
  }
  
  transport.write('AT\n');
  if(transport.read() == 'O') {
    Status.success();
  }
  else {
    Status.failure();    
  }
}

void loop() {
  Status.idle();
}
