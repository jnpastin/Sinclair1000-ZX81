/*
 * Z80 Processor Validation Suite - Performance Measurement Header
 *
 * Function declarations for ISR performance testing and optimization.
 *
 * Hardware: Arduino Mega 2560 + Z80 CPU
 * Date: November 11, 2025
 */

#ifndef PERFORMANCE_H
#define PERFORMANCE_H

#include <Arduino.h>

/**
 * Measure ISR performance at various clock speeds
 *
 * Tests ISR stability and throughput at 100kHz, 500kHz, 1MHz, and 2MHz
 * using both read-only (NOP loop) and read/write (write loop) test programs.
 * Reports ISR call counts, error counts, and identifies maximum stable speed.
 */
void measureISRPerformance();

/**
 * Test read-only performance using NOP loop
 *
 * Measures ISR performance with pure instruction fetch operations.
 * Uses TEST_PROG_NOP_LOOP for continuous read operations.
 *
 * @param frequency Clock frequency to test in Hz
 * @return true if test completed without errors, false otherwise
 */
bool testReadPerformance(uint32_t frequency);

/**
 * Test read/write performance using write loop
 *
 * Measures ISR performance with mixed read (instruction fetch) and write (data store) operations.
 * Uses TEST_PROG_WRITE_LOOP for continuous read/write operations.
 * Enables capture system to track write operations.
 *
 * @param frequency Clock frequency to test in Hz
 * @return true if test completed without errors, false otherwise
 */
bool testWritePerformance(uint32_t frequency);

/**
 * Find the maximum stable clock speed for the Z80 system
 *
 * Sweeps clock frequencies from 1MHz to 4MHz in 100kHz increments.
 * Tests both read and read/write performance at each speed.
 * Returns the highest frequency with zero errors and adequate ISR activity.
 */
void findMaximumSpeed();

/**
 * Run comprehensive performance benchmark
 *
 * Combines performance measurement and maximum speed finding into
 * a single comprehensive test suite.
 */
void runPerformanceBenchmark();

/**
 * Test performance with specific cache size
 *
 * Configures cache to specified size and measures read performance.
 * Tests both cache hit (program fits) and cache miss (program too big) scenarios.
 *
 * @param cacheSize Cache size to test (128, 256, 512, 1024 bytes)
 * @param frequency Clock frequency to test at
 * @return true if test completed successfully, false otherwise
 */
bool testCacheSize(uint16_t cacheSize, uint32_t frequency);

/**
 * Find optimal cache size through comprehensive testing
 *
 * Tests different cache sizes with various program sizes to find
 * the optimal balance between performance and memory usage.
 * Measures ISR performance, cache hit rates, and memory efficiency.
 */
void findOptimalCacheSize();

#endif // PERFORMANCE_H