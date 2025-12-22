# ISR Implementation Plan
## Z80 Memory Request Handler - Step-by-Step Development Guide

**Date:** November 11, 2025  
**Status:** ✅ Phase 1-2 COMPLETE (Full READ/WRITE support with 6 integrated tests) | ✅ Phase 5 COMPLETE (Ring Buffer + Hybrid Backpressure)  
**Based On:** ISR_Proposal.md, HAL_Proposal.md, TODO.md  
**Purpose:** Actionable implementation steps with clear order of operations

---

## 📊 Implementation Status Summary

**Last Updated:** November 11, 2025

### Phase 0: Hardware Preparation
- ✅ **COMPLETE** - MREQ moved to D2 (PE4, INT4)

### Phase 1: Core ISR Infrastructure (100% Complete) ✅
- ✅ Task 1.1: Pin Definitions - COMPLETE
- ✅ Task 1.2: Global Variables - COMPLETE  
- ✅ Task 1.3: INT4 Initialization - COMPLETE
- ✅ Task 1.4: Basic ISR (READ Only) - COMPLETE
- ✅ Task 1.5: Test Program Loading - COMPLETE
- ✅ Task 1.6: Simple Test Programs - COMPLETE
- ✅ Task 1.7: Basic Integration Testing - COMPLETE

### Phase 2: WRITE Support (100% Complete) ✅
- ✅ Task 2.1: WRITE Detection - COMPLETE
- ✅ Task 2.2: Data Capture - COMPLETE
- ✅ Task 2.3: Capture Modes Implementation - COMPLETE
- ✅ Task 2.4: Integration Testing - COMPLETE (ISR test suite now includes 6 tests: 3 READ + 3 WRITE)

### Phase 3: Optimization
- ⏸️ Not Started

### Phase 4: Advanced Features
- ⏸️ Not Started

### Phase 5: Ring Buffer & Hybrid Backpressure (100% Complete) ✅
- ✅ Task 5.1: Ring Buffer Data Structures - COMPLETE
- ✅ Task 5.2: captureOperation() Function - COMPLETE (ISR-optimized)
- ✅ Task 5.3: ISR Integration for Capture - COMPLETE
- ✅ Task 5.4: Buffer Management Functions - COMPLETE
- ✅ Task 5.5: Hybrid Backpressure with Progressive Throttling - COMPLETE

**Current Milestone:** Phases 1, 2, and 5 COMPLETE - ISR with full READ/WRITE support, 6 integrated tests, ring buffer capture, and intelligent hybrid backpressure (WAIT + progressive clock throttling)

---

## Overview

This document provides a detailed, ordered implementation plan for the Z80 Memory Request Handler ISR. Each phase builds on the previous one, with clear deliverables and verification steps.

**Total Estimated Time:** 4-6 weeks  
**Developer Commitment:** 10-15 hours per week  
**Testing Approach:** Incremental validation at each phase

---

## Phase 0: Hardware Preparation (CRITICAL - COMPLETED ✅)

### ⚠️ HARDWARE CHANGE - **COMPLETED November 7, 2025** ✅

**Action:** Move MREQ signal from D49 to D2 - **COMPLETED**

**Status:** ✅ **HARDWARE WIRING COMPLETE**

**Final Pin Configuration:**
- ✅ MREQ moved from D49 (PL0) to D2 (PE4, INT4)
- ✅ Control signals reassigned on PORTL:
  - IORQ: Pin 48 (PL1)
  - RD: Pin 47 (PL2)
  - WR: Pin 46 (PL3)
  - M1: Pin 45 (PL4)
  - RFSH: Pin 44 (PL5)
  - HALT: Pin 43 (PL6)
  - BUSAK: Pin 42 (PL7)

**Why:** PORTL does not support external interrupts on ATmega2560. We need INT4 for falling-edge triggered interrupts.

### Completed Steps:

#### 0.1 Hardware Rewiring - ✅ COMPLETE
- ✅ **Disconnected** MREQ from Arduino pin D49 (PL0)
- ✅ **Connected** MREQ to Arduino pin D2 (PE4, INT4)
- ✅ **Verified** all other signals remain connected (PORTL: IORQ, RD, WR, M1, RFSH, HALT, BUSAK)
- ✅ **Tested** continuity with multimeter
- ✅ **Updated** hardware documentation

**Deliverable:** ✅ Updated wiring with MREQ on D2  
**Verification:** ✅ Multimeter continuity test, visual inspection completed  
**Risk:** Low - simple single-wire change

---

## Phase 1: Core ISR Infrastructure (Week 1: 8-12 hours) - ✅ IN PROGRESS

### Goal: Implement basic interrupt-driven memory reading

**Progress Update (November 7, 2025):**
- ✅ Task 1.1: Pin Definitions - COMPLETE
- ✅ Task 1.2: Global Variables - COMPLETE
- ✅ Task 1.3: INT4 Initialization - COMPLETE
- ✅ Task 1.4: Basic ISR Implementation - COMPLETE
- ✅ Task 1.5: Test Program Loading - COMPLETE
- ✅ Task 1.6: Simple Test Programs - COMPLETE
- ⏸️ Task 1.7: Basic Integration Testing - PENDING

### 1.1 Update Pin Definitions (30 minutes) - ✅ COMPLETE

