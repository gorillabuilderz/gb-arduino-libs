#include <Arduino.h>
#include <SPI.h>

#include <Transport.h>
#include <WizFi210.h>
#include <GBEthernet.h>
#include <GBIMAC.h>
#include <SD.h>

void doTestSD();

GBIMAC macReader(A1);
byte mac[MAC_LENGTH];

IPAddress server(74,125,237,114); // Google

int SD_CARD_CHIP_SELECT = 4;

EthernetClient client;

void setup ()
{
  Serial.begin ( 115200 );

  Serial.println ( "WiFi Shield Test" );

  delay ( 1000 );
  doTestSD ();

  macReader.read ( mac );

  Serial.println ( "MAC:" );
  Serial.print ( mac [ MAC_LENGTH - 4 ], HEX );
  Serial.print ( mac [ MAC_LENGTH - 3 ], HEX );
  Serial.print ( mac [ MAC_LENGTH - 2 ], HEX );
  Serial.println ( mac [ MAC_LENGTH - 1 ], HEX );
  
  Serial.println ( "Init modem" );
  
  // NOTE: If you need to redefine the IO to your wifi shield call this BEFORE you execute any Ethernet* methods
  WizFi210::create ( 10, 2, 5, 6 );
  
  Ethernet.ssid( "BatPhone" );
  Ethernet.passphrase( "password" );
}

void doTestSD ()
{
  File testFile;
  String fileContents = "Testing";

  Serial.println ( "SD Card" );

  pinMode ( SD_CARD_CHIP_SELECT, OUTPUT );
  if ( !SD.begin ( SD_CARD_CHIP_SELECT ) )
  {
    Serial.println ( "SD: Failed" );
    return;
  }

  SD.remove ( "test.txt" );
  testFile = SD.open ( "test.txt", FILE_WRITE );
  
  if ( testFile )
  {
    Serial.println ( "SD: Write" );
    testFile.println ( fileContents );
    testFile.close ();
 
    Serial.println ( "SD: Pass" );
  }
  else
  {
    Serial.println ( "SD: Fail" );
  }
  
  testFile = SD.open ( "test.txt" );

  if ( testFile )
  {
    Serial.println ( "SD: Read" );
 
    char readByte;
    int index = 0;
 
    while ( index < fileContents.length () && testFile.available () )
    {
        readByte = testFile.read ();
        if ( fileContents [ index++ ] != readByte )
        {
          Serial.println ( "SD: Fail" );
          return;
        }
    }
    
    if ( testFile.available () > 2 )
    {
      Serial.println ( "SD: Fail" );
    }

    testFile.close ();

    Serial.println ( "SD: Pass" );
  }
  else
  {
    Serial.println ( "SD: Fail" );
  }
}

int counter;

void loop ()
{
  if ( Ethernet.begin ( mac ) == 0 )
  {
    Serial.println( "X DHCP" );
  }
  else
  {
    delay ( 1000 );
    Serial.println ( "connect" );
  }

  if ( client.connect ( server, 80 ) )
  {
    Serial.println ( "connected" );
    client.println ( "GET /search?q=arduino HTTP/1.0" );
    client.println ();
  } 
  else
  {
    Serial.println ( "failed" );
  }

  counter = 500;
  while ( counter-- )
  {
    if ( client.available () )
    {
      char c = client.read ();
      Serial.print ( c );
    }
    delay ( 5 );
  }

  if ( !client.connected () )
  {
    Serial.println ();
    Serial.println ( "disconnecting." );
    client.stop ();
  }
}

