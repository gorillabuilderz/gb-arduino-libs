#include <GBStatus.h>

// Required for automated GB build verification, you won't need this in your sketch, the IDE takes care of it
#include "StatusTest.h"

/*
 * Sample GB Status file. Below is the simple API:
 
  // Call this to check if the status is currently executing a 'state'
  Status.isState();
  // To flash the success led sequence: 5 flashes, 100ms between flashes
  Status.success();
  // To flash the success led sequence: 5 flashes, 50ms between flashes
  Status.command();
  // To flash the success led sequence: 10 flashes, 200ms between flashes
  Status.error();
  // To flash the success led sequence: Flashes the led idefinitely
  Status.failure();
  // Call this while idle so the status can handle the flash logic
  Status.idle(); 
*/

int state = 0;

void setup() {
  Serial.begin(9600);
  delay(3000);
  Serial.println("GorillaBuilderz Status Led");
}

void loop() {
  if(!Status.isState()) {
      delay(2000);
      
      switch(state++) {
        case 0:
          Serial.println("Success");        
          Status.success();
          break;
        case 1:
          Serial.println("Command");        
          Status.command();
          break;
        case 2:
          Serial.println("Error");        
          Status.error();
          break;
        case 3:
          Serial.println("Failure");        
          Status.failure();
          break;          
      }
  }
  else {
    Status.idle();
  }
}
