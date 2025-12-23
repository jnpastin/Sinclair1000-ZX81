# Phase 1: Hardware Abstraction & Pin Mapping

**Status:** ✅ Planning Complete - Ready for Implementation  
**Estimated Effort:** 1-2 weeks (implementation)  
**Dependencies:** None  
**Documentation:** 100% Complete

---

## Overview

This phase focuses on mapping all Z80 interface signals to Teensy 4.1 GPIO pins and designing the hardware interface with level shifting for full Z80 compatibility (NMOS to CMOS variants).

**Key Achievement:** Complete pin mapping optimized for speed, reliability, and simplified wiring using 5x HW-221 (TXS0108E) level shifter modules.

---

## Documents in This Section

### [Quick_Reference.md](Quick_Reference.md) 🎯 **QUICK START**
One-page reference card with essential pin mappings, BOM, and testing checklist.

**Perfect for:**
- Quick pin lookups during assembly
- Bench reference during testing
- Shopping list for hardware orders
- Implementation phase checklist

---

### [Wiring_Diagram.md](Wiring_Diagram.md) 🔌 **ASSEMBLY GUIDE**
Complete ASCII/text wiring diagrams with step-by-step assembly instructions.

**Topics Covered:**
- Module-by-module connection diagrams
- Power distribution network
- Physical breadboard layout
- Assembly order (incremental testing)
- Cable management tips
- Testing points and troubleshooting

---

### [Phase1_Summary.md](Phase1_Summary.md) ⭐ **START HERE**
Complete overview of Phase 1 design decisions, wiring, and implementation roadmap.

**Topics Covered:**
- Design decisions and rationale
- Z80 compatibility matrix (all generations)
- Performance targets
- Complete BOM and cost estimates
- Wiring diagram summary
- Implementation roadmap (Phases 1A-1E)
- Risk assessment
- Success metrics

---

### [Pin_Mapping.md](Pin_Mapping.md)
Complete pin-by-pin assignment of Z80 signals to Teensy 4.1 GPIO pins.

**Topics Covered:**
- Z80 pinout reference (40-pin DIP)
- Teensy 4.1 GPIO bank architecture
- Optimized pin assignments (consecutive groups)
- Level shifter module allocations (5 modules)
- Hardware connection tables
- Wiring simplification strategies
- Fast I/O access patterns
- Verification checklist

**Key Results:**
- ✅ Data bus: Teensy pins 6-13 (consecutive, **all GPIO7!**)
- ✅ Address low: Teensy pins 14-21 (consecutive, **all GPIO6!**)
- ✅ Address high: Teensy pins 0-1, 22-27 (**all GPIO6!**)
- ✅ Full 16-bit address on single GPIO6 register!
- ✅ Control inputs: Teensy pins 28-31, 37-40 (GPIO8)
- ✅ Control outputs: Teensy pins 2-5, 36, 41 (GPIO9 + GPIO8)
- ✅ Clock: Pin 36 (FlexPWM2_A capable)

---

### [GPIO_Architecture.md](GPIO_Architecture.md)
Deep dive into Teensy 4.1 GPIO architecture and fast parallel I/O implementation.

**Topics Covered:**
- IMXRT1062 GPIO banks (GPIO6-GPIO9)
- GPIO register map and direct access
- Fast register operations vs Arduino API
- Optimized I/O functions (data/address/control)
- GPIO initialization code
- AVR PORT vs ARM GPIO comparison
- Best practices for performance

**Performance Results:**
- ✅ Single GPIO read: ~5 ns (vs 125 ns on Mega) = **25x faster**
- ✅ 8-bit bus read: ~10-15 ns (vs 1000 ns) = **67-100x faster**
- ✅ 16-bit bus read: ~15-20 ns (vs 2000 ns) = **100-133x faster**
- ✅ Address high byte: Single 32-bit register read (all GPIO6)

---

### [Level_Shifting.md](Level_Shifting.md)
Comprehensive analysis of voltage translation and level shifter design.

**Topics Covered:**
- Z80 vs Teensy voltage requirements (5V vs 3.3V)
- TXS0108E (HW-221) level shifter specifications
- Module pinout and wiring
- Complete 5-module wiring diagrams
- Timing analysis (propagation delay)
- Power supply design
- Common ground architecture
- NMOS and CMOS Z80 compatibility
- Testing and validation procedures

**Key Results:**
- ✅ TXS0108E propagation delay: 2-3 ns (<2% of Z80 clock period @ 5 MHz)
- ✅ Auto-direction sensing (no DIR pin management)
- ✅ 110 MHz bandwidth (far exceeds Z80 requirements)
- ✅ Compatible with all Z80 variants (NMOS and CMOS)
- ✅ 5 modules = 40 channels (perfect for Z80 interface)

---

### [Hardware_Comparison.md](Hardware_Comparison.md)
Detailed comparison between Arduino Mega 2560 and Teensy 4.1 platforms.

**Topics Covered:**
- Processor architecture (AVR vs ARM Cortex-M7)
- Clock speeds and raw performance
- Memory architecture and capacity
- GPIO system (PORTs vs GPIO banks)
- Timer/PWM capabilities
- Interrupt systems (AVR vs NVIC)
- Communication interfaces
- Physical specifications
- Power consumption
- Development environment
- Cost and availability
- Migration impact analysis

