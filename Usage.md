

# Usage #

This list of functions (or methods) of the library can basically be obtained by looking at the ShiftRegLCD.cpp.

The examples is for an object named `srlcd`. Of course you can name the object whatever you want. Or even have more than one.


## Installing ##

### Old method ###
Pre-v 0017, but still works.
  * Download the most current version of the library
  * Unrar (extract) the ShiftRegLCD folder, and all its contents into a subfolder in the Arduino environment. It must be in this folder (xxxx is the Arduino version):
> > `arduino-xxxx\hardware\libraries\ShiftRegLCD`
  * Then, when you start the Arduino IDE, it should contain a new menu "ShiftRegLCD" with examples. (**File > Examples > ShiftRegLCD**...). It should also appear in the menu **Sketch > import library**.

### New method ###

  * Download the most current version of the library
  * Create a subfolder in your arduino sketch folder named "libraries", if it doesn't exist. [As described here](http://arduino.cc/blog/?p=313=1)
  * Extract the library. It should be in a subfolder of its own (somthing like /documents/arduino/libraries/ShiftRegLCD/).
  * Then, when you start the Arduino IDE, it should contain a new menu "ShiftRegLCD" with examples. (**File > Examples > ShiftRegLCD**...). It should also appear in the menu **Sketch > import library**.


## Initialization ##

```
#include <ShiftRegLCD.h>
ShiftRegLCD srlcd(Datapin, Clockpin, Enablepin or TWO_WIRE [, Lines [, Font]])
```

> where Lines and Font are optional.
  * Enablepin: can be replaced by constant TWO\_WIRE, if using only 2 wires.
  * Lines: 1 or 2 lines (also 4-line LCD's must be set as 2 - use the setCursor method to access the lines (rows)).
  * Font : 0 or 1, small or big font (8 or 10 pixel tall font, if available).


### Examples ###

Using two wires, 1-line display. Data is on pin 12, clock on pin 13. Enable will also be on the Data output:

```
#include <ShiftRegLCD.h>
ShiftRegLCD srlcd(12, 13, TWO_WIRE);
```


Using two wires, 1-line display with a big (10 pixel high) font (which according to specs can only be on a 1-line display):

```
#include <ShiftRegLCD.h>
ShiftRegLCD srlcd(12, 13, TWO_WIRE, 1,1 );
```


Or, if using three wires, and a 2-line display. Enable will be on pin 8:

```
#include <ShiftRegLCD.h>
ShiftRegLCD srlcd(12, 13, 8, 2);
```







# Functions #

## Backlight ##
Formerly unused shift register bit #1 (or bit#0 if using Bill's method), can now be used to control backlight on/off.

```
srlcd.backlightOn(); // Turn backlight on
```

```
srlcd.backlightOff(); // Turn backlight off
```


## Clear display ##
```
srlcd.clear(); // Clear display, set cursor position to zero
```


## Cursor positioning ##
```
srlcd.home();  // Set cursor position to zero
```
```
srlcd.setCursor(column, row); // Sets cursor position
```
Remember row begins at row 0 (zero), for the first line.
Also column begins at column 0.

## Turn the display on/off ##
```
srlcd.noDisplay(); // Turn the display off
```
```
srlcd.display();   // Turn the display on
```


## Cursor types ##
```
srlcd.noCursor();//Turns the underline cursor off
```
```
srlcd.cursor();  // Turns the underline cursor on
```

```
srlcd.noBlink(); // Turn the blinking cursor off
```
```
srlcd.blink();   // Turn the blinking cursor on
```


## Scrolling ##
These commands scroll the display without changing the display RAM
```
srlcd.scrollDisplayLeft();
```
```
srlcd.scrollDisplayRight();
```


## Text flow direction ##
```
srlcd.shiftLeft();  //  This is for text that flows Left to Right
```
```
srlcd.shiftRight(); // This is for text that flows Right to Left
```


## Text justification ##
```
srlcd.shiftIncrement(); // This will 'right justify' text from the cursor
```
```
srlcd.shiftDecrement(); // This will 'left justify' text from the cursor
```



## Custom character generation ##
There can be up to 8 custom generated characters.
```
srlcd.createChar(uint8_t location, uint8_t charmap[])
```
Where `location` is a number from 0 to 7, and `charmap[]` is an 8 element byte array of a character pattern. Beware the characters are only 5 pixels wide, so the 3 upper bits must always be zero.

For example, to make a bell character as custom character nr. 3:

```
uint8_t bell[8]  = { B00000100,
                     B00001110,
                     B00001110,
                     B00001110,
                     B00011111,
                     B00000000,
                     B00000100 };

srlcd.createChar(3, bell);
```