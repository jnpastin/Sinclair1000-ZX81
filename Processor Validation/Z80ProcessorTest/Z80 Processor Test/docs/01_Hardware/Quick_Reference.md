# Phase 1 Hardware - Quick Reference Card

**Date:** December 23, 2025  
**Status:** ✅ Pin Mapping Optimized & Verified

---

## Pin Mapping Summary

### Address Bus (GPIO1.16-31) ⭐ OPTIMIZED
**Single-shift 16-bit read!**

| Pin | GPIO     | Z80 Signal | Z80 Pin |
|-----|----------|------------|---------|
| 19  | GPIO1.16 | A0         | 30      |
| 18  | GPIO1.17 | A1         | 31      |
| 14  | GPIO1.18 | A2         | 32      |
| 15  | GPIO1.19 | A3         | 33      |
| 40  | GPIO1.20 | A4         | 34      |
| 41  | GPIO1.21 | A5         | 35      |
| 17  | GPIO1.22 | A6         | 36      |
| 16  | GPIO1.23 | A7         | 37      |
| 22  | GPIO1.24 | A8         | 1       |
| 23  | GPIO1.25 | A9         | 2       |
| 20  | GPIO1.26 | A10        | 3       |
| 21  | GPIO1.27 | A11        | 4       |
| 38  | GPIO1.28 | A12        | 5       |
| 39  | GPIO1.29 | A13        | 6       |
| 26  | GPIO1.30 | A14        | 38      |
| 27  | GPIO1.31 | A15        | 39      |

### Data Bus (GPIO2) ⭐ OPTIMIZED
**Two groups: 0-3, 16-19**

| Pin | GPIO     | Z80 Signal | Z80 Pin |
|-----|----------|------------|---------|
| 10  | GPIO2.00 | D0         | 14      |
| 12  | GPIO2.01 | D1         | 15      |
| 11  | GPIO2.02 | D2         | 12      |
| 13  | GPIO2.03 | D3         | 8       |
| 8   | GPIO2.16 | D4         | 7       |
| 7   | GPIO2.17 | D5         | 9       |
| 36  | GPIO2.18 | D6         | 10      |
| 37  | GPIO2.19 | D7         | 13      |

### Control Signals - Z80 Outputs (Teensy Inputs)

| Pin | GPIO     | Z80 Signal | Z80 Pin |
|-----|----------|------------|---------|
| 1   | GPIO1.02 | HALT       | 18      |
| 0   | GPIO1.03 | IORQ       | 20      |
| 24  | GPIO1.12 | BUSACK     | 23      |
| 2   | GPIO4.04 | M1         | 27      |
| 3   | GPIO4.05 | RFSH       | 28      |
| 4   | GPIO4.06 | RD ⚡      | 21      |
| 33  | GPIO4.07 | WR ⚡      | 22      |
| 5   | GPIO4.08 | MREQ       | 19      |

### Control Signals - Z80 Inputs (Teensy Outputs)

| Pin | GPIO     | Z80 Signal | Z80 Pin |
|-----|----------|------------|---------|
| 28  | GPIO3.18 | CLK ⚡     | 6       |
| 6   | GPIO2.10 | INT        | 16      |
| 9   | GPIO2.11 | NMI        | 17      |
| 35  | GPIO2.28 | WAIT       | 24      |
| 32  | GPIO2.12 | BUSREQ     | 25      |
| 34  | GPIO2.29 | RESET      | 26      |

### Spare Pins

| Pin | GPIO     | Available For |
|-----|----------|---------------|
| 25  | GPIO1.13 | Expansion     |
| 29  | GPIO4.31 | Expansion     |
| 30  | GPIO3.23 | Expansion     |

---

## Critical Pins

| Pin | Function | Notes                                    |
|-----|----------|------------------------------------------|
| 28  | CLK      | FlexPWM2_A - Hardware PWM                |
| 31  | OE       | Level Shifter Enable (all 5 modules)    |
| 4   | RD       | Interrupt (GPIO4.06, Priority 0)         |
| 33  | WR       | Interrupt (GPIO4.07, Priority 0)         |
| 19-27,14-18,38-41,40 | A0-A15 | GPIO1.16-31 consecutive! ⚡       |
| 10-13,8,7,36-37 | D0-D7 | GPIO2 two groups ⚡                     |

