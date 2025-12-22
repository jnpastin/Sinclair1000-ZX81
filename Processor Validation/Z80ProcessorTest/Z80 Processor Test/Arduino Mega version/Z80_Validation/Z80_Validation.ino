/*
 * Z80 Processor Validation Suite
 * 
 * This Arduino sketch provides a comprehensive validation framework for the Z80 processor.
 * The Arduino Mega 2560 interfaces directly with the Z80, providing:
 * - Clock generation via hardware timer
 * - Memory emulation (64KB address space)
 * - Bus monitoring and control
 * - Comprehensive instruction testing
 * - Timing validation
 * 
 * Hardware: Arduino Mega 2560 + Z80 CPU
 * Author: Z80 Validation Project
 * Date: November 5, 2025
 * 
 * IMPORTANT: All static data (strings, test vectors, lookup tables, etc.) 
 * MUST be stored in PROGMEM to conserve RAM. Use PROGMEM keyword and 
 * pgm_read_* functions for access. Use F() macro for string literals.
 */

// Include headers
#include "pins.h"
#include "clock.h"
#include "menu.h"
#include "hal.h"
#include "performance.h"
// #include "memory.h"
// #include "test_framework.h"

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for serial port to connect (needed for native USB port only)
  }
  
  Serial.println(F("Z80 Processor Validation Suite"));
  Serial.println(F("================================"));
  Serial.println();
  
  // Initialize hardware
  Serial.println(F("Initializing hardware..."));
  
  // Initialize HAL (configure all pins)
  initializeHAL();
  Serial.println(F("  HAL: Initialized"));
  
    // Initialize clock - auto-starts at 500 KHz
  initializeClock();
  Serial.print(F("  Clock: "));
  Serial.println(getClockStatusString());
  
  
  Serial.println(F("Hardware initialized."));
  Serial.println();
  
  // Reset Z80
  Serial.println(F("Resetting Z80..."));
  resetZ80();
  Serial.println(F("Z80 ready."));
  Serial.println();
  
  // Initialize menu system
  initializeMenu();
  
  Serial.println(F("Starting menu system..."));
  Serial.println();
  delay(1000);
}

void loop() {
  // Run the menu system
  runMenu();
}
