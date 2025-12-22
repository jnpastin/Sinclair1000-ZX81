/*
 * Z80 Processor Validation Suite - Hardware Abstraction Layer (HAL)
 * 
 * This module provides a clean, efficient interface between the test framework
 * and the physical Z80 hardware. It abstracts low-level bus operations, signal
 * monitoring, and memory emulation.
 * 
 * Design Principles:
 * - Performance First: Direct port manipulation and inline functions
 * - Minimal Overhead: All functions execute in 1-5 clock cycles
 * - Safety: Proper bus direction control to prevent hardware damage
 * - Testability: Built-in diagnostics and verification
 * 
 * Hardware: Arduino Mega 2560 + Z80 CPU
 * Date: November 6, 2025
 */

#ifndef HAL_H
#define HAL_H

#include <Arduino.h>
#include "pins.h"

// =============================================================================
// INITIALIZATION & CONFIGURATION
// =============================================================================

/**
 * Initialize all HAL components and configure hardware
 * - Configure address bus pins as inputs (Z80 drives these)
 * - Configure data bus as bidirectional (default to input)
 * - Configure control input pins (MREQ, IORQ, RD, WR, M1, RFSH, HALT, BUSAK)
 * - Configure control output pins (RESET, WAIT, INT, NMI, BUSRQ)
 * - Set all control outputs to inactive state (HIGH for active-low signals)
 * - Verify pin configuration
 * 
 * Performance: < 1ms for complete initialization
 */
void initializeHAL();

/**
 * Initialize External Interrupt 4 (INT4) for MREQ signal
 * Configures falling edge trigger for Z80 memory requests
 * 
 * This function:
 * - Ensures MREQ pin (D2/PE4) is configured as input
 * - Configures INT4 for falling edge trigger (MREQ going active)
 * - Enables INT4 in interrupt mask register
 * - Clears any pending interrupt flags
 * - Enables global interrupts (sei)
 * 
 * CRITICAL: Must be called AFTER initializeHAL()
 * CRITICAL: ISR must be implemented before calling this function
 * 
 * Performance: < 1ms
 */
void initializeMemoryHandler();

// =============================================================================
// BUS CONTROL FUNCTIONS
// =============================================================================

/**
 * Read current 16-bit address from Z80 address bus
 * Uses direct port reads (PINC, PINK) for maximum speed
 * 
 * @return 16-bit address value (A0-A15)
 * 
 * Performance: 2-4 clock cycles (125-250ns @ 16 MHz)
 */
inline uint16_t readAddressBus() {
  // A0-A7 on PORTC, A8-A15 on PORTK
  uint8_t addr_low = PINC;   // Read A0-A7
  uint8_t addr_high = PINK;  // Read A8-A15
  return ((uint16_t)addr_high << 8) | addr_low;
}

/**
 * Read current 8-bit value from Z80 data bus
 * Ensures data bus is configured as input before reading
 * 
 * @return 8-bit data value (D0-D7)
 * 
 * Performance: 2 clock cycles (125ns @ 16 MHz)
 */
inline uint8_t readDataBus() {
  // Data bus is on PORTA (pins 22-29, D0-D7 = PA0-PA7)
  DDRA = 0x00;  // Configure as input (1 cycle)
  return PINA;  // Read data bus (1 cycle)
}

/**
 * Write 8-bit value to Z80 data bus
 * Configures data bus as output and writes value
 * 
 * @param data 8-bit value to write to data bus
 * 
 * Performance: 2 clock cycles (125ns @ 16 MHz)
 */
inline void writeDataBus(uint8_t data) {
  // Data bus is on PORTA (pins 22-29, D0-D7 = PA0-PA7)
  DDRA = 0xFF;  // Configure as output (1 cycle)
  PORTA = data; // Write data bus (1 cycle)
}

// =============================================================================
// CONTROL SIGNAL MONITORING
// =============================================================================
// All Z80 control signals are on PORTL and are ACTIVE LOW
// For maximum performance when checking multiple signals, read PINL once
// and test multiple bits from the cached value

