# Hardware Comparison: Arduino Mega 2560 vs Teensy 4.1

**Status:** 📋 Reference Document  
**Date:** December 22, 2025  
**Purpose:** Comprehensive platform comparison for migration planning

---

## Executive Summary

| Metric           | Arduino Mega 2560              | Teensy 4.1                                   | Change Factor              |
|------------------|--------------------------------|----------------------------------------------|----------------------------|
| **Processor**    | ATmega2560 (8-bit AVR)         | IMXRT1062 (32-bit ARM Cortex-M7)             | Architecture change        |
| **Clock Speed**  | 16 MHz                         | 600 MHz                                      | **37.5x faster**           |
| **Flash Memory** | 256 KB                         | 8 MB                                         | **32x more**               |
| **SRAM**         | 8 KB                           | 1024 KB (DTCM: 512KB, OCRAM: 512KB)          | **128x more**              |
| **EEPROM**       | 4 KB                           | 1080 bytes (emulated)                        | Smaller, but rarely used   |
| **GPIO Pins**    | 54 digital                     | 55 digital                                   | Similar                    |
| **Logic Voltage**| 5V                             | 3.3V                                         | **Level shifting required**|
| **Input Tolerance**| 5.5V max                     | 3.6V max (NOT 5V-tolerant)                   | **Requires level shifters** |
| **Price**        | ~$40                           | ~$30                                         | **Cheaper**                |
| **Availability** | Widely available               | Specialty vendor (PJRC)                      | More limited               |**Key Takeaway:** Teensy 4.1 is **dramatically faster** and has **much more memory**, but requires **level shifting** for 5V Z80 interface.

---

## Processor Architecture

### Arduino Mega 2560 (ATmega2560)

| Feature                 | Specification            | Notes                         |
|-------------------------|--------------------------|-------------------------------|
| **Architecture**        | 8-bit AVR RISC           | Harvard architecture          |
| **Core**                | ATmega2560               | Atmel/Microchip               |
| **Clock Speed**         | 16 MHz                   | Fixed crystal oscillator      |
| **Instruction Set**     | AVR (130+ instructions)  | 8-bit operations              |
| **Pipeline**            | 2-stage                  | Simple pipeline               |
| **Instructions/Cycle**  | 1 (most instructions)    | Single-cycle execution        |
| **Performance**         | 16 MIPS @ 16 MHz         | 1 instruction per clock (ideal)|
| **Registers**           | 32x 8-bit registers      | R0-R31 general purpose        |
| **Cache**               | None                     | No instruction/data cache     |
| **FPU**                 | None                     | Software floating point       |

### Teensy 4.1 (IMXRT1062)

| Feature                 | Specification                  | Notes                                  |
|-------------------------|--------------------------------|----------------------------------------|
| **Architecture**        | 32-bit ARM Cortex-M7           | Harvard architecture                   |
| **Core**                | IMXRT1062 (NXP i.MX RT)        | ARM Cortex-M7 rev5                     |
| **Clock Speed**         | 600 MHz (default)              | Overclockable to 1008 MHz              |
| **Instruction Set**     | Thumb-2 (ARMv7E-M)             | 16/32-bit mixed instructions           |
| **Pipeline**            | 6-stage superscalar            | Dual-issue, out-of-order execution     |
| **Instructions/Cycle**  | Up to 2 (dual-issue)           | Branch prediction, speculation         |
| **Performance**         | 1200+ MIPS @ 600 MHz           | 2+ instructions per clock              |
| **Registers**           | 16x 32-bit registers           | R0-R15 (including PC, SP, LR)          |
| **Cache**               | 32 KB I-cache, 32 KB D-cache   | 2-way set associative                  |
| **FPU**                 | Double-precision FPU           | Hardware floating point                |

**Performance Advantage:** ARM Cortex-M7 @ 600 MHz = **~1200 MIPS** vs AVR @ 16 MHz = **16 MIPS** → **75x faster** (not just 37.5x due to dual-issue).

---

## Memory Architecture

### Arduino Mega 2560

