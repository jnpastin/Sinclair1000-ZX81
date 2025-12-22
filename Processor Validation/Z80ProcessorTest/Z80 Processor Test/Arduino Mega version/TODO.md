# TODO

Project instructions moved to `INSTRUCTIONS.md`.

Remaining items:

- [ ] Include build & upload commands in developer scripts (if desired)
- [ ] Add testing and debugging guidance to docs/ if you want separate guides
- [ ] Link documentation in repo root (`README.md`) pointing to `INSTRUCTIONS.md`


# Z80 Processor Validation Project - TODO

## Project Overview
This project creates a comprehensive validation suite for the Z80 processor using an Arduino Mega 2560. The Arduino will interface with the Z80, providing test vectors, monitoring responses, and verifying correct operation of all processor functionality.

## Hardware Requirements
- [x] Arduino Mega 2560 (53 digital I/O pins, 16 analog inputs, provides 5V power)
- [x] Z80 CPU (40-pin DIP package)
- [x] Clock generation using Arduino hardware timer/counter
- [x] Arduino will emulate memory (no external RAM chip needed)
- [x] Arduino provides +5V and GND to Z80 CPU
- [x] Decoupling capacitors for Z80 power pins (0.1µF ceramic, close to chip)
- [x] Test rig: perfboard with 40-pin DIP socket
  - [x] Two sets of pin headers for each Z80 pin (80 headers total)
  - [x] One header set connects to Arduino Mega
  - [x] One header set for debugging instruments (logic analyzer, oscilloscope, multimeter)
- [x] Jumper wires for Arduino connections
- [x] Logic analyzer (optional, for debugging)
- [x] Oscilloscope (optional, for signal verification)

## Pin Mapping & Hardware Interface
- [x] Document Z80 pin assignments to Arduino Mega pins (see hardware/pin_mapping.md)
  - [x] 16-bit address bus (A0-A15) - Arduino pins: 8-12, 23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43
  - [x] 8-bit data bus (D0-D7) - Arduino pins: 3, 4, 5, 6, 24, 26, 28, 30 (bidirectional)
  - [x] Control signals from Z80: MREQ (38), IORQ (40), RD (42), WR (44), M1 (49), RFSH (47), HALT (36), BUSAK (7)
  - [x] Control signals to Z80: RESET (51), WAIT (48), INT (32), NMI (34), BUSRQ (50)
  - [x] Clock signal (CLK) - Arduino pin 11 using Timer1, output to Z80 pin 6
- [ ] Create pin mapping header file with Arduino pin definitions
- [x] Design schematic for Arduino-Z80 direct interface (5V compatible) - see hardware/Z80 Test Rig/
- [x] Arduino acts as memory controller - monitors address/data buses and responds to MREQ/RD/WR signals
- [x] Create wiring diagram - documented in KiCad schematic

## Software Architecture
- [x] Main Arduino sketch (Z80_Validation.ino)
- [ ] Hardware abstraction layer (HAL)
  - [ ] Bus control functions (read/write operations)
  - [x] Clock generation and control
  - [ ] Signal monitoring functions
  - [ ] Reset and initialization routines
- [ ] Test framework
  - [ ] Test case structure and management
  - [ ] Result logging and reporting
  - [ ] Error detection and debugging support
- [ ] Instruction test modules (organized by instruction type)
- [ ] Timing validation module
- [ ] Utility functions (display, serial communication, data conversion)

## Phase 1: Basic Infrastructure
### Hardware Setup
- [x] Build perfboard test rig
  - [x] Install 40-pin DIP socket for Z80
  - [x] Solder two pin headers adjacent to each socket pin (80 headers total)
  - [x] Label headers clearly (Arduino side vs Debug side)
  - [x] Add decoupling capacitor (0.1µF ceramic) between VCC and GND traces, close to socket
  - [x] Create clean power distribution traces for VCC and GND
- [x] Wire Arduino connections using jumper wires (reference: hardware/pin_mapping.md)
  - [x] Connect Arduino 5V pin to Z80 VCC header (pin 11)
  - [x] Connect Arduino GND to Z80 GND header (pin 29)
  - [x] Wire data bus (D0-D7) to Arduino pins 22-29 (PORTA: PA0-PA7)
  - [x] Wire address bus (A0-A15) to Arduino pins 37-30 (PORTC: PC0-PC7) + A8-A15 (PORTK: PK0-PK7)
  - [x] Wire control outputs from Z80 to Arduino:
    - [x] MREQ to pin D2 (PE4, INT4 external interrupt) ✅ **COMPLETE: Nov 7, 2025**
    - [x] IORQ, RD, WR, M1, RFSH, HALT, BUSAK to pins 48-42 (PORTL: PL1-PL7) ✅ **COMPLETE: Nov 7, 2025**
  - [x] Wire control inputs to Z80 from Arduino pins 53, 52, 51, 50, 10, 11 (PORTB: RESET, WAIT, INT, NMI, BUSRQ, CLK)
  - [x] Wire CLK (pin 6) to Arduino pin 11 (Timer1 output)
- [x] Verify hardware
  - [x] Check all solder joints for shorts and cold joints
  - [x] Verify power rails with multimeter (no shorts between VCC and GND)
  - [x] Test continuity from each header to corresponding Arduino pin
  - [x] Insert Z80 into socket (check orientation - pin 1 indicator)
  - [x] Power on and check voltage at Z80 VCC pin (should be stable 5V)
- [x] Set up debugging access
  - [x] Label debug headers for easy probe connection
  - [x] Test logic analyzer connection to debug headers
  - [x] Verify signals are accessible without interfering with Arduino connections

### Software Foundation
- [x] Create project directory structure
- [x] Create main .ino sketch file
- [x] Implement pin definitions header (pins.h) based on hardware/pin_mapping.md
  - [x] Define data bus pins on PORTA: D0-D7 = pins 22-29 (PA0-PA7)
  - [x] Define address bus pins on PORTC+PORTK: A0-A7 = pins 37-30 (PC0-PC7), A8-A15 = pins A8-A15 (PK0-PK7)
  - [x] Define control input pins:
    - [x] MREQ on D2 (PE4, INT4 external interrupt)
    - [x] IORQ=49, RD=48, WR=47, M1=46, RFSH=45, HALT=44, BUSAK=43 on PORTL
  - [x] Define control output pins on PORTB: RESET=53, WAIT=52, INT=51, NMI=50, BUSRQ=10, CLK=11
  - [x] Define port bit masks and direct port manipulation macros
  - [x] All static data stored in PROGMEM for RAM conservation
- [x] Implement clock generation using Timer1 (pin 11, PB5/OC1A)
  - [x] `void initializeClock(uint32_t frequency)` - Set up Timer1 for hardware PWM clock generation
  - [x] `void startClock()` / `void stopClock()` - Control clock output
  - [x] `void singleStepClock()` - Generate single clock pulse for debugging
  - [x] Configure for stable, jitter-free output at 100 KHz - 3 MHz
  - [x] Default to 500 KHz and auto-start on initialization
  - [x] Runtime frequency adjustment (setClockFrequency)
  - [x] Status query functions (getClockFrequency, isClockRunning)
  - [x] Test mode for hardware verification
