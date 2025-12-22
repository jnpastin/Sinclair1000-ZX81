# Reference Materials

This directory contains reference documentation and supporting materials for the Teensy 4.1 migration project.

---

## Contents

### [Teensy_4.1_Specs.md](Teensy_4.1_Specs.md)
Complete technical specifications for Teensy 4.1.

**Topics:**
- Processor specifications (IMXRT1062)
- Memory architecture and sizes
- GPIO capabilities and speeds
- Timer/PWM peripherals
- Communication interfaces
- Physical dimensions and pinout
- Power requirements
- Datasheet references

---

### [Arduino_Mega_Code_Audit.md](Arduino_Mega_Code_Audit.md)
Comprehensive analysis of the existing Arduino Mega implementation.

**Topics:**
- Code inventory and statistics
- Function catalog
- Performance characteristics
- Dependencies and libraries
- Hardware interface details
- Timing analysis
- Memory usage
- Porting considerations

---

### [API_Mapping.md](API_Mapping.md)
Translation guide from AVR/Arduino to ARM/Teensy APIs.

**Topics:**
- GPIO functions (pinMode, digitalWrite, etc.)
- Port manipulation (PORTA → GPIO registers)
- Timer/PWM functions
- Interrupt functions
- Serial communication
- Memory access (PROGMEM, etc.)
- Utility functions
- Code examples for each mapping

---

### [Glossary.md](Glossary.md)
Definitions of terms and abbreviations used throughout the documentation.

**Topics:**
- Hardware terms
- Software terms
- Z80 terminology
- Arduino/Teensy terminology
- Acronyms and abbreviations
- Common concepts

---

## How to Use These Documents

### For Quick Reference
- **Teensy_4.1_Specs.md**: Look up technical specifications
- **API_Mapping.md**: Find equivalent functions during porting

### For Deep Understanding
- **Arduino_Mega_Code_Audit.md**: Understand what needs to be ported
- **Glossary.md**: Clarify unfamiliar terms

### For Migration Work
1. Start with Arduino_Mega_Code_Audit.md to understand the source
2. Use API_Mapping.md as a translation guide
3. Reference Teensy_4.1_Specs.md for platform capabilities
4. Consult Glossary.md for terminology clarification

---

## External Resources

### Teensy Resources
- PJRC Teensy 4.1 Page: https://www.pjrc.com/store/teensy41.html
- Teensy 4.1 Pinout Card: https://www.pjrc.com/teensy/card11a_rev2.pdf
- Teensyduino: https://www.pjrc.com/teensy/teensyduino.html
- PJRC Forum: https://forum.pjrc.com/

### ARM Cortex-M7 Resources
- ARM Cortex-M7 Overview: https://developer.arm.com/Processors/Cortex-M7
- IMXRT1062 Reference Manual: NXP website
- ARM GCC Optimization Guide: GCC documentation

### Z80 Resources
- Z80 User Manual: Zilog website
- Z80 CPU Datasheet: Zilog Z84C00 datasheet
- Z80 Instruction Set: https://clrhome.org/table/

### Development Tools
- PlatformIO Documentation: https://docs.platformio.org/
- Logic Analyzer Software: Sigrok, Saleae Logic
- Oscilloscope Guides: Various manufacturers

---

## Contributing to Reference Materials

When adding or updating reference documents:
1. Keep technical accuracy paramount
2. Include sources and citations
3. Use consistent formatting
4. Add cross-references where helpful
5. Update this README when adding new documents
6. Include date of last update

---

**Last Updated:** December 22, 2025
