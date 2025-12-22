# Hardware Abstraction Layer (HAL) Proposal
## Z80 Processor Validation Suite

**Date:** November 6, 2025  
**Status:** Proposal - Awaiting Approval  
**Author:** Z80 Validation Project

---

## 1. Executive Summary

This document proposes the design and implementation of a Hardware Abstraction Layer (HAL) for the Z80 Processor Validation Suite. The HAL will provide a clean, efficient interface between the test framework and the physical Z80 hardware, abstracting low-level bus operations, signal monitoring, and memory emulation.

### Key Design Principles
- **Performance First**: Use direct port manipulation and inline functions for sub-microsecond response times
- **Minimal Overhead**: All HAL functions designed to execute in 1-5 clock cycles
- **Safety**: Proper bus direction control to prevent hardware damage
- **Testability**: Built-in diagnostics and verification functions
- **Maintainability**: Clear separation between hardware interface and test logic

---

## 2. Architecture Overview

### 2.1 Module Structure

The HAL will consist of three primary components:

```
hal/
├── hal.h              # Main HAL header (public API)
├── hal.cpp            # HAL implementation
├── hal_diagnostics.h  # Diagnostic and test functions
└── hal_diagnostics.cpp
```

### 2.2 Layered Design

```
┌─────────────────────────────────────┐
│     Test Framework & Menu System    │
├─────────────────────────────────────┤
│     Hardware Abstraction Layer      │
│  ┌──────────┬──────────┬──────────┐ │
│  │   Bus    │ Control  │ Memory   │ │
│  │ Control  │ Signals  │Emulation │ │
│  └──────────┴──────────┴──────────┘ │
├─────────────────────────────────────┤
│  Arduino Hardware (Direct Port I/O) │
└─────────────────────────────────────┘
```

---

## 3. Core HAL Functions

### 3.1 Initialization & Configuration

#### `void initializeHAL()`
**Purpose:** Initialize all HAL components and configure pin directions  
**Implementation:**
- Configure address bus pins as inputs (Z80 drives these)
- Configure data bus as bidirectional (default to input)
- Configure control input pins (MREQ, IORQ, RD, WR, M1, RFSH, HALT, BUSAK)
- Configure control output pins (RESET, WAIT, INT, NMI, BUSRQ)
- Set all control outputs to inactive state (HIGH for active-low signals)
- Initialize CPU (Wait for several clock cycles after startup)
- Verify pin configuration

**Performance Target:** < 1ms for complete initialization

---

### 3.2 Bus Control Functions

#### `inline uint16_t readAddressBus()`
**Purpose:** Read current 16-bit address from Z80 address bus  
**Implementation:**
- Use direct port reads (PINC, PINK, etc.) to read all 16 address lines
- Combine into 16-bit value using bit shifting
- Return immediately without any processing delay

**Performance Target:** 2-4 clock cycles (125-250ns @ 16 MHz)

```cpp
inline uint16_t readAddressBus() {
  // Address bus is cleanly mapped to two complete ports:
  // A0-A7 on PORTC (pins 37-30)
  // A8-A15 on PORTK (pins A8-A15/62-69)
  
  // Read both ports directly (2 clock cycles total)
  uint8_t addr_low = PINC;   // Read A0-A7 from PORTC
  uint8_t addr_high = PINK;  // Read A8-A15 from PORTK
  
  // Combine into 16-bit address
  return ((uint16_t)addr_high << 8) | addr_low;
}
```

#### `inline uint8_t readDataBus()`
**Purpose:** Read current 8-bit value from Z80 data bus  
**Implementation:**
- Explicitly set data bus direction to input (faster than checking)
- Use direct port read to capture all 8 data lines simultaneously
- Return byte value

**Performance Target:** 2 clock cycles (125ns @ 16 MHz)

```cpp
inline uint8_t readDataBus() {
  // Data bus is on PORTA (pins 22-29, D0-D7 = PA0-PA7)
  // Always set direction - faster than checking (2 cycles total)
  DDRA = 0x00;  // Configure as input (1 cycle)
  return PINA;  // Read data bus (1 cycle)
}
```

