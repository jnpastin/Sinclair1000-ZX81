# MREQ Interrupt Problem - Alternative Solutions
## Addressing the Control Signal Port Unity Issue

**Date:** November 6, 2025  
**Problem:** MREQ needs interrupt capability, but PORTL doesn't support interrupts  
**Constraint:** Design relies on reading all control signals from one port with single instruction

---

## The Core Problem

### Current Design Philosophy:
```cpp
// Read all 8 control signals in ONE instruction
uint8_t signals = ~PINL;

// Test multiple signals from cached value (fast!)
bool mreq = signals & PORTL_MREQ_MASK;
bool rd   = signals & PORTL_RD_MASK;
bool wr   = signals & PORTL_WR_MASK;
bool m1   = signals & PORTL_M1_MASK;
```

**Performance:** 1 cycle to read + 4 bit tests = 5 cycles total

### If MREQ moves to different port:
```cpp
// Must read TWO ports
uint8_t signals = ~PINL;        // 1 cycle - RD, WR, M1, etc.
bool mreq = (PINE & (1<<4)) == 0;  // 3 cycles - separate read for MREQ

// Test signals
bool rd = signals & PORTL_RD_MASK;
bool wr = signals & PORTL_WR_MASK;
```

**Performance:** 4 cycles to read + 4 bit tests = 8 cycles total  
**Penalty:** 3 extra cycles (~187 ns) when checking MREQ with other signals

---

## Solution Options Analysis

### Option 1: Move All Control Signals to PORTB ⭐ BEST OPTION

**PORTB has both complete port AND pin change interrupts!**

#### PORTB Pin Mapping on Arduino Mega:
```
PB0 = D53  ✅ Available
PB1 = D52  ✅ Available  
PB2 = D51  ✅ Available
PB3 = D50  ✅ Available
PB4 = D10  ✅ Available
PB5 = D11  ⚠️ Timer1 OC1A (CLK output) - CAN be used as input simultaneously
PB6 = D12  ✅ Available
PB7 = D13  ✅ Available (LED, but usable)
```

#### Pin Change Interrupts on PORTB:
- **PCINT0-PCINT7** (PCINT0 vector)
- All 8 pins support pin change interrupts
- Can trigger on any pin change

#### Proposed PORTB Assignment:

| Z80 Signal | PORTB Pin | Arduino Pin | PCINT | Notes |
|------------|-----------|-------------|-------|-------|
| **MREQ** | **PB0** | **D53** | PCINT0 | Interrupt trigger |
| IORQ | PB1 | D52 | PCINT1 | |
| RD | PB2 | D51 | PCINT2 | |
| WR | PB3 | D50 | PCINT3 | |
| M1 | PB4 | D10 | PCINT4 | |
| RFSH | PB5 | D11 | PCINT5 | Can coexist with Timer1 |
| HALT | PB6 | D12 | PCINT6 | |
| BUSAK | PB7 | D13 | PCINT7 | LED on pin (minor) |

#### Advantages:
✅ All 8 control signals on ONE complete port  
✅ Pin change interrupt available (PCINT0 vector)  
✅ Single instruction read: `PINB`  
✅ Hardware interrupt (not polling)  
✅ All pins are accessible on Arduino Mega  
✅ No external interrupt conflicts  

#### Disadvantages:
⚠️ PB5 (D11) used by Timer1 for CLK - but can be input simultaneously  
⚠️ PB7 (D13) has LED - minor annoyance (can be ignored)  
⚠️ Pin change interrupt (both edges) - need edge detection in ISR  
⚠️ Major hardware rewiring required  

#### Timer1/CLK Conflict Resolution:
PB5 is configured as OUTPUT by Timer1 for CLK generation. Can we also use it as INPUT for RFSH?

**Answer: YES!**
```cpp
// Configure Timer1 on PB5 for CLK output
DDRB |= (1 << PB5);  // PB5 as output for CLK

// Simultaneously read PB5 as input for RFSH
// When Z80 drives RFSH signal, it will override our CLK output
// This works because:
// 1. Z80 RFSH is only active during refresh cycles
// 2. Z80 doesn't care about CLK during refresh
// 3. We can still read the state with PINB

// Alternative: Use external buffer/resistor to isolate
```

**OR:** Move RFSH to a different pin and free PB5 exclusively for CLK.

