# Z80 to Arduino Mega 2560 Pin Mapping

## ✅ HARDWARE UPDATE COMPLETE - November 7, 2025

**MREQ signal relocated from D49 (PL0) to D2 (PE4/INT4)** ✅

**Control signals reassigned on PORTL (D48-D42, PL1-PL7)** ✅

This change enables external interrupt capability for optimal ISR performance. See [../Hardware_Pin_Change.md](../Hardware_Pin_Change.md) for:
- Complete hardware modification guide
- Step-by-step instructions
- Performance benefits (13% faster, 2.5 MHz Z80 support)
- Testing procedures

**This document reflects the completed pin assignments as of November 7, 2025.**

---

## Overview
This document describes the pin-to-pin connections between the Z80 CPU (40-pin DIP) and the Arduino Mega 2560 for the Z80 Validation Test Rig.

## Z80 Pinout Reference (40-pin DIP)

### Left Side (Pins 1-20)
| Z80 Pin | Signal   | Direction | Description                |
|---------|----------|-----------|----------------------------|
| 1       | A11      | Output    | Address Bus bit 11         |
| 2       | A12      | Output    | Address Bus bit 12         |
| 3       | A13      | Output    | Address Bus bit 13         |
| 4       | A14      | Output    | Address Bus bit 14         |
| 5       | A15      | Output    | Address Bus bit 15         |
| 6       | CLK      | Input     | Clock Input                |
| 7       | D4       | Bidir     | Data Bus bit 4             |
| 8       | D3       | Bidir     | Data Bus bit 3             |
| 9       | D5       | Bidir     | Data Bus bit 5             |
| 10      | D6       | Bidir     | Data Bus bit 6             |
| 11      | VCC      | Power     | +5V Power Supply           |
| 12      | D2       | Bidir     | Data Bus bit 2             |
| 13      | D7       | Bidir     | Data Bus bit 7             |
| 14      | D0       | Bidir     | Data Bus bit 0             |
| 15      | D1       | Bidir     | Data Bus bit 1             |
| 16      | INT      | Input     | Interrupt Request          |
| 17      | NMI      | Input     | Non-Maskable Interrupt     |
| 18      | HALT     | Output    | Halt Status                |
| 19      | MREQ     | Output    | Memory Request             |
| 20      | IORQ     | Output    | I/O Request                |

### Right Side (Pins 21-40)
| Z80 Pin | Signal   | Direction | Description                |
|---------|----------|-----------|----------------------------|
| 21      | RD       | Output    | Read                       |
| 22      | WR       | Output    | Write                      |
| 23      | BUSAK    | Output    | Bus Acknowledge            |
| 24      | WAIT     | Input     | Wait                       |
| 25      | BUSRQ    | Input     | Bus Request                |
| 26      | RESET    | Input     | Reset                      |
| 27      | M1       | Output    | Machine Cycle 1            |
| 28      | RFSH     | Output    | Refresh                    |
| 29      | GND      | Power     | Ground                     |
| 30      | A0       | Output    | Address Bus bit 0          |
| 31      | A1       | Output    | Address Bus bit 1          |
| 32      | A2       | Output    | Address Bus bit 2          |
| 33      | A3       | Output    | Address Bus bit 3          |
| 34      | A4       | Output    | Address Bus bit 4          |
| 35      | A5       | Output    | Address Bus bit 5          |
| 36      | A6       | Output    | Address Bus bit 6          |
| 37      | A7       | Output    | Address Bus bit 7          |
| 38      | A8       | Output    | Address Bus bit 8          |
| 39      | A9       | Output    | Address Bus bit 9          |
| 40      | A10      | Output    | Address Bus bit 10         |

## Arduino Mega 2560 Connections

### Data Bus (D0-D7) - PORTA (Pins 22-29) - Bidirectional
**All 8 data bits on single port for atomic read/write operations**

