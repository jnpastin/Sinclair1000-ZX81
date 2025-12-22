# Getting Started with Teensy 4.1 Migration

**Last Updated:** December 22, 2025  
**Status:** 📝 Draft

---

## Prerequisites

### Hardware Requirements
- [ ] Teensy 4.1-NE-Lockable board
- [ ] Z80 CPU (40-pin DIP)
- [ ] Bidirectional level shifters (3.3V ↔ 5V)
  - Recommended: TXS0108E or similar (8-channel, bidirectional)
  - Quantity: ~6 chips (for 40+ signals)
- [ ] Breadboard or perfboard for prototyping
- [ ] Power supply (5V for Z80, 3.3V for Teensy)
- [ ] USB cable (Micro-B or USB-C depending on Teensy variant)
- [ ] Logic analyzer (recommended for debugging)
- [ ] Oscilloscope (recommended for signal verification)
- [ ] Multimeter for continuity and voltage testing

### Software Requirements
- [ ] PlatformIO IDE (or PlatformIO Core)
- [ ] Visual Studio Code (if using PlatformIO IDE)
- [ ] Git for version control
- [ ] Teensy Loader (comes with Teensyduino)
- [ ] Serial terminal program (PuTTY, minicom, or Arduino Serial Monitor)

### Knowledge Requirements
- Basic understanding of Z80 architecture
- Familiarity with Arduino/embedded C++
- Understanding of digital logic and voltage levels
- Experience with logic analyzers (helpful)

---

## Development Environment Setup

### 1. Install PlatformIO

**Option A: PlatformIO IDE (Recommended)**
```bash
# Install VS Code
# Visit: https://code.visualstudio.com/

# Install PlatformIO extension
# 1. Open VS Code
# 2. Go to Extensions (Cmd+Shift+X)
# 3. Search for "PlatformIO IDE"
# 4. Click Install
```

**Option B: PlatformIO Core**
```bash
# Install via Python pip
pip install platformio

# Verify installation
pio --version
```

### 2. Clone Project Repository

```bash
cd ~/git-repos/Sinclair\ 1000\:ZX81/Processor\ Validation/Z80ProcessorTest/
git status  # Verify you're in the correct repo
```

### 3. Install Teensy Platform

```bash
# PlatformIO will auto-install on first build, or manually:
pio platform install teensy
```

### 4. Test Build (Initial)

```bash
cd "Z80 Processor Test"
pio run -e teensy41

# This will likely fail initially - that's expected!
# We need to port the code first.
```

---

## Hardware Setup

### Understanding Voltage Levels

| Component | Operating Voltage | Logic Levels |
|-----------|------------------|--------------|
| Z80 CPU | 5V | 5V TTL |
| Teensy 4.1 | 3.3V | 3.3V CMOS |
| Level Shifter | 3.3V + 5V | Bidirectional |

⚠️ **CRITICAL:** Never connect 5V signals directly to Teensy 4.1 GPIO pins!

### Level Shifter Configuration

```
Z80 (5V) ←→ Level Shifter ←→ Teensy 4.1 (3.3V)
  │              │                    │
  ├─ VCC (5V)    ├─ VCCB (5V)        │
  │              ├─ VCCA (3.3V) ─────┤
  ├─ Data[0-7]   ├─ B[1-8] (5V)      │
  │              ├─ A[1-8] (3.3V) ───┤── GPIO pins
  ├─ Addr[0-15]  ├─ OE (GND)         │
  └─ GND ────────┴─ GND ──────────────┘
```

### Initial Test Setup

1. **Power Verification**
   - Connect Teensy to USB
   - Measure VIN pin (should be ~5V from USB)
   - Measure 3.3V pin (onboard regulator output)
   - Verify GND connections

2. **Level Shifter Test**
   - Power level shifter with 3.3V and 5V
   - Use multimeter to verify both voltage rails
   - Test one channel with simple digital signal

3. **Z80 Power Test**
   - Apply 5V to Z80 VCC (pin 11)
   - Connect GND (pin 29)
   - Measure voltage at Z80 VCC pin
   - Verify no excessive current draw

---

## Project Structure

