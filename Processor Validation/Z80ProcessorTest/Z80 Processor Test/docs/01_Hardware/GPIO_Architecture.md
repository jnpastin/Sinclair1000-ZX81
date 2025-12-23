# Teensy 4.1 GPIO Architecture & Fast I/O

**Status:** 📋 Planning Phase  
**Date:** December 22, 2025  
**Platform:** Teensy 4.1 (IMXRT1062 ARM Cortex-M7 @ 600 MHz)

---

## Overview

This document provides an in-depth look at the Teensy 4.1 (IMXRT1062) GPIO architecture and how to achieve maximum I/O performance for the Z80 processor validation suite.

---

## IMXRT1062 GPIO Overview

The IMXRT1062 processor has **9 GPIO banks** (GPIO1-GPIO9), each with 32 bits:

| GPIO Bank | Physical Pins | Teensy 4.1 Pins | Notes                 |
|-----------|---------------|------------------|-----------------------|
| GPIO1     | GPIO1[0-31]   | Limited exposure | Mostly internal       |
| GPIO2     | GPIO2[0-31]   | Limited exposure | Mostly internal       |
| GPIO3     | GPIO3[0-31]   | Limited exposure | Mostly internal       |
| GPIO4     | GPIO4[0-31]   | Limited exposure | Mostly internal       |
| GPIO5     | GPIO5[0-31]   | Limited exposure | Mostly internal       |
| **GPIO6** | **GPIO6[0-31]** | **Pins 0-23**    | **Primary user GPIO** |
| **GPIO7** | **GPIO7[0-31]** | **Pins 6-15**    | **User GPIO**         |
| **GPIO8** | **GPIO8[0-31]** | **Pins 28-38**   | **User GPIO**         |
| **GPIO9** | **GPIO9[0-31]** | **Pins 2-41**    | **User GPIO**         |

**Key Points:**
- Each GPIO bank has its own set of 32-bit control registers
- Multiple pins can be read/written atomically within a bank
- Direct register access is **much faster** than `digitalWrite()`/`digitalRead()`
- All GPIO pins are **3.3V logic only** (NOT 5V tolerant - VIN/VUSB only)

---

## GPIO Register Map

Each GPIO bank (GPIOx) has the following key registers:

| Register          | Offset | Description               | Access |
|-------------------|--------|---------------------------|---------|
| **GPIOx_DR**      | 0x00   | Data Register             | R/W     |
| **GPIOx_GDIR**    | 0x04   | Direction Register        | R/W     |
| **GPIOx_PSR**     | 0x08   | Pad Status Register       | R       |
| **GPIOx_ICR1**    | 0x0C   | Interrupt Config 1        | R/W     |
| **GPIOx_ICR2**    | 0x10   | Interrupt Config 2        | R/W     |
| **GPIOx_IMR**     | 0x14   | Interrupt Mask            | R/W     |
| **GPIOx_ISR**     | 0x18   | Interrupt Status          | R/W1C   |
| **GPIOx_EDGE_SEL**| 0x1C   | Edge Select               | R/W     |

### Register Details

#### GPIOx_DR (Data Register)
- **Read**: Returns current state of GPIO pins
- **Write**: Sets output values for pins configured as outputs
- Each bit corresponds to one GPIO pin in the bank
- Reading when pin is input returns current logic level
- Writing when pin is output sets the output level

#### GPIOx_GDIR (Direction Register)
- **0** = Input
- **1** = Output
- Must be configured before using pin as output

#### GPIOx_PSR (Pad Status Register)
- Always reflects actual pin state regardless of direction
- Use for reading inputs (more reliable than DR in some cases)

---

## Direct Register Access

### Basic GPIO Operations

