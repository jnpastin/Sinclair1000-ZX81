# Level Shifting - 5V Z80 to 3.3V Teensy

**Status:** 📋 Planning Phase  
**Date:** December 22, 2025  
**Hardware:** HW-221 Level Shifter Modules (TXS0108E)

---

## Overview

The Z80 CPU operates at **5V logic levels** (both NMOS and CMOS variants), while the Teensy 4.1 operates at **3.3V logic levels**. Bidirectional level shifting is required for all signals.

**Critical:** Teensy 4.1 GPIO pins are **NOT 5V-tolerant** (only VIN/VUSB power pins are). Level shifters are **mandatory** in both directions to protect the Teensy from 5V Z80 signals.

---

## Voltage Level Requirements

### Z80 Logic Levels (5V CMOS)

| Parameter    | Symbol | Min | Typ | Max | Unit |
|--------------|--------|-----|-----|-----|------|
| Input HIGH   | V_IH   | 3.5 | -   | 5.0 | V    |
| Input LOW    | V_IL   | 0.0 | -   | 0.8 | V    |
| Output HIGH  | V_OH   | 4.5 | 5.0 | -   | V    |
| Output LOW   | V_OL   | 0.0 | -   | 0.4 | V    |

### Z80 Logic Levels (5V NMOS) - Legacy

| Parameter    | Symbol | Min | Typ | Max | Unit |
|--------------|--------|-----|-----|-----|------|
| Input HIGH   | V_IH   | 2.0 | -   | 5.0 | V    |
| Input LOW    | V_IL   | 0.0 | -   | 0.8 | V    |
| Output HIGH  | V_OH   | 2.4 | 5.0 | -   | V    |
| Output LOW   | V_OL   | 0.0 | -   | 0.4 | V    |

**Note:** NMOS Z80 has lower V_IH (2.0V) and V_OH (2.4V) thresholds than CMOS (3.5V, 4.5V).

### Teensy 4.1 Logic Levels (3.3V)

| Parameter      | Symbol | Min | Typ | Max    | Unit |
|----------------|--------|-----|-----|--------|------|
| Input HIGH     | V_IH   | 2.0 | -   | 3.6    | V    |
| Input LOW      | V_IL   | 0.0 | -   | 0.8    | V    |
| Output HIGH    | V_OH   | 2.4 | 3.3 | -      | V    |
| Output LOW     | V_OL   | 0.0 | -   | 0.4    | V    |
| **Max Input**  | V_IN   | -   | -   | **5.25** | V  |

**Important:** Teensy GPIO pins are **NOT 5V-tolerant** (V_IN max 3.6V absolute maximum). Level shifters are **required** to prevent damage.

---

## Level Shifter Selection: TXS0108E

### Why TXS0108E (HW-221 Module)?

✅ **Bidirectional** - Auto-direction sensing (no DIR pin needed)  
✅ **8 channels** - Perfect for data bus, address bytes, control groups  
✅ **Fast** - 110 MHz bandwidth (Z80 runs <5 MHz)  
✅ **Low propagation delay** - 2-3 ns (negligible at Z80 speeds)  
✅ **Wide voltage range** - 1.2V-3.6V (A side), 1.65V-5.5V (B side)  
✅ **5V compatible** - Works with both NMOS and CMOS Z80  
✅ **Auto-direction** - Seamless for bidirectional data bus  

### TXS0108E Specifications

| Parameter          | Symbol | Min  | Typ | Max | Unit | Notes            |
|--------------------|--------|------|-----|-----|------|------------------|
| A-side voltage     | VCCA   | 1.2  | 3.3 | 3.6 | V    | Teensy side      |
| B-side voltage     | VCCB   | 1.65 | 5.0 | 5.5 | V    | Z80 side         |
| Max data rate      | f_max  | -    | -   | 110 | MHz  | Far exceeds Z80  |
| Propagation delay  | t_pd   | -    | 2   | 3   | ns   | A→B or B→A      |
| Rise time          | t_r    | -    | -   | 2   | ns   | 10%-90%          |
| Fall time          | t_f    | -    | -   | 2   | ns   | 90%-10%          |
| Output enable time | t_OE   | -    | -   | 100 | ns   | Startup          |

**Key Point:** 2-3 ns propagation delay is **negligible** compared to Z80 bus cycle time (>200 ns @ 5 MHz).

---

## HW-221 Module Pinout

