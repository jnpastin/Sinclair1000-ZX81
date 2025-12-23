# Phase 1 Hardware - Quick Reference Card

**Date:** December 22, 2025  
**Status:** ✅ Planning Complete

---

## Pin Mapping (Quick Reference)

| Signal Group              | Teensy Pins     | Z80 Pins                | Level Shifter | GPIO Bank   |  
|---------------------------|-----------------|-------------------------|---------------|-------------|  
| **Data Bus (D0-D7)**      | 6-13            | 14,12,8,7,9,10,13,15    | Module 1      | **GPIO7**   |  
| **Address Low (A0-A7)**   | 14-21           | 30-37                   | Module 2      | **GPIO6**   |  
| **Address High (A8-A15)** | 0-1, 22-27      | 6,5,4,3,2,1,39,38       | Module 3      | **GPIO6**   |  
| **Control In**            | 28-31, 37-40    | 18-23, 27-28            | Module 4      | GPIO8,9     |  
| **Control Out + CLK**     | 2-5, 36, 41     | 6,16,17,24,25,26        | Module 5      | GPIO8,9     |

## Critical Pins

| Pin          | Function | Notes                                    |  
|--------------|----------|------------------------------------------|  
| **36**       | CLK      | FlexPWM2_A - Hardware PWM                |  
| **28**       | /RD      | Interrupt (falling edge)                 |  
| **30**       | /WR      | Interrupt (falling edge)                 |  
| **0-1,14-27**| A0-A15   | All GPIO6 - single register read! ✨     |  
| **6-13**     | D0-D7    | All GPIO7 - single register read/write! ✨|

---

## Level Shifter Modules (HW-221/TXS0108E)

| Module | Signals     | Type           | A-Side (3.3V) | B-Side (5V)     |  
|--------|-------------|----------------|---------------|-----------------|  
| **1**  | D0-D7       | Bidirectional  | Teensy 6-13   | Z80 data bus    |  
| **2**  | A0-A7       | Unidirectional | Teensy 14-21  | Z80 addr low    |  
| **3**  | A8-A15      | Unidirectional | Teensy 0-1,22-27 | Z80 addr high|  
| **4**  | Control In  | Unidirectional | Teensy 28-31,37-40 | Z80 status |  
| **5**  | Control Out | Unidirectional | Teensy 2-5,36,41 | Z80 control  |

**Power:** VCCA = 3.3V, VCCB = 5V, OE = 3.3V (HIGH)

---

## Performance Targets

| Metric        | Target        | vs Mega |  
|---------------|---------------|---------|  
| Max Z80 Clock | **5-10 MHz**  | 5-10x   |  
| ISR Latency   | **50-100 ns** | 16-32x  |  
| Address Read  | **15-20 ns**  | 100x    |  
| Data Read     | **10-15 ns**  | 67-100x |---

## Bill of Materials

| Item               | Qty | Cost     | Source |  
|--------------------|-----|----------|--------|  
| Teensy 4.1         | 1   | $30      | PJRC   |  
| HW-221 modules     | 5   | $15      | Amazon |  
| Z80 CPU            | 1   | $10      | eBay   |  
| Breadboards        | 2   | $10      | Amazon |  
| Misc (wires, caps) | -   | $15      | Amazon |  
| **Total**          | -   | **~$80** | -      |---

## GPIO Register Cheat Sheet

```cpp
// Ultra-fast GPIO operations
uint32_t gpio6 = GPIO6_PSR;     // Read all GPIO6 pins (1.7 ns)
uint32_t gpio7 = GPIO7_PSR;     // Read all GPIO7 pins (1.7 ns)
GPIO8_DR_SET = (1 << 28);       // Set pin 36 HIGH (atomic)
GPIO8_DR_CLEAR = (1 << 28);     // Set pin 36 LOW (atomic)

// Full 16-bit address bus - SINGLE GPIO6 register read!
uint32_t gpio6 = GPIO6_PSR;
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

// Full 8-bit data bus - SINGLE GPIO7 register read!
uint32_t gpio7 = GPIO7_PSR;
uint8_t data = 
  ((gpio7 >> 10) & 0x10) |  // D4 (pin 6)
  ((gpio7 >> 17) & 0x08) |  // D3 (pin 7)
  ((gpio7 >> 16) & 0x20) |  // D5 (pin 8)
  ((gpio7 >> 11) & 0x40) |  // D6 (pin 9)
  ((gpio7 >> 0)  & 0x04) |  // D2 (pin 10)
  ((gpio7 >> 2)  & 0x80) |  // D7 (pin 11)
  ((gpio7 >> 1)  & 0x01) |  // D0 (pin 12)
  ((gpio7 >> 3)  & 0x02);   // D1 (pin 13)
```

---

## Implementation Phases

| Phase     | Goal                        | Duration      |  
|-----------|-----------------------------|--------------| 
| **1A**    | Clock output (Module 5)     | 2-3 days     |  
| **1B**    | Address bus (Modules 2-3)   | 2-3 days     |  
| **1C**    | Control inputs (Module 4)   | 2-3 days     |  
| **1D**    | Data bus (Module 1)         | 3-4 days     |  
| **1E**    | Full integration            | 3-4 days     |  
| **Total** | Hardware validation         | **2-3 weeks**|

---

## Testing Checklist

- [ ] Power-up test (3.3V, 5V present)
- [ ] Clock output (1 MHz square wave)
- [ ] Address bus reads (all 16 bits)
- [ ] Control signal monitoring
- [ ] Interrupt triggering (/RD, /WR)
- [ ] Data bus read (Z80 → Teensy)
- [ ] Data bus write (Teensy → Z80)
- [ ] Z80 NOP loop execution
- [ ] 1 MHz operation
- [ ] 5 MHz operation

---

## Z80 Compatibility

✅ **NMOS Z80** (Zilog Z8400, Mostek, NEC) - 2.5-4 MHz  
✅ **CMOS Z80** (Zilog Z84C00) - 4-20 MHz  
✅ All 40-pin DIP variants

**Voltage:** 5V (TXS0108E handles 3.3V ↔ 5V translation)

---

## Key Optimizations

🚀 **All 16 Address Bits on GPIO6** - Single register read for full address bus  
🚀 **All 8 Data Bits on GPIO7** - Single register read/write for full data bus  
🚀 **Clock Pin 36** - FlexPWM hardware PWM (zero CPU overhead)  
🚀 **Interrupts on GPIO8** - /RD (pin 28) and /WR (pin 30) both interrupt-capable  
🚀 **DIP Pin Order** - Teensy pins follow Z80 physical layout (minimal wire crossing)

---

## Next Actions

1. ⏳ Order hardware (~$80, 1-2 week delivery)
2. 📦 Receive hardware
3. 🔧 Phase 1A - Build Module 5, test clock
4. 🔧 Phase 1B-E - Incremental assembly
5. ✅ Full system validation
6. 🎯 Proceed to Phase 2 (Clock Generation software)

---

**Quick Links:**
- [Phase1_Summary.md](Phase1_Summary.md) - Complete overview
- [Pin_Mapping.md](Pin_Mapping.md) - Detailed pin assignments
- [Level_Shifting.md](Level_Shifting.md) - Voltage translation guide
- [GPIO_Architecture.md](GPIO_Architecture.md) - Fast I/O implementation

---

**Status:** ✅ Ready for hardware procurement and implementation  
**Print this card for bench reference during assembly!**