| Memory Type | Size  | Speed              | Use Case                    |
|-------------|-------|--------------------|-----------------------------|
| **Flash**   | 256 KB| Slow (~2-4 cycles) | Program storage             |
| **SRAM**    | 8 KB  | Fast (1 cycle)     | Variables, stack, heap      |
| **EEPROM**  | 4 KB  | Very slow (~3.3 ms write)| Non-volatile data     |
| **Registers**| 32x 8-bit| Instant         | ALU operations              |

**Memory Map:**
```
0x0000-0x3FFFF  Flash (256 KB) - Program code
0x0100-0x21FF   SRAM (8 KB) - RAM
0x0000-0x0FFF   EEPROM (4 KB) - Non-volatile
```

**Constraints:**
- Only **8 KB SRAM** for all variables, stack, and heap
- PROGMEM required to store constants in Flash
- Careful memory management essential

### Teensy 4.1

| Memory Type      | Size     | Speed                | Use Case                              |
|------------------|----------|----------------------|---------------------------------------|
| **Flash**        | 8 MB (QSPI)| Moderate (~10-20 cycles)| Program + data storage          |
| **DTCM RAM**     | 512 KB   | Ultra-fast (1 cycle) | Stack, variables (tightly coupled)    |
| **OCRAM**        | 512 KB   | Fast (2-3 cycles)    | DMA buffers, large arrays             |
| **ITCM RAM**     | 512 KB   | Ultra-fast (1 cycle) | Code execution (Harvard)              |
| **FlexRAM**      | 512 KB   | Configurable         | DTCM/ITCM/OCRAM partitioning          |
| **EEPROM**       | 1080 bytes| Emulated in Flash   | Non-volatile (limited)                |
| **External RAM** | Up to 8 MB| Moderate (~20 cycles)| Optional PSRAM (not populated)        |

**Memory Map:**
```
0x00000000-0x0007FFFF   ITCM (512 KB) - Code (cached)
0x20000000-0x2007FFFF   DTCM (512 KB) - Data (zero-wait)
0x20200000-0x202FFFFF   OCRAM (512 KB) - General RAM
0x60000000-0x607FFFFF   QSPI Flash (8 MB) - Program storage
0x70000000-0x707FFFFF   External RAM (8 MB) - Optional PSRAM
```

**Advantages:**
- **1 MB total RAM** (vs 8 KB on Mega) - **128x more**
- PROGMEM not needed (but still supported for compatibility)
- Plenty of room for large buffers, caches, and arrays

---

## GPIO System

### Arduino Mega 2560 (AVR PORTs)

| Feature                 | Specification                  | Notes                           |
|-------------------------|--------------------------------|---------------------------------|
| **GPIO Count**          | 54 digital, 16 analog          | Total 70 I/O pins               |
| **Logic Voltage**       | 5V (TTL/CMOS)                  | Native 5V logic                 |
| **Port Organization**   | 8 ports (A-L), 8 pins each     | PORTA, PORTB, ..., PORTL        |
| **Direct Access**       | Yes (PORTA, PORTB, etc.)       | Single-cycle read/write         |
| **Atomic Operations**   | 8 bits per port                | All 8 pins on port at once      |
| **Read/Write Speed**    | ~125 ns (2 cycles @ 16 MHz)    | Direct port manipulation        |
| **pinMode Speed**       | ~5 µs                          | Arduino API overhead            |
| **digitalWrite Speed**  | ~4 µs                          | Arduino API overhead            |
| **digitalRead Speed**   | ~4 µs                          | Arduino API overhead            |
| **Max Toggle Rate**     | ~4 MHz                         | Using direct port writes        |
| **5V Tolerant**         | Yes                            | Native 5V operation             |
| **Pull-up Resistors**   | Internal 20-50 kΩ             | Configurable per pin            |

**Port Examples:**
```cpp
PORTA = 0xFF;        // Set all 8 pins HIGH (125 ns)
uint8_t val = PINA;  // Read all 8 pins (125 ns)
DDRA = 0xFF;         // Configure all 8 as outputs (125 ns)
```

### Teensy 4.1 (GPIO Banks)

