# Z80 Board Layout

**Status:** 📋 Planning Complete  
**Date:** December 23, 2025  
**Hardware:** 63-row breadboard-style proto board

---

## Overview

The Z80 board houses the DIP-40 Z80 CPU socket and 38 debug LEDs with current-limiting resistors. All LEDs are powered from the 5V rail (level shifter outputs) and provide real-time visual feedback of bus activity.

**Board Topology:**
- 63 rows × 10 columns (A-J)
- Each row: A-E connected, F-J connected (breadboard-style)
- Power rails on both sides

---

## Layout Summary

**Rows 1-20:** DIP-40 Z80 socket (centered, straddling gap)

**Rows 21-22:** Spacing / bypass capacitors

**Rows 23-59 (Left, A-E):** 19 LEDs (Data bus + Control signals)

**Rows 24-60 (Right, F-J):** 19 LEDs (Address bus + Control signals)

**Rows 61-63:** Input connectors, test points, power distribution

---

## Z80 Socket (Rows 1-20)

**DIP-40 socket straddling center gap:**

```
+=====+===+===+===+===+===+===+===+===+===+===+
| Row | A | B | C | D | E | F | G | H | I | J |
+=====+===+===+===+===+===+===+===+===+===+===+
|   1 |   |   |   | 11|   |   | 30|   |   |   | A11 / RFSH
|   2 |   |   |   | 12|   |   | 29|   |   |   | A12 / GND
|   3 |   |   |   | 13|   |   | 28|   |   |   | A13 / M1
|   4 |   |   |   | 14|   |   | 27|   |   |   | A14 / RESET
|   5 |   |   |   | 15|   |   | 26|   |   |   | A15 / BUSRQ
|   6 |   |   |   | CLK|  |   | WAIT|  |   |   | CLK / WAIT
|   7 |   |   |   | D4|   |   | NMI|   |   |   | D4 / NMI
|   8 |   |   |   | D5|   |   | INT|   |   |   | D5 / INT
|   9 |   |   |   | D3|   |   | BUSAK| |   |   | D3 / BUSAK
|  10 |   |   |   | D6|   |   | WR |   |   |   | D6 / WR
|  11 |   |   |   | Vcc|  |   | GND|   |   |   | +5V / GND
|  12 |   |   |   | D2|   |   | RD |   |   |   | D2 / RD
|  13 |   |   |   | D7|   |   | MREQ|  |   |   | D7 / MREQ
|  14 |   |   |   | D0|   |   | IORQ|  |   |   | D0 / IORQ
|  15 |   |   |   | D1|   |   | HALT|  |   |   | D1 / HALT
|  16 |   |   |   | A0|   |   | A8 |   |   |   | A0 / A8
|  17 |   |   |   | A1|   |   | A9 |   |   |   | A1 / A9
|  18 |   |   |   | A2|   |   | A10|   |   |   | A2 / A10
|  19 |   |   |   | A3|   |   | A7 |   |   |   | A3 / A7
|  20 |   |   |   | A4|   |   | A6 |   |   |   | A4 / A6 (bottom of socket)
+=====+===+===+===+===+===+===+===+===+===+===+

Note: Column D = Z80 pins 1-20, Column G = Z80 pins 21-40
```

**Z80 Pin Mapping (DIP-40):**
- **Left side (Column D):** Pins 1-20
- **Right side (Column G):** Pins 21-40
- **Pin 11 (Column D, Row 11):** Vcc (+5V) - Connect to right power rail
- **Pin 29 (Column G, Row 2):** GND - Connect to ground rails
- **Pin 31 (Column G, Row 11):** GND - Connect to ground rails

---

## LED Array - Left Side (Rows 23-59, Columns A-E)

**19 LEDs for Data Bus and Control Signals**

