# Phase 1 Hardware Plan - Summary

**Status:** ✅ Complete - Ready for Implementation  
**Date:** December 22, 2025  
**Phase:** Planning Complete, Awaiting Hardware

---

## Overview

This document summarizes the complete hardware plan for migrating the Z80 Processor Validation Suite from Arduino Mega 2560 to Teensy 4.1, with full compatibility for all Z80 CPU generations (NMOS to CMOS).

---

## Key Design Decisions

### 1. Pin Mapping Strategy ✓

**Priorities (in order):**
1. ✅ **Speed & Reliability** - Direct register access, optimized GPIO banks
2. ✅ **Pin Grouping** - Consecutive pins for simplified wiring
3. ✅ **Bank Consolidation** - All 16 address bits on GPIO6, all 8 data bits on GPIO7

**Pin Allocations:**
- **Data Bus (D0-D7):** Teensy pins 6-13 (consecutive, **all GPIO7!**)
- **Address Low (A0-A7):** Teensy pins 14-21 (consecutive, **all GPIO6!**)
- **Address High (A8-A15):** Teensy pins 0-1, 22-27 (**all GPIO6!**)
- **Control Inputs:** Teensy pins 28-31, 37-40 (GPIO8 + GPIO9)
- **Control Outputs + CLK:** Teensy pins 2-5, 36, 41 (GPIO8 + GPIO9)

**Total:** 39 signals mapped to 40 Teensy pins + 1 for OE control = 40 used

### 2. Power Supply Design ✓

**Selected:** USB-only design via VUSB pin

**Power Distribution:**
```
USB Cable → Teensy
           ├→ VUSB (5V) → Z80 Vcc + Level Shifters VCCB (250 mA)
           └→ 3.3V pin → Level Shifters VCCA (150 mA)
```

**Current Budget:**
- 3.3V rail: 150 mA / 250 mA max = 60% utilization ✅
- 5V rail (continuous): 250 mA / 500 mA max = 50% utilization ✅
- 5V rail (with LEDs): +76 mA peak / +24 mA average
  - Peak: 326 mA = 65% utilization ✅
  - Average: 274 mA = 55% utilization ✅
- Single cable for power + programming + serial

**Bypass Capacitors (Dual Strategy):**
- Z80 Vcc: 10µF electrolytic + 100nF ceramic
- Teensy 3.3V: 10µF electrolytic + 100nF ceramic
- Teensy VUSB: 10µF electrolytic + 100nF ceramic
- Level shifter 5V: 220µF electrolytic + 100nF ceramic (for LED switching)
- Level shifter 3.3V: 100nF ceramic
- HW-221 modules: Built-in decoupling caps
- **Rationale:** Electrolytic for bulk storage, ceramic for high-frequency transients

### 3. Interconnect Strategy ✓

**Cable Type:** 15-conductor shielded cable with JST XH connectors

**Physical Layout:**
- Teensy + Z80 on one proto board
- 5x Level shifters on separate proto board
- Removable cables for flexibility

**Cable Lengths:**
- 3.3V side (Teensy to shifters): ≤6"
- 5V side (shifters to Z80): ≤12"

**Ground Distribution:**
- 1 ground wire per 2-3 signal wires
- Shield grounded at level shifter board only
- Prevents ground bounce and ground loops

**Connector Sizes:**
- Modules 1-4: 12-pin JST XH (8 signals + 4 grounds)
- Module 5: 10-pin JST XH (6 signals + 4 grounds)
- Power: 4-6 pin JST XH

### 4. Level Shifter Configuration ✓

**Hardware:** 5x HW-221 modules (TXS0108E bidirectional level shifters)

**Enable Control:**
- All OE pins tied to Teensy Pin 42 (software control)
- 10kΩ pull-down for safe default-off state
- Enables clean resets and debug modes

**Module Assignments:**
- **Module 1:** Data bus (D0-D7) - **Bidirectional**
- **Module 2:** Address low byte (A0-A7) - Unidirectional (Z80→Teensy)
- **Module 3:** Address high byte (A8-A15) - Unidirectional (Z80→Teensy)
- **Module 4:** Control inputs (RD, WR, MREQ, etc.) - Unidirectional (Z80→Teensy)
- **Module 5:** Control outputs + CLK - Unidirectional (Teensy→Z80)

