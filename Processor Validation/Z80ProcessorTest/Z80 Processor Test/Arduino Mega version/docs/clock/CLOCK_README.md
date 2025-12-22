# Z80 Clock Generation Module

## Overview

The clock module provides hardware-based clock generation for the Z80 CPU using Arduino Mega 2560's Timer1. The clock output appears on **pin 11 (PB5/OC1A)**.

## Features

- **Frequency Range**: 100 KHz to 3 MHz
- **Default Frequency**: 500 KHz (auto-starts on initialization)
- **Hardware PWM**: Jitter-free, stable clock output with zero CPU overhead
- **50% Duty Cycle**: Proper timing for Z80 operation
- **Runtime Control**: Start, stop, and frequency adjustment without reset
- **Single-Step Mode**: Generate individual clock pulses for debugging
- **State Queries**: Check frequency and running status

## Quick Start

### Basic Initialization

```cpp
#include "clock.h"

void setup() {
  // Initialize clock to default 500 KHz and auto-start
  initializeClock();
  
  // Clock is now running at 500 KHz
}
```

### Custom Frequency Initialization

```cpp
void setup() {
  // Initialize to 1 MHz and auto-start
  initializeClock(CLOCK_FREQ_1MHZ);
  
  // Or use specific frequency
  initializeClock(2000000);  // 2 MHz
}
```

## API Reference

### Initialization Functions

#### `void initializeClock()`
Initialize clock to default frequency (500 KHz) and start automatically.

**Example:**
```cpp
initializeClock();  // Starts at 500 KHz
```

#### `bool initializeClock(uint32_t frequency)`
Initialize clock to specific frequency and start automatically.

**Parameters:**
- `frequency`: Desired frequency in Hz (100,000 to 3,000,000)

**Returns:** 
- `true` if frequency is valid and clock started
- `false` if frequency is out of range

**Example:**
```cpp
if (initializeClock(1000000)) {
  Serial.println("Clock started at 1 MHz");
}
```

### Control Functions

#### `void startClock()`
Start or resume clock output. Enables Timer1 output on pin 11.

**Example:**
```cpp
startClock();  // Resume clock generation
```

#### `void stopClock()`
Stop clock output. Timer continues internally but no pulses are generated. Useful for debugging and single-stepping.

**Example:**
```cpp
stopClock();  // Stop for debugging
```

#### `bool setClockFrequency(uint32_t frequency)`
Change clock frequency while maintaining running state.

**Parameters:**
- `frequency`: Desired frequency in Hz (100,000 to 3,000,000)

**Returns:**
- `true` if frequency is valid and applied
- `false` if out of range

**Example:**
```cpp
setClockFrequency(CLOCK_FREQ_2MHZ);  // Switch to 2 MHz
setClockFrequency(500000);           // Switch to 500 KHz
```

#### `bool singleStepClock()`
Generate a single clock pulse for instruction-level debugging.

**Requirements:**
- Clock **must be stopped** before calling

**Returns:**
- `true` if pulse generated
- `false` if clock is running (safety check)

**Example:**
```cpp
stopClock();
singleStepClock();  // Execute one Z80 instruction
singleStepClock();  // Execute another
startClock();       // Resume normal operation
```

### Query Functions

#### `uint32_t getClockFrequency()`
Get current configured clock frequency.

**Returns:** Current frequency in Hz (even if clock is stopped)

**Example:**
```cpp
uint32_t freq = getClockFrequency();
Serial.print("Frequency: ");
Serial.println(freq);
```

#### `bool isClockRunning()`
Check if clock output is currently active.

**Returns:**
- `true` if clock is running
- `false` if stopped

**Example:**
```cpp
if (isClockRunning()) {
  Serial.println("Clock is running");
} else {
  Serial.println("Clock is stopped");
}
```

#### `const char* getClockStatusString()`
Get human-readable status string.

**Returns:** Pointer to static string buffer (format: "Running at 500000 Hz")

**Example:**
```cpp
Serial.println(getClockStatusString());
// Output: "Running at 500000 Hz" or "Stopped at 1000000 Hz"
```

## Predefined Frequency Constants

