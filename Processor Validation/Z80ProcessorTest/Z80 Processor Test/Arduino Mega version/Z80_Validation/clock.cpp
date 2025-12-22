/*
 * Z80 Processor Validation Suite - Clock Generation Implementation
 * 
 * This module implements hardware-based clock generation using Timer1.
 * See clock.h for detailed API documentation.
 * 
 * Memory Conservation:
 * - All constant arrays stored in PROGMEM (flash memory)
 * - All string literals use F() macro
 * - Dynamic data minimized to single ClockConfig struct
 * 
 * Hardware: Arduino Mega 2560, Timer1, Pin 11 (OC1A/PB5)
 * Date: November 6, 2025
 */

#include "clock.h"
#include "pins.h"

// =============================================================================
// STATIC VARIABLES
// =============================================================================
static ClockConfig g_clockConfig = {
  .frequency = CLOCK_FREQ_DEFAULT,
  .prescaler = 0,
  .topValue = 0,
  .running = false
};

// =============================================================================
// CONSTANT DATA IN PROGMEM
// =============================================================================
// Available prescaler values for Timer1
static const uint16_t PRESCALERS[5] PROGMEM = {1, 8, 64, 256, 1024};

// =============================================================================
// INTERNAL HELPER FUNCTIONS
// =============================================================================

/**
 * Calculate optimal timer prescaler and TOP value for requested frequency
 * 
 * Timer1 Fast PWM Mode 14 (ICR1 = TOP):
 * f_clock = F_CPU / (prescaler × (ICR1 + 1))
 * 
 * We try prescalers in order (1, 8, 64, 256, 1024) to find the best fit
 * that gives us the requested frequency with maximum resolution.
 */
bool calculateTimerValues(uint32_t frequency, uint16_t* prescaler, uint16_t* topValue) {
  // Validate frequency range
  if (frequency < CLOCK_FREQ_MIN || frequency > CLOCK_FREQ_MAX) {
    return false;
  }
  
  // F_CPU for Arduino Mega 2560
  const uint32_t F_CPU_HZ = 16000000UL;
  
  // Try each prescaler to find the best fit
  for (uint8_t i = 0; i < 5; i++) {
    uint16_t ps = pgm_read_word(&PRESCALERS[i]);
    
    // Calculate TOP value: ICR1 = (F_CPU / (prescaler × f_clock)) - 1
    uint32_t top = (F_CPU_HZ / (ps * frequency)) - 1;
    
    // Check if TOP value fits in 16-bit register (0-65535)
    if (top <= 65535) {
      *prescaler = ps;
      *topValue = (uint16_t)top;
      return true;
    }
  }
  
  // Should never reach here if frequency is within valid range
  return false;
}

/**
 * Apply timer configuration to hardware registers
 * Sets up Timer1 in Fast PWM mode with the calculated values
 * Respects the current running state (connected/disconnected)
 */
static void applyTimerConfiguration() {
  // Disable interrupts while configuring timer
  noInterrupts();
  
  // Stop Timer1
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  
  // Configure Timer1 for Fast PWM Mode 14 (WGM13:0 = 1110)
  // Mode 14: Fast PWM, TOP = ICR1, Update OCR1x at BOTTOM, TOV1 set on TOP
  // COM1A1:0 = 10 (non-inverting mode on OC1A) if running, 00 if stopped
  if (g_clockConfig.running) {
    TCCR1A = (1 << COM1A1) | (0 << COM1A0) | (1 << WGM11) | (0 << WGM10);
  } else {
    TCCR1A = (0 << COM1A1) | (0 << COM1A0) | (1 << WGM11) | (0 << WGM10);
  }
  
  // Set TOP value (frequency determinant)
  ICR1 = g_clockConfig.topValue;
  
  // Set duty cycle to 50% (OCR1A = ICR1/2)
  OCR1A = g_clockConfig.topValue / 2;
  
  // Set prescaler and start timer (WGM13:2 bits, CS12:0 bits for prescaler)
  uint8_t cs_bits;
  switch (g_clockConfig.prescaler) {
    case 1:    cs_bits = (1 << CS10); break;                          // CS12:0 = 001
    case 8:    cs_bits = (1 << CS11); break;                          // CS12:0 = 010
    case 64:   cs_bits = (1 << CS11) | (1 << CS10); break;            // CS12:0 = 011
    case 256:  cs_bits = (1 << CS12); break;                          // CS12:0 = 100
    case 1024: cs_bits = (1 << CS12) | (1 << CS10); break;            // CS12:0 = 101
    default:   cs_bits = 0; break;  // Should never happen
  }
  
  TCCR1B = (1 << WGM13) | (1 << WGM12) | cs_bits;
  
  // Configure pin 11 (PB5/OC1A) as output
  pinMode(Z80_CLK, OUTPUT);
  
  // If stopped, ensure pin is LOW
  if (!g_clockConfig.running) {
    digitalWrite(Z80_CLK, LOW);
  }
  
  interrupts();
}

/**
 * Disconnect Timer1 output from pin (stops clock output)
 */
static void disconnectTimerOutput() {
  // Clear COM1A bits to disconnect OC1A from pin
  TCCR1A &= ~((1 << COM1A1) | (1 << COM1A0));
}

/**
 * Reconnect Timer1 output to pin (resumes clock output)
 */
