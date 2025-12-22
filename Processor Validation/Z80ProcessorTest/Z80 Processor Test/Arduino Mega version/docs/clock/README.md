# Clock Module Documentation

## Contents

### [CLOCK_README.md](CLOCK_README.md)
Complete API reference and usage guide for the clock generation module.

**Includes:**
- Feature overview
- Quick start guide
- Complete API reference with examples
- Predefined frequency constants
- Technical implementation details
- Hardware connection information

### [CLOCK_TROUBLESHOOTING.md](CLOCK_TROUBLESHOOTING.md)
Diagnostic and troubleshooting guide for clock issues.

**Includes:**
- Common issues and solutions
- Step-by-step diagnostic procedures
- Hardware verification methods
- Frequency measurement guide
- Serial command reference
- Expected signal characteristics

## Quick Reference

### Clock Frequencies
- **100 KHz** - Slow mode (debugging)
- **500 KHz** - Default (auto-start)
- **1 MHz** - Normal testing
- **2 MHz** - Fast testing
- **3 MHz** - Maximum speed

### Serial Commands
- `1` - Set to 100 KHz
- `2` - Set to 500 KHz (default)
- `3` - Set to 1 MHz
- `4` - Set to 2 MHz
- `5` - Set to 3 MHz
- `s` - Stop clock
- `r` - Resume clock
- `p` - Single step pulse
- `t` - Test mode (manual toggle)
- `?` - Show status

### Hardware
- **Pin:** Arduino Mega pin 11 (OC1A/PB5)
- **Timer:** Timer1 in Fast PWM Mode 14
- **Duty Cycle:** 50%
- **Output:** Connects to Z80 pin 6 (CLK)

## Related Documentation
- [Hardware Pin Mapping](../hardware/pin_mapping.md)
- [PROGMEM Usage Guidelines](../development/PROGMEM_USAGE.md)
- [Project TODO](../../TODO.md)