#### ISR Implementation:
```cpp
// Configure pin change interrupt on PORTB
PCICR |= (1 << PCIE0);      // Enable PCINT0 vector
PCMSK0 = (1 << PCINT0);     // Only monitor MREQ (PB0)

ISR(PCINT0_vect) {
  // Read all control signals from PORTB (1 cycle)
  register uint8_t control = ~PINB;
  
  // Check if MREQ is active (falling edge)
  if (!(control & (1 << 0))) {
    return;  // Fast exit - rising edge
  }
  
  // MREQ active - read address and handle
  register uint16_t address = ((uint16_t)PINK << 8) | PINC;
  
  if (control & (1 << 2)) {  // RD on PB2
    DDRA = 0xFF;
    PORTA = pgm_read_byte(&g_testProgram[address]);
  }
  else if (control & (1 << 3)) {  // WR on PB3
    DDRA = 0x00;
    g_capturedData = PINA;
    g_lastWriteAddress = address;
  }
}
```

#### Performance:
- READ latency: 25-27 cycles (~1.56 µs) - same as PCINT
- Maintains single-port read for all control signals
- Only 3 cycles slower than external INT

#### Required Changes:
1. ✅ Rewire 8 control signals from PORTL (D49-42) to PORTB (D53-50, D10-13)
2. ✅ Update pins.h definitions
3. ✅ Update hal.cpp interrupt configuration
4. ✅ Update ISR to use PINB instead of PINL
5. ✅ Resolve Timer1/PB5 conflict (move RFSH or use isolation)

---

### Option 2: Keep PORTL, Use Polling

**Accept that PORTL has no interrupts, poll MREQ in tight loop**

#### Implementation:
```cpp
void runZ80() {
  startClock();
  
  while (true) {
    // Poll MREQ continuously
    if (isMemoryRequest()) {
      handleMemoryRequest();
    }
    
    // Check for halt or other exit conditions
    if (isHalted()) break;
  }
  
  stopClock();
}

void handleMemoryRequest() {
  // Not an ISR, just a regular function
  uint8_t control = ~PINL;
  uint16_t address = ((uint16_t)PINK << 8) | PINC;
  
  if (control & PORTL_RD_MASK) {
    DDRA = 0xFF;
    PORTA = pgm_read_byte(&g_testProgram[address]);
  }
  else if (control & PORTL_WR_MASK) {
    DDRA = 0x00;
    // Can't reliably capture writes with polling
  }
}
```

#### Advantages:
✅ No hardware changes  
✅ All signals stay on PORTL  
✅ Simple code  

#### Disadvantages:
❌ Polling loop blocks all other code  
❌ Much slower response time (~50-100 cycles = 3-6 µs)  
❌ Can only support very slow Z80 clocks (<100 KHz)  
❌ May miss fast signals  
❌ Can't reliably capture write operations  
❌ High power consumption  

**Verdict:** ❌ Not acceptable for 1+ MHz operation

---

### Option 3: Timer-Based Polling

**Use hardware timer to check MREQ at high frequency**

#### Implementation:
```cpp
// Configure Timer2 to interrupt every 1 µs
void initializeMemoryHandler() {
  TCCR2A = (1 << WGM21);  // CTC mode
  TCCR2B = (1 << CS21);   // Prescaler 8
  OCR2A = 1;              // ~1 µs period
  TIMSK2 = (1 << OCIE2A); // Enable interrupt
}

ISR(TIMER2_COMPA_vect) {
  // Check MREQ every 1 µs
  if (isMemoryRequest()) {
    handleMemoryRequest();
  }
}
```

#### Advantages:
✅ No hardware changes  
✅ All signals stay on PORTL  
✅ Non-blocking (other code can run)  

#### Disadvantages:
❌ Adds latency (0-1 µs random delay)  
❌ Timer overhead every microsecond  
❌ May miss very short MREQ pulses  
❌ Complex timing analysis  
❌ Still slower than true interrupts  

**Verdict:** ⚠️ Possible but not ideal

---

### Option 4: Hybrid - External INT + Multi-Port Read

**Use external INT4 for MREQ, accept reading from two ports**

#### Implementation:
```cpp
// MREQ on INT4 (Pin D2, PE4)
// Other signals on PORTL (D49-D43: IORQ, RD, WR, M1, RFSH, HALT, BUSAK)

ISR(INT4_vect) {
  // MREQ is active (external interrupt guarantees this)
  // Read other control signals from PORTL
  register uint8_t control = ~PINL;
  register uint16_t address = ((uint16_t)PINK << 8) | PINC;
  
  if (control & PORTL_RD_MASK) {  // Note: RD now on PL1
    DDRA = 0xFF;
    PORTA = pgm_read_byte(&g_testProgram[address]);
  }
  else if (control & PORTL_WR_MASK) {  // WR now on PL2
    DDRA = 0x00;
    g_capturedData = PINA;
    g_lastWriteAddress = address;
  }
}
```

#### Advantages:
✅ Fastest interrupt response (22-24 cycles)  
✅ Minimal hardware changes (only move MREQ)  
✅ External interrupt benefits (falling edge only)  