```
         ┌─────────────────┐
    GND ─┤1              20├─ VCCB (5V)
     OE ─┤2              19├─ B8
     A1 ─┤3              18├─ B7
     A2 ─┤4              17├─ B6
     A3 ─┤5              16├─ B5
     A4 ─┤6              15├─ B4
     A5 ─┤7              14├─ B3
     A6 ─┤8              13├─ B2
     A7 ─┤9              12├─ B1
     A8 ─┤10             11├─ VCCA (3.3V)
         └─────────────────┘
         
A side = Teensy (3.3V)
B side = Z80 (5V)
OE = Output Enable (pull HIGH to enable, LOW to disable)
```

### Module Connections

| Pin | Name  | Connection             | Notes                                        |
|-----|-------|------------------------|----------------------------------------------|
| 1   | GND   | Common ground          | Connect to both Teensy GND and Z80 GND       |
| 2   | OE    | Teensy 3.3V            | Tie HIGH to enable (active HIGH)             |
| 3-10| A1-A8 | Teensy GPIO            | 3.3V side (Teensy pins)                      |
| 11  | VCCA  | Teensy 3.3V            | Power from Teensy 3.3V output                |
| 12-19| B1-B8| Z80 pins               | 5V side (Z80 pins)                           |
| 20  | VCCB  | +5V supply             | Power Z80 and level shifter B side           |

**Important:** OE pin must be HIGH to enable level shifting. Tie to VCCA (3.3V).

---

## Complete Level Shifter Wiring

### Module 1: Data Bus (D0-D7) - Bidirectional

**Purpose:** Bidirectional data bus between Z80 and Teensy

| Module Pin | Teensy Side (3.3V) | Z80 Side (5V)  | Direction     |
|------------|--------------------|-----------------|--------------|
| A1         | Pin 0 (D4)         | Pin 14 (D4)    | ← Bidir →   |
| A2         | Pin 1 (D3)         | Pin 12 (D3)    | ← Bidir →   |
| A3         | Pin 2 (D5)         | Pin 8 (D5)     | ← Bidir →   |
| A4         | Pin 3 (D6)         | Pin 7 (D6)     | ← Bidir →   |
| A5         | Pin 4 (D2)         | Pin 9 (D2)     | ← Bidir →   |
| A6         | Pin 5 (D7)         | Pin 10 (D7)    | ← Bidir →   |
| A7         | Pin 6 (D0)         | Pin 13 (D0)    | ← Bidir →   |
| A8         | Pin 7 (D1)         | Pin 15 (D1)    | ← Bidir →   |
| VCCA       | Teensy 3.3V        | -              | Power        |
| VCCB       | -                  | +5V            | Power        |
| OE         | Teensy 3.3V        | -              | Enable       |
| GND        | Teensy GND         | Z80 GND        | Ground       |**Configuration:** Leave OE high (tie to VCCA). TXS0108E auto-detects direction.

---

### Module 2: Address Low Byte (A0-A7) - Unidirectional

**Purpose:** Address bus low byte (Z80 → Teensy, read-only)

| Module Pin | Teensy Side (3.3V) | Z80 Side (5V)  | Direction  |
|------------|--------------------|-----------------|-----------|
| A1         | Pin 8 (A0)         | Pin 30 (A0)    | ← Input   |
| A2         | Pin 9 (A1)         | Pin 31 (A1)    | ← Input   |
| A3         | Pin 10 (A2)        | Pin 32 (A2)    | ← Input   |
| A4         | Pin 11 (A3)        | Pin 33 (A3)    | ← Input   |
| A5         | Pin 12 (A4)        | Pin 34 (A4)    | ← Input   |
| A6         | Pin 13 (A5)        | Pin 35 (A5)    | ← Input   |
| A7         | Pin 14 (A6)        | Pin 36 (A6)    | ← Input   |
| A8         | Pin 15 (A7)        | Pin 37 (A7)    | ← Input   |
| VCCA       | Teensy 3.3V        | -              | Power      |
| VCCB       | -                  | +5V            | Power      |
| OE         | Teensy 3.3V        | -              | Enable     |
| GND        | Teensy GND         | Z80 GND        | Ground     |**Configuration:** Unidirectional Z80→Teensy. TXS0108E handles this automatically.

---

### Module 3: Address High Byte (A8-A15) - Unidirectional

**Purpose:** Address bus high byte (Z80 → Teensy, read-only)

