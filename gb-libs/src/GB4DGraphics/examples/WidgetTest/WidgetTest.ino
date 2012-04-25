#include <GBStatus.h>
#include <Transport.h>
#include <GB4DLcdDriver.h>
#include <SPI.h>
#include <GB4DGraphics.h>
#include <GString.h>

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

void setup() {
  delay(3000);
  lcd.initialise();

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
  delay(2000);
}

void loop() {
  label2.setText("Button ON");
  button.on();
  line.draw();
  delay(2000);
  Status.idle();
  label2.setText("Button OFF");
  button.off();
  line.clear();
  delay(2000);
}