**File:** `src/pins.h` (or create if doesn't exist)

```cpp
// Z80 Control Signals (Updated for Separate ISRs)
// ✅ HARDWARE UPDATED: November 14, 2025
#define Z80_MREQ        47  // PL2 - MOVED FROM D2!
#define Z80_IORQ       48   // PL1 (PORTL)
#define Z80_RD         3    // PE5 (INT5) - MOVED FROM D47!
#define Z80_WR         2    // PE4 (INT4) - MOVED FROM D46!
#define Z80_M1         45   // PL4 (PORTL)
#define Z80_RFSH       44   // PL5 (PORTL)
#define Z80_HALT       43   // PL6 (PORTL)
#define Z80_BUSAK      42   // PL7 (PORTL)

// Port masks for PORTL (all signals except MREQ)
#define PORTL_IORQ_MASK  (1 << 1)  // PL1
#define PORTL_RD_MASK    (1 << 2)  // PL2
#define PORTL_WR_MASK    (1 << 3)  // PL3
#define PORTL_M1_MASK    (1 << 4)  // PL4
#define PORTL_RFSH_MASK  (1 << 5)  // PL5
#define PORTL_HALT_MASK  (1 << 6)  // PL6
#define PORTL_BUSAK_MASK (1 << 7)  // PL7

// Port masks for PORTE (MREQ only)
#define PORTE_MREQ_MASK  (1 << 4)  // PE4
```

**Action Items:**
- ✅ Create or update `pins.h` with new definitions
- ✅ Add comments explaining the port mapping
- ✅ Include in main sketch: `#include "pins.h"`

**Deliverable:** ✅ Updated pin definitions file  
**Verification:** ✅ Code compiles without errors

**Implementation Notes (November 7, 2025):**
- Pin definitions already existed in `Z80_Validation/pins.h`
- All PORTL and PORTE masks already defined
- MREQ correctly defined on pin 2 (PE4, INT4)
- All port bit assignments documented

---

### 1.2 Create Global Variables (30 minutes) - ✅ COMPLETE

**File:** `src/hal.h` or new `src/isr_handler.h`

```cpp
// Test program storage (in PROGMEM - flash memory)
extern const uint8_t g_testProgram[] PROGMEM;
extern uint16_t g_testProgramLength;

// SRAM cache for fast access (optional, but recommended)
extern uint8_t g_programCache[256];
extern bool g_useCaching;
extern uint16_t g_cacheSize;

// Write capture (volatile - accessed by ISR and main loop)
extern volatile uint8_t g_capturedData;
extern volatile uint16_t g_lastWriteAddress;
extern volatile bool g_dataReady;

// ISR statistics (optional, for debugging)
extern volatile uint32_t g_isrCallCount;
extern volatile uint32_t g_isrErrorCount;
```

**File:** `src/hal.cpp` or `src/isr_handler.cpp`

```cpp
// Initialize all global variables
const uint8_t g_testProgram[] PROGMEM = {};  // Empty initially
uint16_t g_testProgramLength = 0;

uint8_t g_programCache[256];
bool g_useCaching = false;
uint16_t g_cacheSize = 0;

volatile uint8_t g_capturedData = 0;
volatile uint16_t g_lastWriteAddress = 0;
volatile bool g_dataReady = false;

volatile uint32_t g_isrCallCount = 0;
volatile uint32_t g_isrErrorCount = 0;
```

**Action Items:**
- ✅ Create header file with extern declarations
- ✅ Create implementation file with variable definitions
- ✅ Add proper volatile qualifiers for ISR-accessed variables
- ✅ Add comments explaining each variable's purpose

**Deliverable:** ✅ Global variable declarations and definitions  
**Verification:** ✅ Code compiles, variables accessible

**Implementation Notes (November 7, 2025):**
- Variables added to existing `hal.h` and `hal.cpp`
- Added missing variables:
  - `g_cacheSize` - tracks size of cached program
  - `g_dataReady` - signals when write data is available
  - `g_isrCallCount` - counts ISR invocations
  - `g_isrErrorCount` - counts ISR errors
- All variables properly initialized to safe defaults
- Volatile qualifiers applied to ISR-accessed variables

---

### 1.3 Implement INT4 Initialization (1 hour) - ✅ COMPLETE

**File:** `src/hal.cpp`

```cpp
/**
 * Initialize External Interrupt 4 (INT4) for MREQ signal
 * Configures falling edge trigger for Z80 memory requests
 * 
 * CRITICAL: Must be called AFTER pinMode() configuration
 * CRITICAL: Call sei() to enable global interrupts
 */
void initializeMemoryHandler() {
    // Ensure MREQ pin is configured as input
    pinMode(Z80_MREQ, INPUT);
    
    // Configure INT4 for falling edge trigger
    // EICRB controls INT4-INT7
    // ISC41:ISC40 = 10 (falling edge)
    EICRB &= ~((1 << ISC41) | (1 << ISC40));  // Clear bits
    EICRB |= (1 << ISC41);                     // Set bit 1 only
    
    // Enable INT4 in interrupt mask register
    EIMSK |= (1 << INT4);
    
    // Enable global interrupts
    sei();
    
    Serial.println(F("INFO: Memory handler initialized (INT4 on D2)"));
}
```

**Action Items:**
- ✅ Implement `initializeMemoryHandler()` function
- ✅ Add function to HAL header file
- ✅ Call from `setup()` in main sketch
- ✅ Add detailed comments about register configuration

**Deliverable:** ✅ Interrupt initialization function  
**Verification:** ✅ Function compiles, Serial output appears

**Implementation Notes (November 7, 2025):**
- Function implemented in `hal.cpp`
- Function declaration added to `hal.h` with full documentation
- PORTE (PE4) pin configuration added to `initializeHAL()`
- INT4 configuration uses improved bit clearing (both bits cleared first)
- Interrupt flag cleared before enabling (EIFR)
- Global interrupts enabled with `sei()`
- Serial confirmation message added

---

### 1.4 Implement Basic ISR - READ Only (2-3 hours) - ✅ COMPLETE

**File:** `src/hal.cpp` (ISR added at end of file)

```cpp
/**
 * External Interrupt 4 Service Routine
 * Triggered on MREQ falling edge (Z80 memory request)
 * 
 * Performance: ~1.1 µs total latency @ 16 MHz
 * Supports: READ operations with SRAM caching
 * 
 * CRITICAL: Keep ISR as fast as possible!
 * CRITICAL: MREQ is guaranteed active by hardware (falling edge)
 */
ISR(INT4_vect) __attribute__((hot, flatten));

ISR(INT4_vect) {
    // Increment call counter (optional, for debugging)
    g_isrCallCount++;
    
    // ========================================
    // STAGE 1: Read Control Signals (4 cycles)
    // ========================================
    
    // Read control signals from PORTL and invert (Z80 signals are active-low)
    // NO NEED to check MREQ - hardware guarantees it's active!
    register uint8_t control = ~PINL;
    
    // Read 16-bit address from bus (2 cycles)
    // A0-A7 on PORTC, A8-A15 on PORTK
    register uint16_t address = ((uint16_t)PINK << 8) | PINC;
    
    // ========================================
    // STAGE 2: Determine Operation Type (2 cycles)
    // ========================================
    
    // Check if this is a READ operation (RD active)
    if (control & PORTL_RD_MASK) {
        // READ cycle: Z80 is reading from memory
        
        // ====================================
        // STAGE 3a: Provide Data (4-8 cycles)
        // ====================================
        
        // Set data bus as output (1 cycle)
        DDRA = 0xFF;
        
        // Use SRAM cache if available and address in range (FAST PATH)
        if (g_useCaching && address < g_cacheSize) {
            // FAST: Read from SRAM cache (2-3 cycles)
            PORTA = g_programCache[address];
        }
        // Fallback to PROGMEM for larger programs
        else if (address < g_testProgramLength) {
            // SLOWER: Read byte from PROGMEM (4-6 cycles)
            PORTA = pgm_read_byte(&g_testProgram[address]);
        } 
        else {
            // Address out of range - return NOP (0x00)
            PORTA = 0x00;  // Safe default
            g_isrErrorCount++;  // Track out-of-bounds accesses
        }
        
        // Data is now on bus and will remain until RD goes inactive
        return;  // Exit ISR (4 cycles)
    }
    
    // ========================================
    // STAGE 3b: Handle Non-Read Operations
    // ========================================
    
    // Not a READ operation - set data bus to safe input state
    // (WRITE handling will be added in Phase 2)
    DDRA = 0x00;
    
    return;  // Exit ISR
}
```

**Action Items:**
- ✅ Implement ISR with READ-only support
- ✅ Add GCC optimization attributes (`hot`, `flatten`)
- ✅ Include proper comments for each stage
- ✅ Add bounds checking for safety
- ✅ Test SRAM cache path
- ✅ Test PROGMEM fallback path

**Deliverable:** ✅ Working ISR that handles READ operations  
**Verification:** ✅ ISR compiles, no syntax errors

**Implementation Notes (November 7, 2025):**
- ISR implemented step-by-step:
  - **Step 1.4.1:** Created ISR skeleton with attributes and stage markers
  - **Step 1.4.2:** Implemented STAGE 1 - Read control signals (PORTL inversion) and address bus
  - **Step 1.4.3:** Implemented STAGE 2 - Check RD signal for READ operation
  - **Step 1.4.4:** Implemented STAGE 3a - Data provision with SRAM cache and PROGMEM fallback
  - **Step 1.4.5:** Implemented STAGE 3b - Safe data bus state for non-READ operations
- Used `register` keyword for critical variables (control, address)
- SRAM cache provides fast path (2-3 cycles)
- PROGMEM fallback for larger programs (4-6 cycles)
- NOP (0x00) returned for out-of-bounds addresses
- Error counter tracks invalid accesses
- ISR placed at end of `hal.cpp` (not separate file)

---

### 1.5 Implement Test Program Loading (1-2 hours) - ✅ COMPLETE

**File:** `src/hal.cpp`

```cpp
/**
 * Load test program from PROGMEM into SRAM cache
 * Call BEFORE starting Z80 clock for best performance
 * 
 * @param progmem_ptr Pointer to test program in PROGMEM
 * @param length Length of program in bytes (max 256 for caching)
 * @return true if successfully cached, false if using PROGMEM only
 */
bool loadProgramToCache(const uint8_t* progmem_ptr, uint16_t length) {
    // Set PROGMEM pointer for fallback
    g_testProgram = progmem_ptr;
    g_testProgramLength = length;
    
    // Check if program fits in cache
    if (length <= 256) {
        // Copy from PROGMEM to SRAM
        for (uint16_t i = 0; i < length; i++) {
            g_programCache[i] = pgm_read_byte(&progmem_ptr[i]);
        }
        
        g_cacheSize = length;
        g_useCaching = true;
        
        Serial.print(F("INFO: Program cached ("));
        Serial.print(length);
        Serial.println(F(" bytes in SRAM)"));
        
        return true;
    } else {
        // Program too large for cache - use PROGMEM directly
        g_useCaching = false;
        g_cacheSize = 0;
        
        Serial.print(F("INFO: Using PROGMEM ("));
        Serial.print(length);
        Serial.println(F(" bytes, no caching)"));
        
        return false;
    }
}

/**
 * Set test program without caching (PROGMEM only)
 * Use for very large test programs (>256 bytes)
 */
void setTestProgram(const uint8_t* progmem_ptr, uint16_t length) {
    g_testProgram = progmem_ptr;
    g_testProgramLength = length;
    g_useCaching = false;
    g_cacheSize = 0;
    
    Serial.print(F("INFO: Program set ("));
    Serial.print(length);
    Serial.println(F(" bytes, PROGMEM only)"));
}
```

**Action Items:**
- ✅ Implement `loadProgramToCache()` function
- ✅ Implement `setTestProgram()` function
- ✅ Add error handling for null pointers
- ✅ Add Serial output for debugging
- ✅ Add functions to header file

**Deliverable:** ✅ Program loading functions  
**Verification:** ✅ Functions compile, Serial output appears

**Implementation Notes (November 7, 2025):**
- Both functions implemented in `hal.cpp`
- Function declarations added to `hal.h` (new TEST PROGRAM MANAGEMENT section)
- `loadProgramToCache()`:
  - Changed return type from `void` to `bool`
  - Returns `true` if cached, `false` if PROGMEM fallback
  - Always sets `g_testProgram` pointer for fallback capability
  - Sets `g_cacheSize` appropriately
  - Handles programs >256 bytes (automatic PROGMEM fallback)
  - Null pointer check with error message
  - Serial output for both cached and non-cached paths
- `setTestProgram()`:
  - Explicitly disables caching (`g_useCaching = false`)
  - Clears cache size (`g_cacheSize = 0`)
  - Null pointer check with error message
  - Serial confirmation message
    - Calls `clearMemory()` to reset write captures

---

### 1.6 Create Simple Test Program (30 minutes) - ✅ COMPLETE

**Files:** `test_programs.h` and `test_programs.cpp` (new files)

```cpp
#include <Arduino.h>

/**
 * Test Program 1: Simple NOP sequence
 * Purpose: Verify ISR handles basic READ operations
 * Expected: Z80 fetches 4 NOPs, then HALTs
 */
const uint8_t TEST_PROG_NOPS[] PROGMEM = {
    0x00,  // NOP
    0x00,  // NOP
    0x00,  // NOP
    0x76   // HALT
};

/**
 * Test Program 2: Infinite NOP loop
 * Purpose: Verify continuous operation
 * Expected: Z80 executes NOPs indefinitely
 */
const uint8_t TEST_PROG_NOP_LOOP[] PROGMEM = {
    0x00,  // 0x0000: NOP
    0x00,  // 0x0001: NOP
    0xC3,  // 0x0002: JP 0x0000
    0x00,  // 0x0003: (low byte of address)
    0x00   // 0x0004: (high byte of address)
};

/**
 * Test Program 3: Single HALT
 * Purpose: Minimal test - verify basic fetch
 * Expected: Z80 immediately halts
 */
const uint8_t TEST_PROG_HALT[] PROGMEM = {
    0x76   // HALT
};
```

**Action Items:**
- ✅ Create test programs file
- ✅ Define 2-3 simple test programs in PROGMEM
- ✅ Add detailed comments explaining expected behavior
- ✅ Add header file with extern declarations

**Deliverable:** ✅ Test programs in PROGMEM  
**Verification:** ✅ Code compiles, programs defined

**Implementation Notes (November 7, 2025):**
- Created `isr_test_programs.h` with extern declarations and comprehensive documentation
- Created `isr_test_programs.cpp` with three test programs:
  1. **TEST_PROG_NOPS** (4 bytes) - Three NOPs followed by HALT
  2. **TEST_PROG_NOP_LOOP** (5 bytes) - Infinite loop using JP instruction
  3. **TEST_PROG_HALT** (1 byte) - Single HALT for minimal testing
- All programs include:
  - Size constants (e.g., TEST_PROG_NOPS_SIZE)
  - Detailed purpose and expected behavior documentation
  - Machine code explanations
  - Verification criteria
- All programs stored in PROGMEM (flash) to conserve SRAM
- All programs are <256 bytes for SRAM caching capability
- **Compilation verified:** Project compiles without errors on Arduino Mega 2560

---

### 1.7 Integration and First Test (2-3 hours) - ✅ COMPLETE

**Status:** COMPLETE - November 7, 2025

**Implementation:** Integrated into Diagnostic Menu (Option 3)

**Files Modified:**
- `menu.cpp` - Added menu integration
- `isr_test_programs.h` - Added `runISRTest()` declaration
- `isr_test_programs.cpp` - Implemented comprehensive test suite

**Test Suite Implementation:**

Three test programs execute sequentially at 100 kHz:

1. **Test 1: NOP Sequence + HALT** (4 bytes, 1 second)
   - 3 NOPs followed by HALT
   - Validates basic READ operations
   - Expected: ~50,000 MREQ cycles (half READ, half RFSH)

2. **Test 2: Single HALT** (1 byte, 500ms)
   - Simplest possible test
   - Validates minimal instruction fetch
   - Expected: ~25,000 MREQ cycles

3. **Test 3: Infinite NOP Loop** (5 bytes, 500ms)
   - Uses JP instruction to loop
   - Validates continuous operation and jump instructions
   - Expected: ~25,000 MREQ cycles

**Key Features Implemented:**
- Sequential test execution with automatic continuation if tests pass
- Proper WAIT signal management (assert after reset, release before execution)
- RFSH cycle filtering (refresh cycles ignored, not counted as errors)
- Out-of-bounds address handling (returns NOP, normal during HALT)
- Clock state preservation (saves and restores original clock settings)
- Comprehensive results display with pass/fail analysis
- Menu integration with proper waitForKeyPress() handling

**Action Items:**
- ✅ Add ISR initialization to test
- ✅ Load test programs sequentially
- ✅ Start clock at 100 kHz (safe speed)
- ✅ Run tests with appropriate durations
- ✅ Display statistics for each test
- ✅ Add final summary with overall pass/fail

**Deliverable:** ✅ Working test suite that executes all three Z80 programs  

**Verification:** ✅ ALL COMPLETE
- Code compiles and uploads successfully
- Serial output shows ISR calls > 0 for all tests
- Zero errors detected on all tests
- No crashes or resets
- All three tests pass
- Menu integration works correctly

**Actual Results (November 7, 2025):**
- Test 1: ~50,000 ISR calls, 0 errors - PASSED ✅
- Test 2: ~25,000 ISR calls, 0 errors - PASSED ✅
- Test 3: ~25,000 ISR calls, 0 errors - PASSED ✅
- Overall: 3/3 tests passed
- ISR functioning correctly at 100 kHz
- Z80 executes programs as expected
- RFSH filtering working correctly
- No bus contention or hardware issues

**Critical Fixes Applied:**
1. **WAIT Signal Handling:** `resetZ80()` asserts WAIT; test releases it before starting clock
2. **RFSH Filtering:** Added check for `PORTL_RFSH_MASK` to ignore refresh cycles
3. **Out-of-bounds Handling:** Removed error counting for addresses beyond program (normal during HALT)
4. **Clock Management:** Proper save/restore of original clock state

**Implementation Notes:**
- Sequential execution stops on first failure
- Each test resets Z80 and clears ISR counters
- Tests use different durations based on expected activity
- Helper function `runSingleISRTest()` encapsulates common test logic
- Final summary displays pass/fail status clearly
- Integration with existing menu system seamless

---

## Phase 1 Completion Checklist

- ✅ Hardware: MREQ moved to D2
- ✅ Pin definitions updated
- ✅ Global variables defined
- ✅ INT4 interrupt configured
- ✅ ISR implemented (READ only)
- ✅ Program loading functions implemented
- ✅ Test programs created
- ✅ Integration test passes
- ✅ Serial output shows ISR activity
- ✅ No crashes or hardware issues

**Phase 1 Success Criteria:** ✅ ALL MET
- ✅ Z80 executes NOP program at 100 kHz  
- ✅ ISR is called on every memory request  
- ✅ No bus contention or hardware damage  
- ✅ Statistics show expected number of ISR calls
- ✅ RFSH cycles properly filtered
- ✅ All three test programs execute successfully

**Phase 1 COMPLETE - November 7, 2025** 🎉

---

## Phase 2: WRITE Support & Capture (Week 2: 6-10 hours)

### Goal: Add WRITE operation handling and data capture

### 2.1 Extend ISR for WRITE Operations (1-2 hours)

**File:** `src/isr_handler.cpp`

Modify the ISR to add WRITE handling after the READ section:

```cpp
ISR(INT4_vect) {
    g_isrCallCount++;
    
    register uint8_t control = ~PINL;
    register uint16_t address = ((uint16_t)PINK << 8) | PINC;
    
    // READ operation (existing code)
    if (control & PORTL_RD_MASK) {
        DDRA = 0xFF;
        // ... existing READ code ...
        return;
    }
    
    // ========================================
    // NEW: WRITE operation
    // ========================================
    else if (control & PORTL_WR_MASK) {
        // WRITE cycle: Z80 is writing to memory
        
        // Set data bus as input (1 cycle)
        DDRA = 0x00;
        
        // Capture data from Z80 (1 cycle)
        register uint8_t data = PINA;
        
        // Store captured write for verification (4 cycles)
        g_capturedData = data;
        g_lastWriteAddress = address;
        g_dataReady = true;  // Signal to main loop
        
        return;  // Exit ISR
    }
    
    // Neither READ nor WRITE - possibly refresh cycle
    DDRA = 0x00;  // Safe state
    g_isrErrorCount++;
    return;
}
```

**Action Items:**
- [ ] Add WRITE branch to ISR
- [ ] Capture data from PINA
- [ ] Store address and data in global variables
- [ ] Set `g_dataReady` flag

**Deliverable:** ISR with WRITE support  
**Verification:** Code compiles

---

### 2.2 Create WRITE Test Programs (1 hour)

**File:** `src/test_programs.cpp`

```cpp
/**
 * Test Program 3: Simple LD and write
 * Purpose: Verify ISR captures WRITE operations
 * Expected: Writes 0x42 to address 0x8000
 */
const uint8_t TEST_PROG_WRITE[] PROGMEM = {
    0x3E, 0x42,  // 0x0000: LD A, 0x42
    0x32, 0x00, 0x80,  // 0x0002: LD (0x8000), A
    0x76         // 0x0005: HALT
};

/**
 * Test Program 4: Multiple writes
 * Purpose: Test write capture buffer
 * Expected: Multiple writes to different addresses
 */
const uint8_t TEST_PROG_MULTI_WRITE[] PROGMEM = {
    0x3E, 0x11,  // LD A, 0x11
    0x32, 0x00, 0x80,  // LD (0x8000), A
    0x3E, 0x22,  // LD A, 0x22
    0x32, 0x01, 0x80,  // LD (0x8001), A
    0x76         // HALT
};
```

**Action Items:**
- [ ] Create test programs with WRITE operations
- [ ] Use simple LD instructions
- [ ] Write to safe memory regions (e.g., 0x8000+)

**Deliverable:** WRITE test programs  
**Verification:** Code compiles

---

### 2.3 Implement Write Validation (1-2 hours)

**File:** `src/hal.cpp` or new `src/validation.cpp`

```cpp
/**
 * Check for captured writes and validate
 * Call from main loop after running test
 */
void processCaptures() {
    if (g_dataReady) {
        Serial.print(F("WRITE: Addr=0x"));
        Serial.print(g_lastWriteAddress, HEX);
        Serial.print(F(", Data=0x"));
        Serial.println(g_capturedData, HEX);
        
        g_dataReady = false;  // Clear flag
    }
}

/**
 * Validate expected write occurred
 * Returns true if write matches expected values
 */
bool validateWrite(uint16_t expectedAddr, uint8_t expectedData) {
    if (!g_dataReady) {
        Serial.println(F("ERROR: No write captured!"));
        return false;
    }
    
    bool addrMatch = (g_lastWriteAddress == expectedAddr);
    bool dataMatch = (g_capturedData == expectedData);
    
    if (addrMatch && dataMatch) {
        Serial.println(F("✓ Write validation PASSED"));
        g_dataReady = false;
        return true;
    } else {
        Serial.println(F("✗ Write validation FAILED"));
        if (!addrMatch) {
            Serial.print(F("  Expected addr: 0x"));
            Serial.print(expectedAddr, HEX);
            Serial.print(F(", Got: 0x"));
            Serial.println(g_lastWriteAddress, HEX);
        }
        if (!dataMatch) {
            Serial.print(F("  Expected data: 0x"));
            Serial.print(expectedData, HEX);
            Serial.print(F(", Got: 0x"));
            Serial.println(g_capturedData, HEX);
        }
        g_dataReady = false;
        return false;
    }
}
```

**Action Items:**
- [ ] Implement `processCaptures()` function
- [ ] Implement `validateWrite()` function
- [ ] Add helper functions for formatting output

**Deliverable:** Write validation functions  
**Verification:** Functions compile

---

### 2.4 WRITE Integration Test ✅ COMPLETE (2-3 hours)

**Status:** ✅ **COMPLETE** - November 11, 2025  
**Files Modified:** 
- `Z80_Validation/isr_test_programs.cpp` - Added `runSingleWriteTest()` helper, modified `runISRTest()`
- `Z80_Validation/isr_test_programs.h` - Updated documentation

**Implementation Details:**

1. **Added `runSingleWriteTest()` Helper Function**
   - Similar structure to `runSingleISRTest()` but for WRITE validation
   - Takes expected address and data parameters  
   - Uses `validateWrite()` for automated checking
   - Consistent output format with READ tests

2. **Modified `runISRTest()` Function**
   - **Header:** Changed from "MEMORY ISR TEST SUITE" to "ISR TEST SUITE (READ & WRITE)"
   - **Tests:** Expanded from 3 to 6 tests total (3 READ + 3 WRITE)
   - **Removed:** DEBUG data bus pattern section
   - **Updated:** Success message mentions both Phase 1 and Phase 2 complete

**Test Scenarios Implemented:**
- **Test 4:** Single WRITE (0x8000 = 0x42) - validates basic WRITE capture
- **Test 5:** Multiple WRITEs (validates last: 0x8002 = 0x33) - tests sequential writes  
- **Test 6:** Continuous WRITE Loop (0x8000 = 0xAA) - tests sustained WRITE operations

**Integration:**
- No menu changes needed - existing Option 3 now tests both READ and WRITE
- Maintains backward compatibility
- All tests use CAPTURE_OFF mode for simplicity
- Consistent error handling and reporting

**Action Items:** ✅ All completed  
**Deliverable:** ✅ Working WRITE capture test integrated into main test suite  
**Verification:** ✅ All 6 tests pass, WRITE operations captured correctly

---

## Phase 2 Completion Checklist ✅ ALL COMPLETE

- [x] ISR extended with WRITE handling
- [x] WRITE test programs created
- [x] Write validation functions implemented
- [x] Integration test passes (6 total tests: 3 READ + 3 WRITE)
- [x] Captured writes match expected values
- [x] No data bus direction issues
- [x] ISR timing fixes for WR signal detection
- [x] Data bus glitch prevention implemented

**Phase 2 Success Criteria:** ✅ ALL MET
✅ ISR captures WRITE operations  
✅ Address and data are correct  
✅ Multiple writes can be captured (sequential)  
✅ No bus contention or hardware issues  
✅ Full integration with existing test suite  
✅ Reliable operation across Z80 frequencies

---

## Phase 3: Ring Buffer & Backpressure (Week 3: 8-12 hours)

### Goal: Implement adaptive buffering with automatic WAIT states

### 3.1 Define Ring Buffer Structures (1 hour)

**File:** `src/ring_buffer.h` (new file)

```cpp
#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <Arduino.h>

// Configuration
#define CAPTURE_BUFFER_SIZE 16      // Power of 2 for efficiency
#define BUFFER_HIGH_WATER   12      // 75% full triggers WAIT
#define BUFFER_LOW_WATER    4       // 25% full releases WAIT

// Capture entry structure
struct CaptureEntry {
    uint16_t address;               // Z80 address
    uint8_t  data;                  // Data byte
    uint8_t  operation;             // READ=0, WRITE=1
    uint32_t timestamp;             // micros() for timing analysis
};

// Ring buffer (circular queue)
extern volatile CaptureEntry g_captureBuffer[CAPTURE_BUFFER_SIZE];
extern volatile uint8_t g_captureHead;   // ISR writes here (producer)
extern volatile uint8_t g_captureTail;   // App reads here (consumer)

// Capture control
extern volatile bool g_captureEnabled;
extern volatile bool g_waitAsserted;

// Capture modes
enum CaptureMode {
    CAPTURE_OFF = 0,          // No capture
    CAPTURE_BUFFERED = 1,     // Ring buffer with auto-WAIT
    CAPTURE_IMMEDIATE = 2     // Pre-assert WAIT, single operation
};
extern volatile CaptureMode g_captureMode;

// Function declarations
void initializeRingBuffer();
inline void captureOperation(uint16_t addr, uint8_t data, uint8_t op);
void processCaptureBuffer();
uint8_t getCaptureBufferLevel();
void assertWait();
void releaseWait();

#endif // RING_BUFFER_H
```

**Action Items:**
- [ ] Create ring buffer header file
- [ ] Define CaptureEntry structure
- [ ] Define buffer configuration constants
- [ ] Declare global variables
- [ ] Define capture modes enum

**Deliverable:** Ring buffer header file  
**Verification:** Code compiles

---

### 3.2 Implement Ring Buffer Functions (2-3 hours)

**File:** `src/ring_buffer.cpp` (new file)

```cpp
#include "ring_buffer.h"
#include "pins.h"

// Define global variables
volatile CaptureEntry g_captureBuffer[CAPTURE_BUFFER_SIZE];
volatile uint8_t g_captureHead = 0;
volatile uint8_t g_captureTail = 0;
volatile bool g_captureEnabled = false;
volatile bool g_waitAsserted = false;
volatile CaptureMode g_captureMode = CAPTURE_OFF;

/**
 * Initialize ring buffer
 * Call once during setup
 */
void initializeRingBuffer() {
    g_captureHead = 0;
    g_captureTail = 0;
    g_captureEnabled = false;
    g_waitAsserted = false;
    g_captureMode = CAPTURE_OFF;
    
    // Configure WAIT pin as output
    pinMode(Z80_WAIT, OUTPUT);
    digitalWrite(Z80_WAIT, HIGH);  // WAIT inactive (active low)
    
    Serial.println(F("INFO: Ring buffer initialized"));
}

/**
 * Fast capture function called from ISR
 * Adds entry to ring buffer and manages WAIT signal
 */
inline void captureOperation(uint16_t addr, uint8_t data, uint8_t op) {
    if (g_captureMode != CAPTURE_BUFFERED) {
        return;  // Capture disabled or using different mode
    }
    
    uint8_t next = (g_captureHead + 1) % CAPTURE_BUFFER_SIZE;
    
    // Check if buffer is full
    if (next == g_captureTail) {
        // Buffer full - data will be lost!
        if (!g_waitAsserted) {
            assertWait();
            g_waitAsserted = true;
        }
        return;  // Drop this entry to prevent corruption
    }
    
    // Add entry to buffer
    g_captureBuffer[g_captureHead].address = addr;
    g_captureBuffer[g_captureHead].data = data;
    g_captureBuffer[g_captureHead].operation = op;
    g_captureBuffer[g_captureHead].timestamp = micros();
    g_captureHead = next;
    
    // Check buffer level - assert WAIT if approaching full
    uint8_t count = getCaptureBufferLevel();
    
    if (count >= BUFFER_HIGH_WATER && !g_waitAsserted) {
        assertWait();
        g_waitAsserted = true;
    }
}

/**
 * Process captured data from ring buffer
 * Call frequently from main loop
 */
void processCaptureBuffer() {
    while (g_captureTail != g_captureHead) {
        // Get next entry
        volatile CaptureEntry* entry = &g_captureBuffer[g_captureTail];
        
        // Process the captured operation
        Serial.print(entry->timestamp);
        Serial.print(F(": "));
        Serial.print(entry->operation == 0 ? F("READ ") : F("WRITE"));
        Serial.print(F(" Addr=0x"));
        Serial.print(entry->address, HEX);
        Serial.print(F(" Data=0x"));
        Serial.println(entry->data, HEX);
        
        // Advance tail (consumer)
        g_captureTail = (g_captureTail + 1) % CAPTURE_BUFFER_SIZE;
        
        // Check if buffer has drained enough to release WAIT
        uint8_t count = getCaptureBufferLevel();
        
        if (count <= BUFFER_LOW_WATER && g_waitAsserted) {
            releaseWait();
            g_waitAsserted = false;
        }
    }
}

/**
 * Get current buffer usage (for monitoring)
 */
uint8_t getCaptureBufferLevel() {
    uint8_t head = g_captureHead;  // Snapshot (volatile)
    uint8_t tail = g_captureTail;
    
    if (head >= tail) {
        return head - tail;
    } else {
        return CAPTURE_BUFFER_SIZE - tail + head;
    }
}

/**
 * Assert WAIT signal (hold Z80)
 */
void assertWait() {
    digitalWrite(Z80_WAIT, LOW);  // Active low
}

/**
 * Release WAIT signal (resume Z80)
 */
void releaseWait() {
    digitalWrite(Z80_WAIT, HIGH);  // Inactive
}
```

**Action Items:**
- [ ] Implement ring buffer management functions
- [ ] Add capture function for ISR
- [ ] Add processing function for main loop
- [ ] Implement WAIT assertion/release
- [ ] Add buffer level monitoring

**Deliverable:** Ring buffer implementation  
**Verification:** Code compiles

---

### 3.3 Integrate Ring Buffer into ISR (1 hour)

**File:** `src/isr_handler.cpp`

Modify ISR to call capture function:

```cpp
ISR(INT4_vect) {
    g_isrCallCount++;
    
    register uint8_t control = ~PINL;
    register uint16_t address = ((uint16_t)PINK << 8) | PINC;
    
    // READ operation
    if (control & PORTL_RD_MASK) {
        DDRA = 0xFF;
        
        // Get data (same as before)
        uint8_t data;
        if (g_useCaching && address < g_cacheSize) {
            data = g_programCache[address];
        } else if (address < g_testProgramLength) {
            data = pgm_read_byte(&g_testProgram[address]);
        } else {
            data = 0x00;
        }
        
        PORTA = data;
        
        // NEW: Capture READ operation if enabled
        if (g_captureMode == CAPTURE_BUFFERED) {
            captureOperation(address, data, 0);  // 0 = READ
        }
        
        return;
    }
    
    // WRITE operation
    else if (control & PORTL_WR_MASK) {
        DDRA = 0x00;
        register uint8_t data = PINA;
        
        // NEW: Capture WRITE operation if enabled
        if (g_captureMode == CAPTURE_BUFFERED) {
            captureOperation(address, data, 1);  // 1 = WRITE
        }
        
        return;
    }
    
    DDRA = 0x00;
    g_isrErrorCount++;
    return;
}
```

**Action Items:**
- [ ] Add capture calls to ISR
- [ ] Include ring_buffer.h header
- [ ] Test compilation

**Deliverable:** ISR with ring buffer integration  
**Verification:** Code compiles

---

### 3.4 Create Buffered Capture Test (2-3 hours)

**File:** Main sketch

```cpp
void testBufferedCapture() {
    Serial.println(F("\n--- Testing Buffered Capture ---"));
    
    // Initialize ring buffer
    initializeRingBuffer();
    
    // Enable buffered capture mode
    g_captureMode = CAPTURE_BUFFERED;
    
    // Load test program with multiple operations
    loadProgramToCache(TEST_PROG_MULTI_WRITE, sizeof(TEST_PROG_MULTI_WRITE));
    
    // Reset and run
    resetZ80();
    startClock(100000);  // 100 kHz
    
    // Process buffer in main loop
    unsigned long startTime = millis();
    while (millis() - startTime < 2000) {  // 2 second test
        processCaptureBuffer();
        delay(10);  // Don't hog CPU
        
        // Monitor buffer level
        uint8_t level = getCaptureBufferLevel();
        if (level > 8) {
            Serial.print(F("Buffer: "));
            Serial.print(level);
            Serial.println(F(" entries"));
        }
    }
    
    stopClock();
    
    // Process remaining entries
    processCaptureBuffer();
    
    Serial.println(F("\n✓ Buffered capture test complete\n"));
}
```

**Action Items:**
- [ ] Create test function with buffered capture
- [ ] Run longer test (multiple operations)
- [ ] Monitor buffer level
- [ ] Verify WAIT assertion if buffer fills

**Deliverable:** Buffered capture test  
**Verification:**
- Test runs for full duration
- Buffer entries are processed
- WAIT is asserted if buffer gets too full
- No data loss

---

## Phase 3 Completion Checklist

- [ ] Ring buffer structures defined
- [ ] Ring buffer functions implemented
- [ ] ISR integrated with capture
- [ ] WAIT signal control working
- [ ] Buffer test passes
- [ ] Automatic backpressure working

**Phase 3 Success Criteria:**
✅ Ring buffer captures multiple operations  
✅ WAIT is asserted when buffer fills  
✅ WAIT is released when buffer drains  
✅ No data loss during buffered capture  
✅ Z80 runs at full speed when buffer is empty

---

## Phase 4: Performance Optimization (Week 4: 6-10 hours)

### Goal: Maximize Z80 clock speed and ISR performance

### 4.1 Performance Measurement (2-3 hours)

**File:** `src/performance.cpp` (new file)

```cpp
/**
 * Measure ISR performance with logic analyzer or timing
 */
void measureISRPerformance() {
    Serial.println(F("\n--- ISR Performance Test ---"));
    
    // Disable capture to measure pure ISR speed
    g_captureMode = CAPTURE_OFF;
    
    // Test at various clock speeds
    uint32_t testSpeeds[] = {100000, 500000, 1000000, 2000000};
    
    for (int i = 0; i < 4; i++) {
        uint32_t freq = testSpeeds[i];
        
        Serial.print(F("\nTesting at "));
        Serial.print(freq);
        Serial.println(F(" Hz"));
        
        // Load simple test
        loadProgramToCache(TEST_PROG_NOP_LOOP, sizeof(TEST_PROG_NOP_LOOP));
        
        // Reset counters
        g_isrCallCount = 0;
        g_isrErrorCount = 0;
        
        // Run for 1 second
        resetZ80();
        startClock(freq);
        delay(1000);
        stopClock();
        
        // Calculate throughput
        Serial.print(F("  ISR calls: "));
        Serial.println(g_isrCallCount);
        Serial.print(F("  Errors: "));
        Serial.println(g_isrErrorCount);
        Serial.print(F("  Operations/sec: "));
        Serial.println(g_isrCallCount);
        
        if (g_isrErrorCount > 0) {
            Serial.println(F("  ⚠️  ERRORS DETECTED - Clock too fast!"));
            break;  // Don't test higher speeds
        } else {
            Serial.println(F("  ✓ STABLE"));
        }
    }
    
    Serial.println(F("\nPerformance test complete\n"));
}
```

**Action Items:**
- [ ] Create performance measurement function
- [ ] Test at multiple clock speeds
- [ ] Track ISR call count and errors
- [ ] Identify maximum stable clock speed

**Deliverable:** Performance measurement tool  
**Verification:** Tool runs and reports results

---

### 4.2 Compiler Optimization (1-2 hours)

**File:** `platform.io` (if using PlatformIO) or Arduino IDE settings

Add compiler flags:

```ini
[env:megaatmega2560]
platform = atmelavr
board = megaatmega2560
framework = arduino
build_flags = 
    -O3                    # Maximum optimization
    -flto                  # Link-time optimization
    -ffast-math           # Fast math operations
    -funroll-loops        # Unroll loops
    -DNDEBUG              # Disable asserts
```

**Action Items:**
- [ ] Add optimization flags to build system
- [ ] Recompile and test
- [ ] Measure performance improvement
- [ ] Document changes

**Deliverable:** Optimized build configuration  
**Verification:** Code still works, performance improved

---

### 4.3 Cache Size Optimization (1-2 hours)

Test different cache sizes to find optimal balance:

```cpp
// Test configurations:
// 1. 256 bytes (current)
// 2. 512 bytes
// 3. 1024 bytes
// 4. No cache (PROGMEM only)
```

**Action Items:**
- [ ] Test performance with different cache sizes
- [ ] Measure SRAM usage
- [ ] Document optimal configuration

**Deliverable:** Optimized cache configuration  
**Verification:** Best performance/memory trade-off

---

### 4.4 Maximum Speed Test (2-3 hours)

Push the system to find limits:

```cpp
void findMaximumSpeed() {
    Serial.println(F("\n--- Finding Maximum Clock Speed ---"));
    
    g_captureMode = CAPTURE_OFF;
    loadProgramToCache(TEST_PROG_NOP_LOOP, sizeof(TEST_PROG_NOP_LOOP));
    
    // Start at 1 MHz, increment by 100 kHz
    for (uint32_t freq = 1000000; freq <= 4000000; freq += 100000) {
        Serial.print(F("Testing "));
        Serial.print(freq);
        Serial.print(F(" Hz... "));
        
        g_isrCallCount = 0;
        g_isrErrorCount = 0;
        
        resetZ80();
        startClock(freq);
        delay(500);  // Shorter test at high speeds
        stopClock();
        
        if (g_isrErrorCount > 0 || g_isrCallCount < (freq / 10)) {
            Serial.println(F("FAILED"));
            Serial.print(F("Maximum stable speed: "));
            Serial.print(freq - 100000);
            Serial.println(F(" Hz"));
            break;
        } else {
            Serial.println(F("OK"));
        }
    }
}
```

**Action Items:**
- [ ] Implement speed sweep test
- [ ] Find maximum stable frequency
- [ ] Document results
- [ ] Update specifications

**Deliverable:** Maximum speed determination  
**Verification:** Documented maximum stable clock speed

---

## Phase 4 Completion Checklist

- [ ] Performance measurement tool created
- [ ] Compiler optimizations applied
- [ ] Cache size optimized
- [ ] Maximum speed determined and documented
- [ ] Performance targets met or exceeded

**Phase 4 Success Criteria:**
✅ Z80 runs stably at ≥1 MHz  
✅ ISR latency measured and documented  
✅ Performance optimized for maximum throughput  
✅ Stretch goal: ≥2 MHz operation

---

## Phase 5: Advanced Features (Weeks 5-6: 10-15 hours)

### Goal: Add clock throttling, hybrid backpressure, and diagnostics

### 5.1 Clock Throttling (3-4 hours)

Implement dynamic clock speed adjustment:

```cpp
enum BackpressureMode {
    BP_MODE_WAIT_ONLY,      // Use only WAIT states
    BP_MODE_THROTTLE_ONLY,  // Use only clock throttling
    BP_MODE_HYBRID          // Intelligent switching (default)
};

void manageBackpressure() {
    // Implementation from ISR Proposal Section 14.5
    // ... (see proposal for full code)
}
```

**Action Items:**
- [ ] Implement clock speed adjustment
- [ ] Add backpressure mode selection
- [ ] Test at various speeds
- [ ] Validate hybrid mode

**Deliverable:** Dynamic clock throttling  
**Verification:** System adapts to processing load

---

### 5.2 Diagnostics and Monitoring (2-3 hours)

Add comprehensive diagnostic tools:

```cpp
void printDiagnostics() {
    Serial.println(F("\n=== ISR Diagnostics ==="));
    Serial.print(F("ISR calls: "));
    Serial.println(g_isrCallCount);
    Serial.print(F("ISR errors: "));
    Serial.println(g_isrErrorCount);
    Serial.print(F("Buffer level: "));
    Serial.println(getCaptureBufferLevel());
    Serial.print(F("WAIT asserted: "));
    Serial.println(g_waitAsserted ? F("YES") : F("NO"));
    Serial.print(F("Capture mode: "));
    Serial.println(g_captureMode);
    Serial.print(F("Cache enabled: "));
    Serial.println(g_useCaching ? F("YES") : F("NO"));
    Serial.println();
}
```

**Action Items:**
- [ ] Create diagnostic display function
- [ ] Add to menu system
- [ ] Test all modes

**Deliverable:** Diagnostic tools  
**Verification:** Diagnostics display useful information

---

### 5.3 Menu Integration (2-3 hours)

Add ISR controls to existing menu system:

```
ISR & Memory Menu:
1. Enable/Disable Capture
2. Set Capture Mode
3. Set Backpressure Mode
4. View Diagnostics
5. Performance Test
6. Back to Main Menu
```

**Action Items:**
- [ ] Add ISR menu to menu system
- [ ] Implement menu handlers
- [ ] Test all options

**Deliverable:** Complete menu integration  
**Verification:** All menu options work

---

### 5.4 Documentation (2-3 hours)

**Action Items:**
- [ ] Document ISR API
- [ ] Create user guide
- [ ] Write performance report
- [ ] Update TODO.md with completion status

**Deliverable:** Complete documentation  
**Verification:** Documentation is clear and accurate

---

## Phase 5 Completion Checklist

- [ ] Clock throttling implemented
- [ ] Hybrid backpressure working
- [ ] Diagnostics tools created
- [ ] Menu integration complete
- [ ] Documentation finished

**Phase 5 Success Criteria:**
✅ All advanced features working  
✅ System is fully configurable  
✅ Documentation is complete  
✅ Ready for instruction testing

---

## Testing Strategy

### Unit Tests (Throughout Development)
- Test each function in isolation
- Verify error handling
- Check boundary conditions

### Integration Tests (End of Each Phase)
- Test complete workflows
- Verify interactions between components
- Check for memory leaks

### Hardware Tests (Throughout Development)
- Use logic analyzer to verify timing
- Use oscilloscope to check signals
- Verify no bus contention

### Stress Tests (Phase 4-5)
- Run for extended periods (hours)
- Test at maximum clock speeds
- Test with buffer overflow scenarios

---

## Risk Mitigation

| Risk | Impact | Mitigation |
|------|--------|------------|
| Hardware damage from bus contention | Critical | Always default data bus to input |
| ISR too slow | High | Start at low frequencies, optimize incrementally |
| Memory corruption | High | Use volatile qualifiers, test thoroughly |
| WAIT timing issues | Medium | Test with logic analyzer, add safety margins |
| Buffer overflow | Medium | Implement automatic backpressure |

---

## Success Metrics

### Phase 1
- ✅ Z80 executes at 100 kHz
- ✅ ISR responds to every request
- ✅ No hardware issues

### Phase 2
- ✅ WRITE capture works
- ✅ Data validation passes
- ✅ Multiple operations captured

### Phase 3
- ✅ Ring buffer prevents data loss
- ✅ Automatic backpressure works
- ✅ Full-speed operation when possible

### Phase 4
- ✅ Z80 operates at ≥1 MHz
- ✅ ISR latency <1.5 µs
- ✅ Performance optimized

### Phase 5
- ✅ All features complete
- ✅ System fully configurable
- ✅ Documentation finished

---

## Timeline Summary

| Phase | Duration | Cumulative | Key Milestone |
|-------|----------|------------|---------------|
| Phase 0 | 1-2 hours | 2 hours | Hardware ready |
| Phase 1 | 8-12 hours | 14 hours | Basic ISR working |
| Phase 2 | 6-10 hours | 24 hours | WRITE capture working |
| Phase 3 | 8-12 hours | 36 hours | Buffering & backpressure |
| Phase 4 | 6-10 hours | 46 hours | Performance optimized |
| Phase 5 | 10-15 hours | 61 hours | All features complete |

**Total: 4-6 weeks @ 10-15 hours/week**

---

## Next Steps After Completion

1. **Integrate with Test Framework**
   - Connect ISR to instruction tests
   - Add validation logic
   - Create test suites

2. **Begin Instruction Testing**
   - Start with Phase 2 of TODO.md
   - Test 8-bit loads
   - Expand to full instruction set

3. **Advanced Features**
   - I/O port emulation
   - Interrupt testing
   - Timing validation

---

## Conclusion

This implementation plan provides a clear, step-by-step path to implementing the ISR for Z80 memory request handling. Each phase builds on the previous one, with clear deliverables and verification steps.

**Key Principles:**
- 🔧 Incremental development
- ✅ Test at each stage
- 📊 Measure performance continuously
- 🛡️ Safety first (prevent hardware damage)
- 📝 Document everything

**Ready to begin?** Start with Phase 0 (hardware change) and proceed through each phase sequentially.

---

## Phase 5: Ring Buffer & Hybrid Backpressure System (COMPLETED ✅)

**Date Completed:** November 10, 2025  
**Time Required:** 4 hours  
**Complexity:** High

### Goal: Implement intelligent capture system with adaptive backpressure

This phase adds a sophisticated ring buffer system to capture Z80 operations (READ/WRITE) with hybrid backpressure management that prevents buffer overflow through progressive throttling.

### 5.1 Ring Buffer Data Structures - ✅ COMPLETE

Added to hal.h: CaptureEntry struct, CaptureMode enum, buffer configuration constants
Global variables: Ring buffer array, head/tail pointers, backpressure state tracking
Performance: 128 bytes SRAM for buffer

### 5.2 captureOperation() - ISR-Optimized - ✅ COMPLETE

Performance: ~8-10 cycles (~500 ns @ 16 MHz)
Key optimizations: Bitwise AND, no micros() call, register variables, always_inline

### 5.3 ISR Integration - ✅ COMPLETE

Both READ and WRITE operations support all three capture modes
Optimized fast path for CAPTURE_OFF (default) - minimal overhead

### 5.4 Management Functions - ✅ COMPLETE

Implemented: setCaptureMode(), getCaptureBufferLevel(), clearCaptureBuffer()
processCaptureBuffer(), manageHybridBackpressure()

### 5.5 Hybrid Backpressure with Progressive Throttling - ✅ COMPLETE

**Three-Zone Strategy:**
- HIGH (≥75%): Assert WAIT, progressive throttling every 10ms, floor at 100 KHz
- MEDIUM (25-75%): Maintain state (hysteresis for stability)
- LOW (≤25%): Release WAIT immediately, restore speed after 10ms

**Key Features:**
- ✅ No buffer overflow possible (WAIT + progressive throttling)
- ✅ No oscillation (hysteresis prevents rapid cycling)
- ✅ Graceful degradation under load
- ✅ Fast recovery when buffer drains

### 5.6 Performance Characteristics

Ring Buffer: 128 bytes SRAM, ~100-200 µs capacity @ 1-2 MHz
Backpressure: WAIT immediate (1-2 cycles), throttle ~10-50 µs, recovery single-step
ISR Overhead: CAPTURE_OFF +1-2 cycles, CAPTURE_BUFFERED +8-10 cycles

---

**IMPLEMENTATION COMPLETE: Phases 0, 1, 2, and 5 are done!**
Ready for advanced testing and validation.

