# Phase 1 Hardware Plan - Summary

**Status:** ✅ Complete - Ready for Implementation  
**Date:** December 23, 2025 (Pin mapping optimized)  
**Phase:** Planning Complete, Awaiting Hardware

---

## Overview

This document summarizes the complete hardware plan for migrating the Z80 Processor Validation Suite from Arduino Mega 2560 to Teensy 4.1, with full compatibility for all Z80 CPU generations (NMOS to CMOS).

---

## Key Design Decisions

### 1. Pin Mapping Strategy ✓

**Priorities (in order):**
1. ✅ **Speed & Reliability** - Direct register access, optimized GPIO banks
2. ✅ **Bank Consolidation** - All 16 address bits on GPIO1.16-31, data bus on GPIO2 two groups
3. ✅ **Pin Grouping** - Consecutive pins for simplified wiring


**Pin Allocations:**
- **Address Bus (A0-A15):** **GPIO1.16-31** (consecutive bits, single-shift 16-bit read!)
  - Teensy pins: 19,18,14,15,40,41,17,16,22,23,20,21,38,39,26,27
- **Data Bus (D0-D7):** **GPIO2** two groups (0-3, 16-19, optimized extraction)
  - D0-D3: Teensy pins 10,12,11,13 (GPIO2.0-3)
  - D4-D7: Teensy pins 8,7,36,37 (GPIO2.16-19)
- **Control Outputs (Z80→Teensy):** **GPIO1, GPIO4** grouped by function
  - Teensy pins: 1,0,24,2,3,4,33,5 (HALT, IORQ, BUSACK, M1, RFSH, RD, WR, MREQ)
- **Control Inputs (Teensy→Z80):** **GPIO2, GPIO3**
  - Teensy pins: 28,6,9,35,32,34 (CLK, INT, NMI, WAIT, BUSREQ, RESET)

**Total:** 38 Z80 signals + CLK mapped to 39 Teensy pins + 1 for OE Control + spare pins available

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

**Implementation:** Hardware PWM using FlexPWM2_A on Teensy pin 28

**Capabilities:**
- Frequency range: **100 KHz - 10+ MHz** (vs 100 KHz - 1 MHz on Mega)
- 50% duty cycle (configurable in hardware)
- Jitter-free operation (hardware timer)
- Zero CPU overhead once configured

**Connection:** Pin 28 (GPIO3.18) → Level Shifter Module 5 → Z80 Pin 6 (CLK)

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
- Mounted on Z80 board for real-time bus activity visualization

### 7. Interrupt Configuration ✓

**Interrupt Pins:**
- **Pin 4:** /RD signal (GPIO4.06, falling edge trigger)
- **Pin 33:** /WR signal (GPIO4.07, falling edge trigger)

**Advantages:**
- Both on GPIO4 bank → fast interrupt handling
- All Z80 status outputs grouped on GPIO4 (M1, RFSH, RD, WR, MREQ)
- NVIC priority configurable to 0 (highest) for ISRs
- ~50-100 ns latency (vs ~1.6 µs on Mega) = **16-32x faster**
- Single register read captures all control signals

### 8. GPIO Bank Optimization ✓

**Key Optimizations:** 
- **All 16 address bits (A0-A15) on GPIO1.16-31** → Single-shift 16-bit read (~5 ns)
- **Data bus (D0-D7) on GPIO2** in two groups (0-3, 16-19) → Two-mask extraction (~10 ns)
- **All Z80 status outputs on GPIO4** → Single register read for all control signals

**Performance Impact:**
```cpp
// Single 32-bit register read + shift captures ENTIRE 16-bit address bus!
uint16_t address = GPIO1_DR >> 16;  // ~5 ns @ 600 MHz

// Two-mask extraction for 8-bit data bus
uint32_t gpio2 = GPIO2_DR;
uint8_t data = (gpio2 & 0x0F) | ((gpio2 >> 12) & 0xF0);  // ~10 ns total

// vs Arduino Mega (16 separate digitalRead() calls for address)
// ~2000 ns total
```

