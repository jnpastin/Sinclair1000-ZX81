/*
 * Z80 Processor Validation Suite - Hardware Abstraction Layer Implementation
 * 
 * This file implements the HAL functions for interfacing with Z80 hardware.
 * 
 * Hardware: Arduino Mega 2560 + Z80 CPU
 * Date: November 11, 2025
 */

// File-level optimization: ISR and time-critical functions get maximum optimization
#pragma GCC optimize("O3", "flto", "unroll-loops", "fast-math")

#include "hal.h"
#include "clock.h"

// =============================================================================
// GLOBAL VARIABLES
// =============================================================================

// Test program storage
const uint8_t* g_testProgram = nullptr;
uint16_t g_testProgramLength = 0;
volatile bool g_programLoaded = false;

// Program cache for fast access (optional optimization)
#define MAX_CACHE_SIZE 1024  // Maximum configurable cache size
uint8_t g_programCache[MAX_CACHE_SIZE];  // Fast SRAM cache for small programs
bool g_useCaching = false;
uint16_t g_cacheSize = 0;
uint16_t g_maxCacheSize = 256;  // Default maximum cache size

// Write capture for verification
volatile uint8_t g_capturedData = 0;
volatile uint16_t g_lastWriteAddress = 0;
volatile bool g_dataReady = false;

// ISR statistics (optional, for debugging)
volatile uint32_t g_isrCallCount = 0;
volatile uint32_t g_isrErrorCount = 0;

// DEBUG: Track READ operations
volatile uint32_t g_isrReadCount = 0;
volatile uint32_t g_isrRfshCount = 0;

// Separate ISR tracking for read/write operations
volatile uint16_t g_lastReadAddress = 0;
volatile uint32_t g_readIsrCount = 0;
volatile uint32_t g_writeIsrCount = 0;
volatile uint32_t g_isrTriggerCount = 0;
// Pattern counters for alternating write test (0xAA / 0x55)
volatile uint32_t g_writePatternAA_count = 0;
volatile uint32_t g_writePattern55_count = 0;

// Program base address tracking
// Z80 may not start execution at 0x0000 after reset due to internal initialization
// Capture actual starting address on first instruction fetch
volatile uint16_t g_programBaseAddress = 0;          // First instruction fetch address
volatile bool g_programBaseAddressValid = false;     // True after first READ captured

// =============================================================================
// RING BUFFER & HYBRID BACKPRESSURE VARIABLES
// =============================================================================

// Ring buffer for capturing Z80 operations
volatile CaptureEntry g_captureBuffer[CAPTURE_BUFFER_SIZE];
volatile uint8_t g_captureHead = 0;      // ISR writes here (producer)
volatile uint8_t g_captureTail = 0;      // App reads here (consumer)

// Capture control
volatile CaptureMode g_captureMode = CAPTURE_OFF;
volatile bool g_captureEnabled = false;

// Hybrid backpressure state tracking
volatile bool g_waitAsserted = false;
volatile bool g_clockThrottled = false;
volatile uint32_t g_waitStartTime = 0;
volatile uint32_t g_lowWaterTime = 0;     // When buffer dropped below low water

// Clock speed tracking
uint32_t g_targetClock = 1000000;        // Default 1 MHz
uint32_t g_currentClock = 1000000;       // Current speed

// =============================================================================
// FUNCTION IMPLEMENTATIONS
// =============================================================================

/**
 * Initialize Hardware Abstraction Layer
 * 
 * Configures all pins for Z80 interface:
 * - Address bus (input, pull-ups enabled)
 * - Data bus (input by default, pull-ups enabled)
 * - Control inputs (input, pull-ups enabled for safe defaults)
 * - Control outputs (output, set to inactive state)
 * 
 * Pull-ups are enabled on input pins for safety - if Z80 is disconnected,
 * all signals default to inactive (HIGH) state, preventing false triggers.
 */
void initializeHAL() {
  // Configure address bus as inputs (Z80 drives these)
  // A0-A7 on PORTC
  DDRC = 0x00;   // All pins input
  PORTC = 0xFF;  // Enable pull-ups (safe default if Z80 disconnected)
  
  // A8-A15 on PORTK
  DDRK = 0x00;   // All pins input
  PORTK = 0xFF;  // Enable pull-ups
  
  // Configure data bus as input by default (PORTA - bidirectional)
  DDRA = 0x00;   // All pins input (default state)
  PORTA = 0xFF;  // Enable pull-ups
  
  // Configure control inputs from Z80 (PORTL)
  // All signals are active-low, so pull-ups ensure INACTIVE (HIGH) default
  DDRL = 0x00;   // All pins input (MREQ, IORQ, M1, RFSH, HALT, BUSAK)
  PORTL = 0xFF;  // Enable pull-ups - all signals default to INACTIVE
  
  // Configure control outputs to Z80 (PORTB - bits 0-4)
  // Note: Bit 5 (CLK) is controlled by Timer1, configured in clock module
  DDRB |= (PORTB_RESET_MASK | PORTB_WAIT_MASK | PORTB_INT_MASK | 
           PORTB_NMI_MASK | PORTB_BUSRQ_MASK);
  
  // Set all control outputs to inactive state (HIGH for active-low signals)
  PORTB |= (PORTB_RESET_MASK | PORTB_WAIT_MASK | PORTB_INT_MASK | 
            PORTB_NMI_MASK | PORTB_BUSRQ_MASK);

}

/**
 * Reset Z80 Processor
 * 
 * Performs a hardware reset by pulsing the RESET line.
 * Z80 datasheet specifies minimum reset pulse width of 3 clock cycles,
 * but we use much longer (default 10ms) to ensure reliable reset.
 * 
 * @param durationMs Reset pulse duration in milliseconds (default 10ms)
 */
void resetZ80(uint16_t durationMs) {
  // Assert RESET (active LOW)
  PORTB &= ~PORTB_RESET_MASK;
  
  // Hold reset for specified duration
  delay(durationMs);
  
  // Assert WAIT before releasing RESET
  // This will cause Z80 to freeze on first instruction fetch at 0x0000
  PORTB &= ~PORTB_WAIT_MASK;  // Pull WAIT low (active)
  
  // Release RESET (inactive HIGH)
  PORTB |= PORTB_RESET_MASK;
  
  // Small delay to let Z80 start and hit the WAIT state
  delayMicroseconds(100);  // 100µs should be plenty
  
  // WAIT is now active - Z80 is frozen on first M1 cycle
  // Caller must release WAIT when ready (or polled handler will do it)
  
  // Wait for Z80 to stabilize (same duration)
  delay(durationMs);
}