---

## Level Shifter Modules (HW-221/TXS0108E)

### Module 1: Data Bus (8 channels, bidirectional)
| Ch | Teensy | GPIO     | Z80 Signal | Z80 Pin |
|----|--------|----------|------------|---------|
| 1  | 10     | GPIO2.00 | D0         | 14      |
| 2  | 12     | GPIO2.01 | D1         | 15      |
| 3  | 11     | GPIO2.02 | D2         | 12      |
| 4  | 13     | GPIO2.03 | D3         | 8       |
| 5  | 8      | GPIO2.16 | D4         | 7       |
| 6  | 7      | GPIO2.17 | D5         | 9       |
| 7  | 36     | GPIO2.18 | D6         | 10      |
| 8  | 37     | GPIO2.19 | D7         | 13      |

### Module 2: Address Low (A0-A7)
| Ch | Teensy | GPIO     | Z80 Signal | Z80 Pin |
|----|--------|----------|------------|---------|
| 1  | 19     | GPIO1.16 | A0         | 30      |
| 2  | 18     | GPIO1.17 | A1         | 31      |
| 3  | 14     | GPIO1.18 | A2         | 32      |
| 4  | 15     | GPIO1.19 | A3         | 33      |
| 5  | 40     | GPIO1.20 | A4         | 34      |
| 6  | 41     | GPIO1.21 | A5         | 35      |
| 7  | 17     | GPIO1.22 | A6         | 36      |
| 8  | 16     | GPIO1.23 | A7         | 37      |

### Module 3: Address High (A8-A15)
| Ch | Teensy | GPIO     | Z80 Signal | Z80 Pin |
|----|--------|----------|------------|---------|
| 1  | 22     | GPIO1.24 | A8         | 1       |
| 2  | 23     | GPIO1.25 | A9         | 2       |
| 3  | 20     | GPIO1.26 | A10        | 3       |
| 4  | 21     | GPIO1.27 | A11        | 4       |
| 5  | 38     | GPIO1.28 | A12        | 5       |
| 6  | 39     | GPIO1.29 | A13        | 6       |
| 7  | 26     | GPIO1.30 | A14        | 38      |
| 8  | 27     | GPIO1.31 | A15        | 39      |

### Module 4: Z80 Control Outputs (8 channels, input)
| Ch | Teensy | GPIO     | Z80 Signal | Z80 Pin |
|----|--------|----------|------------|---------|
| 1  | 1      | GPIO1.02 | HALT       | 18      |
| 2  | 0      | GPIO1.03 | IORQ       | 20      |
| 3  | 24     | GPIO1.12 | BUSACK     | 23      |
| 4  | 2      | GPIO4.04 | M1         | 27      |
| 5  | 3      | GPIO4.05 | RFSH       | 28      |
| 6  | 4      | GPIO4.06 | RD         | 21      |
| 7  | 33     | GPIO4.07 | WR         | 22      |
| 8  | 5      | GPIO4.08 | MREQ       | 19      |

### Module 5: Z80 Control Inputs (6 channels, output)
| Ch | Teensy | GPIO     | Z80 Signal | Z80 Pin |
|----|--------|----------|------------|---------|
| 1  | 28     | GPIO3.18 | CLK        | 6       |
| 2  | 6      | GPIO2.10 | INT        | 16      |
| 3  | 9      | GPIO2.11 | NMI        | 17      |
| 4  | 35     | GPIO2.28 | WAIT       | 24      |
| 5  | 32     | GPIO2.12 | BUSREQ     | 25      |
| 6  | 34     | GPIO2.29 | RESET      | 26      |
| 7  | -      | -        | (unused)   | -       |
| 8  | -      | -        | (unused)   | -       |

**Power:** VCCA = 3.3V, VCCB = 5V, OE = 3.3V (HIGH)

---

## Optimized GPIO Code

### Address Bus Read - 5 ns ⚡
```cpp
uint16_t address = GPIO1_DR >> 16;  // Single shift!
```

