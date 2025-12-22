# PROGMEM Usage Summary

## Overview
To conserve RAM for the 64KB memory emulation buffer, all static data in the Z80 Validation project is stored in PROGMEM (Flash memory) wherever possible.

## PROGMEM Implementation by File

### clock.cpp
**Static Data in PROGMEM:**
- ✅ `PRESCALERS[5]` - Timer1 prescaler lookup table (10 bytes in flash vs RAM)
  ```cpp
  static const uint16_t PRESCALERS[5] PROGMEM = {1, 8, 64, 256, 1024};
  ```
  **Access:** `uint16_t ps = pgm_read_word(&PRESCALERS[i]);`

**String Literals:**
- ✅ All `Serial.println()` calls use `F()` macro
- ✅ Examples: `F("Testing clock output on pin 11...")`, `F("Clock stopped")`

**Dynamic RAM Usage:**
- `ClockConfig g_clockConfig` - 8 bytes (necessary for runtime state)
- `char buffer[64]` in `getClockStatusString()` - 64 bytes (reusable buffer)
- **Total:** ~72 bytes RAM

### pins.h
**Static Data in PROGMEM:**
- ✅ `DATA_BUS_PINS[8]` - 8 bytes
- ✅ `ADDRESS_BUS_PINS[16]` - 16 bytes
- ✅ `CONTROL_INPUT_PINS[8]` - 8 bytes
- ✅ `CONTROL_OUTPUT_PINS[6]` - 6 bytes
- **Total:** 38 bytes saved in flash instead of RAM

**Access Pattern:**
```cpp
uint8_t pin = pgm_read_byte(&DATA_BUS_PINS[i]);
```

### Z80_Validation.ino
**String Literals:**
- ✅ All `Serial.println()` calls use `F()` macro
- ✅ Menu strings, status messages, command responses all in flash

**Examples:**
```cpp
Serial.println(F("Z80 Processor Validation Suite"));
Serial.println(F("Clock set to 1 MHz"));
Serial.println(F("Unknown command: "));
```

## Memory Savings Summary

### Flash Memory Used (Acceptable)
| Item | Size | Location |
|------|------|----------|
| PRESCALERS array | 10 bytes | clock.cpp |
| Pin arrays | 38 bytes | pins.h |
| String literals | ~500+ bytes | All .ino/.cpp files |
| **Total** | **~548 bytes** | **Flash (not RAM)** |

### RAM Saved (Critical)
Without PROGMEM, these would consume RAM:
- Pin arrays: 38 bytes
- Prescaler array: 10 bytes
- String literals: 500+ bytes
- **Total RAM saved: ~548 bytes**

### RAM Currently Used by Clock Module
- `ClockConfig` struct: 8 bytes (frequency, prescaler, topValue, running)
- Status string buffer: 64 bytes (reusable)
- **Total: ~72 bytes RAM**

## Best Practices Applied

### 1. Constant Arrays in PROGMEM
```cpp
// CORRECT - In flash
static const uint16_t PRESCALERS[5] PROGMEM = {1, 8, 64, 256, 1024};

// WRONG - Would be in RAM
static const uint16_t prescalers[] = {1, 8, 64, 256, 1024};
```

### 2. String Literals with F() Macro
```cpp
// CORRECT - String in flash
Serial.println(F("Clock stopped"));

// WRONG - String copied to RAM
Serial.println("Clock stopped");
```

### 3. Accessing PROGMEM Data
```cpp
// Reading byte from PROGMEM
uint8_t value = pgm_read_byte(&array[index]);

// Reading word (16-bit) from PROGMEM
uint16_t value = pgm_read_word(&array[index]);

// Reading dword (32-bit) from PROGMEM
uint32_t value = pgm_read_dword(&array[index]);
```

### 4. Dynamic vs Static Data
**Store in PROGMEM:**
- ✅ Lookup tables
- ✅ String literals
- ✅ Instruction definitions (future)
- ✅ Test vectors (future)
- ✅ Expected results (future)

