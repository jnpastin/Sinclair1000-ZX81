# Z80 Validation Suite Documentation

## Overview
This directory contains comprehensive documentation for the Z80 Processor Validation Suite project.

## Documentation Structure

### Clock Module (`clock/`)
Documentation for the hardware clock generation system.

- **[CLOCK_README.md](clock/CLOCK_README.md)** - Complete API reference and usage guide
  - Features and capabilities
  - API function reference
  - Usage examples
  - Technical details
  - Hardware connections

- **[CLOCK_TROUBLESHOOTING.md](clock/CLOCK_TROUBLESHOOTING.md)** - Diagnostic and troubleshooting guide
  - Common issues and solutions
  - Hardware verification steps
  - Frequency measurement guide
  - Command reference

### Development Guidelines (`development/`)
Best practices and standards for developing the validation suite.

- **[PROGMEM_USAGE.md](development/PROGMEM_USAGE.md)** - Memory management guidelines
  - PROGMEM implementation patterns
  - RAM conservation strategies
  - Current memory usage
  - Future implementation guidelines

### Hardware Documentation (`hardware/`)
Pin mappings, schematics, and hardware setup information.

- **[pin_mapping.md](hardware/pin_mapping.md)** - Complete pin-to-pin connection reference
  - Z80 pinout reference
  - Arduino Mega connection tables
  - Port assignments
  - Signal descriptions

- **[pin_mapping.csv](hardware/pin_mapping.csv)** - Machine-readable pin mapping
  - CSV format for automation
  - Scripting and code generation
  - Validation tools

- **[separate_isr_implementation_plan.md](separate_isr_implementation_plan.md)** - Separate RD/WR ISR implementation guide ⚠️ **CRITICAL**
  - Hardware modification procedure
  - Step-by-step instructions
  - Before/after pin tables
  - Testing and verification
  - Rollback procedure

### Physical Hardware (`../hardware/`)
Schematics, construction guides, and test rig documentation.