/**
 * Read all control signals at once (OPTIMIZED for speed)
 * This is the FASTEST way to check multiple signals simultaneously
 * 
 * Returns inverted PORTL value where '1' = signal active, '0' = signal inactive
 * Use with PORTL_xxx_MASK constants for bit testing
 * 
 * NOTE: MREQ is NOT in PORTL - it's on PORTE (pin D2/PE4/INT4)
 *       Use isMemoryRequest() to check MREQ status
 * 
 * Example usage:
 *   uint8_t signals = readControlSignals();
 *   bool iorq = signals & PORTL_IORQ_MASK;  // Just a bit test
 *   bool rd = signals & PORTL_RD_MASK;
 *   bool wr = signals & PORTL_WR_MASK;
 *   bool m1 = signals & PORTL_M1_MASK;
 * 
 * Performance: ~2 clock cycles (1 read + 1 invert)
 *              vs ~12 cycles for 4 separate function calls
 * 
 * @return Inverted control signals (1 = active, 0 = inactive) - 7 bits, no MREQ
 */
inline uint8_t readControlSignals() {
  return ~PINL;  // Invert once - signals are active low
}

/**
 * Check if Z80 is requesting memory access
 * 
 * CRITICAL: MREQ on pin D2 (PE4/INT4), not PORTL
 * 
 * @return true if MREQ is active (LOW)
 * 
 * Performance: 2-3 clock cycles
 */
inline bool isMemoryRequest() {
  return (PINL & PORTL_MREQ_MASK) == 0; // Active low on PORTL
}

/**
 * Check if Z80 is requesting I/O access
 * 
 * @return true if IORQ is active (LOW)
 * 
 * Performance: 2-3 clock cycles
 */
inline bool isIORequest() {
  return (PINL & PORTL_IORQ_MASK) == 0; // Active low
}

/**
 * Check if current bus cycle is a read operation
 * 
 * @return true if RD is active (LOW)
 * 
 * Performance: 2-3 clock cycles
 */
inline bool isReadCycle() {
  return (PINE & PORTE_RD_MASK) == 0; // Active low
}

/**
 * Check if current bus cycle is a write operation
 * 
 * @return true if WR is active (LOW)
 * 
 * Performance: 2-3 clock cycles
 */
inline bool isWriteCycle() {
  return (PINE & PORTE_WR_MASK) == 0; // Active low
}

/**
 * Check if Z80 is in machine cycle 1 (opcode fetch)
 * 
 * @return true if M1 is active (LOW)
 * 
 * Performance: 2-3 clock cycles
 */
inline bool isM1Cycle() {
  return (PINL & PORTL_M1_MASK) == 0; // Active low
}

/**
 * Check if Z80 is performing DRAM refresh
 * 
 * @return true if RFSH is active (LOW)
 * 
 * Performance: 2-3 clock cycles
 */
inline bool isRefreshCycle() {
  return (PINL & PORTL_RFSH_MASK) == 0; // Active low
}

/**
 * Check if Z80 has executed HALT instruction
 * 
 * @return true if HALT is active (LOW)
 * 
 * Performance: 2-3 clock cycles
 */
inline bool isHalted() {
  return (PINL & PORTL_HALT_MASK) == 0; // Active low
}

/**
 * Check if Z80 has acknowledged bus request
 * 
 * @return true if BUSAK is active (LOW)
 * 
 * Performance: 2-3 clock cycles
 */
inline bool isBusAcknowledged() {
  return (PINL & PORTL_BUSAK_MASK) == 0; // Active low
}

// =============================================================================
// CONTROL SIGNAL GENERATION
// =============================================================================

/**
 * Perform hardware reset of Z80 processor
 * 
 * @param durationMs Reset pulse duration in milliseconds (default 10ms)
 * 
 * Implementation:
 * - Assert RESET signal (drive LOW)
 * - Wait for specified duration
 * - Release RESET signal (drive HIGH)
 * - Wait additional time for Z80 to stabilize
 */
void resetZ80(uint16_t durationMs = 10);

/**
 * Reset ISR state for new test program
 * 
 * Clears all ISR counters and resets program base address capture.
 * Call this before each test to ensure clean state.
 * 
 * CRITICAL: Call AFTER loading program and resetting Z80, 
 *           BEFORE releasing WAIT to start execution
 * 
 * Performance: < 100 clock cycles (interrupts disabled briefly)
 */
