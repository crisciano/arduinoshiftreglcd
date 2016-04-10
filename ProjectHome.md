![http://i557.photobucket.com/albums/ss11/rar0n/Electronics/Arduino/arduionshiftreglcd-drawing2.jpg](http://i557.photobucket.com/albums/ss11/rar0n/Electronics/Arduino/arduionshiftreglcd-drawing2.jpg)

# NOTE: deprecated! #

**Yet** updated to Arduino 1.0 as well as pre-1.0 compatible, just in case. Big thanks to florian.fida who posted a huge pointer in [Issue #6](http://code.google.com/p/arduinoshiftreglcd/issues/detail?id=6).

Also, as a last update to this library I included a [proper LCD initialization](http://web.alfredstate.edu/weimandn/), as well as added LCD backlight control from formerly unused shift register bit #1.

The reason for deprecating this library, is now twofold:
  1. This library is now also included in a newer and improved (I guess except the proper LCD initialization, for now at least - but it still works!) LCD library that supports more connection methods. Check out the **[New LiquidCrystal library](https://bitbucket.org/fmalpartida/new-liquidcrystal/)** by Francisco.
    * That library supports controlling an LCD via:
      * 4 bit parallel
      * 8-bit parallel
      * I2C bus expander
      * shift register ([LCD3wire library](http://www.arduino.cc/playground/Code/LCD3wires) and arduinoshiftreglcd, so it's fully compatible wiring).
  1. I have made a (long overdue) sequel of sorts, [ShiftRegLCD123](http://code.google.com/p/shiftreglcd123/). It has a few added features, so I wanted to keep it separate.
    * Still only using shiftregisters, as the name suggests
      * 1, 2 and 3-wire mode
      * LCD3Wire and arduinoshiftreglcd wiring compatibility
      * Copes with 16x4 LCD's
      * Fully compatible with official LiquidCrystal library functions
      * LCD backlight control

In addition, Marc made a LCD3wire compatible port that he calls [NewLiquidCrystal](https://github.com/marcmerlin/NewLiquidCrystal) (without a space), if you have connected your LCD in that way. But this is also included in Francisco's new library now (and ShiftRegLCD123).

Also, please note that this was just a hack based on [LadyAda](http://web.alfredstate.edu/weimandn/arduino/LiquidCrystal_library/LiquidCrystal_index.html)'s improvement of the then-official (2009) LiquidCrystal library.

Anyway, below is the old (somewhat updated) description for reference, if needed.


---

**Quick links**




---


# Introduction #

This is a library for connecting an Arduino with only 2 or 3 wires to HD44780-compatible LCD's.

This was originally done via a very simple shift register, the 74LS164. It does not latch its outputs, so each output pin reflect the current "shift status" as bits are shifted in. In 2-wire mode this necessitated the enable-of-the-LCD-enable bit to be at the last position. Hence not pin compatible with LCD3wire library.

Solutions now exists for latched shift registers as well.


# Usage #

`#include <ShiftRegLCD.h>`

`ShiftRegLCD objectName(Datapin, Clockpin, Enablepin or TWO_WIRE [, Lines [, Font]])`
> where Lines and Font are optional.
  * Enablepin: can be replaced by constant TWO\_WIRE, if using only 2 wires.
  * Lines: 1 or 2 lines  (also 4-line LCD's must be set as 2).
  * Font : 0 or 1, small or big font (8 or 10 pixel tall font, if available).

The compulsory "Hello world" example using a two-wire connection:

```
#include <ShiftRegLCD.h>

const byte dataPin  = 10;    // SR Data from Arduino pin 10
const byte clockPin = 11;    // SR Clock from Arduino pin 11

ShiftRegLCD srlcd(dataPin, clockPin, TWO_WIRE);

void setup()
{
  // Turn on backlight (if used)
  srlcd.backlightOn();
  
  // Print a message to the LCD.
  srlcd.print("HELLO, WORLD!");
}

void loop()
{
}
```

More details on the [Usage](Usage.md) page.


---


# Mundane details #

Since I had an old HD44780 LCD display, I wanted to connect it to the Arduino, and still have some extra IO pins to use for other things than the LCD :) I hope this might be useful for others too.
As of now this library seem to be very reliable regarding starting/initializing the display (as opposed to the first version, at least on my LCD).

Generating the 8 allowable custom characters are also supported. Examples of usage are included.

I'm pretty new to the Arduino, but I have dabbled a little in both electronics and C++ before. But not much, this is the first class or library I've written (or modified) for C++. Or very C++ like language.


## Additional, somewhat technical dull details ##

Last (ish) update: as of 2012.02.09, I added in a backlight control bit #1 from the shift register (or bit #0 if using Bill's version of tying together CLK and RCK), as well as a proper LCD initialization from the link below. Schematics are not fully updated for now, but there is a backlight "antiflicker" circuit suggestion for 2-wire mode, and 3-wire with an unlatched shiftregister.

Do **not** connect the LCD backlight directly from the shift register to the LCD, use a driver circuit, such as a transistor and resistors.

As of 2009.07.27 there is a 2-wire connection mode in addition, and also a new schematics, since the enable of the enable-puls needed to be at the last bit position.

Also, a big thanks go to mircho from the arduino forum for pointing out a way of using only two wires! He found it [at ScienceProg.com.](http://www.scienceprog.com/interfacing-lcd-to-atmega-using-two-wires/) There should still be the option of using 3 wires as before, if needed (see third schematics).


**Two ways of connecting to a "shiftregistered" LCD:**
(With this library)

  * **2-wire mode**: The LCD Enable-pin is not used. The Data-pin is used as LCD Enable, together with the AND'ing from shift register bit #7 (the diode and resistor in the schematics form a simple AND gate). This method requires an extra write to the shift register with zeros to clear it between each write of the nibble (4 bits) to the LCD. This avoids accidental Enable-signals. IE 4 shift register writes total to transfer 1 byte to the LCD. Thus using only 2 pins from the Arduino.

  * **3-wire mode**: The Enable-pin goes directly from the Arduino to the LCD. Thus using three pins from the Arduino. No clearing of the shift register between writes is necessary, but since the LCD still operates in 4-bit mode, only a nibble is transferred at a time. IE 2 writes to the shift register for 1 byte to the LCD.

The time difference is not significant, although 3-wire method is slightly faster. (The clearing of the shift registers in 2-wire mode does not need the delays the LCD needs).

In both cases, the two other pins (data and clock) connects to the shift registers respective inputs. Basically any shift register should do. It's very simple, really.

**Note about latched shift registers** (Like the 74HC595):
  * **2-wire mode** Take a look at [issue #7](http://code.google.com/p/arduinoshiftreglcd/issues/detail?id=7) and the schematics below. Two versions now exists for this.
  * **3-wire mode** The shift register latch pin (RCL, STR or similar) is also connected to the LCD enable input. See schematics below.
Keep in mind that these are "quick'n'dirty" solutions. For the latter, the enable pulse will be on at the same time for both the shift register latch and the LCD Enable - but - it's very likely that the LCD is slower. And thus reads the shift register after it has latched new data. This seems to hold true.

There is no reading from the LCD, the R/!W pin is therefore hardwired to GND. I was playing with the idea of implementing that while still using the same connections from the Arduino, but now I don't think I'll ever implement that.

---



# License #

Original version is licensed under [Creative Commons Attribution-ShareAlike 3.0](http://creativecommons.org/licenses/by-sa/3.0/) License. I couldn't find that one here (except for other contents than code), so I hope [GNU GPL v3](http://www.gnu.org/licenses/gpl.html) is not too far off from that one.

## Acknowledgements ##

This library would never have been if not for [LadyAda's improved version](http://web.alfredstate.edu/weimandn/arduino/LiquidCrystal_library/LiquidCrystal_index.html) of the [Arduino "Official" LiquidCrystal library](http://arduino.cc/en/Reference/LiquidCrystal/)!

Don's excellent writeup on LCD initialization ([Character Based LCD Modules by Don](http://web.alfredstate.edu/weimandn/) - click on LCD Initialization).

mircho from the Arduino forum helped with the two-wire method and some coding.

Also thanks to florian.fida, CapnBry, Nadir, Nystrom and Bill for pointing out things!

---


# Schematics #

<font color='#FF0000'>
<h2>Note about the diode-resistor AND "gate"</h2>
</font>
Some tweaking might be in order to get this to work. Not all diodes can be used, and maybe you need to change the resistor value. As far as I have been able to determine, the diode needs to have a low as possible junction capacitance (and / or reverse bias capacitance as well). It seems diodes below some 5 pF (or preferably a bit lower?) should work with most LCD's and types of IC's (74HCxxx and 74LSxxx, CMOS etc.), but I cannot guarantee this, of course.

A diode like 1N4148 should work. Also, in a pinch, some small-signal transistor's base-emitter diodes can be used (2N3904 is reported to work).


<font color='#FF0000'>
<h2>Note about decoupling capacitors</h2>
</font>
The shiftregisters themselves cause sudden changes to the current drawn from it's power, and hence introduces noise. To counter this, a "decoupling capacitor" should be used as close to the shiftregister power pins as possible (Vcc and GND). Otherwise, the shiftregister may get faulty data and then the LCD won't work as intended (show gibberish characters etc). This is also dependent on other factors, like wire lengths etc.

<font color='#FF0000'>
Decoupling capacitors are not shown in the schematics!<br>
</font>
You will have to add those yourself, if needed. It's a good practice to use those anyway, if used correctly. And that is; between the power pins of the shiftregister. **Do not** connect capacitors directly from another pin to GND or Vcc, like in the erroneous, but otherwise useful [Arduino ShiftOut tutorial](http://arduino.cc/en/Tutorial/ShiftOut). Here it is described to connect a capacitor from the Latchpin to GND. **Dont!**

About 0.1 uF (100 nF) capacitance should do, the value is not too critical. Do not use electrolytic capacitors either btw, they have bad high frequency characteristics. Most other types in the 0.1uF range should work, like ceramic, polycarbonate, polyester etc. capacitors.

## Other notes ##
Just FYI, while the contrast pin is shown dragged (connected) to a connector, this of course is not necessary. Probably the best thing is to keep a contrast potmeter near the LCD and "set and forget" about it.


## 2-wire schematics ##

### Unlatched shiftregister ###

#### Basic principle ####

Principle of two-wire method with an unlatched shiftregister and an AND gate (The reason I'm not really using a real AND gate is to keep component count down):

**Not shown:** You should use a decoupling capacitor, ca 100 nF across the shiftregisters power pins, as close as possible to to the shiftregister! This goes for all schematics below too.

![http://i557.photobucket.com/albums/ss11/rar0n/Electronics/Arduino/andgate.png](http://i557.photobucket.com/albums/ss11/rar0n/Electronics/Arduino/andgate.png)


#### With diode-resistor AND gate ####

**Note, not shown:** The power connection for the 74LS164 and decoupling capacitors.

Diode: use 1N4148 or equivalent (works with HC family IC's also)

![http://i557.photobucket.com/albums/ss11/rar0n/Electronics/Arduino/LCD_2-wire.png](http://i557.photobucket.com/albums/ss11/rar0n/Electronics/Arduino/LCD_2-wire.png)


#### Backlight control ####

This prevents most backlight flicker/flimmer from about 20-40 FPS of constant writes to the LCD (also lower FPS somewhat, but some tweaking is neccesary then).

At higher FPS (30-40 ish Hz) the flicker is not too noticeable, but more importantly, it also makes the backlight turn off when off!
(Or almost so, unnoticeably very dim light. Basically, the light is off. At these update frequencies other artifacts show up too, like almost invisible characters at one end of the LCD etc.)

The RC net and diodes are not needed in 3-wire mode for latched shift registers.

![http://i557.photobucket.com/albums/ss11/rar0n/Electronics/Arduino/ShiftRegLCD123/SR_Backlight-2-wire-antiflicker-1.png](http://i557.photobucket.com/albums/ss11/rar0n/Electronics/Arduino/ShiftRegLCD123/SR_Backlight-2-wire-antiflicker-1.png)

### Latched shiftregisters ###

#### Nystrom's version ####

Thanks to Nystrom who posted it in [issue #7](http://code.google.com/p/arduinoshiftreglcd/issues/detail?id=7)

Any small-signal transistor should do, I used BC548B.
(Power connections to 74HC595 not shown)

![http://i557.photobucket.com/albums/ss11/rar0n/Electronics/Arduino/LCD_4-bit_serial_converter_2-wire-latching.png](http://i557.photobucket.com/albums/ss11/rar0n/Electronics/Arduino/LCD_4-bit_serial_converter_2-wire-latching.png)


#### Bill's version ####

Bill's version (who also posted it in [issue #7](http://code.google.com/p/arduinoshiftreglcd/issues/detail?id=7)).

**NOTE** Shift register bits are shifted to one lower bit position when connecting SRCLK and RCK together like this.

![http://i557.photobucket.com/albums/ss11/rar0n/Electronics/Arduino/LCD_4-bit_serial_converter_2-wire-latching-2.png](http://i557.photobucket.com/albums/ss11/rar0n/Electronics/Arduino/LCD_4-bit_serial_converter_2-wire-latching-2.png)



## 3-wire schematics ##

### Unlatched shiftregister ###

![http://i557.photobucket.com/albums/ss11/rar0n/Electronics/Arduino/LCD_3-wire.png](http://i557.photobucket.com/albums/ss11/rar0n/Electronics/Arduino/LCD_3-wire.png)


### Latched shiftregister ###
Nadir's solution from the Arduino forum, of simply tying together LCD Enable and shift register latch input.

![http://i557.photobucket.com/albums/ss11/rar0n/Electronics/Arduino/srlcd595.png](http://i557.photobucket.com/albums/ss11/rar0n/Electronics/Arduino/srlcd595.png)