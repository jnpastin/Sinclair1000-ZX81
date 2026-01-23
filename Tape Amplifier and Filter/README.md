# Timex Sinclair 1000 / ZX81 Tape Interface

> A modern tape interface circuit for loading and saving programs on Timex Sinclair 1000 and ZX81 computers using modern audio devices

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![KiCad](https://img.shields.io/badge/KiCad-9.0-blue.svg)](https://www.kicad.org/)

## Features

- ✅ **Bidirectional Operation**: Load programs TO the TS1000 from tape/audio files, and save programs FROM the TS1000 to modern recording devices
- ✅ **High-Gain Amplification**: 40-50× voltage gain for weak modern audio sources
- ✅ **Active Filtering**: Removes 50/60Hz power line hum and out-of-band noise  
- ✅ **Visual Feedback**: LED indicators show signal level - green for adequate signal, red for excessive signal
- ✅ **Adjustable Volume**: Front-panel volume control for different source levels; threshold trimmers for one-time setup
- ✅ **Input Protection**: Clipping diodes prevent damage from excessive signal levels
- ✅ **Built-in TS-to-TRRS Adapter**: Converts TS1000 MIC output to TRRS for recording on modern phones/computers
- ✅ **Single Supply**: Runs from 4× AA batteries (6V), no external power needed
- ✅ **DIY-Friendly**: Through-hole components on solderable breadboard

## Table of Contents

- [Background](#background)
- [Problem Statement](#problem-statement)
- [How It Works](#how-it-works)
- [Hardware](#hardware)
- [Building the Circuit](#building-the-circuit)
- [Usage](#usage)
- [Adjustment Procedure](#adjustment-procedure)
- [Troubleshooting](#troubleshooting)
- [Technical Details](#technical-details)
- [Contributing](#contributing)
- [License](#license)

## Background

The Timex Sinclair 1000 (US version of the Sinclair ZX81) used cassette tapes for program storage, encoding digital data as audio tones using a Kansas City Standard-like format at 250 baud. While this was revolutionary in 1981, interfacing these vintage computers with modern audio equipment presents several challenges that this project addresses.

## Problem Statement

### Signal Level Requirements

The Timex Sinclair 1000 / ZX81 expects an EAR input signal with specific characteristics:
- **Required Peak-to-Peak Voltage**: Approximately 1-3V p-p for reliable triggering
- **Optimal Range**: 1.6-2.0V p-p (tested and verified)
- **Input Impedance**: Relatively high impedance (typically >10kΩ)
- **Signal Type**: AC-coupled square wave or audio tone burst
- **Frequency Range**: 1-4 kHz (nominally 1.5 kHz for '0' bits, 3 kHz for '1' bits)

### Modern Device Output Limitations

**Vintage Tape Decks** (1980s era):
- Typical headphone output: 100-500mV RMS (280mV-1.4V p-p)
- Usually sufficient for direct connection
- Often have adjustable volume controls
- May suffer from worn heads, belts, and electronics

**Modern Smartphones/Tablets**:
- Headphone output: 50-200mV RMS (140-560mV p-p) into high impedance
- **Problem**: Often too weak for reliable TS1000 triggering without amplification
- Line-level outputs even lower (~50-100mV RMS)
- Volume limiting for hearing protection reduces maximum output

**Modern Laptops**:
- Headphone/line output: 100-300mV RMS (280-840mV p-p)
- **Problem**: Marginally sufficient, prone to errors especially with aged tapes
- Modern audio codecs may have reduced low-frequency response
- USB audio interfaces typically output line level (~300mV RMS maximum)

**Modern Portable Tape Players** (if still available):
- Similar to vintage units but often lower quality
- Typical output: 50-150mV RMS (140-420mV p-p)
- **Problem**: Usually insufficient without amplification

### Additional Challenges

1. **Impedance Mismatch**: Modern devices expect low-impedance loads (16-32Ω headphones), may not drive high-impedance computer inputs efficiently

2. **Signal Degradation**: Aged cassette tapes suffer from:
   - Oxide shedding reducing output level
   - Increased noise floor
   - Frequency response variations

3. **Electrical Noise**: Modern switching power supplies and digital circuitry introduce:
   - 50/60Hz power line hum
   - High-frequency switching noise
   - Ground loop issues

4. **Inconsistent Output Levels**: Different playback devices and tape conditions require adaptive level control

### The Solution (LOADING)

This circuit addresses these issues by providing:
- **Amplification**: 40-50× voltage gain ensures sufficient signal level
- **Impedance Buffering**: High input impedance won't load down source; low output impedance drives computer input
- **Filtering**: Removes power line hum and out-of-band noise
- **Adjustable Sensitivity**: Volume control accommodates different source levels
- **Visual Feedback**: LEDs confirm proper signal levels during loading
- **Protection**: Input clipping prevents damage from excessive signals

**Bottom Line**: While direct connection from vintage equipment may work, modern audio sources typically require amplification and conditioning for reliable tape loading with the TS1000/ZX81.

### The Reverse Problem: Recording FROM the TS1000

When **saving** programs from the TS1000 to modern recording devices:

**TS1000 MIC Output**:
- Standard 3.5mm mono (TS) jack
- Output level: ~1V p-p typical
- Mono signal on Tip, Ground on Sleeve

**Modern Recording Devices**:
- Smartphones/tablets: TRRS (4-conductor) jacks
- Laptops: Often TRRS combo jack (mic + headphone)
- **Problem**: TRRS pinout incompatible with TS mono output
- Mono TS plug in TRRS jack may not make proper contact
- Different TRRS standards (OMTP vs CTIA) add confusion

### The Solution (SAVING)

**Built-in TS-to-TRRS Adapter**: J3 and J4 jacks are panel-mounted in the enclosure and hardwired together to form a passive adapter:
- J4 (TS jack) accepts TS1000 MIC output
- J3 (TRRS jack) connects to phone/computer recording input
- Wiring: J4 Tip → J3 Sleeve (microphone input), J3 Ring2 & J4 Sleeve → Ground
- Properly routes mono signal to TRRS microphone input for clean recording

**Note**: This adapter is entirely passive - just wired jacks in the enclosure, not connected to the PCB or active circuitry.

## How It Works

### For LOADING Programs (Tape/Audio → TS1000)

The amplifier/filter path processes incoming audio:

1. **Amplifies** weak audio signals from tape players, smartphones, or laptops
2. **Filters** the signal to remove 50/60Hz hum and noise
3. **Conditions** the signal with proper DC bias for single-supply operation
4. **Detects** signal presence and quality with LED indicators
5. **Outputs** clean, properly-leveled signal to the TS1000 EAR input

### For SAVING Programs (TS1000 → Phone/Computer)

The built-in TS-to-TRRS adapter (J3/J4):

1. **Accepts** TS1000 MIC output via J4 (mono TS jack)
2. **Converts** to TRRS connector (J3) for modern devices
3. **Routes** signal: J4 Tip → J3 Sleeve (microphone input)
4. **Provides** proper ground: J3 Ring2 & J4 Sleeve → Ground
5. **Enables** direct recording to smartphones, tablets, and laptop combo jacks

**Note**: This is a passive adapter built into the enclosure - no active circuitry or power required.

## Hardware

### Bill of Materials

#### Active Components
| Reference | Value | Description | Notes |
|-----------|-------|-------------|-------|
| U1 | L78L05 | 5V Voltage Regulator | TO-92 package |
| U2 | LM358 | Dual Op-Amp | DIP-8 package |
| U3 | LM393 | Dual Comparator | DIP-8 package |

#### Passive Components
| Reference | Value | Type | Notes |
|-----------|-------|------|-------|
| R1 | 1kΩ | Resistor | 1/4W |
| R2, R4, R11 | 100kΩ | Resistor | 1/4W |
| R3 | 2.2kΩ | Resistor | 1/4W |
| R5, R10 | 10kΩ | Resistor | 1/4W |
| R6 | 1MΩ | Resistor | 1/4W |
| R7, R13, R15 | 22kΩ | Resistor | 1/4W |
| R8 | 47kΩ | Resistor | 1/4W |
| R9, R14 | 470Ω | Resistor | 1/4W, LED current limiting |
| R12 | 15kΩ | Resistor | 1/4W |
| RV1 | 10kΩ | Switched Potentiometer | Panel mount, volume control with integrated power switch (includes SW1) |
| RV2, RV3 | 10kΩ | Trimmer Potentiometer | PCB mount, threshold adjustment |
| C1 | 2.2nF | Ceramic Capacitor | |
| C2, C4, C5, C7 | 4.7µF-10µF | Electrolytic Capacitor | 16V or higher |
| C3, C6, C8, C10 | 0.1µF | Ceramic Capacitor | Decoupling |
| C9 | 1µF | Film Capacitor | **Must be film type** |
| D1, D2, D3 | 1N4148 | Diode | DO-35 package |
| D4 | Red LED | 5mm LED | Weak signal indicator |
| D5 | Green LED | 5mm LED | Signal strength indicator |

#### Connectors & Hardware
| Reference | Description | Notes |
|-----------|-------------|-------|
| J1 | 3.5mm Audio Jack (LOAD IN) | Panel mount, mono (TS), from tape/audio source |
| J2 | 3.5mm Audio Jack (LOAD OUT) | Panel mount, mono (TS), to TS1000 EAR input |
| J3 | 3.5mm Audio Jack (SAVE OUT) | Panel mount, TRRS, to phone/computer |
| J4 | 3.5mm Audio Jack (SAVE IN) | Panel mount, mono (TS), from TS1000 MIC output |
| BT1 | 4× AA Battery Holder | 6V nominal |
| RV1/SW1 | Switched Potentiometer | 10kΩ with integrated SPST switch |

**Note**: J3 and J4 are hardwired together (not on PCB) to form the TS-to-TRRS adapter. SW1 is integrated into the volume potentiometer (RV1).

#### Circuit Board
- **ElectroCookie Large Solderable Breadboard**
  - [Product Link](https://electrocookie.net/product/electrocookie-pcb-prototype-board-large-solderable-breadboard-for-elec/73/category/26/display/1/)
  - Through-hole prototype board with solderable pads
  - Accommodates DIP ICs and standard component spacing

### Enclosure

A custom 3D-printed enclosure design is included (STEP file provided):
- Front panel cutouts for:
  - 4× 3.5mm audio jacks (labeled: LOAD IN, LOAD OUT, SAVE IN, SAVE OUT)
  - 1× potentiometer shaft (labeled: VOLUME - also serves as power switch)
  - 2× LEDs (labeled: SIGNAL, WEAK)
- Rear panel: Battery compartment access
- Built-in TS-to-TRRS adapter (J3/J4 wired together inside enclosure)
- Compact design suitable for desktop use

## Building the Circuit

### Prerequisites

- Basic soldering skills
- Multimeter for testing
- Wire strippers and cutters
- Small screwdrivers

### Assembly Steps

1. **Component Placement Planning**
   - Lay out ICs and larger components first
   - Plan signal flow: Input (left) → Amplification (center) → Output (right)
   - Keep power supply components separate to minimize noise

2. **Solder IC Sockets** (recommended)
   - Install DIP-8 sockets for U2, U3
   - Ensures ICs can be replaced if damaged
   - Verify pin 1 orientation

3. **Install Passive Components**
   - Start with resistors (verify values with multimeter)
   - Add ceramic capacitors (non-polarized)
   - Install electrolytic capacitors (watch polarity!)
   - **Critical**: C9 must be a film capacitor for low distortion

4. **Install Active Components**
   - Insert ICs into sockets (check orientation!)
   - Install U1 voltage regulator (verify pinout: TO-92 package)
   - Install diodes (watch polarity - cathode band)

5. **Wiring**
   - Wire J1, J2 jacks to board (keep wires short and neat)
   - Install RV2, RV3 trimmers on board
   - Wire panel-mount RV1 (switched potentiometer) to board - includes both volume control and power switch
   - Wire LEDs with current-limiting resistors
   - Connect battery holder to RV1's integrated switch
   - **Wire J3 to J4**: J4 Tip → J3 Sleeve, J4 Sleeve & J3 Ring2 → Ground (not connected to PCB)

6. **Initial Testing**
   - **Before applying power**: Check for shorts with multimeter
   - Verify power supply polarity
   - Check all ICs are properly seated
   - Apply power and verify 6V at supply points, 5V at U1 output

7. **Enclosure Assembly**
   - Mount board in enclosure with standoffs or adhesive
   - Position board so RV2, RV3 trimmers are accessible when enclosure is opened
   - Install panel-mount components (jacks, RV1 switched pot, LEDs)
   - Label front panel: VOLUME (note: turning clockwise from off position powers on), LOAD IN, LOAD OUT, SAVE IN, SAVE OUT, SIGNAL, WEAK
   - Ensure battery access and trimmer access remain clear
   - **Do not permanently seal** - you may need to adjust thresholds for different tape sources

### Safety Notes

- Electrolytic capacitors are polarized - incorrect installation can cause explosion
- Ensure adequate ventilation when soldering
- Double-check IC orientation before powering on
- Use fresh batteries for initial testing

## Usage

### Loading Programs INTO the TS1000

**Important**: Proper tape loading requires setting the volume level before starting the load command.

1. **Connect Audio Source**: Plug tape player, smartphone, or laptop into **LOAD IN** jack (J1)
2. **Connect to TS1000**: Plug **LOAD OUT** jack (J2) into the TS1000's EAR input
3. **Power On**: Turn **VOLUME** knob clockwise from off position (click indicates power on)
4. **Find the Data**: Play the tape/audio file and fast-forward until you hear the data tones start
5. **Adjust Volume**: While tones are playing, adjust VOLUME knob until:
   - **Green LED (D5)** is lit (signal above minimum threshold)
   - **Red LED (D4)** is NOT lit (signal not too strong)
   - **This is critical** - both volume and thresholds must be set correctly before loading
6. **Rewind**: Rewind the tape to approximately 5 seconds BEFORE the tones start (the silence preceding the data)
   - The TS1000 expects to "hear" silence first, then the tones
   - Don't rewind to the tones themselves - go back to the quiet part before them
7. **Prepare to Load**: Type `LOAD ""` and press ENTER on the TS1000
   - The TS1000 will display a flashing cursor and wait for audio input
8. **Start Playback**: Press play on the tape player
   - The TS1000 should detect the signal and begin loading
   - You should see the screen flashing as data loads

**Troubleshooting**: 
- If loading fails, check that only the green LED is lit during data tones
- If both LEDs are off, increase VOLUME
- If both LEDs are on, decrease VOLUME
- If you can't get proper LED indication at any volume level, see [Adjustment Procedure](#adjustment-procedure)

**Note**: If LEDs don't behave correctly with various sources, you may need to open the enclosure and adjust the internal threshold trimmers (RV2, RV3). See [Adjustment Procedure](#adjustment-procedure).

### Saving Programs FROM the TS1000

1. **Connect TS1000**: Plug the TS1000's MIC output into **SAVE IN** jack (J4 - mono TS)
2. **Connect Recording Device**: Plug smartphone/laptop into **SAVE OUT** jack (J3 - TRRS) using TRRS cable
3. **Start Recording App**: Launch Audacity, Voice Memos, or similar audio recording app
4. **Save on TS1000**: Type `SAVE "FILENAME"` and press ENTER on the TS1000
5. **Record**: The program will be saved as an audio file on your modern device
6. **Stop Recording**: Stop recording app after TS1000 completes save operation

**Note**: The SAVE path is a passive TS-to-TRRS adapter built into the enclosure - no power or adjustment needed.

## Adjustment Procedure

### Initial Setup (One-Time Threshold Calibration)

The threshold trimmers (RV2, RV3) are internal adjustments that should be set once during initial setup. This requires opening the enclosure.

1. **Power On**: Connect 6V battery and turn VOLUME knob clockwise (click indicates power on)
2. **Input**: Connect tape player or audio source to J1 (LOAD IN)
3. **Output**: Connect J2 (LOAD OUT) to TS1000 EAR input
4. **Volume**: Set VOLUME to mid-position
5. **Play Test Tape**: Use a known good tape or audio file
6. **Adjust Internal Thresholds** (requires small screwdriver):
   - **Recommended Settings**: Lower threshold ~1.6V p-p, Upper threshold ~2.0V p-p (measured at J2 output)
   - **RV3 (LOWER THRESHOLD)**: Adjust until Green LED (D5) turns on when data signal is present
     - Target: ~1.6V p-p output signal
   - **RV2 (UPPER THRESHOLD)**: Adjust until Red LED (D4) turns on only when signal is too strong
     - Target: ~2.0V p-p output signal
   - Goal: Find settings where Green LED is on during normal playback, Red LED is off
   - Fine-tune for reliable reading - test with multiple tapes if possible
7. **Test Load**: Run `LOAD ""` command on TS1000 and verify successful loading
8. **Close Enclosure**: Once thresholds are set correctly, close the enclosure - normal operation uses only the volume control

### Normal Operation (Day-to-Day Use)

Once thresholds are calibrated, you should only need to adjust:
- **RV1 (VOLUME)**: Adjust for different tape/audio source levels
- Start at 50%, increase if TS1000 won't load, decrease if output is distorted

### LED Behavior Guide

**Proper Operation**:
- **Green LED (D5) ON, Red LED (D4) OFF**: Signal level is correct - this is what you want!

**Signal Too Weak**:
- **Both LEDs OFF**: Signal is below minimum threshold - increase VOLUME

**Signal Too Strong**:
- **Both LEDs ON**: Signal is above maximum threshold - decrease VOLUME or source level

**Threshold Adjustment Needed**:
- If you can't get only green LED to light at any volume setting, you may need to re-adjust internal thresholds (RV2, RV3)
- Green LED should turn on at moderate signal levels
- Red LED should only turn on when signal is excessively strong

## Troubleshooting

### Common Issues

| Symptom | Possible Cause | Solution |
|---------|---------------|----------|
| No output signal | Dead battery | Check/replace battery |
| No output signal | Volume/power off | Turn VOLUME knob clockwise past click |
| Distorted output | Gain too high | Reduce VOLUME (turn counterclockwise) |
| TS1000 won't load | Signal too weak | Increase VOLUME (turn clockwise) |
| TS1000 won't load | Thresholds wrong | Re-adjust RV2, RV3 with known good tape |
| LEDs both on | Signal too strong | Reduce VOLUME or reduce source output level |
| LEDs both off | No/weak input signal | Increase VOLUME, check audio source, verify playback |
| Only red LED on | Impossible state | Check wiring, LED polarity |
| Weak output | Bad coupling capacitors | Check C2, C5, C9 |
| Noisy signal | Poor decoupling | Check C3, C6, C8, C10 |
| SAVE not recording | Wrong jack | Verify using J3 (TRRS), not J2 |
| SAVE distorted | AGC enabled | Disable auto-gain in recording app |
| SAVE no audio | Adapter wiring | Verify J4 Tip → J3 Sleeve connection |

### Advanced Troubleshooting

**Measuring Voltages**:
- U2, U3 Pin 8: Should read ~6V (battery voltage)
- U2, U3 Pin 4: Should read 0V (ground)
- U1 Pin 1: Should read ~5V (regulated output)
- Check all IC pins for correct voltages

**Signal Tracing**:
- Use oscilloscope or audio probe at U2 Pin 1 (first amp output)
- Verify signal amplitude increases through amplification stages
- Check for clipping or distortion at any stage

## Technical Details

### Circuit Architecture

The amplifier circuit consists of four main stages:

#### Stage 1: Input Conditioning and Initial Amplification (U2A - LM358)
- **Purpose**: AC couple the input signal and provide initial amplification
- **Configuration**: Non-inverting amplifier with AC coupling
- **Gain**: Approximately 46× (controlled by R7/R1 feedback: 22kΩ/1kΩ + 1)
- **Components**:
  - **C2** (4.7µF): AC coupling capacitor, blocks DC from tape input
  - **D1, D2** (1N4148): Bidirectional clipping diodes for input protection
  - **R2** (100kΩ): Provides DC bias at amplifier non-inverting input
  - **R3** (2.2kΩ): Bias current path to ground
  - **R4** (100kΩ): Input bias resistor
  - **R5** (10kΩ): Sets input impedance and bias point
  - **C1** (2.2nF): High-frequency roll-off capacitor in feedback loop
  - **R7** (22kΩ): Feedback resistor setting gain
  - **R1** (1kΩ): Gain-setting resistor to ground

#### Stage 2: Output Buffering and Further Filtering (U2B - LM358)
- **Purpose**: Buffer the amplified signal and provide high-pass filtering to remove 50/60Hz power line hum
- **Configuration**: Voltage follower with high-pass filtering
- **Components**:
  - **C5** (10µF): Couples AC signal between stages
  - **D3** (1N4148): Output clipping protection
  - **R6** (1MΩ): Provides DC bias for second amplifier stage
  - **C7** (1µF): Decoupling capacitor creating high-pass filter
  - **R8** (47kΩ): Creates voltage divider with regulated 5V supply
  - **R11** (100kΩ): Sets input impedance for buffer stage
  - **C9** (1µF Film): Output coupling capacitor
  - **RV1** (10kΩ pot): Output volume control

#### Stage 3: Dual Comparators for Signal Quality Detection (U3 - LM393)
- **Purpose**: Monitor signal level and provide visual feedback
- **Configuration**: Dual threshold detector with adjustable limits
- **Recommended Threshold Settings**: Lower ~1.6V p-p, Upper ~2.0V p-p (at J2 output)
- **Comparator A** (U3A):
  - Monitors lower (minimum) signal threshold
  - **RV3** (10kΩ trimmer): Sets lower threshold level (adjust for ~1.6V p-p)
  - **R15** (22kΩ): Threshold reference resistor
  - Drives **D5** (Green LED) via **R9** (470Ω) current-limiting resistor
  - Indicates when signal exceeds lower threshold (adequate signal strength)
  
- **Comparator B** (U3B):
  - Monitors upper (maximum) signal threshold  
  - **RV2** (10kΩ trimmer): Sets upper threshold level (adjust for ~2.0V p-p)
  - **R13** (22kΩ): Threshold reference resistor
  - Drives **D4** (Red LED) via **R14** (470Ω) current-limiting resistor
  - Indicates when signal exceeds upper threshold (signal too strong)

#### Stage 4: Power Supply (U1 - L78L05)
- **Purpose**: Generate stable 5V reference for comparator thresholds
- **Configuration**: Linear voltage regulator
- **Input**: 6V from battery (BT1)
- **Output**: Regulated 5V @ 100mA max
- **Components**:
  - **C6** (0.1µF): Input decoupling capacitor
  - **C10** (0.1µF): Output decoupling capacitor
  - **R10** (10kΩ): Voltage divider for creating reference voltages
  - **R12** (15kΩ): Voltage divider for creating reference voltages

#### TS-to-TRRS Adapter (J3/J4 - Passive)
- **Purpose**: Convert TS1000 mono MIC output to TRRS for modern devices
- **Configuration**: Hardwired panel-mount jacks (not on PCB)
- **Wiring**:
  - J4 Tip (TS1000 signal) → J3 Sleeve (TRRS microphone input)
  - J4 Sleeve (ground) → J3 Ring2 and common ground
  - No active components - purely passive adapter

### Power Supply

- **Input**: 6V battery (4× AA or similar)
- **Switch**: Integrated into RV1 (switched potentiometer) - power on/off when turning volume knob
- **Decoupling**: 
  - **C3** (0.1µF): Decoupling for op-amp positive rail
  - **C4** (10µF): Bulk capacitance for op-amp positive rail
  - **C8** (0.1µF): Decoupling for comparator positive rail
- **Current Consumption**: Approximately 5-10mA typical (minimal battery drain)

### Key Features

#### Input Protection
- Bidirectional clipping diodes (D1, D2) protect the amplifier from excessive input signals
- Input coupling capacitor (C2) prevents DC offset damage

#### Signal Conditioning
- High gain first stage amplifies weak tape signals
- Multiple filtering stages remove unwanted frequencies
- DC biasing maintains proper operating points throughout the signal chain

#### Visual Feedback
- **Green LED (D5)**: Indicates signal is above minimum threshold (good signal level)
- **Red LED (D4)**: Indicates signal is above maximum threshold (excessive signal level)
- **Proper operation**: Only green LED should be lit during tape loading
- Adjustable thresholds allow optimization for different tape quality

#### Output
- Clean buffered output suitable for computer input
- Volume control (RV1) allows level adjustment
- Film capacitor (C9) in output path for low distortion

### Frequency Response

The circuit is optimized for the ZX81 tape format which uses:
- **Logic 0**: 4 cycles at ~1.5 kHz
- **Logic 1**: 9 cycles at ~3.0 kHz

The amplifier provides:
- High-pass filtering to remove 50/60Hz power line hum, subsonic noise, and rumble
- Low-pass filtering (via C1 in feedback loop) to reduce high-frequency noise
- Flat response in the 1-4 kHz range where the data signals exist

### Connectors

**Amplifier Circuit** (connected to PCB):
- **J1**: LOAD IN - 3.5mm audio jack (mono TS) from tape player or audio source
- **J2**: LOAD OUT - 3.5mm audio jack (mono TS) to TS1000 EAR input

**TS-to-TRRS Adapter** (panel-mounted in enclosure, not on PCB):
- **J4**: SAVE IN - 3.5mm audio jack (mono TS) from TS1000 MIC output
- **J3**: SAVE OUT - 3.5mm audio jack (TRRS) to phone/computer recording input
- Wiring: J4 Tip → J3 Sleeve, J3 Ring2 & J4 Sleeve → Ground

### Signal Flow

**LOAD Path** (Tape/Audio → TS1000):
```
Audio Source → J1 (LOAD IN) → [C2, D1/D2] → [U2A Amplifier] → [C5] → 
[U2B Buffer] → [C9, RV1] → J2 (LOAD OUT) → TS1000 EAR

                    ↓
              [U3A/U3B Comparators] → LEDs (D5/D4)
```

**SAVE Path** (TS1000 → Phone/Computer):
```
TS1000 MIC → J4 (SAVE IN - TS) → [Passive Adapter] → J3 (SAVE OUT - TRRS) → Recording Device

Adapter Wiring (inside enclosure):
  J4 Tip ──────────────→ J3 Sleeve (Mic Input)
  J4 Sleeve ─────┬────→ Ground
  J3 Ring2 ──────┘
```

### Component Notes

#### Critical Components
- **C9**: Specified as 1µF film capacitor for low distortion in audio path
- **D1, D2, D3**: 1N4148 fast-switching diodes for clean clipping
- **U2**: LM358 dual op-amp, rail-to-rail operation suitable for single supply
- **U3**: LM393 dual comparator with open-collector outputs
- **U1**: L78L05 5V 100mA regulator (TO-92 package)

#### Substitutions
- **U2**: Can substitute TL072, LM833, or other dual op-amps (note: may need supply voltage adjustment)
- **U3**: Can substitute LM339, TLC393
- **C9**: Must use film capacitor (polyester, polypropylene) for low distortion

### Design Considerations

#### Why This Topology?

1. **AC Coupling**: Tape signals are AC-only, DC blocking prevents offset issues
2. **High Input Impedance**: Won't load down tape player output
3. **Gain Distribution**: Two moderate-gain stages better than one high-gain stage
4. **Active Filtering**: Provides both filtering and gain simultaneously, including rejection of 50/60Hz power line hum
5. **Window Comparator**: Monitors both signal presence and amplitude for quality indication
6. **Regulated Reference**: Stable 5V supply ensures consistent threshold detection regardless of battery voltage
7. **Built-in Adapter**: Passive TS-to-TRRS adapter integrated into enclosure for convenience

#### Single Supply Operation

The circuit operates from a single 6V supply using:
- Virtual ground bias points (R2/R3, R4/R5, R6/C7)
- AC coupling between stages
- Rail-to-rail capable op-amps (LM358)
- Open-collector comparators (LM393)

### Historical Context

The ZX81/Timex Sinclair 1000 used Kansas City Standard encoding at 250 baud for tape storage. The circuit must handle:
- Frequency range: ~1-4 kHz
- Dynamic range: Variable depending on tape recorder quality
- Signal conditioning: Often needed due to aged tapes and recorder wear

This design improves upon simple direct-connection methods by providing:
- Proper impedance matching
- Clipping protection
- Visual feedback
- Adjustable sensitivity
- Bidirectional operation for both loading and saving

## Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for:

- Circuit improvements or alternative designs
- Enclosure variations
- PCB layout designs (if someone wants to create a proper PCB)
- Additional documentation or translations
- Build photos and construction tips
- Software tools for working with tape files

### Building Guidelines

If you build this project, please consider:
- Sharing photos of your build
- Documenting any modifications you make
- Reporting any issues you encounter
- Contributing improvements back to the repository

## License

This project is released under the MIT License. See LICENSE file for details.

**TL;DR**: You are free to use, modify, and distribute this design for any purpose, commercial or non-commercial, with attribution.

## Credits

**Design**: Jeremy Pastin (2025)

**Inspired by**: The vintage computing community and the need to preserve and use classic hardware with modern equipment.

## Acknowledgments

- The Sinclair ZX81 and Timex Sinclair 1000 communities for keeping these machines alive
- Modern retro computing enthusiasts who continue to develop tools and hardware for vintage systems
- ElectroCookie for providing excellent prototyping boards

## Version History

- **Rev 1** (2025-12-13): Initial design
  - Amplification and filtering circuit for loading programs
  - Dual comparator with LED indicators for signal quality feedback
  - Built-in passive TS-to-TRRS adapter for saving to modern devices
  - Designed for ElectroCookie solderable breadboard

---

**Questions?** Open an issue or contact through GitHub.

**Built one?** Please share your build photos and experience!
