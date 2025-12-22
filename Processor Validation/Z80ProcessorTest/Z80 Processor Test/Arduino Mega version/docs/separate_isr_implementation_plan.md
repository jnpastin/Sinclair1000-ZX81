# Z80 Processor Validation Suite - Separate RD/WR ISR Implementation Plan

## Session Context
- **Date**: November 14, 2025
- **Issue**: 500KHz performance test fails due to missed ISR invocations, causing Z80 to read 0x00 instead of correct instructions, leading to incomplete loop execution (60% through).
- **Root Cause**: Single ISR on MREQ with polling loop misses interrupts at high speeds; base address capture works, but missed ISRs corrupt execution.
- **Goal**: Implement separate ISRs for RD and WR on interrupt-capable pins to eliminate polling, improve speed, and reduce missed interrupts.
- **Hardware Changes Required**: Rewire Z80 MREQ to Arduino pin 47 (PL2), RD to pin 3 (PE5), WR to pin 2 (PE4).
- **Status**: Hardware rewiring completed. Step 1 (pin mappings) completed. Step 2 completed: 2.1-2.7 all implemented. Step 3 completed: 3.1-3.6 all implemented. Ready for Step 4.
- **Previous Attempts**: Optimized single ISR (removed counters, reduced polling), but polling still causes delays.

## Current Pin Assignments (from pins.h)
- ✅ Z80_MREQ: Pin 47 (PL2) - COMPLETED
- Z80_IORQ: Pin 48 (PL1)
- ✅ Z80_RD: Pin 3 (PE5, INT5) - COMPLETED
- ✅ Z80_WR: Pin 2 (PE4, INT4) - COMPLETED
- Other pins unchanged.

## Implementation Plan

### ✅ Step 1: Update Pin Mappings - COMPLETED
- **File**: `pins.h`
- **Changes**:
  - ✅ Z80_MREQ: 2 → 47
  - ✅ Z80_RD: 47 → 3
  - ✅ Z80_WR: 46 → 6
  - ✅ Update CONTROL_INPUT_PINS array.
  - ✅ Update comments.
- **Other Files**: ✅ Grep for Z80_MREQ, Z80_RD, Z80_WR in all files; update references (e.g., hal.cpp masks, docs).
- **Note**: ✅ Update MREQ checks in ISRs to `PINL & 0x04` (PL2/bit 2) - COMPLETED
- **Rollback**: Revert pins.h to original.

### Step 2: Implement Separate ISRs
- **✅ 2.1 Update hal.h**:
  - Add globals: `volatile uint16_t g_lastReadAddress = 0;`, `volatile uint32_t g_readIsrCount = 0;`, `volatile uint32_t g_writeIsrCount = 0;`.
  - Update `resetISRState()`: Reset `g_lastReadAddress = 0; g_readIsrCount = 0; g_writeIsrCount = 0;`.
- **✅ 2.2 Update hal.cpp - Interrupt Setup**:
  - Remove INT4 setup in `initializeMemoryHandler()`.
  - Add EICRB configuration for INT5 (ISC5[1:0] = 10 for falling) and INT4 (ISC4[1:0] = 10 for falling).
  - Enable INT5 and INT4 in EIMSK.
- **✅ 2.6 Remove Old ISR**:
  - Comment out or delete the entire `ISR(INT4_vect)` function and its attributes.
- **✅ 2.3 Implement ISR(INT5_vect) - Read Handler**:
  - Check MREQ: `if ((PINL & 0x04) != 0) return;`.
  - Read address: `uint16_t address = (PINK << 8) | PINC;`.
  - Capture base: `if (!g_programBaseAddressValid) { g_programBaseAddress = address; g_programBaseAddressValid = true; }`.
  - Calculate offset: `uint16_t programOffset = address - g_programBaseAddress;`.
  - Provide data from cache/PROGMEM.
  - Handle capture.
  - Set `g_lastReadAddress = address; g_readIsrCount++;`.
  - Attributes: `__attribute__((hot, flatten, optimize("O3"), optimize("unroll-loops")))`.
- **✅ 2.4 Implement ISR(INT4_vect) - Write Handler**:
  - Check MREQ: `if ((PINL & 0x04) != 0) return;`.
  - Read address and data.
  - Handle capture.
  - Set `g_lastWriteAddress = address; g_writeIsrCount++;`.
  - Same attributes.
- **✅ 2.5 Update HAL Functions**:
  - Update `isReadCycle()`, `isWriteCycle()` to use new pins (PINE for RD, PINH for WR).
  - Update any mask defines (e.g., PORTL_RD_MASK to PINE bit).