inline void resetISRState() {
  noInterrupts();
  extern volatile uint32_t g_isrCallCount;
  extern volatile uint32_t g_isrErrorCount;
  extern volatile uint32_t g_isrReadCount;
  extern volatile uint32_t g_isrRfshCount;
  extern volatile uint16_t g_programBaseAddress;
  extern volatile bool g_programBaseAddressValid;
  extern volatile uint16_t g_lastReadAddress;
  extern volatile uint16_t g_lastWriteAddress;
  extern volatile uint8_t g_capturedData;
  extern volatile bool g_dataReady;
  extern volatile uint32_t g_readIsrCount;
  extern volatile uint32_t g_writeIsrCount;
  extern volatile uint32_t g_isrTriggerCount;
  extern volatile uint32_t g_writePatternAA_count;
  extern volatile uint32_t g_writePattern55_count;
  
  g_isrCallCount = 0;
  g_isrErrorCount = 0;
  g_isrReadCount = 0;
  g_isrRfshCount = 0;
  g_programBaseAddress = 0;
  g_programBaseAddressValid = false;  // Allow ISR to capture new base address
  g_lastReadAddress = 0;
  g_lastWriteAddress = 0;
  g_capturedData = 0;
  g_dataReady = false;
  g_readIsrCount = 0;
  g_writeIsrCount = 0;
  g_writePatternAA_count = 0;
  g_writePattern55_count = 0;
  g_isrTriggerCount = 0;
  interrupts();
}

/**
 * Hold Z80 in wait state
 * Used when Arduino needs more time to respond
 * 
 * Performance: 1-2 clock cycles
 */
inline void assertWait() {
  PORTB &= ~PORTB_WAIT_MASK; // Drive WAIT LOW (active)
}

/**
 * Allow Z80 to continue execution
 * Release wait state
 * 
 * Performance: 1-2 clock cycles
 */
inline void releaseWait() {
  PORTB |= PORTB_WAIT_MASK; // Drive WAIT HIGH (inactive)
}

/**
 * Generate maskable interrupt request
 * 
 * @param vector Interrupt vector (for mode 0, default 0x00)
 * 
 * Implementation:
 * - Assert INT signal
 * - Wait for Z80 to acknowledge (IORQ + M1)
 * - Place interrupt vector on data bus if in mode 0
 * - Release INT signal
 */
void triggerInterrupt(uint8_t vector = 0x00);

/**
 * Generate non-maskable interrupt
 * 
 * Implementation:
 * - Pulse NMI signal (minimum 2 clock cycles)
 * - NMI is edge-triggered, not level-triggered
 */
void triggerNMI();

/**
 * Request control of the bus
 * Assert BUSRQ signal
 * 
 * Performance: 1-2 clock cycles
 */
inline void assertBusRequest() {
  PORTB &= ~PORTB_BUSRQ_MASK; // Drive BUSRQ LOW (active)
}

/**
 * Release bus request
 * 
 * Performance: 1-2 clock cycles
 */
inline void releaseBusRequest() {
  PORTB |= PORTB_BUSRQ_MASK; // Drive BUSRQ HIGH (inactive)
}


// =============================================================================
// TEST PROGRAM MANAGEMENT
// =============================================================================

/**
 * Set active test program (PROGMEM only, no caching)
 * 
 * Configures the ISR to serve data from the specified test program
 * stored in PROGMEM (flash memory). Use this for large programs (>256 bytes)
 * or when SRAM is constrained.
 * 
 * @param progmem_ptr Pointer to test program in PROGMEM (flash memory)
 * @param length Program length in bytes
 * 
 * Note: This uses PROGMEM access in the ISR (4-6 cycles per read)
 */
void setTestProgram(const uint8_t* progmem_ptr, uint16_t length);

