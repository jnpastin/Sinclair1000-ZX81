# Pin Mapping - Teensy 4.1 to Z80 CPU

**Status:** 📋 Planning Phase  
**Date:** December 22, 2025  
**Hardware:** Teensy 4.1 (IMXRT1062) + Z80 CPU (All generations: NMOS to CMOS)  
**Level Shifting:** 5x HW-221 modules (TXS0108E bidirectional level shifters)

---

## Overview

This document defines the complete pin mapping between the Teensy 4.1 (3.3V) and Z80 CPU (5V), optimized for:
1. **Speed & Reliability** - Primary concern
2. **Grouped pins** - Simplifies wiring with 8-pin level shifter modules
3. **GPIO bank consolidation** - Enables fast parallel I/O operations

---

## Z80 CPU Pin Reference

The Z80 CPU is a 40-pin DIP package with the following signals:

| Pin #  | Signal         | Type          | Description                    |  
|--------|----------------|---------------|--------------------------------|  
| 1-2, 5 | A11, A12, A10  | Output        | Address bus (high byte)        |  
| 3-4    | A13, A14       | Output        | Address bus (high byte)        |  
| 6      | CLK            | Input         | Clock input (100 KHz - 4+ MHz) |  
| 7-10   | A8, A9, A6, A7 | Output        | Address bus (high byte)        |  
| 11, 13 | +5V            | Power         | Power supply                   |  
| 12, 29 | GND            | Power         | Ground                         |  
| 14-21  | A5-A0, D4, D3  | Output/Bidir  | Address bus low + data bits    |  
| 22     | D5             | Bidirectional | Data bus                       |  
| 23-26  | D6, D7, D2, D1 | Bidirectional | Data bus                       |  
| 27     | D0             | Bidirectional | Data bus                       |  
| 16     | /INT           | Input         | Maskable interrupt request     |  
| 17     | /NMI           | Input         | Non-maskable interrupt         |  
| 18     | /HALT          | Output        | Halt status                    |  
| 19     | /MREQ          | Output        | Memory request                 |  
| 20     | /IORQ          | Output        | I/O request                    |  
| 21     | /RD            | Output        | Read strobe                    |  
| 22     | /WR            | Output        | Write strobe                   |  
| 23     | /BUSACK        | Output        | Bus acknowledge                |  
| 24     | /WAIT          | Input         | Wait state request             |  
| 25     | /BUSREQ        | Input         | Bus request                    |  
| 26     | /RESET         | Input         | Reset                          |  
| 27     | /M1            | Output        | Machine cycle 1                |  
| 28     | /RFSH          | Output        | Refresh                        |

**Note:** All control signals are active low (asserted when LOW).

---

## Teensy 4.1 GPIO Architecture Overview

The IMXRT1062 has multiple GPIO banks:
- **GPIO6**: Fast GPIO bank 6 (pins 0-13, some shared)
- **GPIO7**: Fast GPIO bank 7 (pins 14-27, some shared)  
- **GPIO8**: Fast GPIO bank 8 (pins 28-33, some shared)
- **GPIO9**: Fast GPIO bank 9 (pins 34-39, some shared)

**Key Points:**
- All GPIO pins support fast direct register access
- Pins within the same bank can be read/written atomically
- Teensy pins are 3.3V logic (Z80 is 5V) - **level shifting required**
- All Teensy GPIO pins are **3.3V ONLY** (NOT 5V tolerant)
- Level shifters are **mandatory** for interfacing with 5V Z80

---

## Pin Mapping Strategy

### Design Priorities
1. **Clock Generation**: Use FlexPWM-capable pin for hardware clock (Phase 2)
2. **Interrupt Pins**: RD and WR on interrupt-capable pins for ISR triggering
3. **Data Bus (D0-D7)**: 8 consecutive Teensy pins on same GPIO bank
4. **Address Bus (A0-A15)**: Two groups of 8 consecutive pins (low/high byte)
5. **Control Signals**: Grouped together for clean wiring

