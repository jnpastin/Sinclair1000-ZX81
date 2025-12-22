# Clock Troubleshooting Guide

## Issue: Clock Not Running

### Changes Made to Fix

1. **Updated `pins.h`**: Changed `Z80_CLK` from pin 46 to pin 11
   - Pin 11 is the correct hardware PWM output for Timer1 (OC1A/PB5)
   - Pin 46 does not support Timer1 PWM

2. **Added Debug Output**: Enhanced `initializeClock()` to show:
   - Timer configuration values (prescaler, ICR1, OCR1A)
   - Register values (TCCR1A, TCCR1B)
   - Pin configuration confirmation

3. **Added Test Function**: `testClockOutput()` for hardware verification
   - Manually toggles pin 11 at 1 Hz for visual confirmation
   - Helps verify physical connections
   - Press 't' in serial monitor to run test

### Diagnostic Steps

#### Step 1: Upload and Check Serial Output

Upload the updated code and open Serial Monitor (115200 baud). You should see:

```
Z80 Processor Validation Suite
================================

Initializing hardware...
  Configuring Timer1: prescaler=1, ICR1=31, OCR1A=15
  TCCR1A=0x82, TCCR1B=0x19
  Pin 11 configured as OUTPUT
  Clock: Running at 500000 Hz
Hardware initialized.
```

**Expected Values for 500 KHz:**
- Prescaler: 1
- ICR1: 31
- OCR1A: 15 (50% duty cycle)
- TCCR1A: 0x82 (binary 10000010)
  - COM1A1=1, COM1A0=0 (non-inverting PWM)
  - WGM11=1, WGM10=0 (Fast PWM mode part)
- TCCR1B: 0x19 (binary 00011001)
  - WGM13=1, WGM12=1 (Fast PWM mode 14)
  - CS12=0, CS11=0, CS10=1 (prescaler = 1)

#### Step 2: Test Hardware Connection

In Serial Monitor, press `t` to run the test function:

```
Testing clock output on pin 11...
Watch for LED or measure with multimeter/scope
Press any key to stop test

Cycle 1: HIGH
       LOW
Cycle 2: HIGH
       LOW
...
```

**What to check:**
- If you have an LED+resistor, connect it between pin 11 and GND
- LED should blink once per second during test
- With multimeter: voltage should alternate between 0V and 5V
- With oscilloscope: should see square wave at test frequency

#### Step 3: Verify Timer1 Registers

Press `?` to show clock status. If it shows "Running at 500000 Hz", the software thinks it's working.

#### Step 4: Check Physical Connections

Verify Arduino Mega 2560 pin 11:
- Physical location: Digital pin 11 on the board
- Should be labeled "~11" (tilde indicates PWM capability)
- Located on the digital pin header between pins 10 and 12

#### Step 5: Measure with Oscilloscope/Logic Analyzer

Connect probe to pin 11:
- **At 500 KHz**: Should see square wave with 2 μs period (1 μs high, 1 μs low)
- **At 1 MHz**: 1 μs period (500 ns high, 500 ns low)
- **At 100 KHz**: 10 μs period (5 μs high, 5 μs low)

### Common Issues and Solutions

#### Issue 1: No Output on Pin 11

**Possible Causes:**
- Wrong physical pin (double-check it's pin 11)
- Pin damaged or non-functional
- Arduino Mega vs Uno confusion (pin functions differ)

**Solution:**
- Try the test function (`t` command) to verify basic pin control
- Try a simple blink sketch on pin 11 to verify hardware
- Check for shorts or damage on the board

#### Issue 2: Wrong Frequency

**Possible Causes:**
- Clock calculation error
- Wrong prescaler setting
- Crystal oscillator issue

**Solution:**
- Verify with oscilloscope
- Try different preset frequencies (1, 2, 3, 4, 5 keys)
- Check that Arduino is running at 16 MHz (standard Mega 2560)

#### Issue 3: Intermittent Output

**Possible Causes:**
- Poor connection
- Power supply issues
- Noise/interference

**Solution:**
- Check USB power supply quality
- Try external 5V power
- Add decoupling capacitors (0.1 μF near Arduino)

#### Issue 4: Timer Configuration Wrong

**Symptoms:**
- TCCR1A or TCCR1B values don't match expected
- Clock frequency wildly incorrect

**Solution:**
- Verify register values in serial output
- Check for conflicting libraries (Servo, Tone, etc. also use timers)
- Ensure no other code is modifying Timer1

### Verification Commands

Once uploaded, try these commands in Serial Monitor:

```
?    - Should show: "Clock status: Running at 500000 Hz"
t    - Should toggle pin visibly (connect LED to verify)
s    - Stop clock, then measure pin (should be steady LOW)
r    - Resume clock
2    - Set to 500 KHz (default)
3    - Set to 1 MHz (measure to verify)
```

### Expected Frequencies

| Command | Frequency | Period | High Time | Low Time |
|---------|-----------|--------|-----------|----------|
| 1       | 100 KHz   | 10 μs  | 5 μs      | 5 μs     |
| 2       | 500 KHz   | 2 μs   | 1 μs      | 1 μs     |
| 3       | 1 MHz     | 1 μs   | 500 ns    | 500 ns   |
| 4       | 2 MHz     | 500 ns | 250 ns    | 250 ns   |
| 5       | 3 MHz     | 333 ns | 167 ns    | 167 ns   |

### Alternative Test: Simple Blink

If all else fails, try this simple test to verify pin 11 works:

```cpp
void setup() {
  pinMode(11, OUTPUT);
}

void loop() {
  digitalWrite(11, HIGH);
  delay(500);
  digitalWrite(11, LOW);
  delay(500);
}
```

If this doesn't work, pin 11 hardware may be damaged.

### Hardware Schematic Check

Verify your wiring:
```
Arduino Mega Pin 11 (Digital) ──→ Z80 Pin 6 (CLK)
                                   
Optional for testing:
Arduino Mega Pin 11 ──┬── LED Anode (+)
                      └── 220Ω Resistor ── LED Cathode (-) ── GND
```

### Next Steps

If clock still not working after these checks:

1. **Report back with**:
   - Serial output from startup
   - Serial output from `?` command
   - Serial output from `t` command
   - Oscilloscope/logic analyzer capture if available

2. **Try**:
   - Different Arduino Mega board (if available)
   - Different USB cable/power source
   - Reset Arduino completely (unplug, wait 10 seconds, replug)

3. **Consider**:
   - Using external clock source temporarily
   - Testing with slower frequencies first (100 KHz)
   - Checking for library conflicts

## Quick Reference

### Serial Commands
- `1` = 100 KHz
- `2` = 500 KHz (default)
- `3` = 1 MHz
- `4` = 2 MHz
- `5` = 3 MHz
- `s` = Stop
- `r` = Resume
- `p` = Single pulse
- `t` = Test mode
- `?` = Status

### Pin 11 Location
On Arduino Mega 2560, pin 11 is:
- Row: Digital pins (not analog)
- Position: Between pins 10 and 12
- Label: ~11 (tilde indicates PWM)
- Function: OC1A (Timer1 output A)
