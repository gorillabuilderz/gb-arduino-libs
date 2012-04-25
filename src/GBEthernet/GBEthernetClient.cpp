#include "WizFi210.h"

extern "C" {
  #include "string.h"
}

#include "Arduino.h"

#include "GBEthernet.h"
#include "GBEthernetClient.h"
#include "GBEthernetServer.h"
#include "GBDns.h"

uint16_t EthernetClient::_srcport = 1024;

EthernetClient::EthernetClient() : _sock(MAX_SOCK_NUM) {
}

EthernetClient::EthernetClient(uint8_t sock) : _sock(sock) {
}

int EthernetClient::connect(const char* host, uint16_t port) {
	// TODO Implement correctly
//	IPAddress ip
	// WizFi210.lookup();
//	connect(ip, port);
}

int EthernetClient::connect(IPAddress ip, uint16_t port) {
	WizFi210.setAutoTcpConnect(Ethernet.getRawAddress(ip), port);
	bool result = WizFi210.autoAssociateAndConnect();
	readConnectedTimeout = millis() + INITIAL_TIMEOUT;
	return result;
}

size_t EthernetClient::write(uint8_t b) {
	return WizFi210.write(b);
}

size_t EthernetClient::write(const uint8_t *buf, size_t size) {
	return WizFi210.write(buf, size);
}

int EthernetClient::available() {
	return WizFi210.available();
}

int EthernetClient::read() {
	readConnectedTimeout = millis() + CONNECTED_TIMEOUT;
	return WizFi210.read();
}

int EthernetClient::read(uint8_t *buf, size_t size) {
	size_t count = 0;
	while (count < size) {
		int c = read();
		if (c < 0) break;
	    *buf++ = (uint8_t)c;
	    count++;
	}
	return count;
}

int EthernetClient::peek() {
  return -1;
}

void EthernetClient::flush() {
  while (available())
    read();
}

void EthernetClient::stop() {
	// TODO Don't close all connections, only the one this is associated to
    WizFi210.escapeDataMode();
    WizFi210.closeAllConnections();
    WizFi210.disassociate();
}

uint8_t EthernetClient::connected() {
	// Check if wifi is connected
	// If library thinks it's connected AND we haven't timedout
	// OR there are bytes available
	return (WizFi210.connected() && readConnectedTimeout > millis()) || available();
}

uint8_t EthernetClient::status() {
	// TODO Implement
	return -1;
}

// the next function allows us to use the client returned by
// EthernetServer::available() as the condition in an if-statement.

EthernetClient::operator bool() {
  return _sock != MAX_SOCK_NUM;
}