| Feature                 | Specification                      | Notes                                         |
|-------------------------|------------------------------------|-------------------------------------------------|
| **GPIO Count**          | 55 digital, 18 analog              | Total 55 I/O pins                             |
| **Logic Voltage**       | 3.3V (LVCMOS)                      | **3.3V only (NOT 5V tolerant)**               |
| **GPIO Organization**   | 9 GPIO banks, 32 bits each         | GPIO1-GPIO9 (32-bit wide)                     |
| **Direct Access**       | Yes (GPIO6_DR, GPIO7_DR, etc.)     | Ultra-fast register access                    |
| **Atomic Operations**   | 32 bits per bank                   | All 32 pins in bank at once                   |
| **Read/Write Speed**    | ~1.7 ns (1 cycle @ 600 MHz)        | Direct register access                        |
| **pinMode Speed**       | ~150 ns                            | Arduino API overhead                          |
| **digitalWrite Speed**  | ~150 ns                            | Arduino API overhead                          |
| **digitalRead Speed**   | ~150 ns                            | Arduino API overhead                          |
| **Max Toggle Rate**     | 150+ MHz                           | Using direct register writes                  |
| **5V Tolerant**         | **NO** (3.3V max on all GPIO)      | **Level shifters mandatory for 5V systems**   |
| **Pull-up Resistors**   | Internal 22 kΩ                    | Configurable per pin                          |

**GPIO Register Examples:**
```cpp
GPIO6_DR = 0xFFFFFFFF;       // Set all GPIO6 pins HIGH (1.7 ns)
uint32_t val = GPIO6_PSR;    // Read all GPIO6 pins (1.7 ns)
GPIO6_GDIR = 0xFFFFFFFF;     // Configure all GPIO6 as outputs (1.7 ns)

// Atomic set/clear/toggle
GPIO6_DR_SET = (1 << 18);    // Set pin 14 (A0) HIGH (atomic)
GPIO6_DR_CLEAR = (1 << 19);  // Set pin 15 (A1) LOW (atomic)
GPIO6_DR_TOGGLE = (1 << 24); // Toggle pin 22 (A8)
```

**Performance Comparison:**
- **Direct register:** 1.7 ns (Teensy) vs 125 ns (Mega) = **73x faster**
- **Arduino API:** 150 ns (Teensy) vs 4 µs (Mega) = **27x faster**

---

## Timer/PWM System

### Arduino Mega 2560 (AVR Timers)

| Timer      | Type   | Channels | Resolution  | Max Frequency | PWM Pins     |
|------------|--------|----------|-------------|---------------|---------------|
| **Timer0** | 8-bit  | 2        | 256 steps   | 62.5 KHz      | 4, 13        |
| **Timer1** | 16-bit | 3        | 65536 steps | 1 MHz         | 11, 12, 13   |
| **Timer2** | 8-bit  | 2        | 256 steps   | 62.5 KHz      | 9, 10        |
| **Timer3** | 16-bit | 3        | 65536 steps | 1 MHz         | 2, 3, 5      |
| **Timer4** | 16-bit | 3        | 65536 steps | 1 MHz         | 6, 7, 8      |
| **Timer5** | 16-bit | 3        | 65536 steps | 1 MHz         | 44, 45, 46   |**Key Features:**
- **Timer1** used for Z80 clock generation (Pin 11, OC1A)
- Hardware PWM with configurable frequency
- Prescalers: 1, 8, 64, 256, 1024
- CTC mode for arbitrary frequencies
- Input capture for frequency measurement

**Z80 Clock Example (Timer1):**
```cpp
// Generate 1 MHz clock on pin 11 (OC1A)
ICR1 = 7;              // TOP value (16 MHz / (1+1) / 8 = 1 MHz)
OCR1A = 3;             // 50% duty cycle
TCCR1A = _BV(COM1A1) | _BV(WGM11);  // Non-inverting PWM, mode 14
TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS11);  // Mode 14, prescaler=8
```

### Teensy 4.1 (FlexPWM & QuadTimer)