/**
 * Load test program into SRAM cache for fast access
 * 
 * Copies a small program from PROGMEM into SRAM cache for fastest ISR
 * performance. Recommended for most instruction tests (<256 bytes).
 * If program is >256 bytes, automatically falls back to PROGMEM.
 * 
 * @param progmem_ptr Pointer to test program in PROGMEM (flash memory)
 * @param length Program length in bytes (max 256 for caching)
 * @return true if successfully cached in SRAM, false if using PROGMEM fallback
 * 
 * Performance:
 * - Cached (SRAM): 2-3 cycles per ISR read
 * - Fallback (PROGMEM): 4-6 cycles per ISR read
 * 
 * Note: Call BEFORE starting Z80 clock for best results
 */
bool loadProgramToCache(const uint8_t* progmem_ptr, uint16_t length);

/**
 * Set maximum cache size for program storage
 * 
 * Configures the maximum size of the SRAM cache used for program storage.
 * Larger cache sizes allow caching bigger programs but use more SRAM.
 * 
 * @param maxSize Maximum cache size in bytes (128-1024, must be power of 2)
 * @return true if size is valid and set, false otherwise
 * 
 * Valid sizes: 128, 256, 512, 1024 bytes
 * Default: 256 bytes
 * 
 * Note: Existing cached programs are cleared when cache size changes
 */
bool setCacheSize(uint16_t maxSize);

/**
 * Get current maximum cache size
 * 
 * @return Current maximum cache size in bytes
 */
uint16_t getCacheSize();

/**
 * Get current SRAM usage for cache
 * 
 * @return Bytes of SRAM currently used by cache (0 if not caching)
 */
uint16_t getCacheUsage();

// =============================================================================
// TESTING & DIAGNOSTICS
// =============================================================================

/**
 * Run basic HAL self-test
 * 
 * Tests all HAL functions to verify correct operation:
 * - Bus control functions (read/write data bus)
 * - Control signal monitoring
 * - Control signal generation (WAIT, INT, NMI, BUSRQ)
 * - Memory management functions
 * - Z80 reset
 * 
 * Prints detailed results to Serial.
 * Safe to run - does not require Z80 to be connected.
 * 
 * @return true if all tests pass, false if any failures
 */
bool testHAL();

// =============================================================================
// RING BUFFER & HYBRID BACKPRESSURE CONFIGURATION
// =============================================================================

// Ring buffer configuration
#define CAPTURE_BUFFER_SIZE 16      // Power of 2 for efficiency (128 bytes total)
#define BUFFER_HIGH_WATER   12      // 75% full triggers backpressure
#define BUFFER_LOW_WATER    4       // 25% full releases backpressure

// Hybrid backpressure configuration
#define SUSTAINED_WAIT_TIME    10000    // Switch to throttle after 10ms (µs)
#define THROTTLE_FACTOR        2        // Halve clock speed when throttling
#define MIN_CLOCK_SPEED        100000   // Minimum clock: 100 KHz (absolute floor)

// Capture entry structure (8 bytes per entry)
struct CaptureEntry {
    uint16_t address;               // Z80 address (2 bytes)
    uint8_t  data;                  // Data byte (1 byte)
    uint8_t  operation;             // READ=0, WRITE=1 (1 byte)
    uint32_t timestamp;             // micros() for timing analysis (4 bytes)
};

// Capture modes
enum CaptureMode {
    CAPTURE_OFF = 0,          // No capture (default, fastest)
    CAPTURE_BUFFERED = 1,     // Ring buffer with hybrid backpressure
    CAPTURE_IMMEDIATE = 2     // Pre-assert WAIT, single operation (debug)
};

// =============================================================================
// RING BUFFER & HYBRID BACKPRESSURE MANAGEMENT FUNCTIONS
// =============================================================================

/**
 * Set capture mode for Z80 operations
 * 
 * @param mode Capture mode:
 *             CAPTURE_OFF      = No capture (default, fastest)
 *             CAPTURE_BUFFERED = Ring buffer with hybrid backpressure
 *             CAPTURE_IMMEDIATE = Pre-assert WAIT, single operation (debug)
 */
void setCaptureMode(CaptureMode mode);

/**
 * Get current ring buffer usage level
 * 
 * @return Number of entries currently in buffer (0-CAPTURE_BUFFER_SIZE)
 * 
 * Performance: 3-5 clock cycles
 */
uint8_t getCaptureBufferLevel();

