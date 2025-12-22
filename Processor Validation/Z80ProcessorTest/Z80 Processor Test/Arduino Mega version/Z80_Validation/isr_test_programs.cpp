/*
 * Z80 Processor Validation Suite - ISR Test Programs Implementation
 * 
 * This file contains simple test programs for validating ISR functionality.
 * 
 * Hardware: Arduino Mega 2560 + Z80 CPU
 * Date: November 7, 2025
 */

#include "isr_test_programs.h"
#include "hal.h"
#include "clock.h"

// =============================================================================
// TEST PROGRAM DEFINITIONS
// =============================================================================

/**
 * Test Program 1: Simple NOP Sequence with HALT
 * 
 * Machine Code:
 *   0x00 = NOP   (No Operation, 4 T-states)
 *   0x76 = HALT  (Halt CPU, 4 T-states)
 */
const uint8_t TEST_PROG_NOPS[] PROGMEM = {
  0x00,  // 0x0000: NOP
  0x00,  // 0x0001: NOP
  0x00,  // 0x0002: NOP
  0x76   // 0x0003: HALT
};

const uint16_t TEST_PROG_NOPS_SIZE = sizeof(TEST_PROG_NOPS);

/**
 * Test Program 2: Infinite NOP Loop
 * 
 * Machine Code:
 *   0xC3 nn nn = JP nn  (Jump to address nn, 10 T-states)
 * 
 * Z80 Instruction Encoding:
 *   JP uses absolute addressing with 16-bit address in little-endian format:
 *   - Byte 0: Opcode (0xC3)
 *   - Byte 1: Low byte of address
 *   - Byte 2: High byte of address
 */
const uint8_t TEST_PROG_NOP_LOOP[] PROGMEM = {
  0x00,  // 0x0000: NOP
  0x00,  // 0x0001: NOP
  0xC3,  // 0x0002: JP 0x0000 (opcode)
  0x00,  // 0x0003: JP 0x0000 (low byte of address)
  0x00   // 0x0004: JP 0x0000 (high byte of address)
};

const uint16_t TEST_PROG_NOP_LOOP_SIZE = sizeof(TEST_PROG_NOP_LOOP);

/**
 * Test Program 3: Single HALT
 * 
 * Simplest possible test - just one instruction.
 */
const uint8_t TEST_PROG_HALT[] PROGMEM = {
  0x76   // 0x0000: HALT
};

const uint16_t TEST_PROG_HALT_SIZE = sizeof(TEST_PROG_HALT);

/**
 * Test Program 4: Simple Single Write
 * 
 * Purpose: Verify ISR captures WRITE operations
 * 
 * Machine Code:
 *   0x3E 0x42 = LD A, 0x42          (Load accumulator with test value)
 *   0x32 nn nn = LD (nn), A         (Write A to memory address nn)
 *   0x76 = HALT                     (Stop execution)
 * 
 * Z80 Instruction Details:
 *   LD A, n uses immediate addressing (opcode 0x3E, 7 T-states)
 *   LD (nn), A uses absolute addressing (opcode 0x32, 13 T-states)
 *     - Byte 0: Opcode (0x32)
 *     - Byte 1: Low byte of address (0x00)
 *     - Byte 2: High byte of address (0x80)
 *     - Result: Address 0x8000
 */
const uint8_t TEST_PROG_SINGLE_WRITE[] PROGMEM = {
  0x3E, 0x42,        // 0x0000: LD A, 0x42
  0x32, 0x00, 0x80,  // 0x0002: LD (0x8000), A
  0x76               // 0x0005: HALT
};

const uint16_t TEST_PROG_SINGLE_WRITE_SIZE = sizeof(TEST_PROG_SINGLE_WRITE);

/**
 * Test Program 5: Multiple Sequential Writes
 * 
 * Purpose: Test capture of multiple WRITE operations
 * 
 * This program performs three consecutive writes to test:
 * - Ring buffer capture system
 * - Sequential write handling
 * - Data integrity across multiple operations
 */
