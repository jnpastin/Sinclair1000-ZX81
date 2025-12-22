# Phase 6: Timing & Synchronization

**Status:** 📝 Not Started  
**Estimated Effort:** 1 week  
**Dependencies:** Phase 2, 4

---

## Overview

This phase focuses on recalculating timing budgets for the 600 MHz Teensy, validating ISR performance, and determining maximum Z80 clock speeds.

## Documents in This Section

### [Timing_Analysis.md](Timing_Analysis.md)
Comprehensive timing analysis for Teensy 4.1 implementation.

**Topics Covered:**
- Z80 bus cycle timing requirements
- Teensy cycle time calculations (600 MHz)
- Timing budget for ISR execution
- Wait state calculations
- Timing margin analysis
- Logic analyzer timing validation

---

### [ISR_Latency.md](ISR_Latency.md)
Detailed interrupt latency measurement and optimization.

**Topics Covered:**
- Interrupt latency components
- NVIC interrupt dispatch time
- Function call overhead
- ISR execution time breakdown
- Optimization techniques
- Measurement methodology
- Real-world measurements

---

### [Z80_Speed_Limits.md](Z80_Speed_Limits.md)
Analysis of maximum supportable Z80 clock frequencies.

**Topics Covered:**
- Theoretical limits based on ISR performance
- Practical limits with safety margins
- Different scenarios (cached vs uncached)
- Comparison with Arduino Mega limits
- Recommendations for default speeds
- Testing at maximum speeds

---

## Phase 6 Deliverables

- [ ] Complete timing analysis spreadsheet
- [ ] ISR latency measurement results
- [ ] Maximum Z80 speed determination
- [ ] Timing validation test suite
- [ ] Logic analyzer timing captures
- [ ] Timing documentation
- [ ] Performance comparison report

---

## Timing Budget Comparison

### Arduino Mega 2560 (16 MHz)
```
Clock Period: 62.5 ns
ISR Latency: ~4-6 cycles = 250-375 ns
ISR Execution: ~25 cycles = 1.56 µs
Total Response: ~2 µs

Z80 @ 1 MHz:
- T-state: 1 µs
- 4 T-states per cycle: 4 µs
- ISR must complete in <4 µs
- Margin: ~2 µs (50%)
```

### Teensy 4.1 (600 MHz)
```
Clock Period: 1.67 ns
Target ISR Latency: <30 cycles = <50 ns
Target ISR Execution: <60 cycles = <100 ns
Total Response Target: <150 ns

Z80 @ 5 MHz:
- T-state: 200 ns
- 4 T-states per cycle: 800 ns
- ISR must complete in <800 ns
- Margin: ~650 ns (81%)

Z80 @ 10 MHz (Stretch Goal):
- T-state: 100 ns
- 4 T-states per cycle: 400 ns
- ISR must complete in <400 ns
- Margin: ~250 ns (62.5%)
```

---

## Key Timing Metrics

### Critical Path Components

| Operation | Cycles | Time @ 600 MHz | Notes |
|-----------|--------|----------------|-------|
| Interrupt Entry | 12-20 | 20-33 ns | NVIC overhead |
| Stack Frame | 8-12 | 13-20 ns | Register save |
| Read Address Bus | 2-4 | 3-7 ns | GPIO read |
| Cache Lookup | 5-10 | 8-17 ns | Array access |
| Write Data Bus | 2-4 | 3-7 ns | GPIO write |
| Interrupt Exit | 8-12 | 13-20 ns | Register restore |
| **Total** | **37-62** | **62-103 ns** | **Cached** |

### Uncached (Flash) Path
- Additional Flash access: ~150-300 cycles
- Total: ~200-400 cycles = 333-667 ns
- Still fast enough for 1-2 MHz Z80

---

## Z80 Bus Cycle Timing

### Standard Z80 Timing
```
Read Cycle (M1 - Opcode Fetch):
  T1: Address Valid, MREQ active
  T2: RD active, wait for data
  T3: Data must be valid
  T4: RD inactive, data latched

Write Cycle:
  T1: Address Valid, MREQ active
  T2: WR active, data output
  T3: Data stable
  T4: WR inactive

Total: 4 T-states minimum
Some instructions: 5-6 T-states
```