| Z80 Pin | Z80 Signal | Arduino Pin | Port | Bit | Notes           |
|---------|------------|-------------|------|-----|-----------------|
| 14      | D0         | 22          | PA0  | 0   | Data Bus bit 0  |
| 15      | D1         | 23          | PA1  | 1   | Data Bus bit 1  |
| 12      | D2         | 24          | PA2  | 2   | Data Bus bit 2  |
| 8       | D3         | 25          | PA3  | 3   | Data Bus bit 3  |
| 7       | D4         | 26          | PA4  | 4   | Data Bus bit 4  |
| 9       | D5         | 27          | PA5  | 5   | Data Bus bit 5  |
| 10      | D6         | 28          | PA6  | 6   | Data Bus bit 6  |
| 13      | D7         | 29          | PA7  | 7   | Data Bus bit 7  |

### Address Bus (A0-A15) - PORTC + PORTK - Inputs to Arduino
**16-bit address on two complete ports for fast read operations**

#### Low Byte (A0-A7) - PORTC (Pins 37-30)
| Z80 Pin | Z80 Signal | Arduino Pin | Port | Bit | Notes              |
|---------|------------|-------------|------|-----|--------------------|
| 30      | A0         | 37          | PC0  | 0   | Address Bus bit 0  |
| 31      | A1         | 36          | PC1  | 1   | Address Bus bit 1  |
| 32      | A2         | 35          | PC2  | 2   | Address Bus bit 2  |
| 33      | A3         | 34          | PC3  | 3   | Address Bus bit 3  |
| 34      | A4         | 33          | PC4  | 4   | Address Bus bit 4  |
| 35      | A5         | 32          | PC5  | 5   | Address Bus bit 5  |
| 36      | A6         | 31          | PC6  | 6   | Address Bus bit 6  |
| 37      | A7         | 30          | PC7  | 7   | Address Bus bit 7  |

#### High Byte (A8-A15) - PORTK (Pins A8-A15 / 62-69)
| Z80 Pin | Z80 Signal | Arduino Pin | Port | Bit | Notes              |
|---------|------------|-------------|------|-----|--------------------|
| 38      | A8         | A8 (62)     | PK0  | 0   | Address Bus bit 8  |
| 39      | A9         | A9 (63)     | PK1  | 1   | Address Bus bit 9  |
| 40      | A10        | A10 (64)    | PK2  | 2   | Address Bus bit 10 |
| 1       | A11        | A11 (65)    | PK3  | 3   | Address Bus bit 11 |
| 2       | A12        | A12 (66)    | PK4  | 4   | Address Bus bit 12 |
| 3       | A13        | A13 (67)    | PK5  | 5   | Address Bus bit 13 |
| 4       | A14        | A14 (68)    | PK6  | 6   | Address Bus bit 14 |
| 5       | A15        | A15 (69)    | PK7  | 7   | Address Bus bit 15 |

### Control Signals - Outputs from Z80 (Inputs to Arduino)
**✅ HARDWARE COMPLETE: November 14, 2025**  
**Separate ISRs: RD on D3 (PE5/INT5), WR on D2 (PE4/INT4)**
**Control signals distributed across PORTL, PORTE, PORTH for interrupt capability**

| Arduino Pin | Port | Bit | Z80 Pin | Z80 Signal | Notes                        |
|-------------|------|-----|---------|------------|------------------------------|
| 3           | PE5  | 5   | 21      | RD         | Read (active low) - INT5     |
| 2           | PE4  | 4   | 22      | WR         | Write (active low) - INT4    |
| 42          | PL7  | 7   | 23      | BUSAK      | Bus Acknowledge (active low) |
| 43          | PL6  | 6   | 18      | HALT       | Halt Status (active low)     |
| 44          | PL5  | 5   | 28      | RFSH       | Refresh (active low)         |
| 45          | PL4  | 4   | 27      | M1         | Machine Cycle 1 (active low) |
| 46          | PL3  | 3   | -       | -          | **Unused**                   |
| 47          | PL2  | 2   | 19      | MREQ       | Memory Request (active low)  |
| 48          | PL1  | 1   | 20      | IORQ       | I/O Request (active low)     |
| 49          | PL0  | 0   | -       | -          | **Unused**                   |