| Module Pin | Teensy Side (3.3V) | Z80 Side (5V)  | Direction  |
|------------|--------------------|-----------------|-----------|
| A1         | Pin 16 (A8)        | Pin 38 (A8)    | ← Input   |
| A2         | Pin 17 (A9)        | Pin 39 (A9)    | ← Input   |
| A3         | Pin 18 (A10)       | Pin 1 (A10)    | ← Input   |
| A4         | Pin 19 (A11)       | Pin 2 (A11)    | ← Input   |
| A5         | Pin 20 (A12)       | Pin 3 (A12)    | ← Input   |
| A6         | Pin 21 (A13)       | Pin 4 (A13)    | ← Input   |
| A7         | Pin 22 (A14)       | Pin 5 (A14)    | ← Input   |
| A8         | Pin 23 (A15)       | Pin 6 (A15)    | ← Input   |
| VCCA       | Teensy 3.3V        | -              | Power      |
| VCCB       | -                  | +5V            | Power      |
| OE         | Teensy 3.3V        | -              | Enable     |
| GND        | Teensy GND         | Z80 GND        | Ground     |---

### Module 4: Control Inputs (Z80 → Teensy) - Unidirectional

**Purpose:** Z80 control signal outputs (Z80 → Teensy, read-only)

| Module Pin | Teensy Side (3.3V)  | Z80 Side (5V)   | Direction | Signal         |
|------------|---------------------|------------------|-----------|-----------------|
| A1         | Pin 24 (/RD)        | Pin 21 (/RD)    | ← Input   | Read strobe    |
| A2         | Pin 25 (/WR)        | Pin 22 (/WR)    | ← Input   | Write strobe   |
| A3         | Pin 26 (/MREQ)      | Pin 19 (/MREQ)  | ← Input   | Memory request |
| A4         | Pin 27 (/IORQ)      | Pin 20 (/IORQ)  | ← Input   | I/O request    |
| A5         | Pin 28 (/M1)        | Pin 27 (/M1)    | ← Input   | Machine cycle 1|
| A6         | Pin 29 (/RFSH)      | Pin 28 (/RFSH)  | ← Input   | Refresh cycle  |
| A7         | Pin 30 (/HALT)      | Pin 18 (/HALT)  | ← Input   | Halt status    |
| A8         | Pin 31 (/BUSACK)    | Pin 23 (/BUSACK)| ← Input   | Bus acknowledge|
| VCCA       | Teensy 3.3V         | -               | Power     | -              |
| VCCB       | -                   | +5V             | Power     | -              |
| OE         | Teensy 3.3V         | -               | Enable    | -              |
| GND        | Teensy GND          | Z80 GND         | Ground    | -              |**Note:** /RD and /WR on this module are interrupt sources for Teensy ISRs.

---

### Module 5: Control Outputs (Teensy → Z80) - Unidirectional

**Purpose:** Teensy control signal outputs (Teensy → Z80, write-only)

| Module Pin | Teensy Side (3.3V)  | Z80 Side (5V)    | Direction | Signal                   |
|------------|---------------------|-------------------|-----------|--------------------------|
| A1         | Pin 36 (CLK)        | Pin 6 (CLK)      | Output →  | Clock signal             |
| A2         | Pin 37 (/RESET)     | Pin 26 (/RESET)  | Output →  | Reset (active low)       |
| A3         | Pin 38 (/WAIT)      | Pin 24 (/WAIT)   | Output →  | Wait state               |
| A4         | Pin 39 (/INT)       | Pin 16 (/INT)    | Output →  | Interrupt request        |
| A5         | Pin 40 (/NMI)       | Pin 17 (/NMI)    | Output →  | Non-maskable interrupt   |
| A6         | Pin 41 (/BUSREQ)    | Pin 25 (/BUSREQ) | Output →  | Bus request              |
| A7         | (spare)             | -                | -         | Unused                   |
| A8         | (spare)             | -                | -         | Unused                   |
| VCCA       | Teensy 3.3V         | -                | Power     | -                        |
| VCCB       | -                   | +5V              | Power     | -                        |
| OE         | Teensy 3.3V         | -                | Enable    | -                        |
| GND        | Teensy GND          | Z80 GND          | Ground    | -                        |**Note:** CLK signal generated by Teensy FlexPWM (Phase 2).

---

## Timing Analysis

