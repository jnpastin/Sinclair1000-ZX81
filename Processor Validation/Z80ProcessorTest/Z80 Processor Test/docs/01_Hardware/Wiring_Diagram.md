# Wiring Diagram - Teensy 4.1 to Z80 via Level Shifters

**Date:** December 23, 2025  
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
- **Interconnects:** Dupont jumper wires or ribbon cables with pin headers
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

---

## Module 1: Data Bus (Bidirectional)

```
Teensy 4.1 (3.3V)              HW-221 Module 1           Z80 CPU (5V)
                              ┌─────────────┐
Pin 10 (GPIO2[0])  ───────────┤A1        B1 ├──────────  Pin 14 (D0)
Pin 12 (GPIO2[1])  ───────────┤A2        B2 ├──────────  Pin 15 (D1)
Pin 11 (GPIO2[2])  ───────────┤A3        B3 ├──────────  Pin 12 (D2)
Pin 13 (GPIO2[3])  ───────────┤A4        B4 ├──────────  Pin 8  (D3)
Pin 8  (GPIO2[16]) ───────────┤A5        B5 ├──────────  Pin 7  (D4)
Pin 7  (GPIO2[17]) ───────────┤A6        B6 ├──────────  Pin 9  (D5)
Pin 36 (GPIO2[18]) ───────────┤A7        B7 ├──────────  Pin 10 (D6)
Pin 37 (GPIO2[19]) ───────────┤A8        B8 ├──────────  Pin 13 (D7)
                              │             │
3.3V ─────────────────────────┤VCCA     OE  ├──── Pin 31 (enable)
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
Pin 19 (GPIO1[16]) ───────────┤A1        B1 ├──────────  Pin 30 (A0)
Pin 18 (GPIO1[17]) ───────────┤A2        B2 ├──────────  Pin 31 (A1)
Pin 14 (GPIO1[18]) ───────────┤A3        B3 ├──────────  Pin 32 (A2)
Pin 15 (GPIO1[19]) ───────────┤A4        B4 ├──────────  Pin 33 (A3)
Pin 40 (GPIO1[20]) ───────────┤A5        B5 ├──────────  Pin 34 (A4)
Pin 41 (GPIO1[21]) ───────────┤A6        B6 ├──────────  Pin 35 (A5)
Pin 17 (GPIO1[22]) ───────────┤A7        B7 ├──────────  Pin 36 (A6)
Pin 16 (GPIO1[23]) ───────────┤A8        B8 ├──────────  Pin 37 (A7)
                              │             │
3.3V ─────────────────────────┤VCCA     OE  ├──── Pin 31
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
Pin 22 (GPIO1[24]) ───────────┤A1        B1 ├──────────  Pin 1  (A8)
Pin 23 (GPIO1[25]) ───────────┤A2        B2 ├──────────  Pin 2  (A9)
Pin 20 (GPIO1[26]) ───────────┤A3        B3 ├──────────  Pin 3  (A10)
Pin 21 (GPIO1[27]) ───────────┤A4        B4 ├──────────  Pin 4  (A11)
Pin 38 (GPIO1[28]) ───────────┤A5        B5 ├──────────  Pin 5  (A12)
Pin 39 (GPIO1[29]) ───────────┤A6        B6 ├──────────  Pin 6  (A13)
Pin 26 (GPIO1[30]) ───────────┤A7        B7 ├──────────  Pin 38 (A14)
Pin 27 (GPIO1[31]) ───────────┤A8        B8 ├──────────  Pin 39 (A15)
                              │             │
3.3V ─────────────────────────┤VCCA     OE  ├──── Pin 31
GND ──────────────────────────┤GND    VCCB  ├──── 5V
                              └─────────────┘

Direction: Z80 → Teensy (unidirectional)
Purpose: Address bus high byte A8-A15
Note: ⭐ ALL pins on GPIO1 bank = single register read!
```

---

## Module 4: Control Inputs (Z80 → Teensy)

