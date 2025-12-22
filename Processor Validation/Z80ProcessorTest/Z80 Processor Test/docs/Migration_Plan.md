# Z80 Processor Validation Suite - Migration Plan
## Arduino Mega 2560 → Teensy 4.1-NE-Lockable

**Date Created:** December 22, 2025  
**Status:** 📝 Planning Phase  
**Target Completion:** TBD

---

## Executive Summary

This document outlines the high-level migration plan for porting the Z80 Processor Validation Suite from Arduino Mega 2560 to Teensy 4.1-NE-Lockable. The migration involves significant architectural changes due to the platform differences (AVR ATmega2560 → ARM Cortex-M7).

### Key Challenges
- **Architecture Change**: 8-bit AVR → 32-bit ARM Cortex-M7
- **Clock Speed**: 16 MHz → 600 MHz (37.5x faster)
- **Memory Model**: Different Flash/RAM organization
- **I/O System**: PORT registers → GPIO registers
- **Timer System**: AVR Timer1 → FlexPWM/QuadTimer
- **Interrupt System**: External INT → NVIC GPIO interrupts

### Expected Benefits
- ✅ **Much Higher Performance**: 600 MHz enables faster Z80 clock speeds (>3 MHz)
- ✅ **More Memory**: 1MB Flash, 1MB RAM vs 256KB Flash, 8KB RAM
- ✅ **Lower Latency**: Faster interrupt response times
- ✅ **Better I/O**: More flexible GPIO configuration
- ✅ **Future Expandability**: More peripherals and features available

---

## Project Analysis

### Current Arduino Mega 2560 Implementation

**Hardware Interface:**
- 8-bit data bus on PORTA (pins 22-29)
- 16-bit address bus on PORTC + PORTK (pins 30-37, 62-69)
- Control signals on PORTB, PORTL, PORTE
- Clock output via Timer1 on pin 11 (OC1A)
- Interrupts on INT4 (WR) and INT5 (RD)

**Software Architecture:**
- Direct PORT manipulation for speed
- Hardware PWM clock generation (100 KHz - 3 MHz)
- Interrupt-driven memory emulation
- SRAM cache for fast program access (up to 1024 bytes)
- PROGMEM for constant data storage
- Performance-optimized ISRs with compiler pragmas

**Current Status:**
- ✅ Hardware interface complete and tested
- ✅ Clock generation working (Timer1)
- ✅ ISR framework implemented
- ✅ Performance testing and benchmarks functional
- ✅ Menu system and test framework operational
- 🚧 Phase 3 optimization in progress (debugging required)

---

## Migration Phases

### Phase 1: Hardware Abstraction & Pin Mapping
**Status:** 📝 Not Started  
**Estimated Effort:** 1-2 weeks  
**Dependencies:** None

**Objectives:**
- Map all Z80 signals to Teensy 4.1 GPIO pins
- Identify optimal GPIO banks for parallel I/O
- Determine voltage level requirements (3.3V vs 5V)
- Design level shifter circuitry if needed
- Create new pin definitions header

**Deliverables:**
- Complete pin mapping spreadsheet
- Updated `pins.h` for Teensy
- Hardware connection diagram
- Level shifter schematic (if required)

**See:** [01_Hardware/](01_Hardware/)

---

### Phase 2: Clock Generation System
**Status:** 📝 Not Started  
**Estimated Effort:** 1 week  
**Dependencies:** Phase 1

**Objectives:**
- Select appropriate Teensy timer (FlexPWM or QuadTimer)
- Implement clock generation with 50% duty cycle
- Support 100 KHz - 3+ MHz range
- Port clock control API from Arduino implementation
- Test clock stability and jitter

**Deliverables:**
- New `clock.cpp/clock.h` for Teensy
- Timer configuration code
- Clock testing and validation results
- Documentation of timer selection rationale

**See:** [02_Clock/](02_Clock/)

---

### Phase 3: Bus I/O Operations
**Status:** 📝 Not Started  
**Estimated Effort:** 2-3 weeks  
**Dependencies:** Phase 1