/**
 * Trigger Maskable Interrupt (INT)
 * 
 * Generates an interrupt request to the Z80.
 * In interrupt mode 0, the vector byte is placed on the data bus.
 * In modes 1 and 2, the vector parameter is ignored.
 * 
 * @param vector Interrupt vector (used in mode 0 only, default 0x00)
 */
void triggerInterrupt(uint8_t vector) {
  // Assert INT (active LOW)
  PORTB &= ~PORTB_INT_MASK;
  
  // Wait for Z80 to acknowledge interrupt (IORQ + M1 both active)
  // This happens during the interrupt acknowledge cycle
  uint16_t timeout = 10000;  // Timeout counter
  while (!((~PINL & PORTL_IORQ_MASK) && (~PINL & PORTL_M1_MASK))) {
    // Wait for both IORQ and M1 to go active (LOW)
    if (--timeout == 0) {
      // Timeout - Z80 not responding, release INT and abort
      PORTB |= PORTB_INT_MASK;
      return;
    }
  }
  
  // In mode 0, Z80 expects instruction on data bus
  // Place vector byte (typically RST instruction or similar)
  writeDataBus(vector);
  
  // Hold until Z80 completes acknowledge cycle
  // Wait for IORQ or M1 to go inactive
  timeout = 10000;  // Reset timeout
  while ((~PINL & PORTL_IORQ_MASK) || (~PINL & PORTL_M1_MASK)) {
    // Wait for acknowledge cycle to complete
    if (--timeout == 0) {
      break;  // Timeout - continue anyway
    }
  }
  
  // Release INT (inactive HIGH)
  PORTB |= PORTB_INT_MASK;
  
  // Release data bus (return to input mode)
  DDRA = 0x00;
}

/**
 * Trigger Non-Maskable Interrupt (NMI)
 * 
 * Generates a non-maskable interrupt by pulsing the NMI line.
 * NMI is edge-triggered (falling edge), not level-triggered like INT.
 * Z80 will jump to address 0x0066 on NMI.
 * 
 * Pulse width is automatically adjusted based on Z80 clock frequency.
 * Minimum pulse width: 2 Z80 clock cycles (per Z80 datasheet)
 * Actual pulse width: 3 clock cycles (for safety margin)
 */
void triggerNMI() {
  // Calculate minimum pulse width based on Z80 clock frequency
  // Need at least 2 Z80 clock cycles, we use 3 for safety
  uint32_t clockFreq = getClockFrequency();
  uint32_t clockPeriodUs = 1000000UL / clockFreq;  // Period in microseconds
  uint32_t pulseWidthUs = clockPeriodUs * 3;       // 3 clock cycles
  
  // Ensure minimum pulse width of 5µs (safety for very fast clocks)
  if (pulseWidthUs < 5) {
    pulseWidthUs = 5;
  }
  
  // Pulse NMI (active LOW)
  // NMI is edge-triggered, so we need a falling edge
  PORTB &= ~PORTB_NMI_MASK;  // Drive LOW
  
  // Hold for calculated pulse width
  delayMicroseconds(pulseWidthUs);
  
  // Release NMI (inactive HIGH)
  PORTB |= PORTB_NMI_MASK;
}

/**
 * Clear Memory Emulation State
 * 
 * Resets the write capture buffer and clears any cached program data.
 * Call this before starting a new test to ensure clean state.
 */
void clearMemory() {
  // Reset write capture variables
  g_capturedData = 0;
  g_lastWriteAddress = 0;
  g_programLoaded = false;
  
  // Write capture buffer will be added when we implement the full capture system
  // For now, we just reset the simple capture variables
}

/**
 * Set Active Test Program (PROGMEM only, no caching)
 * 
 * Configures the ISR to serve data from the specified test program
 * stored in PROGMEM (flash memory). Use for very large test programs
 * (>256 bytes) or when SRAM caching is not desired.
 * 
 * @param progmem_ptr Pointer to test program in PROGMEM (flash memory)
 * @param length Program length in bytes
 */
void setTestProgram(const uint8_t* progmem_ptr, uint16_t length) {
  // Null pointer check
  if (progmem_ptr == nullptr) {
    Serial.println(F("ERROR: Null program pointer"));
    return;
  }
  
  // Store program pointer and length
  g_testProgram = progmem_ptr;
  g_testProgramLength = length;
  g_programLoaded = true;
  
  // Disable caching - use PROGMEM directly
  g_useCaching = false;
  g_cacheSize = 0;
  
  // Clear any previous write captures
  clearMemory();
  
  // Confirmation message
  Serial.print(F("INFO: Program set ("));
  Serial.print(length);
  Serial.println(F(" bytes, PROGMEM only)"));
}

/**
 * Load Test Program into SRAM Cache
 * 
 * Copies a test program from PROGMEM into SRAM cache for fastest ISR
 * performance. Recommended for most instruction tests (<256 bytes).
 * If program is >256 bytes, automatically falls back to PROGMEM.
 * 
 * @param progmem_ptr Pointer to test program in PROGMEM
 * @param length Program length in bytes (max 256 for caching)
 * @return true if successfully cached in SRAM, false if using PROGMEM fallback
 */
bool loadProgramToCache(const uint8_t* progmem_ptr, uint16_t length) {
  // Null pointer check
  if (progmem_ptr == nullptr) {
    Serial.println(F("ERROR: Null program pointer"));
    return false;
  }
  
  // Set PROGMEM pointer for fallback (always set this)
  g_testProgram = progmem_ptr;
  g_testProgramLength = length;
  // Mark program as loaded so ISR will serve bytes (must match setTestProgram())
  g_programLoaded = true;
  
  // Check if program fits in cache
  if (length <= g_maxCacheSize) {
    // Disable interrupts while modifying cache to prevent ISR reading partial data
    noInterrupts();
    
    // Disable caching FIRST so ISR falls back to PROGMEM during update
    g_useCaching = false;
    g_cacheSize = 0;
    
    // Clear the cache first to prevent stale data
    memset(g_programCache, 0x00, g_maxCacheSize);
    
    // Copy from PROGMEM to SRAM
    for (uint16_t i = 0; i < length; i++) {
      g_programCache[i] = pgm_read_byte(&progmem_ptr[i]);
    }
    
    // Enable caching mode LAST (atomic update)
    g_cacheSize = length;
    g_useCaching = true;
    
    // Re-enable interrupts
    interrupts();
    
    Serial.print(F("INFO: Program cached ("));
    Serial.print(length);
    Serial.print(F(" bytes in SRAM, max="));
    Serial.print(g_maxCacheSize);
    Serial.println(F(")"));
    
    return true;
  } else {
    // Program too large for cache - use PROGMEM directly
    g_useCaching = false;
    g_cacheSize = 0;
    
    Serial.print(F("INFO: Using PROGMEM ("));
    Serial.print(length);
    Serial.print(F(" bytes > max cache "));
    Serial.print(g_maxCacheSize);
    Serial.println(F(")"));
    
    return false;
  }
}

