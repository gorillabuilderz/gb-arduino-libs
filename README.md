# GB Arduino Libs

[GorillaBuilderz](http://gorillabuilderz.com.au) Arduino libraries. Contains libraries and support code for the range of GorillaBuilderz Arduino shields.

Enjoy!

[http://gorillabuilderz.com.au](http://gorillabuilderz.com.au)


## Installation Instructions
1. To install, follow [this Arduino Reference Libraries guide](http://arduino.cc/it/Reference/Libraries), under the "Contributed Libraries" section, which is quoted below. In brief, copy all folders from `gb-arduino-libs/src` folder to your sketchbook/libraries folder so that the structure is such:

```
sketchbook
    |- libraries
    |    |- GB4DGraphics
    |    |- GB4DLcdDriver
    |    |- GBEthernet
    |    |- GBIMAC
    |    |- GBStatus
    |    |- GBTransport
    |    |- GBUtils
```   
Quoted:
> "If you're using one of these libraries, you need to install it first. To do so, 
	download the library and unzip it. It should be in a folder of its own, and 
	will typically contain at least two files, one with a .h suffix and one with 
	a .cpp suffix. Open your Arduino sketchbook folder. If there is already a folder 
	there called libraries, place the library folder in there. If not, create a 
	folder called libraries in the sketchbook folder, and drop the library folder 
	in there. Then re-start the Arduino programming environment, and you should see
	your new library in the Sketch > Import Library menu."
2. Restart the Arduino IDE.
3. Open and play with the GB* Examples from the File->Examples menu.

## Upgrade Instructions
1. Remove all GorillaBuilderz libraries (GB*) folders from your Arduino sktechbook folder.
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

![Architecture](http://gorillabuilderz.com.au/wiki/images/4/45/Gb-arduino-libs-achitecture.png) 

The best place to start to see general usage of the library is to open up the example sketches included. These can be a basis for your own sketch.

### GB4DLcdDriver

An driver for [4D Systems](http://gorillabuilderz.com.au/shop/tag/4d-systems/) SGC based LCD's. Ensure your LCD is loaded with the latest SGC pmmc which can be found of [4D Systems website](http://www.4dsystems.com.au/)

## Continuous Integration Build

Included in the distibution is an [Apache Ant](http://ant.apache.org/) build script. This script is used for 2 reasons. For a [continuous integration](http://en.wikipedia.org/wiki/Continuous_integration) build server, such as [Jenkins](http://jenkins-ci.org/), to automatically verify any code changes will not break compilation of the library and to assist build and packaging of the distribution. As this is a C++ project, I could've created a make file for it, but I'm more familiar with Ant than Make.

If you're interested in running the Ant build yourself, there are a couple of prerequisites for this:

* Apache Ant installation
* avr dude installation