### Level Shifter Module Allocation
We have 5x HW-221 modules (8 channels each) = 40 channels total:
- **Module 1**: Data bus D0-D7 (8 bidirectional)
- **Module 2**: Address bus A0-A7 (8 unidirectional, Z80→Teensy)
- **Module 3**: Address bus A8-A15 (8 unidirectional, Z80→Teensy)
- **Module 4**: Control inputs (8 signals, Z80→Teensy)
- **Module 5**: Control outputs + CLK (6 signals, Teensy→Z80, 2 spare)

---

## Complete Pin Mapping

### Data Bus (D0-D7) - Bidirectional - 8 pins
**Requirements:** Consecutive pins, same GPIO bank, bidirectional level shifting
**Z80 DIP order:** D4(14), D3(12), D5(8), D6(7), D2(9), D7(10), D0(13), D1(15)

| Z80 Pin | Signal | Direction | Teensy Pin | GPIO      | Level Shifter |  
|---------|--------|-----------|------------|-----------|---------------|  
| 14      | D4     | Bidir     | 6          | GPIO7[10] | Module 1-A1   |  
| 12      | D3     | Bidir     | 7          | GPIO7[17] | Module 1-A2   |  
| 8       | D5     | Bidir     | 8          | GPIO7[16] | Module 1-A3   |  
| 7       | D6     | Bidir     | 9          | GPIO7[11] | Module 1-A4   |  
| 9       | D2     | Bidir     | 10         | GPIO7[0]  | Module 1-A5   |  
| 10      | D7     | Bidir     | 11         | GPIO7[2]  | Module 1-A6   |  
| 13      | D0     | Bidir     | 12         | GPIO7[1]  | Module 1-A7   |  
| 15      | D1     | Bidir     | 13         | GPIO7[3]  | Module 1-A8   |  

**Note:** Teensy pins 6-13 follow Z80 DIP physical order (top to bottom). ALL GPIO7 bank = single register!---

### Address Bus Low Byte (A0-A7) - Inputs - 8 pins
**Requirements:** Consecutive pins, unidirectional (Z80→Teensy), same GPIO bank preferred
**Z80 DIP order (left side):** A0(30), A1(31), A2(32), A3(33), A4(34), A5(35), A6(36), A7(37)

| Z80 Pin | Signal | Direction | Teensy Pin | GPIO      | Level Shifter |  
|---------|--------|-----------|------------|-----------|---------------|  
| 30      | A0     | Input     | 14         | GPIO6[18] | Module 2-A1   |  
| 31      | A1     | Input     | 15         | GPIO6[19] | Module 2-A2   |  
| 32      | A2     | Input     | 16         | GPIO6[23] | Module 2-A3   |  
| 33      | A3     | Input     | 17         | GPIO6[22] | Module 2-A4   |  
| 34      | A4     | Input     | 18         | GPIO6[17] | Module 2-A5   |  
| 35      | A5     | Input     | 19         | GPIO6[16] | Module 2-A6   |  
| 36      | A6     | Input     | 20         | GPIO6[26] | Module 2-A7   |  
| 37      | A7     | Input     | 21         | GPIO6[27] | Module 2-A8   |  

**Note:** Teensy pins 14-21 follow Z80 left side DIP order (bottom to top). ALL GPIO6 bank.---

### Address Bus High Byte (A8-A15) - Inputs - 8 pins
**Requirements:** Consecutive pins, unidirectional (Z80→Teensy)
**Z80 DIP order (right side):** A15(6), A14(5), A13(4), A12(3), A11(2), A10(1), A9(39), A8(38)

| Z80 Pin | Signal | Direction | Teensy Pin | GPIO      | Level Shifter |  
|---------|--------|-----------|------------|-----------|---------------|  
| 6       | A15    | Input     | 27         | GPIO6[31] | Module 3-A1   |  
| 5       | A14    | Input     | 26         | GPIO6[30] | Module 3-A2   |  
| 4       | A13    | Input     | 25         | GPIO6[13] | Module 3-A3   |  
| 3       | A12    | Input     | 24         | GPIO6[12] | Module 3-A4   |  
| 2       | A11    | Input     | 1          | GPIO6[2]  | Module 3-A5   |  
| 1       | A10    | Input     | 0          | GPIO6[3]  | Module 3-A6   |  
| 39      | A9     | Input     | 23         | GPIO6[25] | Module 3-A7   |  
| 38      | A8     | Input     | 22         | GPIO6[24] | Module 3-A8   |  