- [ ] Implement basic HAL functions using direct port I/O
  - [ ] `void initializeBus()` - Configure all pins (address bus as inputs, data bus bidirectional)
  - [ ] `uint16_t readAddressBus()` - Read current address from Z80 using PINC|PINK (2 instructions)
  - [ ] `uint8_t readDataBus()` - Read from data bus using PINA (1 instruction)
  - [ ] `void writeDataBus(uint8_t data)` - Write to data bus using PORTA (1 instruction)
  - [ ] `void setDataBusInput()` / `void setDataBusOutput()` - Bus direction control using DDRA
  - [ ] `void resetZ80()` - Assert and release RESET signal using PORTB
  - [ ] `bool isMemoryRequest()` - Check MREQ signal using PINL
  - [ ] `bool isReadCycle()` - Check RD signal using PINL
  - [ ] `bool isWriteCycle()` - Check WR signal using PINL
  - [ ] All functions implemented as inline for maximum speed
- [ ] Implement interrupt-driven memory emulation functions
  - [ ] Create 64KB memory array in Arduino RAM/SRAM
  - [ ] `void handleMemoryRequest()` - ISR for Z80 bus cycles (triggered by INT4 external interrupt on MREQ)
  - [ ] `void writeToEmulatedMemory(uint16_t addr, uint8_t data)` - Store data in Arduino's memory array
  - [ ] `uint8_t readFromEmulatedMemory(uint16_t addr)` - Retrieve data from Arduino's memory array
  - [ ] `void loadProgram(uint16_t startAddr, uint8_t* program, uint16_t length)` - Preload test program
  - [ ] Configure external interrupt INT4 for sub-microsecond response time
- [ ] Create serial communication interface for status reporting
  - [ ] Minimize serial output during test execution (buffer results)
  - [ ] Use binary protocol for result logging where appropriate
  - [ ] Implement `void flushResultBuffer()` - Output buffered results after test completion
- [ ] Implement test result logging system
  - [ ] Buffer test results in memory during execution
  - [ ] Batch output to serial after test completion to avoid slowing tests

### User Interface - Serial Menu System
- [x] Implement interactive serial menu for test control
  - [x] `void displayMainMenu()` - Show main menu options
  - [x] `void processMenuInput()` - Handle user input and command dispatch
  - [x] `char getMenuChoice()` - Get validated menu selection from user
  - [x] `uint16_t getNumber()` - Get multi-digit numeric input
  - [x] `bool confirmAction()` - Confirmation prompts
  - [x] CR/LF handling for all terminal types
- [x] Main Menu Structure (7 options + exit)
  - [x] Option 1: Test individual instruction (framework ready)
  - [x] Option 2: Test instruction group (framework ready)
  - [x] Option 3: Test all instructions (placeholder)
  - [x] Option 4: Configure test mode (FULLY IMPLEMENTED)
  - [x] Option 5: View test results (framework ready)
  - [x] Option 6: System diagnostics (Clock Control FULLY IMPLEMENTED)
  - [x] Option 7: Help/documentation (FULLY IMPLEMENTED)
  - [x] Option 0: Exit
  - [x] Hierarchical navigation with numbered selections
  - [x] Multi-digit number support for instruction selection
  - [x] Clear screen functionality (terminal-agnostic)
  - [x] Status display showing current mode and clock frequency
- [x] Test Mode Selection
  - [x] **Quick Mode**: Test with one random set of input values
    - [x] Toggle in configuration menu
    - [x] Display: "Quick Test - Random sampling"
  - [x] **Thorough Mode**: Test all possible input combinations
    - [x] Toggle in configuration menu
    - [x] Display: "Thorough Test - Complete coverage"
    - [x] Progress indicator for long-running tests (displayProgress() ready)
- [x] Individual Instruction Testing (Framework Ready)
  - [x] Select instruction group (1-12)
  - [x] Display instructions in selected group (awaits instruction database)
  - [x] Allow selection by number
  - [ ] Show instruction details (opcode, description, flags affected)
  - [ ] Execute test in selected mode (Quick/Thorough)
  - [ ] Display results immediately after test
- [x] Instruction Group Testing (Framework Ready)
  - [x] Group 1: 8-bit Load Instructions
  - [x] Group 2: 16-bit Load Instructions
  - [x] Group 3: Exchange Instructions
  - [x] Group 4: 8-bit Arithmetic and Logical
  - [x] Group 5: 16-bit Arithmetic
  - [x] Group 6: Rotate and Shift Instructions
  - [x] Group 7: Bit Manipulation
  - [x] Group 8: Jump Instructions
  - [x] Group 9: Call and Return Instructions
  - [x] Group 10: Input/Output Instructions
  - [x] Group 11: Block Transfer and Search
  - [x] Group 12: CPU Control Instructions
  - [x] Display group menu with all 12 groups
  - [x] Confirmation before running group tests
  - [ ] Execute all tests in group
  - [ ] Display group summary after completion
- [ ] Complete Instruction Set Testing
  - [ ] Confirm before starting (due to time required)
  - [ ] Display estimated time based on mode
  - [ ] Run all instruction groups sequentially
  - [ ] Show overall progress (X of Y instructions tested)
  - [ ] Generate comprehensive report at end
  - [ ] Option to pause/resume during execution
  - [x] Progress indicator implementation ready (`displayProgress()`)
    - [x] Update progress every 1 second (throttled)
    - [x] Display: percentage, count, current item
    - [x] Check for abort key ('X')
- [x] Result Display Functions (Framework Ready)
  - [x] Result display menu with 6 options
  - [ ] `void displayTestResult(testResult)` - Show pass/fail for single test
  - [ ] `void displayGroupSummary(groupResults)` - Show group statistics
  - [ ] `void displayFullReport()` - Complete test suite results
  - [ ] Color coding (if terminal supports): Green=pass, Red=fail, Yellow=warning
  - [ ] Show: Instruction name, Test mode, Pass/Fail, Execution time, Flags state
- [x] Test Configuration (FULLY IMPLEMENTED)
  - [x] Set default test mode (Quick/Thorough) - toggle
  - [x] Configure clock speed for testing - with validation
  - [x] Enable/disable verbose output - toggle
  - [x] Set random seed for reproducible Quick mode tests
  - [x] Reset to defaults option
  - [x] ~~Save/load test configuration to EEPROM~~ (removed - config resets on power cycle)