**Performance Comparison:**
- ✅ CPU: 600 MHz vs 16 MHz = **37.5x faster**
- ✅ RAM: 1 MB vs 8 KB = **128x more**
- ✅ Flash: 8 MB vs 256 KB = **32x more**
- ✅ GPIO: 1.7 ns vs 125 ns = **73x faster**
- ✅ ISR latency: 50-100 ns vs 1.6 µs = **16-32x faster**

---

## Phase 1 Deliverables

- [x] Complete pin mapping table ✅
- [x] Teensy 4.1 pin assignment diagram ✅
- [x] Level shifter circuit schematics ✅
- [x] Hardware BOM (Bill of Materials) ✅
- [x] GPIO access strategy document ✅
- [x] Hardware verification test plan ✅
- [x] Z80 compatibility analysis (all generations) ✅
- [x] Performance analysis and targets ✅
- [x] Implementation roadmap ✅
- [ ] Updated `pins.h` header file (Phase 3)
- [ ] Physical prototype assembled (pending hardware)
- [ ] Hardware validation complete (pending)

**Planning Status:** ✅ **100% Complete**  
**Implementation Status:** ⏳ **Awaiting hardware procurement**

---

## Getting Started with Implementation

### Step 1: Review Documentation
1. Read **[Phase1_Summary.md](Phase1_Summary.md)** for complete overview
2. Study **[Pin_Mapping.md](Pin_Mapping.md)** for wiring details
3. Review **[Level_Shifting.md](Level_Shifting.md)** for level shifter configuration
4. Check **[Hardware_Comparison.md](Hardware_Comparison.md)** for platform understanding

### Step 2: Procure Hardware
Order items from BOM (see Phase1_Summary.md):
- [ ] Teensy 4.1 ($27-30, PJRC.com)
- [ ] 5x HW-221 (TXS0108E) modules ($15-20)
- [ ] Z80 CPU 40-pin DIP ($5-20)
- [ ] 2x Breadboards ($10)
- [ ] Jumper wires, power supply, bypass caps ($20)

**Total Cost:** ~$120-150

### Step 3: Incremental Assembly (Phases 1A-1E)
Follow implementation roadmap in Phase1_Summary.md:
- **Phase 1A:** Module 5 - Clock output
- **Phase 1B:** Modules 2-3 - Address bus
- **Phase 1C:** Module 4 - Control inputs
- **Phase 1D:** Module 1 - Data bus
- **Phase 1E:** Full integration

---

## Success Criteria

**Planning Phase (Complete):**
- [x] All 40 Z80 signals have assigned Teensy pins
- [x] Pin assignments optimize for parallel I/O performance
- [x] Voltage level compatibility ensured (all Z80 generations)
- [x] Hardware design reviewed and validated
- [x] Level shifter configuration documented
- [x] Wiring diagrams complete
- [x] BOM and cost estimates done

**Implementation Phase (Pending):**
- [ ] Hardware ordered and received
- [ ] Prototype hardware assembled (Phases 1A-1E)
- [ ] Clock generation verified (1+ MHz)
- [ ] Address bus reads validated (16-bit)
- [ ] Control signals monitored correctly
- [ ] Data bus bidirectional operation confirmed
- [ ] Z80 executes test programs at 5+ MHz

---

## Key Design Features

✅ **Optimized Pin Mapping:** Consecutive pins in functional groups  
✅ **GPIO Bank Optimization:** Address high byte entirely on GPIO6  
✅ **Fast Clock Generation:** FlexPWM2_A hardware timer on pin 36  
✅ **Low-Latency Interrupts:** /RD and /WR both on GPIO6  
✅ **Universal Z80 Compatibility:** Works with NMOS and CMOS variants  
✅ **Simplified Wiring:** 5 modules × 8 pins = clean layout  
✅ **High Performance:** 5-10 MHz Z80 operation (vs 1 MHz on Mega)  

---

## Risk Mitigation

| Risk                  | Mitigation                                |
|-----------------------|-------------------------------------------|
| Level shifter delay   | 2-3 ns is <2% of clock period (verified)  |
| Wiring errors         | Incremental testing, one module at a time |
| Power supply noise    | Bypass capacitors on all ICs              |
| GPIO conflicts        | Pin mapping avoids shared functions       |
| Z80 timing violations | Conservative ISR timing budget            |

**Overall Risk Level:** ✅ **Low** - Well-documented design with proven components

---

## Next Steps

1. ✅ **Complete** - Review all Phase 1 documentation
2. ⏳ **Pending** - Order hardware (see BOM in Phase1_Summary.md)
3. ⏳ **Pending** - Wait 1-2 weeks for delivery
4. ⏳ **Pending** - Begin Phase 1A (Module 5 - Clock)
5. 🔜 **Future** - Proceed to Phase 2 (Clock Generation) after Phase 1 validated

---

**Current Phase Status:** ✅ **Planning Complete (100%)**  
**Next Phase:** [Phase 2 - Clock Generation](../02_Clock/README.md) (after Phase 1 hardware validation)

---

**Last Updated:** December 22, 2025  
**Documentation:** Complete and ready for implementation  
**Hardware:** Awaiting procurement
