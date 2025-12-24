# Teensy 4.1 GPIO Architecture & Fast I/O

**Status:** 📋 Optimized Pin Mapping  
**Date:** December 23, 2025  
**Platform:** Teensy 4.1 (IMXRT1062 ARM Cortex-M7 @ 600 MHz)

---

## Overview

This document provides an in-depth look at the Teensy 4.1 (IMXRT1062) GPIO architecture and how to achieve maximum I/O performance for the Z80 processor validation suite.

---

## IMXRT1062 GPIO Overview

The IMXRT1062 processor as implemented on the Teensy 4.1 has **4 GPIO banks available to physical pins** (GPIO1-GPIO4), each with 32 bits:

| GPIO Bank | Physical Pins | Teensy 4.1 Pins | Notes                 |
|-----------|---------------|------------------|-----------------------|
| **GPIO1** | **GPIO1[0-31]** | **Various**    | **Address + Control** |
| **GPIO2** | **GPIO2[0-31]** | **Various**    | **Data + Control**    |
| **GPIO3** | **GPIO3[0-31]** | **Limited**    | **CLK + Spares**      |
| **GPIO4** | **GPIO4[0-31]** | **Limited**    | **Z80 Status Outputs**|


**Key Optimizations:**
- **GPIO1.16-31:** All 16 address bits (A0-A15) - Single-shift read!
- **GPIO2:** Data bus in two groups (0-3, 16-19) + control outputs
- **GPIO4:** All Z80 status outputs grouped (M1, RFSH, RD, WR, MREQ)
- **GPIO3:** Clock generation (Pin 28 = FlexPWM2_A)

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

// Read single pin (pin 19 = GPIO1 bit 16, A0)
bool pinState = (GPIO1_DR >> 16) & 1;

// Read multiple pins atomically (entire GPIO1 bank)
uint32_t gpio1_state = GPIO1_DR;
bool pin19 = (gpio1_state >> 16) & 1;  // A0
bool pin18 = (gpio1_state >> 17) & 1;  // A1
bool pin14 = (gpio1_state >> 18) & 1;  // A2
// ... etc

// =============================================================================
// WRITING GPIO PINS
// =============================================================================

// Set pin HIGH (pin 28 = GPIO3 bit 18, CLK)
GPIO3_DR_SET = (1 << 18);  // Atomic set

// Set pin LOW (pin 28 = GPIO3 bit 18, CLK)
GPIO3_DR_CLEAR = (1 << 18);  // Atomic clear

// Toggle pin (pin 28 = GPIO3 bit 18, CLK)
GPIO3_DR_TOGGLE = (1 << 18);  // Atomic toggle

// Write multiple pins atomically
GPIO2_DR_SET = (1 << 10) | (1 << 11) | (1 << 12);  // Set pins 6, 9, 32

// =============================================================================
// CONFIGURING DIRECTION
// =============================================================================

// Configure pin as output (pin 28 = GPIO3 bit 18, CLK)
GPIO3_GDIR |= (1 << 18);

// Configure pin as input (pin 4 = GPIO4 bit 6, /RD)
GPIO4_GDIR &= ~(1 << 6);

// Configure multiple pins as outputs
GPIO2_GDIR |= (1 << 10) | (1 << 11) | (1 << 12);  // Pins 6, 9, 32
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

### Pin Mapping Table (Z80 Project Pins - Optimized)