#### `inline void writeDataBus(uint8_t data)`
**Purpose:** Write 8-bit value to Z80 data bus  
**Implementation:**
- Explicitly set data bus direction to output (faster than checking)
- Use direct port write to set all 8 data lines simultaneously
- Guaranteed safe operation with no bus contention

**Performance Target:** 2 clock cycles (125ns @ 16 MHz)

```cpp
inline void writeDataBus(uint8_t data) {
  // Data bus is on PORTA (pins 22-29, D0-D7 = PA0-PA7)
  // Always set direction - faster than checking (2 cycles total)
  DDRA = 0xFF;  // Configure as output (1 cycle)
  PORTA = data; // Write data bus (1 cycle)
}
```

#### `inline void setDataBusInput()`
**Purpose:** Configure data bus pins as inputs (high-impedance)  
**Implementation:**
- Set DDR register to 0 for all data bus pins
- This allows Z80 to drive the data bus

**Performance Target:** 1 clock cycle (62ns @ 16 MHz)

```cpp
inline void setDataBusInput() {
  DDRA = 0x00; // All pins as input
}
```

#### `inline void setDataBusOutput()`
**Purpose:** Configure data bus pins as outputs  
**Implementation:**
- Set DDR register to 0xFF for all data bus pins
- This allows Arduino to drive the data bus

**Performance Target:** 1 clock cycle (62ns @ 16 MHz)

```cpp
inline void setDataBusOutput() {
  DDRA = 0xFF; // All pins as output
}
```

---

### 3.3 Control Signal Monitoring

**HARDWARE UPDATE COMPLETE:** MREQ moved from D49 (PL0) to D2 (PE4/INT4) for external interrupt support.

**✅ Completed: November 7, 2025**

**Performance Note:** Seven Z80 control signals (IORQ, RD, WR, M1, RFSH, HALT, BUSAK) are mapped to PORTL (pins 48-42, PL1-PL7). MREQ is on PORTE for INT4 interrupt capability. When checking multiple signals simultaneously, it's significantly faster to read PINL once and test multiple bits rather than calling individual functions.

**Three approaches, from slowest to fastest:**

```cpp
// SLOW approach - multiple function calls (N reads + N compares)
bool iorq = isIORequest();       // reads PINL
bool rd = isReadCycle();         // reads PINL again
bool wr = isWriteCycle();        // reads PINL again
bool m1 = isM1Cycle();           // reads PINL again
// Total: ~12 clock cycles for 4 signals

// FAST approach - read once, test multiple bits with comparisons
uint8_t control = PINL;
bool iorq = (control & PORTL_IORQ_MASK) == 0;
bool rd = (control & PORTL_RD_MASK) == 0;
bool wr = (control & PORTL_WR_MASK) == 0;
bool m1 = (control & PORTL_M1_MASK) == 0;
// Total: ~5-6 clock cycles for 4 signals

// FASTEST approach - invert once, then direct bit tests (RECOMMENDED)
uint8_t control = ~PINL;  // Invert once (Z80 signals are active-low)
bool iorq = control & PORTL_IORQ_MASK;  // Just bit test, no comparison
bool rd = control & PORTL_RD_MASK;      // Just bit test
bool wr = control & PORTL_WR_MASK;      // Just bit test
bool m1 = control & PORTL_M1_MASK;      // Just bit test
// Total: ~3-4 clock cycles for 4 signals
// Breakdown: 1 read + 1 invert + 4 bit tests
```

**Recommendation:** 
- Use individual functions for clarity in non-critical code
- Use the FASTEST approach (with `~PINL`) in the critical memory handler for maximum performance

#### `inline bool isMemoryRequest()`
**Purpose:** Check if Z80 is requesting memory access  
**Implementation:**
- Read MREQ pin state (PE4 on PORTE, pin D2)
- Return true if MREQ is active (LOW)

**Performance Target:** 2-3 clock cycles

```cpp
inline bool isMemoryRequest() {
  return (PINE & PORTE_MREQ_MASK) == 0; // Active low on PORTE
}
```

#### `inline bool isIORequest()`
**Purpose:** Check if Z80 is requesting I/O access  
**Implementation:**
- Read IORQ pin state (PL1 on PORTL, pin 48)
- Return true if IORQ is active (LOW)