### Z80 Bus Cycle Timing (5 MHz Operation)

| Parameter     | Symbol | Min | Typ | Max | Unit | Notes             |
|---------------|--------|-----|-----|-----|------|-------------------|
| Clock period  | T_CLK  | 200 | -   | -   | ns   | @ 5 MHz           |
| Clock HIGH    | T_CH   | 90  | -   | -   | ns   | 45% duty          |
| Clock LOW     | T_CL   | 110 | -   | -   | ns   | 55% duty          |
| Address setup | T_AS   | 60  | -   | -   | ns   | Before RD/WR      |
| Address hold  | T_AH   | 20  | -   | -   | ns   | After RD/WR       |
| Data setup    | T_DS   | 50  | -   | -   | ns   | Write cycle       |
| Data hold     | T_DH   | 0   | -   | -   | ns   | Write cycle       |
| Data access   | T_ACC  | -   | -   | 120 | ns   | Read cycle        |

### Level Shifter Propagation Delay

| Signal Path                  | Shifter Delay | % of T_CLK | Impact      |
|------------------------------|---------------|------------|-------------|
| Address (Z80→Teensy)         | 2-3 ns        | 1-1.5%     | Negligible  |
| Data read (Z80→Teensy)       | 2-3 ns        | 1-1.5%     | Negligible  |
| Data write (Teensy→Z80)      | 2-3 ns        | 1-1.5%     | Negligible  |
| Control signals              | 2-3 ns        | 1-1.5%     | Negligible  |
| Clock (Teensy→Z80)           | 2-3 ns        | 1-1.5%     | Negligible  |**Conclusion:** TXS0108E propagation delay (2-3 ns) is **<2% of Z80 clock period** at 5 MHz. This is insignificant compared to Teensy ISR latency budget (~50-100 ns).

### ISR Timing Budget (5 MHz Z80)

| Event                         | Time Budget | TXS0108E Delay | Remaining |
|-------------------------------|-------------|----------------|-----------|  
| Z80 asserts RD/WR             | 0 ns        | -              | -         |
| Level shifter B→A             | 2-3 ns      | ✓              | -         |
| Teensy GPIO interrupt         | 5 ns        | -              | -         |
| ISR entry latency             | 50 ns       | -              | -         |
| Read address bus              | 15 ns       | 2-3 ns ✓       | -         |
| Fetch data from cache         | 10 ns       | -              | -         |
| Write data bus                | 15 ns       | 2-3 ns ✓       | -         |
| ISR exit                      | 20 ns       | -              | -         |
| **Total**                     | **120 ns**  | **~10 ns total**| **110 ns**|**Result:** Level shifter delays consume only **~10 ns of 120 ns budget** (<10%). Plenty of margin!

---

## Power Supply Design

### Power Requirements

| Component                   | Voltage | Current      | Notes                         |
|-----------------------------|---------|--------------|-------------------------------|
| **Teensy 4.1**              | 3.3V    | ~100 mA      | Normal operation              |
| **Z80 CPU**                 | 5V      | 50-200 mA    | CMOS: 50mA, NMOS: 200mA       |
| **TXS0108E (x5)**           | 3.3V+5V | ~10 mA each  | Static + switching current    |
| **Level shifter VCCA (x5)** | 3.3V    | 50 mA total  | A-side power                  |
| **Level shifter VCCB (x5)** | 5V      | 50 mA total  | B-side power                  |

**Total:**
- **3.3V rail:** 150 mA (Teensy + level shifters)
- **5V rail:** 300 mA (Z80 NMOS + level shifters)

### Power Supply Options

#### Selected Design: USB Power via VUSB ✅
```
USB Cable → Teensy
           ├─→ VUSB (5V) → Z80 Vcc + Level Shifters VCCB
           ├─→ Teensy 3.3V pin → Level Shifters VCCA
           └─→ Programming + Serial Communication
```

**Implementation Details:**
- VUSB pin provides 5V directly from USB (500 mA max per USB 2.0 spec)
- Teensy's onboard 3.3V regulator powers VCCA side (250 mA max capacity)
- Single cable for power, programming, and serial communication

**Current Budget (NMOS Z80 + LEDs):**
- 3.3V rail: 150 mA / 250 mA capacity = **60% utilization** ✅
- 5V rail (continuous): 250 mA / 500 mA capacity = **50% utilization** ✅
- 5V rail (with LEDs): +76 mA peak (38 LEDs @ 2 mA each)
  - Peak total: 326 mA / 500 mA = **65%** ✅
  - Average (50% duty): 288 mA / 500 mA = **58%** ✅
  - Average (30% duty): 273 mA / 500 mA = **55%** ✅