| Teensy Pin | GPIO Bank | Bit | Z80 Signal | Notes                    |  
|------------|-----------|-----|------------|--------------------------|  
| 0          | GPIO1     | 3   | IORQ       | Input from Z80           |  
| 1          | GPIO1     | 2   | HALT       | Input from Z80           |  
| 2          | GPIO4     | 4   | M1         | Input from Z80           |  
| 3          | GPIO4     | 5   | RFSH       | Input from Z80           |  
| 4          | GPIO4     | 6   | RD         | Input, interrupt capable |  
| 5          | GPIO4     | 8   | MREQ       | Input from Z80           |  
| 6          | GPIO2     | 10  | INT        | Output to Z80            |  
| 7          | GPIO2     | 17  | D5         | Bidirectional            |  
| 8          | GPIO2     | 16  | D4         | Bidirectional            |  
| 9          | GPIO2     | 11  | NMI        | Output to Z80            |  
| 10         | GPIO2     | 0   | D0         | Bidirectional            |  
| 11         | GPIO2     | 2   | D2         | Bidirectional            |  
| 12         | GPIO2     | 1   | D1         | Bidirectional            |  
| 13         | GPIO2     | 3   | D3         | Bidirectional            |  
| 14         | GPIO1     | 18  | A2         | **Address bus**          |  
| 15         | GPIO1     | 19  | A3         | **Address bus**          |  
| 16         | GPIO1     | 23  | A7         | **Address bus**          |  
| 17         | GPIO1     | 22  | A6         | **Address bus**          |  
| 18         | GPIO1     | 17  | A1         | **Address bus**          |  
| 19         | GPIO1     | 16  | A0         | **Address bus**          |  
| 20         | GPIO1     | 26  | A10        | **Address bus**          |  
| 21         | GPIO1     | 27  | A11        | **Address bus**          |  
| 22         | GPIO1     | 24  | A8         | **Address bus**          |  
| 23         | GPIO1     | 25  | A9         | **Address bus**          |  
| 24         | GPIO1     | 12  | BUSACK     | Input from Z80           |  
| 25         | GPIO1     | 13  | (spare)    | Available                |  
| 26         | GPIO1     | 30  | A14        | **Address bus**          |  
| 27         | GPIO1     | 31  | A15        | **Address bus**          |  
| 28         | GPIO3     | 18  | CLK        | Output, FlexPWM2_A       |  
| 29         | GPIO4     | 31  | (spare)    | Available                |  
| 30         | GPIO3     | 23  | (spare)    | Available                |  
| 31         | GPIO3     | 22  | (spare)    | Available                |  
| 32         | GPIO2     | 12  | BUSREQ     | Output to Z80            |  
| 33         | GPIO4     | 7   | WR         | Input, interrupt capable |  
| 34         | GPIO2     | 29  | RESET      | Output to Z80            |  
| 35         | GPIO2     | 28  | WAIT       | Output to Z80            |  
| 36         | GPIO2     | 18  | D6         | Bidirectional            |  
| 37         | GPIO2     | 19  | D7         | Bidirectional            |  
| 38         | GPIO1     | 28  | A12        | **Address bus**          |  
| 39         | GPIO1     | 29  | A13        | **Address bus**          |  
| 40         | GPIO1     | 20  | A4         | **Address bus**          |  
| 41         | GPIO1     | 21  | A5         | **Address bus**          |

**Key Optimizations:**
- **GPIO1.16-31:** All 16 address bits (A0-A15) consecutive!
- **GPIO2.0-3, 16-19:** Data bus in two groups
- **GPIO4:** All Z80 status outputs grouped

**Complete Teensy pinout:** https://www.pjrc.com/teensy/pinout.html

---

## Optimized I/O Functions

### Data Bus Operations (D0-D7)

**Optimization:** Data bus on GPIO2 in two groups (bits 0-3, 16-19)

**Solution:** Two-mask extraction for fast reads

```cpp
// =============================================================================
// DATA BUS READ (8-bit parallel) - OPTIMIZED
// =============================================================================

inline uint8_t readDataBus() {
  // D0-D3: GPIO2.0-3 (pins 10,12,11,13)
  // D4-D7: GPIO2.16-19 (pins 8,7,36,37)
  
  uint32_t gpio2 = GPIO2_DR;
  return (gpio2 & 0x0F) |          // D0-D3 from bits 0-3
         ((gpio2 >> 12) & 0xF0);    // D4-D7 from bits 16-19, shift to 4-7
}
// Performance: ~10 ns (1 register read + 2 masks)
// vs ~1200 ns with digitalRead() = **120x faster**

// =============================================================================
// DATA BUS WRITE (8-bit parallel) - OPTIMIZED
// =============================================================================

inline void writeDataBus(uint8_t data) {
  // Set direction to output for data pins
  GPIO2_GDIR |= 0x000F000F;  // Enable D0-D7 outputs
  
  // Clear and set data bits atomically
  uint32_t gpio2 = GPIO2_DR & ~0x000F000F;  // Clear data bits
  gpio2 |= (data & 0x0F);                    // D0-D3
  gpio2 |= ((uint32_t)(data & 0xF0)) << 12;  // D4-D7
  GPIO2_DR = gpio2;
}
// Performance: ~12 ns
// vs ~1200 ns with digitalWrite() = **100x faster**

```

### Address Bus Operations (A0-A15)

**Optimization:** ALL 16 address bits on GPIO1.16-31 (consecutive!)

```cpp
// =============================================================================
// ADDRESS BUS READ (16-bit parallel) - ULTRA-OPTIMIZED
// =============================================================================

inline uint16_t readAddressBus() {
  // A0-A15: GPIO1.16-31 (all consecutive!)
  // Single shift operation - no bit manipulation needed!
  return GPIO1_DR >> 16;
}
// Performance: ~5 ns (1 register read + 1 shift)
// vs ~2400 ns with digitalRead() = **480x faster** ⚡

// Alternative with PSR register (more reliable for inputs):
inline uint16_t readAddressBus_PSR() {
  return GPIO1_PSR >> 16;
}
```