```
Teensy 4.1 (3.3V)              HW-221 Module 4           Z80 CPU (5V)
                              ┌─────────────┐
Pin 1  (GPIO1[2])  ───────────┤A1        B1 ├──────────  Pin 18 (/HALT)
Pin 0  (GPIO1[3])  ───────────┤A2        B2 ├──────────  Pin 20 (/IORQ)
Pin 24 (GPIO1[12]) ───────────┤A3        B3 ├──────────  Pin 23 (/BUSACK)
Pin 2  (GPIO4[4])  ───────────┤A4        B4 ├──────────  Pin 27 (/M1)
Pin 3  (GPIO4[5])  ───────────┤A5        B5 ├──────────  Pin 28 (/RFSH)
Pin 4  (GPIO4[6])  ───────────┤A6        B6 ├──────────  Pin 21 (/RD)     ⚡ INT
Pin 33 (GPIO4[7])  ───────────┤A7        B7 ├──────────  Pin 22 (/WR)     ⚡ INT
Pin 5  (GPIO4[8])  ───────────┤A8        B8 ├──────────  Pin 19 (/MREQ)
                              │             │
3.3V ─────────────────────────┤VCCA     OE  ├──── Pin 31
GND ──────────────────────────┤GND    VCCB  ├──── 5V
                              └─────────────┘

Direction: Z80 → Teensy (unidirectional)
Purpose: Z80 control signal outputs (status signals)
Note: Pins 4 & 33 configured as interrupt sources (falling edge)
```

---

## Module 5: Control Outputs (Teensy → Z80)

```
Teensy 4.1 (3.3V)              HW-221 Module 5           Z80 CPU (5V)
                              ┌─────────────┐
Pin 28 (GPIO3[18]) ───────────┤A1        B1 ├──────────  Pin 6  (CLK)     ⏰ PWM
Pin 6  (GPIO2[10]) ───────────┤A2        B2 ├──────────  Pin 16 (/INT)
Pin 9  (GPIO2[11]) ───────────┤A3        B3 ├──────────  Pin 17 (/NMI)
Pin 35 (GPIO2[28]) ───────────┤A4        B4 ├──────────  Pin 24 (/WAIT)
Pin 32 (GPIO2[12]) ───────────┤A5        B5 ├──────────  Pin 25 (/BUSREQ)
Pin 34 (GPIO2[29]) ───────────┤A6        B6 ├──────────  Pin 26 (/RESET)
(spare)                       │A7        B7 │            (unused)
(spare)                       │A8        B8 │            (unused)
                              │             │
3.3V ─────────────────────────┤VCCA     OE  ├──── Pin 31
GND ──────────────────────────┤GND    VCCB  ├──── 5V
                              └─────────────┘

Direction: Teensy → Z80 (unidirectional)
Purpose: Teensy control signal outputs (Z80 inputs)
Note: Pin 28 driven by FlexPWM2_A (hardware PWM clock)
```

---

## Cable Pinout Details

### Cable Overview

**10 Shielded Cables Total:**
- Cables 1-5: Z80 ↔ Level Shifter (5V side)
- Cables 6-10: Teensy ↔ Level Shifter (3.3V side)

**Each cable: 15-conductor shielded**
- 8 signal wires
- 4 ground wires
- 3 spare wires (NC)
- Shield (ground at level shifter only)

---

### Cable 1 & 6: Data Bus (Module 1)

**Cable 1: Z80 → Level Shifter (5V side)**

| Wire Color | Signal | Notes |
|------------|--------|-------|
| White/Green | 5V_D0 | Z80 Pin 14 |
| White/Red | 5V_D1 | Z80 Pin 15 |
| Red | 5V_D2 | Z80 Pin 12 |
| Black/Red | 5V_D3 | Z80 Pin 8 |
| Black/Orange | 5V_D4 | Z80 Pin 7 |
| Green | 5V_D5 | Z80 Pin 9 |
| Orange | 5V_D6 | Z80 Pin 10 |
| White/Blue | 5V_D7 | Z80 Pin 13 |
| Black | GND | Ground return |
| Black/Blue | GND | Ground return |
| Black/Green | GND | Ground return |
| Blue | GND | Ground return |
| Black/White | NC | Spare |
| White | NC | Spare |
| White/Black | NC | Spare |

**Cable 6: Teensy → Level Shifter (3.3V side)**

| Wire Color | Signal | Teensy Pin |
|------------|--------|------------|
| White/Green | 3.3V_D0 | Pin 10 |
| White/Red | 3.3V_D1 | Pin 12 |
| Red | 3.3V_D2 | Pin 11 |
| Black/Red | 3.3V_D3 | Pin 13 |
| Black/Orange | 3.3V_D4 | Pin 8 |
| Green | 3.3V_D5 | Pin 7 |
| Orange | 3.3V_D6 | Pin 36 |
| White/Blue | 3.3V_D7 | Pin 37 |
| Black | GND | Ground return |
| Black/Blue | GND | Ground return |
| Black/Green | GND | Ground return |
| Blue | GND | Ground return |
| Black/White | NC | Spare |
| White | NC | Spare |
| White/Black | NC | Spare |

---

### Cable 2 & 7: Address Low Byte (Module 2)

**Cable 2: Z80 → Level Shifter (5V side)**

