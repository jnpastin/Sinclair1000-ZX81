/*
 * Z80 Processor Validation Suite - Clock Generation Module
 * 
 * This module provides hardware-based clock generation for the Z80 CPU using
 * Arduino Mega 2560's Timer1. The clock is generated on pin 11 (PB5/OC1A).
 * 
 * Features:
 * - Configurable frequency: 100 KHz - 3 MHz
 * - Default: 500 KHz with auto-start on initialization
 * - Hardware PWM for jitter-free, stable output
 * - 50% duty cycle for Z80 compatibility
 * - Runtime frequency adjustment
 * - Start/stop control for debugging
 * - Single-step mode for instruction-level debugging
 * - State query functions
 * 
 * Hardware: Arduino Mega 2560, Timer1, Pin 11 (OC1A)
 * Date: November 6, 2025
 */

#ifndef CLOCK_H
#define CLOCK_H

#include <Arduino.h>

// =============================================================================
// CLOCK FREQUENCY PRESETS
// =============================================================================
#define CLOCK_FREQ_100KHZ   100000UL
#define CLOCK_FREQ_500KHZ   500000UL  // DEFAULT
#define CLOCK_FREQ_1MHZ    1000000UL
#define CLOCK_FREQ_2MHZ    2000000UL
#define CLOCK_FREQ_3MHZ    3000000UL

// =============================================================================
// CLOCK FREQUENCY LIMITS
// =============================================================================
#define CLOCK_FREQ_MIN      100000UL  // 100 KHz minimum
#define CLOCK_FREQ_MAX     3000000UL  // 3 MHz maximum
#define CLOCK_FREQ_DEFAULT  CLOCK_FREQ_500KHZ

// =============================================================================
// CLOCK CONFIGURATION STRUCTURE
// =============================================================================
// Stores the current clock configuration and state
struct ClockConfig {
  uint32_t frequency;     // Configured frequency in Hz
  uint16_t prescaler;     // Timer1 prescaler value (1, 8, 64, 256, 1024)
  uint16_t topValue;      // Timer1 ICR1 TOP value
  bool running;           // true if clock output is active
};

// =============================================================================
// PUBLIC FUNCTIONS
// =============================================================================

/**
 * Initialize clock to default frequency (500 KHz) and start automatically
 * This is the recommended initialization for normal operation.
 * Call this in setup() to start the Z80 clock immediately.
 */
void initializeClock();

/**
 * Initialize clock to specific frequency and start automatically
 * @param frequency Desired clock frequency in Hz (100 KHz - 4 MHz)
 * @return true if frequency is valid and clock started, false otherwise
 */
bool initializeClock(uint32_t frequency);

/**
 * Start or resume clock output
 * Enables Timer1 output on pin 11. If clock was stopped, this resumes
 * generation at the previously configured frequency.
 */
void startClock();

/**
 * Stop clock output
 * Disables Timer1 output on pin 11. Timer continues running internally,
 * but no pulses are generated. Useful for debugging and single-stepping.
 * Clock can be resumed with startClock().
 */
void stopClock();

/**
 * Change clock frequency
 * Updates the clock frequency while maintaining the current running state.
 * If clock is running, it transitions smoothly to the new frequency.
 * If clock is stopped, it updates the configuration but stays stopped.
 * 
 * @param frequency Desired frequency in Hz (100 KHz - 4 MHz)
 * @return true if frequency is valid and applied, false if out of range
 */
bool setClockFrequency(uint32_t frequency);

/**
 * Get current configured clock frequency
 * @return Current clock frequency in Hz (returns configured value even if stopped)
 */
uint32_t getClockFrequency();

/**
 * Check if clock is currently running
 * @return true if clock output is active, false if stopped
 */
bool isClockRunning();

/**
 * Generate a single clock pulse for debugging
 * IMPORTANT: Clock must be stopped before calling this function.
 * Generates exactly one high-to-low-to-high clock transition.
 * Useful for single-stepping through Z80 instructions.
 * 
 * @return true if pulse generated, false if clock is running (safety check)
 */
bool singleStepClock();

/**
 * Get a human-readable string describing the current clock state
 * Format: "Running at 500000 Hz" or "Stopped at 1000000 Hz"
 * Note: Returns pointer to static buffer, not thread-safe
 * 
 * @return Pointer to status string
 */
const char* getClockStatusString();

/**
 * Test clock output with manual pin toggling
 * Runs at 1 Hz with 50% duty cycle for 10 seconds (10 cycles).
 * Disables Timer1 and manually toggles pin 11 for visual verification.
 * Useful for testing hardware connections with LED or oscilloscope.
 * Press any key to stop test early and return to timer-based clock.
 */
void testClockOutput();

// =============================================================================
// INTERNAL FUNCTIONS (for advanced use only)
// =============================================================================

/**
 * Calculate optimal timer prescaler and TOP value for requested frequency
 * This is called internally by initializeClock() and setClockFrequency().
 * 
 * @param frequency Desired frequency in Hz
 * @param prescaler Output: calculated prescaler value
 * @param topValue Output: calculated ICR1 TOP value
 * @return true if valid configuration found, false if frequency out of range
 */
bool calculateTimerValues(uint32_t frequency, uint16_t* prescaler, uint16_t* topValue);

#endif // CLOCK_H
