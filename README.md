# Moon phase calendar
For [Arduino](https://www.arduino.cc/), using an [Adafruit Pro Trinket](https://www.adafruit.com/products/2010) board.

The moon phase calendar shows a date and the [moon phase](http://www.timeanddate.com/astronomy/moon/about-moon-phases.html) for that date. The date starts at a pre-programmed start date. A rotary encoder lets you adjust the date forward and backward. As an alternative to the rotary encoder you can use a push button to advance the date.

The date is shown on a 4-digit 8-segment display as `ddmm`, alternating with the year shown as `yyyy`. The moon phase is presented as one of eight phases via four series of LEDs. 

In the initial stages of creating the planned functionality, we'll omit the manual inputs and automatically advance the date once a second.  

Moon phase calendar hardware
------------------------------

The diagram below gives a schematic overview of the circuitry.
```                         
                              ^
 Adjust Date                  |                
next/previous         +-------+-------+         Moon phase
     o  o             |               |            o  o
  o        o          |               |         o  o  o  o
 o          o ---/--->|               |---/-----o  o  o  o
 o          o    2    |               |   4     o  o  o  o
  o        o          |  Pro Trinket  |            o  o
     o  o             |               |       4-segment LEDs
Rotary encoder        |               |        
                      |               |           Date
                      |               |        ddmm [yyyy]
      next            |               |        +---------+
      +||--------/--->|               |---/----| 3 1 1 2 |
                 1    |               |   2    +---------+
   Push button        |               |       4-digit display
                      |               |      
                      +-------+-------+
                              |
                             --- 
```

We use the following components:
- [Adafruit Pro Trinket - 3V 12MHz](https://www.adafruit.com/products/2010) (*)
- [Rotary encoder](), input initially unused
- [Push button](), alternate input initially unused
- [Grove - 4-Digit Display](http://www.seeedstudio.com/wiki/Grove_-_4-Digit_Display)
- 16 LEDs (2 groups of three, 2 groups of five LEDs)

(*) You can also use the 5V version.

Moon phase calendar software
------------------------------
To perform its function the program must be able to:

1. Determine a date before or after another date. 
1. Determine the moon phase of a specific date. 

### Compute new date
To compute a new date from an existing one we need a function to advance a date, say 28 March 2016 to 29 March 2016 (note the leap year) and from 29 March to 1 April 2016. The standard C library contains funtion [mktime]() that allows to *just add a day* to a date and compute a valid date from a probably invalid date. So given 30 March 2016 `mktime()` will adjust the date to 1 April 2016. 

As the standard C library functions aren't readily available in the Arduino environment, we created the needed functionality inspired on function [mktime()](https://www.sourceware.org/git/gitweb.cgi?p=newlib-cygwin.git;a=blob_plain;f=newlib/libc/time/mktime.c;hb=HEAD) from the [newlib library](https://en.wikipedia.org/wiki/Newlib) maintained by [Red Hat](). Our function only handles dates (not time).

### Compute moon phase
For the moon phase we need a function that can compute the phase from date. ... 


Notes and references
----------------------

### Arduino

### Moon phase

### Hardware


 