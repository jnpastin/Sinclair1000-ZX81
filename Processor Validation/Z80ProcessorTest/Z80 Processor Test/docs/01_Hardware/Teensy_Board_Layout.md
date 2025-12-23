# Teensy Board Layout

**Status:** 📋 Planning Complete  
**Date:** December 23, 2025  
**Hardware:** 30-row breadboard-style proto board

---

## Overview

The Teensy board houses the Teensy 4.1 microcontroller and provides power distribution and signal routing to the level shifter board. Power comes from USB, and the board distributes both 5V (VUSB) and 3.3V to downstream boards.

**Board Topology:**
- 30 rows × 10 columns (A-J)
- Each row: A-E connected, F-J connected (breadboard-style)
- Power rails on both sides (used for ground and signal distribution)

---

## Layout Summary

**Rows 1-6:** Power distribution, bypass capacitors, power output connectors

**Rows 7-30:** Teensy 4.1 (24 rows/pins)

**Rows 25-30 (bottom area):** Signal output connectors to level shifter board

**USB connector:** At top edge (row 7), accessible for cable connection

---

## Teensy 4.1 Placement (Rows 7-30)

**Teensy pin arrangement:**
- 24 pins per side (48 total on main headers)
- Wider than standard DIP - occupies 4 columns

```
+=====+===+===+===+====+===+===+====+===+===+===+
| Row | A | B | C | D  | E | F | G  | H | I | J |
+=====+===+===+===+====+===+===+====+===+===+===+
|   7 |   |   | * | Pin|   |   | Pin| * |   |   | USB end (GND, Pin 0)
|   8 |   |   | * | Pin|   |   | Pin| * |   |   |
|   9 |   |   | * | Pin|   |   | Pin| * |   |   |
| ... |   |   | * | Pin|   |   | Pin| * |   |   | (continues for 24 rows)
|  30 |   |   | * | Pin|   |   | Pin| * |   |   | Bottom end (Pin 23, VUSB)
+=====+===+===+===+====+===+===+====+===+===+===+

Teensy pins occupy:
- Column C: Outer left pins
- Column D: Inner left pins (main header)
- Column G: Inner right pins (main header)
- Column H: Outer right pins

Exposed columns:
- Left side: A, B (2 columns for routing)
- Right side: I, J (3 columns for routing)
```

**Orientation:**
- USB connector at top (row 7) facing board edge
- VUSB pin at bottom (row 30)
- Pin 0 (top), Pin 23 (bottom) on left side
- Pins 24-47 run top to bottom on right side

