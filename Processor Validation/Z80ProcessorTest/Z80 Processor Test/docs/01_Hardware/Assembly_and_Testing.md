# Assembly Order & Testing Strategy

**Status:** 📋 Optimized Pin Mapping  
**Date:** December 23, 2025  
**Approach:** Incremental build with validation at each stage

---

## Overview

This document outlines the recommended assembly sequence and testing procedures for the three-board Z80 validation system. The strategy emphasizes incremental construction with thorough testing at each phase to minimize risk and simplify troubleshooting.

**Key Principles:**
- ✅ Build one board/module at a time
- ✅ Test thoroughly before proceeding
- ✅ Validate power before signals
- ✅ Don't install Z80 until all boards verified
- ✅ Use test equipment at each stage

**Estimated Timeline:** 15-22 hours total (spread over multiple sessions)

---

## Required Test Equipment

| Equipment           | Purpose                              | Minimum Spec               |
|---------------------|--------------------------------------|----------------------------|
| Multimeter          | Voltage, continuity, current         | Basic DC measurements      |
| Oscilloscope        | Clock waveform, signal integrity     | 50 MHz+ bandwidth (optional but helpful) |
| Logic Analyzer      | Multi-signal timing analysis         | 8+ channels (optional)     |
| USB cable           | Power + Teensy programming           | USB Micro-B                |
| Computer            | Code upload, serial monitoring       | Arduino IDE or PlatformIO  |

---

## Phase 1: Teensy Board Assembly

### Why Build First

✅ Simplest board (fewest components)  
✅ Can test independently with USB power  
✅ Validates Teensy and power distribution  
✅ Can upload and run test code before connecting other boards  

### Assembly Steps

1. **Mark Teensy position** on proto board
   - Rows 7-30, columns C-D-G-H
   - USB connector at row 7 (top edge)
   - VUSB pin at row 30

2. **Install power output header** (rows 1-2)
   - 6-pin header (straight or right-angle)
   - Pinout: Pin 1-2=VUSB (5V), Pin 3=3.3V, Pin 4-6=GND

3. **Install OE control header** (row 3)
   - 2-pin header (straight or right-angle)
   - Pin 1=OE signal, Pin 2=GND

4. **Install signal output headers** (rows 25-30)
   - 5× 12-pin headers (straight or right-angle)
   - Label each: Module 1-5
   - Space for Dupont connector access

5. **Solder Teensy 4.1**
   - Option A: Direct solder (permanent)
   - Option B: Machine-pin sockets (removable)
   - Verify orientation (USB at top)
   - Check for solder bridges

6. **Install bypass capacitors**
   - Near VUSB pin (row 30): 10µF electrolytic + 100nF ceramic
   - Near 3.3V pin (~row 20): 10µF electrolytic + 100nF ceramic
   - Short leads (<1 cm), observe polarity on electrolytics

7. **Power distribution wiring**
   - VUSB pin → power output connector Pin 1-2
   - 3.3V pin → power output connector Pin 3
   - Multiple ground connections: Teensy GND pins → connector Pin 4-6
   - Heavy gauge wire (22 AWG) for power

8. **Signal routing**
   - Route each Teensy GPIO pin to appropriate output connector
   - Use solid-core wire (22-24 AWG)
   - Keep wiring organized (follow breadboard traces where possible)
   - Pin 31 → OE control connector

**Component checklist:**
- [ ] Teensy 4.1 installed
- [ ] 2× 10µF capacitors
- [ ] 2× 100nF capacitors
- [ ] 1× 6-pin header
- [ ] 1× 2-pin header
- [ ] 5× 12-pin headers
- [ ] Wire routing complete

### Testing Phase 1

**Pre-power checks:**
1. Visual inspection for solder bridges
2. Continuity test: All grounds connected together
3. Resistance test: No shorts between VUSB, 3.3V, and GND
4. Verify all connector pins wired correctly

**Power-up test:**
1. Connect USB cable
2. Teensy should enumerate (LED blinks on some models)
3. **Measure voltages:**
   - VUSB pin: 4.75-5.25V
   - 3.3V pin: 3.2-3.4V
   - All GND pins: 0V
   - Power connector: Same voltages
4. Check current draw: ~100 mA (Teensy alone)