- [x] Clock Control (FULLY IMPLEMENTED in System Diagnostics)
  - [x] View detailed clock status (frequency, state, hardware info)
  - [x] Start/Stop clock
  - [x] Single-step mode (with safety check)
  - [x] Change frequency (100-3000 kHz range)
  - [x] Test clock output (1 Hz manual toggle)
  - [x] Frequency presets: 100 kHz, 500 kHz, 1 MHz
  - [x] Human-readable frequency display (kHz/MHz)
  - [x] Integration with hardware clock module
  - [x] Config synchronization with actual clock state
- [ ] Error Reporting (when test execution implemented)
  - [ ] Display detailed error information for failed tests
  - [ ] Show expected vs actual results
  - [ ] Display register states at failure
  - [ ] Display memory contents if relevant
  - [ ] Suggest potential causes of failure
- [x] Help System (FULLY IMPLEMENTED)
  - [x] Quick Start Guide
  - [x] Menu Navigation Help
  - [x] Test Mode Explanations (Quick vs Thorough)
  - [x] Instruction Groups Reference (all 12 groups)
  - [x] Troubleshooting Guide
  - [x] Z80 Quick Reference
  - [x] Help menu with 6 topics

### Initial Testing
- [ ] Test clock generation and verify frequency with oscilloscope/logic analyzer
- [ ] Test Z80 reset sequence and verify proper initialization
- [ ] Verify address bus reading - load NOP instructions and monitor address increments
- [ ] Verify data bus bidirectional operations
- [ ] Test memory emulation - simple read cycles (feed NOPs to Z80)
- [ ] Test memory emulation - write cycles (let Z80 write, verify Arduino captures data)
- [ ] Test single-step clock execution for debugging
- [ ] Verify control signal monitoring (MREQ, RD, WR, M1)
- [ ] Test simple program execution (e.g., load accumulator, increment, loop)

## Performance Optimizations

### Speed Optimization Features (for maximum Z80 clock frequency support)
- [x] **Port-Aligned Pin Mapping** - Complete port alignment for single-instruction bus access
  - [x] Data bus on PORTA (pins 22-29) - 256x faster than digitalWrite
  - [x] Address bus on PORTC+PORTK - 256x faster address reads
  - [x] Control signals grouped on dedicated ports
  - [x] Achieves <1 µs bus cycle response time vs >50 µs with digitalWrite

- [x] **Hardware Timer Clock Generation** - Stable, jitter-free clock with zero CPU overhead
  - [x] Use Timer1 for PWM output on pin 11 (PB5/OC1A)
  - [x] Support adjustable frequencies: 100 kHz - 3 MHz
  - [x] 50% duty cycle for proper Z80 timing
  - [x] No software overhead during operation

- [ ] **Interrupt-Driven Bus Handling** - Sub-microsecond response to Z80 requests
  - [x] External interrupt INT4 on MREQ (pin D2/PE4)
  - [ ] Immediate response to MREQ falling edge transitions
  - [ ] ISR uses direct port I/O for minimal latency
  - [ ] Allows Z80 operation at 2-2.5 MHz

- [ ] **Direct Port I/O HAL** - All bus operations use single-instruction port access
  - [ ] Inline functions for zero function call overhead
  - [ ] Direct register access (PORTA, PINC, PINK, PINL, PORTB)
  - [ ] No digitalWrite/digitalRead calls in critical paths
  - [ ] Assembly-optimized critical sections if needed

- [x] **PROGMEM Lookup Tables** - Precomputed data for O(1) access
  - [x] Prescaler lookup table in clock module
  - [x] Pin mapping arrays in PROGMEM
  - [x] All string literals use F() macro
  - [ ] Flag calculation tables (future - all combinations precomputed)
  - [ ] Opcode decode tables for fast instruction identification (future)
  - [ ] Expected result tables for common test cases (future)
  - [ ] Instruction timing tables (future)
  - [x] All constant data stored in flash to preserve RAM for 64KB emulation

- [ ] **Optimized Serial Output** - Minimize test execution slowdown
  - [ ] Buffer test results during execution (no real-time printing)
  - [ ] Batch output after test completion
  - [ ] Binary protocol option for faster data transfer
  - [ ] Conditional compilation to disable debug output in production
  - [ ] Estimate: 100-1000x faster test execution vs real-time serial output
  - [ ] **Exception:** Provide progress updates every 1-2 seconds during long tests
    - [ ] Use millis() timer to throttle update frequency
    - [ ] Display progress percentage, instruction count, estimated time remaining
    - [ ] Keep updates lightweight (single line, no verbose output)
    - [ ] Maintains user feedback without significantly impacting performance
  - [x] All Serial.print strings use F() macro for flash storage

- [ ] **Compiler Optimization Flags** - Enable aggressive optimization
  - [ ] Add to platformio.ini or boards.txt:
    - [ ] `-O3` - Maximum optimization level
    - [ ] `-mcall-prologues` - Reduce code size for function calls
    - [ ] `-ffunction-sections -fdata-sections` - Enable dead code elimination
    - [ ] `-flto` - Link-time optimization
  - [ ] Expected 20-40% speed improvement in compiled code

- [ ] **Memory Access Optimization** - Fast program loading and execution
  - [ ] Pre-load test programs before starting clock
  - [ ] Use memcpy_P() for efficient PROGMEM to RAM transfers
  - [ ] Word-aligned access where possible
  - [ ] Minimize dynamic memory allocation during test execution

- [ ] **Progressive Clock Speed Testing** - Validate timing margins
  - [ ] Start at 100 kHz for initial validation
  - [ ] Increase to 500 kHz, 1 MHz, 2 MHz, 3 MHz
  - [ ] Document maximum stable frequency achieved
  - [ ] Target: 3 MHz stable operation

- [ ] **Conditional Compilation for Debug Code** - Remove overhead in production
  - [ ] `#define DEBUG_MODE 0/1` - Control debug output
  - [ ] `#if DEBUG_MODE` guards around verbose logging
  - [ ] Separate debug and production builds
  - [ ] Eliminates Serial.print overhead in timing-critical code

### Performance Targets
- [ ] Bus cycle response time: <1 µs (achieved with port I/O)
- [ ] Z80 clock frequency support: 1 MHz minimum, 3 MHz target
- [ ] Test throughput: >1000 instructions/second validation rate
- [ ] Memory emulation latency: <250 ns (sub-clock-cycle response)

## Instruction Definition System

### Data Structures and Definitions
- [ ] Create instruction definition data structure (stored in PROGMEM)
  - [ ] `struct Z80Instruction` - Complete instruction metadata
    - [ ] `char* mnemonic` - Instruction name (e.g., "LD A, B") - PROGMEM string
    - [ ] `uint8_t opcode` - Primary opcode byte
    - [ ] `uint8_t prefix` - Prefix byte if applicable (0x00, 0xCB, 0xDD, 0xED, 0xFD)
    - [ ] `uint8_t additionalBytes` - Number of additional opcode bytes
    - [ ] `InstructionGroup group` - Category/group membership
    - [ ] `uint8_t baseTimingCycles` - T-states for basic execution
    - [ ] `uint8_t conditionalTimingCycles` - T-states if condition met (for conditional ops)
    - [ ] `OperandType operand1` - First operand type
    - [ ] `OperandType operand2` - Second operand type (if applicable)
    - [ ] `FlagsAffected flagsAffected` - Which flags are modified
    - [ ] `char* description` - Human-readable description
    - [ ] `testFunction testFunc` - Pointer to test function