```cpp
// Include for GPIO register definitions
#include <imxrt.h>

// =============================================================================
// READING GPIO PINS
// =============================================================================

// Read single pin (pin 14 = GPIO6 bit 18, A0)
bool pinState = (GPIO6_DR >> 18) & 1;

// Read multiple pins atomically (entire GPIO6 bank)
uint32_t gpio6_state = GPIO6_DR;
bool pin14 = (gpio6_state >> 18) & 1;  // A0
bool pin15 = (gpio6_state >> 19) & 1;  // A1
bool pin16 = (gpio6_state >> 23) & 1;  // A2
// ... etc

// =============================================================================
// WRITING GPIO PINS
// =============================================================================

// Set pin HIGH (pin 36 = GPIO8 bit 28)
GPIO8_DR_SET = (1 << 28);  // Atomic set

// Set pin LOW (pin 36 = GPIO8 bit 28)
GPIO8_DR_CLEAR = (1 << 28);  // Atomic clear

// Toggle pin (pin 36 = GPIO8 bit 28)
GPIO8_DR_TOGGLE = (1 << 28);  // Atomic toggle

// Write multiple pins atomically
GPIO8_DR_SET = (1 << 28) | (1 << 29) | (1 << 30);  // Set pins 36, 37, 38

// =============================================================================
// CONFIGURING DIRECTION
// =============================================================================

// Configure pin as output (pin 36 = GPIO8 bit 28)
GPIO8_GDIR |= (1 << 28);

// Configure pin as input (pin 28 = GPIO8 bit 18, /RD)
GPIO8_GDIR &= ~(1 << 18);

// Configure multiple pins as outputs
GPIO8_GDIR |= (1 << 28) | (1 << 29) | (1 << 30);  // Pins 36, 37, 38
```

### Performance Characteristics

| Operation           | Arduino API | Direct Register | Speedup      |
|---------------------|-------------|-----------------|---------------|
| Single pin read     | ~150 ns     | ~5 ns           | **30x**      |
| Single pin write    | ~150 ns     | ~5 ns           | **30x**      |
| 8-bit parallel read | ~1200 ns    | ~10-20 ns       | **60-120x**  |
| 16-bit parallel read| ~2400 ns    | ~15-30 ns       | **80-160x**  |**Note:** Times at 600 MHz. Arduino API includes function call overhead, bounds checking, and pin mapping lookup.

---

## Teensy Pin to GPIO Mapping

### Pin Mapping Table (Z80 Project Pins)

| Teensy Pin | GPIO Bank | Bit | Arduino Pin | Z80 Signal |  
|------------|-----------|-----|-------------|------------|  
| 0          | GPIO6     | 3   | 0           | A10        |  
| 1          | GPIO6     | 2   | 1           | A11        |  
| 2          | GPIO9     | 4   | 2           | /RESET     |  
| 3          | GPIO9     | 5   | 3           | /WAIT      |  
| 4          | GPIO9     | 6   | 4           | /INT       |  
| 5          | GPIO9     | 8   | 5           | /NMI       |  
| 6          | GPIO7     | 10  | 6           | D4         |  
| 7          | GPIO7     | 17  | 7           | D3         |  
| 8          | GPIO7     | 16  | 8           | D5         |  
| 9          | GPIO7     | 11  | 9           | D6         |  
| 10         | GPIO7     | 0   | 10          | D2         |  
| 11         | GPIO7     | 2   | 11          | D7         |  
| 12         | GPIO7     | 1   | 12          | D0         |  
| 13         | GPIO7     | 3   | 13          | D1         |  
| 14         | GPIO6     | 18  | 14          | A0         |  
| 15         | GPIO6     | 19  | 15          | A1         |  
| 16         | GPIO6     | 23  | 16          | A2         |  
| 17         | GPIO6     | 22  | 17          | A3         |  
| 18         | GPIO6     | 17  | 18          | A4         |  
| 19         | GPIO6     | 16  | 19          | A5         |  
| 20         | GPIO6     | 26  | 20          | A6         |  
| 21         | GPIO6     | 27  | 21          | A7         |  
| 22         | GPIO6     | 24  | 22          | A8         |  
| 23         | GPIO6     | 25  | 23          | A9         |  
| 24         | GPIO6     | 12  | 24          | A12        |  
| 25         | GPIO6     | 13  | 25          | A13        |  
| 26         | GPIO6     | 30  | 26          | A14        |  
| 27         | GPIO6     | 31  | 27          | A15        |  
| 28         | GPIO8     | 18  | 28          | /RD        |  
| 29         | GPIO9     | 31  | 29          | /RFSH      |  
| 30         | GPIO8     | 23  | 30          | /WR        |  
| 31         | GPIO8     | 22  | 31          | /MREQ      |  
| 36         | GPIO8     | 28  | 36          | CLK        |  
| 37         | GPIO8     | 29  | 37          | /HALT      |  
| 38         | GPIO8     | 30  | 38          | /BUSACK    |  
| 39         | GPIO8     | 31  | 39          | /M1        |  
| 40         | GPIO9     | 16  | 40          | /IORQ      |  
| 41         | GPIO9     | 17  | 41          | /BUSREQ    |**Complete mapping:** https://www.pjrc.com/teensy/pinout.html