#### `inline bool isReadCycle()`
**Purpose:** Check if current bus cycle is a read operation  
**Implementation:**
- Read RD pin state
- Return true if RD is active (LOW)

#### `inline bool isWriteCycle()`
**Purpose:** Check if current bus cycle is a write operation  
**Implementation:**
- Read WR pin state
- Return true if WR is active (LOW)

#### `inline bool isM1Cycle()`
**Purpose:** Check if Z80 is in machine cycle 1 (opcode fetch)  
**Implementation:**
- Read M1 pin state
- Return true if M1 is active (LOW)

#### `inline bool isRefreshCycle()`
**Purpose:** Check if Z80 is performing DRAM refresh  
**Implementation:**
- Read RFSH pin state
- Return true if RFSH is active (LOW)

#### `inline bool isHalted()`
**Purpose:** Check if Z80 has executed HALT instruction  
**Implementation:**
- Read HALT pin state
- Return true if HALT is active (LOW)

---

### 3.4 Control Signal Generation

#### `void resetZ80(uint16_t durationMs = 10)`
**Purpose:** Perform hardware reset of Z80 processor  
**Implementation:**
- Assert RESET signal (drive LOW)
- Wait for specified duration (default 10ms)
- Release RESET signal (drive HIGH)
- Wait additional time for Z80 to stabilize
- Optional: Clear emulated memory and reset state

**Performance Target:** User-controlled delay

```cpp
void resetZ80(uint16_t durationMs = 10) {
  digitalWrite(Z80_RESET, RESET_ACTIVE);
  delay(durationMs);
  digitalWrite(Z80_RESET, RESET_INACTIVE);
  delay(durationMs); // Stabilization time
}
```

#### `inline void assertWait()`
**Purpose:** Hold Z80 in wait state  
**Implementation:**
- Drive WAIT signal LOW
- Used when Arduino needs more time to respond

#### `inline void releaseWait()`
**Purpose:** Allow Z80 to continue execution  
**Implementation:**
- Drive WAIT signal HIGH

#### `void triggerInterrupt(uint8_t vector = 0x00)`
**Purpose:** Generate maskable interrupt request  
**Implementation:**
- Assert INT signal
- Wait for Z80 to acknowledge (IORQ + M1)
- Place interrupt vector on data bus if in mode 0
- Release INT signal

#### `void triggerNMI()`
**Purpose:** Generate non-maskable interrupt  
**Implementation:**
- Pulse NMI signal (minimum 2 clock cycles)
- NMI is edge-triggered, not level-triggered

---

### 3.5 Memory Request Handler (Critical Path)

#### `void handleMemoryRequest()`
**Purpose:** Service Z80 memory read/write requests  
**Implementation Strategy:**

This is the **most critical** function in the entire HAL, as it must respond to Z80 bus cycles within approximately 500ns-1000ns (depending on clock frequency).

**HARDWARE UPDATE COMPLETE:** MREQ moved from D49 (PORTL) to D2 (PE4/INT4) for external interrupt support. This eliminates MREQ checking overhead and improves performance by 13%.

**✅ Completed: November 7, 2025**

**Implementation: Optimized Interrupt-Driven Approach**

The HAL uses External Interrupt INT4 on MREQ (pin D2/PE4) configured for falling edge trigger. The ISR is heavily optimized for minimum latency and maximum throughput.

**Ultra-Fast ISR Implementation:**

```cpp
// External Interrupt 4 Service Routine (MREQ falling edge)
// Attributes: hot (optimize for speed), flatten (inline everything)
ISR(INT4_vect) __attribute__((hot, flatten));

ISR(INT4_vect) {
  // ✅ NO MREQ CHECK - external interrupt guarantees MREQ is active!
  // This saves 3 cycles compared to pin change interrupt
  
  // Read control signals from PORTL (2 cycles)
  register uint8_t control = ~PINL;
  
  // Read 16-bit address from PORTC + PORTK (4 cycles)
  register uint16_t address = ((uint16_t)PINK << 8) | PINC;
  
  // Check operation type
  if (control & PORTL_RD_MASK) {
    // READ cycle - Arduino provides data to Z80
    DDRA = 0xFF;  // Set data bus as output (1 cycle)
    // Provide data directly from test program in PROGMEM
    PORTA = pgm_read_byte(&g_testProgram[address]);
    // Data held on bus until RD goes inactive
  }
  else if (control & PORTL_WR_MASK) {
    // WRITE cycle - Z80 sends data to Arduino
    DDRA = 0x00;  // Set data bus as input (1 cycle)
    g_capturedData = PINA;  // Capture data for verification (2 cycles)
    // Store address and data for test validation
    g_lastWriteAddress = address;
  }
  // Note: Data bus direction will be corrected on next cycle
}
```