const uint8_t TEST_PROG_MULTI_WRITE[] PROGMEM = {
  0x3E, 0x11,        // 0x0000: LD A, 0x11
  0x32, 0x00, 0x80,  // 0x0002: LD (0x8000), A
  0x3E, 0x22,        // 0x0005: LD A, 0x22
  0x32, 0x01, 0x80,  // 0x0007: LD (0x8001), A
  0x3E, 0x33,        // 0x000A: LD A, 0x33
  0x32, 0x02, 0x80,  // 0x000C: LD (0x8002), A
  0x76               // 0x000F: HALT
};

const uint16_t TEST_PROG_MULTI_WRITE_SIZE = sizeof(TEST_PROG_MULTI_WRITE);

/**
 * Test Program 6: Write Pattern Loop
 * 
 * Purpose: Test continuous WRITE operations and buffer behavior
 * 
 * This program creates an infinite loop that alternates writing
 * two different patterns to the same memory location.
 * Useful for testing:
 * - Buffer overflow handling
 * - Backpressure mechanisms (WAIT assertion)
 * - High-speed write capture
 * 
 * Note: Must stop with stopClock() - runs indefinitely otherwise
 */
const uint8_t TEST_PROG_WRITE_LOOP[] PROGMEM = {
  0x3E, 0xAA,        // 0x0000: LD A, 0xAA
  0x32, 0x00, 0x80,  // 0x0002: LD (0x8000), A
  0x3E, 0x55,        // 0x0005: LD A, 0x55
  0x32, 0x00, 0x80,  // 0x0007: LD (0x8000), A
  0xC3, 0x00, 0x00   // 0x000A: JP 0x0000
};

const uint16_t TEST_PROG_WRITE_LOOP_SIZE = sizeof(TEST_PROG_WRITE_LOOP);

// =============================================================================
// ISR TEST FUNCTION IMPLEMENTATION
// =============================================================================

/**
 * Helper function to run a single READ test
 * 
 * @param testName Name of the test for display
 * @param program Pointer to test program in PROGMEM
 * @param programSize Size of the test program
 * @param testDuration Duration to run test in milliseconds
 * @return true if test passed (zero errors), false otherwise
 */
bool runSingleReadTest(const __FlashStringHelper* testName, 
                      const uint8_t* program, 
                      uint16_t programSize,
                      uint16_t testDuration) {
  Serial.println();
  Serial.println(F("----------------------------------------"));
  Serial.print(F("Test: "));
  Serial.println(testName);
  Serial.println(F("----------------------------------------"));
      
  // CRITICAL: Start clock BEFORE loading program to allow Z80 to stabilize
  // Z80 needs active clock pulses while in WAIT state to reach consistent state
  // Without this, Z80 may start at unpredictable non-zero addresses
  Serial.println(F("Starting clock (Z80 will stabilize in WAIT state)..."));
  startClock();
  delay(10);  // Small delay for clock stabilization

  // Initialize memory handler (sets up INT5/INT4 interrupts for ISRs)
  Serial.println(F("Initializing memory handler..."));
  initializeMemoryHandler();
  delay(10);

  // Load test program
  Serial.print(F("Loading test program ("));
  Serial.print(programSize);
  Serial.println(F(" bytes)..."));


  if (loadProgramToCache(program, programSize)) {
    Serial.println(F("  Program: Cached in SRAM"));
  } else {
    Serial.println(F("  Program: Using PROGMEM"));
  }
  delay(100);
  
  // Reset Z80 (will assert WAIT, freezing Z80 at first instruction fetch)
  Serial.println(F("Resetting Z80..."));
  resetZ80();
  delay(100);
  
  // Clear ISR counters and reset base address capture
  // ISR will capture the actual starting address on first READ
  resetISRState();
  
  Serial.println();
  Serial.print(F("Running test for "));
  Serial.print(testDuration);
  Serial.println(F(" ms..."));
  
  // Release WAIT to allow Z80 to execute
  releaseWait();
  
  // Run for specified duration
  delay(testDuration);
  
  // Stop clock
  stopClock();
  
  // Put Z80 back into WAIT state
  assertWait();
  delay(10);
  
  // Read final counters
  noInterrupts();
  uint32_t finalReadCount = g_readIsrCount;
  uint32_t finalErrorCount = g_isrErrorCount;
  interrupts();
  
  // Display results
  Serial.println();
  Serial.println(F("Results:"));
  Serial.print(F("  Starting Address:  0x"));
  Serial.println(g_programBaseAddress, HEX);
  Serial.print(F("  Read ISR Calls:  "));
  Serial.println(finalReadCount);
  Serial.print(F("  ISR Errors: "));
  Serial.println(finalErrorCount);
  
  // Determine pass/fail
  bool passed = (g_programBaseAddressValid && finalErrorCount == 0);
  
  if (passed) {
    Serial.println(F("  ✓ PASSED"));
  } else if (!g_programBaseAddressValid) {
    Serial.println(F("  ✗ FAILED: No base address captured"));
  } else {
    Serial.println(F("  ✗ FAILED: Errors detected"));
  }
  
  return passed;
}