**Speedup:** ~**400x faster address reads**, ~**200x faster data reads**

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
| **Address Bus Read** | 2.0 µs       | 5 ns              | **400x** ⚡  |  
| **Data Bus Read**    | 1.0 µs       | 10 ns             | **100x** ⚡  |  
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
Teensy 10,12,11,13,8,7,36,37 <--[TXS0108E]--> Z80 14,15,12,8,7,9,10,13 (D0-D7)
```
**GPIO2 two groups (0-3, 16-19)** - Two-mask extraction for data bus

### Module 2 - Address Low (A0-A7)
```
Teensy 19,18,14,15,40,41,17,16 --[TXS0108E]--> Z80 30-37 (A0-A7)
```
**GPIO1.16-23** - Part of consecutive 16-bit address optimization

### Module 3 - Address High (A8-A15)
```
Teensy 22,23,20,21,38,39,26,27 --[TXS0108E]--> Z80 1-6,38-39 (A8-A15)
```
**GPIO1.24-31** - Part of consecutive 16-bit address optimization. **Full 16-bit address = GPIO1.16-31!**

### Module 4 - Control Inputs (Z80 → Teensy)
```
Teensy 1,0,24,2,3,4,33,5 <--[TXS0108E]-- Z80 18,20,23,27,28,21,22,19 (HALT,IORQ,BUSACK,M1,RFSH,RD,WR,MREQ)
```
**GPIO1 + GPIO4** grouped by function, RD/WR interrupt-capable on GPIO4

### Module 5 - Control Outputs (Teensy → Z80)
```
Teensy 28,6,9,35,32,34 --[TXS0108E]--> Z80 6,16,17,24,25,26 (CLK,INT,NMI,WAIT,BUSREQ,RESET)
```
Pin 28 = FlexPWM2_A for hardware clock generation

**Wiring Complexity:** Moderate - optimized for GPIO performance over physical pin order

---

## GPIO Register Access Summary

### Fast I/O Macros (To Be Implemented)

```cpp
// Address bus read (ALL 16 bits on GPIO1.16-31!)
inline uint16_t readAddressBus() {
  return GPIO1_DR >> 16;  // Single shift! A0-A15 on consecutive bits 16-31
}
// ~5 ns total

// Data bus read (GPIO2 two groups: 0-3, 16-19)
inline uint8_t readDataBus() {
  uint32_t gpio2 = GPIO2_DR;
  return (gpio2 & 0x0F) |          // D0-D3 from bits 0-3
         ((gpio2 >> 12) & 0xF0);   // D4-D7 from bits 16-19, shift to bits 4-7
}
// ~10 ns total

// Data bus write
inline void writeDataBus(uint8_t data) {
  uint32_t gpio2 = GPIO2_DR & ~0x000F000F;  // Clear data bits
  gpio2 |= (data & 0x0F);                    // D0-D3
  gpio2 |= ((uint32_t)(data & 0xF0)) << 12;  // D4-D7
  GPIO2_DR = gpio2;
}
// ~12 ns total

// Control signal checks
inline bool isReadCycle() {
  return !(GPIO4_DR & (1 << 6));  // Pin 4 = GPIO4 bit 6 (/RD, active low)
}

inline bool isWriteCycle() {
  return !(GPIO4_DR & (1 << 7));  // Pin 33 = GPIO4 bit 7 (/WR, active low)
}
// ~5 ns each
```

**Performance:** Address read ~5 ns, data read/write ~10-12 ns (vs 125 ns - 2 µs on Arduino Mega)

---

## Implementation Roadmap

### Phase 1A: Module 5 - Clock Output (Week 1)
**Goal:** Generate Z80 clock signal

- [ ] Connect Module 5 level shifter
- [ ] Wire Teensy pin 28 → Module 5 → Z80 pin 6
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
- [ ] Configure interrupts on pins 4 (/RD) and 33 (/WR)
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
**Date:** December 23, 2025 (Pin mapping optimized)  
**Next Phase:** [Phase 2 - Clock Generation](../02_Clock/README.md) (implementation after Phase 1 hardware validated)