**Pros:** 
- Single cable solution
- No external power supply needed
- Adequate for NMOS Z80 testing (200 mA typical)
- Programming + power integrated

**Cons:** 
- Limited to USB current capacity
- Not suitable for high-power peripherals

**Future Option:** External 5V supply to VIN if more current needed

#### Alternative: External 5V Supply (If Needed)
```
Wall Wart (7-12V) → 7805 Regulator → 5V
                                     ├→ Teensy VIN
                                     ├→ Z80 Vcc
                                     └→ Level Shifters VCCB

Tteensy 3.3V pin → Level Shifters VCCA
```

**When to use:** High-power CMOS variants, additional peripherals, or extended testing

### Bypass Capacitors (Critical!)

**Dual capacitor strategy** - Electrolytic for bulk energy storage, ceramic for high-frequency transients:

| Location                  | Electrolytic | Ceramic | Purpose                              |
|---------------------------|--------------|---------|--------------------------------------|
| Z80 Vcc (Pin 11)          | 10µF         | 100nF   | CPU power + switching transients     |
| Teensy 3.3V pin           | 10µF         | 100nF   | Regulator output + level shifters    |
| Teensy VUSB pin           | 10µF         | 100nF   | USB input filtering                  |
| Level shifter 5V rail     | 220µF        | 100nF   | LED switching transients (bulk)      |
| Level shifter 3.3V rail   | -            | 100nF   | High-frequency noise suppression     |
| TXS0108E modules (x5)     | -            | Built-in| Onboard decoupling (HW-221)          |

**Why dual capacitors?**

**Electrolytic (10-220µF):**
- High capacitance = bulk energy storage
- Low frequency response (DC - 100 kHz)
- High ESR (1-5Ω) and ESL (10-20 nH)
- Handles sustained current draw and slow load changes

**Ceramic (100nF):**
- Low ESR (0.01-0.1Ω) and ESL (0.5-2 nH)
- High frequency response (1-100+ MHz)
- Suppresses fast switching transients (<10 ns)
- Critical for LED switching at 5 MHz and digital noise

**Combined impedance:** Low across all frequencies (DC to 100 MHz)

**Placement:**
- **Electrolytics:** Within a few cm of power pins
- **Ceramics:** <1 cm from IC power pins (critical for low inductance)
- Place in parallel (physically close together)

**LED switching impact:**
- 38 LEDs @ 2 mA = 76 mA peak surge in <10 ns
- Ceramic caps handle fast transients (ΔV = 6 mV)
- Electrolytic supplies sustained average current
- 220µF at level shifter board prevents voltage sag during simultaneous LED turn-on

---

## Common Ground Architecture

**Critical:** All grounds must be connected together:

```
Teensy GND ─┬─ Z80 GND
            ├─ Module 1 GND
            ├─ Module 2 GND
            ├─ Module 3 GND
            ├─ Module 4 GND
            ├─ Module 5 GND
            └─ Power supply GND
```

**No ground loops!** Use star ground topology with single connection point.

---

## Level Shifter Enable Control (OE Pins)

### Implementation: Software-Controlled Enable ✅

**All 5 OE pins tied together → Single Teensy GPIO (Pin 42)**

```
Teenosy Pin 42 ──┬→ Module 1 OE
                 ├→ Module 2 OE
                 ├→ Module 3 OE
                 ├→ Module 4 OE
                 └→ Module 5 OE
                 
10kΩ pull-down to GND (keeps disabled if Teensy not powered)
```

**Use Cases:**
1. **Clean Z80 Reset** - Disable level shifters during reset to prevent floating signals
2. **Power Sequencing** - Enable level shifters after power rails stabilize
3. **Hot-Swap Safety** - Disable before removing/inserting Z80
4. **Debug Modes** - Test Teensy in isolation without Z80 interference
5. **Bus Tri-State** - Future expansion for external bus masters

