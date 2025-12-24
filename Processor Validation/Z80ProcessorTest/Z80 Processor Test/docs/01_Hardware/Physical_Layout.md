# Physical Layout - Proto Board Assembly

**Status:** 📋 Optimized Pin Mapping  
**Date:** December 23, 2025  
**Hardware:** Breadboard-style proto boards with threaded insert mounting

---

## Overview

Physical assembly uses three breadboard-style proto boards (PCB versions with pre-wired traces) mounted on a 3D printed base plate with threaded inserts acting as standoffs.

**Proto Board Specification:**
- 63 rows × 10 columns (A-J)
- Each row has two buses:
  - Columns A-E: Electrically connected
  - Columns F-J: Electrically connected
  - Gap between E and F (isolated)
- Power rails on both sides (run perpendicular to rows)

---

## Board Allocation

### Board 1: Level Shifter Board (63 rows)

**Purpose:** 5× HW-221 (TXS0108E) level shifter modules with connectors

**Module Placement:**
- Module 1 (Data Bus): Rows 2-11
- Module 2 (Address Low): Rows 14-23
- Module 3 (Address High): Rows 26-35
- Module 4 (Control In): Rows 38-47
- Module 5 (Control Out + CLK): Rows 50-59
- OE Junction & Caps: Rows 60-63

**Spacing:** 2-3 rows between modules for routing and breathing room

### Board 2: Z80 + LEDs (63 rows)

**Purpose:** DIP-40 Z80 socket, debug LEDs, signal input connectors

**Layout:**
- DIP-40 socket (centered, ~20 rows)
- LED arrays (38 LEDs with resistors)
- JST XH input connectors from level shifter board
- Bypass capacitors

### Board 3: Teensy (30 rows)

**Purpose:** Teensy 4.1 controller with output connectors

**Layout:**
- Teensy 4.1 (socketed or soldered)
- JST XH output connectors to level shifter board
- Bypass capacitors
- USB cable entry point

---

## Level Shifter Board Detailed Layout

### Module Pattern (Repeats for all 5 modules)

**Example: Module 1 (Data Bus, Rows 2-11)**

```
+=====+==================+===+============+====+===+===+=====+===+===+====================+
| Row |        A         | B |     C      | D  | E | F |  G  | H | I |         J          |
+=====+==================+===+============+====+===+===+=====+===+===+====================+
|   2 | Jumper to 3.3V   |   |            | VA |   |   | VB  |   |   | Jumper to 5V       |
+-----+------------------+---+------------+----+---+---+-----+---+---+--------------------+
|   3 | J1 Pin 1 (D0)    |   | Header Pin | A1 |   |   | B1  |   |   | J2 Pin 1 (D0)      |
+-----+------------------+---+------------+----+---+---+-----+---+---+--------------------+
|   4 | J1 Pin 2 (D1)    |   | Header Pin | A2 |   |   | B2  |   |   | J2 Pin 2 (D1)      |
+-----+------------------+---+------------+----+---+---+-----+---+---+--------------------+
|   5 | J1 Pin 3 (D2)    |   | Header Pin | A3 |   |   | B3  |   |   | J2 Pin 3 (D2)      |
+-----+------------------+---+------------+----+---+---+-----+---+---+--------------------+
|   6 | J1 Pin 4 (D3)    |   | Header Pin | A4 |   |   | B4  |   |   | J2 Pin 4 (D3)      |
+-----+------------------+---+------------+----+---+---+-----+---+---+--------------------+
|   7 | J1 Pin 5 (D4)    |   | Header Pin | A5 |   |   | B5  |   |   | J2 Pin 5 (D4)      |
+-----+------------------+---+------------+----+---+---+-----+---+---+--------------------+
|   8 | J1 Pin 6 (D5)    |   | Header Pin | A6 |   |   | B6  |   |   | J2 Pin 6 (D5)      |
+-----+------------------+---+------------+----+---+---+-----+---+---+--------------------+
|   9 | J1 Pin 7 (D6)    |   | Header Pin | A7 |   |   | B7  |   |   | J2 Pin 7 (D6)      |
+-----+------------------+---+------------+----+---+---+-----+---+---+--------------------+
|  10 | J1 Pin 8 (D7)    |   | Header Pin | A8 |   |   | B8  |   |   | J2 Pin 8 (D7)      |
+-----+------------------+---+------------+----+---+---+-----+---+---+--------------------+
|  11 | Jumper to Row 61 |   |            | OE |   |   | GND |   |   | Jumper to GND rail |
+-----+------------------+---+------------+----+---+---+-----+---+---+--------------------+

Left Ground Rail:  J1-GND (4-pin JST XH) - Ground wires from Teensy
Right Ground Rail: J2-GND (4-pin JST XH) - Ground wires to Z80
Left Power Rail:   3.3V distribution to all VA pins
Right Power Rail:  5V distribution to all VB pins
```