**Functional test:**
1. Upload simple blink sketch:
   ```cpp
   void setup() {
     pinMode(13, OUTPUT);
   }
   void loop() {
     digitalWrite(13, HIGH);
     delay(500);
     digitalWrite(13, LOW);
     delay(500);
   }
   ```
2. Verify LED blinks (or measure Pin 13 with multimeter)

**Signal connectivity test:**
1. Upload GPIO toggle sketch:
   ```cpp
   void setup() {
     // Set all output pins
     for (int pin = 0; pin <= 41; pin++) {
       pinMode(pin, OUTPUT);
     }
   }
   void loop() {
     // Toggle all pins
     for (int pin = 0; pin <= 41; pin++) {
       digitalWrite(pin, HIGH);
       delay(10);
       digitalWrite(pin, LOW);
       delay(10);
     }
   }
   ```
2. Use multimeter to verify signals at output connectors
3. Check each of the 40 signal pins toggles correctly

**Pass criteria:**
- ✅ All power rails correct voltage
- ✅ Teensy accepts code uploads
- ✅ All 40 signal pins toggle correctly at connectors
- ✅ No shorts or wiring errors

**If fails:** Troubleshoot Teensy board before proceeding

---

## Phase 2: Level Shifter Board - Module 5 (Clock Test)

### Why Module 5 First

✅ Smallest module (6 signals vs 8)  
✅ Tests clock generation (critically important)  
✅ Validates level shifter operation  
✅ Validates cable assembly technique  

### Assembly Steps

1. **Install HW-221 Module 5** (rows 50-59)
   - Verify orientation (pin 1 indicator)
   - Ensure flush seating, no bent pins

2. **Install Module 5 headers**
   - Row 51-58, column A: 12-pin header (8 A-side signals + VCCA + VB + OE + GND)
   - Row 51-58, column J: 12-pin header (8 B-side signals + power/control)
   - Row 50, column D: Logic analyzer header (10-pin)
   - Alternatively: Use individual pins or smaller grouped headers

3. **Install power input header** (rows 1-2)
   - 6-pin header (straight or right-angle)
   - Or: 2× 3-pin headers for flexibility
   - 2-pin header for OE control

4. **Install bypass capacitors** (row 62)
   - 220µF electrolytic on 5V rail
   - 100nF ceramic on 5V rail
   - 100nF ceramic on 3.3V rail

5. **Power distribution wiring**
   - From power input connector to left power rail (3.3V)
   - From power input connector to right power rail (5V)
   - Ground wires to both ground rails
   - Short jumpers from rails to Module 5 power pins:
     - Row 50, column D (VA) → 3.3V rail
     - Row 50, column G (VB) → 5V rail
     - Row 59, column G (GND) → Right ground rail

6. **OE junction** (row 61)
   - Wire from Module 5 OE pin (row 59, column D) to row 61
   - 10kΩ resistor: row 61 to ground
   - Wire from row 61 to OE input connector
   - This is the common OE bus (other modules will connect here later)

7. **Prepare Cable 1** (Teensy → Module 5)
   - Option A: 12× pre-made Dupont jumper wires (20cm)
   - Option B: 10-conductor ribbon cable with Dupont housings
   - Option C: Hand-crimped individual wires to exact length
   - Label both ends: CLK, INT, NMI, WAIT, BUSREQ, RESET + 6 GND
   - Test continuity before connecting
   - Bundle and secure with cable tie or shrink tubing

**Component checklist:**
- [ ] HW-221 Module 5
- [ ] 2× 12-pin headers (or breakaway strips)
- [ ] 1× 6-pin header (power in)
- [ ] 1× 2-pin header (OE in)
- [ ] 1× 10-pin header (logic analyzer)
- [ ] 1× 220µF capacitor
- [ ] 2× 100nF capacitors
- [ ] 1× 10kΩ resistor
- [ ] Cable/jumpers prepared

### Testing Phase 2

**Pre-power checks:**
1. Verify Module 5 orientation (pin 1 correct)
2. Check power wiring: 3.3V to VA, 5V to VB, grounds connected
3. Verify OE pull-down resistor in place
4. Test cable continuity (all 12 conductors)
5. Check for shorts between power rails

