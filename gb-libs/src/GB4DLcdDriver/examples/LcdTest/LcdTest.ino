#include <GBStatus.h>
#include <Transport.h>
#include <GB4DLcdDriver.h>
#include <SPI.h>

GB4DSerialLcdDriver lcd;
//GB4DSPILcdDriver lcd;

void setup() {
  delay(3000);
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

  // Scroll and disappear
//  for(int index = 0; index < lcd.getHorizontalResolution(); index+=2) {
//    lcd.screenCopyPaste(0,2,0,0,
//          lcd.getHorizontalResolution(), 
//          lcd.getVerticalResolution());
//  }
}

void loop() {
  Status.idle();
  setup();
}
