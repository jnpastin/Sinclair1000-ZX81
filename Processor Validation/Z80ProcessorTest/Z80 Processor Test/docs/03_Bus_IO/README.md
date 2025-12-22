# Phase 3: Bus I/O Operations

**Status:** 📝 Not Started  
**Estimated Effort:** 2-3 weeks  
**Dependencies:** Phase 1

---

## Overview

This phase focuses on replacing AVR PORT-based bus I/O with Teensy GPIO operations, maintaining high performance for data and address bus access.

## Documents in This Section

### [Port_Replacement.md](Port_Replacement.md)
Translation guide from AVR PORT operations to Teensy GPIO.

**Topics Covered:**
- AVR PORT architecture review
- Teensy GPIO register mapping
- Direct register access vs Arduino API
- Performance comparison
- Code migration patterns

---

### [Fast_IO_Implementation.md](Fast_IO_Implementation.md)
Implementation of high-speed parallel I/O on Teensy.

**Topics Covered:**
- GPIO bank selection for 8-bit data bus
- GPIO bank selection for 16-bit address bus
- Atomic multi-pin operations
- Inline function optimization
- Assembly language optimization (if needed)
- Performance benchmarking

---

### [HAL_Porting.md](HAL_Porting.md)
Complete guide for porting the Hardware Abstraction Layer.

**Topics Covered:**
- HAL function inventory
- Function-by-function migration guide
- Testing each HAL function
- Integration testing
- Performance validation

---

## Phase 3 Deliverables

- [ ] GPIO register access macros
- [ ] New `hal.cpp` implementation for Teensy
- [ ] New `hal.h` header for Teensy
- [ ] Bus I/O performance benchmarks
- [ ] Unit tests for each HAL function
- [ ] Integration test suite
- [ ] Performance comparison report

---

## Key HAL Functions to Port

### Initialization
- `initializeHAL()` - Configure all pins and peripherals
- `initializeMemoryHandler()` - Set up interrupt handlers

### Bus Access (Critical Performance)
- `readAddressBus()` - Read 16-bit address
- `readDataBus()` - Read 8-bit data
- `writeDataBus(uint8_t)` - Write 8-bit data
- `setDataBusDirection(bool)` - Configure as input/output

### Control Signals
- `readControlSignals()` - Read all control lines
- `resetZ80()` - Assert RESET signal
- `assertWait()` / `deassertWait()` - WAIT control
- `assertInt()` / `deassertInt()` - INT control
- `assertNMI()` / `deassertNMI()` - NMI control

### Memory Emulation
- `loadTestProgram()` - Load program into cache
- `getCacheSize()` / `setCacheSize()` - Cache management
- ISR implementations for memory requests

---

## Performance Requirements

### Critical Path (ISR)
| Operation | Arduino Mega (16 MHz) | Teensy Target (600 MHz) |
|-----------|----------------------|------------------------|
| Read Address Bus | 2-4 cycles (125-250 ns) | <10 ns |
| Read Data Bus | 2 cycles (125 ns) | <10 ns |
| Write Data Bus | 2 cycles (125 ns) | <10 ns |
| ISR Total | ~25 cycles (1.6 µs) | <100 ns |

### Non-Critical Operations
- Pin configuration: <1 ms
- Control signal changes: <100 ns
- Cache loading: <10 ms for 1KB

---

## AVR PORT to Teensy GPIO Mapping

### Arduino Mega (AVR)
```cpp
// Data bus on PORTA (8 bits, pins 22-29)
uint8_t data = PINA;          // Read
PORTA = data;                 // Write
DDRA = 0x00;                  // Input
DDRA = 0xFF;                  // Output

// Address low on PORTC (8 bits, pins 30-37)
uint8_t addr_low = PINC;

// Address high on PORTK (8 bits, pins 62-69)
uint8_t addr_high = PINK;

// 16-bit address
uint16_t addr = (addr_high << 8) | addr_low;
```

### Teensy 4.1 (ARM) - To Be Implemented
```cpp
// Strategy TBD based on pin assignment
// Likely using GPIO6, GPIO7, GPIO8, GPIO9 registers
// See Fast_IO_Implementation.md for details
```

---

## Testing Strategy

### Unit Tests
1. **Pin Configuration Test**
   - Verify all pins configured correctly
   - Test input/output switching
   
2. **Bus Read Test**
   - Apply known values to address/data buses
   - Verify correct reading
   
3. **Bus Write Test**
   - Write values to data bus
   - Verify with logic analyzer
   
4. **Control Signal Test**
   - Test each control signal independently
   - Verify active-low operation

### Integration Tests
1. **Memory Cycle Emulation**
   - Simulate complete Z80 read cycle
   - Simulate complete Z80 write cycle
   
2. **ISR Performance Test**
   - Measure ISR entry latency
   - Measure ISR execution time
   - Verify no missed interrupts

### Performance Tests
1. **Throughput Test**
   - Maximum read operations per second
   - Maximum write operations per second
   
2. **Latency Test**
   - Interrupt response time
   - Bus operation latency

---

## Getting Started

1. Review Arduino Mega HAL implementation
2. Read [Port_Replacement.md](Port_Replacement.md) for translation guide
3. Study [Fast_IO_Implementation.md](Fast_IO_Implementation.md) for optimization
4. Follow [HAL_Porting.md](HAL_Porting.md) step-by-step
5. Test incrementally with logic analyzer

---

## Success Criteria

- ✅ All HAL functions ported and tested
- ✅ Bus operations meet performance requirements
- ✅ Unit tests pass for all functions
- ✅ Integration tests pass
- ✅ Performance equal to or better than Arduino Mega
- ✅ Code is maintainable and well-documented

---

**Previous Phase:** [02_Clock/](../02_Clock/)  
**Next Phase:** [04_Interrupts/](../04_Interrupts/) - Interrupt System
