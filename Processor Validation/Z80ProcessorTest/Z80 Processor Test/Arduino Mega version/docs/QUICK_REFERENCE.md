# Quick Reference Card
## Z80 HAL/ISR Implementation

**Date:** November 11, 2025  
**Status:** ✅ Phase 1-2 COMPLETE - Full READ/WRITE support with ring buffer
**Print this page for quick reference during implementation**

---

## 🚨 CRITICAL: Hardware Change Required

**MREQ moved from D49 → D2**

Before testing ISR, move ONE wire:
- Disconnect Z80 pin 19 (MREQ) from Arduino D49
- Connect Z80 pin 19 (MREQ) to Arduino D2

See: [Hardware_Pin_Change.md](Hardware_Pin_Change.md)

---

## ✅ Implementation Status

### Phase 1: Core ISR Infrastructure ✅ COMPLETE
- Basic ISR with READ support
- Program loading and caching
- Test program execution

### Phase 2: WRITE Support ✅ COMPLETE  
- WRITE detection and capture
- Validation functions
- Integration testing (6 total tests)
- Data direction management
- Timing fixes for WR detection

### Phase 5: Ring Buffer & Backpressure ✅ COMPLETE
- 256-entry ring buffer
- Hybrid WAIT + clock throttling
- Multiple capture modes

**Ready for Phase 3: Optimization**

## Pin Assignments (NEW)

| Signal | Z80 Pin | Arduino Pin | Port/Bit | Notes |
|--------|---------|-------------|----------|-------|
| **MREQ** | 19 | **D2** | **PE4 (INT4)** | **Interrupt** |
| IORQ | 20 | D49 | PL0 | Shifted from D48 |
| RD | 21 | D48 | PL1 | Shifted from D47 |
| WR | 22 | D47 | PL2 | Shifted from D46 |
| M1 | 27 | D46 | PL3 | Shifted from D45 |
| RFSH | 28 | D45 | PL4 | Shifted from D44 |
| HALT | 18 | D44 | PL5 | Shifted from D43 |
| BUSAK | 23 | D43 | PL6 | Shifted from D42 |

---

## Performance Specifications

| Metric | Value |
|--------|-------|
| READ latency | 1.3 µs |
| WRITE latency | 1.0 µs |
| Max Z80 clock | 2.5 MHz |
| ISR polling timeout | 100 cycles (~6.25µs) |
| Ring buffer size | 256 entries |
| Capture modes | OFF, BUFFERED, IMMEDIATE |

---

## Essential Code Snippets

### 1. Configure INT4 Interrupt
```cpp
void initializeMemoryHandler() {
  // Configure INT4 for falling edge
  EICRB &= ~(1 << ISC40);  // Clear bit 0
  EICRB |=  (1 << ISC41);  // Set bit 1 → falling edge
  
  // Enable INT4
  EIMSK |= (1 << INT4);
  
  // Clear pending interrupts
  EIFR |= (1 << INTF4);
  
  // Enable global interrupts
  sei();
}
```

### 2. ISR Implementation
```cpp
ISR(INT4_vect) __attribute__((hot, flatten));

ISR(INT4_vect) {
  // Read control signals (MREQ guaranteed active)
  register uint8_t control = ~PINL;
  register uint16_t address = ((uint16_t)PINK << 8) | PINC;
  
  // READ operation
  if (control & PORTL_RD_MASK) {
    DDRA = 0xFF;
    PORTA = pgm_read_byte(&g_testProgram[address]);
    return;
  }
  
  // WRITE operation
  else if (control & PORTL_WR_MASK) {
    DDRA = 0x00;
    g_capturedData = PINA;
    g_lastWriteAddress = address;
    return;
  }
  
  // Unexpected state
  DDRA = 0x00;
}
```

### 3. Check MREQ State (non-ISR)
```cpp
// MREQ now on PORTE, bit 4 (active low)
bool mreq_active = (PINE & (1 << PE4)) == 0;

// Or use the HAL function
bool mreq_active = isMemoryRequest();
```

---