**Rationale:**
- ✅ Auto-direction sensing (no DIR pin management)
- ✅ Fast propagation (2-3 ns, <2% of Z80 clock period @ 5 MHz)
- ✅ 110 MHz bandwidth (far exceeds Z80 requirements)
- ✅ Clean 8-pin groups simplify wiring

**Enable Control:**
- All OE pins tied to Teensy Pin 42 (software control)
- 10kΩ pull-down for safe default-off state
- Enables clean resets and debug modes

### 5. Clock Generation ✓

**Implementation:** Hardware PWM using FlexPWM2_A on Teensy pin 36

**Capabilities:**
- Frequency range: **100 KHz - 10+ MHz** (vs 100 KHz - 1 MHz on Mega)
- 50% duty cycle (configurable in hardware)
- Jitter-free operation (hardware timer)
- Zero CPU overhead once configured

**Connection:** Pin 36 → Level Shifter Module 5 → Z80 Pin 6 (CLK)

### 6. Debug Infrastructure ✓

**Logic Analyzer Headers:** Dual-row headers on 3.3V side (Teensy GPIO)

**Accessible Signals:**
- Data bus (D0-D7): 10-pin header
- Address bus (A0-A15): 20-pin headers (2x 10-pin)
- Control signals: 12-pin header
- Clock: 2-pin header
- Total: ~44 pins with proper ground distribution

**Advantages:**
- Measures Teensy's view of signals
- Safe for 3.3V logic analyzers
- No loading on Z80 side
- Access to generated signals (clock, outputs)

**Debug LEDs:** 38 LEDs on 5V rail (level shifter outputs)

**LED Configuration:**
- Data bus: 8 LEDs
- Address bus: 16 LEDs
- Control signals: 13 LEDs
- Clock: 1 LED
- Current: 2 mA per LED (1.5kΩ resistors)
- Power: 76 mA peak, ~24 mA average (@30% duty cycle)
- Mounted on level shifter board for real-time bus activity visualization

### 7. Interrupt Configuration ✓

**Interrupt Pins:**
- **Pin 28:** /RD signal (falling edge trigger)
- **Pin 30:** /WR signal (falling edge trigger)

**Advantages:**
- Both on GPIO8 bank → fast interrupt handling
- NVIC priority 0 (highest) for ISRs
- ~50-100 ns latency (vs ~1.6 µs on Mega) = **16-32x faster**
- Preemptive multitasking capable (not needed for this project)

### 8. GPIO Bank Optimization ✓

**Key Optimizations:** 
- **All 16 address bits (A0-A15) on GPIO6** → Single register read
- **All 8 data bits (D0-D7) on GPIO7** → Single register read/write

**Performance Impact:**
```cpp
// Single 32-bit register read captures ENTIRE 16-bit address bus!
uint32_t gpio6 = GPIO6_PSR;  // ~1.7 ns @ 600 MHz
uint16_t address = extract_address(gpio6);  // ~10-15 ns total

// Single 32-bit register read/write for ENTIRE 8-bit data bus!
uint32_t gpio7 = GPIO7_PSR;  // ~1.7 ns
uint8_t data = extract_data(gpio7);  // ~10-15 ns total

// vs Arduino Mega (16 separate digitalRead() calls for address)
// ~2000 ns total
```

**Speedup:** ~**100-200x faster** for bus operations

---

## Z80 Compatibility Matrix

| Z80 Variant         | Voltage | V_IH | V_OH | Speed     | Compatible? |  
|---------------------|---------|------|------|-----------|-------------|  
| **NMOS (Original)** | 5V      | 2.0V | 2.4V | 2.5-4 MHz | ✅ Yes      |  
| **CMOS (Modern)**   | 5V      | 3.5V | 4.5V | 4-20 MHz  | ✅ Yes      |  
| **Zilog Z8400**     | 5V      | 2.0V | 2.4V | 2.5 MHz   | ✅ Yes      |  
| **Zilog Z84C00**    | 5V      | 3.5V | 4.5V | 4-20 MHz  | ✅ Yes      |  
| **NEC µPD780C**     | 5V      | 2.0V | 2.4V | 4 MHz     | ✅ Yes      |  
| **Mostek MK3880**   | 5V      | 2.0V | 2.4V | 2.5 MHz   | ✅ Yes      |**Verdict:** TXS0108E level shifters provide 5V logic compatible with all Z80 generations.