### Control Signals - Inputs to Z80 (Outputs from Arduino) - PORTB
**All Arduino output control signals on single port**

| Z80 Pin | Z80 Signal | Arduino Pin | Port | Bit | Notes                              |
|---------|------------|-------------|------|-----|------------------------------------|
| 26      | RESET      | 53          | PB0  | 0   | Reset (active low)                 |
| 24      | WAIT       | 52          | PB1  | 1   | Wait State (active low)            |
| 16      | INT        | 51          | PB2  | 2   | Interrupt Request (active low)     |
| 17      | NMI        | 50          | PB3  | 3   | Non-Maskable Interrupt (active low)|
| 25      | BUSRQ      | 10          | PB4  | 4   | Bus Request (active low)           |
| 6       | CLK        | 11          | PB5  | 5   | Clock - Timer1 OC1A output         |

### Power Connections
| Z80 Pin | Z80 Signal | Arduino Source | Notes                     |
|---------|------------|----------------|---------------------------|
| 11      | VCC        | 5V pin         | +5V power from Arduino    |
| 29      | GND        | GND pin        | Ground from Arduino       |

## Arduino Mega Pin Summary

### Ports Used
- **PORTA (22-29)**: Data Bus D0-D7 (8 pins) - Bidirectional
- **PORTC (37-30)**: Address Bus A0-A7 (8 pins) - Input
- **PORTK (A8-A15)**: Address Bus A8-A15 (8 pins) - Input  
- **PORTL (49-42)**: Control Inputs from Z80 (6 pins: 42-45,47-48) - Input ✅ **UPDATED**
- **PORTE (3)**: RD signal (1 pin: PE5, INT5) - Input with external interrupt ✅ **UPDATED**
- **PORTE (2)**: WR signal (1 pin: PE4, INT4) - Input with external interrupt ✅ **UPDATED**
- **PORTB (53,52,51,50,10,11)**: Control Outputs to Z80 + Clock (6 pins) - Output

### Total Pins Used: 38 digital pins + 8 analog pins
- Data Bus: 8 pins (PORTA)
- Address Bus: 16 pins (PORTC + PORTK)
- Control Inputs: 8 pins (6 on PORTL + 2 on PORTE) ✅ **UPDATED**
- Control Outputs: 6 pins (PORTB, includes Timer1 for clock)
- Power: 2 connections (5V, GND)

### Available Pins for Future Use
- **Digital**: D0, D1, D2, D4, D5, D7-D9, D13-D21, D38-D41, D46, D49 (~22 pins available) ✅ 
- **Analog**: A0-A7 (8 pins available if not needed)
- Can add: status LEDs, trigger inputs, logic analyzer sync, serial debugging, etc.

## Key Design Features

### Performance Optimization
- **Separate External Interrupts on RD (INT5) and WR (INT4)**: Dedicated ISRs eliminate polling loops
  - Faster response time (<1µs) for memory operations
  - Supports Z80 at up to 5 MHz (500 KHz target)
  - See [../separate_isr_implementation_plan.md](../separate_isr_implementation_plan.md) for complete analysis
  
- **Atomic Port Operations**: All buses on complete 8-bit ports for single-instruction read/write
  - Data bus: Single PORTA read/write
  - Address low: Single PORTC read  
  - Address high: Single PORTK read
  - Control signals: Distributed across PORTL, PORTE, PORTH for interrupt capability

## Signal Characteristics

### Active Low Signals
The following signals are **active LOW** (logic 0 = active):
- MREQ, IORQ, RD, WR, M1, RFSH
- HALT, BUSAK
- RESET, WAIT, INT, NMI, BUSRQ