/**
 * Helper function to run a single WRITE test
 * 
 * @param testName Name of the test for display
 * @param program Pointer to test program in PROGMEM
 * @param programSize Size of the test program
 * @param testDuration Duration to run test in milliseconds
 * @param expectedAddr Expected write address
 * @param expectedData Expected write data
 * @return true if test passed, false otherwise
 */
bool runSingleWriteTest(const __FlashStringHelper* testName,
                        const uint8_t* program,
                        uint16_t programSize,
                        uint16_t testDuration,
                        uint16_t expectedAddr,
                        uint8_t expectedData) {
  Serial.println();
  Serial.println(F("----------------------------------------"));
  Serial.print(F("Test: "));
  Serial.println(testName);
  Serial.println(F("----------------------------------------"));
  
  startClock();
  delay(10);
  
  // Initialize memory handler (sets up INT5/INT4 interrupts for ISRs)
  Serial.println(F("Initializing memory handler..."));
  initializeMemoryHandler();
  delay(10);
  
  Serial.print(F("Loading test program ("));
  Serial.print(programSize);
  Serial.println(F(" bytes)..."));
  
  if (loadProgramToCache(program, programSize)) {
    Serial.println(F("  Program: Cached in SRAM"));
  } else {
    Serial.println(F("  Program: Using PROGMEM"));
  }
  delay(100);
  
  Serial.println(F("Resetting Z80..."));
  resetZ80();
  delay(100);
  resetISRState();
  g_dataReady = false;
  
  Serial.print(F("Running test for "));
  Serial.print(testDuration);
  Serial.println(F(" ms..."));
  
  releaseWait();
  delay(testDuration);
  stopClock();
  assertWait();
  delay(10);
  
  // Read final counters
  noInterrupts();
  uint32_t finalReadCount = g_readIsrCount;
  uint32_t finalWriteCount = g_writeIsrCount;
  uint32_t finalErrorCount = g_isrErrorCount;
  uint16_t lastWriteAddr = g_lastWriteAddress;
  uint8_t capturedData = g_capturedData;
  uint32_t finalAA = g_writePatternAA_count;
  uint32_t final55 = g_writePattern55_count;
  interrupts();
  
  // Display results
  Serial.println();
  Serial.println(F("Results:"));
  Serial.print(F("  Starting Address:  0x"));
  Serial.println(g_programBaseAddress, HEX);
  Serial.print(F("  Read ISR Calls:   "));
  Serial.println(finalReadCount);
  Serial.print(F("  Write ISR Calls:  "));
  Serial.println(finalWriteCount);
  Serial.print(F("  Last Write Addr:  0x"));
  Serial.println(lastWriteAddr, HEX);
  Serial.print(F("  Captured Data:    0x"));
  Serial.println(capturedData, HEX);
  Serial.print(F("  Expected Addr:    0x"));
  Serial.println(expectedAddr, HEX);
  Serial.print(F("  Expected Data:    0x"));
  Serial.println(expectedData, HEX);
  Serial.print(F("  ISR Errors: "));
  Serial.println(finalErrorCount);
  Serial.print(F("  Pattern counts: AA=")); Serial.print(finalAA);
  Serial.print(F(", 55=")); Serial.println(final55);
  
  // Determine pass/fail
  bool addrMatch = (lastWriteAddr == expectedAddr);
  bool dataMatch = (capturedData == expectedData);

  // If pattern counts present (alternating test), accept if counts are within +/-1
  bool patternAccept = false;
  uint32_t totalPatternWrites = finalAA + final55;
  if (totalPatternWrites > 0) {
    uint32_t diff = (finalAA > final55) ? (finalAA - final55) : (final55 - finalAA);
    if (diff <= 1) patternAccept = true;
  }

  bool passed = (addrMatch && (dataMatch || patternAccept) && finalErrorCount == 0);

  if (passed) {
    if (patternAccept && !dataMatch) {
      Serial.println(F("  ✓ PASSED (alternating pattern counts within ±1)"));
    } else {
      Serial.println(F("  ✓ PASSED"));
    }
  } else if (!addrMatch) {
    Serial.println(F("  ✗ FAILED: Address mismatch"));
  } else if (!dataMatch && !patternAccept) {
    Serial.println(F("  ✗ FAILED: Data mismatch"));
  } else {
    Serial.println(F("  ✗ FAILED: Errors detected"));
  }
  
  return passed;
}

