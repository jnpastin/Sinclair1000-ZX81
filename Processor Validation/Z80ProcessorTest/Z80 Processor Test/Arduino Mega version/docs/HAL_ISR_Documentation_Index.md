# HAL and ISR Documentation Index
## Complete Guide to Hardware Abstraction Layer Implementation

**Date:** November 6, 2025  
**Status:** Current - All documents updated for INT4 implementation

---

## Quick Navigation

### 🚨 Start Here - Critical Documents

1. **[Hardware_Pin_Change.md](Hardware_Pin_Change.md)** - **MUST READ FIRST**
   - Hardware modification required before testing
   - Move MREQ from D49 to D2
   - Step-by-step instructions with diagrams
   - Testing and verification procedures

2. **[ISR_Proposal.md](ISR_Proposal.md)** - **IMPLEMENTATION GUIDE**
   - Complete ISR implementation
   - Ready-to-use code in Appendix A
   - Testing procedures
   - Performance specifications

3. **[HAL_Proposal.md](HAL_Proposal.md)** - **ARCHITECTURE OVERVIEW**
   - Complete HAL design
   - All HAL functions documented
   - Memory management strategy
   - Integration guidelines

---

## Documentation by Purpose

### For Hardware Setup

**Primary:** [Hardware_Pin_Change.md](Hardware_Pin_Change.md)
- What to change on hardware
- Why the change is necessary
- How to test after modification
- Rollback procedure if needed

**Supporting:** [hardware/pin_mapping.md](hardware/pin_mapping.md)
- Complete pin-to-pin reference
- Updated for new MREQ location
- All port assignments
- Connection diagrams

### For Implementation

**Primary:** [ISR_Proposal.md](ISR_Proposal.md)
- Complete ISR code listing (Appendix A)
- Configuration setup code
- Error handling strategies
- Testing framework

**Supporting:** [HAL_Proposal.md](HAL_Proposal.md)
- HAL function specifications
- API reference
- Memory emulation approach
- Diagnostic functions

### For Understanding Design Decisions

**Why INT4 was chosen:**
1. [ISR_Performance_Detailed.md](ISR_Performance_Detailed.md) - Detailed cycle comparison
2. [Interrupt_Performance_Analysis.md](Interrupt_Performance_Analysis.md) - External vs Pin Change
3. [MREQ_Interrupt_Solutions.md](MREQ_Interrupt_Solutions.md) - All 4 options evaluated

**Port selection analysis:**
- [Port_Interrupt_Analysis.md](Port_Interrupt_Analysis.md) - Why no complete ports with interrupts

### For Project Management

**Complete changelog:** [Pin_Mapping_Update_Summary.md](Pin_Mapping_Update_Summary.md)
- All 10 files modified
- Line-by-line changes
- Testing checklist
- Migration guide

---

## Document Relationships

```
Hardware_Pin_Change.md (START HERE!)
    ↓
ISR_Proposal.md (IMPLEMENT THIS)
    ↓
HAL_Proposal.md (UNDERSTAND CONTEXT)
    ↓
Performance Analysis Documents (WHY IT WORKS)
```

### Dependency Map

```
Hardware_Pin_Change.md
    ├── References: hardware/pin_mapping.md
    ├── References: ISR_Proposal.md
    └── References: Pin_Mapping_Update_Summary.md

ISR_Proposal.md
    ├── References: HAL_Proposal.md
    ├── References: ISR_Performance_Detailed.md
    └── Implements: INT4 external interrupt

HAL_Proposal.md
    ├── Defines: All HAL functions
    ├── Defines: Memory strategy
    └── References: ISR_Proposal.md

Performance Analysis
    ├── ISR_Performance_Detailed.md (PORTB vs INT4)
    ├── Interrupt_Performance_Analysis.md (INT vs PCINT)
    ├── MREQ_Interrupt_Solutions.md (4 solutions)
    └── Port_Interrupt_Analysis.md (port capabilities)
```

---

## Reading Paths

### Path 1: Quick Implementation (Minimal Reading)

1. Read warning in [Hardware_Pin_Change.md](Hardware_Pin_Change.md)
2. Perform hardware modification
3. Copy ISR code from [ISR_Proposal.md](ISR_Proposal.md) Appendix A
4. Run tests
5. Done!

**Time:** ~30 minutes hardware + 15 minutes code

### Path 2: Complete Understanding (Full Context)