| Timer          | Type       | Channels   | Resolution  | Max Frequency | PWM Pins  |
|----------------|------------|------------|-------------|---------------|------------|
| **FlexPWM1**   | 16-bit PWM | 12 channels| 65536 steps | 150 MHz       | Multiple  |
| **FlexPWM2**   | 16-bit PWM | 12 channels| 65536 steps | 150 MHz       | Multiple  |
| **FlexPWM3**   | 16-bit PWM | 12 channels| 65536 steps | 150 MHz       | Multiple  |
| **FlexPWM4**   | 16-bit PWM | 12 channels| 65536 steps | 150 MHz       | Multiple  |
| **QuadTimer**  | 16-bit     | 16 channels| 65536 steps | 150 MHz       | Multiple  |
| **GPT**        | 32-bit     | 2 channels | 4B steps    | 150 MHz       | Software  |**Key Features:**
- **FlexPWM** optimized for motor control and high-frequency PWM
- **QuadTimer** for general-purpose timing and pulse counting
- Many more timer channels than AVR
- Much higher frequencies (150 MHz vs 1 MHz)
- Center-aligned and edge-aligned PWM
- Dead-time insertion for motor control

**Z80 Clock Example (FlexPWM2):**
```cpp
// Generate 5 MHz clock on pin 36 (FlexPWM2_A)
// Using FlexPWM submodule 2, channel A
FLEXPWM2_SM2VAL0 = 0;           // Counter start
FLEXPWM2_SM2VAL1 = 59;          // Period (600 MHz / 60 = 10 MHz PWM, /2 = 5 MHz output)
FLEXPWM2_SM2VAL3 = 30;          // Duty cycle (50%)
FLEXPWM2_MCTRL |= FLEXPWM_MCTRL_RUN(4);  // Start submodule 2
```

**Clock Frequency Range:**
- Arduino Mega: **100 KHz - 3 MHz** (limited by ISR latency)
- Teensy 4.1: **100 KHz - 10+ MHz** (much lower ISR latency)

---

## Interrupt System

### Arduino Mega 2560 (AVR Interrupts)

| Feature                    | Specification                      | Notes                              |
|----------------------------|------------------------------------|------------------------------------- |
| **External Interrupts**    | 8 (INT0-INT7)                      | Dedicated interrupt pins           |
| **Pin Change Interrupts**  | 24 (PCINT0-23)                     | Any pin can trigger interrupt      |
| **Available INT Pins**     | 2, 3, 18, 19, 20, 21               | Digital pins with INT              |
| **Trigger Modes**          | LOW, CHANGE, RISING, FALLING       | Configurable per pin               |
| **Priority**               | Fixed priority (INT0 highest)      | No runtime priority change         |
| **ISR Latency**            | ~1-2 µs                            | Includes context save/restore      |
| **Nested Interrupts**      | No (disabled during ISR)           | Can enable manually (risky)        |
| **Interrupt Controller**   | Simple hardware                    | Fixed 35 interrupt vectors         |

**Z80 Project Uses:**
- **INT4 (Pin 2):** /WR signal (write cycle detection)
- **INT5 (Pin 3):** /RD signal (read cycle detection)
- Falling-edge trigger for active-low signals

**Example:**
```cpp
// Enable INT4 for falling edge
EICRA |= (1 << ISC41);  // Falling edge
EICRA &= ~(1 << ISC40);
EIMSK |= (1 << INT4);   // Enable INT4
sei();                  // Global interrupt enable

ISR(INT4_vect) {
  // Handle write cycle (~1.6 µs latency measured)
}
```

### Teensy 4.1 (ARM NVIC)

| Feature                    | Specification                      | Notes                              |
|----------------------------|------------------------------------|------------------------------------- |
| **GPIO Interrupts**        | All GPIO pins                      | Any pin can be interrupt source    |
| **Interrupt Controller**   | NVIC (Nested Vectored)             | ARM Cortex-M7 standard             |
| **Priority Levels**        | 16 priority levels (0-15)          | Fully configurable                 |
| **Preemption**             | Yes                                | Higher priority can preempt lower  |
| **Sub-priority**           | 16 sub-priorities per level        | Fine-grained control               |
| **Trigger Modes**          | LOW, HIGH, CHANGE, RISING, FALLING | Configurable per pin               |
| **ISR Latency**            | ~50-100 ns (12-60 cycles @ 600 MHz)| Ultra-fast context switch          |
| **Nested Interrupts**      | Yes                                | Automatic with priority levels     |
| **Interrupt Vectors**      | 256+ vectors                       | Extensive peripheral support       |