**Key Power Pins:**
- **VUSB (row 30):** 5V from USB - source for all 5V power
- **3.3V (row ~20):** Regulated 3.3V output - source for 3.3V power
- **GND (multiple):** Ground connections throughout
- **VIN (row 30):** Not used (we're USB-powered)

---

## Rows 1-6: Power Distribution

### Row 1-2: Power Output Connectors

**Main power connector to level shifter board:**
- 6-pin JST XH connector
- Pinout:
  - Pin 1: VUSB (5V)
  - Pin 2: VUSB (5V) - duplicate for current capacity
  - Pin 3: 3.3V
  - Pin 4: GND
  - Pin 5: GND
  - Pin 6: GND

**Alternative: Separate connectors**
- 3-pin JST XH for 5V + GND
- 3-pin JST XH for 3.3V + GND

### Row 3: OE Control Output

**OE control connector:**
- 2-pin JST XH or single wire with pin header
- Pinout:
  - Pin 1: Teensy Pin 42 (OE control signal)
  - Pin 2: GND (reference)

### Row 4-5: Bypass Capacitors

**VUSB bypass:**
- 10µF electrolytic: VUSB to GND
- 100nF ceramic: VUSB to GND
- Located near row 30 (VUSB pin)

**3.3V bypass:**
- 10µF electrolytic: 3.3V pin to GND
- 100nF ceramic: 3.3V pin to GND
- Located near 3.3V pin (~row 20)

### Row 6: Test Points

- VUSB test point
- 3.3V test point
- Ground test points

---

## Power Routing

### VUSB (5V) Distribution

**From Teensy VUSB pin (row 30):**
```
VUSB pin (row 30, column D or G)
    ↓
    ├─→ Bypass caps (10µF + 100nF)
    ├─→ Power output connector (row 1-2)
    └─→ Test point (row 6)
```

**Wiring:**
- Heavy gauge wire (22 AWG) from VUSB pin to connector
- Keep trace short and direct
- Multiple ground returns in parallel

### 3.3V Distribution

**From Teensy 3.3V pin (~row 20):**
```
3.3V pin (row 20, column D or G)
    ↓
    ├─→ Bypass caps (10µF + 100nF)
    ├─→ Power output connector (row 1-2)
    └─→ Test point (row 6)
```

**Wiring:**
- Wire from 3.3V pin to connector
- 250 mA capacity (adequate for level shifter VCCA loads)

### Ground Distribution

**Multiple ground connections:**
- Teensy has GND pins at rows 7, 13, 17, 21, 30 (approximate)
- Connect all to common ground net
- Ground to both power rails for distribution
- Multiple ground pins in power connector

**Ground routing:**
- Star topology from Teensy board
- Avoid ground loops
- Heavy gauge wire or multiple parallel wires

---

## Rows 25-30: Signal Output Connectors

### Output Connectors to Level Shifter Board

**5× 12-pin JST XH connectors for signal groups:**

**Connector 1: Data Bus**
- 8 signals: Teensy pins 6-13 (D0-D7)
- 4 grounds

**Connector 2: Address Low Byte**
- 8 signals: Teensy pins 14-21 (A0-A7)
- 4 grounds

**Connector 3: Address High Byte**
- 8 signals: Teensy pins 0-1, 22-27 (A8-A15)
- 4 grounds

**Connector 4: Control Inputs**
- 8 signals: Teensy pins 28-31, 37-40 (HALT, MREQ, IORQ, RD, WR, BUSACK, M1, RFSH)
- 4 grounds

**Connector 5: Control Outputs + Clock**
- 6 signals: Teensy pins 36, 2-5, 41 (CLK, INT, NMI, WAIT, BUSREQ, RESET)
- 4 grounds (or use 10-pin connector: 6 signals + 4 grounds)

**Connector placement:**
- Arranged along bottom rows (25-30)
- Can use columns A-B or I-J (exposed areas)
- Or place in gap rows between Teensy and edge

### Signal Routing

**From Teensy pins to connectors:**
- Teensy pin in column C, D, G, or H
- Route via breadboard traces to connector location
- Use jumper wires where needed
- Keep traces short and organized

**Example: Data Bus (Teensy pins 6-13)**
- Pin 6 at row ~14, column D → Route to connector
- Pin 7 at row ~15, column D → Route to connector
- Etc.

**Ground distribution:**
- 1 ground per 2-3 signal wires in each cable
- Total: 20 ground wires needed (5 connectors × 4 grounds)
- Connect to Teensy ground pins or ground rails

---

## Cable Connections

### From Teensy Board to Level Shifter Board (5 cables)

**Each signal cable:**
- 12-conductor shielded cable (~6" length)
- Teensy end: 12-pin JST XH connector (rows 25-30)
- Level shifter end: Splits into 8-pin + 4-pin JST XH
- Shield grounded at level shifter board only

**Power cable:**
- From power connector (row 1-2)
- To level shifter board power input
- Heavy gauge for VUSB (5V) - 22 AWG recommended
- Multiple ground wires in parallel

**OE control:**
- From OE connector (row 3)
- To level shifter board row 61 (OE junction)
- Can be 2-conductor cable or single wire with ground reference

---

## Physical Layout Considerations

### Teensy Orientation

**Right side (I, J, right power rail) has 3 exposed columns:**
- Column J adjacent to right power rail
- Good for VUSB distribution taps
- Easy access to 5V routing

**Left side (A, B, left power rail) has 2 exposed columns:**
- Column A adjacent to left power rail
- Good for 3.3V distribution
- Ground distribution access

**USB connector at top:**
- USB cable enters from top edge of board
- Keep accessible for connecting/disconnecting
- Consider strain relief or cable tie-down

### Connector Placement Strategy

**Option A: Bottom edge (rows 25-30)**
- All 5 signal connectors along bottom
- Cables route down/away from Teensy
- Clean, organized appearance

**Option B: Side columns (A-B or I-J)**
- Use exposed columns for connector mounting
- Vertical connector arrangement
- More compact, but harder to access

**Recommendation: Bottom edge placement** for easier cable management

---

## Assembly Order

1. **Mark Teensy position** (rows 7-30, columns C-D-G-H)
   - Double-check orientation (USB at top)
   - Verify pin 1 location

2. **Install power output connector** (row 1-2)
   - 6-pin JST XH or separate 3-pin connectors
   - Check pinout matches level shifter board input

3. **Install OE control connector** (row 3)
   - 2-pin JST XH or pin header
   - Connect to Pin 42 routing

4. **Install signal output connectors** (rows 25-30)
   - 5× 12-pin JST XH receptacles
   - Space appropriately for cable access
   - Label each connector (Module 1-5)

5. **Solder Teensy 4.1**
   - Option A: Direct solder (permanent)
   - Option B: Use machine-pin sockets (removable)
   - Ensure proper orientation
   - Check for solder bridges

6. **Power routing jumpers**
   - VUSB pin to power connector
   - 3.3V pin to power connector
   - Multiple grounds to ground net

7. **Install bypass capacitors**
   - Near VUSB pin (row 30): 10µF + 100nF
   - Near 3.3V pin: 10µF + 100nF
   - Short leads, close to pins

8. **Signal routing**
   - Teensy GPIO pins to output connectors
   - Organize wiring for clarity
   - Use solid-core wire (22-24 AWG)

9. **Test points** (row 6)
   - Wire loops or test pins
   - VUSB, 3.3V, GND access

---

## Testing Procedure

### Pre-Power Test
1. Visual inspection: Check for solder bridges, cold joints
2. Continuity test: Verify all connections with multimeter
3. Shorts test: Check for shorts between power rails and ground
4. Pinout verification: Confirm connector pinouts match documentation

### Initial Power-Up (No cables connected)
1. Connect USB cable
2. Measure voltages:
   - VUSB pin should be ~5V
   - 3.3V pin should be ~3.3V
   - All grounds at 0V
3. Check for excessive current draw (should be <100 mA idle)
4. Teensy should enumerate as USB device
5. Verify power output connector voltages

### With Cables Connected
1. Power off, connect cables to level shifter board
2. Power up USB
3. Check voltages at level shifter board
4. Verify signal continuity (Teensy GPIO to level shifter inputs)
5. Upload test program to Teensy
6. Verify signal output with multimeter or logic analyzer

---

## Component List (Teensy Board)

| Component                | Quantity | Notes                                  |
|--------------------------|----------|----------------------------------------|
| Teensy 4.1               | 1        | Main microcontroller                   |
| 12-pin JST XH receptacle | 5        | Signal outputs                         |
| 6-pin JST XH receptacle  | 1        | Power output (or 2× 3-pin)            |
| 2-pin JST XH receptacle  | 1        | OE control output                      |
| 10µF electrolytic cap    | 2        | Bypass (VUSB, 3.3V)                   |
| 100nF ceramic cap        | 2        | Bypass (VUSB, 3.3V)                   |
| 22 AWG solid wire        | ~3 feet  | Power distribution, signal routing     |
| Machine-pin sockets      | 2× 24-pin| Optional, for removable Teensy         |
| Proto board (30 rows)    | 1        | Breadboard-style with power rails      |

---

## Pin Assignments Reference

**Quick reference for connector wiring:**

**Module 1 (Data Bus):** Teensy pins 6-13  
**Module 2 (Address Low):** Teensy pins 14-21  
**Module 3 (Address High):** Teensy pins 0-1, 22-27  
**Module 4 (Control In):** Teensy pins 28-31, 37-40  
**Module 5 (Control Out):** Teensy pins 36, 2-5, 41  
**OE Control:** Teensy pin 42  

See [Pin_Mapping.md](Pin_Mapping.md) for complete signal-to-pin mapping.

---

## Power Budget Summary

**USB Input:** 500 mA max (USB 2.0)

**Teensy Board Consumption:**
- Teensy 4.1: ~100 mA (normal operation)
- Teensy draws from USB directly

**Power Output to Other Boards:**
- 5V (VUSB): ~400 mA available
  - Level shifter 5V: 250 mA
  - Z80 board 5V: remainder
- 3.3V: 150 mA actual / 250 mA max capacity
  - Level shifter 3.3V: 150 mA

**Margin:** Adequate for full system operation

---

## Summary

**Teensy Board Features:**
✅ Simple 30-row layout with Teensy 4.1  
✅ USB-powered (VUSB distribution)  
✅ 5V and 3.3V power output to downstream boards  
✅ 5 signal output connectors (12-pin each)  
✅ OE control output  
✅ Bypass capacitors for clean power  
✅ Test points for debugging  
✅ Compact design with good access to all connections  

**Key Design Points:**
- VUSB pin sources all 5V power (not VIN)
- 3.3V pin can supply up to 250 mA
- Multiple grounds for proper distribution
- Bottom-mounted connectors for clean cable routing
- USB connector accessible at top edge
- Removable Teensy option via machine-pin sockets

This board acts as the central power and signal distribution hub for the entire system!