### Timing Requirements for ISR

At various Z80 frequencies:

| Z80 Freq | T-State | ISR Budget | Teensy Target | Margin |
|----------|---------|------------|---------------|--------|
| 1 MHz | 1000 ns | ~2000 ns | 100 ns | 1900 ns |
| 2 MHz | 500 ns | ~1000 ns | 100 ns | 900 ns |
| 3 MHz | 333 ns | ~666 ns | 100 ns | 566 ns |
| 5 MHz | 200 ns | ~400 ns | 100 ns | 300 ns |
| 10 MHz | 100 ns | ~200 ns | 100 ns | 100 ns |

---

## Wait State Strategy

### When to Use WAIT
- ISR cannot respond in time
- Cache miss (Flash access needed)
- Complex operations (e.g., I/O emulation)

### WAIT Implementation
```cpp
void assertWait() {
  // Assert WAIT signal (active low)
  // Extends current T-state
  // Z80 pauses until WAIT released
}

void deassertWait() {
  // Release WAIT signal
  // Z80 continues execution
}
```

### Automatic WAIT (Backpressure)
- Detect when ISR is too slow
- Automatically assert WAIT
- Release when ready
- Ensures data integrity

---

## Testing Strategy

### Timing Measurements
1. **ISR Latency Test**
   - GPIO toggle at interrupt entry
   - Measure with logic analyzer
   - Test at various frequencies
   
2. **ISR Execution Test**
   - GPIO toggle at entry and exit
   - Measure total ISR time
   - Test cached vs uncached
   
3. **End-to-End Timing**
   - Z80 asserts RD/WR
   - Teensy provides/captures data
   - Measure total cycle time
   - Verify timing margins

### Speed Limit Testing
1. **Incremental Frequency Test**
   - Start at 1 MHz
   - Increase in 500 KHz steps
   - Monitor for errors
   - Find maximum stable frequency
   
2. **Stress Testing**
   - Run at maximum speed
   - Execute instruction patterns
   - Monitor error rates
   - Long-term stability test
   
3. **Margin Testing**
   - Test at 90% of maximum
   - Verify stable operation
   - Set recommended limits

### Validation with Logic Analyzer
1. **Capture Timing Diagrams**
   - Z80 clock signal
   - RD/WR signals
   - Address/Data buses
   - ISR timing markers (GPIO toggles)
   
2. **Verify Timing Specs**
   - Setup times
   - Hold times
   - Response times
   - Recovery times
   
3. **Document Results**
   - Save timing captures
   - Create timing diagrams
   - Document violations (if any)

---

## Optimization Opportunities

### If Timing Too Tight
1. **Inline Critical Functions**
   - Remove function call overhead
   - Use `__attribute__((always_inline))`
   
2. **Assembly Language**
   - Hand-optimize critical path
   - Use ARM SIMD instructions
   
3. **Tightly Coupled Memory**
   - Place ISR in ITCM
   - Place data in DTCM
   - Zero-wait-state access
   
4. **Reduce ISR Work**
   - Defer non-critical operations
   - Use double-buffering
   - Process in main loop

### If Timing Comfortable
1. **Add Safety Checks**
   - Data validation
   - Error detection
   - Debugging instrumentation
   
2. **Support Higher Frequencies**
   - Test at 10+ MHz
   - Optimize for speed
   - Document capabilities

---

## Getting Started

1. Review Z80 timing requirements
2. Calculate Teensy timing budgets in [Timing_Analysis.md](Timing_Analysis.md)
3. Measure ISR performance in [ISR_Latency.md](ISR_Latency.md)
4. Determine speed limits in [Z80_Speed_Limits.md](Z80_Speed_Limits.md)
5. Validate with logic analyzer

---

## Success Criteria

- ✅ Complete timing analysis documented
- ✅ ISR latency measured and optimized
- ✅ Maximum Z80 frequency determined
- ✅ Timing margins verified adequate
- ✅ Logic analyzer validation complete
- ✅ Performance exceeds Arduino Mega
- ✅ Stable operation at target frequencies

---

**Previous Phase:** [05_Memory/](../05_Memory/)  
**Next Phase:** [07_Build/](../07_Build/) - Build System & Configuration
