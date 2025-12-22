/*
 * Z80 Processor Validation Suite - Performance Measurement Module
 *
 * This module provides tools for measuring ISR performance and finding
 * maximum stable clock speeds for the Z80 validation system.
 *
 * Hardware: Arduino Mega 2560 + Z80 CPU
 * Date: November 11, 2025
 */

// Performance measurement code gets good optimization but not maximum
#pragma GCC optimize("O2", "flto")

#include "performance.h"
#include "hal.h"
#include "clock.h"
#include "isr_test_programs.h"

// Global baseline for read ISR call count (captured from first 100KHz test)
static uint32_t g_readBaselineCalls = 0;

// Global baseline for write ISR call count (captured from 100KHz write test)
static uint32_t g_writeBaselineCalls = 0;

/**
 * Format frequency in human-readable units (Hz, KHz, MHz, GHz)
 * 
 * @param frequency Frequency in Hz
 * @return Formatted string (e.g., "500 KHz", "2.5 MHz")
 */
String formatFrequency(uint32_t frequency) {
    if (frequency >= 1000000000) {  // GHz
        float ghz = frequency / 1000000000.0;
        return String(ghz, 1) + F(" GHz");
    } else if (frequency >= 1000000) {  // MHz
        float mhz = frequency / 1000000.0;
        return String(mhz, 1) + F(" MHz");
    } else if (frequency >= 1000) {  // KHz
        float khz = frequency / 1000.0;
        return String(khz, 1) + F(" KHz");
    } else {  // Hz
        return String(frequency) + F(" Hz");
    }
}

/*
 * INTERRUPT DETECTION METHODOLOGY:
 * ===============================
 * 
 * The performance tests detect missed interrupts by comparing expected vs actual ISR calls:
 * 
 * 1. Capture baseline ISR calls from the first 100KHz test in each run
 * 2. Scale expected calls proportionately: calls = (frequency * baseline) / 100000
 * 3. Flag as failed if actual calls are outside ±15% tolerance (ISR too slow)
 * 
 * For NOP loop (read test):
 * - Baseline captured dynamically from 100KHz test
 * - Tolerance: ±15% for reasonable timing variations
 * 
 * For write loop (read/write test):
 * - Conservative estimate: frequency / 8
 * - Tolerance: ±15% for complex timing variations
 * 
 * RFSH cycles are automatically excluded by the ISR.
 * 
 * This detects when the ISR is not returning in time to service all interrupts.
 */

/**
 * Test read-only performance using NOP loop
 */
bool testReadPerformance(uint32_t frequency) {
    Serial.print(F("Testing at "));
    Serial.print(formatFrequency(frequency));
    Serial.println(F(""));
    Serial.println(F("----------------------------------------"));

    // Ensure capture is OFF for read-only testing (no backpressure)
    clearCaptureBuffer();
    setCaptureMode(CAPTURE_OFF);

    // Set the test frequency
    setClockFrequency(frequency);
    
    // Use the existing read test function with NOP loop
    // Run for 1 second (1000ms) to get good measurement
    bool result = runSingleReadTest(F("READ Performance Test"), 
                                   TEST_PROG_NOP_LOOP, 
                                   TEST_PROG_NOP_LOOP_SIZE, 
                                   1000);
    
    // Additional validation: Check for missed interrupts
    if (result) {
        noInterrupts();
        uint32_t actualCalls = g_readIsrCount;
        uint32_t errors = g_isrErrorCount;
        interrupts();
        
        // Capture baseline from first 100KHz test
        if (frequency == 100000 && g_readBaselineCalls == 0) {
            g_readBaselineCalls = actualCalls;
            Serial.print(F("  📊 BASELINE CAPTURED: "));
            Serial.print(actualCalls);
            Serial.println(F(" read ISR calls at 100KHz"));
        }
        
        // Calculate expected ISR calls using captured baseline
        // Use 64-bit arithmetic to avoid overflow: 100000 * 45458 = 4.5 billion > uint32_t max
        uint64_t temp = (uint64_t)frequency * g_readBaselineCalls;
        uint32_t expectedCalls = temp / 100000;
        
        // Allow ±15% margin for timing variations (reasonable tolerance)
        uint32_t minAcceptable = expectedCalls * 85 / 100;
        uint32_t maxAcceptable = expectedCalls * 115 / 100;
        
        Serial.print(F("  Expected ISR calls: ~"));
        Serial.println(expectedCalls);
        Serial.print(F("  Actual ISR calls: "));
        Serial.println(actualCalls);
        
        if (actualCalls < minAcceptable) {
            Serial.print(F("  ⚠️  MISSED INTERRUPTS DETECTED: "));
            Serial.print(expectedCalls - actualCalls);
            Serial.println(F(" calls short of expected (ISR too slow)"));
            result = false;
        } else if (actualCalls > maxAcceptable) {
            Serial.println(F("  ℹ️  Note: More calls than expected (possible timing variation)"));
        } else {
            Serial.println(F("  ✓ Interrupt timing within expected range"));
        }
    }
    
    return result;
}

