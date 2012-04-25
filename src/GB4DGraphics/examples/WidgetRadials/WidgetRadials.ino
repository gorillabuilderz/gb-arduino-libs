#include <GBStatus.h>
#include <Transport.h>
#include <GB4DLcdDriver.h>
#include <SPI.h>
#include <GB4DGraphics.h>
#include <GString.h>

#include <stdio.h>

// Initialise the SPI driver using the default chip select. 
// NOTE: Remember to have the UART/SPI switch on SPI when uploading this sketch and running it
//       using the SPI driver!
GB4DSPILcdDriver lcd;

// Uncomment the following line and comment above to use the serial driver
// NOTE: If using serial driver, remember to have the UART/SPI switch on SPI when uploading 
//       this sketch but switched to UART when running this sketch.
// GB4DSerialLcdDriver lcd;

Label altitudeValueLabel(lcd, 5, 20, 90, Label::CENTER, "0");
RadialGauge altitudeGauge(lcd, 50, 50, 45, 0, 1000, SGC_COLORS.GREEN);
RadialLine altitudeThousandsLine(lcd, 50, 50, 20, 0, 10, SGC_COLORS.GREEN);
RadialTicks altitudeTicks(lcd, 50, 50, 45, 0, 1000, 100, 1, SGC_COLORS.GREEN);

Label variometerValueLabel(lcd, 5, 120, 90, Label::CENTER, "0");
RadialGauge variometerGauge(lcd, 50, 150, 45, -10, 10, 180, 360, SGC_COLORS.BLUE);
RadialTicks variometerTicks(lcd, 50, 150, 45, -10, 10, 1, 1, 180, 360, SGC_COLORS.BLUE);

Label airspeedValueLabel(lcd, 5, 220, 90, Label::CENTER, "0");
RadialGauge airspeedGauge(lcd, 50, 250, 45, 0, 260, 0, 300, SGC_COLORS.RED);
RadialTicks airspeedTicks(lcd, 50, 250, 45, 0, 260, 20, 1, 0, 300, SGC_COLORS.RED);

void setup() {
  delay(3000);
  lcd.initialise();

  Label altitudeLabel(lcd, 100, 20, "Altitude");
  altitudeLabel.draw();
  Label vsiLabel(lcd, 100, 120, "VSI");
  vsiLabel.draw();
  Label airspeedLabel(lcd, 100, 220, "Airspeed");
  airspeedLabel.draw();

  altitudeGauge.setValue(0);
  altitudeGauge.draw();
  altitudeThousandsLine.draw();
  altitudeTicks.draw();
  altitudeValueLabel.draw();
  
  variometerValueLabel.draw();
  variometerGauge.draw();
  variometerTicks.draw();
  
  airspeedValueLabel.draw();
  airspeedGauge.draw();
  airspeedTicks.draw();
}
int count = 0;
void loop() {
  Status.idle();
  count++;
  delay(50);
  
  // Set the altimeter
  double altitude = 1750 + sin((double)count/(double)100)*1750;
  altitudeGauge.setValue(altitude);  
  altitudeThousandsLine.setValue(altitude/(double)1000);
  altitudeValueLabel.setText(String((int)altitude));
  
  // Set the VSI
  double verticalSpeed = sin((double)count/(double)100)*10;
  variometerValueLabel.setText(String((int)verticalSpeed));
  variometerGauge.setValue(verticalSpeed);
  
  // Set the airspeed
  double airspeed = 125 + sin((double)count/(double)100)*125;
  airspeedGauge.setValue(airspeed);
  airspeedValueLabel.setText(String((int)airspeed));
}
