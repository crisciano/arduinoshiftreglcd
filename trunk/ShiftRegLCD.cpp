// ShiftRegLCD  - Shiftregister-based LCD library for Arduino
//
// Connects an LCD using 3 pins from the Arduino, via an 8-bit shiftregister.
//
// Based very much on the "official" ShiftRegLCD library for the Arduino (http://arduino.cc/en/Reference/Libraries)
//   and also the improved version (with example CustomChar1) from LadyAda:
//   (http://web.alfredstate.edu/weimandn/arduino/ShiftRegLCD_library/ShiftRegLCD_index.html)
//
// Modified to work serially with the shiftOut() function, an 8-bit shiftregister (SR) and an LCD in 4-bit mode.
//
// Bits #7 - #4 from SR connects to LCD d7 - d4.
// Bit  #3      from SR connects to RS (register select). There are 3 unused (wasted) bits in the SR.
// 3 Pins required from the Arduino for Data, Clock, and Enable.
// Only "Enable" connects directly to the LCD, data and clock to SR.
// LCD RW-pin hardwired to LOW (only writing to LCD). Busy Flag (BF / data bit D7) is not used.
//
// Any shift register should do. I used 74LS164, for the reason that's what I had at hand.
//       Project homepage: http://code.google.com/p/arduinoshiftreglcd/
//
//
// History
// 2009.05.23  raron, but; based mostly (as in almost verbatim) on the "official" ShiftRegLCD library.
// 2009.07.23  raron (yes exactly 2 months later). Incorporated some proper initialization routines
//               inspired (lets say copy-paste-tweaked) from ShiftRegLCD library improvements from LadyAda:
//                 http://web.alfredstate.edu/weimandn/arduino/ShiftRegLCD_library/ShiftRegLCD_index.html
//               Also a little re-read of the datasheet for the HD44780 LCD controller.

#include "ShiftRegLCD.h"
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "WProgram.h"

#define fastWrite(_pin_, _state_) ( _pin_ < 8 ? (_state_ ?  PORTD |= 1 << _pin_ : PORTD &= ~(1 << _pin_ )) : (_state_ ?  PORTB |= 1 << (_pin_ -8) : PORTB &= ~(1 << (_pin_ -8)  )))

void shiftOutZero( uint8_t dataPin, uint8_t clockPin, uint8_t bits=8 )
{
	fastWrite( dataPin, LOW );
	for( uint8_t cnt = 0; cnt < 8; cnt ++ )
	{
		fastWrite( clockPin, HIGH );
		fastWrite( clockPin, LOW );
	}
}

// assume 1 line 8 pixel high font
ShiftRegLCD::ShiftRegLCD(uint8_t srdata, uint8_t srclock, uint8_t enable) {
	init(srdata, srclock, enable, 1, 0);
}
// Set nr. of lines, assume 8 pixel high font
ShiftRegLCD::ShiftRegLCD(uint8_t srdata, uint8_t srclock, uint8_t enable, uint8_t lines) {
	init(srdata, srclock, enable, lines, 0);
}
// Set nr. of lines and font
ShiftRegLCD::ShiftRegLCD(uint8_t srdata, uint8_t srclock, uint8_t enable, uint8_t lines, uint8_t font) {
	init(srdata, srclock, enable, lines, font);
}


void ShiftRegLCD::init(uint8_t srdata, uint8_t srclock, uint8_t enable, uint8_t lines, uint8_t font)
{
  _srdata_pin = srdata; _srclock_pin = srclock; _enable_pin = enable == 100 ? srdata : enable;
  pinMode(_srclock_pin, OUTPUT);
  pinMode(_srdata_pin, OUTPUT);
  pinMode(_enable_pin, OUTPUT);

  digitalWrite(_enable_pin, LOW);
  digitalWrite(_srdata_pin, LOW);
  digitalWrite(_srclock_pin, LOW);

  shiftOutZero ( _srdata_pin, _srclock_pin );

  if (lines>1)
  	_numlines = LCD_2LINE;
  else
  	_numlines = LCD_1LINE;

  _displayfunction = LCD_4BITMODE | _numlines;

  // for some displays you can select a 10 pixel high font
  if (font != 0) {
  _displayfunction |= LCD_5x10DOTS; }
  else {
    _displayfunction |= LCD_5x8DOTS;
  }

  // AT THIS TIME THIS IS FOR 4-BIT MODE ONLY
  // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
  // according to datasheet, we need at least 40ms after power rises above 2.7V
  // before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
  delayMicroseconds(50000);
  send4bits(LCD_FUNCTIONSET | LCD_8BITMODE);
  delayMicroseconds(4500);  // wait more than 4.1ms
  // second try
  send4bits(LCD_FUNCTIONSET | LCD_8BITMODE);
  delayMicroseconds(150);
  // third go
  send4bits(LCD_FUNCTIONSET | LCD_8BITMODE);

  // finally, set to 4-bit interface
  send4bits(LCD_FUNCTIONSET | LCD_4BITMODE);

  // finally, set # lines, font size, etc.
  command(LCD_FUNCTIONSET | _displayfunction);
  // turn the display on with no cursor or blinking default
  _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
  display();
  // clear it off
  clear();
  // Initialize to default text direction (for romance languages)
  _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
  // set the entry mode
  command(LCD_ENTRYMODESET | _displaymode);

  home();

}


