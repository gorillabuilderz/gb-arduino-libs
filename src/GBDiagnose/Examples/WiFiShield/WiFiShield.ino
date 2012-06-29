#include <GBStatus.h>
#include <Transport.h>
#include <SC16SpiTransport.h>
#include <WizFi210.h>
#include <SPI.h>
#include <GString.h>

// Required for automated GB build verification, you won't need this in your sketch, the IDE takes care of it
#include "WiFiShield.h"

// Update this to change from default
static const int CHIP_SELECT_PIN  = 2;
static const int RESET_PIN  = A1;
char *SSID = "BatPhone";
char *PASSWORD = "password";
byte MAC[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte SERVER[] = {  74, 125, 237, 72};
int PORT = 80;

// This is usage to specify the chip select
SC16SpiTransport transport(CHIP_SELECT_PIN, SC16IS740_BAUDRATE.B115200);

// Initialise the SPI driver using the default chip select. 
// NOTE: Remember to have the UART/SPI switch on SPI when uploading this sketch and running it
//       using the SPI driver!
//WizFi210 wizFi;
// Redefine your IO here
// resetPin, chipSelectPin, associatePin, wifiOkPin
WizFi210 wizFi(A1, 2, 5, 6);

void setup() {
  delay(3000);

  Serial.begin(115200);
  Serial.println("\n\r\n\rGorillaBuilderz WiFi Shield Diagnose Tool");
  Serial.println();
  Serial.print("Chip Select for SPI operations: ");
  Serial.println(CHIP_SELECT_PIN);
  Serial.println();
  Serial.print("Initialising transport to SPI/UART Bridge using chip select: ");
  Serial.println(transport.getChipSelect());
  Serial.println("...");
  if(transport.initialise()) {
    Serial.println("Success!");
  }
  else {
    Serial.println("FAIL");
    Serial.println("Halting tests");
    return;
  }

  Serial.println();
  Serial.print("Initialising Module using reset: ");
  Serial.println(wizFi.getResetPin());
  Serial.println("...");
  
  if(wizFi.initialise()) {
    Serial.println("Success!");
  }
  else {
    Serial.println("FAIL");
    Serial.println("Halting tests");
    return;
  }
  
  // WARNING: Mac address must be set in order to associate  
  wizFi.setMac(MAC);
  wizFi.enableDHCP(true);
  wizFi.setWPAPSK(SSID, PASSWORD);

//  if(!wizFi.associate(SSID)) {
//    Serial.println("FAILED WiFi Associated");    
//    return;
//  }
//  
//  Serial.println("WiFi Associated");
  wizFi.setAutoAssociate(SSID);
  printStatus();
  wizFi.setAutoTcpConnect(SERVER, PORT);
  wizFi.autoAssociateAndConnect();
  wizFi.println("GET /search?q=arduino HTTP/1.0");
  wizFi.println();  
  printStatus();  
  
}

void printStatus() {
  Serial.print("Associated: ");
  Serial.print( wizFi.associated());
  
  Serial.print(", Connected: ");
  Serial.println( wizFi.connected());    
}

void loop() {
  Status.idle();
  
  if(wizFi.available()) {
    char c = wizFi.read();
    Serial.print(c);  
  }
  else {
    delay(1000);    
    printStatus();    
  }
}