**Power-up test (no Teensy connection yet):**
1. Connect power cable from Teensy board
2. Connect OE cable from Teensy board
3. **Measure at Module 5 pins:**
   - VA pin (row 50, column D): 3.2-3.4V
   - VB pin (row 50, column G): 4.75-5.25V
   - GND pin (row 59, column G): 0V
4. OE should be pulled LOW (~0V) by 10kΩ resistor

**Level shifter enable test:**
1. Upload test code to Teensy:
   ```cpp
   #define LEVEL_SHIFTER_OE_PIN 31
   
   void setup() {
     pinMode(LEVEL_SHIFTER_OE_PIN, OUTPUT);
     digitalWrite(LEVEL_SHIFTER_OE_PIN, LOW);  // Disabled
     delay(1000);
     digitalWrite(LEVEL_SHIFTER_OE_PIN, HIGH); // Enabled
   }
   ```
2. Measure OE signal with multimeter
3. Should transition from 0V to 3.3V

**Clock generation test:**
1. Upload clock test code:
   ```cpp
   #define LEVEL_SHIFTER_OE_PIN 31
   #define CLOCK_PIN 28
   
   void setup() {
     // Enable level shifters
     pinMode(LEVEL_SHIFTER_OE_PIN, OUTPUT);
     digitalWrite(LEVEL_SHIFTER_OE_PIN, HIGH);
     
     // Start clock at 1 MHz
     analogWriteFrequency(CLOCK_PIN, 1000000);  // 1 MHz
     analogWrite(CLOCK_PIN, 128);  // 50% duty cycle
   }
   
   void loop() {
     // Clock runs continuously
   }
   ```
2. Connect cable from Teensy Module 5 output
3. **Measure with oscilloscope at B-side (5V, column J):**
   - Frequency: 1 MHz ±1%
   - Duty cycle: 45-55%
   - Amplitude: 0-5V
   - Clean edges (no ringing)
4. **Measure at logic analyzer header (A-side, 3.3V):**
   - Same signal at 3.3V levels
   - Verify bidirectional operation

**Control signal test:**
1. Modify code to toggle other Module 5 signals:
   ```cpp
   void setup() {
     pinMode(42, OUTPUT);
     digitalWrite(42, HIGH);  // Enable shifters
     
     // Module 5 pins: 36(CLK), 2(INT), 3(NMI), 4(WAIT), 5(BUSREQ), 41(RESET)
     pinMode(2, OUTPUT);
     pinMode(3, OUTPUT);
     pinMode(4, OUTPUT);
     pinMode(5, OUTPUT);
     pinMode(41, OUTPUT);
   }
   
   void loop() {
     digitalWrite(2, HIGH); delay(500);
     digitalWrite(2, LOW);  delay(500);
     // Repeat for pins 3, 4, 5, 41
   }
   ```
2. Verify each signal shifts correctly 3.3V → 5V
3. Check with multimeter or scope on B-side

**Pass criteria:**
- ✅ Power rails stable (3.3V, 5V)
- ✅ OE control works (enables/disables shifting)
- ✅ Clock runs at 1 MHz with clean waveform
- ✅ All 6 control signals shift correctly
- ✅ No crosstalk or signal integrity issues

**If fails:** Debug Module 5 and cable before proceeding to other modules

---

## Phase 3: Level Shifter Board - Complete Remaining Modules

### Build Order

**Module 1** (Data Bus, rows 2-11) → Test  
**Module 2** (Address Low, rows 14-23) → Test  
**Module 3** (Address High, rows 26-35) → Test  
**Module 4** (Control Inputs, rows 38-47) → Test  

**Strategy:** Build and test ONE module at a time before proceeding

### Assembly Steps (Per Module)

1. **Install HW-221 module** at designated rows
2. **Install headers:**
   - 12-pin header for A-side (8 signals + VCCA + VB + OE + GND)
   - 12-pin header for B-side (8 signals + power/control)
   - Or: Use individual pins arranged as needed
   - 10-pin logic analyzer header
3. **Power jumpers:**
   - VA pin → 3.3V rail
   - VB pin → 5V rail
   - GND pin → ground rail
4. **OE connection:**
   - OE pin → row 61 (common OE bus)
5. **Prepare cables:**
   - 12× Dupont jumper wires per module
   - Or: 10-conductor ribbon cable with Dupont connectors
   - Label each connection clearly