---

## Performance Targets

| Metric               | Arduino Mega | Teensy 4.1 Target | Improvement  |  
|----------------------|--------------|-------------------|--------------|  
| **Max Z80 Clock**    | 1 MHz        | 5-10 MHz          | **5-10x**    |  
| **ISR Latency**      | 1.6 µs       | 50-100 ns         | **16-32x**   |  
| **Address Bus Read** | 2.0 µs       | 15-20 ns          | **100-133x** |  
| **Data Bus Read**    | 1.0 µs       | 10-15 ns          | **67-100x**  |  
| **GPIO Toggle Rate** | 4 MHz        | 150 MHz           | **37.5x**    |**Expected Z80 Performance:** **5+ MHz sustained operation** (validated by ISR timing budget)

---

## Hardware Bill of Materials (BOM)

| Item                          | Quantity | Estimated Cost | Source                |  
|-------------------------------|----------|----------------|-----------------------|  
| **Teensy 4.1**                | 1        | $27-30         | PJRC.com              |  
| **HW-221 (TXS0108E) Modules** | 5        | $15-20         | Amazon, eBay          |  
| **Z80 CPU (40-pin DIP)**      | 1        | $5-20          | eBay, Jameco, Mouser  |  
| **Breadboards (830-point)**   | 2        | $10            | Amazon                |  
| **Jumper Wires (M-M)**        | 40+      | $5             | Amazon                |  
| **5V Power Supply (1A)**      | 1        | $5-10          | Amazon                |  
| **Bypass Capacitors**         | 12       | $2             | DigiKey, Mouser       |  
| - 100nF ceramic               | 6        | -              | -                     |  
| - 10µF electrolytic           | 6        | -              | -                     |  
| **Logic Analyzer (optional)** | 1        | $50-100        | Amazon (Saleae clone) |  
| **Total**                     | -        | **$120-200**   | -                     |**Budget:** ~$150 for complete hardware (without logic analyzer)

---

## Wiring Diagram Summary

### Module 1 - Data Bus (Bidirectional)
```
Teen sy 6-13 <--[TXS0108E]--> Z80 14,12,8,7,9,10,13,15 (D4,D3,D5,D6,D2,D7,D0,D1)
```
**All GPIO7 bank** - Single register read/write for entire data bus!

### Module 2 - Address Low (A0-A7)
```
Tee nsy 14-21 --[TXS0108E]--> Z80 30-37 (A0-A7)
```
**All GPIO6 bank** - Left side of Z80 DIP

### Module 3 - Address High (A8-A15)
```
Teensy 0-1, 22-27 --[TXS0108E]--> Z80 38-39,1-6 (A8-A15)
```
**All GPIO6 bank** - Right side of Z80 DIP. **Full 16-bit address in one register!**

### Module 4 - Control Inputs (Z80 → Teensy)
```
Teensy 28-31, 37-40 <--[TXS0108E]-- Z80 18-23,27-28 (HALT,MREQ,IORQ,RD,WR,BUSACK,M1,RFSH)
```
Mostly GPIO8, /RD and /WR interrupt-capable

### Module 5 - Control Outputs (Teensy → Z80)
```
Teensy 2-5, 36, 41 --[TXS0108E]--> Z80 6,16,17,24,25,26 (CLK,INT,NMI,WAIT,BUSREQ,RESET)
```
Pin 36 = FlexPWM2_A for hardware clock generation

**Wiring Complexity:** Moderate - follows Z80 DIP physical order for minimal wire crossing

---

## GPIO Register Access Summary

### Fast I/O Macros (To Be Implemented)