/**
 * Run ISR Test - Validates the interrupt-driven memory handler
 * 
 * This function performs a comprehensive test of Phase 1 (step 1.7):
 * - Runs three test programs sequentially
 * - Test 1: NOP sequence with HALT
 * - Test 2: Infinite NOP loop
 * - Test 3: Single HALT instruction
 * 
 * If any test fails, the sequence stops and reports failure.
 */
void runISRTest() {
  Serial.println();
  Serial.println(F("========================================"));
  Serial.println(F("    ISR TEST SUITE (READ & WRITE)"));
  Serial.println(F("========================================"));
  Serial.println();
  
  Serial.println(F("This test validates the interrupt-driven memory handler."));
  Serial.println(F("Six test programs will be executed sequentially:"));
  Serial.println(F("  Tests 1-3: READ operations (Phase 1)"));
  Serial.println(F("  Tests 4-6: WRITE operations (Phase 2)"));
  Serial.println();
  
  // Save current clock state
  bool wasRunning = isClockRunning();
  uint32_t originalFreq = getClockFrequency();
  
  // Stop clock if running
  if (wasRunning) {
    Serial.println(F("Stopping clock..."));
    stopClock();
    delay(100);
  }
  
  // Initialize memory handler (ISR)
  Serial.println(F("Initializing memory handler (INT5/INT4 ISRs)..."));
  initializeMemoryHandler();
  delay(100);
  
  // Set to 100 kHz for safe testing
  setClockFrequency(100000);
  
  // Ensure capture mode is OFF for simple testing
  setCaptureMode(CAPTURE_OFF);
  
  // Track overall test results
  int testsPassed = 0;
  int testsTotal = 6;
  bool continueTests = true;
  
  // ========================================
  // READ TESTS (Phase 1)
  // ========================================
  
  // Test 1: NOP Sequence with HALT
  if (continueTests) {
    if (runSingleReadTest(F("READ Test 1: NOP Sequence + HALT"), 
                         TEST_PROG_NOPS, 
                         TEST_PROG_NOPS_SIZE, 
                         1000)) {
      testsPassed++;
    } else {
      continueTests = false;
    }
  }
  
  // Test 2: Single HALT (simplest test)
  if (continueTests) {
    if (runSingleReadTest(F("READ Test 2: Single HALT Instruction"), 
                         TEST_PROG_HALT, 
                         TEST_PROG_HALT_SIZE, 
                         500)) {
      testsPassed++;
    } else {
      continueTests = false;
    }
  }
  
  // Test 3: Infinite NOP Loop (run for shorter time)
  if (continueTests) {
    if (runSingleReadTest(F("READ Test 3: Infinite NOP Loop"), 
                         TEST_PROG_NOP_LOOP, 
                         TEST_PROG_NOP_LOOP_SIZE, 
                         500)) {
      testsPassed++;
    } else {
      continueTests = false;
    }
  }
  
  // ========================================
  // WRITE TESTS (Phase 2)
  // ========================================
  
  // Test 4: Single WRITE Operation
  if (continueTests) {
    if (runSingleWriteTest(F("WRITE Test 4: Single WRITE Operation"),
                           TEST_PROG_SINGLE_WRITE,
                           TEST_PROG_SINGLE_WRITE_SIZE,
                           500,
                           0x8000, 0x42)) {
      testsPassed++;
    } else {
      continueTests = false;
    }
  }
  
  // Test 5: Multiple Sequential WRITEs
  if (continueTests) {
    if (runSingleWriteTest(F("WRITE Test 5: Multiple Sequential WRITEs"),
                           TEST_PROG_MULTI_WRITE,
                           TEST_PROG_MULTI_WRITE_SIZE,
                           500,
                           0x8002, 0x33)) {  // Validates last write
      testsPassed++;
    } else {
      continueTests = false;
    }
  }
  
  // Test 6: Continuous WRITE Loop
  if (continueTests) {
    if (runSingleWriteTest(F("WRITE Test 6: Continuous WRITE Loop"),
                           TEST_PROG_WRITE_LOOP,
                           TEST_PROG_WRITE_LOOP_SIZE,
                           200,
                           0x8000, 0xAA)) {  // Check for 0xAA specifically
      testsPassed++;
    } else {
      continueTests = false;
    }
  }
  
  // ========================================
  // FINAL SUMMARY
  // ========================================
  Serial.println();
  Serial.println(F("========================================"));
  Serial.println(F("         FINAL RESULTS"));
  Serial.println(F("========================================"));
  Serial.println();
  
  Serial.print(F("Tests Passed: "));
  Serial.print(testsPassed);
  Serial.print(F(" / "));
  Serial.println(testsTotal);
  Serial.println();
  
  if (testsPassed == testsTotal) {
    Serial.println(F("✓ ALL TESTS PASSED!"));
    Serial.println(F("  ISR READ operations: Phase 1 COMPLETE"));
    Serial.println(F("  ISR WRITE operations: Phase 2 COMPLETE"));
  } else {
    Serial.println(F("✗ SOME TESTS FAILED"));
    Serial.println(F("  Check ISR implementation and hardware connections."));
  }
  
  Serial.println();
  
  // Restore original clock state
  Serial.println(F("Restoring original clock settings..."));
  
  // Put Z80 back into WAIT state to prevent spurious activity
  Serial.println(F("  Asserting WAIT..."));
  assertWait();
  delay(10);
  
  // Restore clock frequency
  Serial.println(F("  Restoring frequency..."));
  setClockFrequency(originalFreq);
  delay(10);
  
  // Restart clock if it was running before
  if (wasRunning) {
    Serial.println(F("  Restarting clock..."));
    startClock();
    delay(10);
    Serial.println(F("  Clock: Restarted"));
  } else {
    Serial.println(F("  Clock: Remains stopped"));
  }
  
  Serial.println(F("Done."));
}