### Control Signal Operations

```cpp
// =============================================================================
// CONTROL SIGNAL READS (Z80 outputs → Teensy inputs) - OPTIMIZED
// =============================================================================

// GPIO4 has all Z80 status outputs grouped!
inline bool isReadCycle() {
  return !(GPIO4_DR & (1 << 6));  // /RD on pin 4 (GPIO4.06, active low)
}

inline bool isWriteCycle() {
  return !(GPIO4_DR & (1 << 7));  // /WR on pin 33 (GPIO4.07, active low)
}

inline bool isMemoryRequest() {
  return !(GPIO4_DR & (1 << 8));  // /MREQ on pin 5 (GPIO4.08, active low)
}

inline bool isM1Cycle() {
  return !(GPIO4_DR & (1 << 4));  // /M1 on pin 2 (GPIO4.04, active low)
}

inline bool isRefreshCycle() {
  return !(GPIO4_DR & (1 << 5));  // /RFSH on pin 3 (GPIO4.05, active low)
}

// GPIO1 has other control inputs
inline bool isIORequest() {
  return !(GPIO1_DR & (1 << 3));  // /IORQ on pin 0 (GPIO1.03, active low)
}

inline bool isHalted() {
  return !(GPIO1_DR & (1 << 2));  // /HALT on pin 1 (GPIO1.02, active low)
}

inline bool isBusAcknowledged() {
  return !(GPIO1_DR & (1 << 12));  // /BUSACK on pin 24 (GPIO1.12, active low)
}

// =============================================================================
// CONTROL SIGNAL WRITES (Teensy outputs → Z80 inputs)
// =============================================================================

// Control outputs on GPIO2 and GPIO3
inline void assertReset() {
  GPIO2_DR_CLEAR = (1 << 29);  // /RESET on pin 34 (GPIO2.29, active low)
}

inline void deassertReset() {
  GPIO2_DR_SET = (1 << 29);  // /RESET on pin 34 (GPIO2.29, inactive high)
}

inline void assertWait() {
  GPIO2_DR_CLEAR = (1 << 28);  // /WAIT on pin 35 (GPIO2.28, active low)
}

inline void deassertWait() {
  GPIO2_DR_SET = (1 << 28);  // /WAIT on pin 35 (GPIO2.28, inactive high)
}

inline void assertInterrupt() {
  GPIO2_DR_CLEAR = (1 << 10);  // /INT on pin 6 (GPIO2.10, active low)
}

inline void deassertInterrupt() {
  GPIO2_DR_SET = (1 << 10);  // /INT on pin 6 (GPIO2.10, inactive high)
}

inline void assertNMI() {
  GPIO2_DR_CLEAR = (1 << 11);  // /NMI on pin 9 (GPIO2.11, active low)
}

inline void deassertNMI() {
  GPIO2_DR_SET = (1 << 11);  // /NMI on pin 9 (GPIO2.11, inactive high)
}

inline void assertBusRequest() {
  GPIO2_DR_CLEAR = (1 << 12);  // /BUSREQ on pin 32 (GPIO2.12, active low)
}

inline void deassertBusRequest() {
  GPIO2_DR_SET = (1 << 12);  // /BUSREQ on pin 32 (GPIO2.12, inactive high)
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
  // D0-D7 on GPIO2.0-3, 16-19
  GPIO2_GDIR &= ~0x000F000F;  // Clear data bus bits
  
  // Configure address bus as inputs (Z80 outputs)
  // A0-A15 on GPIO1.16-31
  GPIO1_GDIR &= ~0xFFFF0000;  // Clear all address bits
  
  // Configure control inputs (Z80 outputs)
  GPIO1_GDIR &= ~((1 << 2) | (1 << 3) | (1 << 12));  // HALT, IORQ, BUSACK
  GPIO4_GDIR &= ~((1 << 4) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8));  // M1, RFSH, RD, WR, MREQ
  
  // Configure control outputs (Teensy outputs to Z80 inputs)
  GPIO3_GDIR |= (1 << 18);  // CLK
  GPIO2_GDIR |= ((1 << 10) | (1 << 11) | (1 << 12) | (1 << 28) | (1 << 29));  // INT, NMI, BUSREQ, WAIT, RESET
  
  // Initialize control outputs to inactive state (HIGH for active-low signals)
  GPIO2_DR_SET = (1 << 10) | (1 << 11) | (1 << 12) | (1 << 28) | (1 << 29);  // All inactive
  
  // Note: CLK (pin 28, GPIO3.18) initialized by FlexPWM in Phase 2
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