- [ ] Create operand type enumeration
  - [ ] `enum OperandType`
    - [ ] `OP_NONE` - No operand
    - [ ] `OP_REG_A` - Register A (Accumulator)
    - [ ] `OP_REG_B` - Register B
    - [ ] `OP_REG_C` - Register C
    - [ ] `OP_REG_D` - Register D
    - [ ] `OP_REG_E` - Register E
    - [ ] `OP_REG_H` - Register H
    - [ ] `OP_REG_L` - Register L
    - [ ] `OP_REG_I` - Interrupt register I
    - [ ] `OP_REG_R` - Refresh register R
    - [ ] `OP_REG_BC` - Register pair BC
    - [ ] `OP_REG_DE` - Register pair DE
    - [ ] `OP_REG_HL` - Register pair HL
    - [ ] `OP_REG_SP` - Stack pointer
    - [ ] `OP_REG_AF` - Register pair AF
    - [ ] `OP_REG_IX` - Index register IX
    - [ ] `OP_REG_IY` - Index register IY
    - [ ] `OP_IMM8` - 8-bit immediate value (n)
    - [ ] `OP_IMM16` - 16-bit immediate value (nn)
    - [ ] `OP_ADDR_IMM` - Direct address (nn)
    - [ ] `OP_ADDR_BC` - Indirect address (BC)
    - [ ] `OP_ADDR_DE` - Indirect address (DE)
    - [ ] `OP_ADDR_HL` - Indirect address (HL)
    - [ ] `OP_ADDR_SP` - Indirect address (SP)
    - [ ] `OP_ADDR_IX_D` - Indexed address (IX+d)
    - [ ] `OP_ADDR_IY_D` - Indexed address (IY+d)
    - [ ] `OP_DISPLACEMENT` - 8-bit signed displacement (d)
    - [ ] `OP_RELATIVE` - Relative jump offset (e)
    - [ ] `OP_BIT_NUM` - Bit number (0-7)
    - [ ] `OP_RST_ADDR` - Restart address (00h-38h)
    - [ ] `OP_CONDITION` - Condition code (NZ, Z, NC, C, PO, PE, P, M)
    - [ ] `OP_PORT_N` - 8-bit port address
    - [ ] `OP_PORT_C` - Port address in C register
- [ ] Create instruction group enumeration
  - [ ] `enum InstructionGroup`
    - [ ] `GROUP_LOAD_8BIT` - 8-bit load operations
    - [ ] `GROUP_LOAD_16BIT` - 16-bit load operations
    - [ ] `GROUP_EXCHANGE` - Exchange instructions
    - [ ] `GROUP_BLOCK_TRANSFER` - Block transfer/search
    - [ ] `GROUP_ARITHMETIC_8BIT` - 8-bit arithmetic
    - [ ] `GROUP_ARITHMETIC_16BIT` - 16-bit arithmetic
    - [ ] `GROUP_LOGICAL` - Logical operations (AND, OR, XOR)
    - [ ] `GROUP_ROTATE_SHIFT` - Rotate and shift operations
    - [ ] `GROUP_BIT_OPS` - Bit set/reset/test
    - [ ] `GROUP_JUMP` - Jump instructions
    - [ ] `GROUP_CALL_RETURN` - Call and return
    - [ ] `GROUP_IO` - Input/output operations
    - [ ] `GROUP_CPU_CONTROL` - CPU control (NOP, HALT, DI, EI, etc.)
    - [ ] `GROUP_INTERRUPT` - Interrupt mode and control
- [ ] Create flags enumeration
  - [ ] `enum FlagsAffected`
    - [ ] `FLAG_NONE` - No flags affected
    - [ ] `FLAG_S` - Sign flag (bit 7)
    - [ ] `FLAG_Z` - Zero flag (bit 6)
    - [ ] `FLAG_H` - Half carry flag (bit 4)
    - [ ] `FLAG_PV` - Parity/Overflow flag (bit 2)
    - [ ] `FLAG_N` - Add/Subtract flag (bit 1)
    - [ ] `FLAG_C` - Carry flag (bit 0)
    - [ ] `FLAG_ALL` - All flags affected
    - [ ] `FLAG_STANDARD` - S, Z, H, PV, N, C (typical arithmetic)
    - [ ] `FLAG_LOGIC` - S, Z, H=0, PV, N=0, C=0 (logical ops)
    - [ ] Bit flags to track individual flag modifications

### Timing Cycle Definitions
- [ ] Document T-state counts for all instructions
  - [ ] Create timing reference table
  - [ ] Include M-cycle breakdown for each instruction
  - [ ] Document memory access cycles (M1, MR, MW)
  - [ ] Document I/O cycles timing
- [ ] Instruction timing categories
  - [ ] **Simple instructions** (4 T-states): NOP, EX AF, AF'
  - [ ] **Register operations** (4-8 T-states): LD r, r'; ADD A, r
  - [ ] **Immediate loads** (7-10 T-states): LD r, n
  - [ ] **Memory operations** (7-19 T-states): LD r, (HL); LD (nn), A
  - [ ] **16-bit operations** (10-20 T-states): LD dd, nn; ADD HL, ss
  - [ ] **Jump/Call** (10-17 T-states): JP, CALL, RET
  - [ ] **Block operations** (16-21 T-states per iteration): LDIR, CPIR
  - [ ] **Indexed operations** (+4-8 T-states vs normal): (IX+d), (IY+d)
  - [ ] **Extended prefix operations** (+4 T-states): ED, CB, DD, FD prefixes
- [ ] Conditional timing variations
  - [ ] Jumps: taken vs not taken (different T-states)
  - [ ] Calls/Returns: conditional execution timing
  - [ ] Block operations: repeat vs non-repeat termination
  - [ ] DJNZ: branch taken vs fall through
- [ ] Create timing validation functions
  - [ ] `uint8_t getExpectedTiming(instruction, condition)` - Calculate expected T-states
  - [ ] `uint8_t measureActualTiming(instruction)` - Measure real execution time
  - [ ] `bool validateTiming(instruction)` - Compare expected vs actual

### Instruction Database Implementation
- [ ] Create master instruction table
  - [ ] Define array of all Z80 instructions (698 total including undocumented)
  - [ ] Group instructions by category for easy lookup
  - [ ] Index by opcode for fast access
  - [ ] Include all variants (register combinations, addressing modes)