**Key Optimizations Applied:**

1. **External Interrupt**: MREQ on INT4 (falling edge) - no state checking needed
2. **Register Variables**: Critical variables stored in CPU registers
3. **No MREQ Check**: Hardware edge trigger saves 3 cycles per interrupt
4. **Direct Port Access**: No abstraction layers (PORTA, PINA, DDRA, PINL, PINC, PINK)
5. **Single Address Read**: Read address bus once, use for both operations
6. **GCC Attributes**: `__attribute__((hot, flatten))` for aggressive optimization
7. **Batch Signal Read**: Read PINL once, test multiple bits from cached value
8. **Direct PROGMEM Access**: Test program stored in flash, accessed directly
9. **Minimal Branching**: Two-level if/else structure for predictable execution

**Performance Characteristics:**

| Operation | Cycle Count | Time @ 16 MHz |
|-----------|-------------|---------------|
| ISR Entry (hardware) | 4-6 cycles | 250-375ns |
| ISR Exit (hardware) | 4 cycles | 250ns |
| READ operation | 16-20 cycles | 1.0-1.25µs |
| WRITE operation | 12-15 cycles | 0.75-0.93µs |
| **Total READ latency** | **22-28 cycles** | **1.37-1.75µs** |
| **Total WRITE latency** | **18-23 cycles** | **1.12-1.43µs** |

**Performance Improvement:** 13% faster than pin change interrupt (3 cycles saved)

**Maximum Z80 Clock Speed:**
- With WAIT states: Up to 2.5 MHz
- Without WAIT states: Up to 1.5 MHz (conservative)
- Performance limited by ISR entry latency (~250ns)

**Setup Requirements:**

```cpp
void initializeMemoryHandler() {
  // Configure INT4 (PE4, pin D2) for falling edge trigger
  // EICRB controls INT4-INT7 (bits 0-1 for INT4)
  EICRB &= ~(1 << ISC40);   // Clear ISC40
  EICRB |=  (1 << ISC41);   // Set ISC41 -> falling edge
  
  // Enable INT4 in the external interrupt mask register
  EIMSK |= (1 << INT4);     // Enable INT4
  
  // Clear any pending INT4 interrupt flags
  EIFR |= (1 << INTF4);
  
  sei(); // Enable global interrupts
}

// Test program stored in PROGMEM (flash memory, not RAM)
const uint8_t g_testProgram[] PROGMEM = {
  0x00,  // NOP
  0x3E, 0x42,  // LD A, 0x42
  0xC9,  // RET
  // ... more test code
};

// Variables for capturing Z80 writes
volatile uint8_t g_capturedData = 0;
volatile uint16_t g_lastWriteAddress = 0;
```

**Additional Optimization Notes:**

- **PROGMEM access**: `pgm_read_byte()` adds ~4-6 cycles but saves precious SRAM
- **Alternative**: Small programs (<256 bytes) could be cached in RAM for 2-3 cycle access
- **Bus direction**: Setting DDRA on every operation is fast (1 cycle) and prevents bus contention
- **No error checking**: ISR assumes well-formed Z80 bus cycles for speed
- **Verification mode**: Write captures can be validated in main loop, not in ISR

---

## 4. Diagnostic & Test Functions

### 4.1 Bus Diagnostics

#### `bool verifyBusConnections()`
**Purpose:** Verify all bus connections are working  
**Implementation:**
- Test each pin can be read
- Check for shorts or opens
- Report detailed results

#### `void dumpBusState()`
**Purpose:** Display current state of all signals  
**Implementation:**
- Read and display address bus value
- Read and display data bus value
- Display all control signal states
- Useful for debugging