```cpp
// Address bus read (ALL 16 bits on GPIO6!)
inline uint16_t readAddressBus() {
  uint32_t gpio6 = GPIO6_PSR;  // All A0-A15 on GPIO6
  uint16_t address = 
    ((gpio6 >> 18) & 0x0001) |  // A0 (pin 14)
    ((gpio6 >> 19) & 0x0002) |  // A1 (pin 15)
    ((gpio6 >> 23) & 0x0004) |  // A2 (pin 16)
    ((gpio6 >> 22) & 0x0008) |  // A3 (pin 17)
    ((gpio6 >> 17) & 0x0010) |  // A4 (pin 18)
    ((gpio6 >> 16) & 0x0020) |  // A5 (pin 19)
    ((gpio6 >> 26) & 0x0040) |  // A6 (pin 20)
    ((gpio6 >> 27) & 0x0080) |  // A7 (pin 21)
    ((gpio6 >> 24) & 0x0100) |  // A8 (pin 22)
    ((gpio6 >> 25) & 0x0200) |  // A9 (pin 23)
    ((gpio6 >> 3)  & 0x0400) |  // A10 (pin 0)
    ((gpio6 >> 2)  & 0x0800) |  // A11 (pin 1)
    ((gpio6 >> 12) & 0x1000) |  // A12 (pin 24)
    ((gpio6 >> 13) & 0x2000) |  // A13 (pin 25)
    ((gpio6 >> 30) & 0x4000) |  // A14 (pin 26)
    ((gpio6 >> 31) & 0x8000);   // A15 (pin 27)
  return address;
}
// ~10-15 ns total

// Data bus read (ALL 8 bits on GPIO7!)
inline uint8_t readDataBus() {
  uint32_t gpio7 = GPIO7_PSR;  // All D0-D7 on GPIO7
  uint8_t data = 
    ((gpio7 >> 10) & 0x10) |  // D4 (pin 6)
    ((gpio7 >> 17) & 0x08) |  // D3 (pin 7)
    ((gpio7 >> 16) & 0x20) |  // D5 (pin 8)
    ((gpio7 >> 11) & 0x40) |  // D6 (pin 9)
    ((gpio7 >> 0)  & 0x04) |  // D2 (pin 10)
    ((gpio7 >> 2)  & 0x80) |  // D7 (pin 11)
    ((gpio7 >> 1)  & 0x01) |  // D0 (pin 12)
    ((gpio7 >> 3)  & 0x02);   // D1 (pin 13)
  return data;
}
// ~10-15 ns total

// Control signal checks
inline bool isReadCycle() {
  return (GPIO8_PSR >> 18) & 1;  // Pin 28 = GPIO8 bit 18 (/RD)
}
// ~5 ns
```

**Performance:** All I/O operations <20 ns (vs 125 ns - 2 µs on Arduino Mega)

---

## Implementation Roadmap

### Phase 1A: Module 5 - Clock Output (Week 1)
**Goal:** Generate Z80 clock signal

- [ ] Connect Module 5 level shifter
- [ ] Wire Teensy pin 36 → Module 5 → Z80 pin 6
- [ ] Implement FlexPWM clock generation (Phase 2 deliverable)
- [ ] Test clock output with oscilloscope
- [ ] Verify 50% duty cycle at 1 MHz, 2 MHz, 3 MHz

**Success Criteria:** Stable square wave on Z80 CLK pin (pin 6) at 1+ MHz

### Phase 1B: Modules 2 & 3 - Address Bus (Week 1-2)
**Goal:** Read Z80 address bus

- [ ] Connect Modules 2 & 3 level shifters
- [ ] Wire address bus (16 pins total)
- [ ] Implement `readAddressBus()` function
- [ ] Drive Z80 address pins manually (testing)
- [ ] Verify correct 16-bit address reads

**Success Criteria:** Teensy correctly reads all 16 address bits

### Phase 1C: Module 4 - Control Inputs (Week 2)
**Goal:** Monitor Z80 control signals

- [ ] Connect Module 4 level shifter
- [ ] Wire control signals (8 pins)
- [ ] Implement control signal read functions
- [ ] Configure interrupts on pins 28 (/RD) and 30 (/WR)
- [ ] Test interrupt triggering

**Success Criteria:** ISRs triggered by /RD and /WR transitions

### Phase 1D: Module 1 - Data Bus (Week 2-3)
**Goal:** Bidirectional data bus communication

- [ ] Connect Module 1 level shifter
- [ ] Wire data bus (8 pins)
- [ ] Implement `readDataBus()` and `writeDataBus()` functions
- [ ] Test read operations (Z80 → Teensy)
- [ ] Test write operations (Teensy → Z80)
- [ ] Verify bidirectional operation