**Objectives:**
- Replace AVR PORT operations with Teensy GPIO registers
- Implement fast parallel bus read/write
- Maintain inline function performance
- Port HAL interface functions
- Optimize for ARM architecture

**Deliverables:**
- Updated `hal.cpp/hal.h` for Teensy
- GPIO register access macros
- Performance benchmarks for bus operations
- Bus I/O test suite

**See:** [03_Bus_IO/](03_Bus_IO/)

---

### Phase 4: Interrupt System
**Status:** 📝 Not Started  
---

## Next Steps

1. Read through Phase 1 documentation (01_Hardware/)
2. Procure Teensy 4.1 hardware and level shifters
3. Set up development environment
4. Begin pin mapping and GPIO analysis
5. Create hardware prototype for testing

---

**Document Status:** 📝 Draft  
**Last Updated:** December 22, 2025  
**Next Review:** After Phase 1 completion
---

## Risk Assessment

### High Risk Items
1. **Voltage Compatibility** - Z80 is 5V, Teensy is 3.3V
   - *Mitigation:* Level shifters or voltage-tolerant design
   
2. **Timing Changes** - 600 MHz may require timing adjustments
   - *Mitigation:* Comprehensive timing analysis and testing
   
3. **GPIO Speed** - Teensy GPIO may have different characteristics
   - *Mitigation:* Early prototyping and benchmarking

### Medium Risk Items
1. **API Differences** - Arduino/AVR vs Teensy/ARM APIs
   - *Mitigation:* Create abstraction layer where needed
   
2. **Interrupt Latency** - Different interrupt architecture
   - *Mitigation:* NVIC priority tuning and testing

### Low Risk Items
1. **Memory Space** - Teensy has much more memory
2. **Build System** - PlatformIO supports both platforms
3. **USB Serial** - Well-supported on Teensy

---

## Timeline Estimate

| Phase | Duration | Dependencies |
|-------|----------|--------------|
| Phase 1: Hardware & Pins | 1-2 weeks | None |
| Phase 2: Clock | 1 week | Phase 1 |
| Phase 3: Bus I/O | 2-3 weeks | Phase 1 |
| Phase 4: Interrupts | 1-2 weeks | Phase 3 |
| Phase 5: Memory | 1 week | Phase 3, 4 |
| Phase 6: Timing | 1 week | Phase 2, 4 |
| Phase 7: Build | 2-3 days | Phase 1 |
| Phase 8: Testing | 2-3 weeks | All |
| **Total** | **10-14 weeks** | |

---

## Success Criteria

- ✅ All Z80 signals properly interfaced to Teensy
- ✅ Clock generation stable at 100 KHz - 3+ MHz
- ✅ Bus read/write operations functional
- ✅ ISRs responding reliably to Z80 memory requests
- ✅ All original test programs execute correctly
- ✅ Performance meets or exceeds Arduino Mega implementation
- ✅ Code compiles without errors or warnings
- ✅ Documentation complete and accurate
- Performance comparison data
- Memory usage analysis

**See:** [05_Memory/](05_Memory/)

---

### Phase 6: Timing & Synchronization
**Status:** 📝 Not Started  
**Estimated Effort:** 1 week  
**Dependencies:** Phase 2, 4

**Objectives:**
- Recalculate timing budgets for 600 MHz
- Measure ISR latency and cycle counts
- Determine maximum Z80 clock speed
- Update WAIT state and backpressure logic
- Validate timing with logic analyzer

**Deliverables:**
- Timing analysis documentation
- Updated ISR cycle counts
- Z80 speed limit testing results
- Timing validation test suite

**See:** [06_Timing/](06_Timing/)

---

### Phase 7: Build System & Configuration
**Status:** 📝 Not Started  
**Estimated Effort:** 2-3 days  
**Dependencies:** Phase 1

**Objectives:**
- Configure PlatformIO for Teensy 4.1
- Set compiler flags and optimization
- Configure USB serial parameters
- Set up debugging environment
- Document build process

**Deliverables:**
- Updated `platformio.ini`
- Build documentation
- Debug configuration
- Compilation verification