---

## Optimized I/O Functions

### Data Bus Operations (D0-D7)

**Challenge:** Data bus spread across multiple GPIO banks (GPIO6, 7, 9)

**Solution:** Use fast bit manipulation or lookup table

```cpp
// =============================================================================
// DATA BUS READ (8-bit parallel)
// =============================================================================

inline uint8_t readDataBus() {
  // Pins: 0(D4), 1(D3), 2(D5), 3(D6), 4(D2), 5(D7), 6(D0), 7(D1)
  // Banks: GPIO6(0,1), GPIO9(2,3,4,5), GPIO7(6,7)
  
  uint32_t gpio6 = GPIO6_PSR;
  uint32_t gpio7 = GPIO7_PSR;
  uint32_t gpio9 = GPIO9_PSR;
  
  uint8_t data = 0;
  data |= ((gpio7 >> 10) & 1) << 0;  // D0 (pin 6, GPIO7 bit 10)
  data |= ((gpio7 >> 17) & 1) << 1;  // D1 (pin 7, GPIO7 bit 17)
  data |= ((gpio9 >> 6) & 1) << 2;   // D2 (pin 4, GPIO9 bit 6)
  data |= ((gpio6 >> 2) & 1) << 3;   // D3 (pin 1, GPIO6 bit 2)
  data |= ((gpio6 >> 3) & 1) << 4;   // D4 (pin 0, GPIO6 bit 3)
  data |= ((gpio9 >> 4) & 1) << 5;   // D5 (pin 2, GPIO9 bit 4)
  data |= ((gpio9 >> 5) & 1) << 6;   // D6 (pin 3, GPIO9 bit 5)
  data |= ((gpio9 >> 8) & 1) << 7;   // D7 (pin 5, GPIO9 bit 8)
  
  return data;
}
// Performance: ~10-15 ns (3 register reads + bit manipulation)
// vs ~1200 ns with digitalWrite() = **80-120x faster**

// =============================================================================
// DATA BUS WRITE (8-bit parallel)
// =============================================================================

inline void writeDataBus(uint8_t data) {
  // Set direction to output for data pins
  GPIO6_GDIR |= (1 << 2) | (1 << 3);  // D3, D4
  GPIO7_GDIR |= (1 << 10) | (1 << 17);  // D0, D1
  GPIO9_GDIR |= (1 << 4) | (1 << 5) | (1 << 6) | (1 << 8);  // D2, D5, D6, D7
  
  // Write data to pins
  if (data & 0x01) GPIO7_DR_SET = (1 << 10); else GPIO7_DR_CLEAR = (1 << 10);  // D0
  if (data & 0x02) GPIO7_DR_SET = (1 << 17); else GPIO7_DR_CLEAR = (1 << 17);  // D1
  if (data & 0x04) GPIO9_DR_SET = (1 << 6);  else GPIO9_DR_CLEAR = (1 << 6);   // D2
  if (data & 0x08) GPIO6_DR_SET = (1 << 2);  else GPIO6_DR_CLEAR = (1 << 2);   // D3
  if (data & 0x10) GPIO6_DR_SET = (1 << 3);  else GPIO6_DR_CLEAR = (1 << 3);   // D4
  if (data & 0x20) GPIO9_DR_SET = (1 << 4);  else GPIO9_DR_CLEAR = (1 << 4);   // D5
  if (data & 0x40) GPIO9_DR_SET = (1 << 5);  else GPIO9_DR_CLEAR = (1 << 5);   // D6
  if (data & 0x80) GPIO9_DR_SET = (1 << 8);  else GPIO9_DR_CLEAR = (1 << 8);   // D7
}
// Performance: ~15-20 ns (direction set + 8 atomic writes)
```

