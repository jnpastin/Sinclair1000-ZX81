/*
 * Z80 Processor Validation Suite - ISR Test Programs
 * 
 * This file contains simple Z80 test programs stored in PROGMEM (flash memory)
 * for validating the ISR and memory emulation functionality.
 * 
 * These programs test the ISR's ability to handle Z80 memory requests,
 * not the Z80 processor's instruction execution (that comes later).
 * 
 * All programs are designed to be small (<256 bytes) for SRAM caching.
 * 
 * Hardware: Arduino Mega 2560 + Z80 CPU
 * Date: November 7, 2025
 */

#ifndef ISR_TEST_PROGRAMS_H
#define ISR_TEST_PROGRAMS_H

#include <Arduino.h>

// =============================================================================
// TEST PROGRAM DECLARATIONS
// =============================================================================

/**
 * Test Program 1: Simple NOP Sequence with HALT
 * 
 * Purpose: Verify ISR handles basic READ operations
 * Size: 4 bytes
 * 
 * Expected Behavior:
 * - Z80 fetches and executes 3 NOPs (no operation)
 * - Z80 fetches and executes HALT
 * - Z80 stops and HALT signal goes active
 * 
 * Verification:
 * - Check HALT signal is active (LOW)
 * - PC should be at 0x0004
 * - ISR call count should be at least 4 (could be more with refresh cycles)
 */
extern const uint8_t TEST_PROG_NOPS[] PROGMEM;
extern const uint16_t TEST_PROG_NOPS_SIZE;

/**
 * Test Program 2: Infinite NOP Loop
 * 
 * Purpose: Verify continuous operation and jump instructions
 * Size: 5 bytes
 * 
 * Expected Behavior:
 * - Z80 executes NOP at 0x0000
 * - Z80 executes NOP at 0x0001
 * - Z80 executes JP 0x0000 at 0x0002 (3-byte instruction)
 * - Z80 jumps back to 0x0000
 * - Loop repeats indefinitely
 * 
 * Verification:
 * - ISR call count continuously increases
 * - PC cycles through 0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0000...
 * - No HALT signal
 * 
 * Note: Stop with stopClock() to end test
 */
extern const uint8_t TEST_PROG_NOP_LOOP[] PROGMEM;
extern const uint16_t TEST_PROG_NOP_LOOP_SIZE;

/**
 * Test Program 3: Single HALT
 * 
 * Purpose: Minimal test - verify Z80 can fetch and execute one instruction
 * Size: 1 byte
 * 
 * Expected Behavior:
 * - Z80 fetches HALT at 0x0000
 * - Z80 immediately halts
 * - HALT signal goes active
 * 
 * Verification:
 * - Check HALT signal is active (LOW)
 * - PC should be at 0x0001
 * - ISR call count should be at least 1
 * 
 * Note: Fastest test for basic ISR verification
 */
extern const uint8_t TEST_PROG_HALT[] PROGMEM;
extern const uint16_t TEST_PROG_HALT_SIZE;

/**
 * Test Program 4: Simple Single Write
 * 
 * Purpose: Verify ISR captures WRITE operations
 * Size: 6 bytes
 * 
 * Machine Code:
 *   LD A, 0x42          ; 0x3E 0x42 - Load accumulator with test value
 *   LD (0x8000), A      ; 0x32 0x00 0x80 - Write A to memory address 0x8000
 *   HALT                ; 0x76 - Stop execution
 * 
 * Expected Behavior:
 * - Z80 loads 0x42 into register A
 * - Z80 writes 0x42 to address 0x8000
 * - Z80 halts
 * 
 * Verification:
 * - One WRITE captured to address 0x8000
 * - Data should be 0x42
 * - WR signal should have been active during write
 */
extern const uint8_t TEST_PROG_SINGLE_WRITE[] PROGMEM;
extern const uint16_t TEST_PROG_SINGLE_WRITE_SIZE;