#### Disadvantages:
⚠️ MREQ not on same port (but don't need to check it in ISR)  
⚠️ `readControlSignals()` function doesn't include MREQ  
⚠️ Outside ISR, need to read two pins to check MREQ + others  

#### Impact on Non-ISR Code:
```cpp
// In main code, checking multiple signals becomes slower:
uint8_t control = ~PINL;        // 1 cycle
bool mreq = (PINE & (1<<4)) == 0;  // 3 cycles

// vs original:
uint8_t control = ~PINL;  // 1 cycle (includes MREQ)
```

**Penalty:** Only affects non-ISR code (diagnostics, polling, etc.)  
**ISR:** No penalty - MREQ is guaranteed active by interrupt

**Verdict:** ✅ Acceptable if non-ISR MREQ checking is rare

---

## Recommendation Matrix

| Criteria | Option 1 (PORTB) | Option 2 (Polling) | Option 3 (Timer) | Option 4 (Hybrid) |
|----------|------------------|-------------------|------------------|-------------------|
| **Performance** | ⭐⭐⭐ | ❌ | ⭐ | ⭐⭐⭐⭐ |
| **Single Port Unity** | ✅ | ✅ | ✅ | ❌ |
| **Hardware Changes** | ⚠️ Major | ✅ None | ✅ None | ⚠️ Minor |
| **Code Complexity** | ⭐⭐⭐ | ⭐⭐⭐⭐ | ⭐⭐ | ⭐⭐⭐ |
| **Max Z80 Speed** | 1-2 MHz | <100 KHz | ~500 KHz | 2+ MHz |
| **Reliability** | ⭐⭐⭐⭐ | ⭐ | ⭐⭐ | ⭐⭐⭐⭐ |

---

## Final Recommendation

### If willing to rewire hardware: **Option 1 - Move to PORTB** ⭐

**Pros:**
- Maintains design philosophy (all signals on one port)
- Hardware interrupt support
- Clean, maintainable code
- Best long-term solution

**Cons:**
- Requires rewiring 8 signals
- Need to resolve Timer1/PB5 conflict
- Pin change interrupt (slightly slower than external INT)

### If hardware must stay as-is: **Option 4 - Hybrid INT4** ⭐⭐

**Pros:**
- Fastest performance
- Minimal rewiring (only MREQ)
- External interrupt benefits

**Cons:**
- MREQ separate from other signals
- Non-ISR code must read two ports for complete signal state
- Breaks design philosophy slightly

---

## My Strong Recommendation: **Option 1 - PORTB**

**Rationale:**
1. Maintains the core design principle (all signals on one port)
2. Hardware is not yet finalized, so rewiring is acceptable
3. Pin change interrupts are only 3 cycles slower than external INT
4. Results in cleanest, most maintainable code
5. No compromises in the API or usage patterns

**The 3-cycle performance difference (180 ns) is negligible compared to maintaining design elegance and code simplicity.**

---

## Implementation Guide for Option 1

### Step 1: Update Pin Definitions
```cpp
// pins.h - Update control signal definitions
#define Z80_MREQ   53  // PB0 (was 49/PL0)
#define Z80_IORQ   52  // PB1 (was 48/PL1)
#define Z80_RD     51  // PB2 (was 47/PL2)
#define Z80_WR     50  // PB3 (was 46/PL3)
#define Z80_M1     10  // PB4 (was 45/PL4)
#define Z80_RFSH   54  // Move to available pin (was 44/PL5)
#define Z80_HALT   12  // PB6 (was 43/PL6)
#define Z80_BUSAK  13  // PB7 (was 42/PL7)

// Update port masks
#define PORTB_MREQ_BIT   0
#define PORTB_IORQ_BIT   1
// ... etc
```

### Step 2: Update HAL Functions
```cpp
// hal.h - Update all PINL references to PINB
inline uint8_t readControlSignals() {
  return ~PINB;  // Was ~PINL
}

inline bool isMemoryRequest() {
  return (PINB & PORTB_MREQ_MASK) == 0;  // Was PINL
}
// ... update all control signal functions
```

### Step 3: Update Interrupt Configuration
```cpp
// hal.cpp
void initializeMemoryHandler() {
  PCICR |= (1 << PCIE0);      // Enable PCINT0 (PORTB)
  PCMSK0 = (1 << PCINT0);     // Monitor MREQ on PB0
  sei();
}
```

### Step 4: Resolve Timer1/PB5 Conflict
**Option A:** Move RFSH to another available pin (e.g., available pin on PORTE)  
**Option B:** Use external buffer to isolate CLK output from RFSH input

---

**End of Analysis**
