# RAM Validation
In the process of trying to troubleshoot my Sinclair 1000, I wanted to validate that the SRAM chip was still functional.  The RAM is an 2K (2k words @ 8 bits/word) that is accessed via 11 address lines.  The chip is enabled by bringing the CS pin LOW.  Writes are enabled by bringing the WE pin LOW, while reads are enabled by bringing the OE pin LOW.  Note that the WE pin takes precedence over the OE pin in the event that both are low.  Data is written and read via 8 I/O pins.

## Test methodology
The goal was to test each address in the SRAM by writing each possible 8 bit combination and ensuring that the correct data was returned on read.  An arduino based setup was the direction that I decided on.  Unfortunately, I do not have an Arduino Mega or other device with enough pins to control the entire chip.  However, I did have a few Nanos lying around, which works well on a breadboard as a bonus.  Between two Nanos, I was able to devise a circuit that: 
1. Controlled the entire chip
2. Only required any interaction on a single Nano
3. Tested each of 256 combinations for each address
4. Gave a visual indication of proper functioning
5. Allowed manual stepping through the loops
6. Could be built out on breadboards

The schematic for the circuit and the program flowchart have been uploaded here.  Manual stepping through the program is accomplished by bringing the `Halt` pin on the second device high.  Bringing the `ManualStep` pin high momentarily will execute one iteration of the loop.  If the two devices ever get out of sync, simply press the reset switch on both.
