// WizFi is connected via SPI
#include <SPI.h>
// WizFi210 relies on GorillaBuilderz Transport drivers
#include <Transport.h>
#include <WizFi210.h>

char *SSID = "BatPhone";
char *PASSWORD = "password";
//byte SERVER[] = { 174,143,152,58 }; // Google
//byte SERVER[] = { 172,18,3,98 }; // Google
byte SERVER[] = { 192,168,1,5 }; // Google
int PORT = 10000;

void setup() {
  delay(3000);
  Serial.begin(115200);
  Serial.println("GorillaBuilderz Wifi Test");
  Serial.println("");

  if(!WizFi210.initialise()) {
    Serial.println("WizFi init failed");
    while(true);
  }
  else {
    Serial.println("WizFi init success");    
  }
  WizFi210.enableDHCP(true);
  WizFi210.setWPAPSK(SSID, PASSWORD);  
//  WizFi210.associate(SSID);
  WizFi210.setAutoAssociate(SSID);
  // Other computer is listening with netcat, command:
  // $ nc -l 10000
  WizFi210.setAutoTcpConnect(SERVER, PORT);
  
  if(WizFi210.autoAssociateAndConnect()) {
    WizFi210.write("Testing!!!");
    
    char response = -1;
    
    while(WizFi210.connected()) {
      if(WizFi210.available()) {
        response = WizFi210.read();
        Serial.print(response);

        if(response == 'e') {
          Serial.println();
          Serial.println("Exiting!");
          // Escape data mode to send some commands to the modem
          WizFi210.escapeDataMode();
          WizFi210.closeAllConnections();  
          WizFi210.disassociate();  
        }
      }
    }
    
    Serial.println("Done");
  }
  else {
    Serial.println("!!!Connect Failed!!!");
  }
}

void loop() {

}