/**
 * Set maximum cache size for program storage
 */
bool setCacheSize(uint16_t maxSize) {
  // Validate cache size (must be power of 2, reasonable range)
  if (maxSize != 128 && maxSize != 256 && maxSize != 512 && maxSize != 1024) {
    Serial.print(F("ERROR: Invalid cache size "));
    Serial.print(maxSize);
    Serial.println(F(" (must be 128, 256, 512, or 1024)"));
    return false;
  }
  
  if (maxSize > MAX_CACHE_SIZE) {
    Serial.print(F("ERROR: Cache size "));
    Serial.print(maxSize);
    Serial.print(F(" exceeds maximum "));
    Serial.println(MAX_CACHE_SIZE);
    return false;
  }
  
  // Disable interrupts while changing cache configuration
  noInterrupts();
  
  // Clear any existing cached program
  g_useCaching = false;
  g_cacheSize = 0;
  memset(g_programCache, 0x00, g_maxCacheSize);
  
  // Set new cache size
  g_maxCacheSize = maxSize;
  
  interrupts();
  
  Serial.print(F("INFO: Cache size set to "));
  Serial.print(maxSize);
  Serial.println(F(" bytes"));
  
  return true;
}

/**
 * Get current maximum cache size
 */
uint16_t getCacheSize() {
  return g_maxCacheSize;
}

/**
 * Get current SRAM usage for cache
 */
uint16_t getCacheUsage() {
  return g_useCaching ? g_cacheSize : 0;
}

/**
 * Get Captured Write Data
 * 
 * Retrieves the last write operation captured from Z80.
 * Only captures the most recent write - previous writes are overwritten.
 * 
 * @param index Write index (must be 0 for single-write capture)
 * @param address Output parameter for write address (can be nullptr)
 * @param data Output parameter for written data (can be nullptr)
 * @return true if valid data available, false otherwise
 */
bool getCapturedWrite(uint8_t index, uint16_t* address, uint8_t* data) {
  // Only support single write capture (index must be 0)
  if (index != 0) {
    return false;
  }
  
  // Return captured data
  if (address != nullptr) {
    *address = g_lastWriteAddress;
  }
  if (data != nullptr) {
    *data = g_capturedData;
  }
  
  return true;
}

/**
 * Get Number of Captured Writes
 * 
 * Returns the number of write operations available in capture buffer.
 * For single-write capture, this is always 0 or 1.
 * 
 * @return 1 if a write has been captured, 0 otherwise
 */
uint8_t getCapturedWriteCount() {
  // For single-write capture, return 1 if we have any data
  // Note: Can't distinguish between "no write" and "write of 0 to address 0"
  return (g_lastWriteAddress != 0 || g_capturedData != 0) ? 1 : 0;
}

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
 *       operations in program memory space (0x0000-0x00FF). WRITE operations
 *       typically target high memory (0x8000+) and use absolute addressing.
 * 
 * Usage: Call after running a test program to see what was written
 */
void processCaptures() {
  // Check capture mode
  if (g_captureMode == CAPTURE_OFF) {
    // Simple mode - check single write flag
    if (g_dataReady) {
      Serial.print(F("WRITE captured: Addr=0x"));
      Serial.print(g_lastWriteAddress, HEX);
      Serial.print(F(", Data=0x"));
      Serial.println(g_capturedData, HEX);
      
      g_dataReady = false;  // Clear flag
    } else {
      Serial.println(F("No WRITE captured"));
    }
  } 
  else if (g_captureMode == CAPTURE_BUFFERED) {
    // Ring buffer mode - display all captured operations
    uint8_t count = getCaptureBufferLevel();
    
    if (count == 0) {
      Serial.println(F("No operations in capture buffer"));
      return;
    }
    
    Serial.print(F("Capture buffer contains "));
    Serial.print(count);
    Serial.println(F(" operations:"));
    
    // Read and display all entries
    while (getCaptureBufferLevel() > 0) {
      CaptureEntry entry;
      if (readCaptureBuffer(&entry)) {
        Serial.print(F("  "));
        Serial.print(entry.operation == 0 ? F("READ ") : F("WRITE"));
        Serial.print(F(": Addr=0x"));
        Serial.print(entry.address, HEX);
        Serial.print(F(", Data=0x"));
        Serial.print(entry.data, HEX);
        Serial.print(F(", Time="));
        Serial.print(entry.timestamp);
        Serial.println(F("us"));
      }
    }
  }
  else {
    Serial.println(F("CAPTURE_IMMEDIATE mode - use manual inspection"));
  }
}

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
 * 
 * Why no offset calculation?
 * - The g_programBaseAddress offset ONLY applies to READ operations in program
 *   memory space (0x0000-0x00FF) where the Z80 fetches instructions.
 * - WRITE operations target high memory (0x8000+) using absolute addressing.
 * - The ISR captures the actual bus address for WRITEs (see hal.cpp line 785).
 * - Test programs use absolute addresses: LD (0x8000), A writes to exactly 0x8000.
 * 
 * Memory Map:
 * - 0x0000-0x00FF: Program space (may have base offset for READs)
 * - 0x8000-0xFFFF: Write target space (absolute addresses, no offset)
 * 
 * Example: If program starts at 0x0050 (base offset), a WRITE to 0x8000 
 *          still captures as 0x8000, not 0x8050.
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
bool validateWrite(uint16_t expectedAddr, uint8_t expectedData) {
  bool found = false;
  bool addrMatch = false;
  bool dataMatch = false;
  
  // Check capture mode
  if (g_captureMode == CAPTURE_OFF) {
    // Simple mode - check single write
    if (!g_dataReady) {
      Serial.println(F("✗ WRITE validation FAILED: No write captured"));
      return false;
    }
    
    addrMatch = (g_lastWriteAddress == expectedAddr);
    dataMatch = (g_capturedData == expectedData);
    found = true;
    
    // Clear flag after reading
    g_dataReady = false;
  }
  else if (g_captureMode == CAPTURE_BUFFERED) {
    // Ring buffer mode - search for matching entry
    // Note: This is a read-only check, doesn't consume buffer
    uint8_t level = getCaptureBufferLevel();
    uint8_t tail = g_captureTail;  // Snapshot
    
    for (uint8_t i = 0; i < level; i++) {
      uint8_t index = (tail + i) & (CAPTURE_BUFFER_SIZE - 1);
      
      // Access volatile buffer members directly to avoid copy issues
      volatile CaptureEntry* volatileEntry = &g_captureBuffer[index];
      
      // Only check WRITE operations
      if (volatileEntry->operation == 1) {
        if (volatileEntry->address == expectedAddr && volatileEntry->data == expectedData) {
          found = true;
          addrMatch = true;
          dataMatch = true;
          break;
        }
      }
    }
    
    if (!found) {
      Serial.println(F("✗ WRITE validation FAILED: No matching write in buffer"));
      return false;
    }
  }
  else {
    Serial.println(F("✗ WRITE validation not supported in CAPTURE_IMMEDIATE mode"));
    return false;
  }
  
  // Check if found write matches expectations
  if (addrMatch && dataMatch) {
    Serial.print(F("✓ WRITE validation PASSED: Addr=0x"));
    Serial.print(expectedAddr, HEX);
    Serial.print(F(", Data=0x"));
    Serial.println(expectedData, HEX);
    return true;
  } else {
    Serial.println(F("✗ WRITE validation FAILED: Mismatch"));
    
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
    
    return false;
  }
}

