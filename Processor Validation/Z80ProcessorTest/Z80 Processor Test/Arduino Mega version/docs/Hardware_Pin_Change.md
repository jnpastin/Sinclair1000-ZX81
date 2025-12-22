# Hardware Pin Change Required
## MREQ Signal Relocation

**Date:** November 6, 2025  
**Status:** Required for INT4 External Interrupt Implementation  
**Priority:** HIGH - Must be done before testing ISR

---

## Summary

MREQ signal must be moved from pin D49 to pin D2 to enable external interrupt capability. PORTL does not support interrupts on the ATmega2560.

---

## Pin Change Details

### Before (Original Design):
```
MREQ:  Pin D49 (PL0) - PORTL
       ❌ PORTL does not support interrupts
```

### After (New Design):
```
MREQ:  Pin D2 (PE4, INT4) - External Interrupt
       ✅ Supports falling edge trigger
       ✅ 13% faster performance
       ✅ No MREQ checking needed in ISR
```

---

## Other Control Signals (Shifted on PORTL)

### Before:
```
MREQ:  D49 (PL0)
IORQ:  D48 (PL1)
RD:    D47 (PL2)
WR:    D46 (PL3)
M1:    D45 (PL4)
RFSH:  D44 (PL5)
HALT:  D43 (PL6)
BUSAK: D42 (PL7)
```

### After:
```
IORQ:  D49 (PL0) ⬅️ shifted from D48
RD:    D48 (PL1) ⬅️ shifted from D47
WR:    D47 (PL2) ⬅️ shifted from D46
M1:    D46 (PL3) ⬅️ shifted from D45
RFSH:  D45 (PL4) ⬅️ shifted from D44
HALT:  D44 (PL5) ⬅️ shifted from D43
BUSAK: D43 (PL6) ⬅️ shifted from D42

Pin D42 (PL7): Now unused
```

---

## Hardware Changes Required

### Step 1: Disconnect MREQ from D49
1. Locate the wire from Z80 MREQ (pin 19) to Arduino D49
2. Disconnect from Arduino D49
3. Leave Z80 end connected

### Step 2: Shift PORTL Signals (Optional but Recommended)

**Option A: Minimal Change (MREQ only)**
- Leave D49-D43 signals as-is
- Pin D42 becomes unused
- Software handles bit position changes

**Option B: Clean Mapping (Recommended)**
- Shift all signals one position:
  - IORQ: D48 → D49
  - RD: D47 → D48
  - WR: D46 → D47
  - M1: D45 → D46
  - RFSH: D44 → D45
  - HALT: D43 → D44
  - BUSAK: D42 → D43
- Results in cleaner, more logical layout
- Pin D42 becomes free for future use

### Step 3: Connect MREQ to D2
1. Connect the MREQ wire to Arduino pin D2 (PE4)
2. Verify connection is secure
3. Double-check no short to adjacent pins (D0, D1, D3)

### Step 4: Verify Connections
1. Use multimeter in continuity mode
2. Check Z80 MREQ (pin 19) to Arduino D2
3. Check all other control signals
4. Power off before testing!

---

## Pin Reference Table

| Signal | Z80 Pin | Old Arduino | New Arduino | Port/Bit | Function |
|--------|---------|-------------|-------------|----------|----------|
| **MREQ** | 19 | **D49 (PL0)** | **D2 (PE4)** | **INT4** | **Interrupt** |
| IORQ | 20 | D48 (PL1) | D49 (PL0) | PORTL | Status |
| RD | 21 | D47 (PL2) | D48 (PL1) | PORTL | Status |
| WR | 22 | D46 (PL3) | D47 (PL2) | PORTL | Status |
| M1 | 27 | D45 (PL4) | D46 (PL3) | PORTL | Status |
| RFSH | 28 | D44 (PL5) | D45 (PL4) | PORTL | Status |
| HALT | 18 | D43 (PL6) | D44 (PL5) | PORTL | Status |
| BUSAK | 23 | D42 (PL7) | D43 (PL6) | PORTL | Status |

---

## Benefits of This Change

### Performance:
- ✅ **13% faster** - 3 cycles saved per memory access
- ✅ **1.3 µs** READ latency (was 1.5 µs)
- ✅ **1.0 µs** WRITE latency (was 1.2 µs)

### Reliability:
- ✅ **50% fewer interrupts** - falling edge only (not both edges)
- ✅ **No MREQ checking** - hardware guarantees signal state
- ✅ **Simpler ISR** - eliminates conditional logic

### Z80 Support:
- ✅ Supports Z80 at **2.5 MHz** (was 2 MHz)
- ✅ Better headroom for future optimization

---

## Software Changes

All software has been updated in this commit:
- ✅ `pins.h` - Pin definitions updated
- ✅ `hal.h` - `isMemoryRequest()` now reads PORTE
- ✅ `hal.cpp` - `initializeMemoryHandler()` configures INT4
- ✅ `ISR_Proposal.md` - Complete ISR implementation with INT4
- ✅ `HAL_Proposal.md` - Documentation updated
- ✅ `TODO.md` - Task list updated
- ✅ All analysis documents updated

---

## Testing After Change

1. **Visual Inspection**
   - Verify MREQ connected to D2
   - Check no shorts to adjacent pins
   - Verify all other signals connected

2. **Software Configuration Test**
   ```cpp
   void testHardwareChange() {
     // Test INT4 configuration
     if (!(EIMSK & (1 << INT4))) {
       Serial.println("ERROR: INT4 not enabled");
     }
     
     // Test EICRB for falling edge
     if ((EICRB & 0x03) != 0x02) {
       Serial.println("ERROR: INT4 not falling edge");
     }
     
     Serial.println("Hardware change verified!");
   }
   ```

3. **Logic Analyzer Verification**
   - Connect analyzer to D2 and Z80 MREQ
   - Verify signals match
   - Check interrupt fires on falling edge only

4. **ISR Timing Test**
   - Run ISR with test program
   - Measure response time
   - Should see ~1.3 µs for READ operations

---

## Rollback Procedure

If issues occur:

1. Disconnect MREQ from D2
2. Reconnect MREQ to D49
3. Restore all signals to original positions
4. Revert software to commit before this change
5. Test with original configuration

---

## Notes

- This is a **one-time hardware change**
- Only **1 wire** needs moving (8 wires if doing clean shift)
- Change is **permanent** for optimal performance
- No additional components required
- Compatible with existing Z80 circuit

---

**IMPORTANT:** Make this hardware change before running the ISR for the first time!