### Testing Phase 3 (Per Module)

**For each module after assembly:**

1. **Pre-power check:**
   - Verify module orientation
   - Check power connections
   - Test cable continuity
   - Check for shorts

2. **Power test:**
   - Connect cable from Teensy
   - Measure VA, VB, GND at module pins
   - Verify OE connected to common bus

3. **Signal toggle test:**
   - Upload code to toggle all pins for this module
   - Example for Module 1 (Data Bus):
   ```cpp
   void setup() {
     pinMode(42, OUTPUT);
     digitalWrite(42, HIGH);  // Enable shifters
     
     // Module 1: Data bus D0-D7 = Teensy pins 6-13
     for (int pin = 6; pin <= 13; pin++) {
       pinMode(pin, OUTPUT);
     }
   }
   
   void loop() {
     // Toggle each data line
     for (int pin = 6; pin <= 13; pin++) {
       digitalWrite(pin, HIGH); delay(200);
       digitalWrite(pin, LOW);  delay(200);
     }
   }
   ```
4. **Measure on logic analyzer (A-side, 3.3V):**
   - All 8 signals toggle correctly
   - Clean transitions
   - Correct timing

5. **Measure on B-side (5V, column J):**
   - All 8 signals at 5V levels
   - Verify with multimeter or scope

6. **Signal mapping verification:**
   - Ensure correct Teensy pin → Module pin → B-side pin
   - Cross-reference with pin mapping doc

**Pass criteria (per module):**
- ✅ All 8 signals shift correctly 3.3V ↔ 5V
- ✅ No stuck signals or shorts
- ✅ Signal mapping matches documentation
- ✅ Power consumption reasonable (check total current)

**After all 4 modules complete:**
- ✅ 5 total modules operational
- ✅ All 40 signals (38 + OE + spare) shifting correctly
- ✅ System draws ~400 mA (Teensy + 5 level shifters)

---

## Phase 4: Z80 Board Assembly (No Z80 Yet)

### Assembly Steps

1. **Install DIP-40 socket** (rows 1-20)
   - Center over gap (columns D and G)
   - Pin 1 indicator at top (row 1)
   - Solder carefully, check for bridges

2. **Install bypass capacitors** (rows 21-22)
   - 10µF electrolytic: Vcc (row 11) to GND
   - 100nF ceramic: Vcc (row 11) to GND
   - Keep leads very short

3. **Install input headers** (rows 61-62)
   - 5× 12-pin headers (straight or right-angle)
   - Label: Module 1-5 to match level shifter cables
   - Space for Dupont connector access

4. **Install LEDs** (rows 23-60, every other row)
   - **Left side (rows 23,25,27...59):** 19 LEDs
     - Yellow: D0-D7 (rows 23-37)
     - Blue: Control signals (rows 39-59)
   - **Right side (rows 24,26,28...60):** 19 LEDs
     - Green: A0-A15 (rows 24-54)
     - Blue: Control signals (rows 56-60)
   - Verify polarity: anode (longer lead) faces resistor side

5. **Install resistors** (adjacent to LEDs)
   - 38× 1.5kΩ, 1/4W
   - One per LED

6. **LED circuit wiring:**
   - **Left side:** Signal → Resistor → LED anode → LED cathode → GND
   - **Right side:** GND → LED cathode → LED anode → Resistor → Signal

7. **Power distribution:**
   - 5V from input connectors to right power rail
   - Distribute to LED circuits
   - Multiple grounds from connectors to both ground rails
   - Vcc connection: 5V rail → Socket Pin 11 (row 11)
   - Ground: Both rails → Socket Pins 29 & 31

8. **Signal routing:**
   - From input connectors → Z80 socket pins
   - Branch signals to feed LED circuits
   - Use breadboard traces where possible
   - Jumper wires where needed

**Component checklist:**
- [ ] DIP-40 socket
- [ ] 1× 10µF capacitor
- [ ] 1× 100nF capacitor
- [ ] 5× 12-pin headers
- [ ] 8× yellow LEDs
- [ ] 16× green LEDs
- [ ] 14× blue LEDs
- [ ] 38× 1.5kΩ resistors
- [ ] All wiring complete

### Testing Phase 4