**Keep in RAM:**
- ✅ Runtime state variables
- ✅ Configuration that changes
- ✅ Buffers for active use
- ✅ Memory emulation array (64KB)

## Future PROGMEM Usage

### When Implementing Instruction Tests
```cpp
// Instruction opcode table - PROGMEM
static const uint8_t OPCODES[] PROGMEM = {
  0x00, 0x01, 0x02, // NOP, LD BC,nn, LD (BC),A...
};

// Instruction mnemonics - PROGMEM
static const char MNEM_NOP[] PROGMEM = "NOP";
static const char MNEM_LD_BC_NN[] PROGMEM = "LD BC,nn";
static const char* const MNEMONICS[] PROGMEM = {
  MNEM_NOP, MNEM_LD_BC_NN, // ...
};

// Access
char buffer[16];
strcpy_P(buffer, (char*)pgm_read_word(&MNEMONICS[index]));
```

### Flag Calculation Lookup Tables
```cpp
// Precomputed flag values - PROGMEM (256 bytes)
static const uint8_t ADD_FLAGS[256] PROGMEM = { /* ... */ };
static const uint8_t SUB_FLAGS[256] PROGMEM = { /* ... */ };
static const uint8_t AND_FLAGS[256] PROGMEM = { /* ... */ };
```

### Test Vectors - PROGMEM
```cpp
struct TestVector {
  uint8_t opcode;
  uint8_t operand1;
  uint8_t operand2;
  uint8_t expected_result;
  uint8_t expected_flags;
};

static const TestVector ADD_TESTS[] PROGMEM = {
  {0x80, 0x10, 0x20, 0x30, 0x00},  // ADD A,B
  {0x80, 0xFF, 0x01, 0x00, 0x50},  // Zero + Carry
  // ...
};
```

## Memory Budget

### Arduino Mega 2560 Memory
- **Flash:** 256 KB (program storage)
- **SRAM:** 8 KB (runtime variables)
- **EEPROM:** 4 KB (persistent storage)

### Allocation Strategy
- **Flash:** Store all static data, strings, lookup tables
- **SRAM:** 
  - 64 KB memory emulation: Won't fit! Need alternative strategy
  - Runtime state: ~1-2 KB
  - Stack: ~1 KB
  - Heap: Minimize
- **EEPROM:** Configuration, test results (optional)

### Note on 64KB Memory Emulation
The Arduino Mega 2560 has only 8KB of SRAM, so true 64KB memory emulation is not possible in RAM. Alternative strategies:
1. Use external SRAM chip (23LC1024 or similar)
2. Use SD card for memory backing
3. Implement partial memory emulation (e.g., 4KB window)
4. Use compression for test programs

## Verification

### Check RAM Usage
After compilation, check the output:
```
Sketch uses 12345 bytes (4%) of program storage space.
Global variables use 234 bytes (2%) of dynamic memory.
```

**Target:** Keep global variables under 2KB to leave room for stack/heap.

### Check PROGMEM Effectiveness
Add to code temporarily:
```cpp
Serial.print(F("Free RAM: "));
extern int __heap_start, *__brkval;
int v;
Serial.println((int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval));
```

## Compliance Checklist

- ✅ All constant arrays use `PROGMEM` keyword
- ✅ All string literals use `F()` macro
- ✅ PROGMEM data accessed with `pgm_read_*()` functions
- ✅ No unnecessary global variables
- ✅ Reusable buffers instead of multiple allocations
- ✅ Documentation includes memory usage notes
- ✅ Future modules will follow same pattern

## References
- [Arduino PROGMEM Guide](https://www.arduino.cc/reference/en/language/variables/utilities/progmem/)
- [AVR libc pgmspace.h](https://www.nongnu.org/avr-libc/user-manual/group__avr__pgmspace.html)
- Arduino Mega 2560 Datasheet