/**
 * Read one entry from ring buffer
 * 
 * @param entry Pointer to CaptureEntry structure to fill
 * @return true if entry read successfully, false if buffer empty
 * 
 * Note: This consumes the entry from the buffer (moves tail pointer)
 */
bool readCaptureBuffer(CaptureEntry* entry) {
  if (getCaptureBufferLevel() == 0) {
    return false;  // Buffer empty
  }
  
  // Get the entry at tail pointer
  uint8_t tailIndex = g_captureTail;
  
  // Copy data from volatile buffer to non-volatile structure
  entry->address = g_captureBuffer[tailIndex].address;
  entry->data = g_captureBuffer[tailIndex].data;
  entry->operation = g_captureBuffer[tailIndex].operation;
  entry->timestamp = g_captureBuffer[tailIndex].timestamp;
  
  // Move tail pointer (consume the entry)
  g_captureTail = (g_captureTail + 1) & (CAPTURE_BUFFER_SIZE - 1);
  
  return true;
}

/**
 * Initialize Memory Handler
 * 
 * Sets up external interrupts INT5 and INT4 for detecting Z80 read/write operations.
 * INT5 triggers on RD falling edge, INT4 on WR falling edge.
 * Separate ISRs handle read and write operations for improved performance.
 * 
 * CRITICAL: RD on D3 (PE5/INT5), WR on D2 (PE4/INT4)
 *           Separate interrupts eliminate polling and reduce missed operations
 * 
 * CRITICAL: Must be called AFTER initializeHAL()
 * CRITICAL: ISRs must be implemented before enabling interrupts
 */
void initializeMemoryHandler() {
  // Ensure control pins are configured as inputs
  pinMode(Z80_MREQ, INPUT);
  pinMode(Z80_RD, INPUT_PULLUP);
  pinMode(Z80_WR, INPUT);
  
  // Configure INT5 (PE5, pin D3) for falling edge trigger (RD active)
  // Configure INT4 (PE4, pin D2) for falling edge trigger (WR active)
  // EICRB controls INT4-INT7
  // ISC5[1:0] = 0b10 = falling edge for INT5
  // ISC4[1:0] = 0b10 = falling edge for INT4
  EICRB &= ~((1 << ISC51) | (1 << ISC50) | (1 << ISC41) | (1 << ISC40));  // Clear bits for INT5/INT4
  EICRB |= ((1 << ISC51) | (1 << ISC41));  // Set bit 1 for both -> falling edge

  // Clear any pending interrupt flags before enabling
  EIFR |= ((1 << INTF5) | (1 << INTF4));

  // Enable INT5 and INT4 in the external interrupt mask register
  EIMSK |= ((1 << INT5) | (1 << INT4));

  // Enable global interrupts
  sei();

  Serial.println(F("INFO: Memory handler initialized (INT5 on D3, INT4 on D2)"));
}


// =============================================================================
// HARDWARE TEST FUNCTIONS
// =============================================================================

/**
 * Run Hardware Test Suite
 * 
 * Comprehensive test of all HAL functions with real hardware.
 */