**Pre-power checks:**
1. Visual inspection: LED polarity, resistor placement
2. Continuity: 5V rail to socket Pin 11
3. Continuity: Ground rails to socket Pins 29 & 31
4. Check for shorts between power rails
5. Verify all connector pins wired correctly

**Power test (no Z80 installed):**
1. Connect all 5 cables from level shifter board
2. Power up system
3. **Measure at socket:**
   - Pin 11 (Vcc): 4.75-5.25V
   - Pins 29, 31 (GND): 0V
4. Check current draw: Should be ~400 mA (no LEDs lit yet)

**LED test:**
1. Upload test code to toggle all signals:
   ```cpp
   void setup() {
     pinMode(42, OUTPUT);
     digitalWrite(42, HIGH);  // Enable shifters
     
     // Set all output pins
     for (int pin = 0; pin <= 41; pin++) {
       if (pin != 42) pinMode(pin, OUTPUT);
     }
   }
   
   void loop() {
     // Turn all signals HIGH (LEDs should light)
     for (int pin = 0; pin <= 41; pin++) {
       if (pin != 42) digitalWrite(pin, HIGH);
     }
     delay(1000);
     
     // Turn all signals LOW (LEDs should go off)
     for (int pin = 0; pin <= 41; pin++) {
       if (pin != 42) digitalWrite(pin, LOW);
     }
     delay(1000);
   }
   ```

2. **Observe LEDs:**
   - All 38 LEDs should light together when signals HIGH
   - All 38 LEDs should turn off when signals LOW
   - Check brightness (should be even, ~2 mA each)

3. **Individual LED test:**
   - Modify code to toggle one signal at a time
   - Verify correct LED responds to correct signal
   - Confirm color coding:
     - Yellow = Data (D0-D7)
     - Green = Address (A0-A15)
     - Blue = Control signals

4. **Measure current:**
   - All LEDs on: ~400 mA + 76 mA = 476 mA
   - All LEDs off: ~400 mA
   - Confirms LED current budget

**Socket signal verification:**
1. Run signal toggle code
2. With multimeter or scope, measure at socket pins:
   - Data bus pins (14,12,8,7,9,10,13,15): Toggle 0-5V
   - Address pins (30-39, 1-6): Toggle 0-5V
   - Control pins: Toggle 0-5V
3. Verify correct signal at correct pin (cross-ref pin mapping doc)

**Pass criteria:**
- ✅ All 38 LEDs working
- ✅ Correct LED responds to correct signal
- ✅ Color scheme correct (yellow/green/blue)
- ✅ All 40 socket pins have correct signals
- ✅ Power rails stable, current draw as expected
- ✅ No shorts or wiring errors

**If fails:** Debug Z80 board before installing Z80 CPU

---

## Phase 5: First Z80 Power-Up

### Preparation

**Upload Z80 initialization firmware:**
```cpp
#define LEVEL_SHIFTER_OE_PIN 31
#define CLOCK_PIN 28
#define RESET_PIN 34
#define INT_PIN 6
#define NMI_PIN 9
#define WAIT_PIN 35
#define BUSREQ_PIN 32

void setup() {
  Serial.begin(115200);
  Serial.println("Z80 Test Rig - First Power-Up");
  
  // Enable level shifters
  pinMode(LEVEL_SHIFTER_OE_PIN, OUTPUT);
  digitalWrite(LEVEL_SHIFTER_OE_PIN, HIGH);
  
  // Initialize control signals to safe states
  pinMode(RESET_PIN, OUTPUT);
  digitalWrite(RESET_PIN, LOW);  // Hold Z80 in reset
  
  pinMode(INT_PIN, OUTPUT);
  digitalWrite(INT_PIN, HIGH);  // No interrupt
  
  pinMode(NMI_PIN, OUTPUT);
  digitalWrite(NMI_PIN, HIGH);  // No NMI
  
  pinMode(WAIT_PIN, OUTPUT);
  digitalWrite(WAIT_PIN, HIGH);  // No wait states
  
  pinMode(BUSREQ_PIN, OUTPUT);
  digitalWrite(BUSREQ_PIN, HIGH);  // No bus request
  
  // Clock OFF initially
  analogWrite(CLOCK_PIN, 0);
  
  Serial.println("System initialized, Z80 in RESET");
  Serial.println("Ready for Z80 installation");
}

void loop() {
  // Wait for command to start
}
```