/**
 * Process captured data from ring buffer
 * 
 * Call this frequently (e.g., in main loop or test function).
 * Consumes entries from buffer and automatically releases WAIT
 * when buffer drains below LOW_WATER mark.
 * 
 * For each entry, calls validation functions or logs data.
 * Returns when buffer is empty or after processing all entries.
 */
void processCaptureBuffer();

/**
 * Hybrid backpressure management
 * 
 * Call from main loop periodically to manage WAIT states and clock throttling.
 * 
 * Behavior:
 * - When buffer ≥75% full: Asserts WAIT immediately
 * - If WAIT sustained >10ms: Switches to clock throttling
 * - When buffer ≤25% full: Releases WAIT/throttle, restores speed
 * 
 * This provides fast burst handling with WAIT, and sustained operation
 * via clock throttling for long processing delays.
 */
void manageHybridBackpressure();

/**
 * Clear ring buffer and reset capture state
 * 
 * Resets head and tail pointers to empty the buffer.
 * Does NOT change capture mode or backpressure settings.
 */
void clearCaptureBuffer();

/**
 * Read one entry from ring buffer
 * 
 * @param entry Pointer to CaptureEntry structure to fill
 * @return true if entry read successfully, false if buffer empty
 * 
 * Note: This consumes the entry from the buffer (moves tail pointer)
 */
bool readCaptureBuffer(CaptureEntry* entry);

// =============================================================================
// WRITE CAPTURE & VALIDATION FUNCTIONS
// =============================================================================

/**
 * Get Captured Write Data (Simple Capture Mode)
 * 
 * Retrieves the last write operation captured from Z80 in CAPTURE_OFF mode.
 * Only captures the most recent write - previous writes are overwritten.
 * 
 * @param index Write index (must be 0 for single-write capture)
 * @param address Output parameter for write address (can be nullptr)
 * @param data Output parameter for written data (can be nullptr)
 * @return true if valid data available, false otherwise
 * 
 * Note: For multiple write capture, use CAPTURE_BUFFERED mode and readCaptureBuffer()
 */
bool getCapturedWrite(uint8_t index, uint16_t* address, uint8_t* data);

/**
 * Get Number of Captured Writes (Simple Capture Mode)
 * 
 * Returns the number of write operations available in simple capture.
 * For single-write capture, this is always 0 or 1.
 * 
 * @return 1 if a write has been captured, 0 otherwise
 * 
 * Note: For ring buffer mode, use getCaptureBufferLevel() instead
 */
uint8_t getCapturedWriteCount();

/**
 * Process and Display Captured Writes
 * 
 * Checks for captured write operations and displays them to Serial.
 * This is a convenience function for debugging and validation.
 * 
 * For simple capture mode (CAPTURE_OFF):
 * - Checks g_dataReady flag
 * - Displays address and data in hex format
 * - Clears g_dataReady flag after display
 * 
 * For ring buffer mode (CAPTURE_BUFFERED):
 * - Reads all entries from capture buffer
 * - Displays each operation with timestamp
 * 
 * NOTE: Addresses displayed are ABSOLUTE bus addresses, not program-relative.
 *       The base address offset (g_programBaseAddress) only applies to READ
 *       operations in program memory space. WRITE operations use absolute addressing.
 * 
 * Usage: Call after running a test program to see what was written
 */
void processCaptures();

/**
 * Validate Expected Write Operation
 * 
 * Checks if a write operation with specific address and data was captured.
 * Returns true if the write matches expectations, false otherwise.
 * 
 * For simple capture mode (CAPTURE_OFF):
 * - Checks g_dataReady flag
 * - Compares g_lastWriteAddress and g_capturedData with expected values
 * - Clears g_dataReady flag after validation
 * 
 * For ring buffer mode (CAPTURE_BUFFERED):
 * - Searches buffer for matching entry
 * - Does NOT clear the buffer (allows multiple validations)
 * 
 * NOTE: Address Offset Handling
 * ==============================
 * This function uses ABSOLUTE addresses for validation, NOT program-relative offsets.
 * The g_programBaseAddress offset ONLY applies to READ operations in program memory
 * space (0x0000-0x00FF). WRITE operations target high memory (0x8000+) using
 * absolute addressing, so no offset is applied.
 * 
 * Displays detailed pass/fail messages to Serial.
 * 
 * @param expectedAddr Expected memory address (ABSOLUTE, not relative)
 * @param expectedData Expected data value
 * @return true if write matches, false otherwise
 * 
 * Usage:
 *   if (validateWrite(0x8000, 0x42)) {
 *     Serial.println("Test passed!");
 *   }
 */
