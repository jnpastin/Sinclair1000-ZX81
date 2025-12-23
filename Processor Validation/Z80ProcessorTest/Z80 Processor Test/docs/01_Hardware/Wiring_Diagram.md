# Wiring Diagram - Teensy 4.1 to Z80 via Level Shifters

**Date:** December 22, 2025  
**Hardware:** Teensy 4.1 + 5x HW-221 (TXS0108E) + Z80 CPU (40-pin DIP)

---

## Overview Diagram

```
        ┌───────────────┐
        │  Teensy 4.1   │
        │   (3.3V)      │
        └───┬───────┬───┘
            │       │
    ┌───────┘       └───────┐
    │                       │
    ▼                       ▼
┌────────┐             ┌────────┐
│ Module │             │ Module │
│   1    │  (bidir)    │   5    │  (output)
│ Data   │             │ Control│
└────┬───┘             └────┬───┘
     │                      │
     │   ┌────────────┐     │
     │   │  Module 2  │     │
     │   │  Address   │     │
     │   │  Low Byte  │     │
     │   └──────┬─────┘     │
     │          │           │
     │   ┌──────▼─────┐     │
     │   │  Module 3  │     │
     │   │  Address   │     │
     │   │ High Byte  │     │
     │   └──────┬─────┘     │
     │          │           │
     │   ┌──────▼─────┐     │
     │   │  Module 4  │     │
     │   │  Control   │     │
     │   │   Input    │     │
     │   └──────┬─────┘     │
     │          │           │
     └──────────┴───────────┘
                │
                ▼
        ┌───────────────┐
        │   Z80 CPU     │
        │    (5V)       │
        └───────────────┘
```

**Physical Implementation:**
- **Teensy + Z80:** Proto board (wired like breadboard)
- **Level Shifters:** Separate proto board (5x HW-221 modules)
- **Interconnects:** 15-conductor shielded cable with JST XH connectors
- **Power:** Single USB cable (VUSB + 3.3V rails)

---

## Debug Headers

### Logic Analyzer Interface (3.3V Side) ✅

**Placement:** Between Teensy GPIO pins and level shifter inputs (A-side)

```
Teeensy GPIO → Dual-Row Headers → Level Shifter A-side
                     ↓
              Logic Analyzer Probes
```

**Header Layout:**
- **Data Bus:** 10-pin header (D0-D7 + 2x GND)
- **Address Low:** 10-pin header (A0-A7 + 2x GND)
- **Address High:** 10-pin header (A8-A15 + 2x GND)
- **Control Signals:** 12-pin header (8 control + 4x GND)
- **Clock:** 2-pin header (CLK + GND)

**Total:** ~44 pins accessible for probing

**Ground Distribution:** Multiple ground pins (1 per 3-4 signals) for stable probe ground reference