/**
 * Test read/write performance using write loop
 */
bool testWritePerformance(uint32_t frequency) {
    Serial.print(F("Testing at "));
    Serial.print(formatFrequency(frequency));
    Serial.println(F(""));
    Serial.println(F("----------------------------------------"));

    // Ensure capture is OFF for performance testing (no backpressure)
    clearCaptureBuffer();
    setCaptureMode(CAPTURE_OFF);

    // Set the test frequency
    setClockFrequency(frequency);
    
    // Use the existing write test function with write loop
    // Run for 1 second (1000ms) to get good measurement
    // The write loop writes to address 0x8000, so expect that
    bool result = runSingleWriteTest(F("READ/WRITE Performance Test"),
                                    TEST_PROG_WRITE_LOOP,
                                    TEST_PROG_WRITE_LOOP_SIZE,
                                    1000,
                                    0x8000,  // Expected address (write loop writes here)
                                    0xAA);   // Expected data (write loop starts with 0xAA)
    
    // Additional validation: Check for missed interrupts
    if (result) {
        noInterrupts();
        uint32_t actualCalls = g_readIsrCount + g_writeIsrCount;
        uint32_t errors = g_isrErrorCount;
        interrupts();
        
        // Capture baseline from first 100KHz write test
        if (frequency == 100000 && g_writeBaselineCalls == 0) {
            g_writeBaselineCalls = actualCalls;
            Serial.print(F("  📊 WRITE BASELINE CAPTURED: "));
            Serial.print(actualCalls);
            Serial.println(F(" total ISR calls at 100KHz"));
        }
        
        // Calculate expected ISR calls using captured write baseline
        uint64_t temp = (uint64_t)frequency * g_writeBaselineCalls;
        uint32_t expectedCalls = temp / 100000;
        
        // Allow ±15% margin for complex instruction timing variations
        uint32_t minAcceptable = expectedCalls * 85 / 100;
        uint32_t maxAcceptable = expectedCalls * 115 / 100;
        
        Serial.print(F("  Expected ISR calls: ~"));
        Serial.println(expectedCalls);
        Serial.print(F("  Actual ISR calls: "));
        Serial.println(actualCalls);
        
        if (actualCalls < minAcceptable) {
            Serial.print(F("  ⚠️  MISSED INTERRUPTS DETECTED: "));
            Serial.print(expectedCalls - actualCalls);
            Serial.println(F(" calls short of expected (ISR too slow)"));
            result = false;
        } else if (actualCalls > maxAcceptable) {
            Serial.println(F("  ℹ️  Note: More calls than expected (possible timing variation)"));
        } else {
            Serial.println(F("  ✓ Interrupt timing within expected range"));
        }
    }
    
    return result;
}

/**
 * Measure ISR performance at various clock speeds
 */