```
+=====+=================+===============================+
| Row | Signal          | LED Circuit                   |
+=====+=================+===============================+
|  23 | D0              | 5V─[1.5kΩ]─[LED]─Signal       |
|  25 | D1              | 5V─[1.5kΩ]─[LED]─Signal       |
|  27 | D2              | 5V─[1.5kΩ]─[LED]─Signal       |
|  29 | D3              | 5V─[1.5kΩ]─[LED]─Signal       |
|  31 | D4              | 5V─[1.5kΩ]─[LED]─Signal       |
|  33 | D5              | 5V─[1.5kΩ]─[LED]─Signal       |
|  35 | D6              | 5V─[1.5kΩ]─[LED]─Signal       |
|  37 | D7              | 5V─[1.5kΩ]─[LED]─Signal       |
|  39 | RESET           | 5V─[1.5kΩ]─[LED]─Signal       |
|  41 | HALT            | 5V─[1.5kΩ]─[LED]─Signal       |
|  43 | INT             | 5V─[1.5kΩ]─[LED]─Signal       |
|  45 | NMI             | 5V─[1.5kΩ]─[LED]─Signal       |
|  47 | CLK             | 5V─[1.5kΩ]─[LED]─Signal       |
|  49 | M1              | 5V─[1.5kΩ]─[LED]─Signal       |
|  51 | RFSH            | 5V─[1.5kΩ]─[LED]─Signal       |
|  53 | RD              | 5V─[1.5kΩ]─[LED]─Signal       |
|  55 | WR              | 5V─[1.5kΩ]─[LED]─Signal       |
|  57 | MREQ            | 5V─[1.5kΩ]─[LED]─Signal       |
|  59 | WAIT            | 5V─[1.5kΩ]─[LED]─Signal       |
+=====+=================+===============================+
```

**Physical Layout (Left Side):**
- **Column A:** 5V from right power rail (via jumper)
- **Column B:** 1.5kΩ resistor
- **Column C:** LED anode
- **Column D:** LED cathode
- **Column E:** Signal wire from level shifter board (connects to Z80 pin or input connector)

**Even rows (24, 26, 28, etc.):** Empty, used for spacing and wiring access

---

## LED Array - Right Side (Rows 24-60, Columns F-J)

**19 LEDs for Address Bus and Control Signals**

```
+=====+=================+===============================+
| Row | Signal          | LED Circuit                   |
+=====+=================+===============================+
|  24 | A0              | 5V─[1.5kΩ]─[LED]─Signal       |
|  26 | A1              | 5V─[1.5kΩ]─[LED]─Signal       |
|  28 | A2              | 5V─[1.5kΩ]─[LED]─Signal       |
|  30 | A3              | 5V─[1.5kΩ]─[LED]─Signal       |
|  32 | A4              | 5V─[1.5kΩ]─[LED]─Signal       |
|  34 | A5              | 5V─[1.5kΩ]─[LED]─Signal       |
|  36 | A6              | 5V─[1.5kΩ]─[LED]─Signal       |
|  38 | A7              | 5V─[1.5kΩ]─[LED]─Signal       |
|  40 | A8              | 5V─[1.5kΩ]─[LED]─Signal       |
|  42 | A9              | 5V─[1.5kΩ]─[LED]─Signal       |
|  44 | A10             | 5V─[1.5kΩ]─[LED]─Signal       |
|  46 | A11             | 5V─[1.5kΩ]─[LED]─Signal       |
|  48 | A12             | 5V─[1.5kΩ]─[LED]─Signal       |
|  50 | A13             | 5V─[1.5kΩ]─[LED]─Signal       |
|  52 | A14             | 5V─[1.5kΩ]─[LED]─Signal       |
|  54 | A15             | 5V─[1.5kΩ]─[LED]─Signal       |
|  56 | IORQ            | 5V─[1.5kΩ]─[LED]─Signal       |
|  58 | BUSACK          | 5V─[1.5kΩ]─[LED]─Signal       |
|  60 | BUSREQ          | 5V─[1.5kΩ]─[LED]─Signal       |
+=====+=================+===============================+
```

**Physical Layout (Right Side):**
- **Column F:** Signal wire from level shifter board (connects to Z80 pin or input connector)
- **Column G:** LED cathode
- **Column H:** LED anode
- **Column I:** 1.5kΩ resistor
- **Column J:** 5V from right power rail (via jumper)

**Odd rows (25, 27, 29, etc.):** Empty, used for spacing and wiring access

---

## LED Circuit Detail

