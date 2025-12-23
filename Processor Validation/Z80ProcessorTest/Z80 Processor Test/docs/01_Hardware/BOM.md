# Bill of Materials (BOM)

**Status:** 📋 Planning Complete  
**Date:** December 23, 2025  
**Project:** Z80 Processor Validation - Teensy 4.1 Hardware

---

## Overview

Complete component list for three-board system: Level Shifter Board, Z80 Board, and Teensy Board.

---

## Main Components

| Component                | Quantity | Notes                                      |
|--------------------------|----------|--------------------------------------------|
| Teensy 4.1               | 1        | IMXRT1062 @ 600 MHz                        |
| HW-221 Level Shifter     | 5        | TXS0108E 8-channel bidirectional           |
| Z80 CPU (DIP-40)         | 1+       | NMOS or CMOS, suggest having 2-3 for testing |
| Proto boards (63 row)    | 2        | Breadboard-style with power rails          |
| Proto board (30 row)     | 1        | Half-size breadboard-style                 |

---

## Passive Components

### Capacitors

| Type                     | Value  | Voltage | Quantity | Notes                              |
|--------------------------|--------|---------|----------|------------------------------------|
| Electrolytic (radial)    | 10µF   | 16V+    | 5        | Bypass caps (3× Teensy, 1× Z80, 1× spare) |
| Electrolytic (radial)    | 220µF  | 16V+    | 1        | Level shifter board 5V bulk        |
| Ceramic (multilayer)     | 100nF  | 25V+    | 10       | High-freq bypass (includes extras) |

**Capacitor allocation:**
- **Teensy board:** 2× 10µF + 2× 100nF (VUSB and 3.3V)
- **Z80 board:** 1× 10µF + 1× 100nF (Z80 Vcc)
- **Level shifter board:** 1× 220µF + 2× 100nF (5V and 3.3V rails)
- **Spares:** 2× 10µF, 5× 100nF

### Resistors

| Type           | Value | Power  | Quantity | Notes                           |
|----------------|-------|--------|----------|---------------------------------|
| Carbon film    | 1.5kΩ | 1/4W   | 40       | LED current limiting (includes extras)|
| Carbon film    | 10kΩ  | 1/4W   | 1        | OE pull-down                    |

---

## Connectors

### JST XH Connectors (2.5mm pitch)

**Level Shifter Board:**

| Type          | Pins | Quantity | Notes                                    |
|---------------|------|----------|------------------------------------------|
| Receptacle    | 8    | 10       | Signal connectors (5× A-side, 5× B-side)|
| Receptacle    | 4    | 10       | Ground connectors (5× A-side, 5× B-side)|
| Receptacle    | 6    | 1        | Power input from Teensy                  |
| Receptacle    | 2    | 1        | OE control input                         |

**Z80 Board:**

| Type          | Pins | Quantity | Notes                                    |
|---------------|------|----------|------------------------------------------|
| Receptacle    | 12   | 5        | Signal inputs from level shifter         |

**Teensy Board:**

| Type          | Pins | Quantity | Notes                                    |
|---------------|------|----------|------------------------------------------|
| Receptacle    | 12   | 5        | Signal outputs to level shifter          |
| Receptacle    | 6    | 1        | Power output                             |
| Receptacle    | 2    | 1        | OE control output                        |

**JST XH Plug Housings (for cables):**

| Type          | Pins | Quantity | Notes                                    |
|---------------|------|----------|------------------------------------------|
| Plug housing  | 12   | 10       | Cable ends (Teensy→LS, LS→Z80)          |
| Plug housing  | 8    | 10       | Signal cables (split at LS board)        |
| Plug housing  | 4    | 10       | Ground cables (split at LS board)        |
| Plug housing  | 6    | 1        | Power cable                              |
| Plug housing  | 2    | 1        | OE control                               |

**JST XH Crimp Terminals:**

| Type                 | Quantity | Notes                           |
|----------------------|----------|---------------------------------|
| Female crimp pins    | 200      | Bulk quantity, allows for mistakes/spares |

---

## Headers and Sockets

| Component                | Quantity | Notes                                |
|--------------------------|----------|--------------------------------------|
| 10-pin male header       | 5        | Logic analyzer headers (LS board)    |
| DIP-40 socket            | 1        | Z80 socket (machined pin preferred)  |
| 24-pin machine-pin socket| 2        | Optional: Removable Teensy mounting  |
| Test point pins          | 10       | Optional: Various test points        |

---

## LEDs