/**
 * Test Program 5: Multiple Sequential Writes
 * 
 * Purpose: Test capture of multiple WRITE operations
 * Size: 15 bytes
 * 
 * Machine Code:
 *   LD A, 0x11          ; 0x3E 0x11
 *   LD (0x8000), A      ; 0x32 0x00 0x80
 *   LD A, 0x22          ; 0x3E 0x22
 *   LD (0x8001), A      ; 0x32 0x01 0x80
 *   LD A, 0x33          ; 0x3E 0x33
 *   LD (0x8002), A      ; 0x32 0x02 0x80
 *   HALT                ; 0x76
 * 
 * Expected Behavior:
 * - Z80 writes 0x11 to 0x8000
 * - Z80 writes 0x22 to 0x8001
 * - Z80 writes 0x33 to 0x8002
 * - Z80 halts
 * 
 * Verification:
 * - Three WRITEs captured (requires ring buffer or sequential polling)
 * - Address sequence: 0x8000, 0x8001, 0x8002
 * - Data sequence: 0x11, 0x22, 0x33
 * 
 * Note: Tests ring buffer capture system (Phase 5)
 */
extern const uint8_t TEST_PROG_MULTI_WRITE[] PROGMEM;
extern const uint16_t TEST_PROG_MULTI_WRITE_SIZE;

/**
 * Test Program 6: Write Pattern Loop
 * 
 * Purpose: Test continuous WRITE operations and buffer behavior
 * Size: 13 bytes
 * 
 * Machine Code:
 *   0x0000: LD A, 0xAA       ; 0x3E 0xAA - Load pattern 0xAA
 *   0x0002: LD (0x8000), A   ; 0x32 0x00 0x80 - Write to 0x8000
 *   0x0005: LD A, 0x55       ; 0x3E 0x55 - Load pattern 0x55
 *   0x0007: LD (0x8000), A   ; 0x32 0x00 0x80 - Write to 0x8000
 *   0x000A: JP 0x0000        ; 0xC3 0x00 0x00 - Jump to start
 * 
 * Expected Behavior:
 * - Z80 alternately writes 0xAA and 0x55 to address 0x8000
 * - Loop continues indefinitely until clock is stopped
 * 
 * Verification:
 * - Continuous WRITEs to 0x8000
 * - Data alternates between 0xAA and 0x55
 * - Tests buffer overflow handling and backpressure
 * 
 * Note: Stop with stopClock() to end test
 *       Tests hybrid backpressure system (Phase 5)
 */
extern const uint8_t TEST_PROG_WRITE_LOOP[] PROGMEM;
extern const uint16_t TEST_PROG_WRITE_LOOP_SIZE;

// =============================================================================
// ISR TEST FUNCTION
// =============================================================================

/**
 * Run ISR Test
 * 
 * Purpose: Validate READ and WRITE operation handling
 * 
 * This function:
 * 1. Initializes the memory handler (INT4 ISR)
 * 2. Tests six scenarios sequentially:
 *    - Tests 1-3: READ operations (Phase 1)
 *    - Tests 4-6: WRITE operations (Phase 2)
 * 3. Displays validation results with pass/fail
 * 4. Restores original clock settings
 * 
 * Expected Results:
 * - All six tests should pass (6/6)
 * - READ operations work correctly
 * - WRITE operations are captured with correct address and data
 * - No errors or bus contention
 * 
 * This test validates Phase 1 (READ) and Phase 2 (WRITE) of the ISR implementation plan.
 */
void runISRTest();

/**
 * Run Single Read Test
 *
 * Tests a single read operation with specified program and duration.
 * Used for performance testing and validation.
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
                      uint16_t testDuration);

/**
 * Run Single Write Test
 *
 * Tests a single write operation with specified program, duration, and expected results.
 * Used for performance testing and validation.
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
                        uint8_t expectedData);

#endif // TEST_PROGRAMS_H