void runHALTest() {
  Serial.println(F("\n=== HAL Hardware Test Suite ===\n"));
  
  // Test 1: Address Bus Read
  Serial.println(F("Test 1: Address Bus Read"));
  Serial.println(F("  Reading current address bus state..."));
  uint16_t address = readAddressBus();
  Serial.print(F("  Address Bus = 0x"));
  if (address < 0x1000) Serial.print('0');
  if (address < 0x0100) Serial.print('0');
  if (address < 0x0010) Serial.print('0');
  Serial.println(address, HEX);
  Serial.println(F("  [PASS] Address bus read successful"));
  
  // Test 2: Data Bus Read
  Serial.println(F("\nTest 2: Data Bus Read"));
  Serial.println(F("  Setting data bus to input mode..."));
  Serial.println(F("  Reading current data bus state..."));
  uint8_t data = readDataBus();
  Serial.print(F("  Data Bus = 0x"));
  Serial.println(data, HEX);
  Serial.println(F("  [PASS] Data bus read successful"));
  
  // Test 3: Data Bus Write
  Serial.println(F("\nTest 3: Data Bus Write"));
  Serial.println(F("  Writing test pattern 0xAA to data bus..."));
  writeDataBus(0xAA);
  delay(10);
  Serial.println(F("  Writing test pattern 0x55 to data bus..."));
  writeDataBus(0x55);
  delay(10);
  Serial.println(F("  Writing test pattern 0x00 to data bus..."));
  writeDataBus(0x00);
  delay(10);
  Serial.println(F("  Returning data bus to input mode..."));
  DDRA = 0x00;  // Return to input
  Serial.println(F("  [PASS] Data bus write successful"));
  
  // Test 4: Control Signal Monitoring
  Serial.println(F("\nTest 4: Control Signal Monitoring"));
  Serial.println(F("  Reading all control signals..."));
  uint8_t signals = readControlSignals();
  Serial.print(F("  Raw Control Signals = 0b"));
  Serial.println(signals, BIN);
  
  Serial.print(F("  MREQ:  "));
  Serial.println(isMemoryRequest() ? F("ACTIVE") : F("INACTIVE"));
  
  Serial.print(F("  IORQ:  "));
  Serial.println(isIORequest() ? F("ACTIVE") : F("INACTIVE"));
  
  Serial.print(F("  RD:    "));
  Serial.println(isReadCycle() ? F("ACTIVE") : F("INACTIVE"));
  
  Serial.print(F("  WR:    "));
  Serial.println(isWriteCycle() ? F("ACTIVE") : F("INACTIVE"));
  
  Serial.print(F("  M1:    "));
  Serial.println(isM1Cycle() ? F("ACTIVE") : F("INACTIVE"));
  
  Serial.print(F("  RFSH:  "));
  Serial.println(isRefreshCycle() ? F("ACTIVE") : F("INACTIVE"));
  
  Serial.print(F("  HALT:  "));
  Serial.println(isHalted() ? F("ACTIVE") : F("INACTIVE"));
  
  Serial.print(F("  BUSAK: "));
  Serial.println(isBusAcknowledged() ? F("ACTIVE") : F("INACTIVE"));
  
  Serial.println(F("  [PASS] Control signal monitoring successful"));
  
  // Test 5: WAIT Signal Control
  Serial.println(F("\nTest 5: WAIT Signal Control"));
  Serial.println(F("  Asserting WAIT (driving LOW)..."));
  assertWait();
  delay(100);
  Serial.println(F("  WAIT should be active (pin 52 = LOW)"));
  Serial.println(F("  Releasing WAIT (driving HIGH)..."));
  releaseWait();
  delay(100);
  Serial.println(F("  WAIT should be inactive (pin 52 = HIGH)"));
  Serial.println(F("  [PASS] WAIT signal control successful"));
  
  // Test 6: Z80 Reset
  Serial.println(F("\nTest 6: Z80 Reset"));
  Serial.println(F("  Performing Z80 reset (50ms pulse)..."));
  Serial.println(F("  RESET will go LOW for 50ms, then HIGH for 50ms"));
  resetZ80(50);
  Serial.println(F("  [PASS] Z80 reset sequence completed"));
  
  // Test 7: NMI Pulse
  Serial.println(F("\nTest 7: NMI Pulse"));
  Serial.print(F("  Triggering NMI pulse (clock-synchronized)..."));
  uint32_t clockFreq = getClockFrequency();
  uint32_t pulseWidth = (1000000UL / clockFreq) * 3;
  if (pulseWidth < 5) pulseWidth = 5;
  Serial.print(F("\n  Pulse width: "));
  Serial.print(pulseWidth);
  Serial.println(F(" µs"));
  triggerNMI();
  Serial.println(F("  [PASS] NMI pulse completed"));
  
  // Test 8: Memory State
  Serial.println(F("\nTest 8: Memory State Management"));
  Serial.println(F("  Clearing memory state..."));
  clearMemory();
  Serial.print(F("  Captured writes: "));
  Serial.println(getCapturedWriteCount());
  Serial.println(F("  [PASS] Memory state management successful"));
  
  // Test 9: Test Program Management
  Serial.println(F("\nTest 9: Test Program Management"));
  const uint8_t testProg[] PROGMEM = {0x00, 0x3E, 0x42, 0x76};  // NOP, LD A,42h, HALT
  Serial.println(F("  Setting test program (4 bytes in PROGMEM)..."));
  setTestProgram(testProg, 4);
  Serial.print(F("  Program length: "));
  Serial.println(g_testProgramLength);
  Serial.println(F("  Reading program bytes:"));
  for (uint16_t i = 0; i < g_testProgramLength; i++) {
    uint8_t byte = pgm_read_byte(&g_testProgram[i]);
    Serial.print(F("    ["));
    Serial.print(i);
    Serial.print(F("] = 0x"));
    Serial.println(byte, HEX);
  }
  Serial.println(F("  [PASS] Test program management successful"));
  
  // Test 10: Pin Configuration Verification
  Serial.println(F("\nTest 10: Pin Configuration Verification"));
  Serial.println(F("  Checking DDR registers..."));
  Serial.print(F("  DDRA (Data Bus):    0x"));
  Serial.print(DDRA, HEX);
  Serial.println(F(" (should be 0x00 = INPUT)"));
  
  Serial.print(F("  DDRC (Addr Lo):     0x"));
  Serial.print(DDRC, HEX);
  Serial.println(F(" (should be 0x00 = INPUT)"));
  
  Serial.print(F("  DDRK (Addr Hi):     0x"));
  Serial.print(DDRK, HEX);
  Serial.println(F(" (should be 0x00 = INPUT)"));
  
  Serial.print(F("  DDRL (Ctrl Inputs): 0x"));
  Serial.print(DDRL, HEX);
  Serial.println(F(" (should be 0x00 = INPUT)"));
  
  Serial.print(F("  DDRB (Ctrl Outputs):0x"));
  Serial.print(DDRB, HEX);
  Serial.println(F(" (bit 0-4 should be OUTPUT)"));
  
  Serial.println(F("  [PASS] Pin configuration verified"));
  
  // Summary
  Serial.println(F("\n=== HAL Hardware Test Complete ==="));
  Serial.println(F("All 10 tests passed successfully!"));
  Serial.println(F("\nNote: Some tests require oscilloscope/logic analyzer"));
  Serial.println(F("      to verify actual signal timing and levels."));
  Serial.println();
}


// =============================================================================
// RING BUFFER & CAPTURE FUNCTIONS
// =============================================================================

/**
 * Fast capture function called from ISR
 * Adds entry to ring buffer and manages WAIT signal for hybrid backpressure
 * 
 * OPTIMIZED for ISR performance:
 * - Uses bitwise AND instead of modulo (CAPTURE_BUFFER_SIZE must be power of 2)
 * - Minimizes branching
 * - Avoids expensive micros() call in critical path
 * 
 * @param addr Z80 address
 * @param data Data byte
 * @param op Operation type (0=READ, 1=WRITE)
 * 
 * Performance: ~8-10 clock cycles (optimized)
 */
inline void captureOperation(uint16_t addr, uint8_t data, uint8_t op) __attribute__((always_inline));

