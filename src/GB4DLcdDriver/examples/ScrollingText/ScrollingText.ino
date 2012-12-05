/***************************************************************************************************
 * This example will scroll multiple blocks of text accross a 4D Systems LCD attatched to a
 * GorillaBuilderz LCD Shield. The example supports scroll on and scroll off the screen.
 *
 *
 * This example code is provided by GorillaBuilderz and is in the public domain.
 *
 ***************************************************************************************************
 * www.GorillaBuilderz.com
 ***************************************************************************************************/

#include <GBStatus.h>
#include <Transport.h>
#include <GB4DLcdDriver.h>
#include <SPI.h>
 
// Use SPI switch to upload the sketch and then switch to Serial
// mode to restart and run the sketch. 
GB4DSerialLcdDriver lcd;
//  GB4DSPILcdDriver lcd;

#define SCROLL_DELAY_MS                100
 
#define DISPLAY_CHARACTERS_PER_LINE    20
 
#define DISPLAY_BLOCK_COUNT            4
#define DISPLAY_LINE1_LENGTH           3
#define DISPLAY_LINE2_LENGTH           5
#define DISPLAY_LONGEST_LINE_LENGTH    ( DISPLAY_LINE1_LENGTH > DISPLAY_LINE2_LENGTH ? \
                                         DISPLAY_LINE1_LENGTH : DISPLAY_LINE2_LENGTH )
 
typedef struct
{
  char line1 [ DISPLAY_LINE1_LENGTH ];
  char line2 [ DISPLAY_LINE2_LENGTH ];
} display_block;
 
display_block display_data [ DISPLAY_BLOCK_COUNT ];
 
short current_block = DISPLAY_BLOCK_COUNT;
 
void setup ( void )
{
  delay ( 3000 );
  lcd.initialise ();
  lcd.version ( true );
  lcd.setBackgroundColor ( SGC_COLORS.DRK_GREEN );
}
 
void loop ( void )
{
  signed short x_pos;
  short counter;
 
  if ( ++current_block >= DISPLAY_BLOCK_COUNT )
  {
    current_block = 0;

    // Place code here to update text blocks from an external source.
    strcpy ( display_data [ 0 ].line1, "ABC" );
    strcpy ( display_data [ 0 ].line2, "1.123" );
    strcpy ( display_data [ 1 ].line1, "CRL" );
    strcpy ( display_data [ 1 ].line2, "99999" );
    strcpy ( display_data [ 2 ].line1, "XYZ" );
    strcpy ( display_data [ 2 ].line2, "2.234" );
    strcpy ( display_data [ 3 ].line1, "xyz" );
    strcpy ( display_data [ 3 ].line2, "3.345" );
  }
 
  x_pos = DISPLAY_CHARACTERS_PER_LINE;
  while ( x_pos-- > -DISPLAY_LONGEST_LINE_LENGTH )
  {
    lcd.clearScreen ();
 
    counter = DISPLAY_LINE1_LENGTH;
    while ( counter-- )
    {
      if ( ( x_pos + counter ) < DISPLAY_CHARACTERS_PER_LINE && ( x_pos + counter ) >= 0 )
      {
        lcd.drawString ( ( x_pos + counter ), 16, SGC_FONT_SIZE.LARGEST, SGC_COLORS.WHITE,
                         display_data [ current_block ].line1 [ counter ] );
      }
    }
 
    counter = DISPLAY_LINE2_LENGTH;
    while ( counter-- )
    {
      if ( ( x_pos + counter ) < DISPLAY_CHARACTERS_PER_LINE && ( x_pos + counter ) >= 0 )
      {
        lcd.drawString ( ( x_pos + counter ), 18, SGC_FONT_SIZE.LARGEST, SGC_COLORS.WHITE,
                         display_data [ current_block ].line2 [ counter ] );
      }
    }
    delay ( SCROLL_DELAY_MS );
  }
}