- [ ] Implement instruction lookup functions
  - [ ] `Z80Instruction* findInstructionByOpcode(uint8_t opcode, uint8_t prefix)`
  - [ ] `Z80Instruction* findInstructionByMnemonic(char* mnemonic)`
  - [ ] `Z80Instruction** getInstructionsByGroup(InstructionGroup group, uint16_t* count)`
  - [ ] `void listAllInstructions()` - Display all available instructions
  - [ ] `void listInstructionGroup(InstructionGroup group)` - Display group members
- [ ] Implement instruction metadata functions
  - [ ] `char* getInstructionDescription(Z80Instruction* inst)`
  - [ ] `uint8_t getInstructionSize(Z80Instruction* inst)` - Total bytes including operands
  - [ ] `char* getFlagsString(Z80Instruction* inst)` - Human-readable flags list
  - [ ] `char* getTimingString(Z80Instruction* inst)` - Formatted timing info
- [ ] Create test data generators
  - [ ] `void generateRandomOperands(Z80Instruction* inst, uint8_t* operands)`
  - [ ] `void generateAllOperandCombinations(Z80Instruction* inst, callback)`
  - [ ] `bool isValidOperandCombination(Z80Instruction* inst, uint8_t* operands)`

### Group-Level Organization
- [ ] Define group metadata structure
  - [ ] `struct InstructionGroupInfo`
    - [ ] `InstructionGroup id` - Group identifier
    - [ ] `char* name` - Group name (e.g., "8-bit Load Instructions")
    - [ ] `char* description` - Detailed group description
    - [ ] `uint16_t instructionCount` - Number of instructions in group
    - [ ] `Z80Instruction** instructions` - Array of instruction pointers
    - [ ] `uint32_t estimatedQuickTestTime` - Estimated time for quick test (ms)
    - [ ] `uint32_t estimatedThoroughTestTime` - Estimated time for thorough test (ms)
- [ ] Implement group management functions
  - [ ] `void initializeInstructionGroups()` - Build group tables
  - [ ] `InstructionGroupInfo* getGroupInfo(InstructionGroup group)`
  - [ ] `void displayGroupInfo(InstructionGroup group)` - Show group details
  - [ ] `uint16_t getGroupSize(InstructionGroup group)` - Count instructions in group

### Documentation Generation
- [ ] Auto-generate instruction reference from database
  - [ ] Create formatted instruction list with all metadata
  - [ ] Generate opcode table
  - [ ] Create timing reference chart
  - [ ] Generate flag effects matrix
- [ ] Create test coverage report
  - [ ] List all defined instructions
  - [ ] Mark which have test functions implemented
  - [ ] Calculate coverage percentage
  - [ ] Identify gaps in test coverage

## Phase 2: Instruction Set Validation

### 8-bit Load Instructions
- [ ] LD r, r' (register to register) - 49 combinations
- [ ] LD r, n (immediate to register) - 7 instructions
- [ ] LD r, (HL) (memory indirect to register) - 7 instructions
- [ ] LD (HL), r (register to memory indirect) - 7 instructions
- [ ] LD (HL), n (immediate to memory indirect)
- [ ] LD A, (BC) / LD A, (DE) (indirect addressing)
- [ ] LD (BC), A / LD (DE), A (indirect addressing)
- [ ] LD A, (nn) / LD (nn), A (direct addressing)

### 16-bit Load Instructions
- [ ] LD dd, nn (16-bit immediate load) - BC, DE, HL, SP
- [ ] LD IX, nn / LD IY, nn (index register immediate load)
- [ ] LD HL, (nn) (direct memory to HL)
- [ ] LD (nn), HL (HL to direct memory)
- [ ] LD dd, (nn) (extended instructions for BC, DE, HL, SP)
- [ ] LD (nn), dd (extended instructions)
- [ ] LD SP, HL / LD SP, IX / LD SP, IY
- [ ] PUSH qq / POP qq (BC, DE, HL, AF)
- [ ] PUSH IX / POP IX
- [ ] PUSH IY / POP IY

### Exchange Instructions
- [ ] EX DE, HL
- [ ] EX AF, AF'
- [ ] EXX (exchange all register pairs)
- [ ] EX (SP), HL / EX (SP), IX / EX (SP), IY

### 8-bit Arithmetic and Logical Instructions
- [ ] ADD A, r (all 8 registers including (HL))
- [ ] ADD A, n (immediate)
- [ ] ADC A, r (add with carry)
- [ ] ADC A, n
- [ ] SUB r (subtract)
- [ ] SUB n
- [ ] SBC A, r (subtract with carry)
- [ ] SBC A, n
- [ ] AND r / AND n (logical AND)
- [ ] OR r / OR n (logical OR)
- [ ] XOR r / XOR n (logical XOR)
- [ ] CP r / CP n (compare)
- [ ] INC r / DEC r (increment/decrement register)
- [ ] INC (HL) / DEC (HL) (memory operations)

### 16-bit Arithmetic Instructions
- [ ] ADD HL, ss (HL = HL + BC/DE/HL/SP)
- [ ] ADC HL, ss (with carry)
- [ ] SBC HL, ss (subtract with carry)
- [ ] ADD IX, pp / ADD IY, rr (index register arithmetic)
- [ ] INC ss / DEC ss (16-bit increment/decrement)
- [ ] INC IX / DEC IX / INC IY / DEC IY

### Rotate and Shift Instructions
- [ ] RLCA, RLA, RRCA, RRA (accumulator rotates)
- [ ] RLC r, RL r, RRC r, RR r (register rotates)
- [ ] SLA r, SRA r, SRL r (arithmetic and logical shifts)
- [ ] RLD, RRD (digit rotates)

### Bit Manipulation Instructions
- [ ] BIT b, r (test bit) - 64 combinations (8 bits × 8 registers)
- [ ] SET b, r (set bit) - 64 combinations
- [ ] RES b, r (reset bit) - 64 combinations
- [ ] Test all with (HL), (IX+d), (IY+d) addressing modes

### Jump Instructions
- [ ] JP nn (unconditional jump)
- [ ] JP cc, nn (conditional jumps: NZ, Z, NC, C, PO, PE, P, M)
- [ ] JR e (relative jump)
- [ ] JR cc, e (conditional relative jumps: NZ, Z, NC, C)
- [ ] JP (HL) / JP (IX) / JP (IY) (indirect jumps)
- [ ] DJNZ e (decrement and jump if not zero)

### Call and Return Instructions
- [ ] CALL nn (unconditional call)
- [ ] CALL cc, nn (conditional calls: NZ, Z, NC, C, PO, PE, P, M)
- [ ] RET (unconditional return)
- [ ] RET cc (conditional returns)
- [ ] RETI (return from interrupt)
- [ ] RETN (return from NMI)
- [ ] RST p (restart to vector: 00h, 08h, 10h, 18h, 20h, 28h, 30h, 38h)

### Input/Output Instructions
- [ ] IN A, (n) (direct I/O)
- [ ] IN r, (C) (indirect I/O)
- [ ] INI, INIR (block input, repeat)
- [ ] IND, INDR (block input decrement, repeat)
- [ ] OUT (n), A (direct output)
- [ ] OUT (C), r (indirect output)
- [ ] OUTI, OTIR (block output, repeat)
- [ ] OUTD, OTDR (block output decrement, repeat)

