Wordclock
=========
Arduino Word Clock with minute resolution of time in words and linear display of seconds.

Rather than using custom PCBs cheap off the shelf modules are used instead. This includes the touch sensor modules that use the TTP223 IC.

The Display LEDs are built directly into the front panel so any size clock can be built. 

The clock includes the following modes, word clock, digital clock, analogue clock, temperature & humidity, & also three games, Game of Life, Simon & Tetris.

The clock can be stand alone or run as a slave off a Master Clock if required.

In stand alone mode the clock runs off it's built in temperature compensated real time clock with an accuracy of ±2ppm from 0°C to +40°C

When running as a slave off a Master Clock time is synchronised on every 30seconds past the minute.

Summer/Winter time is also automatically adjusted by one of my Pragotron Master Clocks on the BSTin (D7 Nano) pin.

A manual switch can be added instead if quick summer/winter time change is required.

There is an option for PIR control so the clock automatically turns off when no one is in the room.

The Clock measures 500mm x 500mm weighs 12lb (5.5Kg) and is designed to be wall mounted.

There are touch pads in each corner to setup and control the clock.

A mini USB socket provides brightness information over the serial port and also allows software programming in situe.






Web site: http://home.btconnect.com/brettoliver1/Word_Clock/Word_Clock.htm

Instructables: https://www.instructables.com/id/Arduino-Word-Clock/

![alt tag](https://raw.githubusercontent.com/brettoliver/wordclock/master/faceplate/Brett11_print_ready.png)

![alt tag](https://raw.githubusercontent.com/brettoliver/wordclock/master/drawings/Word_clock_side_animation.gif)

![alt tag](https://raw.githubusercontent.com/brettoliver/wordclock/master/hardware/Schematic.png)

![alt tag](https://raw.githubusercontent.com/brettoliver/wordclock/master/hardware/Schmatic_mod01.jpg)

![alt tag](https://raw.githubusercontent.com/brettoliver/wordclock/master/hardware/Schmatic_mod02.jpg)

![alt tag](https://raw.githubusercontent.com/brettoliver/wordclock/master/hardware/Schmatic_mod03.jpg)

![alt tag](https://raw.githubusercontent.com/brettoliver/wordclock/master/hardware/Schmatic_mod04.jpg)

[![WordClock on YouTube](http://img.youtube.com/vi/nqQW0eoTaXs/0.jpg)](https://youtu.be/nqQW0eoTaXs)


<a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-nc-sa/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/">Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License</a>.