**Key Points:**
- **Row 2:** Power connections (VA to 3.3V, VB to 5V)
- **Rows 3-10:** 8 signal paths (module pins straddle center gap)
- **Row 11:** OE to common bus (row 61), GND to ground rail
- **Column A:** J1 signal connector (8-pin JST XH)
- **Column C:** Logic analyzer header (10-pin, 8 signals + 2 grounds)
- **Column D:** Module A-side (3.3V, from Teensy)
- **Column G:** Module B-side (5V, to Z80)
- **Column J:** J2 signal connector (8-pin JST XH)
- **Ground Rails:** Separate 4-pin JST XH connectors for ground wires

### Complete Module Allocations

**Module 1: Data Bus (D0-D7)**
- Rows: 2-11
- A-side: Teensy pins 10,12,11,13,8,7,36,37
- B-side: Z80 pins 14,15,12,8,7,9,10,13

**Module 2: Address Low Byte (A0-A7)**
- Rows: 14-23
- A-side: Teensy pins 19,18,14,15,40,41,17,16
- B-side: Z80 pins 30,31,32,33,34,35,36,37

**Module 3: Address High Byte (A8-A15)**
- Rows: 26-35
- A-side: Teensy pins 22,23,20,21,38,39,26,27
- B-side: Z80 pins 1,2,3,4,5,6,38,39

**Module 4: Control Inputs**
- Rows: 38-47
- Signals: HALT, IORQ, BUSACK, M1, RFSH, RD, WR, MREQ
- A-side: Teensy pins 1,0,24,2,3,4,33,5
- B-side: Z80 pins 18,20,23,27,28,21,22,19

**Module 5: Control Outputs + Clock**
- Rows: 50-59
- Signals: CLK, INT, NMI, WAIT, BUSREQ, RESET
- A-side: Teensy pins 28,6,9,35,32,34
- B-side: Z80 pins 6,16,17,24,25,26

### Rows 60-63: Support Circuitry

**Row 60:** Spare / Test points

**Row 61: OE Junction**
```
Columns A-E bus:
- Jumpers from all 5 modules (rows 11, 23, 35, 47, 59 column D)
- 10kΩ resistor to ground
- Wire to Teensy Pin 31 (via separate JST connector or direct wire)
```

**Row 62: Bypass Capacitors**
- 220µF electrolytic on 5V rail (columns F-J)
- 100nF ceramic on 5V rail (columns F-J)
- 100nF ceramic on 3.3V rail (columns A-E)

**Row 63: Test Points**
- 3.3V test point
- 5V test point
- Ground test points

### Power Distribution

**Left Side Power Rails (3.3V + GND):**
- 3.3V from Teensy → Distribute to VA pins (rows 2, 14, 26, 38, 50)
- Ground for A-side and reference

**Right Side Power Rails (5V + GND):**
- 5V from VUSB → Distribute to VB pins (rows 2, 14, 26, 38, 50)
- 5V also powers LED circuits on Z80 board
- Ground for B-side and reference

**Ground interconnection:** Both ground rails connected together (star ground topology)

---

## Cable Connections

### From Teensy to Level Shifter Board (5 cables)

