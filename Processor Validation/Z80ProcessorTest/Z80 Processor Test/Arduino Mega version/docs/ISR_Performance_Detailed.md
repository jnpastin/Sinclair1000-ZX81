# Detailed ISR Performance Analysis
## PORTB (Pin Change) vs INT4 (External Interrupt) - Cycle-by-Cycle Comparison

**Date:** November 6, 2025  
**Critical Question:** Does external INT eliminate the need to check MREQ state entirely?

---

## Executive Summary

**Answer: YES - External INT eliminates MREQ checking entirely!**

When using external interrupt triggered on falling edge only:
- ✅ ISR only fires when MREQ goes LOW (active)
- ✅ MREQ state is guaranteed by hardware
- ✅ No software check needed
- ✅ **We never need to query MREQ state in ISR**

This changes the performance calculation significantly.

---

## Option 1: PORTB with Pin Change Interrupt

### Configuration:
- All 8 control signals on PORTB (D53, 52, 51, 50, 10, 12, 13, + one more)
- Pin change interrupt on MREQ (PB0)
- Triggers on BOTH rising and falling edges

### ISR Code:
```cpp
ISR(PCINT0_vect) {
  // [4-6 cycles] Hardware ISR entry
  
  // Read all control signals from PORTB (1 cycle)
  register uint8_t control = ~PINB;
  
  // ⚠️ MUST check if MREQ is active (falling edge vs rising edge)
  // (2-3 cycles for check + conditional branch)
  if (!(control & PORTB_MREQ_MASK)) {
    // Rising edge - MREQ going inactive
    return;  // [4 cycles] Fast exit
  }
  // Total wasted on rising edge: 4-6 + 1 + 3 + 4 = 12-14 cycles
  
  // MREQ is active (falling edge confirmed)
  
  // Read address bus (2 cycles)
  register uint16_t address = ((uint16_t)PINK << 8) | PINC;
  
  // Check operation type - RD or WR (already have control)
  // (2 cycles for test + branch)
  if (control & PORTB_RD_MASK) {
    // READ operation
    DDRA = 0xFF;  // (1 cycle)
    
    if (address < g_testProgramLength) {
      PORTA = pgm_read_byte(&g_testProgram[address]);  // (4-6 cycles)
    } else {
      PORTA = 0x00;  // (1 cycle)
    }
    
    return;  // [4 cycles] ISR exit
  }
  
  else if (control & PORTB_WR_MASK) {
    // WRITE operation
    DDRA = 0x00;  // (1 cycle)
    g_capturedData = PINA;  // (1 cycle)
    g_lastWriteAddress = address;  // (2 cycles)
    return;  // [4 cycles] ISR exit
  }
  
  // Unexpected state (neither RD nor WR active)
  DDRA = 0x00;  // (1 cycle)
  return;  // [4 cycles] ISR exit
}
```

### Cycle Count (PORTB + Pin Change):

**READ Operation (falling edge):**
```
Hardware ISR entry:           4-6 cycles
Read PINB:                    1 cycle
Invert:                       1 cycle
Check MREQ (with branch):     3 cycles     ⚠️ REQUIRED
Read address:                 2 cycles
Check RD (with branch):       2 cycles
Set data direction:           1 cycle
PROGMEM read:                 4-6 cycles
Hardware ISR exit:            4 cycles
─────────────────────────────────────────
TOTAL:                        22-28 cycles (~1.5 µs @ 16 MHz)
```

**Rising Edge (wasted):**
```
Hardware ISR entry:           4-6 cycles
Read PINB:                    1 cycle
Invert:                       1 cycle
Check MREQ (fast exit):       3 cycles
Hardware ISR exit:            4 cycles
─────────────────────────────────────────
TOTAL:                        13-15 cycles (~0.9 µs wasted)
```

---

## Option 2: INT4 External Interrupt (MREQ on D2) + PORTL

### Configuration:
- MREQ on D2 (PE4, INT4) - triggers on falling edge ONLY
- Other 7 signals on PORTL (D49-D43): IORQ, RD, WR, M1, RFSH, HALT, BUSAK
- External interrupt configured for falling edge trigger only