**Software Control:**
```cpp
#define LEVEL_SHIFTER_OE_PIN  42

void setup() {
    pinMode(LEVEL_SHIFTER_OE_PIN, OUTPUT);
    digitalWrite(LEVEL_SHIFTER_OE_PIN, LOW);  // Start disabled
    
    // ... power-up sequence ...
    
    digitalWrite(LEVEL_SHIFTER_OE_PIN, HIGH); // Enable after setup
}

void resetZ80() {
    digitalWrite(LEVEL_SHIFTER_OE_PIN, LOW);  // Isolate bus
    digitalWrite(RESET_PIN, LOW);
    delay(10);
    digitalWrite(RESET_PIN, HIGH);
    delay(1);
    digitalWrite(LEVEL_SHIFTER_OE_PIN, HIGH); // Re-enable
}
```

**Hardware:**
- 10kΩ resistor from OE net to GND (pull-down)
- Single trace/wire connecting all 5 OE pins in parallel

---

## Interconnect Cables

### Selected Design: Shielded Cable with JST XH Connectors ✅

**Cable Type:** 15-conductor shielded cable

**Connector:** JST XH series (2.5mm pitch)
- Module 1 (Data): 12-pin (8 signals + 4 grounds)
- Module 2 (Addr Low): 12-pin (8 signals + 4 grounds)
- Module 3 (Addr High): 12-pin (8 signals + 4 grounds)
- Module 4 (Control In): 12-pin (8 signals + 4 grounds)
- Module 5 (Control Out): 10-pin (6 signals + 4 grounds)
- Power: 4-6 pin (VUSB, 3.3V, multiple GNDs)

**Cable Length Recommendations:**
- **3.3V side (Teensy to Level Shifters):** ≤6" (keep short)
- **5V side (Level Shifters to Z80):** ≤12" (can be longer)
- **Rationale:** Z80 has stronger drive strength, Teensy side more sensitive to noise

**Ground Distribution (Critical for Signal Integrity):**
```
Example 12-pin connector pinout:
Pin 1:  Signal 1
Pin 2:  GND
Pin 3:  Signal 2
Pin 4:  Signal 3
Pin 5:  GND
Pin 6:  Signal 4
Pin 7:  Signal 5
Pin 8:  GND
Pin 9:  Signal 6
Pin 10: Signal 7
Pin 11: GND
Pin 12: Signal 8
```

**Ground ratio:** 1 ground per 2-3 signals to prevent ground bounce

**Shield Connection:**
- Ground shield at **level shifter board only** (central point)
- Leave floating at Teensy and Z80 boards
- Prevents ground loop currents while maintaining EMI protection
- Crimp shield wire into a ground pin or solder to connector housing

**Signal Integrity Benefits:**
- ✅ Twisted conductor pairs reduce crosstalk
- ✅ Shield provides EMI protection
- ✅ Multiple ground returns minimize ground bounce
- ✅ Controlled impedance better than ribbon cable
- ✅ JST latching prevents accidental disconnects

**Reflection Prevention (if needed):**
- Add 47-100Ω series resistors on driving end if reflections observed
- Monitor with logic analyzer for ringing, multiple edges, or slow rise times

**Assembly Tips:**
- Test continuity before connecting
- Label both cable ends with heat shrink markers
- Use strain relief (heat shrink over cable jacket at connector)
- Plan pinout before crimping (JST XH terminals not reusable)

---

## Debug LEDs

### Implementation: LEDs on 5V Side (Level Shifter Outputs) ✅

**Design Decision:** All signal indicator LEDs powered from 5V rail

```
Level Shifter B-side output (5V) ──[1.5kΩ]──[LED]──GND
                                   
When signal HIGH (5V): (5V - 2V) / 1.5kΩ ≈ 2 mA
When signal LOW (0V): LED off
```

**LED Configuration:**

| Signal Group        | Count | Current (peak) | Current (avg @30% duty) |
|---------------------|-------|----------------|-------------------------|
| Data Bus (D0-D7)    | 8     | 16 mA          | 5 mA                    |
| Address Bus (A0-A15)| 16    | 32 mA          | 10 mA                   |
| Control Signals     | 13    | 26 mA          | 8 mA                    |
| Clock (CLK)         | 1     | 2 mA           | 1 mA (50% duty)         |
| **Total**           | **38**| **76 mA**      | **24 mA**               |

**Power Budget Impact:**
- Continuous loads (Z80 + level shifters): 250 mA
- LED peak: +76 mA
- LED average (@30% duty): +24 mA
- **Peak total: 326 mA / 500 mA = 65%** ✅
- **Average total: 274 mA / 500 mA = 55%** ✅