### Z80 Installation

**CRITICAL STEPS:**

1. **Power off entire system** (unplug USB)
2. **Verify Z80 orientation:**
   - Pin 1 indicator (notch or dot) at row 1
   - Match socket pin 1 indicator
   - **Triple-check this!** Wrong orientation = destroyed chip
3. **Insert Z80 carefully:**
   - Align all 40 pins
   - Press gently but firmly
   - Ensure no bent pins
   - Verify seated flush in socket
4. **Visual inspection:**
   - All pins inserted correctly
   - No bent pins underneath
   - Socket not cracked

### Power-Up Sequence

**Step 1: Initial power-up (Z80 in reset)**
1. Reconnect USB power
2. Open serial monitor (115200 baud)
3. Should see: "System initialized, Z80 in RESET"
4. **Measure Z80 power consumption:**
   - NMOS Z80: ~200 mA (plus 400 mA system = 600 mA total)
   - CMOS Z80: ~50 mA (plus 400 mA system = 450 mA total)
5. If current excessive (>800 mA): **Power off immediately**, check for shorts

**Step 2: Start clock**
1. Add to loop() in firmware:
   ```cpp
   void loop() {
     if (Serial.available()) {
       char cmd = Serial.read();
       if (cmd == 'c') {
         // Start clock at 100 kHz (slow and safe)
         analogWriteFrequency(CLOCK_PIN, 100000);
         analogWrite(CLOCK_PIN, 128);
         Serial.println("Clock started at 100 kHz");
       }
     }
   }
   ```
2. Upload and send 'c' via serial
3. Clock LED should blink (appears solid to human eye)
4. Measure clock with scope: 100 kHz, 50% duty cycle

**Step 3: Release reset**
1. Add to firmware:
   ```cpp
   if (cmd == 'r') {
     digitalWrite(RESET_PIN, HIGH);
     Serial.println("RESET released, Z80 running");
   }
   ```
2. Send 'r' via serial
3. **Observe LEDs immediately:**
   - Green LEDs (address): Should show activity (incrementing pattern)
   - Yellow LEDs (data): Should show values (instructions being fetched)
   - Blue LEDs (control): Should pulse (M1, MREQ, RD active)
   - HALT LED (blue): Should be OFF (CPU running)

**Step 4: Analyze behavior**
1. With no external memory, Z80 will see 0xFF (pull-ups) or 0x00 on data bus
2. Expected behavior depends on what bus floats to:
   - **0x00 (NOP):** Address increments continuously, wraps around
   - **0xFF (RST 38h):** Jumps to address 0x0038, then sees more 0xFF
3. Either pattern shows Z80 is executing instructions
4. Use logic analyzer to capture first few cycles

### Testing Phase 5

**NOP Loop Test (if data bus floats to 0x00):**

Expected operation:
1. PC starts at 0x0000 (after reset)
2. Z80 fetches instruction at 0x0000
3. Sees 0x00 (NOP)
4. PC increments to 0x0001
5. Repeat forever

**Observable indicators:**
- Green LEDs: Show incrementing pattern (address bus counts up)
- Yellow LEDs: Show 0x00 (all off if active-high LEDs)
- M1 LED: Pulses regularly (instruction fetch)
- MREQ LED: Active during fetch
- RD LED: Active during fetch
- WR LED: Off (no writes)
- IORQ LED: Off (no I/O)

**Verification steps:**
1. Count address increment rate: Should match clock / 4
   - 100 kHz clock → 25 kHz address increment → 40 µs per instruction
2. Measure M1 pulse width with scope
3. Verify MREQ and RD overlap during M1
4. Capture with logic analyzer:
   - Address increments sequentially
   - Data reads as 0x00
   - M1, MREQ, RD timing correct

**Increase clock frequency:**
```cpp
if (cmd == 'f') {
  // Increase to 1 MHz
  analogWriteFrequency(CLOCK_PIN, 1000000);
  analogWrite(CLOCK_PIN, 128);
  Serial.println("Clock increased to 1 MHz");
}
```

Retest at higher frequency:
- 1 MHz, 2 MHz, 4 MHz, 5 MHz
- Verify stable operation at each frequency
- Address LEDs should blur into patterns
- System should remain stable