**Each cable:** 12-conductor shielded cable (~6" length)

**Teensy end:** 12-pin JST XH connector (8 signals + 4 grounds)

**Level shifter end:** Splits into two connectors just before board
- 8-pin JST XH → Column A, rows 3-10 (signals)
- 4-pin JST XH → Left ground rail (grounds)

**Ground distribution:** 1 ground per 2-3 signal wires in cable

**Shield:** Grounded at level shifter board only (prevent ground loops)

### From Level Shifter Board to Z80 Board (5 cables)

**Each cable:** 12-conductor shielded cable (~12" length)

**Level shifter end:** Splits from board connectors
- 8-pin JST XH from Column J, rows 3-10 (signals)
- 4-pin JST XH from right ground rail (grounds)

**Z80 board end:** 12-pin JST XH connector (8 signals + 4 grounds)

**Rationale for longer length:** Z80 side has stronger drive strength (5V), less sensitive to cable capacitance

### Power and Control

**Main power:** USB cable to Teensy
- VUSB (5V) distributed to level shifter board and Z80 board
- Teensy 3.3V pin distributed to level shifter board

**OE Control:** Single wire or small 2-pin JST connector
- From Teensy Pin 31 to level shifter row 61

---

## Connector Bill of Materials

### Level Shifter Board

**Per module (×5):**
- 1× 8-pin JST XH (A-side signals)
- 1× 4-pin JST XH (A-side grounds, on left power rail)
- 1× 8-pin JST XH (B-side signals)
- 1× 4-pin JST XH (B-side grounds, on right power rail)
- 1× 10-pin male header (logic analyzer, 2.54mm pitch)

**Board total:**
- 10× 8-pin JST XH receptacles
- 10× 4-pin JST XH receptacles
- 5× 10-pin headers
- 1× 2-4 pin JST XH (power input from Teensy)
- 1× 2-pin JST XH or wire terminal (OE control)

### Cables (Teensy to Level Shifter)

**5 cables, each requires:**
- 1× 12-pin JST XH plug (Teensy end)
- 1× 8-pin JST XH plug (level shifter signals)
- 1× 4-pin JST XH plug (level shifter grounds)
- 12-conductor shielded cable (~6" + 1" split = 7" total)

### Cables (Level Shifter to Z80)

**5 cables, each requires:**
- 1× 8-pin JST XH plug (level shifter signals)
- 1× 4-pin JST XH plug (level shifter grounds)
- 1× 12-pin JST XH plug (Z80 board end)
- 12-conductor shielded cable (~12" + 1" split = 13" total)

---

## Assembly Tips

### Level Shifter Board Assembly Order

1. **Install HW-221 modules** (rows 2-11, 14-23, 26-35, 38-47, 50-59)
   - Orient correctly (pin 1 indicator)
   - Ensure flush seating

2. **Install JST XH receptacles**
   - Column A positions (signal inputs)
   - Column J positions (signal outputs)
   - Ground rail positions (both sides)
   - Check polarity marking

3. **Install logic analyzer headers** (column C positions)
   - 10-pin headers for each module
   - Leave accessible for probe clips

4. **Power rail jumpers** (row 2 for each module)
   - Short solid-core wire (22-24 AWG)
   - Column A → left 3.3V rail
   - Column J → right 5V rail

5. **OE junction wiring** (row 61)
   - Jumpers from rows 11, 23, 35, 47, 59 column D
   - All converge at row 61
   - Add 10kΩ pull-down resistor
   - Wire to OE control connector

6. **Bypass capacitors** (row 62)
   - 220µF electrolytic on 5V rail
   - 100nF ceramics on both rails
   - Keep leads short (<1 cm)

7. **Test points** (row 63)
   - Small wire loops or test point pins
   - 3.3V, 5V, GND access

8. **Power distribution wiring**
   - Heavy gauge wire (22 AWG) along power rails
   - Star ground topology from center point

### Cable Assembly

**For each 12-conductor cable with split end:**

1. **Measure and cut cable**
   - Main run: 6" (Teensy) or 12" (Z80)
   - Split section: 1" from level shifter end

2. **Strip outer jacket** at split point (1")

3. **Separate into two groups:**
   - 8 signal wires → 8-pin JST XH
   - 4 ground wires → 4-pin JST XH

4. **Crimp JST terminals**
   - Use proper crimping tool
   - Test crimp strength (gentle pull test)
   - Insert in correct pin order

5. **Label cables**
   - Heat shrink with module number/signal group
   - Both ends labeled identically

6. **Shield termination**
   - Gather shield strands at level shifter end
   - Solder to ground pin or ring terminal
   - Connect to ground rail on board

### Testing Before Full Assembly

**Per module bring-up:**
1. Install one module only
2. Check power rail continuity (3.3V, 5V, GND)
3. Verify OE control reaches module
4. Connect one cable, test for shorts
5. Power up, check voltage at module pins
6. Test with multimeter: input signal → output signal
7. Once verified, proceed to next module

**Benefits:**
- Isolates issues to single module
- Easier troubleshooting
- Prevents cascading failures
- Builds confidence incrementally

---

## 3D Printed Mounting Plate

### Design Requirements

**Board Spacing:**
- Teensy board: Center position
- Level shifter board: Left or right side
- Z80 board: Opposite side from level shifter
- Spacing: 1-2" between boards for cable routing

**Standoff Height:**
- 0.5-1" above plate
- Allows cable routing underneath
- Provides airflow for heat dissipation
- Access to bottom of boards if needed

**Mounting Scheme:**
- 4 mounting points per board (corners)
- M3 threaded inserts (standard)
- Brass inserts heated into printed plastic
- M3 screws from top secure boards

**Cable Management:**
- Tie-down points along plate edges
- Channel or clips for cable routing
- USB cable strain relief near Teensy
- Keep cables organized and accessible

**Material:**
- PLA or PETG (adequate rigidity)
- 3-4mm wall thickness
- Infill: 20-30% (strong enough)

### Suggested Plate Layout (Top View)

```
┌──────────────────────────────────────────────────────────┐
│                                                          │
│                    [Teensy Board]                        │
│                      (30 rows)                           │
│                          ↓                               │
│                    (USB Cable Entry)                     │
│                          │                               │
│        Cables ←──────────┼──────────→ Cables             │
│          ↓               │               ↓               │
│                          │                               │
│  [Level Shifter Board]   │          [Z80 + LEDs]         │
│       (63 rows)          │           (63 rows)           │
│                          │                               │
│  - 5 HW-221 modules      │      - DIP-40 socket          │
│  - JST connectors        │      - 38 LEDs                │
│  - Logic analyzer        │      - Input connectors       │
│    headers               │                               │
│                          │                               │
│  ○ ○                     │                   ○ ○         │
│    (standoffs)           │             (standoffs)       │
│  ○ ○                     │                   ○ ○         │
│                                                          │
└──────────────────────────────────────────────────────────┘

Dimensions (approximate):
- Plate: 12" × 8" (adjust to fit boards with spacing)
- Standoffs: M3 threaded inserts, 4 per board (12 total)
```

**Orientation Notes:**
- USB cable enters from bottom/side (easy access)
- Level shifters visible and accessible (logic analyzer probing)
- Z80 socket accessible (chip swapping)
- LEDs visible from top (debugging)

---

## Z80 Board and Teensy Board Layouts

**Note:** Detailed layouts for these boards to be documented separately based on similar breadboard-style proto board topology. Key considerations:

**Z80 Board:**
- DIP-40 socket centered, straddling gap
- 5 input connectors (12-pin JST XH from level shifters)
- LED arrays grouped by function
- Bypass capacitors near Z80 socket

**Teensy Board:**
- Teensy 4.1 socketed or soldered
- 5 output connectors (12-pin JST XH to level shifters)
- Power distribution from USB
- Bypass capacitors on 3.3V and VUSB
- OE control output

---

## Summary

**Layout Strategy:**
✅ Breadboard-style proto boards maximize reuse of pre-wired traces  
✅ Linear module arrangement simplifies access and troubleshooting  
✅ Separate signal and ground connectors improves signal integrity  
✅ Modular cable approach allows independent testing and replacement  
✅ 3D printed mounting provides stable, professional assembly  
✅ Incremental bring-up strategy minimizes risk

**Next Steps:**
1. Finalize Z80 and Teensy board layouts
2. Generate detailed wiring diagrams per module
3. Create 3D model for mounting plate
4. Order components (see BOM)
5. Begin assembly with Module 1 (Data Bus)
