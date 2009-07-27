// Shift-Register connected Liquid Crystal Display example with:
//   Data on pin 12
//   Clock on pin 13
//   Enable on pin 8
// LCD d4, d5, d6, d7 from corresponding outputs on the shiftregister
// RS on bit 3 from the shift register. Thus only 4-bit mode on LCD 
// despite 8 bit shiftregister (3 wasted bits).
// R/!W (or rw) hardwired to GND (write to LCD only, no reading from)

// USAGE: ShiftRegLCD LCDobjectvariablename(Datapin, Clockpin, Enablepin [, Lines [, Font]])
//   where Lines and/or Font are optional.
//   Lines: 1 or 2 lines (or more if possible)
//   Font : 0 or 1, small or big font (8 or 10 pixel tall font, if available).
 
#include <ShiftRegLCD.h>

ShiftRegLCD srlcd(2, 3, ENABLE_AS_DATA, 2); // no specification of Lines or Font neccesary (but optional)

void setup()
{
  // Print a message to the LCD.
  //srlcd.home();
  pinMode( 13, OUTPUT );
  Serial.begin(9600);
}

uint8_t toggle = 0;
void loop()
{
  toggle = toggle ^ 1;
  if( toggle == 0 )
  {
   digitalWrite( 13, HIGH );
   srlcd.clear();
   srlcd.setCursor(0,0);
   srlcd.print("SLON");
  }
  else
  {
   digitalWrite( 13, LOW );
   srlcd.clear();
   srlcd.setCursor(0,1);
   srlcd.print("KROKODIL");
  }
  delay( 2000 );
} 