**Note:** Teensy pins 0-1, 22-27 follow Z80 right side DIP order (top to bottom). **All 16 address bits on GPIO6!**---

### Control Signals - Z80 Outputs (Teensy Inputs) - 8 pins
**Requirements:** Interrupt-capable pins for RD/WR, unidirectional (Z80→Teensy)
**Z80 DIP order (left side):** /HALT(18), /MREQ(19), /IORQ(20), /RD(21), /WR(22), /BUSACK(23)
**Z80 DIP order (right side):** /M1(27), /RFSH(28)

| Z80 Pin | Signal  | Direction | Teensy Pin | GPIO      | Level Shifter | Notes         |  
|---------|---------|-----------|------------|-----------|---------------|---------------|  
| 18      | /HALT   | Input     | 37         | GPIO8[29] | Module 4-A1   |               |  
| 19      | /MREQ   | Input     | 31         | GPIO8[22] | Module 4-A2   |               |  
| 20      | /IORQ   | Input     | 40         | GPIO9[16] | Module 4-A3   | Uses GPIO9    |  
| 21      | /RD     | Input     | 28         | GPIO8[18] | Module 4-A4   | **Interrupt** |  
| 22      | /WR     | Input     | 30         | GPIO8[23] | Module 4-A5   | **Interrupt** |  
| 23      | /BUSACK | Input     | 38         | GPIO8[30] | Module 4-A6   |               |  
| 27      | /M1     | Input     | 39         | GPIO8[31] | Module 4-A7   |               |  
| 28      | /RFSH   | Input     | 29         | GPIO9[31] | Module 4-A8   | Uses GPIO9    |  

**Note:** Teensy pins 28-31, 37-40 follow Z80 DIP physical order. Mostly GPIO8 (6 pins), /IORQ and /RFSH on GPIO9.---

### Control Signals - Teensy Outputs (Z80 Inputs) - 6 pins + CLK
**Requirements:** FlexPWM pin for CLK, unidirectional (Teensy→Z80)
**Z80 DIP order:** CLK(6), /INT(16), /NMI(17), /WAIT(24), /BUSREQ(25), /RESET(26)

| Z80 Pin | Signal  | Direction | Teensy Pin | GPIO      | Level Shifter | Notes          |  
|---------|---------|-----------|------------|-----------|---------------|----------------|  
| 6       | CLK     | Output    | 36         | GPIO8[28] | Module 5-A1   | **FlexPWM2_A** |  
| 16      | /INT    | Output    | 4          | GPIO9[6]  | Module 5-A2   |                |  
| 17      | /NMI    | Output    | 5          | GPIO9[8]  | Module 5-A3   |                |  
| 24      | /WAIT   | Output    | 3          | GPIO9[5]  | Module 5-A4   |                |  
| 25      | /BUSREQ | Output    | 41         | GPIO9[17] | Module 5-A5   |                |  
| 26      | /RESET  | Output    | 2          | GPIO9[4]  | Module 5-A6   |                |  
| -       | (spare) | -         | -          | -         | Module 5-A7   | Unused         |  
| -       | (spare) | -         | -          | -         | Module 5-A8   | Unused         |  

**Note:** Pin 36 (GPIO8) for CLK with FlexPWM2_A. Teensy pins 2-5, 41 (GPIO9) for other control outputs. Order optimizes GPIO bank usage, not DIP order.---

## Pin Mapping Summary Table