### ISR Code:
```cpp
ISR(INT4_vect) {
  // [4-6 cycles] Hardware ISR entry
  
  // ✅ MREQ is GUARANTEED active - no check needed!
  // Hardware only triggers this ISR on falling edge
  
  // Read remaining control signals from PORTL (1 cycle)
  register uint8_t control = ~PINL;
  
  // ⚠️ NO MREQ CHECK NEEDED - hardware guarantees it!
  
  // Read address bus (2 cycles)
  register uint16_t address = ((uint16_t)PINK << 8) | PINC;
  
  // Check operation type - RD or WR
  // (2 cycles for test + branch)
  if (control & PORTL_RD_MASK) {
    // READ operation
    DDRA = 0xFF;  // (1 cycle)
    
    if (address < g_testProgramLength) {
      PORTA = pgm_read_byte(&g_testProgram[address]);  // (4-6 cycles)
    } else {
      PORTA = 0x00;  // (1 cycle)
    }
    
    return;  // [4 cycles] ISR exit
  }
  
  else if (control & PORTL_WR_MASK) {
    // WRITE operation
    DDRA = 0x00;  // (1 cycle)
    g_capturedData = PINA;  // (1 cycle)
    g_lastWriteAddress = address;  // (2 cycles)
    return;  // [4 cycles] ISR exit
  }
  
  // Unexpected state (neither RD nor WR active)
  DDRA = 0x00;  // (1 cycle)
  return;  // [4 cycles] ISR exit
}
```

### Cycle Count (INT4 + PORTL):

**READ Operation (falling edge only - no rising edge triggers!):**
```
Hardware ISR entry:           4-6 cycles
Read PINL:                    1 cycle
Invert:                       1 cycle
⚠️ NO MREQ CHECK:             0 cycles     ✅ ELIMINATED!
Read address:                 2 cycles
Check RD (with branch):       2 cycles
Set data direction:           1 cycle
PROGMEM read:                 4-6 cycles
Hardware ISR exit:            4 cycles
─────────────────────────────────────────
TOTAL:                        19-25 cycles (~1.3 µs @ 16 MHz)
```

**Rising Edge:**
```
ISR doesn't fire at all!      0 cycles     ✅ NO OVERHEAD!
```

---

## Direct Comparison

### Performance Table:

| Metric | PORTB (PCINT) | INT4 + PORTL | Difference |
|--------|---------------|--------------|------------|
| **READ latency** | 22-28 cycles | 19-25 cycles | 🏆 **INT4 faster by 3 cycles** |
| **READ latency (µs)** | 1.5 µs | 1.3 µs | 🏆 **INT4 faster by 0.2 µs** |
| **WRITE latency** | 18-24 cycles | 15-21 cycles | 🏆 **INT4 faster by 3 cycles** |
| **Rising edge overhead** | 13-15 cycles | 0 cycles | 🏆 **INT4: No overhead!** |
| **ISR calls per Z80 cycle** | 2 (both edges) | 1 (falling only) | 🏆 **INT4: 50% fewer!** |
| **MREQ check required?** | ✅ Yes (3 cycles) | ❌ No (0 cycles) | 🏆 **INT4: 3 cycles saved** |
| **Single port read?** | ✅ Yes (PORTB) | ⚠️ No (PORTL + PE4) | PORTB advantage |

### Total CPU Overhead Per Memory Access:

**PORTB (Pin Change):**
- Falling edge ISR: 22-28 cycles
- Rising edge ISR: 13-15 cycles
- **Total: 35-43 cycles per memory access**