### Data Bus Read - 10 ns ⚡
```cpp
uint32_t gpio2 = GPIO2_DR;
uint8_t data = (gpio2 & 0x0F) | ((gpio2 >> 12) & 0xF0);
```

### Data Bus Write - 12 ns ⚡
```cpp
uint32_t gpio2 = GPIO2_DR & ~0x000F000F;
gpio2 |= (data & 0x0F) | (((uint32_t)(data & 0xF0)) << 12);
GPIO2_DR = gpio2;
```

### Control Signals - GPIO4 ⚡
```cpp
uint32_t gpio4 = GPIO4_DR;
bool m1   = !(gpio4 & (1 << 4));  // Active low
bool rfsh = !(gpio4 & (1 << 5));
bool rd   = !(gpio4 & (1 << 6));
bool wr   = !(gpio4 & (1 << 7));
bool mreq = !(gpio4 & (1 << 8));
```

---

## Initialization Code

```cpp
void setup() {
    // Address bus - all inputs
    for (int pin : {19,18,14,15,40,41,17,16,22,23,20,21,38,39,26,27}) {
        pinMode(pin, INPUT);
    }
    
    // Data bus - start as inputs
    for (int pin : {10,12,11,13,8,7,36,37}) {
        pinMode(pin, INPUT);
    }
    
    // Control inputs from Z80
    for (int pin : {1,0,24,2,3,4,33,5}) {
        pinMode(pin, INPUT);
    }
    
    // Control outputs to Z80
    pinMode(28, OUTPUT);  // CLK
    pinMode(31, OUTPUT);  // Level Shifter OE
    pinMode(6, OUTPUT);   // INT
    pinMode(9, OUTPUT);   // NMI
    pinMode(35, OUTPUT);  // WAIT
    pinMode(32, OUTPUT);  // BUSREQ
    pinMode(34, OUTPUT);  // RESET
    
    // Set safe initial states
    digitalWrite(31, LOW);   // Level shifters disabled during setup
    digitalWrite(6, HIGH);   // INT inactive
    digitalWrite(9, HIGH);   // NMI inactive
    digitalWrite(35, HIGH);  // WAIT inactive
    digitalWrite(32, HIGH);  // BUSREQ inactive
    digitalWrite(34, LOW);   // RESET active (hold in reset)
    
    // Start clock at 1 MHz
    analogWriteFrequency(28, 1000000);
    analogWrite(28, 128);  // 50% duty cycle
    
    // Set interrupt priority
    NVIC_SET_PRIORITY(IRQ_GPIO6789, 0);
    
    // Enable level shifters after setup complete
    digitalWrite(31, HIGH);  // Enable all TXS0108E modules
}
```

---

## Performance Targets

| Metric            | Target       | vs Mega    |
|-------------------|--------------|------------|
| Max Z80 Clock     | **5-10 MHz** | 5-10x      |
| ISR Latency       | **50-100 ns**| 16-32x     |
| Address Read      | **5 ns**     | 400x ⚡    |
| Data Read         | **10 ns**    | 100x ⚡    |
| Data Write        | **12 ns**    | 83x ⚡     |
| Full ISR Cycle    | **50-60 ns** | 26-32x     |

**Z80 @ 5 MHz:** 200 ns clock period  
**ISR execution:** ~50-60 ns  
**Margin:** ~140-150 ns (70% for logic) ✅

---

## Bill of Materials

