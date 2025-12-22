# Development Guidelines

## Contents

### [PROGMEM_USAGE.md](PROGMEM_USAGE.md)
Comprehensive guide to memory management and PROGMEM usage.

**Includes:**
- PROGMEM implementation by file
- Memory savings summary
- Best practices and patterns
- Future usage guidelines
- Verification methods
- Compliance checklist

## Development Standards

### Memory Conservation
The Arduino Mega 2560 has limited RAM (8 KB), so we must be careful with memory usage:

- **PROGMEM** - Store all constant data in flash memory
- **F() Macro** - Keep string literals in flash
- **Static Arrays** - Use PROGMEM for lookup tables
- **Dynamic Allocation** - Minimize or eliminate

### Code Organization
- Module-based architecture (clock, hal, memory, tests)
- Header files for public APIs
- Implementation in .cpp files
- Documentation alongside code

### Naming Conventions
- `snake_case` for functions
- `UPPER_CASE` for constants/defines
- `camelCase` for variables (Arduino style)
- `PascalCase` for structs/classes

### Documentation Requirements
- API documentation in header files
- Implementation notes in .cpp files
- Usage examples in docs/
- Troubleshooting guides where appropriate

## Code Quality

### Required Practices
- ✅ Use PROGMEM for all constant data
- ✅ Use F() macro for string literals
- ✅ Comment complex algorithms
- ✅ Include usage examples
- ✅ Document memory usage
- ✅ Test at multiple clock speeds

### Prohibited Practices
- ❌ Large arrays in RAM
- ❌ String literals without F()
- ❌ Unnecessary dynamic allocation
- ❌ Blocking delays in critical code
- ❌ digitalWrite in time-critical paths

## Testing Standards

### Clock Speeds
Test all functionality at:
- 100 KHz (minimum)
- 500 KHz (default)
- 1 MHz (normal)
- 2 MHz (fast)
- 3 MHz (maximum)

### Validation
- Verify with oscilloscope/logic analyzer
- Check timing with T-state counts
- Validate flag behavior
- Test edge cases
- Document anomalies

## Future Guidelines

### When Adding New Modules
1. Create module directory structure
2. Define public API in header file
3. Implement in .cpp file
4. Use PROGMEM for constants
5. Write documentation
6. Add to main docs README
7. Update TODO.md

### Memory Budget
- Keep global RAM usage < 2 KB
- Reserve space for stack (~1 KB)
- Plan for future memory emulation
- Document memory usage per module

## Related Documentation
- [Clock Module](../clock/README.md)
- [Hardware Pin Mapping](../hardware/pin_mapping.md)
- [Project TODO](../../TODO.md)
- [Physical Hardware](../../hardware/)