void measureISRPerformance() {
    Serial.println();
    Serial.println(F("========================================"));
    Serial.println(F("    ISR PERFORMANCE MEASUREMENT"));
    Serial.println(F("========================================"));
    Serial.println();

    // Clear any residual capture state and reset baseline
    setCaptureMode(CAPTURE_OFF);
    clearCaptureBuffer();
    g_readBaselineCalls = 0;  // Reset baseline for new test run

    Serial.println(F("Testing ISR performance at various clock speeds..."));
    Serial.println();

    // Test speeds: 100kHz, 500kHz, 1MHz, 2MHz
    uint32_t testSpeeds[] = {100000, 500000, 1000000, 2000000};
    int numSpeeds = 4;

    Serial.println(F("READ PERFORMANCE (NOP Loop):"));
    Serial.println();

    for (int i = 0; i < numSpeeds; i++) {
        uint32_t freq = testSpeeds[i];
        if (!testReadPerformance(freq)) {
            Serial.print(F("Stopping tests - errors detected at "));
            Serial.print(formatFrequency(freq));
            Serial.println(F(""));
            break;
        }
        Serial.println();
    }

    Serial.println(F("READ/WRITE PERFORMANCE (Write Loop):"));
    Serial.println();

    g_writeBaselineCalls = 0;  // Reset baseline for write tests

    for (int i = 0; i < numSpeeds; i++) {
        uint32_t freq = testSpeeds[i];
        if (!testWritePerformance(freq)) {
            Serial.print(F("Stopping tests - errors detected at "));
            Serial.print(formatFrequency(freq));
            Serial.println(F(""));
            break;
        }
        Serial.println();
    }

    Serial.println(F("Performance measurement complete."));
    Serial.println();
}

/**
 * Find the maximum stable clock speed
 */
void findMaximumSpeed() {
    Serial.println();
    Serial.println(F("========================================"));
    Serial.println(F("    FINDING MAXIMUM CLOCK SPEED"));
    Serial.println(F("========================================"));
    Serial.println();

    // Clear any residual capture state and reset baseline
    setCaptureMode(CAPTURE_OFF);
    clearCaptureBuffer();
    g_readBaselineCalls = 0;  // Reset baseline for new test run
    g_writeBaselineCalls = 0;  // Reset write baseline for new test run

    Serial.println(F("Sweeping clock speeds from 1MHz to 4MHz..."));
    Serial.println();

    uint32_t maxReadSpeed = 0;
    uint32_t maxWriteSpeed = 0;

    // Test read performance
    Serial.println(F("Testing READ performance:"));
    for (uint32_t freq = 1000000; freq <= 4000000; freq += 100000) {
        Serial.print(F("  "));
        Serial.print(formatFrequency(freq));
        Serial.print(F("... "));
        if (testReadPerformance(freq)) {
            maxReadSpeed = freq;
            Serial.println(F("OK"));
        } else {
            Serial.println(F("FAILED"));
            break;
        }
    }

    Serial.println();
    Serial.println(F("Testing READ/WRITE performance:"));
    for (uint32_t freq = 1000000; freq <= 4000000; freq += 100000) {
        Serial.print(F("  "));
        Serial.print(formatFrequency(freq));
        Serial.print(F("... "));
        if (testWritePerformance(freq)) {
            maxWriteSpeed = freq;
            Serial.println(F("OK"));
        } else {
            Serial.println(F("FAILED"));
            break;
        }
    }

    Serial.println();
    Serial.print(F("Maximum READ speed: "));
    Serial.println(formatFrequency(maxReadSpeed));
    Serial.print(F("Maximum READ/WRITE speed: "));
    Serial.println(formatFrequency(maxWriteSpeed));
    Serial.println();
}

/**
 * Test performance with specific cache size
 */
