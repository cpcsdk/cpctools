CPC Booster Firmware sourcecode
===============================

What you have here is the latest version of the CPC Bosster+ Firmware.

Some useful information:
 * To program this on an ATMega, you can use any ISP programmer. 
 * Flags to program:
	- Ext. Crystal/Resonator High Freq
	- Brown-out detection Level at VCC=4.0 V;
	- Preserve EEPROM memory through chip erase cycle
	- Serial Program Downloading (SPI) enabled (pin conflict with the CPC I/O)
	- The rest must not be selected (JTAG disabled).
    This gives a result of (avrdude options) -U lfuse:w:0x7f:m -U hfuse:w:0xf1:m

As this will DISABLE SPI programming, set the flags AFTER programming the
firmware. If you want to repair that you need an high voltage programmer or the
"fusebit doctor": http://mdiy.pl/atmega-fusebit-doctor-hvpp/?lang=en to recover
your chip.