```
Z80 Processor Test/
├── platformio.ini          # Build configuration (needs updating)
├── src/
│   └── main.cpp           # Empty placeholder (will migrate here)
├── include/               # Header files
├── lib/                   # Custom libraries
├── test/                  # Unit tests
├── docs/                  # This documentation
│   ├── 01_Hardware/      # Pin mapping, GPIO, level shifting
│   ├── 02_Clock/         # Timer and clock generation
│   ├── 03_Bus_IO/        # Bus I/O implementation
│   ├── 04_Interrupts/    # ISR and interrupt handling
│   ├── 05_Memory/        # Memory management
│   ├── 06_Timing/        # Timing analysis
│   ├── 07_Build/         # Build system
│   ├── 08_Testing/       # Test procedures
│   ├── Reference/        # Reference materials
│   └── Development/      # Development logs
└── Arduino Mega version/ # Original source code
    └── Z80_Validation/   # Arduino Mega implementation
```

---

## Migration Workflow

### Phase 1: Setup and Planning (You Are Here)
1. ✅ Read this document
2. ✅ Review Migration_Plan.md
3. ⬜ Gather hardware components
4. ⬜ Set up development environment
5. ⬜ Review Arduino Mega source code

### Phase 2: Hardware Mapping
1. ⬜ Read 01_Hardware documentation
2. ⬜ Create pin mapping spreadsheet
3. ⬜ Design level shifter circuit
4. ⬜ Build hardware prototype

### Phase 3: Software Porting
1. ⬜ Follow phases 2-6 in Migration_Plan.md
2. ⬜ Implement each module incrementally
3. ⬜ Test as you go

### Phase 4: Integration and Testing
1. ⬜ Follow Phase 8 testing procedures
2. ⬜ Validate with logic analyzer
3. ⬜ Run comprehensive test suite

---

## Quick Reference

### Important Files (Arduino Mega)
- `Z80_Validation.ino` - Main program
- `pins.h` - Pin definitions (needs complete rewrite)
- `hal.h/hal.cpp` - Hardware abstraction (needs porting)
- `clock.h/clock.cpp` - Clock generation (needs porting)
- `isr_test_programs.cpp` - ISR implementation (needs porting)
- `menu.cpp` - User interface (minimal changes needed)

### Key Documentation (Arduino Mega)
- `Arduino Mega version/docs/README.md` - Original docs index
- `Arduino Mega version/docs/ISR_Proposal.md` - ISR design
- `Arduino Mega version/docs/HAL_Proposal.md` - HAL architecture
- `Arduino Mega version/docs/progress.md` - Original development log

### Teensy 4.1 Resources
- Teensy 4.1 Pinout: https://www.pjrc.com/teensy/pinout.html
- Teensy Documentation: https://www.pjrc.com/teensy/techspecs.html
- IMXRT1062 Reference Manual: NXP website
- PlatformIO Teensy: https://docs.platformio.org/en/latest/boards/teensy/teensy41.html

---

## Common Commands

```bash
# Build project
pio run

# Upload to Teensy
pio run -t upload

# Open serial monitor
pio device monitor

# Clean build
pio run -t clean

# Build with verbose output
pio run -v

# List available boards
pio boards teensy
```

---

## Troubleshooting

### Build Issues
- **Error:** "Platform 'teensy' not installed"
  - **Solution:** Run `pio platform install teensy`

- **Error:** "No such file or directory: pins.h"
  - **Solution:** This is expected - the file needs to be created during migration

### Hardware Issues
- **Problem:** Teensy not recognized by computer
  - **Solution:** Press the program button on Teensy, check USB cable

- **Problem:** Level shifter not working
  - **Solution:** Verify both voltage rails, check OE pin is grounded

### Upload Issues
- **Error:** "Teensy Loader unable to open port"
  - **Solution:** Close Arduino IDE or other programs using the serial port

---

## Next Steps

1. Review the [Migration_Plan.md](Migration_Plan.md) document
2. Read through Phase 1 documentation: [01_Hardware/](01_Hardware/)
3. Start with pin mapping analysis
4. Proceed through migration phases sequentially

---

## Getting Help

- Review existing documentation in `docs/` folder
- Check `Development/Troubleshooting.md` for common issues
- Refer to original Arduino Mega implementation for reference
- Consult Teensy forums: https://forum.pjrc.com/
- PlatformIO community: https://community.platformio.org/

---

**Ready to begin?** → [Migration_Plan.md](Migration_Plan.md)