### Block Transfer and Search Instructions
- [ ] LDI, LDIR (load and increment, repeat)
- [ ] LDD, LDDR (load and decrement, repeat)
- [ ] CPI, CPIR (compare and increment, repeat)
- [ ] CPD, CPDR (compare and decrement, repeat)

### CPU Control Instructions
- [ ] NOP (no operation)
- [ ] HALT (halt until interrupt)
- [ ] DI (disable interrupts)
- [ ] EI (enable interrupts)
- [ ] IM 0, IM 1, IM 2 (interrupt modes)
- [ ] CCF (complement carry flag)
- [ ] SCF (set carry flag)
- [ ] NEG (negate accumulator)
- [ ] CPL (complement accumulator)
- [ ] DAA (decimal adjust accumulator)

### Index Register Instructions (IX/IY variants)
- [ ] All applicable instructions with IX+d and IY+d displacement addressing
- [ ] Test with various displacement values (positive, negative, zero)

### Undocumented Instructions (optional, for completeness)
- [ ] SLL r (undocumented shift left logical)
- [ ] Undocumented flag behaviors
- [ ] Undocumented IX/IY high/low byte access

## Phase 3: Flag Verification
- [ ] Create flag testing framework
- [ ] Test Sign flag (S) - bit 7
  - [ ] Verify set when result is negative (bit 7 = 1)
  - [ ] Test with arithmetic and logical operations
- [ ] Test Zero flag (Z) - bit 6
  - [ ] Verify set when result is zero
  - [ ] Test with all instruction types that affect Z
- [ ] Test Half Carry flag (H) - bit 4
  - [ ] Verify carry from bit 3 to bit 4
  - [ ] Test with ADD, ADC, SUB, SBC, CP
  - [ ] Critical for DAA instruction
- [ ] Test Parity/Overflow flag (P/V) - bit 2
  - [ ] Verify parity mode (even parity = 1)
  - [ ] Verify overflow mode (signed arithmetic overflow)
  - [ ] Test mode switching with different instructions
- [ ] Test Add/Subtract flag (N) - bit 1
  - [ ] Verify set after subtraction operations
  - [ ] Verify cleared after addition operations
- [ ] Test Carry flag (C) - bit 0
  - [ ] Verify carry out from bit 7
  - [ ] Test with arithmetic operations
  - [ ] Test with rotate and shift operations
