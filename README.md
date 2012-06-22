# GB Arduino Libs

[GorillaBuilderz](http://gorillabuilderz.com.au) Arduino libraries. Contains libraries and support code for the range of GorillaBuilderz Arduino shields.

Enjoy!

[http://gorillabuilderz.com.au](http://gorillabuilderz.com.au)


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

## Modules Included

The modules included in this library:

* *GBUtils*: General utilities used by other modules
* *GBStatus*: A basic LED status flasher
* *GBIMAC*: Reads the IMAC address built into every GorillaBuilderz shield
* *GBTransport*: Basic transport operations to communicate with different devices on a GorillaBuilderz shield. Transports include SPI, Serial and SC16 UART bridge.
* *GB4DLcdDriver*: 4D Systems SGC Driver 
* *GB4DGraphics*: Basic graphics widgets built on the GB4DLcdDriver 
* *GBEthernet*: Work in progress support for our WiFi shield
* *GBDiagnose*: A suit of sketches to assist troubleshooting

![Architecture](wifi-shield/docs/Achitecture.png?raw=true) 

The best place to start to see general usage of the library is to open up the example sketches included. These can be a basis for your own sketch.

### GB4DLcdDriver

An driver for [4D Systems](http://gorillabuilderz.com.au/shop/tag/4d-systems/) SGC based LCD's. Ensure your LCD is loaded with the latest SGC pmmc which can be found of [4D Systems website](http://www.4dsystems.com.au/)

## Continuous Integration Build

Included in the distibution is an [Apache Ant](http://ant.apache.org/) build script. This script is used for 2 reasons. For a [continuous integration](http://en.wikipedia.org/wiki/Continuous_integration) build server, such as [Jenkins](http://jenkins-ci.org/), to automatically verify any code changes will not break compilation of the library and to assist build and packaging of the distribution. As this is a C++ project, I could've created a make file for it, but I'm more familiar with Ant than Make.

If you're interested in running the Ant build yourself, there are a couple of prerequisites for this:

* Apache Ant installation
* avr dude installation

[libraries]: http://arduino.cc/it/Reference/Libraries  "Arduino Libraries"
[environment]: http://arduino.cc/en/Guide/Environment "Arduino Environment"