#### `void testDataBusDirection()`
**Purpose:** Verify bidirectional data bus operation  
**Implementation:**
- Switch between input and output modes
- Verify each mode functions correctly

---

### 4.2 Signal Timing Tests

#### `uint32_t measureBusResponseTime()`
**Purpose:** Measure Arduino's bus response latency  
**Implementation:**
- Simulate memory request
- Time how long Arduino takes to respond
- Report in nanoseconds

#### `void verifySetupHoldTimes()`
**Purpose:** Ensure Arduino meets Z80 timing requirements  
**Implementation:**
- Check data bus setup time before RD/WR assertion
- Check hold time after RD/WR deassertion
- Verify WAIT signal timing

---

## 5. Test Data Management

### 5.1 On-Demand Response Approach

**Design Philosophy:** Instead of emulating a full 64KB memory array, the HAL responds to individual read/write requests with appropriate test data. This eliminates the need for large memory buffers and allows test programs to be stored efficiently in Arduino flash (PROGMEM).

**Test Program Storage:**

```cpp
// Test programs stored in flash memory (PROGMEM), not SRAM
// Small programs (<256 bytes) can be cached in SRAM if needed
const uint8_t g_testProgram[] PROGMEM = {
  // Example: Simple NOP test
  0x00,        // NOP
  0x00,        // NOP
  0x76,        // HALT
  
  // Example: LD A, n test
  0x3E, 0x42,  // LD A, 0x42
  0x3E, 0xFF,  // LD A, 0xFF
  0x76,        // HALT
};

// Test program metadata
struct TestProgram {
  const uint8_t* data;  // Pointer to PROGMEM
  uint16_t length;      // Program length
  uint16_t startAddr;   // Load address (usually 0x0000)
};
```

**Read Request Handling:**

When Z80 requests a read (opcode fetch or data read), the HAL:
1. Receives address from Z80
2. Looks up corresponding byte from test program in PROGMEM
3. Returns data on bus

```cpp
// Called from ISR during READ cycle
inline uint8_t getTestData(uint16_t address) {
  // Direct PROGMEM access (4-6 cycles)
  if (address < sizeof(g_testProgram)) {
    return pgm_read_byte(&g_testProgram[address]);
  }
  return 0x00;  // Return NOP for unmapped addresses
}
```

**Write Request Handling:**

When Z80 performs a write, the HAL:
1. Captures address and data
2. Stores for verification (not in memory array)
3. Test framework validates the write was correct

```cpp
// Write capture structure
struct WriteCapture {
  uint16_t address;
  uint8_t data;
  uint32_t timestamp;  // For ordering verification
};

volatile WriteCapture g_writes[16];  // Circular buffer
volatile uint8_t g_writeIndex = 0;

// Called from ISR during WRITE cycle  
inline void captureWrite(uint16_t address, uint8_t data) {
  g_writes[g_writeIndex].address = address;
  g_writes[g_writeIndex].data = data;
  g_writeIndex = (g_writeIndex + 1) & 0x0F;  // Wrap at 16
}
```

### 5.2 Arduino SRAM Usage

The Arduino Mega 2560 has 8KB of SRAM. With on-demand response approach:

```
Total SRAM: 8192 bytes

Allocations:
- Stack (Arduino runtime):     ~512 bytes
- Global variables:            ~256 bytes
- Menu system buffers:         ~512 bytes
- Test result buffers:        ~1024 bytes
- HAL data structures:         ~128 bytes
- Write capture buffer:        ~512 bytes (16 entries × 32 bytes)
- Small program cache:         ~256 bytes (optional)
----------------------------------------
- Remaining for tests:       ~5000 bytes
```

**Benefits of On-Demand Approach:**

1. **No large memory arrays**: Test programs in PROGMEM (flash), not SRAM
2. **Scalable**: Can support any test program size (limited only by flash)
3. **Simple**: No memory management overhead
4. **Fast**: Direct PROGMEM read is only 4-6 cycles
5. **Flexible**: Different tests can have different data sources

**Test Program Loading:**