- **Files**: `hal.cpp`, `hal.h`
- **Data Separation**: Shared globals (e.g., g_capturedData, g_lastReadAddress, g_lastWriteAddress) tagged by type; no conflicts in performance tests.
- **HAL Updates**: Update functions like isReadCycle() to use new pins (e.g., PINE for RD).
- **✅ 2.7 Update Test Files**:
  - Update `isr_test_programs.cpp`, `performance.cpp` for new ISRs (replaced `g_isrCallCount` checks with `g_programBaseAddressValid` for reads and `g_lastWriteAddress == expectedAddress` for writes; adjusted performance tests to use new counters `g_readIsrCount`, `g_writeIsrCount`; moved prints to post-test; updated comments from INT6 to INT4).
  - Files: `isr_test_programs.cpp`, `performance.cpp`, `menu.cpp`
- **Rollback**: Revert hal.h, hal.cpp.

### ✅ Step 3: Update Documentation - COMPLETED
- **Files**: docs/*.md, README.md
- **Changes**: Update pin mappings, hardware notes.
- **✅ 3.1 Update pin_mapping.md**: Updated control signals table, port summary, performance optimization, code examples, hardware history, and last updated note.
- **✅ 3.2 Update docs/README.md**: Updated reference to separate_isr_implementation_plan.md and added November 14, 2025 changelog entry.
- **✅ 3.3 Update docs/hardware/README.md**: Updated control signals and key signals with new pin assignments.
- **✅ 3.4 Update docs/hardware/pin_mapping.md**: See 3.1 above.
- **✅ 3.5 Update Menu and UI**: Implemented Hardware Status feature in menu.cpp with displayHardwareStatus() function showing clock, ISR, memory handler, and Z80 status.
- **✅ 3.6 Update Headers and Declarations**: Verified isr_test_programs.h and performance.h don't need updates (globals in hal.h).
- **Rollback**: Revert docs, menu.cpp, headers.

### Step 4: Testing
- **4.1 Compile and Upload Code**:
  - ✅ Compile the updated code with separate ISRs.
  - ✅ Upload to Arduino Mega 2560.
- **4.2 Hardware Rewiring Verification**:
  - ✅ Verify connections with multimeter; run basic pin tests - COMPLETED
  - Update `docs/hardware/pin_mapping.md` if needed.
- **4.3 Verify Read ISR Functionality**:
  - Run a dedicated read test (e.g., single read operation).
  - Check that g_readIsrCount increments correctly.
  - Verify g_lastReadAddress is captured accurately.
  - Ensure no errors in Serial output for read operations.
- **4.4 Verify Write ISR Functionality**:
  - Run a dedicated write test (e.g., single write operation).
  - Check that g_writeIsrCount increments correctly.
  - Verify g_lastWriteAddress and captured write data.
  - Ensure writes are handled without errors.
- **4.5 100KHz Performance Test**:
  - Run the 100KHz performance test.
  - Verify program base address capture works correctly.
  - Check g_readIsrCount and g_writeIsrCount for expected values.
  - Ensure no missed ISRs at this speed.
- **4.6 500KHz Performance Test**:
  - Run the 500KHz performance test.
  - Monitor for reduced missed ISRs compared to previous single ISR implementation.
  - Verify loop execution completes 100% (previously 60%).
  - Check ISR counters and captured data integrity.
- **4.7 Documentation and Results**:
  - Document test results in this plan.
  - Update performance metrics if improved.
  - Note: WRITE Test 6 (continuous write loop) validation updated to use ISR-side pattern counters (`g_writePatternAA_count` and `g_writePattern55_count`).
    The test now accepts the alternating 0xAA/0x55 pattern as a pass when the two counters differ by at most ±1 (implementation in `hal.cpp` + `isr_test_programs.cpp`).
- **4.8 Rollback if Issues**:
  - If tests fail or issues arise, rollback hardware and code changes as per rollback plan.

### Risk Mitigation
- **Transitional Globals**: Keep old globals as aliases during testing for backward compatibility.
- **Incremental Testing**: Run `runSingleReadTest` at 100KHz first to verify address capture.
- **Diagnostics**: Print new globals post-test for debugging.
- **Buffer Separation**: Evaluate separate `g_readCaptureBuffer` and `g_writeCaptureBuffer` for organization (increases RAM by ~1KB).
- **Test Simplification**: Remove `runReadWriteTest` as redundant (writes include reads).
- **Timing Safeguards**: Add atomic operations for shared globals to prevent races.

### Rollback Plan
- Hardware: Rewire to original (MREQ:47→2, RD:3→47, WR:6→46).
- Code: Revert pins.h, hal.cpp, docs via git or backups.
- Test: Verify 100KHz works.

### Next Steps
- ✅ Hardware rewiring completed
- ✅ Step 1 (pin mappings) completed
- ✅ Step 2 (separate ISRs) completed
- ✅ Step 3 (documentation) completed
- Ready for Step 4: Testing at 100KHz, then 500KHz
- **Post-Implementation**:
  - Commit in stages; backup branch.
  - Update CHANGELOG.md.
  - Benchmark ISR latency with oscilloscope.
  - Code review for race conditions.</content>
<parameter name="filePath">/Users/jeremyp/git-repos/Sinclair 1000:ZX81/Processor Validation/Z80_Validation/docs/separate_isr_implementation_plan.md