**Z80 Project Uses:**
- **Pin 28 (/RD):** GPIO8 interrupt, priority 0 (highest)
- **Pin 30 (/WR):** GPIO8 interrupt, priority 0 (highest)
- Both on same GPIO bank for fast ISR context

**Example:**
```cpp
// Attach interrupt to pin 24 (/RD signal)
attachInterrupt(digitalPinToInterrupt(24), rdISR, FALLING);
NVIC_SET_PRIORITY(IRQ_GPIO6789, 0);  // Highest priority

void rdISR() {
  // Handle read cycle (~50-100 ns latency)
  // Can be preempted by higher priority (none here)
}
```

**Latency Comparison:**
- **Arduino Mega:** ~1.6 µs (measured)
- **Teensy 4.1:** ~50-100 ns (estimated) = **16-32x faster**

---

## Communication Interfaces

### Arduino Mega 2560

| Interface          | Count | Speed               | Pins                            |
|--------------------|-------|---------------------|---------------------------------|
| **UART (Serial)**  | 4     | Up to 2 Mbps        | 0/1, 19/18, 17/16, 15/14        |
| **I2C (Wire)**     | 1     | 400 Kbps            | 20, 21 (SDA, SCL)               |
| **SPI**            | 1     | 8 Mbps              | 50-53 (MISO, MOSI, SCK, SS)     |
| **USB**            | 1 (ATmega16U2) | 12 Mbps (USB 1.1) | Via USB-serial converter     |

### Teensy 4.1

| Interface          | Count | Speed               | Pins                            |
|--------------------|-------|---------------------|---------------------------------|
| **UART (Serial)**  | 8     | Up to 12 Mbps       | Multiple pin options            |
| **I2C (Wire)**     | 3     | 1 Mbps              | Multiple pin options            |
| **SPI**            | 3     | 30+ Mbps            | Multiple pin options            |
| **USB**            | 2 (Host + Device)| 480 Mbps (USB 2.0) | Native USB controller      |
| **Ethernet**       | 1     | 100 Mbps            | Built-in PHY                    |
| **CAN**            | 3     | 1 Mbps              | CAN FD capable                  |
| **I2S**            | 2     | 192 KHz audio       | Digital audio                   |
| **SD Card**        | 1     | SDIO                | Built-in microSD slot           |

**Advantages:** More peripherals, much faster, native USB, Ethernet, SD card.

---

## Physical Specifications

### Arduino Mega 2560