- [ ] Test flag behavior with block instructions
- [ ] Test flag preservation (which instructions don't affect which flags)
- [ ] Create comprehensive flag state verification after each test

## Phase 4: Timing Validation
- [ ] Measure instruction timing (T-states)
- [ ] Create timing reference table for all instructions
- [ ] Verify M1 cycle (opcode fetch) timing
- [ ] Verify memory read/write cycle timing
- [ ] Verify I/O cycle timing
- [ ] Test WAIT state insertion and timing
- [ ] Test REFRESH cycle timing
- [ ] Verify timing differences between normal and indexed instructions
- [ ] Test timing with different clock frequencies
- [ ] Document any timing anomalies

## Phase 5: Interrupt Testing
- [ ] Test interrupt mode 0 (IM 0)
  - [ ] Verify instruction execution from data bus
- [ ] Test interrupt mode 1 (IM 1)
  - [ ] Verify jump to address 0x0038
- [ ] Test interrupt mode 2 (IM 2)
  - [ ] Verify vector table lookup
  - [ ] Test with various I register values
- [ ] Test DI/EI instructions
- [ ] Test interrupt acknowledge cycle
- [ ] Test interrupt response time
- [ ] Test nested interrupts
- [ ] Test NMI (non-maskable interrupt)
  - [ ] Verify jump to address 0x0066
  - [ ] Test NMI priority over INT
- [ ] Test RETI and RETN instructions
- [ ] Test HALT state and interrupt wakeup

## Phase 6: Special Features
- [ ] Test REFRESH register operation
  - [ ] Verify R register increments
  - [ ] Test REFRESH timing
- [ ] Test I register (interrupt vector)
- [ ] Test alternative register set (shadow registers)
  - [ ] Verify EXX operation
  - [ ] Verify EX AF, AF' operation
  - [ ] Test independence of main and shadow sets
- [ ] Test stack operations
  - [ ] Verify stack pointer operations
  - [ ] Test stack overflow/underflow scenarios
  - [ ] Test deep nesting with CALL/RET
- [ ] Test BUSRQ/BUSACK (bus request/acknowledge)
  - [ ] Verify bus release
  - [ ] Test DMA-like operations

## Phase 7: Edge Cases and Stress Testing
- [ ] Test boundary conditions
  - [ ] Address bus limits (0x0000, 0xFFFF)
  - [ ] Data bus limits (0x00, 0xFF)
  - [ ] Register overflow/underflow
- [ ] Test consecutive operations
  - [ ] Back-to-back memory accesses
  - [ ] Rapid flag changes
  - [ ] Quick interrupt sequences
- [ ] Test self-modifying code
- [ ] Test invalid opcodes behavior
- [ ] Test maximum stack depth
- [ ] Test with all RAM locations
- [ ] Long-running instruction sequences
- [ ] Random instruction sequences (fuzzing)

## Phase 8: Documentation and Reporting
- [ ] Create test report template
- [ ] Document all test results
  - [ ] Pass/fail status for each instruction
  - [ ] Flag verification results
  - [ ] Timing measurements
  - [ ] Any anomalies or issues found
- [ ] Create instruction coverage report
- [ ] Document hardware setup with photos
- [ ] Create schematic diagram
- [ ] Write user manual for the test suite
- [ ] Create quick reference guide
- [ ] Document known limitations
- [ ] Create troubleshooting guide

## Phase 9: Optimization and Enhancement
- [ ] Optimize test execution speed
- [ ] Add parallel testing where possible
- [ ] Implement test suite selection (run specific test groups)
- [ ] Add verbose/quiet modes
- [ ] Create graphical display of test progress (if LCD available)
- [ ] Add SD card logging for detailed results
- [ ] Implement automated regression testing
- [ ] Create comparison mode (compare multiple Z80 chips)
- [ ] Add temperature testing (test at different temperatures)
- [ ] Add voltage margin testing

## Phase 10: Advanced Features (Optional)
- [ ] Implement Z80 emulator on Arduino for comparison
- [ ] Create instruction trace mode
- [ ] Add single-step debugging capability
- [ ] Implement breakpoint functionality
- [ ] Create memory dump utility
- [ ] Add disassembler for debugging
- [ ] Create logic analyzer integration
- [ ] Implement CPLD/FPGA assistance for faster testing
- [ ] Create PC interface for remote control and monitoring
- [ ] Build custom PCB for the tester

## Code Organization
```
Z80_Validation/
├── Z80_Validation.ino          # Main sketch
├── TODO.md                      # This file
├── README.md                    # Project documentation
├── pins.h                       # Pin definitions from hardware/pin_mapping.md
├── hal.h / hal.cpp             # Hardware abstraction layer
├── z80_control.h / .cpp        # Z80 control functions
├── memory.h / .cpp             # Memory access functions
├── test_framework.h / .cpp     # Testing infrastructure
├── test_results.h / .cpp       # Result logging
├── hardware/                    # Hardware documentation
│   ├── README.md               # Hardware documentation index
│   ├── Z80 Test Rig/           # KiCad project folder
│   │   ├── Z80 Test Rig.kicad_sch   # KiCad schematic
│   │   ├── Z80 Test Rig.kicad_pro   # KiCad project file
│   │   └── Z80 Test Rig.kicad_pcb   # PCB layout (optional)
│   ├── pin_mapping.md          # Pin connection table (completed)
│   ├── pin_mapping.csv         # Machine-readable pin mapping (completed)
│   ├── perfboard_layout.md     # Perfboard construction guide
│   ├── BOM.md                  # Bill of materials
│   └── photos/                 # Test rig photos
├── instructions/
│   ├── test_load_8bit.cpp
│   ├── test_load_16bit.cpp
│   ├── test_arithmetic.cpp
│   ├── test_logical.cpp
│   ├── test_rotate_shift.cpp
│   ├── test_bit_ops.cpp
│   ├── test_jumps.cpp
│   ├── test_calls.cpp
│   ├── test_io.cpp
│   └── test_block_ops.cpp
├── timing/
│   └── timing_tests.cpp
├── interrupts/
│   └── interrupt_tests.cpp
├── utilities/
│   ├── display.cpp
│   ├── serial_comm.cpp
│   └── helpers.cpp
└── docs/
    └── test_report_template.md
```

## Testing Strategy
1. **Unit Testing**: Test each instruction individually
2. **Integration Testing**: Test instruction sequences
3. **Flag Verification**: Verify flag states after every operation
4. **Timing Validation**: Measure and verify timing
5. **Stress Testing**: Push the processor to limits
6. **Regression Testing**: Ensure fixes don't break existing tests

## Phase 10: Memory Management & Strategy

### Memory Emulation Functions
- [ ] Implement memory management functions
  - [ ] `void clearMemory()` - Zero all emulated memory
  - [ ] `void setMemoryPattern(uint8_t pattern)` - Fill with pattern for testing
  - [ ] `bool verifyMemoryPattern(uint8_t pattern)` - Verify memory contents
  - [ ] `void saveMemorySnapshot()` - Capture memory state for debugging
  - [ ] `void restoreMemorySnapshot()` - Restore previous memory state
  - [ ] `void dumpMemory(uint16_t start, uint16_t length)` - Display memory contents
- [ ] Stack management
  - [ ] Initialize stack pointer to safe location
  - [ ] Implement stack overflow detection
  - [ ] Implement stack underflow detection
  - [ ] Test deep nesting scenarios
- [ ] Memory access optimization
  - [ ] Fast memory read/write for Z80 bus timing
  - [ ] Handle memory request ISR efficiently
  - [ ] Minimize latency in bus response

## Phase 11: Error Handling & Recovery

### Comprehensive Error Detection
- [ ] Implement error detection mechanisms
  - [ ] Illegal instruction detection
  - [ ] Bus conflict detection (simultaneous read/write)
  - [ ] Timeout detection (Z80 stuck/halted unexpectedly)
  - [ ] Memory access violations
  - [ ] Stack overflow/underflow
  - [ ] Clock failure detection
- [ ] Error classification system
  - [ ] `enum ErrorType`: FATAL, ERROR, WARNING, INFO
  - [ ] `struct ErrorLog`: timestamp, type, instruction, state
  - [ ] Error history buffer (circular buffer of last N errors)
- [ ] Recovery mechanisms
  - [ ] Automatic Z80 reset on fatal errors
  - [ ] State preservation before reset
  - [ ] Test continuation after non-fatal errors
  - [ ] Watchdog timer implementation
- [ ] Diagnostic functions
  - [ ] `void dumpProcessorState()` - Show all register values
  - [ ] `void dumpBusState()` - Show all bus signal states
  - [ ] `void displayErrorLog()` - Show recent errors
  - [ ] `bool selfTest()` - Arduino hardware self-test

## Phase 12: Performance Optimization

### Speed Improvements
- [ ] Optimize critical paths
  - [ ] Fast digital I/O using direct port manipulation
  - [ ] Minimize ISR latency
  - [ ] Use lookup tables for common operations
  - [ ] Optimize memory emulation response time
- [ ] Implement variable clock speeds
  - [ ] Slow mode: 100 KHz - 500 KHz (for debugging)
  - [ ] Normal mode: 1 MHz - 2 MHz (for testing)
  - [ ] Fast mode: 3 MHz (for stress testing)
  - [ ] Dynamic clock adjustment during testing
- [ ] Memory optimization
  - [ ] Minimize RAM usage for 64KB emulation
  - [ ] Consider external SRAM if Arduino RAM insufficient
  - [ ] Use PROGMEM for constant data (instruction tables, strings)
  - [ ] Efficient data structures for test results

## Phase 13: Data Persistence & Logging

### Test Result Storage
- [ ] Implement EEPROM storage
  - [ ] Save test configuration
  - [ ] Save test statistics (pass/fail counts)
  - [ ] Save error history
  - [ ] Save last test session information
  - [ ] EEPROM wear leveling consideration
- [ ] SD card support (optional)
  - [ ] Detailed test logs to SD card
  - [ ] CSV export of test results
  - [ ] Memory dumps for failed tests
  - [ ] Session replay capability
- [ ] Serial data export
  - [ ] Export results in JSON format
  - [ ] Export results in CSV format
  - [ ] Export results in human-readable format
  - [ ] Real-time streaming of test progress

## Phase 14: Advanced Testing Features

### Instruction Sequence Testing
- [ ] Test common instruction patterns
  - [ ] Function call/return sequences
  - [ ] Loop constructs
  - [ ] Arithmetic computation chains
  - [ ] String manipulation sequences
  - [ ] Table lookup operations
- [ ] Test instruction interactions
  - [ ] Flag propagation between instructions
  - [ ] Register dependency chains
  - [ ] Memory coherency across operations
  - [ ] Stack operations in context
- [ ] Real-world program validation
  - [ ] Execute simple programs (bubble sort, string copy, etc.)
  - [ ] Verify program outputs
  - [ ] Compare with expected results
  - [ ] Benchmark execution time

### Random Testing (Fuzzing)
- [ ] Implement random instruction stream generator
  - [ ] Generate valid random instruction sequences
  - [ ] Ensure sequences won't crash (avoid infinite loops)
  - [ ] Monitor for unexpected behavior
  - [ ] Log anomalies for investigation
- [ ] Stress testing modes
  - [ ] Rapid clock speed changes
  - [ ] Random interrupt injection
  - [ ] Random WAIT state insertion
  - [ ] Memory thrashing patterns
- [ ] Endurance testing
  - [ ] Run tests continuously for extended periods
  - [ ] Monitor for gradual degradation
  - [ ] Temperature monitoring (if available)
  - [ ] Power stability monitoring

## Phase 15: Comparison & Validation

### Reference Implementation Comparison
- [ ] Implement or integrate Z80 emulator
  - [ ] Use emulator as golden reference
  - [ ] Run same tests on both real Z80 and emulator
  - [ ] Compare results instruction-by-instruction
  - [ ] Flag discrepancies for analysis
- [ ] ZEXALL integration
  - [ ] Port ZEXALL test suite to this platform
  - [ ] Run ZEXALL tests
  - [ ] Parse ZEXALL results
  - [ ] Generate comparison reports
- [ ] Cross-validation with other tools
  - [ ] Compare with logic analyzer captures
  - [ ] Validate against published Z80 documentation
  - [ ] Compare timing with datasheet specifications

## Phase 16: User Experience Enhancements

### Visual Feedback
- [ ] LED status indicators (using available pins)
  - [ ] Power LED
  - [ ] Test running LED (blinking)
  - [ ] Pass LED (green)
  - [ ] Fail LED (red)
  - [ ] Activity indicators for bus operations
- [ ] LCD display support (optional)
  - [ ] 16x2 or 20x4 character LCD
  - [ ] Display current test name
  - [ ] Display pass/fail counts
  - [ ] Display progress percentage
- [ ] Progress bar in serial terminal
  - [ ] Visual test progress
  - [ ] Estimated time remaining
  - [ ] Current instruction being tested

### User Interface Improvements
- [ ] Command history (up/down arrow support)
- [ ] Tab completion for commands
- [ ] Bookmark favorite tests for quick access
- [ ] Custom test suites (user-defined groups)
- [ ] Test scheduling (run specific tests at certain times)
- [ ] Remote control via serial commands (for automation)

## Phase 17: Documentation & Training

### User Documentation
- [ ] Write comprehensive user manual
  - [ ] Getting started guide
  - [ ] Hardware assembly instructions with photos
  - [ ] Software installation and setup
  - [ ] Menu navigation guide
  - [ ] Test interpretation guide
  - [ ] Troubleshooting section
- [ ] Create quick reference cards
  - [ ] Pin mapping quick reference
  - [ ] Menu command reference
  - [ ] Common error codes
  - [ ] Signal descriptions
- [ ] Video tutorials
  - [ ] Hardware assembly video
  - [ ] Software walkthrough
  - [ ] Test execution examples
  - [ ] Debugging demonstrations

### Developer Documentation
- [ ] Code documentation
  - [ ] Function-level comments
  - [ ] API documentation
  - [ ] Architecture diagrams
  - [ ] Data flow diagrams
- [ ] Contribute guidelines
  - [ ] How to add new instruction tests
  - [ ] How to add new features
  - [ ] Code style guidelines
  - [ ] Testing requirements
- [ ] Design decisions document
  - [ ] Why specific approaches were chosen
  - [ ] Trade-offs considered
  - [ ] Known limitations
  - [ ] Future improvement ideas

## Phase 18: Quality Assurance

### Code Quality
- [ ] Code review checklist
- [ ] Static analysis (if tools available for Arduino)
- [ ] Memory leak detection
- [ ] Timing analysis of critical sections
- [ ] Code coverage analysis
- [ ] Refactoring for maintainability

### Testing the Tester
- [ ] Validate test framework correctness
- [ ] Test with known-good Z80
- [ ] Test with known-defective Z80 (if available)
- [ ] Verify false positive rate is zero
- [ ] Verify false negative rate is zero
- [ ] Cross-platform validation

## Phase 19: Advanced Features (Future Enhancements)

### Multi-Processor Testing
- [ ] Support testing multiple Z80s sequentially
- [ ] Compare results between different Z80 chips
- [ ] Identify chip-to-chip variations
- [ ] Batch testing capability

### Remote Access
- [ ] Web interface (if ESP32/WiFi added)
- [ ] Remote monitoring
- [ ] Cloud result storage
- [ ] Email notifications on test completion

### Extended Processor Support
- [ ] Add support for Z80A, Z80B variants
- [ ] Add support for Z180/Z380 (future)
- [ ] Document differences between variants
- [ ] Variant-specific test modes

## Success Criteria
- [ ] All documented Z80 instructions validated
- [ ] All flag operations verified
- [ ] Timing measurements within specifications
- [ ] Interrupt handling fully tested
- [ ] Zero false positives in test results
- [ ] Complete documentation produced
- [ ] Test suite runs reliably and repeatably

## Resources and References
- [x] Z80 CPU User Manual (Zilog)
- [x] Z80 Assembly Language Programming (Lance Leventhal)
- [x] Z80 instruction set reference
- [x] Arduino Mega 2560 specifications
- [x] KiCad schematic created (hardware/Z80 Test Rig/)
- [x] Pin mapping documented (docs/hardware/pin_mapping.md and pin_mapping.csv)
- [x] Clock module documentation (docs/clock/)
- [x] PROGMEM usage guidelines (docs/development/PROGMEM_USAGE.md)
- [ ] Logic analyzer documentation (if used)
- [ ] Existing Z80 test suites for reference (e.g., ZEXALL)

## Notes
- Hardware schematic completed in KiCad (see hardware/Z80 Test Rig/)
- Pin mappings documented in docs/hardware/pin_mapping.md and pin_mapping.csv
- Clock generated on Arduino pin 11 using Timer1 (OC1A)
- Frequency range: 100 KHz - 3 MHz, default 500 KHz with auto-start
- Total pins used: 38 digital pins (16 pins still available for expansion)
- Documentation organized in docs/ directory with subdirectories
- All constant data stored in PROGMEM to conserve RAM
- May need to implement simplified versions initially before full validation
- Clock speed can be adjusted for easier monitoring (start slow, increase later)
- Consider implementing a PC-based monitoring tool for detailed analysis
- Budget adequate time for hardware debugging
- Keep detailed logs of all issues encountered and resolutions

## Estimated Timeline
- Phase 1 (Infrastructure): 2-3 weeks
- Phase 2 (Instructions): 6-8 weeks
- Phase 3 (Flags): 2 weeks
- Phase 4 (Timing): 2 weeks
- Phase 5 (Interrupts): 2 weeks
- Phase 6 (Special Features): 1-2 weeks
- Phase 7 (Edge Cases): 2 weeks
- Phase 8 (Documentation): 1-2 weeks
- Phase 9-10 (Optional): 2-4 weeks

**Total Estimated Time: 20-30 weeks for complete implementation**

---
*Last Updated: November 6, 2025*

