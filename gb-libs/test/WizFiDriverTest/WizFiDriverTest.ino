#include <SPI.h>

// Need to include GorillaBuilderz WizFi Ethernet libraries
#include <Transport.h>
#include <WizFi210.h>
#include <GBEthernet.h>

IPAddress server(173,194,33,104); // Google

void setup() {
  // start the serial library:
  Serial.begin(9600);
  WizFi210.setAutoTcpConnect(Ethernet.getRawAddress(server), 80);
}

void loop() {
}