### Address Bus Operations (A0-A15)

**Advantage:** Address high byte (A8-A15) entirely on GPIO6!

```cpp
// =============================================================================
// ADDRESS BUS READ (16-bit parallel) - HIGHLY OPTIMIZED
// =============================================================================

inline uint16_t readAddressBus() {
  // Address low byte (A0-A7): pins 8-15, mostly GPIO7 + some GPIO6
  uint32_t gpio7 = GPIO7_PSR;
  uint32_t gpio6_low = GPIO6_PSR;
  
  uint8_t addr_low = 0;
  addr_low |= ((gpio7 >> 16) & 1) << 0;  // A0 (pin 8, GPIO7 bit 16)
  addr_low |= ((gpio7 >> 11) & 1) << 1;  // A1 (pin 9, GPIO7 bit 11)
  addr_low |= ((gpio7 >> 0) & 1) << 2;   // A2 (pin 10, GPIO7 bit 0)
  addr_low |= ((gpio7 >> 2) & 1) << 3;   // A3 (pin 11, GPIO7 bit 2)
  addr_low |= ((gpio7 >> 1) & 1) << 4;   // A4 (pin 12, GPIO7 bit 1)
  addr_low |= ((gpio7 >> 3) & 1) << 5;   // A5 (pin 13, GPIO7 bit 3)
  addr_low |= ((gpio6_low >> 18) & 1) << 6;  // A6 (pin 14, GPIO6 bit 18)
  addr_low |= ((gpio6_low >> 19) & 1) << 7;  // A7 (pin 15, GPIO6 bit 19)
  
  // Address high byte (A8-A15): pins 16-23, ALL GPIO6 (OPTIMIZED!)
  uint32_t gpio6_high = GPIO6_PSR;  // Same register as above, cached
  
  uint8_t addr_high = 0;
  addr_high |= ((gpio6_high >> 23) & 1) << 0;  // A8  (pin 16, GPIO6 bit 23)
  addr_high |= ((gpio6_high >> 22) & 1) << 1;  // A9  (pin 17, GPIO6 bit 22)
  addr_high |= ((gpio6_high >> 17) & 1) << 2;  // A10 (pin 18, GPIO6 bit 17)
  addr_high |= ((gpio6_high >> 16) & 1) << 3;  // A11 (pin 19, GPIO6 bit 16)
  addr_high |= ((gpio6_high >> 26) & 1) << 4;  // A12 (pin 20, GPIO6 bit 26)
  addr_high |= ((gpio6_high >> 27) & 1) << 5;  // A13 (pin 21, GPIO6 bit 27)
  addr_high |= ((gpio6_high >> 24) & 1) << 6;  // A14 (pin 22, GPIO6 bit 24)
  addr_high |= ((gpio6_high >> 25) & 1) << 7;  // A15 (pin 23, GPIO6 bit 25)
  
  return ((uint16_t)addr_high << 8) | addr_low;
}
// Performance: ~15-20 ns (2 register reads + bit manipulation)
// vs ~2400 ns with digitalWrite() = **120-160x faster**
```

### Control Signal Operations