## Testing Checklist

### Hardware
- [ ] MREQ wire moved from D49 to D2
- [ ] Continuity: Z80 pin 19 to Arduino D2
- [ ] No shorts to D0, D1, D3

### Software
- [ ] INT5 and INT4 configuration added
- [ ] ISR code added to hal.cpp
- [x] pins.h: Z80_MREQ = 47, Z80_RD = 3, Z80_WR = 6
- [ ] hal.h: isMemoryRequest() uses PINL, isReadCycle() uses PINE, isWriteCycle() uses PINH
- [ ] Code compiles

### Functional
- [ ] INT4 enabled in EIMSK
- [ ] Falling edge in EICRB
- [ ] ISR fires on falling edge
- [ ] READ operations work
- [ ] WRITE captured correctly

---

## Common Issues

**ISR not firing?**
→ Check EIMSK register: `Serial.println(EIMSK, BIN);`  
→ Should show bit 4 set (0x10)

**Wrong edge triggering?**
→ Check EICRB register: `Serial.println(EICRB, BIN);`  
→ Should show bits 1:0 = 0b10 (falling edge)

**MREQ always active?**
→ Check wire connection  
→ Verify Z80 pin 19 to Arduino D2

**Performance too slow?**
→ Use register variables  
→ Add __attribute__((hot, flatten))  
→ Verify compiler optimization (-O2 or -O3)

---

## Port Bit Masks

```cpp
// PORTL (D49-D43) - 7 control signals
#define PORTL_IORQ_BIT   0
#define PORTL_RD_BIT     1
#define PORTL_WR_BIT     2
#define PORTL_M1_BIT     3
#define PORTL_RFSH_BIT   4
#define PORTL_HALT_BIT   5
#define PORTL_BUSAK_BIT  6

#define PORTL_RD_MASK    (1 << PORTL_RD_BIT)
#define PORTL_WR_MASK    (1 << PORTL_WR_BIT)

// PORTE (D2) - MREQ
#define PORTE_MREQ_BIT   4
#define PORTE_MREQ_MASK  (1 << PORTE_MREQ_BIT)
```

---

## Register Configuration Values

```cpp
// INT4 configuration
EICRB = 0x02;   // Bits 1:0 = 10 (falling edge)
EIMSK = 0x10;   // Bit 4 = 1 (INT4 enabled)

// Verify with:
if (!(EIMSK & (1 << INT4))) {
  Serial.println("ERROR: INT4 not enabled");
}
if ((EICRB & 0x03) != 0x02) {
  Serial.println("ERROR: INT4 not falling edge");
}
```

---

## Documentation Quick Links

**Start Here:** [HAL_ISR_Documentation_Index.md](HAL_ISR_Documentation_Index.md)

**Hardware:** [Hardware_Pin_Change.md](Hardware_Pin_Change.md)

**Implementation:** [ISR_Proposal.md](ISR_Proposal.md) Appendix A

**Reference:** [hardware/pin_mapping.md](hardware/pin_mapping.md)

**Troubleshooting:** See Testing sections in ISR_Proposal.md

---

## Implementation Steps (Quick Path)

1. **Read** [Hardware_Pin_Change.md](Hardware_Pin_Change.md) (5 min)
2. **Move** MREQ wire from D49 to D2 (10 min)
3. **Test** continuity with multimeter (5 min)
4. **Copy** ISR code from ISR_Proposal.md Appendix A (5 min)
5. **Add** initializeMemoryHandler() config (5 min)
6. **Compile** and upload (5 min)
7. **Test** with simple program (10 min)

**Total: ~45 minutes**

---

## Key Points to Remember

✅ **MREQ is on D2 (PE4), not D49 (PL0) anymore**  
✅ **INT4 only fires on falling edge (MREQ active)**  
✅ **No need to check MREQ state in ISR**  
✅ **Use register variables for speed**  
✅ **Test at 500 KHz first, then increase**  

---

*For complete documentation, see [HAL_ISR_Documentation_Index.md](HAL_ISR_Documentation_Index.md)*
