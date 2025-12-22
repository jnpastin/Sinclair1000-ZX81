# Phase 7: Build System & Configuration

**Status:** 📝 Not Started  
**Estimated Effort:** 2-3 days  
**Dependencies:** Phase 1

---

## Overview

This phase focuses on configuring the PlatformIO build system for Teensy 4.1, setting compiler flags, and establishing the development workflow.

## Documents in This Section

### [PlatformIO_Setup.md](PlatformIO_Setup.md)
Complete PlatformIO configuration for Teensy 4.1.

**Topics Covered:**
- platformio.ini configuration
- Teensy platform installation
- Board selection and settings
- Upload configuration
- Serial monitor setup
- Custom build scripts

---

### [Compiler_Flags.md](Compiler_Flags.md)
ARM GCC compiler flags and optimization settings.

**Topics Covered:**
- Optimization levels (-O0, -O2, -O3)
- ARM-specific flags (-mcpu, -mfpu, etc.)
- Link-time optimization (LTO)
- Debug vs Release builds
- Size vs speed tradeoffs
- Per-file optimization

---

### [Dependencies.md](Dependencies.md)
Required libraries and dependencies for the project.

**Topics Covered:**
- Core Teensy libraries
- Third-party libraries (if any)
- Library versions and compatibility
- Installation instructions
- Custom library paths
- Dependency management

---

## Phase 7 Deliverables

- [ ] Updated platformio.ini
- [ ] Build script documentation
- [ ] Compiler flags documented and tested
- [ ] Debug and release configurations
- [ ] Upload and monitor procedures
- [ ] Dependencies documented
- [ ] Build verification test

---

## PlatformIO Configuration

### Current platformio.ini (Basic)
```ini
[env:teensy41]
platform = teensy
board = teensy41
framework = arduino
```

### Enhanced Configuration (To Be Implemented)

```ini
; PlatformIO Project Configuration File

[platformio]
default_envs = teensy41_release

; Common settings for all environments
[env]
platform = teensy
board = teensy41
framework = arduino
monitor_speed = 115200
lib_deps = 
    ; Add any required libraries here

; Debug build - with debugging symbols, minimal optimization
[env:teensy41_debug]
build_type = debug
build_flags = 
    -DDEBUG
    -Og                    ; Debug optimization
    -g3                    ; Maximum debug info
    -mcpu=cortex-m7
    -mfloat-abi=hard
    -mfpu=fpv5-d16
    -mthumb
    -Wall                  ; All warnings
    -Wextra                ; Extra warnings
debug_tool = jlink         ; Or your debug probe
debug_init_break = tbreak setup

; Release build - maximum optimization
[env:teensy41_release]
build_type = release
build_flags = 
    -DNDEBUG
    -O3                    ; Maximum optimization
    -flto                  ; Link-time optimization
    -mcpu=cortex-m7
    -mfloat-abi=hard
    -mfpu=fpv5-d16
    -mthumb
    -funroll-loops
    -ffast-math
    -ffunction-sections    ; Remove unused functions
    -fdata-sections
    -Wall
build_unflags = 
    -Os                    ; Remove size optimization if present

; Performance profiling build
[env:teensy41_profile]
build_type = release
build_flags = 
    -DPROFILE
    -O2                    ; Balanced optimization
    -g                     ; Some debug info
    -mcpu=cortex-m7
    -mfloat-abi=hard
    -mfpu=fpv5-d16
    -mthumb
    -finstrument-functions ; Function profiling
```

---

## Compiler Flags Explained

### Core ARM Flags
```ini
-mcpu=cortex-m7         # Target Cortex-M7 processor
-mfloat-abi=hard        # Use hardware floating point
-mfpu=fpv5-d16          # FPU configuration
-mthumb                 # Use Thumb instruction set (16/32-bit)
```

### Optimization Flags
```ini
-O3                     # Maximum optimization (speed)
-Os                     # Optimize for size
-Og                     # Optimize for debugging
-flto                   # Link-time optimization
-funroll-loops          # Unroll loops for speed
-ffast-math             # Fast floating point (less precise)
```

### Code Size Reduction
```ini
-ffunction-sections     # Place functions in separate sections
-fdata-sections         # Place data in separate sections
-Wl,--gc-sections       # Linker removes unused sections
```

