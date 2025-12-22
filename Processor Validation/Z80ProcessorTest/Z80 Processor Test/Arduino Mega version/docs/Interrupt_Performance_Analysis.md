# Interrupt Performance Analysis
## External Interrupts (INT) vs Pin Change Interrupts (PCINT)

**Date:** November 6, 2025  
**Purpose:** Compare performance of interrupt methods for MREQ signal handling

---

## 1. ATmega2560 Interrupt Types

### External Interrupts (INT0-INT7)
- **Count:** 8 dedicated external interrupt pins
- **Trigger Modes:** 
  - Low level
  - Any edge (rising or falling)
  - Falling edge only ⭐
  - Rising edge only
- **Pins Available:**
  - INT4 = Pin D2 (PE4)
  - INT5 = Pin D3 (PE5)
  - INT6 = Pin D18 (PD3) - Serial1 RX (avoid)
  - INT7 = Pin D19 (PD2) - Serial1 TX (avoid)

### Pin Change Interrupts (PCINT0-PCINT23)
- **Count:** 24 pin change interrupt pins (3 groups of 8)
- **Trigger Mode:** 
  - Any edge only (cannot select rising/falling)
  - Triggers on ANY pin change in the group
- **Groups:**
  - PCINT0 (PCINT0-7): PORTB (pins 53, 52, 51, 50, 10, 11, 12, 13)
  - PCINT1 (PCINT8-15): PORTJ/PORTK (limited, pins 14, 15)
  - PCINT2 (PCINT16-23): PORTK (pins A8-A15 / 62-69)

---

## 2. Performance Comparison

### 2.1 Interrupt Latency

| Metric | External INT | Pin Change INT | Winner |
|--------|--------------|----------------|--------|
| **Hardware trigger detection** | 1 cycle | 1 cycle | TIE |
| **Edge filtering (falling only)** | Hardware | Software | 🏆 **INT** |
| **ISR entry overhead** | 4-6 cycles | 4-6 cycles | TIE |
| **Edge detection in software** | Not needed | 2-3 cycles | 🏆 **INT** |
| **Fast-exit for wrong edge** | Not needed | 5 cycles | 🏆 **INT** |
| **Total minimum latency** | **4-6 cycles** | **9-11 cycles** | 🏆 **INT wins by 5 cycles** |

### 2.2 Detailed Cycle Count Analysis

#### External Interrupt (INT4/INT5) - Falling Edge Triggered

```cpp
// Configuration: Trigger on falling edge only
EICRA |= (1 << ISC51) | (0 << ISC50);  // Falling edge on INT5

ISR(INT5_vect) {
  // Hardware guarantees this is a falling edge (MREQ going active)
  // No need to check - we KNOW MREQ is active
  
  // [4-6 cycles] Hardware ISR entry
  
  // Read address immediately (2 cycles)
  register uint16_t address = ((uint16_t)PINK << 8) | PINC;
  
  // Check RD/WR (2 cycles)
  register uint8_t control = ~PINL;
  
  // Branch to READ/WRITE handler (2 cycles)
  if (control & PORTL_RD_MASK) {
    // READ operation (6-8 cycles)
    DDRA = 0xFF;
    PORTA = pgm_read_byte(&g_testProgram[address]);
    return;  // [4 cycles] Hardware ISR exit
  }
  // ... WRITE handling
}

// TOTAL READ LATENCY: 4-6 + 2 + 2 + 2 + 8 + 4 = 22-24 cycles (~1.4 µs)
```

#### Pin Change Interrupt (PCINT) - Any Edge

```cpp
// Configuration: Trigger on ANY edge (rising or falling)
PCICR |= (1 << PCIE0);  // Enable group
PCMSK0 = (1 << PCINT0); // Enable specific pin

ISR(PCINT0_vect) {
  // This ISR fires on BOTH rising and falling edges!
  
  // [4-6 cycles] Hardware ISR entry
  
  // Read control signals and invert (2 cycles)
  register uint8_t control = ~PINL;
  
  // Check if this is a falling edge (MREQ going active) (2-3 cycles)
  if (!(control & PORTL_MREQ_MASK)) {
    // Rising edge or noise - fast exit
    return;  // [4 cycles] Hardware ISR exit
  }
  // Note: On rising edge, we've wasted 10-13 cycles total
  
  // MREQ is active - proceed with memory access
  
  // Read address (2 cycles)
  register uint16_t address = ((uint16_t)PINK << 8) | PINC;
  
  // Check RD/WR (already have control) (2 cycles)
  if (control & PORTL_RD_MASK) {
    // READ operation (6-8 cycles)
    DDRA = 0xFF;
    PORTA = pgm_read_byte(&g_testProgram[address]);
    return;  // [4 cycles] Hardware ISR exit
  }
  // ... WRITE handling
}

// TOTAL READ LATENCY (falling edge): 4-6 + 2 + 3 + 2 + 2 + 8 + 4 = 25-27 cycles (~1.6 µs)
// WASTED TIME (rising edge): 4-6 + 2 + 3 + 4 = 13-15 cycles (~0.9 µs)
```