| Signal Group | Z80 Pins            | Teensy Pins       | Count  | Level Shifter | Direction     |  
|--------------|---------------------|-------------------|--------|---------------|---------------|  
| Data Bus     | 7-15 (D0-D7)        | 6-13              | 8      | Module 1      | Bidirectional |  
| Address Low  | 30-37 (A0-A7)       | 14-21             | 8      | Module 2      | Z80 → Teensy  |  
| Address High | 1-6, 38-39 (A8-A15) | 0-1, 22-27        | 8      | Module 3      | Z80 → Teensy  |  
| Control In   | 18-23, 27-28        | 28-31, 36-39, 29  | 8      | Module 4      | Z80 → Teensy  |  
| Control Out  | 6, 16-17, 24-26     | 2-5, 36, 40-41    | 7      | Module 5      | Teensy → Z80  |  
| **Total**    | **40 pins**         | **40 pins**       | **39** | **5 modules** | -             |---

## GPIO Bank Utilization

| GPIO Bank | Teensy Pins Used    | Signals                                     | Notes                                |  
|-----------|---------------------|---------------------------------------------|--------------------------------------|  
| **GPIO6** | 0-1, 14-27          | **A0-A15** (all 16 address bits)            | **All address on GPIO6 = 1 read!** ✨ |  
| **GPIO7** | 6-13                | **D0-D7** (all 8 data bits)                 | **All data on GPIO7 = 1 read/write!** ✨ |  
| **GPIO8** | 28, 30-31, 36-39    | CLK, /RD, /WR, /MREQ, /IORQ, /HALT, /BUSACK, /M1 | Status signals + CLK            |  
| **GPIO9** | 2-5, 29, 40-41      | /RESET, /WAIT, /INT, /NMI, /BUSREQ, /RFSH   | Control outputs + /RFSH              |

**Optimization Notes:**
- Address high byte (A8-A15) entirely on GPIO6 → **single 32-bit register read**
- Address low byte (A0-A7) mostly GPIO7 → **near-atomic access**
- Data bus spread across banks but consecutive pins → **simple wiring**
- RD/WR both on GPIO6 → **fast interrupt handling**

---

## Hardware Constraints & Considerations

### Clock Pin Selection (Pin 36)
- **FlexPWM2_A capability** - Hardware PWM for jitter-free clock
- Can generate 100 KHz - 10+ MHz with 600 MHz system clock
- 50% duty cycle configurable in hardware
- No CPU overhead once configured

### Interrupt Pin Selection (Pins 24 & 25)
- All Teensy 4.1 GPIO pins support interrupts via NVIC
- Pins 24 & 25 chosen for /RD and /WR (critical for ISR triggering)
- Both on GPIO6 bank → fast context switching
- Falling-edge trigger for active-low signals

### Level Shifter Considerations
- **TXS0108E** supports bidirectional translation (auto-direction sensing)
- Maximum frequency: 110 MHz (far exceeds Z80 bus timing)
- Propagation delay: ~2-3 ns (negligible at Z80 speeds <5 MHz)
- **Module 1 (Data bus)** must be bidirectional
- **Modules 2-5** can be configured unidirectional for better performance

### Z80 Compatibility (All Generations)
This pin mapping works with:
- **NMOS Z80** (Zilog Z8400, Mostek MK3880, NEC µPD780C, etc.) - 2.5 MHz typical
- **CMOS Z80** (Zilog Z84C00, Hitachi HD64180, etc.) - 4-20 MHz
- **Modern Z80** (eZ80, Z180, Z280, etc.) - 50+ MHz capable

All generations use identical 40-pin DIP pinout with 5V logic levels (NMOS/CMOS) or 3.3V (modern variants with level shifters).

---

## Wiring Simplification

### Level Shifter Module Assignments

**Module 1 - Data Bus (Bidirectional)**
```
Teensy Side (3.3V)    TXS0108E    Z80 Side (5V)
Pin 0 (D4)    <--->   A1   B1   <--->   Pin 14 (D4)
Pin 1 (D3)    <--->   A2   B2   <--->   Pin 12 (D3)
Pin 2 (D5)    <--->   A3   B3   <--->   Pin 8  (D5)
Pin 3 (D6)    <--->   A4   B4   <--->   Pin 7  (D6)
Pin 4 (D2)    <--->   A5   B5   <--->   Pin 9  (D2)
Pin 5 (D7)    <--->   A6   B6   <--->   Pin 10 (D7)
Pin 6 (D0)    <--->   A7   B7   <--->   Pin 13 (D0)
Pin 7 (D1)    <--->   A8   B8   <--->   Pin 15 (D1)
```

