# Phase 5: Memory & Performance

**Status:** 📝 Not Started  
**Estimated Effort:** 1 week  
**Dependencies:** Phase 3, 4

---

## Overview

This phase focuses on adapting memory management for Teensy's different memory architecture and leveraging its increased resources for better performance.

## Documents in This Section

### [Memory_Model.md](Memory_Model.md)
Understanding and adapting to Teensy's memory architecture.

**Topics Covered:**
- Arduino Mega memory model (Flash, SRAM, EEPROM)
- Teensy 4.1 memory model (ITCM, DTCM, OCRAM, Flash)
- PROGMEM usage and alternatives
- Constant data storage strategies
- Memory organization best practices
- Flash vs RAM tradeoffs

---

### [Cache_Optimization.md](Cache_Optimization.md)
Leveraging Teensy's 1MB RAM for optimal caching.

**Topics Covered:**
- Arduino Mega cache limitations (8KB SRAM)
- Teensy 4.1 cache opportunities (1MB RAM)
- Cache size optimization strategies
- Cache hit rate analysis
- Dynamic cache sizing
- Memory allocation strategies
- Performance impact of cache size

---

### [Performance_Tuning.md](Performance_Tuning.md)
Compiler optimization and performance tuning for ARM GCC.

**Topics Covered:**
- GCC optimization flags for ARM Cortex-M7
- Function inlining strategies
- Loop unrolling
- Link-time optimization (LTO)
- Profile-guided optimization
- Code size vs speed tradeoffs
- Benchmarking methodology

---

## Phase 5 Deliverables

- [ ] Memory usage analysis document
- [ ] Updated cache management code
- [ ] Compiler optimization configuration
- [ ] Performance benchmark suite
- [ ] Memory map documentation
- [ ] Optimization comparison report
- [ ] Best practices guide

---

## Memory Comparison

### Arduino Mega 2560
- **Flash:** 256 KB (program storage)
- **SRAM:** 8 KB (variables, stack)
- **EEPROM:** 4 KB (non-volatile storage)
- **Cache:** Up to 1024 bytes (12.5% of SRAM)

### Teensy 4.1
- **Flash:** 8 MB (program storage)
- **RAM:** 1024 KB total
  - DTCM: 512 KB (fast, zero-wait-state)
  - OCRAM: 512 KB (slower but still fast)
- **FlexSPI Flash:** External, slower access
- **Cache:** Can be much larger (e.g., 64-256 KB)

---

## PROGMEM Adaptation

### Arduino Mega Pattern
```cpp
// Store in Flash (PROGMEM)
const uint8_t testData[] PROGMEM = {0x00, 0x01, ...};

// Read from Flash
uint8_t value = pgm_read_byte(&testData[i]);
```

### Teensy Alternatives

**Option 1: Keep PROGMEM for Compatibility**
```cpp
// Teensy supports PROGMEM but it's less necessary
const uint8_t testData[] PROGMEM = {0x00, 0x01, ...};
uint8_t value = pgm_read_byte(&testData[i]);
```

**Option 2: Use Regular Const (Recommended)**
```cpp
// With 1MB RAM, can store in RAM for faster access
const uint8_t testData[] = {0x00, 0x01, ...};
uint8_t value = testData[i];  // Direct access, faster
```

**Option 3: Hybrid Approach**
```cpp
// Small, frequently accessed data in RAM
// Large, infrequently accessed data in PROGMEM
```

---

## Cache Optimization Strategy

### Arduino Mega (Limited)
- Maximum practical cache: 1024 bytes
- Must carefully manage cache size
- Frequently falls back to PROGMEM

### Teensy 4.1 (Abundant)
- Can support much larger caches (64-256 KB)
- Rarely need to access Flash during execution
- Can cache entire test suites

### Recommended Teensy Cache Sizes
- Small programs (<4KB): Cache entire program
- Medium programs (4-64KB): Cache working set
- Large programs (>64KB): Intelligent caching with LRU

---

## Compiler Optimization

### Current Arduino Mega Settings
```cpp
#pragma GCC optimize("O3", "flto", "unroll-loops", "fast-math")
```

### Teensy ARM Cortex-M7 Optimizations

**platformio.ini settings:**
```ini
[env:teensy41]
build_flags = 
    -O3                    ; Maximum optimization
    -flto                  ; Link-time optimization
    -mcpu=cortex-m7       ; Target processor
    -mfloat-abi=hard      ; Hardware floating point
    -mfpu=fpv5-d16        ; FPU configuration
    -funroll-loops        ; Loop unrolling
    -ffast-math           ; Fast math operations
    -mthumb               ; Thumb instruction set
```

**Per-file optimization:**
```cpp
// Critical ISR code
#pragma GCC optimize("O3", "unroll-loops")

// Non-critical UI code
#pragma GCC optimize("O2")
```

---

## Performance Metrics

### Target Improvements Over Arduino Mega

| Metric | Arduino Mega | Teensy Target | Improvement |
|--------|--------------|---------------|-------------|
| ISR Latency | 1.6 µs | <100 ns | 16x faster |
| Bus Read | 125 ns | <10 ns | 12x faster |
| Bus Write | 125 ns | <10 ns | 12x faster |
| Max Z80 Clock | 1 MHz | 5+ MHz | 5x higher |
| Cache Size | 1 KB | 64+ KB | 64x larger |
| Memory Access | PROGMEM slow | RAM fast | Significant |

---

## Testing Strategy

### Memory Tests
1. **PROGMEM Verification**
   - Test reading from PROGMEM
   - Compare PROGMEM vs RAM access speed
   - Verify data integrity
   
2. **Cache Performance**
   - Test various cache sizes
   - Measure hit rates
   - Find optimal cache size
   
3. **Memory Usage**
   - Monitor RAM usage
   - Check for memory leaks
   - Verify stack size adequate

### Performance Tests
1. **Benchmark Suite**
   - Bus operation speed
   - ISR execution time
   - Cache performance
   - Overall throughput
   
2. **Optimization Comparison**
   - Test different optimization levels
   - Measure code size vs speed
   - Profile hot spots
   
3. **Long-term Stability**
   - Extended operation test
   - Memory usage over time
   - Performance consistency

---

## Memory Management Best Practices

1. **Use DTCM for Critical Code**
   - ISRs should execute from DTCM
   - Time-critical functions in DTCM
   
2. **Use DTCM for Critical Data**
   - Bus buffers in DTCM
   - Cache memory in DTCM
   - ISR variables in DTCM
   
3. **Minimize Flash Access**
   - Cache frequently used data
   - Pre-load lookup tables
   - Avoid Flash access in ISRs
   
4. **Monitor Memory Usage**
   - Track stack high-water mark
   - Monitor heap fragmentation
   - Watch for memory leaks

---

## Getting Started

1. Review Arduino Mega memory usage
2. Read [Memory_Model.md](Memory_Model.md) to understand Teensy memory
3. Plan cache strategy in [Cache_Optimization.md](Cache_Optimization.md)
4. Configure optimization in [Performance_Tuning.md](Performance_Tuning.md)
5. Benchmark and iterate

---

## Success Criteria

- ✅ Memory model understood and documented
- ✅ PROGMEM usage adapted or removed
- ✅ Cache size optimized for available RAM
- ✅ Compiler optimizations configured and tested
- ✅ Performance benchmarks meet targets
- ✅ Memory usage stable and efficient
- ✅ No memory leaks or corruption

---

**Previous Phase:** [04_Interrupts/](../04_Interrupts/)  
**Next Phase:** [06_Timing/](../06_Timing/) - Timing & Synchronization