**Pass criteria:**
- ✅ Z80 powers up without excessive current
- ✅ Clock runs at specified frequency
- ✅ Address bus increments (visible on green LEDs)
- ✅ Control signals show activity (M1, MREQ, RD)
- ✅ No stuck signals or erratic behavior
- ✅ Operates stably up to 5 MHz

**If fails, troubleshooting:**

**No LED activity:**
- Check RESET is released (HIGH)
- Verify clock is running (measure with scope)
- Check Z80 orientation (pin 1 correct?)
- Measure power at Z80 pins

**Erratic LED patterns:**
- Reduce clock frequency
- Check bypass capacitors
- Verify all grounds connected
- Check for loose connections

**Address doesn't increment:**
- Z80 may be halted
- Check HALT signal (should be HIGH/inactive)
- Verify data bus actually has 0x00
- Try different Z80 chip

**High current draw:**
- Z80 installed backwards? (destroy and replace)
- Short circuit somewhere
- Wrong voltage on pins

---

## Phase 6: Full System Validation

Once basic operation confirmed, proceed to comprehensive testing:

### Test Sequence

**1. NOP Loop with Verification**
- Upload program that feeds 0x00 (NOP) on data bus
- Verify address increments continuously
- Measure timing with logic analyzer
- Confirm instruction cycle: 4 clock periods per NOP

**2. Simple Instruction Test**
```
Example: LD A, 5
0x3E, 0x05  
```
- Feed specific instruction sequence
- Verify correct fetch timing
- Confirm M1 signal for opcode fetch
- Verify MREQ during memory read

**3. Arithmetic Operations**
- LD, ADD, SUB, INC, DEC instructions
- Verify internal register operations (no visible external change)
- Test flag effects

**4. Memory Access Patterns**
- LD (HL), A - Memory write
- LD A, (HL) - Memory read
- Verify MREQ and WR for writes
- Verify MREQ and RD for reads

**5. Jump Instructions**
- JP addr - Absolute jump
- JR offset - Relative jump
- Verify PC changes correctly (address bus jumps)

**6. Stack Operations**
- PUSH, POP
- CALL, RET
- Verify SP manipulation
- Check memory writes during PUSH

**7. I/O Operations**
- IN A, (port)
- OUT (port), A
- Verify IORQ signal (vs MREQ)
- Check address bus shows port number

**8. Interrupt Response (if implemented)**
- Toggle INT line
- Verify Z80 acknowledges (M1 + IORQ)
- Check jump to interrupt vector
- Test interrupt service routine

**9. Instruction Set Validation**
- Run comprehensive test suite
- Exercise all opcodes
- Verify undocumented flags (if testing accuracy)
- Compare against known-good Z80 behavior

### Validation Tools

**Serial Logging:**
- Teensy logs all bus transactions
- Timestamp each operation
- Compare against expected sequence

**Logic Analyzer Capture:**
- Record timing of key operations
- Measure setup/hold times
- Verify signal integrity at full speed

**LED Observation:**
- Visual confirmation of expected patterns
- Quick sanity check during development
- Useful for demos

### Performance Testing

**Maximum Clock Frequency:**
1. Incrementally increase clock
2. Test at: 1, 2, 4, 5, 6, 8, 10 MHz
3. Find maximum stable frequency
4. Typical limits:
   - NMOS Z80: 4-6 MHz (depends on variant)
   - CMOS Z80: 8-20 MHz (newer parts)
5. Note any signal integrity issues

**Sustained Operation:**
- Run for extended periods (hours)
- Monitor for intermittent issues
- Check thermal stability
- Verify no memory leaks in Teensy code

### Documentation

**For each test:**
- Record expected behavior
- Capture logic analyzer traces
- Document any anomalies
- Note clock frequencies tested
- Save successful configurations

---

## Troubleshooting Guide

### Common Issues

**Issue: Teensy doesn't enumerate**
- **Cause:** USB cable bad, Teensy damaged, driver issue
- **Fix:** Try different cable, different USB port, reinstall Teensy drivers

**Issue: No 5V or 3.3V power**
- **Cause:** Wiring error, short circuit, damaged regulator
- **Fix:** Check continuity, look for shorts, measure at Teensy pins directly

**Issue: Level shifter doesn't shift**
- **Cause:** OE not enabled, power missing, bad module
- **Fix:** Verify OE is HIGH, check VCCA/VCCB voltages, swap module

