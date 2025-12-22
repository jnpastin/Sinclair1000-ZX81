# ATmega2560 External Interrupt Pin Analysis
## Finding Complete Ports with External Interrupt Capability

**Date:** November 6, 2025  
**Purpose:** Identify external interrupt pins on complete 8-bit ports

---

## ATmega2560 External Interrupt Mapping

| INT# | Pin | Port Pin | Arduino Pin | Port Completeness |
|------|-----|----------|-------------|-------------------|
| INT0 | PD0 | D0 | D21 | ⚠️ **PORTD: Incomplete (5 of 8 pins)** |
| INT1 | PD1 | D1 | D20 | ⚠️ **PORTD: Incomplete (5 of 8 pins)** |
| INT2 | PD2 | D2 | D19 | ⚠️ **PORTD: Incomplete (5 of 8 pins)** |
| INT3 | PD3 | D3 | D18 | ⚠️ **PORTD: Incomplete (5 of 8 pins)** |
| INT4 | PE4 | E4 | D2  | ⚠️ **PORTE: Incomplete (6 of 8 pins)** |
| INT5 | PE5 | E5 | D3  | ⚠️ **PORTE: Incomplete (6 of 8 pins)** |
| INT6 | PE6 | E6 | ❌ Not accessible | ⚠️ **PORTE: Incomplete** |
| INT7 | PE7 | E7 | ❌ Not accessible | ⚠️ **PORTE: Incomplete** |

---

## Port Completeness Analysis

### PORTD (INT0-INT3)
```
PD0 (INT0) = D21  ✅ Available (I2C SCL)
PD1 (INT1) = D20  ✅ Available (I2C SDA)
PD2 (INT2) = D19  ✅ Available (Serial1 TX)
PD3 (INT3) = D18  ✅ Available (Serial1 RX)
PD4         = ❌ Not broken out on Mega 2560
PD5         = ❌ Not broken out on Mega 2560
PD6         = ❌ Not broken out on Mega 2560
PD7         = D38  ✅ Available
```
**Status:** ⚠️ **Incomplete - Only 5 of 8 pins accessible**
- Cannot use for 8-bit parallel reading
- Missing PD4, PD5, PD6

### PORTE (INT4-INT7)
```
PE0 = D0  ✅ Available (Serial0 RX)
PE1 = D1  ✅ Available (Serial0 TX)
PE2 = ❌ Not broken out on Mega 2560
PE3 = D5  ✅ Available (PWM)
PE4 (INT4) = D2  ✅ Available
PE5 (INT5) = D3  ✅ Available
PE6 (INT6) = ❌ Not broken out on Mega 2560
PE7 (INT7) = ❌ Not broken out on Mega 2560
```
**Status:** ⚠️ **Incomplete - Only 4 of 8 pins accessible**
- Cannot use for 8-bit parallel reading
- Missing PE2, PE6, PE7

---

## Answer: **NO COMPLETE PORTS WITH EXTERNAL INTERRUPTS**

❌ **There are NO complete 8-bit ports on the Arduino Mega 2560 that have external interrupt capability.**

All external interrupt pins (INT0-INT7) are on incomplete ports:
- **PORTD**: Only 5 of 8 pins broken out
- **PORTE**: Only 4 of 8 pins broken out

---

## Complete Ports on Arduino Mega 2560

For reference, here are the COMPLETE 8-bit ports:

| Port | All 8 Pins? | Arduino Pins | Current Use in Z80 Project |
|------|-------------|--------------|----------------------------|
| **PORTA** | ✅ Yes | D22-D29 (PA0-PA7) | **Z80 Data Bus D0-D7** |
| PORTB | ⚠️ Almost | D53,52,51,50,10-13 | Control outputs + CLK |
| **PORTC** | ✅ Yes | D37-D30 (PC0-PC7) | **Z80 Address Bus A0-A7** |
| PORTD | ❌ No | 5 of 8 pins | Available (I2C, Serial1) |
| PORTE | ❌ No | 4 of 8 pins | Available (Serial0) |
| **PORTF** | ✅ Yes | A0-A7 (PF0-PF7) | Available (Analog) |
| PORTG | ❌ No | 4 of 8 pins | Available |
| PORTH | ❌ No | 4 of 8 pins | Available |
| PORTJ | ❌ No | 2 of 8 pins | Available |
| **PORTK** | ✅ Yes | A8-A15 (PK0-PK7) | **Z80 Address Bus A8-A15** |
| **PORTL** | ✅ Yes | D49-D42 (PL0-PL7) | **Z80 Control Inputs** |

---

## Complete Ports WITHOUT External Interrupts

The following complete ports do NOT have external interrupt capability:

### 1. PORTA (Already Used - Data Bus)
- **Pins:** D22-D29 (PA0-PA7)
- **Current Use:** Z80 Data Bus D0-D7
- **Interrupts:** None
- **Available:** ❌ No (already in use)

### 2. PORTC (Already Used - Address Low)
- **Pins:** D37-D30 (PC0-PC7)
- **Current Use:** Z80 Address Bus A0-A7
- **Interrupts:** None
- **Available:** ❌ No (already in use)

### 3. PORTF (AVAILABLE!)
- **Pins:** A0-A7 (PF0-PF7)
- **Current Use:** None (analog inputs)
- **Interrupts:** None
- **Available:** ✅ Yes (not currently used)
- **Note:** Shared with ADC, but can be used as digital I/O