| Type               | Color        | Size | Quantity | Notes                        |
|--------------------|--------------|------|----------|------------------------------|
| Standard LED       | Yellow       | 5mm  | 8        | Data bus                     |
| Standard LED       | Green        | 5mm  | 16       | Address bus                  |
| Standard LED       | Blue         | 5mm  | 14       | Control signals              |

**LED color scheme:**
- **Yellow:** Data bus (D0-D7) - 8 LEDs
- **Green:** Address bus (A0-A15) - 16 LEDs
- **Blue:** Control signals (CLK, INT, NMI, WAIT, BUSREQ, RESET, HALT, M1, RFSH, RD, WR, MREQ, IORQ, BUSACK) - 14 LEDs

---

## Wire and Cable

| Type                      | Gauge    | Quantity | Notes                             |
|---------------------------|----------|----------|-----------------------------------|
| 15-conductor shielded cable| -       | 10 feet  | Main interconnect cable           |
| Solid-core wire (spools)  | 22 AWG   | 1 spool  | Breadboard jumpers, power routing |
| Solid-core wire (spools)  | 24 AWG   | 1 spool  | Signal routing                    |

**Cable breakdown:**
- Teensy → Level Shifter: 5 cables × ~7" = ~3 feet
- Level Shifter → Z80: 5 cables × ~13" = ~6 feet
- Power cables: ~1 foot
- **Total needed:** ~10 feet of 15-conductor shielded cable

---

## Mounting Hardware

### 3D Printed Base Plate

| Component                 | Quantity | Notes                              |
|---------------------------|----------|------------------------------------|
| M3 threaded inserts       | 12       | 4 per board × 3 boards             |
| M3 × 10mm screws          | 12       | Board mounting                     |
| 3D printer filament (PLA) | ~50g     | For base plate (~8" × 12")        |

---

## Optional Components

| Component                 | Quantity | Notes                              |
|---------------------------|----------|------------------------------------|
| ZIF socket (DIP-40)       | 1        | For frequent Z80 swapping          |
| LED holders (3mm/5mm)     | 40       | Makes LED replacement easier       |
| Heat shrink tubing        | Assorted | Cable labeling and strain relief   |
| Cable ties                | 20+      | Cable management                   |

---

## Component Summary by Board

### Level Shifter Board
- 5× HW-221 modules
- 10× 8-pin JST XH receptacles
- 10× 4-pin JST XH receptacles
- 1× 6-pin JST XH receptacle (power in)
- 1× 2-pin JST XH receptacle (OE control)
- 5× 10-pin headers (logic analyzer)
- 1× 220µF electrolytic capacitor
- 2× 100nF ceramic capacitors
- 1× 10kΩ resistor
- Proto board (63 rows)

### Z80 Board
- 1× DIP-40 socket
- 5× 12-pin JST XH receptacles
- 38× LEDs (8 yellow, 16 green, 14 blue)
- 38× 1.5kΩ resistors
- 1× 10µF electrolytic capacitor
- 1× 100nF ceramic capacitor
- Proto board (63 rows)

### Teensy Board
- 1× Teensy 4.1
- 5× 12-pin JST XH receptacles
- 1× 6-pin JST XH receptacle (power out)
- 1× 2-pin JST XH receptacle (OE control)
- 2× 10µF electrolytic capacitors
- 2× 100nF ceramic capacitors
- 2× 24-pin machine-pin sockets (optional)
- Proto board (30 rows)

### Cables
- 10× 12-pin JST XH plug housings
- 10× 8-pin JST XH plug housings
- 10× 4-pin JST XH plug housings
- 1× 6-pin JST XH plug housing
- 1× 2-pin JST XH plug housing
- 200× JST XH crimp terminals
- ~10 feet 15-conductor shielded cable
- Solid-core wire (22 & 24 AWG)

---

## Assembly Notes

**Quality Considerations:**
- **Machined-pin DIP sockets** preferred over tin-plated for Z80 (better contact, longer life)
- **Quality capacitors** (Panasonic, Nichicon) better than generic for bypass applications
- **Shielded cable** essential for signal integrity at 5 MHz

**Future Expansion:**
- Consider having extra proto boards for future mods
- Extra Z80 CPUs useful for testing different variants
- Spare level shifters for repairs/expansion

---

## Summary

Complete three-board system for Z80 processor validation with Teensy 4.1 controller, featuring:
- 5V/3.3V level shifting for full Z80 compatibility
- 38 LED debug display (yellow=data, green=address, blue=control)
- Logic analyzer headers for detailed signal inspection
- Modular design with removable cables
- 3D printed mounting system

The system provides hands-on learning about hardware interfacing, signal integrity, and embedded systems!