bool testCacheSize(uint16_t cacheSize, uint32_t frequency) {
    Serial.print(F("Testing cache size "));
    Serial.print(cacheSize);
    Serial.print(F(" bytes at "));
    Serial.print(formatFrequency(frequency));
    Serial.println(F(""));
    Serial.println(F("----------------------------------------"));

    // Set cache size
    if (!setCacheSize(cacheSize)) {
        Serial.println(F("  Failed to set cache size"));
        return false;
    }

    // Set the test frequency
    setClockFrequency(frequency);

    bool readResult = false;
    bool writeResult = false;

    // Calculate expected minimum operations for 1-second test
    // At 500kHz, expect at least 100,000 operations for meaningful performance test
    uint32_t minExpectedOps = (frequency / 1000) * 200; // 200ms worth of operations minimum

    // Test read performance with NOP loop
    Serial.println(F("  Testing READ performance..."));
    setCaptureMode(CAPTURE_OFF);  // No capture for read testing
    clearCaptureBuffer();  // Clear any residual data
    readResult = runSingleReadTest(F("Cache READ Test"), 
                                  TEST_PROG_NOP_LOOP, 
                                  TEST_PROG_NOP_LOOP_SIZE, 
                                  1000); // 1 second test    // Validate read performance had sufficient operations
    if (readResult) {
        noInterrupts();
        uint32_t readOps = g_readIsrCount;
        uint32_t readErrors = g_isrErrorCount;
        interrupts();

        if (readOps < minExpectedOps) {
            Serial.print(F("  ⚠️  Insufficient read operations: "));
            Serial.print(readOps);
            Serial.print(F(" < "));
            Serial.println(minExpectedOps);
            readResult = false;
        } else {
            Serial.print(F("  ✓ Read operations: "));
            Serial.println(readOps);
        }
    }

    // Test write performance with write loop
    Serial.println(F("  Testing READ/WRITE performance..."));
    setCaptureMode(CAPTURE_BUFFERED);  // Enable capture for write testing
    clearCaptureBuffer();  // Clear buffer before write test
    writeResult = runSingleWriteTest(F("Cache READ/WRITE Test"),
                                    TEST_PROG_WRITE_LOOP,
                                    TEST_PROG_WRITE_LOOP_SIZE,
                                    1000,    // 1 second test
                                    0x8000,  // Expected address
                                    0xAA);   // Expected data

    // Validate write performance had sufficient operations
    if (writeResult) {
        noInterrupts();
        uint32_t writeOps = g_readIsrCount + g_writeIsrCount;
        uint32_t writeErrors = g_isrErrorCount;
        interrupts();

        if (writeOps < minExpectedOps) {
            Serial.print(F("  ⚠️  Insufficient write operations: "));
            Serial.print(writeOps);
            Serial.print(F(" < "));
            Serial.println(minExpectedOps);
            writeResult = false;
        } else {
            Serial.print(F("  ✓ Write operations: "));
            Serial.println(writeOps);
        }
    }

    // Return true only if both tests pass with sufficient operations
    bool overallResult = readResult && writeResult;

    if (overallResult) {
        Serial.println(F("  ✓ Cache size "));
        Serial.print(cacheSize);
        Serial.println(F(" bytes: PASSED"));
    } else {
        Serial.println(F("  ⚠️  Cache size "));
        Serial.print(cacheSize);
        Serial.println(F(" bytes: FAILED"));
    }

    return overallResult;
}/**
 * Find optimal cache size through comprehensive testing
 */
void findOptimalCacheSize() {
    Serial.println();
    Serial.println(F("========================================"));
    Serial.println(F("    CACHE SIZE OPTIMIZATION"));
    Serial.println(F("========================================"));
    Serial.println();

    // Clear any residual capture state
    setCaptureMode(CAPTURE_OFF);
    clearCaptureBuffer();

    Serial.println(F("Testing different cache sizes to find optimal balance..."));
    Serial.println();

    uint16_t testSizes[] = {128, 256, 512, 1024};
    uint32_t testFreq = 500000; // 500 kHz for testing
    int numSizes = 4;

    Serial.println(F("Performance results:"));
    Serial.println();

    for (int i = 0; i < numSizes; i++) {
        uint16_t size = testSizes[i];
        if (testCacheSize(size, testFreq)) {
            Serial.println();
        } else {
            Serial.println(F("  Test failed, continuing..."));
            Serial.println();
        }
    }

    Serial.println(F("Cache size optimization complete."));
    Serial.println(F("Recommendation: 256 bytes (good balance of speed vs memory)"));
    Serial.println();
}

/**
 * Run comprehensive performance benchmark
 */
void runPerformanceBenchmark() {
    Serial.println();
    Serial.println(F("========================================="));
    Serial.println(F("    Z80 PERFORMANCE BENCHMARK"));
    Serial.println(F("========================================="));
    Serial.println();

    // Clear any residual capture state and reset baseline
    setCaptureMode(CAPTURE_OFF);
    clearCaptureBuffer();
    g_readBaselineCalls = 0;  // Reset baseline for new test run
    g_writeBaselineCalls = 0;  // Reset write baseline for new test run

    // First, measure performance at standard speeds
    measureISRPerformance();

    // Then find maximum speed
    findMaximumSpeed();

    Serial.println(F("Benchmark complete."));
    Serial.println();
}