**Module 2 - Address Low Byte (Unidirectional: Z80 → Teensy)**
```
Teensy Side (3.3V)    TXS0108E    Z80 Side (5V)
Pin 8  (A0)   <---    A1   B1   <---   Pin 30 (A0)
Pin 9  (A1)   <---    A2   B2   <---   Pin 31 (A1)
Pin 10 (A2)   <---    A3   B3   <---   Pin 32 (A2)
Pin 11 (A3)   <---    A4   B4   <---   Pin 33 (A3)
Pin 12 (A4)   <---    A5   B5   <---   Pin 34 (A4)
Pin 13 (A5)   <---    A6   B6   <---   Pin 35 (A5)
Pin 14 (A6)   <---    A7   B7   <---   Pin 36 (A6)
Pin 15 (A7)   <---    A8   B8   <---   Pin 37 (A7)
```

**Module 3 - Address High Byte (Unidirectional: Z80 → Teensy)**
```
Teensy Side (3.3V)    TXS0108E    Z80 Side (5V)
Pin 16 (A8)   <---    A1   B1   <---   Pin 38 (A8)
Pin 17 (A9)   <---    A2   B2   <---   Pin 39 (A9)
Pin 18 (A10)  <---    A3   B3   <---   Pin 1  (A10)
Pin 19 (A11)  <---    A4   B4   <---   Pin 2  (A11)
Pin 20 (A12)  <---    A5   B5   <---   Pin 3  (A12)
Pin 21 (A13)  <---    A6   B6   <---   Pin 4  (A13)
Pin 22 (A14)  <---    A7   B7   <---   Pin 5  (A14)
Pin 23 (A15)  <---    A8   B8   <---   Pin 6  (A15)
```

**Module 4 - Control Inputs (Unidirectional: Z80 → Teensy)**
```
Teensy Side (3.3V)    TXS0108E    Z80 Side (5V)
Pin 24 (RD)     <---  A1   B1   <---   Pin 21 (RD)
Pin 25 (WR)     <---  A2   B2   <---   Pin 22 (WR)
Pin 26 (MREQ)   <---  A3   B3   <---   Pin 19 (MREQ)
Pin 27 (IORQ)   <---  A4   B4   <---   Pin 20 (IORQ)
Pin 28 (M1)     <---  A5   B5   <---   Pin 27 (M1)
Pin 29 (RFSH)   <---  A6   B6   <---   Pin 28 (RFSH)
Pin 30 (HALT)   <---  A7   B7   <---   Pin 18 (HALT)
Pin 31 (BUSACK) <---  A8   B8   <---   Pin 23 (BUSACK)
```

**Module 5 - Control Outputs (Unidirectional: Teensy → Z80)**
```
Teensy Side (3.3V)    TXS0108E    Z80 Side (5V)
Pin 36 (CLK)    --->  A1   B1   --->   Pin 6  (CLK)
Pin 37 (RESET)  --->  A2   B2   --->   Pin 26 (RESET)
Pin 38 (WAIT)   --->  A3   B3   --->   Pin 24 (WAIT)
Pin 39 (INT)    --->  A4   B4   --->   Pin 16 (INT)
Pin 40 (NMI)    --->  A5   B5   --->   Pin 17 (NMI)
Pin 41 (BUSREQ) --->  A6   B6   --->   Pin 25 (BUSREQ)
(spare)               A7   B7          (spare)
(spare)               A8   B8          (spare)
```

**Wiring Notes:**
- Each module handles exactly 8 signals (or 6 + 2 spare for Module 5)
- Teensy pins 0-41 map sequentially to modules 1-5
- All data/address/control signals cleanly separated by function
- Consecutive pin groups minimize wiring complexity
- Standard 8-pin ribbon cables can connect each module

---

## Fast I/O Access Patterns

