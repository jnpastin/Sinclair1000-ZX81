# Connector Change Summary

**Date:** December 24, 2025  
**Change:** JST XH Connectors → Standard 0.1" Pin Headers

---

## Reason for Change

JST XH connectors (2.5mm pitch) are physically too large to fit adjacent to each other on standard 2.54mm (0.1") pitch proto board. The connector housings require spacing that wastes board holes between each connector.

**Solution:** Use standard 0.1" (2.54mm) pin headers with Dupont connectors instead.

---

## Impact Summary

### Advantages of Pin Headers
✅ **Space efficient:** 2.54mm pitch matches proto board perfectly  
✅ **No wasted holes:** Headers can be placed side-by-side  
✅ **Easier sourcing:** Standard component, readily available  
✅ **Lower cost:** Significantly cheaper than JST XH  
✅ **Flexible:** Can use individual pins or grouped housings  
✅ **Better for prototyping:** Easy to rework and debug  
✅ **Standard Dupont wires:** Pre-made jumpers widely available  

### Disadvantages
⚠️ **Less secure:** Dupont connectors can pull out more easily  
⚠️ **No keying:** Must label carefully to prevent mis-insertion  
⚠️ **Wire management:** Individual wires need more organization  

### Mitigation
- Use shrink tubing or cable ties to bundle wires by function
- Color-code wires by signal type
- Label both ends of every connection
- Use right-angle headers where beneficial
- Consider adding strain relief

---

## Connector Replacement Mapping

### Old Design (JST XH)
- 5× 12-pin JST XH receptacles (Teensy board)
- 10× 8-pin JST XH receptacles (Level shifter A-side)
- 10× 4-pin JST XH receptacles (Level shifter grounds)
- 1× 6-pin JST XH (power)
- 1× 2-pin JST XH (OE control)
- Matching plug housings for all cables

### New Design (Pin Headers)
- 5× 12-pin headers (Teensy → Level shifter modules)
- 5× 12-pin headers (Level shifter → Z80 board)
- 5× 12-pin headers (Level shifter module mounting)
- 1× 6-pin header (power distribution)
- Individual pins or small headers for control signals
- Standard Dupont jumper wires (pre-made or crimped)

---

## Wire/Cable Options

### Option 1: Pre-made Dupont Jumpers (Easiest)
- Buy assorted female-to-female Dupont jumper wire kit
- Lengths: 10cm, 20cm, 30cm mix
- Group and label wires by function
- Use cable ties to bundle

**Pros:** No crimping needed, fast assembly  
**Cons:** May have excess wire length to manage

### Option 2: Custom Ribbon Cable (Best Signal Integrity)
- 10-conductor ribbon cable per module
- IDC Dupont connectors on ribbon cable
- Clean, organized routing

**Pros:** Professional appearance, good signal integrity  
**Cons:** Requires IDC crimping tool, less flexible

### Option 3: Hand-Crimped Dupont (Most Flexible)
- Individual wires cut to exact length
- Crimp Dupont pins on each end
- Insert into connector housings

**Pros:** Perfect lengths, maximum flexibility  
**Cons:** Time-consuming, requires crimping tool and skill

---

## Pin Header Specifications

**Type:** 0.1" (2.54mm) pitch, single-row, male pins  
**Mounting:** Through-hole, straight or right-angle  
**Contact:** Gold-plated preferred for reliability  
**Pitch:** Standard 2.54mm (0.1")  

**Recommended Products:**
- Sullins Connector Solutions (Mouser/DigiKey)
- Harwin M20 series
- Generic 40-pin breakaway headers (cut to size)

**Mating Connector:** Standard Dupont 2.54mm crimp housing  
**Wire Gauge:** 24-26 AWG for signals, 22 AWG for power  

---

## Updated Bill of Materials

### Headers Required
**Teensy Board:**
- 60 pins total (can use 2× 40-pin breakaway strips)

**Level Shifter Board:**
- 66 pins total (can use 2× 40-pin breakaway strips)

**Z80 Board:**
- 60 pins total (can use 2× 40-pin breakaway strips)

**Total: ~200 pins** or **5× 40-pin breakaway header strips**

### Dupont Wires/Connectors
**Option 1 (Pre-made):**
- 1× 120-piece Dupont jumper wire kit (mixed lengths)

**Option 2/3 (DIY):**
- Dupont connector housings (2-12 pin, assorted)
- 200× Dupont crimp terminals (24-26 AWG)
- Dupont crimping tool
- 22-26 AWG stranded wire

---

## Documentation Files Requiring Updates

The following files contain JST XH references that need updating:

1. ✅ BOM.md - PARTIALLY UPDATED
2. ⏳ Assembly_and_Testing.md
3. ⏳ Level_Shifting.md
4. ⏳ Phase1_Summary.md
5. ⏳ Physical_Layout.md
6. ⏳ Quick_Reference.md
7. ⏳ README.md
8. ⏳ Teensy_Board_Layout.md
9. ⏳ Wiring_Diagram.md
10. ⏳ Z80_Board_Layout.md

---

## Assembly Process Changes

### Old Process (JST XH)
1. Install JST XH receptacles on boards
2. Crimp JST XH terminals on wires
3. Insert terminals into JST plug housings
4. Plug housings into receptacles

### New Process (Pin Headers)
1. Install pin headers on boards (straight or right-angle)
2. Option A: Use pre-made Dupont jumpers (fastest)
3. Option B: Crimp Dupont terminals and assemble housings
4. Label and bundle wires by function
5. Connect Dupont housings to pin headers

---

## Testing Recommendations

With Dupont connectors being less secure than JST:

1. **Initial Assembly:**
   - Test continuity of every connection
   - Verify no shorts between adjacent pins
   - Check all connections are fully seated

2. **During Operation:**
   - Periodically check for loose connections
   - Add strain relief where cables are stressed
   - Use cable ties to prevent accidental disconnection

3. **Debugging:**
   - Individual pins can be probed easily
   - Can disconnect/reconnect single signals
   - Much easier to rewire if needed

---

## Future PCB Design Considerations

If/when moving to a custom PCB design:
- Can use either JST XH or pin headers
- PCB footprint allows proper JST XH spacing
- Or retain pin headers for flexibility
- Consider IDC box headers for ribbon cables
- Add mounting holes for strain relief

---

## Status

**Current:** Documentation update in progress  
**Next Steps:** Update all referenced documentation files  
**Testing:** Will verify during physical assembly  