### 2.3 Performance Summary

| Operation | External INT | Pin Change INT | Difference |
|-----------|--------------|----------------|------------|
| **READ latency (falling edge)** | 22-24 cycles (1.38 µs) | 25-27 cycles (1.56 µs) | 🏆 **INT faster by 3 cycles (0.19 µs)** |
| **WRITE latency (falling edge)** | 18-20 cycles (1.13 µs) | 21-23 cycles (1.31 µs) | 🏆 **INT faster by 3 cycles (0.19 µs)** |
| **Wasted time (rising edge)** | 0 cycles (doesn't fire) | 13-15 cycles (0.9 µs) | 🏆 **INT: No wasted cycles** |
| **CPU overhead per Z80 cycle** | 1 ISR call (falling) | 2 ISR calls (both edges) | 🏆 **INT: 50% less overhead** |

---

## 3. Detailed Advantages

### External Interrupt (INT) Advantages

✅ **Faster by ~3 cycles (0.19 µs)** per memory access
- No need to determine which edge caused the interrupt
- Hardware filters edges before calling ISR
- Jump directly to memory handling code

✅ **50% fewer ISR invocations**
- Only fires on falling edge (MREQ going active)
- Rising edge (MREQ going inactive) is ignored by hardware
- Reduces CPU overhead and power consumption

✅ **Cleaner code**
- No edge detection logic needed
- Simpler ISR structure
- Easier to understand and maintain

✅ **Lower interrupt latency jitter**
- Consistent execution path (no fast-exit branch)
- More predictable timing for logic analyzer analysis

✅ **Better for future expansion**
- Each INT pin is independent
- Can add more external interrupts without conflicts
- No shared interrupt vectors

### Pin Change Interrupt (PCINT) Advantages

✅ **More pins available**
- 24 PCINT pins vs 8 INT pins
- Better for designs using many interrupt sources

✅ **Can monitor multiple pins with one ISR**
- Useful if we wanted to interrupt on IORQ, RD, WR, etc.
- (But we don't need this for our application)

---

## 4. Real-World Impact

### At 1 MHz Z80 Clock (1 µs per cycle)

**Memory Read Cycle (4 T-states = 4 µs):**

| Method | ISR Latency | % of T-state | Z80 Wait? |
|--------|-------------|--------------|-----------|
| External INT | 1.38 µs | 34.5% | ❌ No wait needed |
| Pin Change INT | 1.56 µs | 39.0% | ❌ No wait needed |

**Conclusion:** Both methods work fine at 1 MHz. INT is slightly faster but difference is small.

### At 2 MHz Z80 Clock (500 ns per cycle)

**Memory Read Cycle (4 T-states = 2 µs):**

| Method | ISR Latency | % of T-state | Z80 Wait? |
|--------|-------------|--------------|-----------|
| External INT | 1.38 µs | 69.0% | ⚠️ Tight timing |
| Pin Change INT | 1.56 µs | 78.0% | ⚠️ Very tight timing |

**Conclusion:** INT gives us 180 ns more margin. Could be critical at 2 MHz.

### At 3 MHz Z80 Clock (333 ns per cycle)

**Memory Read Cycle (4 T-states = 1.33 µs):**

| Method | ISR Latency | % of T-state | Z80 Wait? |
|--------|-------------|--------------|-----------|
| External INT | 1.38 µs | 103.5% | ✅ **WAIT required** |
| Pin Change INT | 1.56 µs | 117.0% | ✅ **WAIT required** |

**Conclusion:** Both require WAIT states at 3 MHz. INT is still 180 ns faster.

---

## 5. Additional Considerations

### Code Complexity

**External INT:**
```cpp
// Simple, direct ISR
ISR(INT5_vect) {
  // Handle memory request
  // MREQ is guaranteed active (falling edge)
}
```

**Pin Change INT:**
```cpp
// More complex ISR
ISR(PCINT0_vect) {
  // Check if MREQ caused interrupt
  // Check if falling edge (not rising)
  // Then handle memory request
}
```

**Winner:** 🏆 **External INT** - Simpler code

### Power Consumption

**External INT:**
- 1 ISR call per Z80 memory cycle
- Less time in interrupt handler
- More time in low-power idle state

**Pin Change INT:**
- 2 ISR calls per Z80 memory cycle (both edges)
- More time in interrupt handler
- More power consumption

**Winner:** 🏆 **External INT** - Lower power

### Debugging

**External INT:**
- Logic analyzer shows 1 interrupt per memory access
- Clean traces, easy to analyze
- ISR timing is consistent

**Pin Change INT:**
- Logic analyzer shows 2 interrupts per memory access
- Need to filter out "fast exit" events
- More complex timing analysis

**Winner:** 🏆 **External INT** - Easier debugging

---

## 6. Recommendation

### For MREQ Signal Handling:

**Use External Interrupt (INT4 or INT5)** ⭐⭐⭐

**Reasons:**
1. **Faster:** 3 cycles (180 ns) faster per memory access
2. **Cleaner:** No edge detection logic needed
3. **Lower overhead:** 50% fewer ISR invocations
4. **Better margin:** More timing headroom for 2+ MHz operation
5. **Easier debug:** Cleaner logic analyzer traces
6. **Lower power:** Less CPU time in ISR

**The performance difference is small (~12%) but meaningful:**
- At 1 MHz: Not critical, but still better
- At 2 MHz: Could be the difference between working and not working
- At 3+ MHz: Every cycle counts, INT gives best chance of success

### Suggested Pin Assignment:

| Signal | Pin | Interrupt Type | Priority |
|--------|-----|----------------|----------|
| **MREQ** | **D2 (PE4)** | **INT4** | Highest |
| ~~MREQ~~ | ~~D49 (PL0)~~ | ~~Not supported~~ | ❌ |

Move MREQ from D49 to D2, use external interrupt INT4.

---

## 7. Configuration Code Comparison

### External Interrupt (INT4) Configuration

```cpp
void initializeMemoryHandler() {
  // Configure INT4 for falling edge trigger
  EICRB &= ~((1 << ISC41) | (1 << ISC40));  // Clear bits
  EICRB |= (1 << ISC41);                     // Set for falling edge
  
  // Enable INT4
  EIMSK |= (1 << INT4);
  
  // Enable global interrupts
  sei();
}

// ISR for INT4
ISR(INT4_vect) {
  // MREQ is active (falling edge guaranteed by hardware)
  register uint16_t address = ((uint16_t)PINK << 8) | PINC;
  register uint8_t control = ~PINL;
  
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

### Pin Change Interrupt Configuration

```cpp
void initializeMemoryHandler() {
  // Configure PCINT0 (if MREQ was on PORTB)
  PCICR |= (1 << PCIE0);      // Enable PCINT0 group
  PCMSK0 = (1 << PCINT0);     // Enable specific pin
  
  // Enable global interrupts
  sei();
}

// ISR for PCINT0
ISR(PCINT0_vect) {
  // Check if MREQ is active (might be rising edge)
  register uint8_t control = ~PINL;
  if (!(control & PORTL_MREQ_MASK)) {
    return;  // Fast exit - rising edge
  }
  
  // MREQ is active (falling edge)
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

**Line count:**
- External INT: ~15 lines
- Pin Change INT: ~20 lines

**Complexity:**
- External INT: Simpler, no edge detection
- Pin Change INT: More complex, needs edge detection

---

## 8. Conclusion

**External Interrupts (INT) are definitively better for MREQ signal handling.**

While the performance difference is modest (~180 ns or 3 cycles), external interrupts offer:
- Slightly better performance
- Significantly cleaner code
- Lower CPU overhead
- Better debugging experience
- Lower power consumption

**Recommendation: Use INT4 (Pin D2) for MREQ signal** ⭐

The only scenario where Pin Change Interrupts would be preferable is if we ran out of external interrupt pins and needed to monitor many signals. Since we only need one interrupt for MREQ, external interrupts are the clear winner.

---

## Appendix: ATmega2560 Interrupt Pin Mapping

### Available External Interrupts (sorted by convenience)

| INT# | Pin | Port | Current Use | Available? | Recommendation |
|------|-----|------|-------------|------------|----------------|
| INT4 | D2  | PE4  | None | ✅ Yes | ⭐ **Best choice** |
| INT5 | D3  | PE5  | None | ✅ Yes | ⭐ **Alternate** |
| INT0 | D21 | PD0  | I2C SCL | ❌ No | Avoid |
| INT1 | D20 | PD1  | I2C SDA | ❌ No | Avoid |
| INT2 | D19 | PD2  | Serial1 TX | ⚠️ Maybe | Only if not using Serial1 |
| INT3 | D18 | PD3  | Serial1 RX | ⚠️ Maybe | Only if not using Serial1 |
| INT6 | D18 | PD3  | Serial1 RX | ❌ No | Same as INT3 |
| INT7 | D19 | PD2  | Serial1 TX | ❌ No | Same as INT2 |

**Decision: Use INT4 on Pin D2 (PE4) for MREQ signal.**

---

**End of Analysis**