### 4. PORTK (Already Used - Address High)
- **Pins:** A8-A15 (PK0-PK7)
- **Current Use:** Z80 Address Bus A8-A15
- **Interrupts:** None (but has PCINT16-23 for Pin Change Interrupts)
- **Available:** ❌ No (already in use)

### 5. PORTL (Already Used - Control Signals)
- **Pins:** D49-D42 (PL0-PL7)
- **Current Use:** Z80 Control Inputs (MREQ, IORQ, RD, WR, M1, RFSH, HALT, BUSAK)
- **Interrupts:** None (no external or pin change interrupt support)
- **Available:** ❌ No (already in use)

---

## Implications for Design

### Option A: Use Incomplete Port with External Interrupt
**Compromise:** Accept that the interrupt pin is not on a complete port

**Recommendation:** Use INT4 or INT5 on PORTE
- Move MREQ to Pin D2 (PE4, INT4) or Pin D3 (PE5, INT5)
- Keep all other control signals on PORTL
- Read control signals from PORTL in ISR (works fine)
- External interrupt still faster than pin change interrupt

**Code Impact:**
```cpp
ISR(INT4_vect) {
  // MREQ is active (on PE4, not in PORTL anymore)
  // Read other control signals from PORTL
  register uint8_t control = ~PINL;  // Still works fine
  register uint16_t address = ((uint16_t)PINK << 8) | PINC;
  
  // Check RD/WR from PORTL
  if (control & PORTL_RD_MASK) { /* ... */ }
}
```

### Option B: Move ALL Control Signals to PORTF
**Complete redesign:** Put all control signals on PORTF for consistency

**PORTF Assignment:**
```
PF0 (A0) = MREQ  (no external interrupt, but complete port)
PF1 (A1) = IORQ
PF2 (A2) = RD
PF3 (A3) = WR
PF4 (A4) = M1
PF5 (A5) = RFSH
PF6 (A6) = HALT
PF7 (A7) = BUSAK
```

**Problems:**
- ❌ No external interrupt capability on PORTF
- ❌ Would require polling or timer-based checking
- ❌ Much slower than interrupt-driven approach
- ❌ Major hardware rewiring required

**Verdict:** ❌ Not recommended

### Option C: Use External Interrupt + Polling Combination
**Hybrid approach:** Use external interrupt for speed, accept incomplete port

**Implementation:**
```cpp
// MREQ on INT4 (Pin D2, PE4) for fast interrupt
// Other control signals on PORTL for easy 8-bit read
ISR(INT4_vect) {
  // Fast interrupt triggered by MREQ
  // Read other signals from PORTL
  register uint8_t control = ~PINL;
  // ... handle memory request
}
```

**Verdict:** ✅ **Best approach - already our recommendation**

---

## Recommendation

### **Accept that external interrupt pins are not on complete ports**

**Use INT4 (Pin D2, PE4) for MREQ:**
- ✅ Fastest interrupt response
- ✅ Falling-edge triggered (hardware filtered)
- ✅ Clean, simple code
- ⚠️ MREQ not on same port as other control signals (acceptable)

**Keep other control signals on PORTL (D49-D42):**
- ✅ All 8 control signals on one port
- ✅ Single instruction read: `PINL`
- ✅ Easy to mask and test
- ❌ No interrupt capability (but we only need one interrupt)

**Result:**
- MREQ interrupt response: ~1.38 µs (fastest possible)
- Control signal reading still fast: 1 cycle to read all 8 signals
- Best of both worlds

---

## Alternative: What if we NEEDED a complete port with interrupts?

If this were an absolute requirement, the only option would be:

### **Use Pin Change Interrupts on PORTK**

PORTK is the ONLY complete 8-bit port with interrupt capability:
- **Port:** PORTK (PK0-PK7)
- **Pins:** A8-A15 (Arduino pins 62-69)
- **Interrupts:** PCINT16-PCINT23 (Pin Change Interrupts)
- **Currently used for:** Address Bus A8-A15

**Major redesign required:**
1. Move address bus A8-A15 somewhere else (no good options)
2. Move control signals to PORTK
3. Use pin change interrupts (slower than external interrupts)
4. Complex address bus reading (split across multiple ports)

**Verdict:** ❌ **Not worth it** - far too complex for minimal benefit

---

## Conclusion

**There are NO complete 8-bit ports with external interrupt capability on the Arduino Mega 2560.**

This is a hardware limitation of the ATmega2560 chip - external interrupts are only available on PORTD and PORTE, neither of which has all 8 pins broken out on the Arduino Mega 2560 board.

**Recommended Solution:**
- Use INT4 (Pin D2, PE4) for MREQ interrupt
- Keep other control signals on PORTL (D49-D42)
- Accept that MREQ is not on the same port (no practical disadvantage)
- This gives the best performance while maintaining clean design

---

## Summary Table

| Requirement | Complete Port? | External INT? | Pin Change INT? | Recommendation |
|-------------|----------------|---------------|-----------------|----------------|
| **MREQ (interrupt needed)** | Not required | ✅ **INT4** | Alternative | **Use INT4 (D2)** |
| **Control signals (grouped read)** | ✅ **PORTL** | ❌ No | ❌ No | **Keep on PORTL** |
| **Address bus (16-bit read)** | ✅ **PORTC+PORTK** | ❌ No | PORTK only | **Keep as is** |
| **Data bus (bidirectional)** | ✅ **PORTA** | ❌ No | ❌ No | **Keep as is** |

**All requirements met with current design + moving MREQ to INT4.**

---

**End of Analysis**