```cpp
CLOCK_FREQ_100KHZ   // 100,000 Hz
CLOCK_FREQ_500KHZ   // 500,000 Hz (DEFAULT)
CLOCK_FREQ_1MHZ     // 1,000,000 Hz
CLOCK_FREQ_2MHZ     // 2,000,000 Hz
CLOCK_FREQ_3MHZ     // 3,000,000 Hz

CLOCK_FREQ_MIN      // 100,000 Hz (minimum)
CLOCK_FREQ_MAX      // 3,000,000 Hz (maximum)
CLOCK_FREQ_DEFAULT  // 500,000 Hz
```

## Usage Examples

### Example 1: Default Startup
```cpp
void setup() {
  Serial.begin(115200);
  
  // Initialize with defaults (500 KHz, auto-start)
  initializeClock();
  
  Serial.print("Clock: ");
  Serial.println(getClockStatusString());
  // Output: "Clock: Running at 500000 Hz"
}
```

### Example 2: Progressive Speed Testing
```cpp
void runProgressiveTest() {
  const uint32_t frequencies[] = {
    CLOCK_FREQ_100KHZ,
    CLOCK_FREQ_500KHZ,
    CLOCK_FREQ_1MHZ,
    CLOCK_FREQ_2MHZ,
    CLOCK_FREQ_3MHZ
  };
  
  for (uint8_t i = 0; i < 5; i++) {
    setClockFrequency(frequencies[i]);
    Serial.print("Testing at: ");
    Serial.println(getClockStatusString());
    
    // Run tests at this frequency
    runInstructionTests();
    
    delay(1000);
  }
}
```

### Example 3: Single-Step Debugging
```cpp
void debugInstruction() {
  // Stop clock for debugging
  stopClock();
  
  // Load test instruction into memory
  loadInstruction(0x3E, 0x42);  // LD A, 42h
  
  // Execute one instruction at a time
  Serial.println("Executing: LD A, 42h");
  for (int i = 0; i < 4; i++) {  // LD A,n takes 4 T-states
    singleStepClock();
    Serial.print("T-state ");
    Serial.print(i + 1);
    Serial.println(" complete");
    
    // Read and display Z80 state here
    delay(100);
  }
  
  // Resume normal operation
  startClock();
}
```

### Example 4: Dynamic Frequency Adjustment
```cpp
void adjustFrequencyBasedOnTest() {
  // Start with slow speed for initial validation
  setClockFrequency(CLOCK_FREQ_100KHZ);
  
  if (runBasicTests()) {
    Serial.println("Basic tests passed, increasing speed...");
    setClockFrequency(CLOCK_FREQ_1MHZ);
  }
  
  if (runAdvancedTests()) {
    Serial.println("Advanced tests passed, full speed!");
    setClockFrequency(CLOCK_FREQ_3MHZ);
  }
}
```

## Technical Details

### Timer1 Configuration
- **Mode**: Fast PWM Mode 14 (TOP = ICR1)
- **Output**: OC1A (Pin 11, PB5)
- **Duty Cycle**: 50% (OCR1A = ICR1/2)
- **Prescalers**: Auto-selected from {1, 8, 64, 256, 1024}

### Frequency Calculation
```
f_clock = F_CPU / (prescaler × (ICR1 + 1))

Where:
- F_CPU = 16,000,000 Hz (Arduino Mega)
- prescaler = {1, 8, 64, 256, 1024}
- ICR1 = 0 to 65535
```

### Example Calculations

| Frequency | Prescaler | ICR1 | Actual Frequency |
|-----------|-----------|------|------------------|
| 3 MHz     | 1         | 4    | 3,200,000 Hz     |
| 2 MHz     | 1         | 7    | 2,000,000 Hz     |
| 1 MHz     | 1         | 15   | 1,000,000 Hz     |
| 500 KHz   | 1         | 31   | 500,000 Hz       |
| 100 KHz   | 8         | 19   | 100,000 Hz       |

## Hardware Connection

Connect Arduino Mega **pin 11** to Z80 **pin 6 (CLK)**.

```
Arduino Mega Pin 11 (OC1A) -----> Z80 Pin 6 (CLK)
```

## Notes

- Clock starts automatically at 500 KHz when `initializeClock()` is called
- Frequency changes maintain the running/stopped state
- Single-step mode requires clock to be stopped (safety feature)
- No CPU overhead during normal operation (hardware PWM)
- All frequency calculations are done at runtime for flexibility

## See Also

- `pins.h` - Pin definitions
- `hal.h` - Hardware abstraction layer
- Z80 CPU datasheet for timing requirements