**Each LED:**
```
5V ──[1.5kΩ]──[LED]──Signal (from level shifter)
                     
When signal HIGH (5V): No current (LED off)
When signal LOW (0V): Current flows, LED on

Wait, this is inverted! Need to reconsider...
```

**Actually, for active-high indication:**
```
Signal (from level shifter) ──[1.5kΩ]──[LED]──GND
                                        
When signal HIGH (5V): (5V - 2V) / 1.5kΩ = 2 mA, LED on ✅
When signal LOW (0V): No current, LED off ✅
```

**Corrected Physical Layout:**

**Left Side (A-E):**
- **Column A:** Signal wire from level shifter board
- **Column B:** 1.5kΩ resistor
- **Column C:** LED anode
- **Column D:** LED cathode  
- **Column E:** GND from left ground rail

**Right Side (F-J):**
- **Column F:** GND from right ground rail
- **Column G:** LED cathode
- **Column H:** LED anode
- **Column I:** 1.5kΩ resistor
- **Column J:** Signal wire from level shifter board

---

## Rows 21-22: Bypass Capacitors

**Row 21-22:**
- 10µF electrolytic capacitor: Vcc (Pin 11) to GND (Pin 29)
- 100nF ceramic capacitor: Vcc (Pin 11) to GND (Pin 29)
- Place physically close to socket pins

**Connections:**
- Positive terminal → Column D or G (Vcc at row 11)
- Negative terminal → Ground rail
- Keep leads short (<1 cm)

---

## Rows 61-63: Connectors and Power

### Row 61-62: Input Connectors

**5× 12-pin JST XH connectors from level shifter board:**

| Module | Signals                          | Connector Location        |
|--------|----------------------------------|---------------------------|
| 1      | Data Bus (D0-D7) + 4 GND        | Row 61, mixed columns     |
| 2      | Address Low (A0-A7) + 4 GND     | Row 61, mixed columns     |
| 3      | Address High (A8-A15) + 4 GND   | Row 62, mixed columns     |
| 4      | Control In (8 signals) + 4 GND  | Row 62, mixed columns     |
| 5      | Control Out (6 signals) + 4 GND | Row 62, mixed columns     |

**Signal routing:**
- Each signal goes to:
  1. Corresponding Z80 socket pin (rows 1-20)
  2. Corresponding LED row (rows 23-60)
- Use breadboard-style traces where possible
- Jumper wires as needed for routing

### Row 63: Power Distribution and Test Points

**Power Input:**
- 5V from level shifter board (powers LEDs and Z80)
- Multiple ground connections
- Distribute via power rails on both sides

**Test Points:**
- 5V test point
- Ground test points (multiple)
- Key signal test points (optional)

---

## Signal Routing

### Direct Connections (Z80 Socket ↔ LEDs)

Signals that go directly from Z80 socket to LED, no connector needed:

**Left Side:**
- D0-D7: Socket rows 7-8, 10, 12-15 → LED rows 23, 25, 27, 29, 31, 33, 35, 37

**Right Side:**
- A0-A7: Socket rows 16-19, 20 → LED rows 24, 26, 28, 30, 32, 34, 36, 38
- A8-A15: Socket rows 1, 3-5, 16, 17, 18, 19, 20 → LED rows 40, 42, 44, 46, 48, 50, 52, 54

### Connector Input Signals (Level Shifter → Z80 + LED)

Signals from level shifter board that must split to socket and LED:

**From Module 4 (Control Inputs, Z80 → Teensy):**
- HALT, MREQ, IORQ, RD, WR, BUSACK, M1, RFSH
- These are outputs from Z80, need to tap at socket
- Run to connector for level shifter board

**From Module 5 (Control Outputs, Teensy → Z80):**
- CLK, INT, NMI, WAIT, BUSREQ, RESET
- These come from level shifter board
- Split to Z80 socket pin and LED

**Routing strategy:**
- Control inputs: T-junction at socket pin → to LED and to output connector
- Control outputs: From input connector → T-junction to socket pin and LED

---

## Power Budget (Z80 Board)

**5V Rail Loads:**