**Why 5V side:**
- ✅ Ample power budget available (250 mA headroom on 5V rail)
- ✅ Signals already buffered by level shifters (no loading on Teensy/Z80)
- ✅ Better voltage headroom (5V vs 3.3V) for stable LED current
- ✅ Single power source for all LEDs simplifies wiring
- ✅ No impact on 3.3V rail (stays at 60% utilization)

**Component Selection:**
- LEDs: Standard 5mm or 3mm (red, green, yellow)
- Resistors: 1.5kΩ (1/4W) for 2 mA per LED
- Forward voltage: 1.8-2.2V typical
- Brightness: ~2 mA provides good visibility without excessive power

**Physical Placement:**
- Mount LEDs on level shifter board near corresponding signal outputs
- Group by function (data bus together, address bus together, etc.)
- Use LED arrays or individual LEDs as space permits

**Benefits:**
- Real-time visual feedback of bus activity
- No oscilloscope needed for basic functionality verification
- Helps debug timing issues, stuck signals, or incorrect wiring
- Educational - shows Z80 operation patterns

**Duty Cycle Consideration:**
- LEDs flash with actual bus activity
- Clock LED at 5 MHz appears constantly lit to human eye (50% duty)
- Data/address LEDs flicker based on instruction execution
- Average power much lower than peak due to real-world duty cycles



## Compatibility with Z80 Variants

### NMOS Z80 (Original, 1976-1990s)
- **Voltage:** 5V ±5%
- **V_IH:** 2.0V min → **3.3V Teensy output OK** ✓
- **V_OH:** 2.4V min → **TXS0108E provides 4.5V** ✓
- **Current:** 200 mA typ (high power)
- **Speed:** 2.5-4 MHz typical

**Verdict:** Fully compatible with TXS0108E level shifters.

### CMOS Z80 (Modern, 1990s-present)
- **Voltage:** 5V ±10%
- **V_IH:** 3.5V min → **TXS0108E provides 4.5V** ✓
- **V_OH:** 4.5V min → **TXS0108E provides 4.5V** ✓
- **Current:** 50 mA typ (low power)
- **Speed:** 4-20 MHz

**Verdict:** Fully compatible with TXS0108E level shifters.

---

## Testing & Validation

### Level Shifter Test Procedure

1. **Power-up test**
   - [ ] Measure VCCA = 3.3V ±0.1V on all modules
   - [ ] Measure VCCB = 5.0V ±0.1V on all modules
   - [ ] Verify OE pin = 3.3V (HIGH) on all modules

2. **Static level test**
   - [ ] Teensy output 0V → Z80 input reads 0V
   - [ ] Teensy output 3.3V → Z80 input reads 5V
   - [ ] Z80 output 0V → Teensy input reads 0V
   - [ ] Z80 output 5V → Teensy input reads 3.3V

3. **Dynamic test**
   - [ ] Generate 1 MHz square wave on Teensy pin
   - [ ] Verify 1 MHz square wave on Z80 pin (with scope)
   - [ ] Measure propagation delay <5 ns

4. **Data bus test**
   - [ ] Write 0xAA to data bus from Teensy
   - [ ] Verify Z80 reads 0xAA
   - [ ] Write 0x55 to data bus from Z80
   - [ ] Verify Teensy reads 0x55

---

## Hardware Checklist

- [ ] 5x HW-221 (TXS0108E) level shifter modules ordered
- [ ] Teensy 4.1 development board
- [ ] Z80 CPU (NMOS or CMOS, 40-pin DIP)
- [ ] Breadboard (2x 830-point recommended)
- [ ] Jumper wires (40+ male-to-male)
- [ ] 5V power supply (1A minimum)
- [ ] Bypass capacitors (6x 100nF, 6x 10µF)
- [ ] Logic analyzer or oscilloscope (for debugging)

---

## Next Steps

1. **Order components** - See hardware checklist
2. **Build Module 5 first** - Test clock generation (easiest to verify)
3. **Add Modules 2-3** - Test address bus reads
4. **Add Module 4** - Test control signal monitoring
5. **Add Module 1 last** - Test bidirectional data bus
6. **Full system test** - Run Z80 test programs

---

**Status:** ✅ Complete - Ready for hardware implementation  
**Next Document:** [Hardware_Comparison.md](Hardware_Comparison.md) - Platform comparison