1. **Hardware Context** - [hardware/pin_mapping.md](hardware/pin_mapping.md) (15 min)
2. **HAL Architecture** - [HAL_Proposal.md](HAL_Proposal.md) (45 min)
3. **Design Constraints** - [Port_Interrupt_Analysis.md](Port_Interrupt_Analysis.md) (20 min)
4. **Solution Options** - [MREQ_Interrupt_Solutions.md](MREQ_Interrupt_Solutions.md) (30 min)
5. **Performance Comparison** - [ISR_Performance_Detailed.md](ISR_Performance_Detailed.md) (30 min)
6. **Interrupt Details** - [Interrupt_Performance_Analysis.md](Interrupt_Performance_Analysis.md) (20 min)
7. **ISR Implementation** - [ISR_Proposal.md](ISR_Proposal.md) (45 min)
8. **Hardware Change** - [Hardware_Pin_Change.md](Hardware_Pin_Change.md) (20 min)
9. **Changelog Review** - [Pin_Mapping_Update_Summary.md](Pin_Mapping_Update_Summary.md) (15 min)

**Total Time:** ~4 hours for complete understanding

### Path 3: Troubleshooting (Problem-Focused)

**Problem: Hardware not working**
→ [Hardware_Pin_Change.md](Hardware_Pin_Change.md) - Testing section

**Problem: ISR not firing**
→ [ISR_Proposal.md](ISR_Proposal.md) - Testing & Verification section

**Problem: Performance too slow**
→ [ISR_Performance_Detailed.md](ISR_Performance_Detailed.md) - Performance tables

**Problem: Understanding design decision**
→ [Pin_Mapping_Update_Summary.md](Pin_Mapping_Update_Summary.md) - Rationale section

---

## Document Summary Table

| Document | Pages | Purpose | Audience | Priority |
|----------|-------|---------|----------|----------|
| **Hardware_Pin_Change.md** | ~5 | Hardware modification guide | All users | 🔴 CRITICAL |
| **ISR_Proposal.md** | ~25 | ISR implementation spec | Implementers | 🔴 CRITICAL |
| **HAL_Proposal.md** | ~30 | HAL architecture | Architects | 🟡 Important |
| **ISR_Performance_Detailed.md** | ~15 | Cycle-by-cycle analysis | Optimizers | 🟢 Reference |
| **Interrupt_Performance_Analysis.md** | ~10 | INT vs PCINT comparison | Decision makers | 🟢 Reference |
| **MREQ_Interrupt_Solutions.md** | ~12 | Four solution options | Decision makers | 🟢 Reference |
| **Port_Interrupt_Analysis.md** | ~8 | Port capability analysis | Architects | 🟢 Reference |
| **Pin_Mapping_Update_Summary.md** | ~8 | Complete changelog | All users | 🟡 Important |
| **hardware/pin_mapping.md** | ~10 | Pin reference | All users | 🟡 Important |

---

## Key Technical Specifications

### Performance (from ISR_Proposal.md)

| Metric | Value | Notes |
|--------|-------|-------|
| READ latency | 1.3 µs | 19-25 cycles @ 16 MHz |
| WRITE latency | 1.0 µs | 15-21 cycles @ 16 MHz |
| Max Z80 speed | 2.5 MHz | With current implementation |
| ISR overhead | 50% less | vs pin change interrupt |
| Cycle savings | 3 cycles | MREQ checking eliminated |

### Pin Assignments (from Hardware_Pin_Change.md)

| Signal | Old Pin | New Pin | Port | Interrupt |
|--------|---------|---------|------|-----------|
| MREQ | D49 (PL0) | **D2 (PE4)** | PORTE | **INT4** |
| IORQ | D48 (PL1) | D49 (PL0) | PORTL | - |
| RD | D47 (PL2) | D48 (PL1) | PORTL | - |
| WR | D46 (PL3) | D47 (PL2) | PORTL | - |
| M1 | D45 (PL4) | D46 (PL3) | PORTL | - |
| RFSH | D44 (PL5) | D45 (PL4) | PORTL | - |
| HALT | D43 (PL6) | D44 (PL5) | PORTL | - |
| BUSAK | D42 (PL7) | D43 (PL6) | PORTL | - |

---

## Common Questions

### Q: Why did MREQ move?
**A:** PORTL doesn't support interrupts on ATmega2560. See [Port_Interrupt_Analysis.md](Port_Interrupt_Analysis.md).