### Clock Signal
- **CLK (Z80 pin 6)**: Generated by Arduino Timer1
- Connected to Arduino pin 11 (PB5 / OC1A)
- Frequency: Adjustable (typically 1-4 MHz for testing)
- Duty cycle: 50%
- Hardware timer provides stable, jitter-free clock generation

**Timer1 Configuration for 4 MHz Clock:**
```c
// Configure Timer1 for 4 MHz clock output (50% duty cycle)
DDRB |= (1 << PB5);                  // Set pin 11 as output
TCCR1A = (1 << COM1A0);              // Toggle OC1A on compare match
TCCR1B = (1 << WGM12) | (1 << CS10); // CTC mode, no prescaler
OCR1A = 1;                            // 4 MHz: (16MHz / (2 * 1 * 4MHz)) - 1
TCNT1 = 0;                            // Reset counter
```

### Data Bus Direction Control
The data bus (D0-D7) on PORTA must be configured as:
- **INPUT** when Z80 is writing (WR is LOW, MREQ is LOW)
- **OUTPUT** when Z80 is reading (RD is LOW, MREQ is LOW)
- **INPUT** (high impedance) at all other times

**Fast Direction Control:**
```c
// Set data bus as output (1 instruction)
DDRA = 0xFF;

// Set data bus as input (1 instruction)
DDRA = 0x00;
```

## Fast Port Operations

### Single-Instruction Bus Access
Direct port access enables extremely fast bus operations:

```c
// Read entire data bus (1 instruction, ~62.5 ns @ 16 MHz)
uint8_t data = PINA;

// Write entire data bus (1 instruction, ~62.5 ns @ 16 MHz)
PORTA = data;

// Read 16-bit address (2 instructions, ~125 ns @ 16 MHz)
uint16_t address = PINC | ((uint16_t)PINK << 8);

// Read control signals from PORTL (1 instruction, ~62.5 ns @ 16 MHz)
uint8_t control = PINL;  // 6 signals: IORQ, MREQ, M1, RFSH, HALT, BUSAK

// Check memory request (MREQ on PL2, active low) ⚠️ UPDATED
bool mreq_active = !(PINL & (1 << PL2));

// Check read cycle (RD on PE5, active low) ⚠️ UPDATED
bool read_active = !(PINE & (1 << PE5));

// Check write cycle (WR on PE4, active low) ⚠️ UPDATED
bool write_active = !(PINE & (1 << PE4));

// Set all control outputs inactive (1 instruction)
PORTB = 0x1F;  // Bits 0-4 high (active low signals)
```

### Performance Comparison

| Operation | digitalWrite/Read Method | Direct Port Method | Speedup |
|-----------|-------------------------|-------------------|---------|
| Data bus read | ~16 µs (8 calls) | ~0.0625 µs (1 instruction) | 256x |
| Data bus write | ~16 µs (8 calls) | ~0.0625 µs (1 instruction) | 256x |
| Address read | ~32 µs (16 calls) | ~0.125 µs (2 instructions) | 256x |
| Control read | ~16 µs (8 calls) | ~0.0625 µs (1 instruction) | 256x |

**Total bus cycle response time:**
- digitalWrite/Read method: >50 µs
- Direct port method: <1 µs

This allows the Arduino to respond within Z80 timing requirements even at 4 MHz clock speeds!

## Hardware Modification History

### November 14, 2025 - Separate RD/WR ISRs ⚠️ CRITICAL

**Change:** Control signals redistributed for separate external interrupts
- MREQ: D2 (PE4/INT4) → D47 (PL2)
- RD: D47 (PL2) → D3 (PE5/INT5) 
- WR: D46 (PL3) → D2 (PE4/INT4)

**Reason:** Single MREQ ISR with polling causes missed interrupts at 500KHz. Separate ISRs eliminate polling:
- Dedicated ISR for each operation type (ISR(INT5_vect) for reads, ISR(INT4_vect) for writes)
- No polling loops in ISRs
- Faster response time (<1µs)
- Support for Z80 at 5 MHz (500 KHz target)