static void reconnectTimerOutput() {
  // Set COM1A bits for non-inverting mode
  TCCR1A |= (1 << COM1A1);
  TCCR1A &= ~(1 << COM1A0);
}

// =============================================================================
// PUBLIC FUNCTION IMPLEMENTATIONS
// =============================================================================

/**
 * Initialize clock to default frequency (500 KHz) and start automatically
 */
void initializeClock() {
  initializeClock(CLOCK_FREQ_DEFAULT);
}

/**
 * Initialize clock to specific frequency and start automatically
 */
bool initializeClock(uint32_t frequency) {
  // Calculate timer values
  uint16_t prescaler, topValue;
  if (!calculateTimerValues(frequency, &prescaler, &topValue)) {
    // Invalid frequency
    return false;
  }
  
  // Store configuration
  g_clockConfig.frequency = frequency;
  g_clockConfig.prescaler = prescaler;
  g_clockConfig.topValue = topValue;
  g_clockConfig.running = true;
  
  // Apply configuration to hardware
  applyTimerConfiguration();
  
  return true;
}

/**
 * Start or resume clock output
 */
void startClock() {
  if (!g_clockConfig.running) {
    reconnectTimerOutput();
    g_clockConfig.running = true;
  }
}

/**
 * Stop clock output
 */
void stopClock() {
  if (g_clockConfig.running) {
    disconnectTimerOutput();
    g_clockConfig.running = false;
    
    // Set clock pin low when stopped (Z80 default state)
    digitalWrite(Z80_CLK, LOW);
  }
}

/**
 * Change clock frequency
 */
bool setClockFrequency(uint32_t frequency) {
  // Calculate new timer values
  uint16_t prescaler, topValue;
  if (!calculateTimerValues(frequency, &prescaler, &topValue)) {
    return false;
  }
  
  // Remember running state
  bool wasRunning = g_clockConfig.running;
  
  // Update configuration (preserve running state)
  g_clockConfig.frequency = frequency;
  g_clockConfig.prescaler = prescaler;
  g_clockConfig.topValue = topValue;
  g_clockConfig.running = wasRunning;
  
  // Reconfigure timer (respects g_clockConfig.running)
  applyTimerConfiguration();
  
  return true;
}

/**
 * Get current configured clock frequency
 */
uint32_t getClockFrequency() {
  return g_clockConfig.frequency;
}

/**
 * Check if clock is currently running
 */
bool isClockRunning() {
  return g_clockConfig.running;
}

/**
 * Generate a single clock pulse for debugging
 */
bool singleStepClock() {
  // Safety check: clock must be stopped
  if (g_clockConfig.running) {
    return false;
  }
  
  // Generate one pulse: LOW -> HIGH -> LOW
  // Z80 typically operates on rising edge
  digitalWrite(Z80_CLK, LOW);
  delayMicroseconds(1);  // Hold low
  digitalWrite(Z80_CLK, HIGH);
  delayMicroseconds(1);  // Hold high
  digitalWrite(Z80_CLK, LOW);
  delayMicroseconds(1);  // Return to low
  
  return true;
}

/**
 * Get a human-readable string describing the current clock state
 */
const char* getClockStatusString() {
  static char buffer[64];
  
  if (g_clockConfig.running) {
    snprintf(buffer, sizeof(buffer), "Running at %lu Hz", g_clockConfig.frequency);
  } else {
    snprintf(buffer, sizeof(buffer), "Stopped at %lu Hz", g_clockConfig.frequency);
  }
  
  return buffer;
}

/**
 * Test clock output with manual pin toggling
 * Runs at 1Hz (1 second period) with 50% duty cycle for 10 seconds
 * Useful for verifying hardware connections
 */
void testClockOutput() {
  Serial.println(F("Testing clock output on pin 11..."));
  Serial.println(F("Running at 1 Hz (50% duty cycle) for 10 seconds"));
  Serial.println(F("Watch for LED or measure with multimeter/scope"));
  Serial.println(F("Press any key to stop test"));
  Serial.println();
  
  // Remember current state and stop timer-based clock
  bool wasRunning = g_clockConfig.running;
  if (wasRunning) {
    stopClock();
  }
  
  // Completely disconnect Timer1 from the pin
  TCCR1A = 0;
  
  // Configure pin as output
  pinMode(Z80_CLK, OUTPUT);
  digitalWrite(Z80_CLK, LOW);
  
  // Clear any pending serial input (CR/LF from the 't' command)
  while (Serial.available()) {
    Serial.read();
  }
  delay(10);
  
  // Toggle pin manually at 1Hz for 10 seconds (10 cycles)
  for (uint8_t i = 0; i < 10; i++) {
    if (Serial.available()) {
      Serial.read();
      Serial.println(F("Test aborted by user"));
      break;
    }
    
    digitalWrite(Z80_CLK, HIGH);
    Serial.print(F("Cycle "));
    Serial.print(i + 1);
    Serial.println(F(": HIGH"));
    delay(500);
    
    digitalWrite(Z80_CLK, LOW);
    Serial.println(F("       LOW"));
    delay(500);
  }
  
  Serial.println(F("Test complete"));
  
  // Always reconfigure timer since we disabled it with TCCR1A = 0
  applyTimerConfiguration();
  
  if (wasRunning) {
    Serial.println(F("Clock restored"));
  }
}