**Issue: Clock not running**
- **Cause:** Wrong pin, bad PWM setup, level shifter issue
- **Fix:** Measure at Teensy pin 36, verify PWM code, check module 5

**Issue: LEDs don't light**
- **Cause:** LED polarity reversed, resistor wrong value, wiring error
- **Fix:** Check LED orientation, verify resistor (1.5kΩ), trace circuit

**Issue: Wrong LED lights**
- **Cause:** Signal mapping error, crossed wires
- **Fix:** Compare wiring to pin mapping doc, test signals individually

**Issue: Z80 doesn't run**
- **Cause:** Wrong orientation, clock missing, reset held low, power issue
- **Fix:** Check pin 1 alignment, verify clock, check RESET signal, measure Vcc

**Issue: Erratic Z80 behavior**
- **Cause:** Clock too fast, signal integrity issues, loose connection
- **Fix:** Reduce frequency, check bypass caps, reseat Z80, check cables

**Issue: Z80 gets hot**
- **Cause:** Wrong orientation (destroyed), short circuit
- **Fix:** Power off immediately, check orientation, replace Z80

### Debug Procedures

**For power issues:**
1. Measure voltages at source (Teensy)
2. Measure at intermediate point (level shifter board)
3. Measure at load (Z80 board)
4. Check current draw at each stage
5. Look for voltage drops (bad connections)

**For signal issues:**
1. Verify signal at source (Teensy GPIO)
2. Check at level shifter input (3.3V side)
3. Check at level shifter output (5V side)
4. Verify at destination (Z80 pin)
5. Use logic analyzer to see timing

**For Z80 issues:**
1. Verify all control signals at correct idle states
2. Check clock is running and clean
3. Confirm reset sequence (LOW → HIGH transition)
4. Monitor current draw (should be stable)
5. Try different Z80 chip (may be defective)

---

## Safety Notes

**Electrical Safety:**
- ⚠️ USB power limited to 500 mA (safe)
- ⚠️ No high voltages present
- ⚠️ ESD sensitive: Use grounding strap or touch grounded metal

**Component Protection:**
- ⚠️ Z80 orientation CRITICAL: Double-check before power
- ⚠️ Level shifters can be damaged by incorrect power sequencing
- ⚠️ Teensy GPIO not 5V tolerant: Level shifters mandatory
- ⚠️ Electrolytic capacitor polarity: Reversed = explosion

**Heat Warnings:**
- ⚠️ Z80 getting hot = problem (power off, investigate)
- ⚠️ Voltage regulators warm = normal
- ⚠️ Level shifters cool = normal (low power)

---

## Timeline Estimate

| Phase | Task                          | Time      | Cumulative |
|-------|-------------------------------|-----------|------------|
| 1     | Teensy board assembly + test  | 2-3 hrs   | 3 hrs      |
| 2     | Module 5 assembly + test      | 2-3 hrs   | 6 hrs      |
| 3     | Modules 1-4 (1.5 hrs each)    | 6-8 hrs   | 14 hrs     |
| 4     | Z80 board assembly + test     | 4-6 hrs   | 20 hrs     |
| 5     | First Z80 power-up            | 1-2 hrs   | 22 hrs     |
| 6     | Full validation               | Variable  | -          |

**Total assembly time: 15-22 hours** (can spread over multiple sessions)

**Validation testing:** Ongoing as you develop test programs and firmware

---

## Summary

This incremental assembly strategy provides:

✅ **Risk Mitigation** - Catch issues early before they compound  
✅ **Easy Debugging** - Know exactly which stage introduced problem  
✅ **Confidence Building** - Each success validates the approach  
✅ **Learning Opportunity** - Understand each subsystem thoroughly  
✅ **Flexible Pacing** - Natural stopping points between phases  

**Key Success Factors:**
- Don't skip testing phases
- Document all findings
- Keep workspace organized
- Label all cables and connections
- Take breaks between phases (fresh eyes catch errors)

**When complete, you'll have:**
- Working Z80 test platform
- Understanding of signal-level operations
- Ability to validate any Z80 CPU
- Foundation for future enhancements
- Valuable hands-on experience with CPU interfacing

Good luck with the build! 🚀