/********** high level commands, for the user! */
void ShiftRegLCD::clear()
{
  command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
  delayMicroseconds(2000);    // this command takes a long time!
}

void ShiftRegLCD::home()
{
  command(LCD_RETURNHOME);  // set cursor position to zero
  delayMicroseconds(2000);  // this command takes a long time!
}

void ShiftRegLCD::setCursor(uint8_t col, uint8_t row)
{
  int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
  if ( row > _numlines ) {
    row = _numlines-1;    // we count rows starting w/0
  }

  command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

// Turn the display on/off (quickly)
void ShiftRegLCD::noDisplay() {
  _displaycontrol &= ~LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void ShiftRegLCD::display() {
  _displaycontrol |= LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void ShiftRegLCD::noCursor() {
  _displaycontrol &= ~LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void ShiftRegLCD::cursor() {
  _displaycontrol |= LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void ShiftRegLCD::noBlink() {
  _displaycontrol &= ~LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void ShiftRegLCD::blink() {
  _displaycontrol |= LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void ShiftRegLCD::scrollDisplayLeft(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void ShiftRegLCD::scrollDisplayRight(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void ShiftRegLCD::shiftLeft(void) {
  _displaymode |= LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void ShiftRegLCD::shiftRight(void) {
  _displaymode &= ~LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void ShiftRegLCD::shiftIncrement(void) {
  _displaymode |= LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void ShiftRegLCD::shiftDecrement(void) {
  _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// Allows us to fill the first 8 CGRAM locations with custom characters
void ShiftRegLCD::createChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7; // we only have 8 locations 0-7
  command(LCD_SETCGRAMADDR | location << 3);
  for (int i=0; i<8; i++) {
    write(charmap[i]);
  }
  command(LCD_SETDDRAMADDR); // Reset function to normal display
}


void ShiftRegLCD::command(uint8_t value) {
  send(value, LOW);
}

void ShiftRegLCD::write(uint8_t value) {
  send(value, HIGH);
}

void ShiftRegLCD::send(uint8_t value, uint8_t mode) {
  uint8_t val1, val2;
  mode = mode ? SR_RS_BIT : 0; // RS bit; LOW: command.  HIGH: character.
  val1 = SR_ENABLE_BIT | mode | ((value >> 2) & 0x3C); // upper nibble
  val2 = SR_ENABLE_BIT | mode | ((value << 2) & 0x3C); // lower nibble

  digitalWrite(_srdata_pin, LOW);
  shiftOutZero ( _srdata_pin, _srclock_pin, 6 );
  shiftOut (_srdata_pin, _srclock_pin, MSBFIRST, val1);
  digitalWrite(_srdata_pin, HIGH);
  delayMicroseconds(1);                 // enable pulse must be >450ns
  digitalWrite(_srdata_pin, LOW);
  shiftOutZero ( _srdata_pin, _srclock_pin, 6 );
  shiftOut (_srdata_pin, _srclock_pin, MSBFIRST, val2);
  digitalWrite(_enable_pin, HIGH);
  delayMicroseconds(1);                 // enable pulse must be >450ns
  digitalWrite(_enable_pin, LOW);
}

void ShiftRegLCD::send4bits(uint8_t value) {
  digitalWrite(_srdata_pin, LOW);
  shiftOutZero ( _srdata_pin, _srclock_pin, 6 );
  value = ((value >> 2) & 0x3C);
  shiftOut (_srdata_pin, _srclock_pin, MSBFIRST, value | SR_ENABLE_BIT);
  delayMicroseconds(1);                 // enable pulse must be >450ns
  digitalWrite(_enable_pin, HIGH);
  delayMicroseconds(1);                 // enable pulse must be >450ns
  digitalWrite(_enable_pin, LOW);
}
