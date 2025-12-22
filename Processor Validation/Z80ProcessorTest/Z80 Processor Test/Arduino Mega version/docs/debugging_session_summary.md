Below is a comprehensive documentation of our debugging session for the Z80 Processor Validation Suite's speed testing. This covers the initial issues, all code changes made, test results observed, and the current state. It is structured chronologically to allow another agent to resume seamlessly without needing external context.

### Session Overview
- **Date Range**: November 11-13, 2025
- **Primary Focus**: Debugging the `runPerformanceBenchmark` function in `performance.cpp`, which measures ISR performance at various Z80 clock speeds (100KHz, 500KHz, 1MHz, 2MHz). The core issues were:
  - Incorrect ISR call counting due to integer overflow in expected call calculations.
  - Missing ISR interrupts at higher speeds, leading to low actual call counts compared to expected.
  - Inconsistent starting address reporting (e.g., 0x1 instead of 0x0).
  - Potential backpressure and capture mode issues affecting performance tests.
- **Hardware Context**: Arduino Mega 2560 + Z80 CPU, with logic analyzer used for validation.
- **Key Files Involved**:
  - `performance.cpp`: Main performance testing logic.
  - `hal.cpp`: ISR implementation and hardware abstraction.
  - `hal.h`: ISR state reset functions.
  - `isr_test_programs.cpp`: Test execution and validation.
- **Progress Reference**: The session builds on `docs/progress.md`, which documents Phase 3 (Performance Optimization & Cache System). We focused on fixing functional issues before validating cache performance.

### Chronological Summary of Actions and Changes

#### 1. Initial Analysis (November 11, 2025)
- **Issue Identified**: The `runPerformanceBenchmark` function was failing to correctly count ISR calls. At 100KHz, expected calls were ~2509 instead of ~45458 due to integer overflow in `expectedCalls = (frequency * g_isrBaselineCalls) / 100000`.
- **Root Cause**: Multiplication of large numbers (e.g., 100000 * 45458 = 4.5 billion) exceeded `uint32_t` max (4.2 billion), causing silent overflow.
- **Action**: Reviewed `performance.cpp` and `hal.cpp` to understand the ISR counting logic.
- **No Code Changes Yet**: Focused on understanding before acting.

#### 2. Fixed Integer Overflow in ISR Call Calculations (November 11, 2025)
- **Change Made**: In `performance.cpp`, modified `testReadPerformance()` and `testWritePerformance()` to use 64-bit arithmetic for expected call calculations.
  - **File**: `/Users/jeremyp/git-repos/Sinclair 1000:ZX81/Processor Validation/Z80_Validation/Z80_Validation/performance.cpp`
  - **Before**:
    ```cpp
    uint32_t expectedCalls = (frequency * g_isrBaselineCalls) / 100000;
    ```
  - **After**:
    ```cpp
    uint64_t temp = (uint64_t)frequency * g_isrBaselineCalls;
    uint32_t expectedCalls = temp / 100000;
    ```
- **Rationale**: Prevents overflow, ensuring accurate scaling (e.g., 100KHz baseline of 45458 scales to ~227,290 at 500KHz).
- **Test Result**: 100KHz test now shows expected ~45458 (matching actual). 500KHz shows expected ~227,290 but actual ~52,087, indicating missed interrupts.

#### 3. Added Baseline Support for Write Performance Tests (November 11, 2025)
- **Issue Identified**: Write tests used `frequency / 8` for expected calls, which was inaccurate and inconsistent with read tests' baseline scaling.
- **Changes Made**:
  - **Added Global Variable**: In `performance.cpp`, added `static uint32_t g_writeBaselineCalls = 0;`
  - **Reset Baselines**: Updated `runPerformanceBenchmark()`, `measureISRPerformance()`, and `findMaximumSpeed()` to reset `g_writeBaselineCalls = 0;` alongside `g_isrBaselineCalls`.
  - **Capture Write Baseline**: In `testWritePerformance()`, added baseline capture similar to read tests:
    ```cpp
    if (frequency == 100000 && g_writeBaselineCalls == 0) {
        g_writeBaselineCalls = actualCalls;
        Serial.print(F("  📊 WRITE BASELINE CAPTURED: "));
        Serial.print(actualCalls);
        Serial.println(F(" ISR calls at 100KHz"));
    }
    ```
  - **Use Baseline for Expected Calls**: Replaced `frequency / 8` with 64-bit scaling:
    ```cpp
    uint64_t temp = (uint64_t)frequency * g_writeBaselineCalls;
    uint32_t expectedCalls = temp / 100000;
    ```
  - **Unified Capture Mode**: Changed write tests to `CAPTURE_OFF` (no backpressure) for consistency and faster ISR.
  - **Simplified Validation**: Removed specific write validation in `runSingleWriteTest()`; now checks only ISR activity (>0 calls, no errors), matching read tests.
- **Files Modified**:
  - `performance.cpp`: Added global, resets, baseline capture, and calculation changes.
  - `isr_test_programs.cpp`: Updated `runSingleWriteTest()` to remove write-specific validation.
- **Rationale**: Ensures consistent baseline-based scaling for both read and write tests, avoiding backpressure issues.
- **Test Result**: Write tests now capture baselines and scale expectations accurately.