| Wire Color | Signal | Z80 Pin |
|------------|--------|---------|
| Black/Orange | 5V_A0 | Pin 30 |
| Black/Red | 5V_A1 | Pin 31 |
| Green | 5V_A2 | Pin 32 |
| Orange | 5V_A3 | Pin 33 |
| Red | 5V_A4 | Pin 34 |
| White/Blue | 5V_A5 | Pin 35 |
| White/Green | 5V_A6 | Pin 36 |
| White/Red | 5V_A7 | Pin 37 |
| Black | GND | Ground return |
| Black/Blue | GND | Ground return |
| Black/Green | GND | Ground return |
| Blue | GND | Ground return |
| Black/White | NC | Spare |
| White | NC | Spare |
| White/Black | NC | Spare |

**Cable 7: Teensy → Level Shifter (3.3V side)**

| Wire Color | Signal | Teensy Pin |
|------------|--------|------------|
| Black/Orange | 3.3V_A0 | Pin 19 |
| Black/Red | 3.3V_A1 | Pin 18 |
| Green | 3.3V_A2 | Pin 14 |
| Orange | 3.3V_A3 | Pin 15 |
| Red | 3.3V_A4 | Pin 40 |
| White/Blue | 3.3V_A5 | Pin 41 |
| White/Green | 3.3V_A6 | Pin 17 |
| White/Red | 3.3V_A7 | Pin 16 |
| Black | GND | Ground return |
| Black/Blue | GND | Ground return |
| Black/Green | GND | Ground return |
| Blue | GND | Ground return |
| Black/White | NC | Spare |
| White | NC | Spare |
| White/Black | NC | Spare |

---

### Cable 3 & 8: Address High Byte (Module 3)

**Cable 3: Z80 → Level Shifter (5V side)**

| Wire Color | Signal | Z80 Pin |
|------------|--------|---------|
| Black/Orange | 5V_A8 | Pin 1 |
| Black/Red | 5V_A9 | Pin 2 |
| Green | 5V_A10 | Pin 3 |
| Orange | 5V_A11 | Pin 4 |
| Red | 5V_A12 | Pin 5 |
| White/Blue | 5V_A13 | Pin 6 |
| White/Green | 5V_A14 | Pin 38 |
| White/Red | 5V_A15 | Pin 39 |
| Black | GND | Ground return |
| Black/Blue | GND | Ground return |
| Black/Green | GND | Ground return |
| Blue | GND | Ground return |
| Black/White | NC | Spare |
| White | NC | Spare |
| White/Black | NC | Spare |

**Cable 8: Teensy → Level Shifter (3.3V side)**

| Wire Color | Signal | Teensy Pin |
|------------|--------|------------|
| Black/Orange | 3.3V_A8 | Pin 22 |
| Black/Red | 3.3V_A9 | Pin 23 |
| Green | 3.3V_A10 | Pin 20 |
| Orange | 3.3V_A11 | Pin 21 |
| Red | 3.3V_A12 | Pin 38 |
| White/Blue | 3.3V_A13 | Pin 39 |
| White/Green | 3.3V_A14 | Pin 26 |
| White/Red | 3.3V_A15 | Pin 27 |
| Black | GND | Ground return |
| Black/Blue | GND | Ground return |
| Black/Green | GND | Ground return |
| Blue | GND | Ground return |
| Black/White | NC | Spare |
| White | NC | Spare |
| White/Black | NC | Spare |

---

### Cable 4 & 9: Control Inputs - Z80 to Teensy (Module 4)

**Cable 4: Z80 → Level Shifter (5V side)**

| Wire Color | Signal | Z80 Pin | Direction |
|------------|--------|---------|-----------|
| Black/Orange | 5V_HALT | Pin 18 | Z80 → Teensy |
| Black/Red | 5V_MREQ | Pin 19 | Z80 → Teensy |
| Green | 5V_IORQ | Pin 20 | Z80 → Teensy |
| Orange | 5V_RD | Pin 21 | Z80 → Teensy |
| Red | 5V_WR | Pin 22 | Z80 → Teensy |
| White/Blue | 5V_BUSAK | Pin 23 | Z80 → Teensy |
| White/Green | 5V_M1 | Pin 27 | Z80 → Teensy |
| White/Red | 5V_RFSH | Pin 28 | Z80 → Teensy |
| Black | GND | Ground return | - |
| Black/Blue | GND | Ground return | - |
| Black/Green | GND | Ground return | - |
| Blue | GND | Ground return | - |
| Black/White | NC | Spare | - |
| White | NC | Spare | - |
| White/Black | NC | Spare | - |

**Cable 9: Level Shifter → Teensy (3.3V side)**