**Impact:**
- MREQ, RD, WR now on interrupt-capable pins
- New global counters: g_readIsrCount, g_writeIsrCount
- New address tracking: g_lastReadAddress, g_lastWriteAddress
- Control signals distributed across PORTL (6 pins), PORTE (1 pin), PORTH (1 pin)

**Documentation:**
- Implementation plan: [../separate_isr_implementation_plan.md](../separate_isr_implementation_plan.md)
- ISR code: hal.cpp, hal.h
- Performance tests: performance.cpp

**Status:** Hardware rewired, software implemented, testing in progress

---

### November 6, 2025 - MREQ Pin Relocation ⚠️ CRITICAL

**Change:** MREQ signal moved from D49 (PL0) to D2 (PE4/INT4)

**Reason:** PORTL does not support interrupts on ATmega2560. External interrupt (INT4) provides:
- 13% faster ISR performance (1.5µs → 1.3µs)
- 50% fewer interrupts (falling edge only)
- Simpler ISR code (no MREQ state checking needed)
- Support for Z80 at 2.5 MHz (was 2 MHz)

**Impact:**
- Other control signals shifted one position (IORQ: D48→D49, RD: D47→D48, etc.)
- Pin D42 (PL7) now unused
- Requires one wire to be moved on hardware

**Documentation:**
- Complete guide: [../Hardware_Pin_Change.md](../Hardware_Pin_Change.md)
- ISR implementation: [../ISR_Proposal.md](../ISR_Proposal.md)
- Performance analysis: [../ISR_Performance_Detailed.md](../ISR_Performance_Detailed.md)
- Full changelog: [../Pin_Mapping_Update_Summary.md](../Pin_Mapping_Update_Summary.md)

**Status:** Software updated, hardware modification required before testing

---

## Testing and Debug Headers

Each Z80 pin on the perfboard has **two header pins**:
1. **Arduino Connection Header**: For wiring to Arduino Mega
2. **Debug Header**: For logic analyzer, oscilloscope, or test probes

This dual-header design allows real-time monitoring of all signals without disconnecting the Arduino.

## Notes

1. **Pin Direction**: Arduino pins connected to Z80 outputs (address bus, control signals) must be configured as INPUT with internal pull-ups disabled.

2. **Bidirectional Pins**: Data bus pins on PORTA require dynamic direction switching based on the current bus cycle. Use DDRA register for fast direction control.

3. **Port Alignment**: This mapping aligns buses to complete ports for maximum performance. All data bus operations are single-instruction port reads/writes.

4. **Decoupling**: A 0.1µF ceramic capacitor should be placed between VCC (pin 11) and GND (pin 29) as close to the Z80 chip as possible.

5. **Signal Integrity**: Keep wire lengths short and use good quality jumper wires. Consider twisted pairs for high-speed signals (CLK, data bus).

6. **Clock Generation**: Hardware Timer1 provides stable, jitter-free clock generation. Output on pin 11 (PB5/OC1A). Do not use software delays or analogWrite().

7. **Bus Timing**: With direct port operations and external interrupt, Arduino can respond to memory requests in ~1.3 µs, supporting Z80 operation up to 2.5 MHz.

8. **Interrupt Handling**: INT and NMI inputs should be pulled HIGH when not in use (configure PORTB outputs appropriately). MREQ uses INT4 external interrupt on D2 for optimal performance.

9. **Testing**: Start with low clock speed (100 kHz) for initial testing, then gradually increase as validation confirms timing margins.

10. **Hardware Modification**: Control signals rewired for separate ISRs. See [../separate_isr_implementation_plan.md](../separate_isr_implementation_plan.md).

---
*Generated from KiCad schematic: Z80 Test Rig.kicad_sch*  
*Last Updated: November 14, 2025 - Separate RD/WR ISRs implemented*