### Debug Flags
```ini
-g                      # Include debug info
-g3                     # Maximum debug info
-DDEBUG                 # Define DEBUG macro
```

---

## Build Commands

### Basic Commands
```bash
# Build for release
pio run -e teensy41_release

# Build for debug
pio run -e teensy41_debug

# Upload firmware
pio run -e teensy41_release -t upload

# Clean build
pio run -t clean

# Monitor serial output
pio device monitor
```

### Advanced Commands
```bash
# Build with verbose output
pio run -e teensy41_release -v

# Check code size
pio run -e teensy41_release -t size

# Run static analysis
pio check

# Generate compile_commands.json for IDE
pio run -t compiledb
```

---

## Build Targets

### Debug Build
**Purpose:** Development and troubleshooting
- Optimization: -Og (debug-friendly)
- Symbols: Full debug info
- Assertions: Enabled
- Logging: Verbose
- Size: Larger

### Release Build
**Purpose:** Production use
- Optimization: -O3 (maximum speed)
- Symbols: Minimal or none
- Assertions: Disabled
- Logging: Minimal
- Size: Smaller, faster

### Profile Build
**Purpose:** Performance analysis
- Optimization: -O2 (balanced)
- Symbols: Partial
- Instrumentation: Enabled
- Logging: Timing data

---

## Upload Procedures

### USB Upload (Default)
1. Connect Teensy via USB
2. Run upload command
3. Teensy Loader uploads automatically

```bash
pio run -t upload
```

### Manual Upload
1. Build firmware
2. Press program button on Teensy
3. Use Teensy Loader application

### Troubleshooting Upload
- Verify USB connection
- Check Teensy Loader running
- Press program button manually
- Try different USB port
- Verify serial monitor closed

---

## Serial Monitor Setup

### Basic Monitor
```bash
# Default monitor
pio device monitor

# Specify baud rate
pio device monitor -b 115200

# With filters
pio device monitor --filter colorize
```

### platformio.ini Configuration
```ini
[env:teensy41_release]
monitor_speed = 115200
monitor_filters = 
    colorize
    time
    log2file
```

---

## Dependency Management

### Core Dependencies
- **Teensy Core:** Automatically installed by PlatformIO
- **ARM GCC Toolchain:** Automatically installed

### Optional Libraries
```ini
lib_deps = 
    ; Add if needed:
    ; SPI
    ; Wire
    ; SD
```

### Custom Libraries
Place in `lib/` directory:
```
lib/
  └── CustomLib/
      ├── CustomLib.h
      └── CustomLib.cpp
```

---

## Build Verification

### Compilation Check
```bash
# Verify code compiles
pio run -e teensy41_release

# Check for warnings
pio run -e teensy41_release -v 2>&1 | grep warning
```

### Size Check
```bash
# Check firmware size
pio run -e teensy41_release -t size

# Expected output:
# RAM:   [==        ]  15.2% (used 156160 bytes from 1048576 bytes)
# Flash: [=         ]   8.5% (used 694016 bytes from 8126464 bytes)
```

### Upload Verification
```bash
# Upload and monitor
pio run -t upload && pio device monitor
```

---

## Continuous Integration (Optional)

### GitHub Actions Example
```yaml
name: Build Firmware

on: [push, pull_request]

jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v2
      - name: Set up Python
        uses: actions/setup-python@v2
        with:
          python-version: '3.x'
      - name: Install PlatformIO
        run: pip install platformio
      - name: Build Firmware
        run: pio run -e teensy41_release
```

---

## Getting Started

1. Ensure PlatformIO installed (see Getting_Started.md)
2. Read [PlatformIO_Setup.md](PlatformIO_Setup.md) for detailed config
3. Review [Compiler_Flags.md](Compiler_Flags.md) for optimization
4. Update platformio.ini with recommended settings
5. Test build and upload

---

## Success Criteria

- ✅ PlatformIO configured correctly
- ✅ Debug and release builds work
- ✅ Upload to Teensy successful
- ✅ Serial monitor functioning
- ✅ Compiler flags optimized
- ✅ Build documentation complete
- ✅ Build time reasonable (<2 minutes)

---

**Previous Phase:** [06_Timing/](../06_Timing/)  
**Next Phase:** [08_Testing/](../08_Testing/) - Testing & Validation