| Wire Color | Signal | Teensy Pin | Notes |
|------------|--------|------------|-------|
| Black/Orange | 3.3V_HALT | Pin 1 | |
| Black/Red | 3.3V_MREQ | Pin 5 | Interrupt capable |
| Green | 3.3V_IORQ | Pin 0 | |
| Orange | 3.3V_RD | Pin 4 | Interrupt capable |
| Red | 3.3V_WR | Pin 33 | Interrupt capable |
| White/Blue | 3.3V_BUSAK | Pin 24 | |
| White/Green | 3.3V_M1 | Pin 2 | |
| White/Red | 3.3V_RFSH | Pin 3 | |
| Black | GND | Ground return | |
| Black/Blue | GND | Ground return | |
| Black/Green | GND | Ground return | |
| Blue | GND | Ground return | |
| Black/White | NC | Spare | |
| White | NC | Spare | |
| White/Black | NC | Spare | |

---

### Cable 5 & 10: Control Outputs - Teensy to Z80 (Module 5)

**Cable 5: Level Shifter → Z80 (5V side)**

| Wire Color | Signal | Z80 Pin | Direction |
|------------|--------|---------|-----------|
| Black/Orange | 5V_CLK | Pin 6 | Teensy → Z80 |
| Black/Red | 5V_INT | Pin 16 | Teensy → Z80 |
| Green | 5V_NMI | Pin 17 | Teensy → Z80 |
| Orange | 5V_WAIT | Pin 24 | Teensy → Z80 |
| Red | 5V_BUSRQ | Pin 25 | Teensy → Z80 |
| White/Blue | 5V_RESET | Pin 26 | Teensy → Z80 |
| White/Green | NC | Spare| - |
| White/Red | NC | Spare| - |
| Black | GND | Ground return | - |
| Black/Blue | GND | Ground return | - |
| Black/Green | GND | Ground return | - |
| Blue | GND | Ground return | - |
| Black/White | NC | Spare | - |
| White | NC | Spare | - |
| White/Black | NC | Spare | - |

**Cable 10: Teensy → Level Shifter (3.3V side)**

| Wire Color | Signal | Teensy Pin | Notes |
|------------|--------|------------|-------|
| Black/Orange | 3.3V_CLK | Pin 28 | FlexPWM2_A (hardware PWM) |
| Black/Red | 3.3V_INT | Pin 6 | |
| Green | 3.3V_NMI | Pin 9 | |
| Orange | 3.3V_WAIT | Pin 35 | |
| Red | 3.3V_BUSRQ | Pin 32 | |
| White/Blue | 3.3V_RESET | Pin 34 | |
| White/Green | NC | Spare| - |
| White/Red | NC | Spare | - |
| Black | GND | Ground return | |
| Black/Blue | GND | Ground return | |
| Black/Green | GND | Ground return | |
| Blue | GND | Ground return | |
| Black/White | NC | Spare | |
| White | NC | Spare | |
| White/Black | NC | Spare | |

---

### Shield Connection

**Critical:** Shield grounded at **Level Shifter Board ONLY**

- Strip shield braid at level shifter end, solder to board ground
- Leave shield floating (unconnected) at Teensy and Z80 ends
- This prevents ground loops while maintaining EMI protection

---

## Power Distribution

```
┌──────────────────────────────────────────────────────────┐
│                     Power Supply                         │
│                                                          │
│   USB 5V or External 5V Supply (1A minimum)              │
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
Teensy GND ─┬─ Z80 GND (pin 29)
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
│  ┌──────────┐  ┌──────────┐            │
│  │ Module 3 │  │ Module 4 │            │
│  │ Addr Hi  │  │ Control  │            │
│  └────┬─────┘  └────┬─────┘            │
│       │             │                  │
│  ┌────▼─────┐  ┌────▼─────┐            │
│  │ Module 2 │  │ Module 5 │            │
│  │ Addr Lo  │  │ Control  │            │
│  └────┬─────┘  └────┬─────┘            │
│       │             │                  │
│  ┌────▼─────────────▼──────┐           │
│  │     Teensy 4.1          │           │
│  │     (mounted on         │           │
│  │      female headers)    │           │
│  └─────────────────────────┘           │
└────────────────────────────────────────┘

┌────────────────────────────────────────┐
│         Breadboard 2 (Bottom)          │
│                                        │
│  ┌──────────────────────┐              │
│  │   Module 1 (Data)    │              │
│  └──────────┬───────────┘              │
│             │                          │
│  ┌──────────▼───────────┐              │
│  │       Z80 CPU        │              │
│  │     (40-pin DIP)     │              │
│  │                      │              │
│  └──────────────────────┘              │
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