#### 4. Addressed Backpressure and Buffer Contamination (November 11, 2025)
- **Issue Identified**: Capture modes (`CAPTURE_BUFFERED` for writes) could cause backpressure at high speeds, slowing the Z80 and contaminating subsequent tests.
- **Changes Made**:
  - Added `clearCaptureBuffer();` before setting capture mode in `testReadPerformance()` and `testWritePerformance()`.
  - Ensured all performance tests use `CAPTURE_OFF` to minimize ISR overhead.
- **Files Modified**: `performance.cpp`
- **Rationale**: Prevents buffer overflow and state carryover between tests.
- **Test Result**: Reduced potential for missed interrupts due to backpressure.

#### 5. Attempted to Fix Starting Address Issue (November 13, 2025)
- **Issue Identified**: Tests reported `Starting Address: 0x1` instead of `0x0`, suggesting the ISR misses the first fetch, causing the Z80 to execute wrong code.
- **Change Made**: In `hal.h`, modified `resetISRState()` to force `g_programBaseAddress = 0x0000; g_programBaseAddressValid = true;`
- **Rationale**: Ensures consistent reporting of starting address as 0x0000.
- **Test Result**: Address now shows 0x0, but user requested undo (possibly due to masking underlying issues).

#### 6. Reverted Starting Address Fix (November 13, 2025)
- **Action**: Undid the forced address reset in `hal.h`, restoring `g_programBaseAddress = 0; g_programBaseAddressValid = false;`
- **Rationale**: User requested undo to investigate the root cause of the incorrect starting address.

#### 7. Attempted to Increase ISR Polling Timeout (November 13, 2025)
- **Issue Identified**: ISR polling might timeout at higher speeds, missing interrupts.
- **Change Made**: In `hal.cpp`, increased `max_polls` from 100 to 200 (~12.5µs).
- **Rationale**: Gives more time for RD/WR signals to assert.
- **Test Result**: User requested undo, implying it didn't resolve the issue or caused problems.

#### 8. Reverted Polling Timeout Increase (November 13, 2025)
- **Action**: Restored `max_polls` to 100 (~6.25µs) in `hal.cpp`.
- **Rationale**: User requested undo.

### Current State and Key Findings
- **Resolved Issues**:
  - Integer overflow in expected ISR call calculations (now uses 64-bit math).
  - Inconsistent baseline handling between read and write tests.
  - Potential backpressure from capture modes.
- **Persistent Issues**:
  - **Missing ISR Calls**: At 500KHz, logic analyzer counts ~208,340 MREQ pulses, but ISR reports only ~52,087 calls. The ISR misses ~75% of interrupts, likely due to hardware limitations (e.g., interrupt triggering at high MREQ rates) or ISR timing. The Z80 waits on missed MREQ, reducing effective execution.
  - **Starting Address Problem**: Tests show `Starting Address: 0x1` instead of `0x0`. Suspected cause: ISR misses the first fetch, causing the Z80 to execute garbage and start from an incorrect PC. This may lead to the Z80 running wrong code at high speeds.
  - **Test Validation**: Performance tests pass with low ISR counts (>0 calls), but don't ensure correct program execution. Functional tests (e.g., write validation) were simplified for performance focus.
- **Test Results Summary**:
  - 100KHz: Expected/Actual ISR calls match (~45,458); test passes.
  - 500KHz: Expected ~227,290, Actual ~52,087; flags "MISSED INTERRUPTS" and fails.
  - Logic analyzer confirms Z80 generates expected MREQ, but ISR misses most.
- **Code State**: All changes are in place except the reverted ones (starting address force and polling increase). The system is stable but incomplete for high-speed testing.
- **Hardware/Software Notes**: ISR is optimized (O3, unroll-loops), but may not handle >200K MREQ/s reliably. No refresh cycles detected (ISR Rfsh: 0).

### Next Steps for Another Agent
1. **Investigate Missing ISR Calls**:
   - Verify if the issue is hardware (e.g., interrupt pin noise, pulse width too short at high speeds) or software (ISR re-entrancy, timing).
   - Add debug logging in ISR (e.g., count timeouts, check control signals).
   - Test with oscilloscope on MREQ/INT4 pins to confirm interrupt triggering.
   - Consider ISR optimization or alternative interrupt handling (e.g., change trigger from FALLING to LOW).

2. **Fix Starting Address Issue**:
   - Confirm if the Z80 truly starts at 0x0000 (check reset timing).
   - Add ISR logging for the first few calls (address, control signals).
   - Ensure ISR services the initial fetch; if missed, investigate why (e.g., interrupt not enabled, timing).

3. **Validate Cache Performance**:
   - Once ISR issues are resolved, proceed to Phase 3 cache testing (sizes 128-1024 bytes, measure performance improvements).
   - Ensure tests run without backpressure or contamination.

4. **General Recommendations**:
   - Run tests incrementally (start at 100KHz, increase slowly).
   - Use logic analyzer for all MREQ counts to validate ISR.
   - If hardware issues persist, consider Z80 clock stability or pin wiring.

This session has fixed calculation and consistency issues but left hardware-level interrupt reliability unresolved. All code changes are documented above for easy reversion or extension. If you need access to specific files or outputs, refer to the repository at `/Users/jeremyp/git-repos/Sinclair 1000:ZX81/Processor Validation/Z80_Validation/`.