- **README.md** - Hardware construction overview
- **Z80 Test Rig/** - KiCad schematic files

### HAL (Hardware Abstraction Layer) Documentation
Architecture and implementation of the HAL system.

📋 **[HAL_ISR_Documentation_Index.md](HAL_ISR_Documentation_Index.md)** - **START HERE** for HAL/ISR work
- Complete navigation guide for all HAL documents
- Multiple reading paths (quick, complete, troubleshooting)
- Document relationships and dependencies
- Testing checklist and implementation timeline

- **[HAL_Proposal.md](HAL_Proposal.md)** - Complete HAL design specification
  - Architecture overview
  - Core HAL functions (bus control, signal monitoring)
  - ISR implementation strategy
  - Memory emulation approach
  - Performance specifications
  - Testing strategy

- **[ISR_Proposal.md](ISR_Proposal.md)** - Interrupt Service Routine implementation ⭐ **KEY DOCUMENT**
  - ISR design and optimization
  - External interrupt (INT4) configuration
  - Cycle-by-cycle performance analysis
  - Error handling and edge cases
  - Complete ISR code listing
  - Testing and verification procedures

### Performance Analysis & Design Decisions
Technical analysis documents supporting design choices.

- **[ISR_Performance_Detailed.md](ISR_Performance_Detailed.md)** - Detailed ISR performance comparison
  - PORTB (PCINT) vs INT4 (External) analysis
  - Cycle-by-cycle breakdown
  - Maximum Z80 clock speed analysis
  - Real-world impact assessment

- **[Interrupt_Performance_Analysis.md](Interrupt_Performance_Analysis.md)** - External vs Pin Change interrupts
  - ATmega2560 interrupt types
  - Performance comparison tables
  - Z80 clock speed impact analysis
  - Configuration code examples

- **[MREQ_Interrupt_Solutions.md](MREQ_Interrupt_Solutions.md)** - Alternative solutions evaluated
  - Four solution options analyzed
  - Port unity vs performance tradeoffs
  - PORTB vs INT4+PORTL comparison
  - Recommendation matrix

- **[Port_Interrupt_Analysis.md](Port_Interrupt_Analysis.md)** - Port interrupt capability analysis
  - ATmega2560 port completeness survey
  - External interrupt pin mapping
  - Design implications and constraints

- **[Pin_Mapping_Update_Summary.md](Pin_Mapping_Update_Summary.md)** - Complete change log 📋 **CHANGELOG**
  - All files modified (10 files)
  - Line-by-line change summary
  - Technical improvements quantified
  - Testing status checklist
  - Migration guide

## Quick Links

### Getting Started
1. Review hardware setup: [hardware/pin_mapping.md](hardware/pin_mapping.md)
2. ⚠️ **CRITICAL**: Review hardware change: [Hardware_Pin_Change.md](Hardware_Pin_Change.md)
3. 📋 **Quick Reference**: [QUICK_REFERENCE.md](QUICK_REFERENCE.md) - Print this for implementation!
4. Understand clock operation: [clock/CLOCK_README.md](clock/CLOCK_README.md)
5. Review HAL architecture: [HAL_Proposal.md](HAL_Proposal.md)
6. Understand ISR implementation: [ISR_Proposal.md](ISR_Proposal.md)
7. Review memory guidelines: [development/PROGMEM_USAGE.md](development/PROGMEM_USAGE.md)

### Key Technical Documents
- **Pin Mapping Change**: [Hardware_Pin_Change.md](Hardware_Pin_Change.md) - **Must read before hardware testing!**
- **ISR Implementation**: [ISR_Proposal.md](ISR_Proposal.md) - Complete ISR code and testing
- **Performance Analysis**: [ISR_Performance_Detailed.md](ISR_Performance_Detailed.md) - Why INT4 was chosen
- **Complete Changelog**: [Pin_Mapping_Update_Summary.md](Pin_Mapping_Update_Summary.md) - All changes documented

### Troubleshooting
- Clock not working? → [clock/CLOCK_TROUBLESHOOTING.md](clock/CLOCK_TROUBLESHOOTING.md)
- Pin connections? → [hardware/pin_mapping.md](hardware/pin_mapping.md)
- Hardware change needed? → [Hardware_Pin_Change.md](Hardware_Pin_Change.md)
- Memory issues? → [development/PROGMEM_USAGE.md](development/PROGMEM_USAGE.md)
- ISR performance? → [ISR_Performance_Detailed.md](ISR_Performance_Detailed.md)

### Development
- Adding new features? → Review [development/PROGMEM_USAGE.md](development/PROGMEM_USAGE.md)
- Creating new modules? → Follow patterns in clock module
- Understanding HAL? → Read [HAL_Proposal.md](HAL_Proposal.md)
- Optimizing performance? → See [ISR_Performance_Detailed.md](ISR_Performance_Detailed.md)

## Documentation Standards

### File Organization
- Module-specific docs in subdirectories (e.g., `clock/`, `hal/`, `memory/`)
- Cross-cutting concerns in `development/`
- Hardware documentation in `hardware/`
- Physical hardware in `../hardware/`
- Test documentation in `../instructions/`, `../timing/`, etc.
- Design analysis documents in root `docs/` (e.g., ISR proposals, performance analysis)

### Naming Conventions
- README files for primary documentation
- UPPERCASE for major reference documents
- Descriptive names (e.g., `CLOCK_TROUBLESHOOTING.md`)
- Technical proposals use PascalCase (e.g., `HAL_Proposal.md`, `ISR_Proposal.md`)
- Analysis documents describe content (e.g., `ISR_Performance_Detailed.md`)

### Content Guidelines
- Include working code examples
- Provide troubleshooting sections
- Document memory usage
- Show command references
- Include hardware specifications

## Future Documentation

### Planned Additions

**HAL Module (`hal/`)** ✅ **COMPLETED - See root docs/**
- ✅ HAL_Proposal.md - Hardware Abstraction Layer design (COMPLETE)
- ✅ ISR_Proposal.md - Interrupt Service Routine implementation (COMPLETE)
- ✅ Performance analysis documents (COMPLETE)
- 🔲 HAL API reference (extract from HAL_Proposal.md)
- 🔲 Bus operation guide
- 🔲 Direct port I/O patterns

**Memory Module (`memory/`)**
- MEMORY_README.md - Memory emulation system
- Addressing modes
- Access patterns
- Performance optimization
- PROGMEM vs SRAM strategies (see development/PROGMEM_USAGE.md)

**Test Framework (`testing/`)**
- TEST_FRAMEWORK_README.md - Test system architecture
- Writing test cases
- Test result interpretation
- Coverage reports

**Instruction Testing (`instructions/`)**
- Instruction group guides
- Test methodology
- Expected results tables
- Flag verification

## Contributing

When adding new documentation:
1. Place in appropriate subdirectory
2. Follow existing format and style
3. Include practical examples
4. Add entry to this README
5. Cross-reference related docs
6. Update TODO.md if applicable
7. For design documents, include performance analysis
8. For hardware changes, create migration guides

## Version History

- **November 14, 2025** - Separate RD/WR ISR implementation
  - Added separate_isr_implementation_plan.md
  - Updated pin_mapping.md for new assignments (MREQ D47, RD D3/INT5, WR D2/INT4)
  - Updated hardware/README.md pin table
  - Documented separate ISR architecture for 5 MHz support

- **November 6, 2025** - Major HAL documentation update
  - Added HAL architecture proposal (HAL_Proposal.md)
  - Added ISR implementation specification (ISR_Proposal.md)
  - Added comprehensive performance analysis (4 documents)
  - Added hardware pin change guide (Hardware_Pin_Change.md)
  - Added complete changelog (Pin_Mapping_Update_Summary.md)
  - Documented MREQ pin relocation (D49 → D2 for INT4)
  - Updated README with new documentation structure

- **November 6, 2025** - Initial documentation structure
  - Clock module documentation
  - PROGMEM usage guidelines
  - Hardware pin mapping reference
  - Documentation README with navigation