**INT4 (External):**
- Falling edge ISR: 19-25 cycles
- Rising edge ISR: 0 cycles (doesn't fire)
- **Total: 19-25 cycles per memory access**

🏆 **INT4 is 16-18 cycles (1.0-1.1 µs) faster per memory access!**

---

## Critical Insight: Do We Ever Need to Check MREQ?

### In the ISR: **NO - NEVER!**

**With External Interrupt (INT4):**
```cpp
ISR(INT4_vect) {
  // ✅ MREQ is active - guaranteed by hardware
  // ✅ No need to check MREQ state
  // Just check RD/WR and handle accordingly
}
```

**With Pin Change Interrupt (PORTB):**
```cpp
ISR(PCINT0_vect) {
  // ⚠️ Could be rising OR falling edge
  // ⚠️ MUST check if MREQ is active
  if (!(control & MREQ_MASK)) return;  // 3 cycles penalty
}
```

### Outside the ISR: **RARELY!**

In diagnostic/debug code, we might want to check MREQ state:

```cpp
// Diagnostic function - check all control signals
void dumpBusState() {
  // Option 1: INT4 + PORTL
  bool mreq = (PINE & (1<<4)) == 0;  // 3 cycles - separate read
  uint8_t others = ~PINL;             // 1 cycle
  
  // Option 2: PORTB
  uint8_t all = ~PINB;                // 1 cycle - includes MREQ
  bool mreq = all & MREQ_MASK;        // 1 cycle
}
```

**Impact:** PORTB saves ~2 cycles in diagnostic code (rarely executed)

**Conclusion:** The diagnostic case is negligible compared to ISR performance.

---

## Maximum Z80 Clock Speed Analysis

### Z80 Memory Read Cycle Timing:

| Z80 Clock | T-state | Memory Cycle | Arduino ISR | Margin |
|-----------|---------|--------------|-------------|---------|
| **1 MHz** | 1.0 µs | 4 µs | PORTB: 1.5 µs | ✅ 2.5 µs (63%) |
| **1 MHz** | 1.0 µs | 4 µs | INT4: 1.3 µs | ✅ 2.7 µs (68%) |
| **2 MHz** | 0.5 µs | 2 µs | PORTB: 1.5 µs | ⚠️ 0.5 µs (25%) |
| **2 MHz** | 0.5 µs | 2 µs | INT4: 1.3 µs | ✅ 0.7 µs (35%) |
| **3 MHz** | 0.33 µs | 1.33 µs | PORTB: 1.5 µs | ❌ -0.17 µs (needs WAIT) |
| **3 MHz** | 0.33 µs | 1.33 µs | INT4: 1.3 µs | ⚠️ 0.03 µs (2%) |

**Analysis:**
- At 1 MHz: Both work fine, INT4 has better margin
- At 2 MHz: INT4 has 40% more margin than PORTB (critical!)
- At 3 MHz: PORTB fails, INT4 barely works

---

## Code Complexity Comparison

### PORTB Approach:
```cpp
// Pros: Clean single-port read
uint8_t control = ~PINB;  // All 8 signals

// Cons: Must check MREQ in ISR
if (!(control & MREQ_MASK)) return;  // Edge detection overhead
```

### INT4 + PORTL Approach:
```cpp
// Pros: No MREQ check needed
// ISR code is simpler - just handle the request

// Cons: MREQ not in PORTL read
uint8_t control = ~PINL;  // 7 signals (no MREQ)
// But MREQ is guaranteed active by interrupt!
```

**Complexity verdict:** INT4 is actually SIMPLER - no edge detection logic!

---

## Real-World Impact: Per-Instruction Overhead

Typical Z80 program executing at 1 MHz:

**Example: 1000 instructions, each with 1 memory read:**

| Approach | ISR Calls | Total Cycles | Time | Power |
|----------|-----------|--------------|------|-------|
| PORTB | 2000 (both edges) | 35,000-43,000 | 2.2-2.7 ms | Higher |
| INT4 | 1000 (falling only) | 19,000-25,000 | 1.2-1.6 ms | Lower |

🏆 **INT4 uses 44% less CPU time and 50% fewer interrupts!**

---

## Recommendation Matrix

| Criterion | PORTB (PCINT) | INT4 + PORTL | Winner |
|-----------|---------------|--------------|---------|
| **Performance (speed)** | 1.5 µs | 1.3 µs | 🏆 INT4 |
| **Performance (overhead)** | 35-43 cycles | 19-25 cycles | 🏆 INT4 |
| **Max Z80 speed** | ~2 MHz | ~2.5 MHz | 🏆 INT4 |
| **ISR complexity** | More complex | Simpler | 🏆 INT4 |
| **Rising edge overhead** | 13-15 cycles | 0 cycles | 🏆 INT4 |
| **Power efficiency** | Lower | Higher | 🏆 INT4 |
| **Single port read** | ✅ Yes | ❌ No | PORTB |
| **Hardware changes** | Major (8 wires) | Minor (1 wire) | 🏆 INT4 |
| **Diagnostic code** | Simpler | Slightly complex | PORTB |

### Scores:
- **INT4 + PORTL: 8 points** 🏆🏆🏆
- **PORTB (PCINT): 2 points**

---

## Final Recommendation: **INT4 + PORTL** 🏆

### Reasons:

1. **Faster by 3 cycles** (200 ns) per memory access - 13% faster
2. **50% fewer ISR invocations** - no rising edge interrupts
3. **Simpler ISR code** - no MREQ checking needed
4. **Better Z80 clock support** - can reach 2.5 MHz vs 2 MHz
5. **Lower power consumption** - less time in ISR
6. **Less CPU overhead** - 44% reduction in interrupt processing
7. **Minimal hardware change** - only move 1 wire (MREQ) vs 8 wires

### The ONLY disadvantage:
- ⚠️ Diagnostic code must read two ports to get MREQ + other signals
- But this is negligible - diagnostics are rarely executed

### Key Insight You Discovered:

**External interrupt eliminates MREQ checking entirely!**

The hardware does the edge detection for us. We never need to query MREQ state in the ISR because the interrupt only fires when MREQ is active. This is a 3-cycle (200 ns) savings that makes external interrupt definitively superior.

---

## Implementation Recommendation

### Pin Assignment:
```
MREQ:  D2  (PE4, INT4) - External interrupt, falling edge only
IORQ:  D49 (PL0)       - PORTL
RD:    D48 (PL1)       - PORTL
WR:    D47 (PL2)       - PORTL
M1:    D46 (PL3)       - PORTL
RFSH:  D45 (PL4)       - PORTL
HALT:  D44 (PL5)       - PORTL
BUSAK: D43 (PL6)       - PORTL
```

### ISR Implementation:
```cpp
void initializeMemoryHandler() {
  // Configure INT4 for falling edge only
  EICRB &= ~((1 << ISC41) | (1 << ISC40));  // Clear
  EICRB |= (1 << ISC41);                     // Falling edge
  EIMSK |= (1 << INT4);                      // Enable INT4
  sei();
}

ISR(INT4_vect) {
  // MREQ is active - guaranteed by hardware
  // No need to check MREQ state!
  
  register uint8_t control = ~PINL;
  register uint16_t address = ((uint16_t)PINK << 8) | PINC;
  
  if (control & PORTL_RD_MASK) {
    DDRA = 0xFF;
    PORTA = pgm_read_byte(&g_testProgram[address]);
  }
  else if (control & PORTL_WR_MASK) {
    DDRA = 0x00;
    g_capturedData = PINA;
    g_lastWriteAddress = address;
  }
}
```

---

## Answering Your Specific Questions

### Q1: "Do we ever need to query the state of MREQ?"

**A1: In the ISR - NO, NEVER!**
- External interrupt only fires on falling edge (MREQ going active)
- MREQ state is guaranteed by hardware
- No software check needed
- This saves 3 cycles (200 ns) per memory access

**A1: Outside ISR - RARELY!**
- Only in diagnostic/debug code (e.g., `dumpBusState()`)
- Requires reading PE4 separately: `(PINE & (1<<4)) == 0`
- Adds 2-3 cycles to diagnostic functions
- Negligible impact (diagnostics rarely executed)

### Q2: "Is querying the remaining control signals concurrently sufficient?"

**A2: YES - ABSOLUTELY!**

In the ISR, we only need to check RD/WR to determine operation type:
```cpp
ISR(INT4_vect) {
  // MREQ guaranteed active - don't check it
  uint8_t control = ~PINL;  // Get RD, WR, M1, etc.
  
  if (control & RD_MASK) {
    // Handle READ
  }
  else if (control & WR_MASK) {
    // Handle WRITE
  }
}
```

We never need to re-check MREQ because:
1. Interrupt only fires when MREQ is active
2. MREQ remains active throughout the bus cycle
3. Z80 timing guarantees MREQ is stable

---

## Conclusion

**Your intuition was exactly right!**

External interrupt on falling edge eliminates the need to check MREQ state in the ISR. This is a fundamental advantage that makes INT4 + PORTL the clear winner:

- ✅ **3 cycles faster** per access
- ✅ **50% fewer interrupts**
- ✅ **Simpler ISR code**
- ✅ **Better Z80 clock support**
- ✅ **Only 1 wire to move** vs 8 wires

The single-port read advantage of PORTB is completely overshadowed by these benefits.

**Recommendation: Use INT4 (D2) for MREQ, keep other signals on PORTL** 🏆

---

**End of Analysis**
