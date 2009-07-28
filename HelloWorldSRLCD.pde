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

ShiftRegLCD srlcd(11, 12, TWO_WIRE, 2); // no specification of Lines or Font neccesary (but optional)

void setup()
{
  // Print a message to the LCD.
  //srlcd.home();
  pinMode( 13, OUTPUT );
  Serial.begin(9600);
}

uint8_t toggle = 0;
uint8_t timetotoggle = 0;
uint32_t counter = 0;
uint32_t seconds = 0;
void loop()
{
  if( counter == 0 )
  {
    toggle = toggle ^ 1;
    seconds++;
    
    timetotoggle = 1;
  }

  if( timetotoggle )
  {  
    if( toggle == 0 )
    {
     digitalWrite( 13, HIGH );
     srlcd.setCursor(0,0);
     srlcd.print("SLON    ");
    }
    else
    {
     digitalWrite( 13, LOW );
     srlcd.setCursor(0,0);
     srlcd.print("KROKODIL");
    }

    srlcd.setCursor( 0, 1 );
    srlcd.print( seconds );
    
    timetotoggle = 0;
  }

  counter++;
  if( counter > 1000000 )
  {
    counter = counter % 2000000;
  }
  delayMicroseconds( 1 );
} 