```cpp
// Set active test program
void setTestProgram(const uint8_t* progmem_ptr, uint16_t length) {
  g_testProgram = progmem_ptr;
  g_testProgramLength = length;
  g_writeIndex = 0;  // Reset write capture
}

// Example: Load instruction test
const uint8_t LD_A_Test[] PROGMEM = {
  0x3E, 0x42,  // LD A, 0x42
  0x76         // HALT
};

setTestProgram(LD_A_Test, sizeof(LD_A_Test));
```

**Optional: Small Program Cache**

For performance-critical tests, small programs (<256 bytes) can be cached in SRAM:

```cpp
uint8_t g_programCache[256];  // Fast SRAM cache
bool g_useCaching = false;

void loadProgramToCache(const uint8_t* progmem_ptr, uint16_t length) {
  for (uint16_t i = 0; i < length && i < 256; i++) {
    g_programCache[i] = pgm_read_byte(&progmem_ptr[i]);
  }
  g_useCaching = true;
}

inline uint8_t getTestData(uint16_t address) {
  if (g_useCaching && address < 256) {
    return g_programCache[address];  // 2-3 cycles vs 4-6 for PROGMEM
  }
  return pgm_read_byte(&g_testProgram[address]);
}
```

---

## 6. API Summary

### Initialization
- `void initializeHAL()`

### Bus Control
- `uint16_t readAddressBus()`
- `uint8_t readDataBus()`
- `void writeDataBus(uint8_t data)`
- `void setDataBusInput()`
- `void setDataBusOutput()`

### Control Signal Monitoring
- `bool isMemoryRequest()`
- `bool isIORequest()`
- `bool isReadCycle()`
- `bool isWriteCycle()`
- `bool isM1Cycle()`
- `bool isRefreshCycle()`
- `bool isHalted()`

### Control Signal Generation
- `void resetZ80(uint16_t durationMs = 10)`
- `void assertWait()`
- `void releaseWait()`
- `void triggerInterrupt(uint8_t vector = 0x00)`
- `void triggerNMI()`

### Memory Emulation
- `void initializeMemory()`
- `void loadProgram(uint16_t startAddr, const uint8_t* program, uint16_t length)`
- `uint8_t readMemory(uint16_t address)`
- `void writeMemory(uint16_t address, uint8_t data)`
- `void clearMemory()`
- `void handleMemoryRequest()` *(Critical path function)*

### Diagnostics
- `bool verifyBusConnections()`
- `void dumpBusState()`
- `void testDataBusDirection()`
- `bool testMemoryArray()`
- `void displayMemoryMap()`
- `uint32_t measureBusResponseTime()`
- `void verifySetupHoldTimes()`

---

## 7. Implementation Plan

### Phase 1: Basic HAL Infrastructure (Week 1)
1. Create `hal.h` and `hal.cpp` files
2. Implement initialization functions
3. Implement basic bus read/write functions
4. Implement control signal monitoring functions
5. Add comprehensive comments and documentation

### Phase 2: Memory Emulation (Week 2)
1. Allocate 2KB memory array
2. Implement memory read/write functions
3. Implement program loading
4. Implement polling-based memory request handler
5. Test with simple Z80 programs (NOP sequences)

### Phase 3: Control Signals (Week 3)
1. Implement reset function
2. Implement WAIT signal control
3. Implement interrupt generation
4. Test each control signal independently

### Phase 4: Diagnostics (Week 4)
1. Implement bus verification tests
2. Implement timing measurement functions
3. Add diagnostic menu to main system
4. Performance optimization and profiling

### Phase 5: Advanced Features (Week 5+)
1. Implement interrupt-driven memory handler (if needed)
2. Expand memory emulation if SRAM available
3. Add memory access tracking and logging
4. Optimize critical path for higher clock speeds

---

## 8. Testing Strategy

### Unit Tests
Each HAL function will have dedicated test:
- `testReadAddressBus()` - Verify address capture
- `testDataBusReadWrite()` - Verify data bus operations
- `testControlSignals()` - Verify all control signals
- `testMemoryEmulation()` - Verify memory functions

### Integration Tests
- Run simple Z80 programs (NOP loops, simple math)
- Verify instruction fetch cycle
- Verify memory read/write cycles
- Verify interrupt handling

### Performance Tests
- Measure response time for each function
- Verify timing meets Z80 requirements
- Test at various clock frequencies
- Profile memory request handler

