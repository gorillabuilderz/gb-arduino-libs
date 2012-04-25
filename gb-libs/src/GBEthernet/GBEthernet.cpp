#include "GBEthernet.h"
#include "WizFi210.h"

#include "GBDhcp.h"

// XXX: don't make assumptions about the value of MAX_SOCK_NUM.
uint8_t EthernetClass::_state[MAX_SOCK_NUM] = {
  0, 0, 0, 0 };
uint16_t EthernetClass::_server_port[MAX_SOCK_NUM] = {
  0, 0, 0, 0 };

int EthernetClass::begin(uint8_t *mac_address)
{
	// If the ssid is not set, return 0
	if(_ssid == NULL) {
		return 0;
	}

	// Initialise the modem
	if(!WizFi210.initialise()) {
		return 0;
	}

	// If the password is set, setup the passphrase
	if(_passphrase != NULL) {
		WizFi210.setWPAPSK(_ssid, _passphrase);
	}

	// Set auto associate with the desired network
	WizFi210.setAutoAssociate(_ssid);

	WizFi210.setMac(mac_address);
	WizFi210.enableDHCP(true);

	// Associate to the network here...
	return WizFi210.associate(_ssid);
}

void EthernetClass::begin(uint8_t *mac_address, IPAddress local_ip)
{
  // Assume the DNS server will be the machine on the same network as the local IP
  // but with last octet being '1'
  IPAddress dns_server = local_ip;
  dns_server[3] = 1;
  begin(mac_address, local_ip.raw_address(), dns_server.raw_address());
}

void EthernetClass::begin(uint8_t *mac_address, IPAddress local_ip, IPAddress dns_server)
{
  // Assume the gateway will be the machine on the same network as the local IP
  // but with last octet being '1'
  IPAddress gateway = local_ip;
  gateway[3] = 1;
  begin(mac_address, local_ip, dns_server, gateway);
}

void EthernetClass::begin(uint8_t *mac_address, IPAddress local_ip, IPAddress dns_server, IPAddress gateway)
{
  IPAddress subnet(255, 255, 255, 0);
  begin(mac_address, local_ip, dns_server, gateway, subnet);
}

void EthernetClass::begin(uint8_t *mac, IPAddress local_ip, IPAddress dns_server, IPAddress gateway, IPAddress subnet)
{
	_dnsServerAddress = dns_server;

	WizFi210.initialise();

	WizFi210.setMac(mac);
	WizFi210.setNetworkParameters(local_ip.raw_address(), subnet.raw_address(), gateway.raw_address());
}

IPAddress EthernetClass::localIP()
{
  IPAddress ret;
//  W5100.getIPAddress(ret.raw_address());
  return ret;
}

IPAddress EthernetClass::subnetMask()
{
  IPAddress ret;
//  W5100.getSubnetMask(ret.raw_address());
  return ret;
}

IPAddress EthernetClass::gatewayIP()
{
  IPAddress ret;
//  W5100.getGatewayIp(ret.raw_address());
  return ret;
}

IPAddress EthernetClass::dnsServerIP()
{
  return _dnsServerAddress;
}

EthernetClass Ethernet;

/************** GB Extensions *******************/
void EthernetClass::passphrase(char *passphrase) {
	_passphrase = passphrase;
}

void EthernetClass::ssid(char *ssid) {
	_ssid = ssid;
}

uint8_t* EthernetClass::getRawAddress(IPAddress ipAddress) {
	return ipAddress.raw_address();
}