| Feature              | Specification                    |
|----------------------|----------------------------------|
| **Form Factor**      | 101.6 x 53.3 mm (4" x 2.1")      |
| **Weight**           | 37 grams                         |
| **Mounting Holes**   | 4x 3.2mm (0.125")                |
| **Header Pitch**     | 2.54 mm (0.1")                   |
| **Power Input**      | 7-12V DC (barrel jack or VIN pin)|
| **Current per I/O**  | 40 mA max (20 mA recommended)    |
| **Total I/O Current**| 200 mA max                       |

### Teensy 4.1

| Feature              | Specification                    |
|----------------------|----------------------------------|
| **Form Factor**      | 61 x 18 mm (2.4" x 0.7")         |
| **Weight**           | 5 grams                          |
| **Mounting Holes**   | 4x 2mm                           |
| **Header Pitch**     | 2.54 mm (0.1")                   |
| **Power Input**      | 5V USB or 3.6-5.5V VIN pin       |
| **Current per I/O**  | 4 mA source, 8 mA sink (per pin) |
| **Total I/O Current**| ~500 mA total                    |

**Note:** Teensy is **much smaller and lighter** than Arduino Mega.

---

## Power Consumption

### Arduino Mega 2560

| Mode               | Current @ 5V  | Power        |
|--------------------|---------------|---------------|
| **Active (no load)**| 80-90 mA      | 0.4-0.45 W   |
| **Active (typical)**| 100-150 mA    | 0.5-0.75 W   |
| **Sleep mode**     | 15-20 mA      | 0.075-0.1 W  |

### Teensy 4.1

| Mode               | Current @ 5V  | Power        |
|--------------------|---------------|---------------|
| **Active (600 MHz)**| 100-130 mA    | 0.5-0.65 W   |
| **Active (24 MHz)** | 20-30 mA      | 0.1-0.15 W   |
| **Sleep mode**     | 5-10 mA       | 0.025-0.05 W |

**Note:** Despite being 37.5x faster, Teensy consumes similar power to Mega in active mode!

---

## Development Environment

### Arduino Mega 2560

| Tool              | Support    | Notes                           |
|-------------------|------------|---------------------------------|
| **Arduino IDE**   | Native     | Official support                |
| **PlatformIO**    | Excellent  | Full support                    |
| **Debugger**      | Limited    | Serial + debugWIRE (complex)    |
| **Bootloader**    | Optiboot   | 0.5 KB overhead                 |
| **Upload Method** | USB Serial | Via ATmega16U2                  |
| **Upload Speed**  | 115200 baud| ~30 seconds for 256KB           |

### Teensy 4.1

| Tool              | Support           | Notes                           |
|-------------------|-------------------|---------------------------------|
| **Arduino IDE**   | Via Teensyduino   | Add-on required                 |
| **PlatformIO**    | Excellent         | Full support                    |
| **Debugger**      | SWD/JTAG          | Via external adapter            |
| **Bootloader**    | Native ROM        | 0 KB overhead                   |
| **Upload Method** | USB HID           | Native USB, no serial adapter   |
| **Upload Speed**  | USB 2.0           | ~5 seconds for 8MB              |

**Advantage:** Teensy has native USB, much faster uploads.

---

## Cost & Availability

### Arduino Mega 2560

| Source                | Price (USD) | Availability       |
|-----------------------|-------------|--------------------|
| **Official Arduino**  | $40-45      | Widely available   |
| **Clone (Generic)**   | $10-20      | Very common        |
| **Distributors**      | DigiKey, Mouser, Amazon, eBay | Global |

### Teensy 4.1

| Source            | Price (USD) | Availability           |
|-------------------|-------------|------------------------|
| **Official PJRC** | $27-30      | Direct from manufacturer|
| **Distributors**  | SparkFun, Adafruit, DigiKey | Limited       |
| **Clones**        | Not available| Proprietary design    |**Note:** Teensy is **cheaper** than official Arduino Mega, but only from PJRC or select distributors.

---

## Migration Impact Summary

| Aspect             | Impact                         | Mitigation                        |
|--------------------|--------------------------------|-----------------------------------|
| **Performance**    | ✅ 37.5x-75x faster            | Huge improvement, use it!         |
| **Memory**         | ✅ 128x more RAM               | Eliminates PROGMEM needs          |
| **GPIO Speed**     | ✅ 73x faster registers        | Rewrite HAL for direct access     |
| **Timers**         | 🟡 Different API            | Port clock.cpp to FlexPWM         |
| **Interrupts**     | 🟡 NVIC vs AVR               | Port ISR to attachInterrupt()     |
| **Level Shifting** | ⚠️ Required (3.3V→5V)        | Add TXS0108E modules              |
| **Cost**           | ✅ Cheaper                     | No issue                          |
| **Availability**   | 🟡 PJRC only                | Order early                       |
| **Code Changes**   | 🟡 Moderate                 | HAL abstraction helps             |

**Overall:** Migration is **highly beneficial** with manageable challenges.

---

## Recommendation

**✅ Proceed with Teensy 4.1 migration**

**Reasons:**
1. **Massive performance improvement** (37.5x-75x)
2. **Eliminates memory constraints** (128x more RAM)
3. **Faster I/O and interrupts** (25x-73x)
4. **Cheaper than Arduino Mega** ($27 vs $40)
5. **Level shifting is straightforward** (TXS0108E modules)
6. **Can achieve 5+ MHz Z80 operation** (vs 1 MHz on Mega)

**Challenges:**
- Code porting (HAL, timers, interrupts)
- Level shifter hardware design
- Learning ARM architecture
- PJRC-only source

**Mitigation:**
- Good documentation (this repo!)
- HW-221 modules simplify level shifting
- PlatformIO supports both platforms
- Plan ahead for lead times

---

**Status:** ✅ Complete - Ready for decision-making  
**Next Document:** Return to [Phase 1 README](README.md)