### Q: Why not move all signals to PORTB?
**A:** Would require moving 8 wires vs 1 wire, and only 3 cycles faster than PCINT. See [MREQ_Interrupt_Solutions.md](MREQ_Interrupt_Solutions.md).

### Q: Why external interrupt instead of pin change?
**A:** 13% faster, 50% fewer interrupts, simpler code. See [ISR_Performance_Detailed.md](ISR_Performance_Detailed.md).

### Q: Do I need to check MREQ in the ISR?
**A:** No! External interrupt guarantees MREQ is active. See [ISR_Proposal.md](ISR_Proposal.md) section 3.1.

### Q: Can I run Z80 at 4 MHz?
**A:** Not yet. Current implementation supports up to 2.5 MHz. Would need WAIT states for higher speeds. See [HAL_Proposal.md](HAL_Proposal.md) section 3.5.

### Q: Where's the complete ISR code?
**A:** [ISR_Proposal.md](ISR_Proposal.md) - Appendix A (bottom of document). Ready to copy and use.

---

## Testing Checklist

### Hardware Verification
- [ ] MREQ wire moved from D49 to D2
- [ ] All other signals shifted (IORQ D48→D49, etc.)
- [ ] Continuity test: Z80 pin 19 to Arduino D2
- [ ] No shorts to adjacent pins (D0, D1, D3)
- [ ] Power connections verified

### Software Verification
- [ ] INT5 and INT4 configuration code added
- [ ] ISR code added to hal.cpp
- [x] pins.h updated (Z80_MREQ = 47, Z80_RD = 3, Z80_WR = 6)
- [ ] hal.h updated (isMemoryRequest() uses PINL, isReadCycle() uses PINE, isWriteCycle() uses PINH)
- [ ] Code compiles without errors

### Functional Testing
- [ ] INT4 enabled (EIMSK register)
- [ ] Falling edge configured (EICRB register)
- [ ] ISR fires on MREQ falling edge (logic analyzer)
- [ ] READ operations work (test program execution)
- [ ] WRITE operations captured correctly
- [ ] No spurious interrupts on rising edge

### Performance Testing
- [ ] READ latency < 1.5 µs (target: 1.3 µs)
- [ ] WRITE latency < 1.3 µs (target: 1.0 µs)
- [ ] Z80 runs at 500 KHz (initial test)
- [ ] Z80 runs at 1 MHz (primary target)
- [ ] Z80 runs at 2 MHz (stretch goal)

---

## Implementation Timeline

### Phase 1: Hardware (30 minutes)
1. Power off all hardware
2. Disconnect MREQ from D49
3. Connect MREQ to D2
4. Verify with multimeter
5. Document with photo

### Phase 2: Software Update (15 minutes)
1. Update pins.h (if not already done)
2. Update hal.h (if not already done)
3. Update hal.cpp interrupt config
4. Add ISR code from Appendix A
5. Compile and verify no errors

### Phase 3: Testing (1 hour)
1. Configuration test (INT4 setup)
2. Interrupt trigger test (logic analyzer)
3. Simple program test (NOP execution)
4. WRITE capture test
5. Performance measurement

### Phase 4: Validation (ongoing)
1. Run comprehensive test suite
2. Measure timing at multiple clock speeds
3. Stress test (extended operation)
4. Document results

---

## Support and Resources

### If You Get Stuck

1. **Hardware Issues**: See Testing section in [Hardware_Pin_Change.md](Hardware_Pin_Change.md)
2. **Software Issues**: See Testing section in [ISR_Proposal.md](ISR_Proposal.md)
3. **Understanding Issues**: Read the Complete Understanding path above
4. **Performance Issues**: See [ISR_Performance_Detailed.md](ISR_Performance_Detailed.md)

### Additional Documentation

- Main project README: `../README.md`
- TODO list: `../TODO.md`
- Clock documentation: `clock/CLOCK_README.md`
- Memory guidelines: `development/PROGMEM_USAGE.md`

---

## Version History

- **November 6, 2025** - Initial index created
  - Covers all HAL/ISR documentation
  - Provides multiple reading paths
  - Includes quick reference tables
  - Links all related documents

---

**Next Steps:**
1. Read [Hardware_Pin_Change.md](Hardware_Pin_Change.md)
2. Perform hardware modification
3. Implement ISR from [ISR_Proposal.md](ISR_Proposal.md)
4. Test and validate
5. Enjoy Z80 validation at 2.5 MHz! 🎉
