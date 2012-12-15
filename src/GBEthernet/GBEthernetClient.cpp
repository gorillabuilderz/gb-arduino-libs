#include "WizFi210.h"

extern "C" {
  #include "string.h"
}

#include <Arduino.h>

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
	// TODO Implement correctly, with DNS lookup
	// IPAddress ip(WizFi210Class::getInstance()->lookup());
	// return connect(ip, port);
	return 0;
}

int EthernetClient::connect(IPAddress ip, uint16_t port) {
	WizFi210::getInstance()->setAutoTcpConnect(Ethernet.getRawAddress(ip), port);

	if(!Ethernet.associated() && !Ethernet.associate()) {
		return false;
	}

	int result = WizFi210::getInstance()->autoConnectExistingAssociation();
	// Give the modem a bit to connect
	delay(500);
	return result;
}

size_t EthernetClient::write(uint8_t b) {
	return WizFi210::getInstance()->write(b);
}

size_t EthernetClient::write(const uint8_t *buf, size_t size) {
	return WizFi210::getInstance()->write(buf, size);
}

int EthernetClient::available() {
	return WizFi210::getInstance()->available();
}

int EthernetClient::read() {
	readConnectedTimeout = millis() + CONNECTED_TIMEOUT;
	return WizFi210::getInstance()->read();
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
    WizFi210::getInstance()->escapeDataMode();
    WizFi210::getInstance()->closeAllConnections();
    flush();
}

uint8_t EthernetClient::connected() {
	return WizFi210::getInstance()->connected();
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