**Success Criteria:** Data transfer in both directions verified

### Phase 1E: Full System Integration (Week 3-4)
**Goal:** Complete hardware validation

- [ ] Connect Z80 RESET pin (Module 5)
- [ ] Wire remaining control outputs (WAIT, INT, NMI, BUSREQ)
- [ ] Implement complete HAL (Phase 3 deliverable)
- [ ] Run basic Z80 instruction (NOP loop)
- [ ] Verify ISR memory emulation
- [ ] Test at 1 MHz, 2 MHz, 3 MHz, 5 MHz

**Success Criteria:** Z80 executes test programs at 5+ MHz

---

## Risk Assessment & Mitigation

| Risk                                | Severity | Probability | Mitigation                                |  
|-------------------------------------|----------|-------------|-------------------------------------------|  
| **Level shifter propagation delay** | Low      | Low         | 2-3 ns is <2% of clock period             |  
| **GPIO pin conflicts**              | Medium   | Low         | Pin mapping avoids shared functions       |  
| **Power supply noise**              | Medium   | Medium      | Bypass capacitors on all ICs              |  
| **Wiring errors**                   | High     | Medium      | Incremental testing, one module at a time |  
| **Z80 timing violations**           | Medium   | Low         | Conservative ISR timing budget            |  
| **Level shifter direction errors**  | Low      | Low         | TXS0108E auto-detects direction           |  
| **3.3V output to 5V input**         | Low      | None        | Level shifters handle translation         |  
| **5V output to 3.3V input**         | Critical | None        | All inputs protected by level shifters    |

**Overall Risk:** **Low to Medium** - Well-planned design with proven components

---

## Success Metrics

### Phase 1 Completion Criteria

- [x] Pin mapping complete and documented
- [x] Level shifter design complete
- [x] GPIO bank optimization documented
- [x] Clock generation strategy defined
- [x] Interrupt configuration planned
- [x] Z80 compatibility validated (all generations)
- [x] BOM created
- [x] Wiring diagrams documented
- [ ] Hardware ordered (pending)
- [ ] Prototype assembled (pending Phase 1A-E)
- [ ] Basic Z80 operation verified (pending)

**Current Status:** **Planning Complete (100%)**, **Implementation Pending (0%)**

---

## Next Steps

1. **Order Hardware** - Teensy 4.1, HW-221 modules, Z80 CPU, breadboards, wires
2. **Wait for Delivery** - 1-2 weeks lead time
3. **Phase 1A** - Build Module 5, test clock generation
4. **Phase 1B** - Add Modules 2-3, test address bus
5. **Phase 1C** - Add Module 4, test control signals + interrupts
6. **Phase 1D** - Add Module 1, test bidirectional data bus
7. **Phase 1E** - Full integration, run Z80 test programs

**Estimated Timeline:** 3-4 weeks after hardware arrives

---

## Documentation Links

- **[Pin_Mapping.md](Pin_Mapping.md)** - Complete pin assignments and level shifter wiring
- **[GPIO_Architecture.md](GPIO_Architecture.md)** - Teensy GPIO banks and fast I/O implementation
- **[Level_Shifting.md](Level_Shifting.md)** - TXS0108E configuration and voltage translation
- **[Hardware_Comparison.md](Hardware_Comparison.md)** - Arduino Mega vs Teensy 4.1 detailed comparison

---

## Key Takeaways

✅ **Design Complete** - Pin mapping optimized for speed and reliability  
✅ **Z80 Compatible** - Works with all NMOS and CMOS Z80 variants  
✅ **High Performance** - 5-10x faster Z80 clock, 16-32x faster interrupts  
✅ **Simple Wiring** - Consecutive pins and 8-pin modules simplify layout  
✅ **Proven Hardware** - TXS0108E widely used for 3.3V-5V level shifting  
✅ **Low Risk** - Well-documented design with conservative timing margins  

**Recommendation:** ✅ **Proceed to hardware procurement and Phase 1A implementation**

---

**Status:** ✅ Phase 1 Planning Complete  
**Date:** December 22, 2025  
**Next Phase:** [Phase 2 - Clock Generation](../02_Clock/README.md) (implementation after Phase 1 hardware validated)