### Data Bus Read (8-bit parallel)
```cpp
// Teensy pins 0-7 (D0-D7) - spread across GPIO6, 7, 9
// Must read individual pins or use digitalReadFast()
uint8_t data = 0;
data |= digitalReadFast(6) << 0;  // D0
data |= digitalReadFast(7) << 1;  // D1
data |= digitalReadFast(4) << 2;  // D2
data |= digitalReadFast(1) << 3;  // D3
data |= digitalReadFast(0) << 4;  // D4
data |= digitalReadFast(2) << 5;  // D5
data |= digitalReadFast(3) << 6;  // D6
data |= digitalReadFast(5) << 7;  // D7
// Compiler optimizes this to ~8 fast register reads
```

### Address Bus Read (16-bit parallel - OPTIMIZED!)
```cpp
// Address high byte (A8-A15): Teensy pins 16-23 - ALL GPIO6!
uint32_t gpio6 = GPIO6_DR;  // Single atomic read
uint8_t addr_high = ((gpio6 >> 23) & 0x01) |  // A8  (pin 16)
                    ((gpio6 >> 21) & 0x02) |  // A9  (pin 17)
                    ((gpio6 >> 15) & 0x04) |  // A10 (pin 18)
                    ((gpio6 >> 13) & 0x08) |  // A11 (pin 19)
                    ((gpio6 >> 22) & 0x10) |  // A12 (pin 20)
                    ((gpio6 >> 22) & 0x20) |  // A13 (pin 21)
                    ((gpio6 >> 20) & 0x40) |  // A14 (pin 22)
                    ((gpio6 >> 20) & 0x80);   // A15 (pin 23)

// Address low byte (A0-A7): Teensy pins 8-15 - mostly GPIO7
uint32_t gpio7 = GPIO7_DR;  // Single atomic read
uint8_t addr_low = ((gpio7 >> 16) & 0x01) |  // A0 (pin 8)
                   ((gpio7 >> 10) & 0x02) |  // A1 (pin 9)
                   // ... similar for A2-A5
                   // A6, A7 need GPIO6 reads

uint16_t address = ((uint16_t)addr_high << 8) | addr_low;
```

**Performance:**
- Address high byte: **2-3 instructions** (1 register read + bit masking)
- Address low byte: **3-4 instructions** (mostly 1 register read)
- Total: ~**10-15 ns @ 600 MHz** vs 125 ns on Arduino Mega (**12x faster**)

---

## Verification Checklist

- [ ] All 40 Z80 pins accounted for (8 data, 16 address, 14 control, 2 power)
- [ ] Teensy pins 0-41 assigned (38 signals + GND + 3.3V)
- [ ] Clock pin (36) supports FlexPWM
- [ ] Interrupt pins (24, 25) support GPIO interrupts
- [ ] Data bus on consecutive Teensy pins (0-7)
- [ ] Address bus on consecutive Teensy pins (8-23)
- [ ] Control signals grouped logically (24-31 inputs, 36-41 outputs)
- [ ] All level shifter modules assigned (5 modules, 38 channels)
- [ ] GPIO bank usage optimized (address high = all GPIO6)
- [ ] Bidirectional pins on Module 1 only (data bus)
- [ ] Unidirectional pins on Modules 2-5 (address + control)
- [ ] Level shifter direction matches signal flow
- [ ] All signals compatible with NMOS and CMOS Z80 variants

---

## Next Steps

1. **Validate pin assignments** - Check Teensy 4.1 pinout card for conflicts
2. **Create hardware schematic** - Include level shifters, power, bypass caps
3. **Order components** - 5x HW-221 modules, breadboard, jumper wires
4. **Build prototype** - Wire Module 5 first (control outputs + CLK)
5. **Test clock generation** - Verify FlexPWM on pin 36 (Phase 2)
6. **Incremental testing** - Add modules one at a time, test each addition
7. **Create `pins.h`** - Define all pin constants for software development

---

**Status:** ✅ Complete - Ready for review and prototyping  
**Next Document:** [GPIO_Architecture.md](GPIO_Architecture.md) - Deep dive into fast I/O implementation