```cpp
// =============================================================================
// CONTROL SIGNAL READS (Z80 outputs → Teensy inputs)
// =============================================================================

// Most control inputs on GPIO6, a few on GPIO8/9
inline bool isReadCycle() {
  return (GPIO6_PSR >> 12) & 1;  // /RD on pin 24 (GPIO6 bit 12)
}

inline bool isWriteCycle() {
  return (GPIO6_PSR >> 13) & 1;  // /WR on pin 25 (GPIO6 bit 13)
}

inline bool isMemoryRequest() {
  return (GPIO6_PSR >> 30) & 1;  // /MREQ on pin 26 (GPIO6 bit 30)
}

inline bool isIORequest() {
  return (GPIO6_PSR >> 31) & 1;  // /IORQ on pin 27 (GPIO6 bit 31)
}

inline bool isM1Cycle() {
  return (GPIO8_PSR >> 18) & 1;  // /M1 on pin 28 (GPIO8 bit 18)
}

inline bool isRefreshCycle() {
  return (GPIO9_PSR >> 31) & 1;  // /RFSH on pin 29 (GPIO9 bit 31)
}

inline bool isHalted() {
  return (GPIO8_PSR >> 23) & 1;  // /HALT on pin 30 (GPIO8 bit 23)
}

inline bool isBusAcknowledged() {
  return (GPIO8_PSR >> 22) & 1;  // /BUSACK on pin 31 (GPIO8 bit 22)
}

// =============================================================================
// CONTROL SIGNAL WRITES (Teensy outputs → Z80 inputs)
// =============================================================================

// All control outputs on GPIO8/9
inline void assertReset() {
  GPIO8_DR_CLEAR = (1 << 29);  // /RESET on pin 37 (active low)
}

inline void deassertReset() {
  GPIO8_DR_SET = (1 << 29);  // /RESET on pin 37 (inactive high)
}

inline void assertWait() {
  GPIO8_DR_CLEAR = (1 << 30);  // /WAIT on pin 38 (active low)
}

inline void deassertWait() {
  GPIO8_DR_SET = (1 << 30);  // /WAIT on pin 38 (inactive high)
}

inline void assertInterrupt() {
  GPIO8_DR_CLEAR = (1 << 31);  // /INT on pin 39 (active low)
}

inline void deassertInterrupt() {
  GPIO8_DR_SET = (1 << 31);  // /INT on pin 39 (inactive high)
}

inline void assertNMI() {
  GPIO9_DR_CLEAR = (1 << 16);  // /NMI on pin 40 (active low)
}

inline void deassertNMI() {
  GPIO9_DR_SET = (1 << 16);  // /NMI on pin 40 (inactive high)
}

inline void assertBusRequest() {
  GPIO9_DR_CLEAR = (1 << 17);  // /BUSREQ on pin 41 (active low)
}

inline void deassertBusRequest() {
  GPIO9_DR_SET = (1 << 17);  // /BUSREQ on pin 41 (inactive high)
}

// Performance: ~5 ns per operation (single register write)
```

---

## GPIO Initialization

```cpp
// =============================================================================
// GPIO INITIALIZATION
// =============================================================================

void initializeGPIO() {
  // Configure data bus as inputs initially (bidirectional)
  GPIO6_GDIR &= ~((1 << 2) | (1 << 3));  // D3, D4
  GPIO7_GDIR &= ~((1 << 10) | (1 << 17));  // D0, D1
  GPIO9_GDIR &= ~((1 << 4) | (1 << 5) | (1 << 6) | (1 << 8));  // D2, D5-D7
  
  // Configure address bus as inputs (Z80 outputs)
  GPIO6_GDIR &= ~((1 << 18) | (1 << 19) |  // A6, A7
                  (1 << 16) | (1 << 17) | (1 << 22) | (1 << 23) |  // A8-A11
                  (1 << 24) | (1 << 25) | (1 << 26) | (1 << 27));  // A12-A15
  GPIO7_GDIR &= ~((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) |  // A2-A5
                  (1 << 11) | (1 << 16));  // A0, A1
  
  // Configure control inputs (Z80 outputs)
  GPIO6_GDIR &= ~((1 << 12) | (1 << 13) | (1 << 30) | (1 << 31));  // RD, WR, MREQ, IORQ
  GPIO8_GDIR &= ~((1 << 18) | (1 << 22) | (1 << 23));  // M1, BUSACK, HALT
  GPIO9_GDIR &= ~(1 << 31);  // RFSH
  
  // Configure control outputs (Teensy outputs to Z80 inputs)
  GPIO8_GDIR |= ((1 << 28) | (1 << 29) | (1 << 30) | (1 << 31));  // CLK, RESET, WAIT, INT
  GPIO9_GDIR |= ((1 << 16) | (1 << 17));  // NMI, BUSREQ
  
  // Initialize control outputs to inactive state (HIGH for active-low signals)
  GPIO8_DR_SET = (1 << 29) | (1 << 30) | (1 << 31);  // RESET, WAIT, INT high
  GPIO9_DR_SET = (1 << 16) | (1 << 17);  // NMI, BUSREQ high
  
  // Note: CLK (pin 36, GPIO8 bit 28) initialized by FlexPWM in Phase 2
}
```

