# GB Arduino Libs

[GorillaBuilderz][gb] Arduino libraries:  Libraries and support code for the range of GorillaBuilderz Arduino shields.

Enjoy!

[http://gorillabuilderz.com.au][gb]


## Installation Instructions
1. Installation is just like any other [Arduino library][libraries] install. Locate the directory where your sketches are stored (refferred to as the [sketchbook][environment]), copy all folders from the `gb-arduino-libs/src` folder to your sketchbook `libraries` folder so that the structure under the libraries folder is such:
```
libraries
    |- GB4DGraphics
    |- GB4DLcdDriver
    |- GBDiagnose
    |- GBEthernet
    |- GBIMAC
    |- GBStatus
    |- GBTransport
    |- GBUtils
```   
If you're not too sure where to find your sketchbook folder, you can view or change the location by the Arduino IDE Preferences dialog. Generally this can be one of three places depending on your Opertating System:
    * Linux: `$HOME/sketchbook`
    * Windows: `My Documents\Arduino`
    * Mac: `$HOME/Documents/Arduino`

2. Restart the Arduino IDE.
3. Open and play with the GB* Examples from the File->Examples menu.

## Upgrade Instructions
1. Remove all GorillaBuilderz libraries (GB*) folders from your [Arduino sketchbook][environment] folder.
2. Follow the Installation Instructions above

## Quick Start
Below are a few code snippets to get an idea of general usage.

**Note:** Code snippets below aren't designed to compile in a sketch, please see examples provided witht the libraries.

### GBIMAC

```cpp
#include <SPI.h>
#include <GBIMAC.h>

GBIMAC mac(A2);
byte macAddress[MAC_LENGTH];
mac.read(macAddress);
```

### WiFi (GBEthernet)

#### Client

```cpp
#include <SPI.h>
// Need to include GorillaBuilderz WizFi Ethernet libraries
#include <Transport.h>
#include <WizFi210.h>
#include <GBEthernet.h>
// To read the GBIMAC, so you don't have to provide a MAC address....
#include <GBIMAC.h>

IPAddress server(74,125,237,114); // Google
// Initialise the client
EthernetClient client;

// NOTE: If you need to redefine the IO to your wifi shield call this BEFORE you execute any Ethernet* methods
WizFi210::create(10, 2, 5, 6);

// Set the network name
Ethernet.ssid("BatPhone");
// Initialise secure network passphrase
Ethernet.passphrase("password");
// Returns 1 if successfully associated to network and IP Address obtained. Use a previously read mac address from GBIMAC
Ethernet.begin(mac)
// Returns true if connected
if(client.connect(server, 80)) {
  println("Connection Established");
}
// Make a HTTP Request
client.println("GET /search?q=arduino HTTP/1.0");
client.println();
// Check if the client is still connected
if (client.connected()) {
    Serial.println("Still connected");
}
// Check if there are available bytes for reading
if(client.available()) {
  char c = client.read();
  Serial.print(c);
}
```

#### Server
Work in progress

### 4D SGC Driver (GB4DLcdDriver)
```cpp
// Use SPI switch to upload the sketch and then switch to Serial
// mode to run the sketch and restart.
GB4DSerialLcdDriver lcd;
// To run in SPI mode, switch must be set to SPI
//GB4DSPILcdDriver lcd;

lcd.initialise();
lcd.version(true);

// Different ways to set the background color
delay(1000);
lcd.setBackgroundColor(SGC_COLORS.WHITE);
lcd.clearScreen();

delay(1000);
lcd.replaceBackgroundColor(SGC_COLORS.RED);
  
delay(1000);    
lcd.setBackgroundColor(SGC_COLORS.GREEN);
lcd.clearScreen();  
delay(1000);    
lcd.setBackgroundColor(SGC_COLORS.BLUE);
lcd.clearScreen();  
delay(1000);    
lcd.setBackgroundColor(SGC_COLORS.BLACK);
lcd.clearScreen();  
  
lcd.clearScreen();
lcd.drawString(0,0, SGC_FONT_SIZE.SMALL, SGC_COLORS.WHITE, "GorillaBuilderz!!!");
lcd.drawString(0,1, SGC_FONT_SIZE.SMALL, SGC_COLORS.WHITE, "4D LCD Shield 1");  

// Fonts, shapes and colors
lcd.drawString(1,3, SGC_FONT_SIZE.SMALL, SGC_COLORS.WHITE, "SMALL - WHITE");    
lcd.drawCircle(100, 100, 20, SGC_COLORS.WHITE);
lcd.drawPixel(150, 250, SGC_COLORS.WHITE);
  
lcd.drawString(2,4, SGC_FONT_SIZE.MEDIUM, SGC_COLORS.RED, "MEDIUM - RED");    
lcd.drawTriangle(50, 150, 200, 150, 100, 200, SGC_COLORS.RED);  
lcd.drawRectangle(150, 5, 200, 50, SGC_COLORS.RED);  

lcd.drawString(3,4, SGC_FONT_SIZE.LARGE, SGC_COLORS.GREEN, "LARGE - GREEN");  
lcd.drawLine(20, 200, 50, 250, SGC_COLORS.GREEN);

lcd.drawString(4,4, SGC_FONT_SIZE.LARGEST, SGC_COLORS.BLUE, "LARGETST - BLUE!");  

lcd.drawElipse(200, 250, 15, 30, SGC_COLORS.BLUE);    

lcd.drawString(0,38, SGC_FONT_SIZE.SMALL, SGC_COLORS.RED, "Bye Bye!");  

lcd.drawGraphicsString(0, 100, SGC_FONT_SIZE.SMALL, SGC_COLORS.RED, "Graphics String!!!");     
```

### 4D Systems Graphics (GB4DGraphics)
Built as a layer above GB4DLcdDriver.

```cpp
// Initialise the SPI driver using the default chip select. 
// NOTE: Remember to have the UART/SPI switch on SPI when uploading this sketch and running it
//       using the SPI driver!
GB4DSPILcdDriver lcd;

// Uncomment the following line and comment above to use the serial driver
// NOTE: If using serial driver, remember to have the UART/SPI switch on SPI when uploading 
//       this sketch but switched to UART when running this sketch.
// GB4DSerialLcdDriver lcd;

// *** Using the widgets ***
// 2 options for creating a label
// Label creation using defaults
Label label1(lcd, 5, 1, "Initial Text 1");
// Label creation specifying each option
Label label2(lcd, 25, 40, SGC_FONT_SIZE.MEDIUM, SGC_COLORS.RED, SGC_COLORS.BLACK, "Initial Text 2");

// Justification
Label leftLabel(lcd, 5, 220, 200, Label::LEFT, "Text: L");
Label rightLabel(lcd, 5, 235, 200, Label::RIGHT, "Text: R");
Label centerLabel(lcd, 5, 250, 200, Label::CENTER, "Text: C");

// Create a button specifying position using defaults
Button button(lcd, 10, 70, "BUTTON");

// Fixed width
Button fixed1(lcd, 1, 140, 150, "Short");
Button fixed2(lcd, 1, 180, 150, "Long Name");

// Create a line using defaults
Line line(lcd, 150, 50, 200, 200, SGC_COLORS.RED);

Circle circleFilled(lcd, 15, 300, 10, true, SGC_COLORS.BLUE);
Circle circleNonFilled(lcd, 50, 300, 10, true, SGC_COLORS.RED);

// Draw each item
label1.draw();
label2.draw();
leftLabel.draw();
rightLabel.draw();
centerLabel.draw();
button.draw();
fixed1.draw();
fixed2.draw();
line.draw();
circleFilled.draw();
circleNonFilled.draw();
// Turn the button 'on' to signal it being pressed
button.on();
delay(2000);
// Change the label text
label1.setText("I Have Changed");
// Clear the label text
label2.clear();
// Turn the button 'off' to signal release
button.off();
```

## Documentation

Further documentation can be found at https://github.com/gorillabuilderz/gb-arduino-libs/wiki.

## Issues

Report issues at https://github.com/gorillabuilderz/gb-arduino-libs/issues.

## Contributors

Find all contributors at https://github.com/gorillabuilderz/gb-arduino-libs/graphs/contributors.

## License

This software is licensed under the Apache 2 license, quoted below:

Copyright 2012 GorillaBuilderz (http://gorillabuilderz.com.au/).

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0.

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.


[gb]: http://gorillabuilderz.com.au "GorillaBuilderz"
[libraries]: http://arduino.cc/it/Reference/Libraries  "Arduino Libraries"
[environment]: http://arduino.cc/en/Guide/Environment "Arduino Environment"