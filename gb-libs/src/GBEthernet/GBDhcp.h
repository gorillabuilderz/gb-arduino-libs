// DHCP Library v0.3 - April 25, 2009
// Author: Jordan Terrell - blog.jordanterrell.com

#ifndef Dhcp_h
#define Dhcp_h

#include "GBEthernetUdp.h"

class DhcpClass {
private:
  EthernetUDP _dhcpUdpSocket;
  
  void presend_DHCP();
  void send_DHCP_MESSAGE(uint8_t, uint16_t);
  
  uint8_t parseDHCPResponse(unsigned long responseTimeout, uint32_t& transactionId);
public:
  IPAddress getLocalIp();
  IPAddress getSubnetMask();
  IPAddress getGatewayIp();
  IPAddress getDhcpServerIp();
  IPAddress getDnsServerIp();
  
  int beginWithDHCP(uint8_t *, unsigned long timeout = 60000, unsigned long responseTimeout = 4000);
};

#endif