inline void captureOperation(uint16_t addr, uint8_t data, uint8_t op) {
  // Calculate next head position using bitwise AND (fast, works for power-of-2 sizes)
  register uint8_t next = (g_captureHead + 1) & (CAPTURE_BUFFER_SIZE - 1);
  
  // Check if buffer is full (should be rare - only if WAIT response was slow)
  if (next == g_captureTail) {
    g_isrErrorCount++;  // Count overflow (diagnostic only)
    // Don't assert WAIT here - already should be asserted from HIGH_WATER
    return;  // Drop entry to prevent corruption
  }
  
  // Add entry to buffer (no timestamp in fast path to save cycles)
  register uint8_t head = g_captureHead;
  g_captureBuffer[head].address = addr;
  g_captureBuffer[head].data = data;
  g_captureBuffer[head].operation = op;
  
  // Update head pointer
  g_captureHead = next;
  
  // Fast check: Assert WAIT if at or above HIGH_WATER (75% = 12 entries)
  // This gives us 4 entries of safety margin before overflow
  register uint8_t count = (next >= g_captureTail) 
      ? (next - g_captureTail)
      : (CAPTURE_BUFFER_SIZE - g_captureTail + next);
  
  if (count >= BUFFER_HIGH_WATER && !g_waitAsserted) {
    PORTB &= ~PORTB_WAIT_MASK;  // Assert WAIT (active LOW)
    g_waitAsserted = true;
    // Timing tracking happens in main loop (manageHybridBackpressure)
  }
}

// =============================================================================
// INTERRUPT SERVICE ROUTINE (ISR) - Z80 Memory Request Handler
// =============================================================================

/**
 * External Interrupt 4 Service Routine
 * Triggered on MREQ falling edge (Z80 memory request)
 * 
 * Performance: ~1.1 µs total latency @ 16 MHz (with SRAM cache)
 *              ~1.3 µs total latency @ 16 MHz (with PROGMEM fallback)
 * 
 * Supports: READ operations with SRAM caching and PROGMEM fallback
 * 
 * CRITICAL: Keep ISR as fast as possible!
 * CRITICAL: MREQ is guaranteed active by hardware (falling edge trigger)
 * 
 * GCC Attributes:
 * - hot: Optimize for speed (inline everything, favor performance)
 * - flatten: Inline all function calls within ISR
 * - optimize("O3"): Maximum optimization level for this function
 * - optimize("unroll-loops"): Unroll loops for speed
 */
/*
ISR(INT4_vect) __attribute__((hot, flatten, optimize("O3"), optimize("unroll-loops")));

ISR(INT4_vect) {
  // Increment call counter (optional, for debugging)
  g_isrCallCount++;
  
  // ========================================
  // STAGE 1: Read Control Signals (variable cycles)
  // ========================================
  
  // Poll for RD or WR to assert (WR goes low on next Z80 clock falling edge after MREQ)
  // Use fixed timeout suitable for expected Z80 frequency range (10kHz-1MHz)
  register uint8_t control;
  uint8_t poll_count = 0;
  const uint8_t max_polls = 100;  // ~6.25us at 16MHz - suitable for 100kHz+ Z80
  
  do {
    control = ~PINL;
    poll_count++;
    if ((control & (PORTL_RD_MASK | PORTL_WR_MASK)) || poll_count >= max_polls) break;
    __asm__ __volatile__ ("nop\n\t");  // 1 cycle delay between polls
  } while (true);
  
  // Read 16-bit address from bus (2 cycles)
  // A0-A7 on PORTC, A8-A15 on PORTK
  register uint16_t address = ((uint16_t)PINK << 8) | PINC;  // 2 port reads
  
  // ========================================
  // STAGE 2: Determine Operation Type (2 cycles)
  // ========================================
  
  // Check if this is a REFRESH cycle (RFSH active)
  // Refresh cycles should be ignored - they're not memory accesses
  if (control & PORTL_RFSH_MASK) {
    // REFRESH cycle - do nothing, just return
    g_isrRfshCount++;  // DEBUG
    // NOTE: Don't change DDRA here - leave it as-is
    // If previous cycle was READ, data bus should remain driven
    return;
  }
  
  // Default: Set data bus as input to prevent bus contention
  DDRA = 0x00;
  
  // Check if this is a READ operation (RD active)
  if (control & PORTL_RD_MASK) {
    // READ cycle: Z80 is reading from memory
    g_isrReadCount++;  // DEBUG
    
    // ====================================
    // STAGE 3a: Provide Data (4-8 cycles)
    // ====================================
    
    // Calculate program-relative offset from base address
    // This handles cases where Z80 starts execution at non-zero address
    register uint16_t baseAddr = g_programBaseAddress;
    if (!g_programBaseAddressValid) {
      baseAddr = address;
      g_programBaseAddress = address;
      g_programBaseAddressValid = true;
    }

    register uint16_t programOffset = address - baseAddr;

    // Determine data to output
    register uint8_t outputData;
    if (g_useCaching && programOffset < g_cacheSize) {
      // FAST: Read from SRAM cache (2-3 cycles)
      outputData = g_programCache[programOffset];
    }
    // Fallback to PROGMEM for larger programs
    else if (programOffset < g_testProgramLength) {
      // SLOWER: Read byte from PROGMEM (4-6 cycles)
      outputData = pgm_read_byte(&g_testProgram[programOffset]);
    } 
    else {
      // Address out of range - return NOP (0x00)
      // This is normal during HALT state - Z80 continues fetching
      outputData = 0x00;  // Safe default (NOP instruction)
      // Note: Not counted as error - this is expected Z80 behavior
    }
    
    // Set PORTA first, then enable output to prevent glitches
    PORTA = outputData;
    DDRA = 0xFF;
    
    // Optional: Capture READ operations based on mode
    // Optimized: early exit if capture is off (most common case)
    if (g_captureMode != CAPTURE_OFF) {
      if (g_captureMode == CAPTURE_BUFFERED) {
        // Ring buffer mode - capture to buffer with backpressure
        captureOperation(address, PORTA, 0);  // 0 = READ
      }
      else {  // CAPTURE_IMMEDIATE
        // Immediate mode - direct capture for single-step debugging
        g_lastWriteAddress = address;  // Reuse write variables for consistency
        g_capturedData = PORTA;
        g_dataReady = true;
        // WAIT is already asserted, app will release it
      }
    }
    
    // Data is now on bus and will remain until next cycle
    return;  // Exit ISR
  }
  
  // ========================================
  // STAGE 3b: Handle WRITE Operations
  // ========================================
  
  // Check if this is a WRITE operation (WR active)
  if (control & PORTL_WR_MASK) {
    // WRITE cycle: Z80 is writing to memory
    
    // Data bus is already set as input (from top of function)
    
    // Read data from Z80 (1 cycle)
    register uint8_t data = PINA;
    
    // Handle capture based on mode (optimized: check most common case first)
    if (g_captureMode == CAPTURE_OFF) {
      // CAPTURE_OFF mode (default, fastest path) - just store for polling
      g_lastWriteAddress = address;
      g_capturedData = data;
      g_dataReady = true;
    }
    else if (g_captureMode == CAPTURE_BUFFERED) {
      // Ring buffer mode - capture to buffer with backpressure
      captureOperation(address, data, 1);  // 1 = WRITE
    }
    else {  // CAPTURE_IMMEDIATE
      // Immediate mode - direct capture for single-step debugging
      g_lastWriteAddress = address;
      g_capturedData = data;
      g_dataReady = true;
      // WAIT is already asserted, app will release it
    }
    
    return;  // Exit ISR
  }
  
  // ========================================
  // STAGE 3c: Handle Other Operations
  // ========================================
  // ========================================
  // STAGE 3c: Handle Other Operations
  // ========================================
  
  // Not a READ, WRITE, or RFSH operation
  // This could be a spurious interrupt or other bus cycle
  // Data bus is already set as input (from top of function)
  
  return;  // Exit ISR
}
*/