### Hardware Tests
- Verify with logic analyzer
- Check signal integrity with oscilloscope
- Measure actual timing delays
- Verify no bus contention issues

---

## 9. Risk Assessment

### High Risk Items
1. **Memory Emulation Speed**: Arduino may not respond fast enough at higher clock speeds
   - **Mitigation**: Start at low frequency (100-500 KHz), optimize code, use WAIT signal if needed

2. **SRAM Limitations**: 64KB not possible without external hardware
   - **Mitigation**: Use 2KB window, sufficient for all tests

3. **Bus Contention**: Improper direction control could damage hardware
   - **Mitigation**: Always default to input, careful state management

### Medium Risk Items
1. **Interrupt Latency**: ISR may not be fast enough
   - **Mitigation**: Use polling for initial implementation

2. **Timing Variations**: Arduino clock crystal tolerance
   - **Mitigation**: Use adjustable timing, test with oscilloscope

### Low Risk Items
1. **Code Complexity**: HAL could become difficult to maintain
   - **Mitigation**: Excellent documentation, clear API design

---

## 10. Performance Specifications

### Target Performance Metrics
- **Bus Read Response**: < 250ns (4 clock cycles @ 16 MHz)
- **Bus Write Response**: < 500ns (8 clock cycles @ 16 MHz)
- **Memory Request Handler**: < 1000ns total latency
- **Control Signal Changes**: < 125ns (2 clock cycles)

### Clock Frequency Support
- **Minimum**: 100 KHz (proven achievable)
- **Initial Target**: 500 KHz (safe, allows debugging)
- **Stretch Goal**: 2-4 MHz (optimized code, may require WAIT states)
- **Z80 Maximum**: 8 MHz (unlikely achievable with Arduino)

---

## 11. Open Questions

1. **Should we implement external SRAM now or later?**
   - Recommendation: Later, only if 2KB proves insufficient

2. **Polling vs. Interrupt-driven memory handler?**
   - Recommendation: Polling initially, interrupt if needed

3. **How much diagnostics vs. performance optimization?**
   - Recommendation: Diagnostics first, optimize after tests prove functionality

4. **Should HAL track memory access history?**
   - Recommendation: Optional feature, enabled by compile-time flag

5. **How to handle I/O requests (IORQ)?**
   - Recommendation: Phase 2 feature, not needed for initial instruction tests

---

## 12. Success Criteria

The HAL will be considered complete and successful when:

1. ✅ All API functions implemented and tested
2. ✅ Z80 can fetch and execute NOP instructions
3. ✅ Z80 can perform memory reads and writes
4. ✅ Reset function properly initializes Z80
5. ✅ Bus diagnostics pass all tests
6. ✅ Can run at 500 KHz stably for extended periods
7. ✅ No bus contention or hardware damage
8. ✅ Code is well-documented and maintainable
9. ✅ Integration with test framework is clean
10. ✅ Performance metrics are met

---

## 13. Conclusion

This HAL design provides a solid foundation for the Z80 validation suite. The architecture prioritizes:
- **Safety**: Proper bus direction control and signal management
- **Performance**: Direct port I/O and inline functions
- **Pragmatism**: 2KB memory window instead of unrealistic 64KB
- **Testability**: Comprehensive diagnostics and verification

The phased implementation plan allows for incremental development and testing, reducing risk and allowing for course correction as needed.

**Estimated Timeline:** 4-5 weeks for full implementation and testing

**Recommendation:** Approve this design and proceed with Phase 1 implementation.

---

## Appendices

### Appendix A: Pin Mapping Reference
See `pins.h` for complete pin definitions.

### Appendix B: Z80 Timing Requirements
- Address Setup Time: 30ns before MREQ/IORQ
- Data Setup Time: 60ns before WR inactive
- Data Hold Time: 0ns after WR inactive
- Read Data Access Time: Max 250ns after RD active

### Appendix C: Arduino Performance Characteristics
- Clock Speed: 16 MHz (62.5ns per cycle)
- Digital I/O: ~4 clock cycles (250ns) via digitalWrite()
- Direct Port I/O: 1-2 clock cycles (62-125ns)
- ISR Entry Latency: 40-80 clock cycles (2.5-5µs)

---

**END OF PROPOSAL**
