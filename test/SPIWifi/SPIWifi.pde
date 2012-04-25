#include <SPI.h>
#include <SC16SpiTransport.h>

String SSID = "Testing";
String PASSWORD = "password";

int RESET_PIN  = 6;

SC16SpiTransport transport(2, SC16IS740_BAUDRATE.B115200);

String MODEM_OK = "[OK]";
String MODEM_ERROR = "[ERROR]";
String MODEM_ERROR_INVALID_INPUT = "[ERROR: INVALID INPUT]";
String MODEM_ERROR_SOCKET_FAILURE = "[ERROR: SOCKET FAILURE]";
String MODEM_ERROR_NO_CID = "[ERROR: NO CID]";

void reset() {
  Serial.println("Modem reset");  
  pinMode(RESET_PIN, OUTPUT);
  digitalWrite(RESET_PIN, HIGH);    
  digitalWrite(RESET_PIN, LOW);  
  delay(200);
  digitalWrite(RESET_PIN, HIGH);    
  delay(7000);  
}

void setup() {
  delay(3000);
  Serial.begin(115200);
  Serial.println("GorillaBuilderz Wifi Test");
  Serial.println("");

  reset();  
  
  if(!transport.initialise()) {
    Serial.println("Transport init failed");
  }
  else {
    Serial.println("Transport init success");    
  }

  
  Serial.println("Initialising modem:");
  
  sendCommand("\n");  
  sendCommand("AT\n");
  receiveResponse();

  transport.enableHardwareFlowControl(true);  

  // Enalbe hardware flow control
  sendCommand("AT&R1\n");
  receiveResponse();
  
  sendCommand("AT+WS\n");
  receiveResponse();

  // Disconnect
//  sendCommand("AT+WD\n");
//  receiveResponse();

  // Enable DHCP
  sendCommand("AT+NDHCP=1\n");
  receiveResponse();
  
  // Setup Wireless Security Details
//  sendCommand("AT+WPAPSK=BatPhone,p@s$w0rd\n");
//  receiveResponse();
  
  // Connect
  sendCommand("AT+WA=BatPhone\n");  
  receiveResponse();  

  sendCommand("\n");
  sendCommand("\n");
  sendCommand("\n");  
  sendCommand("AT\n");
  receiveResponse();  
  
//  sendCommand("AT+NCTCP=192.168.1.6,8080\n");
  sendCommand("AT+DNSLOOKUP=google.com\n");
  receiveResponse();  

//  sendCommand("AT\n");  
//  receiveResponse();  
  
  sendCommand("\n");
  sendCommand("\n");
  sendCommand("\n");  
  sendCommand("AT\n");
  receiveResponse();  
  
  sendCommand("AT+WD\n");  
  receiveResponse();    
}

void sendCommand(const char *command) {
  transport.select();
  transport.prepareWrite();   
  transport.write(command);
  transport.deselect();  
}


String receiveResponse() {
  unsigned long timeout = millis() + 10000;
  
  String response = "";
  int available;

  transport.select();  
  while(timeout > millis()) {
    if(available = transport.available()) {
//      Serial.print("available: ");
//      Serial.println(available);
      transport.deselect();      
      transport.select();
      
      transport.prepareRead();
      for(int index = 0; index < available; index++) {
        char responseChar = transport.read();
        response += responseChar;
        Serial.print(responseChar);   
      }      
      transport.deselect();      
      
        if(response.indexOf(MODEM_OK) >= 0) {
          Serial.println();
          Serial.println("-- OK Received");
          break;
        }
        else if(response.indexOf(MODEM_ERROR) >= 0 ||
        response.indexOf(MODEM_ERROR_INVALID_INPUT) >= 0 ||
        response.indexOf(MODEM_ERROR_SOCKET_FAILURE) >= 0 ||
        response.indexOf(MODEM_ERROR_NO_CID) >= 0){
          Serial.println();          
          Serial.print("-- Received error: ");
          Serial.println(response);
          Serial.println("-- !!locking up!!");
          while(true);
        }
        
      transport.select();
    }
  }
  
  if(timeout <= millis()) {
    Serial.println();     
    Serial.println("-- Timed Out");
  }
  
  return response;
}

void loop() {

}