**Why 3.3V Side:**
- ✅ Safe for all modern logic analyzers (3.3V or 5V-tolerant)
- ✅ Measures what Teensy actually sees (after level shifting)
- ✅ No loading on Z80 side (probes don't affect Z80 signal quality)
- ✅ Access to Teensy-generated signals (clock, control outputs)
- ✅ Faster measurements (no level shifter delay in probe path)

**Optional:** Can probe 5V side with clip leads for Z80 signal verification

```

---

## Module 1: Data Bus (Bidirectional)

```
Teensy 4.1 (3.3V)              HW-221 Module 1           Z80 CPU (5V)
                              ┌─────────────┐
Pin 0 (GPIO6[3])  ────────────┤A1        B1 ├──────────  Pin 14 (D4)
Pin 1 (GPIO6[2])  ────────────┤A2        B2 ├──────────  Pin 12 (D3)
Pin 2 (GPIO9[4])  ────────────┤A3        B3 ├──────────  Pin 8  (D5)
Pin 3 (GPIO9[5])  ────────────┤A4        B4 ├──────────  Pin 7  (D6)
Pin 4 (GPIO9[6])  ────────────┤A5        B5 ├──────────  Pin 9  (D2)
Pin 5 (GPIO9[8])  ────────────┤A6        B6 ├──────────  Pin 10 (D7)
Pin 6 (GPIO7[10]) ────────────┤A7        B7 ├──────────  Pin 13 (D0)
Pin 7 (GPIO7[17]) ────────────┤A8        B8 ├──────────  Pin 15 (D1)
                              │             │
3.3V ─────────────────────────┤VCCA     OE  ├──── 3.3V (enable)
GND ──────────────────────────┤GND    VCCB  ├──── 5V
                              └─────────────┘

Direction: Bidirectional (auto-sensing)
Purpose: Z80 data bus D0-D7
```

---

## Module 2: Address Low Byte (Z80 → Teensy)

```
Teensy 4.1 (3.3V)              HW-221 Module 2           Z80 CPU (5V)
                              ┌─────────────┐
Pin 8  (GPIO7[16]) ───────────┤A1        B1 ├──────────  Pin 30 (A0)
Pin 9  (GPIO7[11]) ───────────┤A2        B2 ├──────────  Pin 31 (A1)
Pin 10 (GPIO7[0])  ───────────┤A3        B3 ├──────────  Pin 32 (A2)
Pin 11 (GPIO7[2])  ───────────┤A4        B4 ├──────────  Pin 33 (A3)
Pin 12 (GPIO7[1])  ───────────┤A5        B5 ├──────────  Pin 34 (A4)
Pin 13 (GPIO7[3])  ───────────┤A6        B6 ├──────────  Pin 35 (A5)
Pin 14 (GPIO6[18]) ───────────┤A7        B7 ├──────────  Pin 36 (A6)
Pin 15 (GPIO6[19]) ───────────┤A8        B8 ├──────────  Pin 37 (A7)
                              │             │
3.3V ─────────────────────────┤VCCA     OE  ├──── 3.3V
GND ──────────────────────────┤GND    VCCB  ├──── 5V
                              └─────────────┘

Direction: Z80 → Teensy (unidirectional)
Purpose: Address bus low byte A0-A7
```

---

## Module 3: Address High Byte (Z80 → Teensy)

```
Teensy 4.1 (3.3V)              HW-221 Module 3           Z80 CPU (5V)
                              ┌─────────────┐
Pin 16 (GPIO6[23]) ───────────┤A1        B1 ├──────────  Pin 38 (A8)
Pin 17 (GPIO6[22]) ───────────┤A2        B2 ├──────────  Pin 39 (A9)
Pin 18 (GPIO6[17]) ───────────┤A3        B3 ├──────────  Pin 1  (A10)
Pin 19 (GPIO6[16]) ───────────┤A4        B4 ├──────────  Pin 2  (A11)
Pin 20 (GPIO6[26]) ───────────┤A5        B5 ├──────────  Pin 3  (A12)
Pin 21 (GPIO6[27]) ───────────┤A6        B6 ├──────────  Pin 4  (A13)
Pin 22 (GPIO6[24]) ───────────┤A7        B7 ├──────────  Pin 5  (A14)
Pin 23 (GPIO6[25]) ───────────┤A8        B8 ├──────────  Pin 6  (A15)
                              │             │
3.3V ─────────────────────────┤VCCA     OE  ├──── 3.3V
GND ──────────────────────────┤GND    VCCB  ├──── 5V
                              └─────────────┘

Direction: Z80 → Teensy (unidirectional)
Purpose: Address bus high byte A8-A15
Note: ⭐ ALL pins on GPIO6 bank = single register read!
```

---

## Module 4: Control Inputs (Z80 → Teensy)

```
Teensy 4.1 (3.3V)              HW-221 Module 4           Z80 CPU (5V)
                              ┌─────────────┐
Pin 24 (GPIO6[12]) ───────────┤A1        B1 ├──────────  Pin 21 (/RD)     ⚡ INT
Pin 25 (GPIO6[13]) ───────────┤A2        B2 ├──────────  Pin 22 (/WR)     ⚡ INT
Pin 26 (GPIO6[30]) ───────────┤A3        B3 ├──────────  Pin 19 (/MREQ)
Pin 27 (GPIO6[31]) ───────────┤A4        B4 ├──────────  Pin 20 (/IORQ)
Pin 28 (GPIO8[18]) ───────────┤A5        B5 ├──────────  Pin 27 (/M1)
Pin 29 (GPIO9[31]) ───────────┤A6        B6 ├──────────  Pin 28 (/RFSH)
Pin 30 (GPIO8[23]) ───────────┤A7        B7 ├──────────  Pin 18 (/HALT)
Pin 31 (GPIO8[22]) ───────────┤A8        B8 ├──────────  Pin 23 (/BUSACK)
                              │             │
3.3V ─────────────────────────┤VCCA     OE  ├──── 3.3V
GND ──────────────────────────┤GND    VCCB  ├──── 5V
                              └─────────────┘

Direction: Z80 → Teensy (unidirectional)
Purpose: Z80 control signal outputs (status signals)
Note: Pins 24 & 25 configured as interrupt sources (falling edge)
```

---

## Module 5: Control Outputs (Teensy → Z80)

```
Teensy 4.1 (3.3V)              HW-221 Module 5           Z80 CPU (5V)
                              ┌─────────────┐
Pin 36 (GPIO8[28]) ───────────┤A1        B1 ├──────────  Pin 6  (CLK)     ⏰ PWM
Pin 37 (GPIO8[29]) ───────────┤A2        B2 ├──────────  Pin 26 (/RESET)
Pin 38 (GPIO8[30]) ───────────┤A3        B3 ├──────────  Pin 24 (/WAIT)
Pin 39 (GPIO8[31]) ───────────┤A4        B4 ├──────────  Pin 16 (/INT)
Pin 40 (GPIO9[16]) ───────────┤A5        B5 ├──────────  Pin 17 (/NMI)
Pin 41 (GPIO9[17]) ───────────┤A6        B6 ├──────────  Pin 25 (/BUSREQ)
(spare)                       │A7        B7 │            (unused)
(spare)                       │A8        B8 │            (unused)
                              │             │
3.3V ─────────────────────────┤VCCA     OE  ├──── 3.3V
GND ──────────────────────────┤GND    VCCB  ├──── 5V
                              └─────────────┘

Direction: Teensy → Z80 (unidirectional)
Purpose: Teensy control signal outputs (Z80 inputs)
Note: Pin 36 driven by FlexPWM2_A (hardware PWM clock)
```

---

## Power Distribution

```
┌──────────────────────────────────────────────────────────┐
│                     Power Supply                         │
│                                                          │
│   USB 5V or External 5V Supply (1A minimum)             │
└────┬──────────────────────────────────┬──────────────────┘
     │                                  │
     │  ┌───────────────┐               │
     └──┤ Teensy VIN    │               │
        │               │               │
        │  3.3V Reg ────┼───┐           │
        └───────────────┘   │           │
                            │           │
                            ▼           ▼
                        ┌────────┐  ┌────────┐
                        │ 3.3V   │  │  5V    │
                        └───┬────┘  └───┬────┘
                            │           │
            ┌───────────────┼───────────┼───────────────┐
            │               │           │               │
            ▼               ▼           ▼               ▼
    ┌─────────────┐   ┌─────────┐   ┌─────────┐   ┌─────────┐
    │  Teensy     │   │ Level   │   │ Level   │   │   Z80   │
    │  Internal   │   │ Shifter │   │ Shifter │   │   CPU   │
    │    3.3V     │   │  VCCA   │   │  VCCB   │   │   5V    │
    └─────────────┘   └─────────┘   └─────────┘   └─────────┘
                           ↑               ↑
                           │               │
                      (5 modules)     (5 modules)
                        3.3V side       5V side
```

**Critical:**
- All grounds connected together (common ground)
- Bypass capacitors on ALL ICs:
  - Teensy: 100nF + 10µF on 3.3V pin
  - Z80: 100nF + 10µF on pin 11 (+5V)
  - Each TXS0108E: 100nF on VCCA and VCCB

---

## Common Ground Network

```
Teensy GND ─┬─ Z80 GND (pins 12, 29)
            ├─ Module 1 GND
            ├─ Module 2 GND
            ├─ Module 3 GND
            ├─ Module 4 GND
            ├─ Module 5 GND
            └─ Power Supply GND

⚠️ CRITICAL: Single-point star ground to avoid ground loops
```

---

## Physical Layout Recommendation

```
┌────────────────────────────────────────┐
│          Breadboard 1 (Top)            │
│                                        │
│  ┌──────────┐  ┌──────────┐          │
│  │ Module 3 │  │ Module 4 │          │
│  │ Addr Hi  │  │ Control  │          │
│  └────┬─────┘  └────┬─────┘          │
│       │             │                 │
│  ┌────▼─────┐  ┌────▼─────┐          │
│  │ Module 2 │  │ Module 5 │          │
│  │ Addr Lo  │  │ Control  │          │
│  └────┬─────┘  └────┬─────┘          │
│       │             │                 │
│  ┌────▼─────────────▼──────┐         │
│  │     Teensy 4.1           │         │
│  │     (mounted on         │         │
│  │      female headers)     │         │
│  └──────────────────────────┘         │
└────────────────────────────────────────┘

┌────────────────────────────────────────┐
│         Breadboard 2 (Bottom)          │
│                                        │
│  ┌──────────────────────┐             │
│  │   Module 1 (Data)    │             │
│  └──────────┬───────────┘             │
│             │                          │
│  ┌──────────▼───────────┐             │
│  │       Z80 CPU        │             │
│  │     (40-pin DIP)     │             │
│  │                      │             │
│  └──────────────────────┘             │
│                                        │
└────────────────────────────────────────┘

Wiring: Jumper wires between breadboards
Modules: HW-221 mounted in breadboard IC positions
```

---

## Assembly Order (Incremental Testing)

### Step 1: Power + Module 5 (Clock)
```
Teensy ──[Module 5]──► Z80 CLK (pin 6)
                     ► Z80 RESET (pin 26)
```
**Test:** Measure clock signal on Z80 pin 6 with oscilloscope

### Step 2: Add Modules 2 & 3 (Address)
```
Teensy ◄──[Module 2]─── Z80 A0-A7
       ◄──[Module 3]─── Z80 A8-A15
```
**Test:** Drive address pins manually, verify Teensy reads correct values

### Step 3: Add Module 4 (Control Inputs)
```
Teensy ◄──[Module 4]─── Z80 /RD, /WR, /MREQ, /IORQ, etc.
```
**Test:** Manually toggle control pins, verify interrupt triggering

### Step 4: Add Module 1 (Data Bus)
```
Teensy ◄──►[Module 1]◄──► Z80 D0-D7
```
**Test:** Bidirectional data transfer in both directions

### Step 5: Full System
```
All modules connected, Z80 powered and clocked
```
**Test:** Z80 executes NOP loop, Teensy ISRs respond to memory requests

---

## Testing Points

| Test Point | Signal              | Expected           | Measure With |  
|------------|---------------------|--------------------|--------------|  
| **TP1**    | Teensy 3.3V         | 3.3V ±0.1V        | Multimeter   |  
| **TP2**    | Z80 +5V (pin 11)    | 5.0V ±0.1V        | Multimeter   |  
| **TP3**    | Module VCCA (all)   | 3.3V               | Multimeter   |  
| **TP4**    | Module VCCB (all)   | 5.0V               | Multimeter   |  
| **TP5**    | Z80 CLK (pin 6)     | 1 MHz square wave  | Oscilloscope |  
| **TP6**    | Teensy pin 36       | 1 MHz square wave  | Oscilloscope |  
| **TP7**    | Z80 /RESET (pin 26) | 5V (inactive)      | Multimeter   |  
| **TP8**    | Common ground       | 0V                 | Multimeter   |---

## Cable Management Tips

1. **Color Coding:**
   - Red: Power (+5V, +3.3V)
   - Black: Ground
   - Yellow: Data bus
   - Blue: Address bus
   - Green: Control signals
   - White/Orange: Clock

2. **Grouping:**
   - Use 8-wire ribbon cables for each module
   - Keep data/address/control groups separate
   - Route power lines separately from signals

3. **Length:**
   - Keep all signal wires <15 cm (6 inches)
   - Shorter is better for high-speed signals
   - Power wires can be longer

---

## Troubleshooting Guide

| Problem               | Check                   | Solution                          |  
|-----------------------|-------------------------|-----------------------------------|  
| No clock output       | Pin 36, Z80 pin 6       | Verify FlexPWM configuration      |  
| Wrong voltage levels  | VCCA, VCCB on modules   | Check power connections           |  
| Erratic readings      | Ground connections      | Verify common ground              |  
| No ISR triggering     | Pins 24, 25             | Check level shifter Module 4      |  
| Data corruption       | Module 1 wiring         | Verify bidirectional connections  |  
| Z80 not running       | /RESET (pin 26)         | Must be HIGH (5V) to run          |

---

**Status:** ✅ Complete wiring reference  
**Next:** Print this document and use during hardware assembly!

---

**Related Documents:**
- [Pin_Mapping.md](Pin_Mapping.md) - Detailed pin assignments
- [Quick_Reference.md](Quick_Reference.md) - One-page summary
- [Phase1_Summary.md](Phase1_Summary.md) - Complete hardware plan
