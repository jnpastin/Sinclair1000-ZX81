# Hardware Documentation

## Contents

### [pin_mapping.md](pin_mapping.md)
Complete pin-to-pin connection reference between the Z80 CPU and Arduino Mega 2560.

**Includes:**
- Z80 pinout reference (40-pin DIP)
- Arduino Mega connection tables
- Port assignments (PORTA, PORTC, PORTK, PORTL, PORTB)
- Data bus mapping (D0-D7)
- Address bus mapping (A0-A15)
- Control signal mappings
- Power connections
- Available pins for future expansion

### [pin_mapping.csv](pin_mapping.csv)
Machine-readable version of the pin mapping table.

**Format:**
```
Z80_Pin,Z80_Signal,Direction,Arduino_Pin,Port,Bit,Notes
```

**Use Cases:**
- Import into spreadsheet applications
- Parse with scripts for code generation
- Reference for automated testing
- Validation tools

## Pin Summary

### Buses
| Bus | Z80 Pins | Arduino Pins | Port | Direction |
|-----|----------|--------------|------|-----------|
| Data (D0-D7) | 14,15,12,8,7,9,10,13 | 22-29 | PORTA | Bidirectional |
| Address Low (A0-A7) | 30-37 | 37-30 | PORTC | Input (from Z80) |
| Address High (A8-A15) | 38-40,1-5 | A8-A15 | PORTK | Input (from Z80) |

### Control Signals
| Signal Group | Arduino Port | Pins Used | Direction |
|--------------|--------------|-----------|-----------|
| Z80 Outputs | PORTL, PORTE, PORTH | 49-42,3,6 (8 pins) | Input to Arduino |
| Z80 Inputs | PORTB | 53,52,51,50,10,11 | Output from Arduino |

### Key Signals
- **CLK** - Pin 11 (Timer1/OC1A) → Z80 Pin 6
- **RESET** - Pin 53 → Z80 Pin 26
- **MREQ** - Pin 47 (PL2) ← Z80 Pin 19
- **RD** - Pin 3 (PE5/INT5) ← Z80 Pin 21
- **WR** - Pin 2 (PE4/INT4) ← Z80 Pin 22

## Port Assignments

### Why Port-Aligned?
Port-aligned pin assignments enable **256x faster** bus operations compared to digitalWrite:

```cpp
// Fast: Single instruction (1 cycle)
PORTA = data;  // Write entire 8-bit data bus

// Slow: 8 function calls (100+ cycles)
digitalWrite(pin0, bit0);
digitalWrite(pin1, bit1);
// ... etc
```

### Benefits
- **Sub-microsecond bus response** required for Z80 operation
- **Atomic operations** - entire bus updated simultaneously
- **Simple code** - direct register access
- **Hardware PWM** - Timer1 clock generation on pin 11

## Hardware Setup

### Required Components
- Arduino Mega 2560
- Z80 CPU (40-pin DIP)
- 40-pin DIP socket
- Perfboard with dual pin headers (80 headers total)
- Decoupling capacitor (0.1µF ceramic)
- Jumper wires

### Physical Construction
See [../../hardware/README.md](../../hardware/README.md) for:
- Perfboard layout guide
- Wiring instructions
- Photo documentation
- Test rig assembly

### Schematic
See [../../hardware/Z80 Test Rig/](../../hardware/Z80 Test Rig/) for:
- KiCad schematic files
- PCB layout (optional)
- Bill of materials

## Pin Usage Summary

**Total Pins Used:** 38 digital + 8 analog = 46 pins

| Purpose | Pin Count | Pins |
|---------|-----------|------|
| Data Bus | 8 | 22-29 (PORTA) |
| Address Bus | 16 | 30-37 (PORTC), A8-A15 (PORTK) |
| Control Inputs | 8 | 42-49 (PORTL) |
| Control Outputs | 6 | 10,11,50,51,52,53 (PORTB) |
| **Total Used** | **38** | |
| **Available** | **16** | D0-D9, D13-D21, D38-D41 |

### Available for Future Use
- Status LEDs
- Logic analyzer trigger
- Serial debugging
- Expansion headers
- Test mode switches

## Verification

### Continuity Testing
Use multimeter to verify each connection:
1. Power off Arduino
2. Insert Z80 into socket
3. Check continuity from Z80 pin to Arduino pin
4. Verify no shorts between adjacent pins
5. Check power rails (VCC, GND)

### Voltage Testing
1. Power on Arduino (no Z80 yet)
2. Verify 5V on VCC pin (Z80 pin 11)
3. Verify 0V on GND pin (Z80 pin 29)
4. Check all control outputs are HIGH (inactive)
5. Insert Z80 and recheck voltages

### Signal Testing
1. Upload clock test sketch
2. Measure clock output on pin 11 with scope
3. Verify 500 KHz square wave (default)
4. Test other frequencies (100K, 1M, 2M, 3M)
5. Check duty cycle (~50%)

## Troubleshooting

### Common Issues
- **No clock output** - Check pin 11 connection, verify Timer1 config
- **Wrong frequencies** - Check crystal (should be 16 MHz)
- **Intermittent signals** - Check solder joints, power supply
- **Bus conflicts** - Verify data bus direction control

### Debug Resources
- [Clock Troubleshooting Guide](../clock/CLOCK_TROUBLESHOOTING.md)
- [Hardware README](../../hardware/README.md)
- Logic analyzer recommended for bus debugging

## Related Documentation
- [Clock Module](../clock/README.md)
- [PROGMEM Usage](../development/PROGMEM_USAGE.md)
- [Main Documentation Index](../README.md)
- [Hardware Directory](../../hardware/)
- [Project TODO](../../TODO.md)

## Updates
- **November 6, 2025** - Moved to docs/hardware/
  - Pin mapping reference
  - CSV format for automation
  - Hardware documentation index