bool validateWrite(uint16_t expectedAddr, uint8_t expectedData);

// =============================================================================
// GLOBAL VARIABLES (defined in hal.cpp)
// =============================================================================
// These are accessed by the ISR and main code

// Program base address tracking (volatile - updated by ISR)
// Z80 may not start execution at 0x0000 after reset; ISR captures actual start address
extern volatile uint16_t g_programBaseAddress;      // First instruction fetch address
extern volatile bool g_programBaseAddressValid;     // True after first READ captured

// Test program storage (in PROGMEM - flash memory)
extern const uint8_t* g_testProgram;      // Pointer to current test program
extern uint16_t g_testProgramLength;      // Length of test program
extern volatile bool g_programLoaded;     // True when a test program is loaded

// SRAM cache for fast access (optional, but recommended)
extern uint8_t g_programCache[1024];       // Fast SRAM cache for small programs
extern bool g_useCaching;                 // True if using SRAM cache
extern uint16_t g_cacheSize;              // Size of cached program in bytes

// Write capture (volatile - accessed by ISR and main loop)
extern volatile uint8_t g_capturedData;   // Last data written by Z80
extern volatile uint16_t g_lastWriteAddress; // Last write address
extern volatile bool g_dataReady;         // True when write data is available

// ISR debug counters
extern volatile uint32_t g_isrReadCount;  // Number of READ operations
extern volatile uint32_t g_isrRfshCount;  // Number of REFRESH cycles
extern volatile uint32_t g_isrCallCount;  // Number of times ISR has been called
extern volatile uint32_t g_isrErrorCount; // Number of ISR errors (out-of-bounds, etc.)
extern volatile uint16_t g_lastReadAddress; // Last read address
extern volatile uint32_t g_readIsrCount;    // Number of read ISR calls
extern volatile uint32_t g_writeIsrCount;   // Number of write ISR calls
extern volatile uint32_t g_isrTriggerCount; // Number of ISR triggers (before checks)
extern volatile uint32_t g_writePatternAA_count; // Number of writes with data 0xAA
extern volatile uint32_t g_writePattern55_count; // Number of writes with data 0x55

// Ring buffer for capturing Z80 operations (volatile - accessed by ISR)
extern volatile CaptureEntry g_captureBuffer[CAPTURE_BUFFER_SIZE];
extern volatile uint8_t g_captureHead;      // ISR writes here (producer)
extern volatile uint8_t g_captureTail;      // App reads here (consumer)

// Capture control (volatile - accessed by ISR)
extern volatile CaptureMode g_captureMode;  // Current capture mode
extern volatile bool g_captureEnabled;      // True when capture is active

// Hybrid backpressure state tracking (volatile - accessed by ISR and main loop)
extern volatile bool g_waitAsserted;        // True when WAIT is active
extern volatile bool g_clockThrottled;      // True when clock is throttled
extern volatile uint32_t g_waitStartTime;   // micros() when WAIT was asserted
extern volatile uint32_t g_lowWaterTime;    // micros() when buffer dropped below low water

// Clock speed tracking (for throttling)
extern uint32_t g_targetClock;              // Desired clock speed (Hz)
extern uint32_t g_currentClock;             // Current clock speed (Hz)

// =============================================================================
// HARDWARE TEST FUNCTIONS
// =============================================================================

/**
 * Run Hardware Test Suite
 * 
 * Tests HAL functions with actual hardware:
 * - Pin configuration verification
 * - Bus read/write operations
 * - Control signal monitoring
 * - Control signal generation (RESET, WAIT, INT, NMI)
 * - Data bus direction control
 * 
 * Prints detailed test results to serial console.
 */
void runHALTest();


#endif // HAL_H