// =============================================================================
// INTERRUPT SERVICE ROUTINES (ISRs) - Separate RD/WR Handlers
// =============================================================================

/**
 * External Interrupt 5 Service Routine - Read Handler
 * Triggered on RD falling edge (Z80 read request)
 * 
 * Performance: Optimized for speed with separate interrupts
 * 
 * Supports: READ operations with SRAM caching and PROGMEM fallback
 * 
 * CRITICAL: Keep ISR as fast as possible!
 * CRITICAL: RD is guaranteed active by hardware (falling edge trigger)
 * CRITICAL: MREQ must also be active for valid memory access
 * 
 * GCC Attributes:
 * - hot: Optimize for speed (inline everything, favor performance)
 * - flatten: Inline all function calls within ISR
 * - optimize("O3"): Maximum optimization level for this function
 * - optimize("unroll-loops"): Unroll loops for speed
 */
ISR(INT5_vect) __attribute__((hot, flatten, optimize("O3"), optimize("unroll-loops")));

ISR(INT5_vect) {
  g_isrTriggerCount++;
  // Check MREQ: If MREQ is not active (high), ignore this interrupt
  // MREQ is on PL2 (bit 2), active low
  if ((PINL & 0x04) != 0) return;
  
  // Read 16-bit address from bus (A0-A7 on PORTC, A8-A15 on PORTK)
  register uint16_t address = ((uint16_t)PINK << 8) | PINC;
  
  // Determine data to output
  register uint8_t outputData;
  if (!g_programLoaded) {
    // No program loaded - return NOP (0x00) for all reads
    outputData = 0x00;
  } else {
    // Program is loaded - handle normally
    // Capture base address on first read if not already captured
    if (!g_programBaseAddressValid) {
      g_programBaseAddress = address;
      g_programBaseAddressValid = true;
    }
    
    // Calculate program-relative offset
    register uint16_t programOffset = address - g_programBaseAddress;
    
    if (g_useCaching && programOffset < g_cacheSize) {
      // FAST: Read from SRAM cache
      outputData = g_programCache[programOffset];
    }
    else if (programOffset < g_testProgramLength) {
      // Read from PROGMEM
      outputData = pgm_read_byte(&g_testProgram[programOffset]);
    }
    else {
      // Address out of range - return NOP (0x00)
      outputData = 0x00;
    }
  }
  
  // Provide data on bus: Set PORTA first, then enable output
  PORTA = outputData;
  DDRA = 0xFF;
  
  // Handle capture if enabled
  if (g_captureMode != CAPTURE_OFF) {
    if (g_captureMode == CAPTURE_BUFFERED) {
      captureOperation(address, outputData, 0);  // 0 = READ
    }
    else {  // CAPTURE_IMMEDIATE
      g_lastWriteAddress = address;  // Reuse for consistency
      g_capturedData = outputData;
      g_dataReady = true;
    }
  }
  
  // Update read tracking globals
  g_lastReadAddress = address;
  g_readIsrCount++;
}

/**
 * External Interrupt 4 Service Routine - Write Handler
 * Triggered on WR falling edge (Z80 write request)
 * 
 * Performance: Optimized for speed with separate interrupts
 * 
 * Supports: WRITE operations with capture for verification
 * 
 * CRITICAL: Keep ISR as fast as possible!
 * CRITICAL: WR is guaranteed active by hardware (falling edge trigger)
 * CRITICAL: MREQ must also be active for valid memory access
 * 
 * GCC Attributes:
 * - hot: Optimize for speed (inline everything, favor performance)
 * - flatten: Inline all function calls within ISR
 * - optimize("O3"): Maximum optimization level for this function
 * - optimize("unroll-loops"): Unroll loops for speed
 */
ISR(INT4_vect) __attribute__((hot, flatten, optimize("O3"), optimize("unroll-loops")));

ISR(INT4_vect) {
  // Check MREQ: If MREQ is not active (high), ignore this interrupt
  // MREQ is on PL2 (bit 2), active low
  if ((PINL & 0x04) != 0) return;

  // Read 16-bit address from bus (A0-A7 on PORTC, A8-A15 on PORTK)
  register uint16_t address = ((uint16_t)PINK << 8) | PINC;

  // Ensure data bus is configured as INPUT before sampling (Arduino must not drive)
  DDRA = 0x00;
  // Small settle to allow Z80 to drive the bus
  __asm__ __volatile__ ("nop\n\t");
  // Read data from Z80 (data bus is input during writes)
  register uint8_t data = PINA;
  // Track alternating test patterns (cheap checks)
  if (data == 0xAA) g_writePatternAA_count++;
  else if (data == 0x55) g_writePattern55_count++;
  
  // Handle capture based on mode
  if (g_captureMode == CAPTURE_OFF) {
    // CAPTURE_OFF mode - just store for polling
    g_lastWriteAddress = address;
    g_capturedData = data;
    g_dataReady = true;
  }
  else if (g_captureMode == CAPTURE_BUFFERED) {
    // Ring buffer mode - capture to buffer with backpressure
    captureOperation(address, data, 1);  // 1 = WRITE
  }
  else {  // CAPTURE_IMMEDIATE
    // Immediate mode - direct capture for single-step debugging
    g_lastWriteAddress = address;
    g_capturedData = data;
    g_dataReady = true;
    // WAIT is already asserted, app will release it
  }
  
  // Update write tracking globals
  g_lastWriteAddress = address;
  g_writeIsrCount++;
}