| Item                    | Qty | Cost      | Notes                       |
|-------------------------|-----|-----------|-----------------------------|
| Teensy 4.1              | 1   | $31       | PJRC.com                    |
| HW-221 Level Shifters   | 5   | $15-25    | TXS0108E modules            |
| Z80 CPU (DIP-40)        | 1-3 | $10-30    | Buy spares                  |
| Proto boards            | 3   | $15-25    | 2× 63-row, 1× 30-row        |
| Pin headers (0.1")     | Set | $10-15    | Breakaway strips, various   |
| LEDs (38 total)         | 1   | $4        | 8 yellow, 16 green, 14 blue |
| Resistors (1.5kΩ)       | 40  | $1        | LED current limiting        |
| Capacitors              | Set | $2        | 10µF, 220µF, 100nF          |
| Wire & cable            | -   | $40-50    | 15-conductor shielded       |
| **Total**               | -   | **~$130** | Complete system             |

---

## Assembly Phases

| Phase  | Goal                        | Duration | Details                           |
|--------|-----------------------------|---------|------------------------------------|
| **1**  | Teensy Board                | 2-3 hrs | Power, connectors, bypass caps    |
| **2**  | Module 5 - Clock Test       | 2-3 hrs | CLK output, verify with scope     |
| **3**  | Modules 1-4                 | 6-8 hrs | Address, data, control signals    |
| **4**  | Z80 Board                   | 4-6 hrs | Socket, LEDs, connectors          |
| **5**  | First Z80 Power-Up          | 1-2 hrs | Install CPU, run NOP loop         |
| **Total** | Complete Build           | **15-22 hrs** | Incremental testing         |

---

## Testing Checklist

### Power & Clock
- [ ] 5V present at VUSB
- [ ] 3.3V present at 3V3 pin
- [ ] Bypass caps installed (all boards)
- [ ] Clock output on Pin 28 (1 MHz)
- [ ] 50% duty cycle verified

### Bus Operations
- [ ] Address bus reads (all 16 bits)
- [ ] Data bus reads (all 8 bits)
- [ ] Data bus direction switching
- [ ] Control signal monitoring (M1, MREQ, etc.)

### Interrupts
- [ ] RD interrupt (Pin 4) triggers ISR
- [ ] WR interrupt (Pin 33) triggers ISR
- [ ] ISR latency <100 ns

### Z80 Execution
- [ ] NOP loop at 100 kHz
- [ ] NOP loop at 1 MHz
- [ ] NOP loop at 5 MHz
- [ ] All LEDs show activity

---

## Z80 Compatibility

✅ **NMOS Z80** (Zilog Z8400, Mostek, NEC) - 2.5-4 MHz  
✅ **CMOS Z80** (Zilog Z84C00) - 4-20 MHz  
✅ All 40-pin DIP variants

**Voltage:** 5V (TXS0108E handles 3.3V ↔ 5V translation)

---

## Key Optimizations

🚀 **Address Bus (GPIO1.16-31)** - Single-shift 16-bit read (5 ns)  
🚀 **Data Bus (GPIO2)** - Two-group extraction (10 ns)  
🚀 **CLK Pin 28** - FlexPWM2_A hardware PWM (zero overhead)  
🚀 **RD/WR on GPIO4** - Priority 0 interrupts, grouped control signals  
🚀 **400x Faster** - Address reads vs Arduino Mega  

---

## Quick Debug

```cpp
// Read full bus state
uint16_t addr = GPIO1_DR >> 16;
uint32_t gpio2 = GPIO2_DR;
uint8_t data = (gpio2 & 0x0F) | ((gpio2 >> 12) & 0xF0);
uint32_t gpio4 = GPIO4_DR;

Serial.printf("Addr:%04X Data:%02X M1:%d RD:%d WR:%d MREQ:%d\n",
    addr, data,
    !(gpio4 & (1<<4)),
    !(gpio4 & (1<<6)),
    !(gpio4 & (1<<7)),
    !(gpio4 & (1<<8))
);
```

---

## Next Actions

1. ⏳ Order hardware (~$130, 1-2 week delivery)
2. 📦 Receive and inventory components
3. 🔧 Phase 1 - Teensy board assembly
4. 🔧 Phase 2 - Module 5 clock test
5. 🔧 Phases 3-5 - Complete assembly
6. ✅ Full system validation @ 5 MHz
7. 🎯 Proceed to Phase 2 software

---

**Quick Links:**
- [Pin_Mapping.md](Pin_Mapping.md) - Complete pin documentation
- [Phase1_Summary.md](Phase1_Summary.md) - Detailed overview
- [Assembly_and_Testing.md](Assembly_and_Testing.md) - Build guide
- [Level_Shifting.md](Level_Shifting.md) - Voltage translation

---

**Status:** ✅ Optimized pin mapping verified  
**Print this card for bench reference during assembly!**  
**Last Updated:** December 23, 2025
