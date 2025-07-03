# ROM Validation
In the process of trying to troubleshoot my Sinclair 1000, I wanted to validate that the ROM chip was still functional and that it had data on it.  The ROM is an 8K (8k words @ 8 bits/word) that is accessed via 13 address lines.  The chip is enabled by bringing the CS1 pin LOW.  **NOTE: The datasheet in this repository does not indicate that CS1 is active LOW, but on mine it was.**  Data is read via 8 output pins.

## Test methodology
The goal was to read each address on the ROM and ensure that data was returned.  An arduino based setup was the direction that I decided on.  Unfortunately, I do not have an Arduino Mega or other device with enough pins to control the entire chip.  However, I did have a few Nanos lying around, which works well on a breadboard as a bonus.  Between two Nanos, I was able to devise a circuit that: 
1. Controlled the entire chip
2. Only required any interaction on a single Nano
3. Output the content of the ROM for archival
4. Gave a visual indication of proper functioning
5. Allowed manual changes of the address and chip select lines
6. Could be built out on breadboards

The schematic for the circuit has been uploaded here.  Manual stepping through the program is accomplished by bringing the `Halt` pin on the second device high.  Bringing the `ManualStep` pin high momentarily will execute one iteration of the loop.  If the two devices ever get out of sync, simply press the reset switch on both.