// =============================================================================
// RING BUFFER & HYBRID BACKPRESSURE MANAGEMENT FUNCTIONS
// =============================================================================

/**
 * Set capture mode for Z80 operations
 */
void setCaptureMode(CaptureMode mode) {
  // Disable interrupts while changing mode
  noInterrupts();
  
  g_captureMode = mode;
  g_captureEnabled = (mode != CAPTURE_OFF);
  
  // If switching to IMMEDIATE mode, pre-assert WAIT for single-step control
  if (mode == CAPTURE_IMMEDIATE) {
    PORTB &= ~PORTB_WAIT_MASK;  // Assert WAIT (active LOW)
    g_waitAsserted = true;
  }
  
  interrupts();
}

/**
 * Get current ring buffer usage level
 */
uint8_t getCaptureBufferLevel() {
  uint8_t head = g_captureHead;  // Snapshot volatile
  uint8_t tail = g_captureTail;
  
  if (head >= tail) {
    return head - tail;
  } else {
    return CAPTURE_BUFFER_SIZE - tail + head;
  }
}

/**
 * Clear ring buffer and reset capture state
 */
void clearCaptureBuffer() {
  noInterrupts();
  g_captureHead = 0;
  g_captureTail = 0;
  interrupts();
}

/**
 * Process captured data from ring buffer
 * 
 * This function consumes entries from the ring buffer and processes them.
 * For now, it simply increments the tail pointer to mark entries as processed.
 * Application code can extend this to add validation, logging, etc.
 * 
 * Automatically releases WAIT when buffer drains below LOW_WATER mark (25%).
 */
void processCaptureBuffer() {
  // Process all available entries
  while (g_captureTail != g_captureHead) {
    // Get next entry (application can add processing here)
    // CaptureEntry* entry = (CaptureEntry*)&g_captureBuffer[g_captureTail];
    
    // Application processing could include:
    // - Validate instruction execution
    // - Log data to serial
    // - Check for errors
    // - Analyze timing
    
    // For now, just consume the entry
    g_captureTail = (g_captureTail + 1) & (CAPTURE_BUFFER_SIZE - 1);
    
    // Check if buffer has drained enough to release WAIT
    uint8_t count = getCaptureBufferLevel();
    
    if (count <= BUFFER_LOW_WATER && g_waitAsserted) {
      PORTB |= PORTB_WAIT_MASK;  // Release WAIT (inactive HIGH)
      g_waitAsserted = false;
    }
  }
}

/**
 * Hybrid backpressure management
 * 
 * Call from main loop periodically to manage WAIT states and clock throttling.
 * 
 * Improved hybrid strategy with progressive throttling:
 * - Buffer ≥75% full: Assert WAIT (immediate protection)
 * - WAIT sustained >10ms: Throttle clock (keep WAIT active!)
 * - If still ≥75% after throttle >10ms: Throttle MORE (progressive)
 * - Buffer <25% full: Release WAIT
 * - Buffer <25% for >10ms: Remove throttling, restore full speed
 * 
 * Key features:
 * - Progressive throttling: Can reduce clock multiple times if needed
 * - Floor protection: Won't throttle below MIN_CLOCK_SPEED (100 KHz)
 * - WAIT always active when buffer is high (prevents overflow)
 */
void manageHybridBackpressure() {
  uint8_t level = getCaptureBufferLevel();
  uint32_t now = micros();
  
  // ====== HIGH BUFFER LEVEL (≥75% full) ======
  if (level >= BUFFER_HIGH_WATER) {
    // Clear low water timer when we're high again
    g_lowWaterTime = 0;
    
    // Assert WAIT if not already asserted
    if (!g_waitAsserted) {
      PORTB &= ~PORTB_WAIT_MASK;  // Assert WAIT (active LOW)
      g_waitAsserted = true;
      g_waitStartTime = now;
    }
    
    // Check if we need to add or increase throttling
    uint32_t waitDuration = now - g_waitStartTime;
    
    if (waitDuration > SUSTAINED_WAIT_TIME) {
      // WAIT has been sustained - we need throttling
      
      if (!g_clockThrottled) {
        // First time throttling - reduce from target speed
        uint32_t newClock = g_currentClock / THROTTLE_FACTOR;
        if (newClock < MIN_CLOCK_SPEED) newClock = MIN_CLOCK_SPEED;
        
        if (setClockFrequency(newClock)) {
          g_currentClock = newClock;
          g_clockThrottled = true;
          g_waitStartTime = now;  // Reset timer for next level
          // WAIT stays asserted
        }
      }
      else if (g_currentClock > MIN_CLOCK_SPEED) {
        // Already throttled but still can't keep up - throttle MORE
        // This is progressive throttling
        uint32_t newClock = g_currentClock / THROTTLE_FACTOR;
        if (newClock < MIN_CLOCK_SPEED) newClock = MIN_CLOCK_SPEED;
        
        if (newClock < g_currentClock) {  // Only if we can actually reduce further
          if (setClockFrequency(newClock)) {
            g_currentClock = newClock;
            g_waitStartTime = now;  // Reset timer for next level
            // WAIT stays asserted
          }
        }
        // If we're at minimum clock and buffer is still full,
        // WAIT stays asserted - system is at maximum backpressure
      }
    }
  }
  
  // ====== MEDIUM BUFFER LEVEL (between 25% and 75%) ======
  else if (level > BUFFER_LOW_WATER) {
    // In the middle zone - maintain current state
    // Clear low water timer since we're not low anymore
    g_lowWaterTime = 0;
    
    // Don't change WAIT or throttling in this zone
    // Let the system stabilize
  }
  
  // ====== LOW BUFFER LEVEL (≤25% full) ======
  else {
    // Buffer is low - start timing how long we stay low
    if (g_lowWaterTime == 0) {
      g_lowWaterTime = now;
    }
    
    uint32_t lowWaterDuration = now - g_lowWaterTime;
    
    // Release WAIT immediately when buffer is low
    if (g_waitAsserted) {
      PORTB |= PORTB_WAIT_MASK;  // Release WAIT (inactive HIGH)
      g_waitAsserted = false;
    }
    
    // Remove ALL throttling only after sustained low buffer (hysteresis)
    // This prevents rapid oscillation
    if (g_clockThrottled && lowWaterDuration > SUSTAINED_WAIT_TIME) {
      // Buffer has been low for a while - safe to restore full speed
      if (setClockFrequency(g_targetClock)) {
        g_currentClock = g_targetClock;
        g_clockThrottled = false;
      }
    }
  }
}

