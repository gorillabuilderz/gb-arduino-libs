#ifndef ethernet_h
#define ethernet_h

#include <inttypes.h>
#include <IPAddress.h>
#include "GBEthernetClient.h"
#include "GBEthernetServer.h"
#include "WizFi210.h"

#define MAX_SOCK_NUM 4

class EthernetClass {
private:
  IPAddress _dnsServerAddress;
  char *_ssid;
  char *_passphrase;
  WizFi210* _wizFi;

public:
  static uint8_t _state[MAX_SOCK_NUM];
  static uint16_t _server_port[MAX_SOCK_NUM];
  // Initialise the Ethernet shield to use the provided MAC address and gain the rest of the
  // configuration through DHCP.
  // Returns 0 if the DHCP configuration failed, and 1 if it succeeded
  int begin(uint8_t *mac_address);
  void begin(uint8_t *mac_address, IPAddress local_ip);
  void begin(uint8_t *mac_address, IPAddress local_ip, IPAddress dns_server);
  void begin(uint8_t *mac_address, IPAddress local_ip, IPAddress dns_server, IPAddress gateway);
  void begin(uint8_t *mac_address, IPAddress local_ip, IPAddress dns_server, IPAddress gateway, IPAddress subnet);

  IPAddress localIP();
  IPAddress subnetMask();
  IPAddress gatewayIP();
  IPAddress dnsServerIP();

  // GB Wifi Functions
  void passphrase(char *passphrase);
  void ssid(char *ssid);

  // Created because it appears IPAddress is friending Client, not EthernetClient
  uint8_t* getRawAddress(IPAddress ipAddress);

  // Allows a user to set their own instance of the WizFi class with their own IO pin selection
  // Use syntax:
  // Ethernet.setWizFi(WizFi210::create(resetPin, chipSelectPin, associatePin, wifiOkPin))
  void setWizFi(WizFi210* wizFi);

  friend class EthernetClient;
  friend class EthernetServer;
};

extern EthernetClass Ethernet;

#endif