**See:** [07_Build/](07_Build/)

---

### Phase 8: Testing & Validation
**Status:** 📝 Not Started  
**Estimated Effort:** 2-3 weeks  
**Dependencies:** All previous phases

**Objectives:**
- Port test framework to Teensy
- Validate all hardware interfaces
- Run comprehensive instruction tests
- Performance benchmarking
- Regression testing
- Hardware verification with logic analyzer

**Deliverables:**
- Complete test suite results
- Performance benchmark comparison
- Hardware validation report
- Known issues and workarounds
- Final migration report

**See:** [08_Testing/](08_Testing/)

---

## Risk Assessment

### High Risk Items
1. **Voltage Compatibility** - Z80 is 5V, Teensy is 3.3V
   - *Mitigation:* Level shifters or voltage-tolerant design
   
2. **Timing Changes** - 600 MHz may require timing adjustments
   - *Mitigation:* Comprehensive timing analysis and testing
   
3. **GPIO Speed** - Teensy GPIO may have different characteristics
   - *Mitigation:* Early prototyping and benchmarking

### Medium Risk Items
1. **API Differences** - Arduino/AVR vs Teensy/ARM APIs
   - *Mitigation:* Create abstraction layer where needed
   
2. **Interrupt Latency** - Different interrupt architecture
   - *Mitigation:* NVIC priority tuning and testing

### Low Risk Items
1. **Memory Space** - Teensy has much more memory
2. **Build System** - PlatformIO supports both platforms
3. **USB Serial** - Well-supported on Teensy

---

## Timeline Estimate

| Phase | Duration | Dependencies |
|-------|----------|--------------|
| Phase 1: Hardware & Pins | 1-2 weeks | None |
| Phase 2: Clock | 1 week | Phase 1 |
| Phase 3: Bus I/O | 2-3 weeks | Phase 1 |
| Phase 4: Interrupts | 1-2 weeks | Phase 3 |
| Phase 5: Memory | 1 week | Phase 3, 4 |
| Phase 6: Timing | 1 week | Phase 2, 4 |
| Phase 7: Build | 2-3 days | Phase 1 |
| Phase 8: Testing | 2-3 weeks | All |
| **Total** | **10-14 weeks** | |

---

## Success Criteria

- ✅ All Z80 signals properly interfaced to Teensy
- ✅ Clock generation stable at 100 KHz - 3+ MHz
- ✅ Bus read/write operations functional
- ✅ ISRs responding reliably to Z80 memory requests
- ✅ All original test programs execute correctly
- ✅ Performance meets or exceeds Arduino Mega implementation
- ✅ Code compiles without errors or warnings
- ✅ Documentation complete and accurate

---

## Next Actions

**Immediate:**
1. Order hardware: Teensy 4.1, level shifters (TXS0108E, qty 6), breadboard
2. Install PlatformIO with Teensy platform: `pio platform install teensy`
3. Read `01_Hardware/README.md` for Phase 1 details

**Phase 1 Start:**
1. Create pin mapping spreadsheet (40+ Z80 signals → Teensy GPIO)
2. Select GPIO banks for optimal 8-bit data / 16-bit address bus access
3. Design level shifter circuit (3.3V ↔ 5V bidirectional)
4. Build hardware prototype
5. Create new `pins.h` for Teensy

---

**Last Updated:** December 22, 2025  
**For Continuation:** Read this file + `Getting_Started.md` + relevant phase README
---

## Critical Requirements & Risks

### Must Address
1. **5V ↔ 3.3V Level Shifting** - Z80 is 5V, Teensy is 3.3V (bidirectional shifters needed)
2. **ISR Performance** - Must complete in <150ns for 5+ MHz Z80 operation
3. **GPIO Speed** - Need fast parallel I/O for 8-bit data + 16-bit address buses

### Success Criteria
- Clock: 100 KHz - 5+ MHz, stable, 50% duty cycle
- ISR latency: <150 ns total response time
- Z80 operation: 5+ MHz sustained (vs 1 MHz on Arduino Mega)
- All original test programs execute correctly
- System stable for 1+ hour continuous operation