---

## Comparison: AVR vs ARM GPIO

| Feature                | Arduino Mega (AVR)     | Teensy 4.1 (ARM)          | Advantage           |
|------------------------|------------------------|---------------------------|---------------------|
| **Architecture**       | 8-bit PORTs            | 32-bit GPIO banks         | ARM (wider)         |
| **Register Access**    | PORTA, PORTB, etc.     | GPIO6_DR, GPIO7_DR, etc.  | Similar             |
| **Atomic Operations**  | Single PORT            | Single bank (32 bits)     | ARM (wider)         |
| **Pin Grouping**       | 8 pins per PORT        | 32 pins per GPIO          | ARM (more flexible) |
| **Performance**        | ~125 ns/op @ 16 MHz    | ~5 ns/op @ 600 MHz        | **ARM (25x)**       |
| **API Simplicity**     | `PORTA = value`        | `GPIO6_DR = value`        | Equal               |
| **Multiple Banks**     | Yes (A, B, C, etc.)    | Yes (6, 7, 8, 9)          | Equal               |**Key Takeaway:** ARM GPIO is similar in concept to AVR PORTs, just **much faster** and **wider**.

---

## Best Practices

### 1. Use Direct Register Access
```cpp
// BAD (slow, ~150 ns)
digitalWrite(pin, HIGH);

// GOOD (fast, ~5 ns)
GPIO8_DR_SET = (1 << 28);
```

### 2. Cache Register Reads
```cpp
// BAD (multiple register reads)
bool rd = (GPIO6_PSR >> 12) & 1;
bool wr = (GPIO6_PSR >> 13) & 1;
bool mreq = (GPIO6_PSR >> 30) & 1;

// GOOD (single register read)
uint32_t gpio6 = GPIO6_PSR;
bool rd = (gpio6 >> 12) & 1;
bool wr = (gpio6 >> 13) & 1;
bool mreq = (gpio6 >> 30) & 1;
```

### 3. Use Inline Functions
```cpp
// Force inlining for zero function call overhead
inline __attribute__((always_inline)) uint16_t readAddressBus() {
  // ... implementation
}
```

### 4. Group Related Pins on Same Bank
```cpp
// GOOD: A8-A15 all on GPIO6 → single register read
// LESS OPTIMAL: D0-D7 spread across GPIO6, 7, 9 → multiple reads
```

### 5. Use Atomic Set/Clear Operations
```cpp
// GOOD (atomic, no read-modify-write race)
GPIO8_DR_SET = (1 << 28);    // Set pin 36
GPIO8_DR_CLEAR = (1 << 29);  // Clear pin 37

// AVOID (read-modify-write, not atomic)
GPIO8_DR |= (1 << 28);
GPIO8_DR &= ~(1 << 29);
```

---

## Performance Summary

| Operation           | Arduino Mega | Teensy 4.1 | Speedup        |
|---------------------|--------------|------------|----------------|
| Single GPIO read    | 125 ns       | 5 ns       | **25x**        |
| Single GPIO write   | 125 ns       | 5 ns       | **25x**        |
| 8-bit bus read      | 1000 ns      | 10-15 ns   | **67-100x**    |
| 16-bit bus read     | 2000 ns      | 15-20 ns   | **100-133x**   |
| ISR entry latency   | 1.6 µs       | ~50 ns     | **32x**        |
| Clock frequency     | 16 MHz       | 600 MHz    | **37.5x**      |**Result:** Teensy 4.1 can support **5-10 MHz Z80 operation** vs 1 MHz on Arduino Mega.

---

## Next Steps

1. **Create `hal.h` skeleton** - Define inline GPIO functions
2. **Implement data bus functions** - Read/write with multi-bank optimization
3. **Implement address bus functions** - Leverage GPIO6 for A8-A15
4. **Test GPIO performance** - Benchmark read/write operations
5. **Optimize ISR paths** - Minimize cycles in memory handler

---

**Status:** ✅ Complete - Ready for implementation (Phase 3)  
**Next Document:** [Level_Shifting.md](Level_Shifting.md) - Voltage translation design