| Component           | Current (typ) | Current (peak) | Notes                      |
|---------------------|---------------|----------------|----------------------------|
| Z80 CPU (NMOS)      | 200 mA        | 250 mA         | Worst case old NMOS        |
| 38 LEDs @ 2 mA      | 24 mA (avg)   | 76 mA          | ~30% duty cycle typical    |
| **Total**           | **224 mA**    | **326 mA**     | Well within 500 mA budget  |

**Ground connections:** Multiple ground wires from connectors to both ground rails

---

## Assembly Order

1. **Install DIP-40 socket** (rows 1-20)
   - Use high-quality socket (machined pin recommended)
   - Ensure proper orientation (pin 1 indicator)
   - Solder carefully, check for bridges

2. **Install bypass capacitors** (rows 21-22)
   - 10µF electrolytic + 100nF ceramic
   - Short leads, close to Vcc/GND pins

3. **Install LEDs** (rows 23-60, every other row)
   - Orient correctly (anode/cathode)
   - Consider LED holders or sockets for easier replacement
   - Color scheme: Data bus=yellow, Address bus=green, Control signals=blue

4. **Install current-limiting resistors** (adjacent to LEDs)
   - 1.5kΩ, 1/4W
   - Match to corresponding LED row

5. **Install input connectors** (rows 61-62)
   - 5× 12-pin JST XH receptacles
   - Check pinout matches level shifter board outputs

6. **Power distribution wiring**
   - 5V from connector to power rails
   - Multiple ground connections

7. **Signal routing jumpers**
   - Z80 socket pins to LEDs
   - Z80 socket pins to output connectors (control inputs)
   - Input connectors to Z80 pins and LEDs (control outputs)

8. **Test points** (row 63)
   - Wire loops or test pins for key signals

---

## Testing Strategy

### Power-Up Test (No Z80 installed)
1. Connect 5V and ground from level shifter board
2. Measure voltage at socket pins: Pin 11 should be 5V, Pins 29/31 should be 0V
3. Check for shorts between Vcc and GND
4. Verify power rail continuity

### LED Test
1. With power applied, manually connect LED signal lines to 5V or GND
2. Each LED should light when signal pulled HIGH (5V)
3. Verify correct resistor values (measure LED current ~2 mA)

### Z80 Socket Test
1. Install Z80 (power off first!)
2. Connect all signal cables from level shifter board
3. Power up, observe:
   - CLK LED should flash at clock rate
   - RESET LED should be lit initially, then off after release
   - Other LEDs will show activity based on Z80 operation

### Full System Test
1. Run simple Z80 test program
2. Observe LED patterns:
   - Address bus should show incrementing pattern
   - Data bus should show instruction/data bytes
   - Control signals should pulse appropriately

---

## Component List (Z80 Board)

| Component               | Quantity | Notes                                    |
|-------------------------|----------|------------------------------------------|
| DIP-40 socket           | 1        | Machined pin recommended                 |
| LEDs (5mm or 3mm)       | 38       | Various colors optional                  |
| 1.5kΩ resistors (1/4W)  | 38       | Current limiting                         |
| 10µF electrolytic cap   | 1        | Bypass capacitor                         |
| 100nF ceramic cap       | 1        | Bypass capacitor                         |
| 12-pin JST XH receptacle| 5        | Input connectors from level shifter      |
| 22 AWG solid wire       | ~2 feet  | Power distribution, signal jumpers       |
| Proto board (63 rows)   | 1        | Breadboard-style with power rails        |

---

## Summary

**Z80 Board Features:**
✅ DIP-40 socket for easy Z80 swapping  
✅ 38 LEDs for comprehensive bus visualization  
✅ Organized LED layout (data/control left, address/control right)  
✅ Even-row spacing provides physical clearance and wiring access  
✅ Bypass capacitors close to Z80 socket  
✅ 5 input connectors from level shifter board  
✅ Direct signal tapping from Z80 pins where possible  
✅ Power budget: 224 mA typical, 326 mA peak (within limits)  

**Visual Feedback:**
- **Data bus (D0-D7):** 8 yellow LEDs showing byte values
- **Address bus (A0-A15):** 16 green LEDs showing memory/IO addresses
- **Control signals:** 14 blue LEDs showing CPU state (clock, interrupts, memory cycles, etc.)

This provides real-time insight into Z80 operation without needing a logic analyzer for basic debugging!
