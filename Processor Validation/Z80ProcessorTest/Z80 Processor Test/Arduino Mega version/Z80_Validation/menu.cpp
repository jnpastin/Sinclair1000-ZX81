/*
 * menu.cpp - Serial Menu System Implementation
 * 
 * All static strings stored in PROGMEM using F() macro.
 */

// Conservative optimization for menu/debugging code
#pragma GCC optimize("O2")

#include "menu.h"
#include "hal.h"
#include "isr_test_programs.h"
#include "performance.h"

// Global configuration with defaults
TestConfiguration g_config = {
  false,      // thoroughMode (Quick mode default)
  500000,     // clockSpeed (500 kHz)
  false,      // verboseOutput (off)
  0           // randomSeed (auto)
};

// Currently selected instruction group (for test execution context)
uint8_t g_selectedGroup = 0;

// Instruction group names stored in PROGMEM
const char groupName1[] PROGMEM = "8-bit Load Instructions";
const char groupName2[] PROGMEM = "16-bit Load Instructions";
const char groupName3[] PROGMEM = "Exchange Instructions";
const char groupName4[] PROGMEM = "8-bit Arithmetic and Logical";
const char groupName5[] PROGMEM = "16-bit Arithmetic";
const char groupName6[] PROGMEM = "Rotate and Shift Instructions";
const char groupName7[] PROGMEM = "Bit Manipulation";
const char groupName8[] PROGMEM = "Jump Instructions";
const char groupName9[] PROGMEM = "Call and Return Instructions";
const char groupName10[] PROGMEM = "Input/Output Instructions";
const char groupName11[] PROGMEM = "Block Transfer and Search";
const char groupName12[] PROGMEM = "CPU Control Instructions";

const char* const groupNames[] PROGMEM = {
  groupName1, groupName2, groupName3, groupName4,
  groupName5, groupName6, groupName7, groupName8,
  groupName9, groupName10, groupName11, groupName12
};

// Utility Functions
void clearScreen() {
  // Simple approach: print blank lines instead of ANSI codes
  // Works with all terminal types
  for(uint8_t i = 0; i < 5; i++) {
    Serial.println();
  }
  
  // Alternative ANSI codes (commented out - enable if terminal supports it):
  // Serial.print(F("\033[2J\033[H"));
}

void printSeparator() {
  Serial.println(F("================================================================================"));
}

void printHeader(const __FlashStringHelper* title) {
  printSeparator();
  Serial.print(F("                    "));
  Serial.println(title);
  printSeparator();
}

void waitForKeyPress() {
  Serial.println();
  Serial.print(F("Press Enter to continue..."));
  while(!Serial.available());
  // Clear all pending input including CR/LF
  while(Serial.available()) {
    Serial.read();
  }
  Serial.println();
}

void printConfigStatus() {
  Serial.print(F("Current Mode: ["));
  Serial.print(g_config.thoroughMode ? F("THOROUGH") : F("QUICK"));
  Serial.print(F("]  |  Clock: ["));
  printFrequency(g_config.clockSpeed);
  Serial.print(F("]  |  Status: [READY]"));
  Serial.println();
}

// Helper function to print frequency in human-readable format
void printFrequency(uint32_t freq) {
  if(freq >= 1000000) {
    // Display in MHz
    uint16_t mhz = freq / 1000000;
    uint16_t fraction = (freq % 1000000) / 100000; // First decimal place
    Serial.print(mhz);
    if(fraction > 0) {
      Serial.print(F("."));
      Serial.print(fraction);
    }
    Serial.print(F(" MHz"));
  } else {
    // Display in kHz
    uint16_t khz = freq / 1000;
    Serial.print(khz);
    Serial.print(F(" kHz"));
  }
}

// Helper function to print clock status in human-readable format
void printClockStatus() {
  if(isClockRunning()) {
    Serial.print(F("Running at "));
    printFrequency(getClockFrequency());
  } else {
    Serial.print(F("Stopped at "));
    printFrequency(getClockFrequency());
  }
}

// Input Functions
char getMenuChoice(char minOption, char maxOption) {
  while(true) {
    if(Serial.available() > 0) {
      char c = Serial.read();
      
      // Ignore CR and LF
      if(c == '\n' || c == '\r') {
        continue;
      }
      
      Serial.print(c); // Echo
      
      if(c >= '0' + minOption && c <= '0' + maxOption) {
        Serial.println();
        // Clear any remaining CR/LF from the buffer
        delay(10); // Brief delay to let any trailing characters arrive
        while(Serial.available() > 0) {
          char discard = Serial.read();
          if(discard != '\n' && discard != '\r') {
            break; // Stop if we see something that's not a newline
          }
        }
        return c;
      } else {
        Serial.println(F(" - Invalid choice. Try again."));
        Serial.print(F("Enter selection ("));
        Serial.print(minOption);
        Serial.print(F("-"));
        Serial.print(maxOption);
        Serial.print(F("): "));
      }
    }
  }
}

uint16_t getNumber(uint16_t max) {
  char buffer[6]; // Up to 5 digits + null
  uint8_t pos = 0;
  bool hadCR = false;  // Track if we've seen CR (for CR+LF handling)
  
  while(true) {
    if(Serial.available() > 0) {
      char c = Serial.read();
      
      if(c == '\r') {
        // Carriage return - mark that we saw it
        hadCR = true;
        if(pos == 0) {
          // Enter on empty input = 0 (cancel)
          Serial.println(F("0"));
          return 0;
        }
        buffer[pos] = '\0';
        uint16_t num = atoi(buffer);
        if(num <= max) {
          Serial.println();
          return num;
        } else {
          Serial.print(F(" - Number too large (max "));
          Serial.print(max);
          Serial.println(F("). Try again: "));
          pos = 0;
          hadCR = false;
        }
      } else if(c == '\n') {
        // Line feed - only process if we haven't just seen CR
        if(!hadCR) {
          if(pos == 0) {
            // Enter on empty input = 0 (cancel)
            Serial.println(F("0"));
            return 0;
          }
          buffer[pos] = '\0';
          uint16_t num = atoi(buffer);
          if(num <= max) {
            Serial.println();
            return num;
          } else {
            Serial.print(F(" - Number too large (max "));
            Serial.print(max);
            Serial.println(F("). Try again: "));
            pos = 0;
          }
        }
        hadCR = false; // Reset CR flag after LF
      } else if(c >= '0' && c <= '9' && pos < 5) {
        hadCR = false; // Any other character resets CR flag
        buffer[pos++] = c;
        Serial.print(c); // Echo
      } else if(c == 8 || c == 127) { // Backspace or DEL
        hadCR = false;
        if(pos > 0) {
          pos--;
          Serial.print(F("\b \b")); // Erase character
        }
      } else {
        // Ignore other characters but reset CR flag
        if(c != '\n' && c != '\r') {
          hadCR = false;
        }
      }
    }
  }
}

uint32_t getFrequency() {
  Serial.println();
  Serial.println(F("Enter clock frequency in kHz (100-3000): "));
  uint16_t khz = getNumber(3000);
  if(khz < 100) khz = 100;
  if(khz > 3000) khz = 3000;
  return (uint32_t)khz * 1000;
}

bool confirmAction(const __FlashStringHelper* message) {
  Serial.println(message);
  Serial.println(F("Press 1 to CONFIRM, 0 to CANCEL: "));
  char c = getMenuChoice(0, 1);
  return (c == '1');
}

// Main Menu
void displayMainMenu() {
  clearScreen();
  printHeader(F("Z80 PROCESSOR VALIDATION SUITE v1.0"));
  printConfigStatus();
  Serial.println();
  Serial.println(F("MAIN MENU:"));
  Serial.println(F("  1. Test Individual Instruction [Framework Ready]"));
  Serial.println(F("  2. Test Instruction Group [Framework Ready]"));
  Serial.println(F("  3. Test All Instructions (Complete Validation) [Not Implemented]"));
  Serial.println(F("  4. Test Configuration [IMPLEMENTED]"));
  Serial.println(F("  5. View Results [Not Implemented]"));
  Serial.println(F("  6. System Diagnostics [Clock Control IMPLEMENTED]"));
  Serial.println(F("  7. Help [IMPLEMENTED]"));
  Serial.println(F("  0. Exit"));
  Serial.println();
  Serial.print(F("Enter selection (0-7): "));
}

void processMainMenu() {
  displayMainMenu();
  char choice = getMenuChoice(0, 7);
  
  switch(choice) {
    case '1':
      processTestIndividualMenu();
      break;
    case '2':
      processTestGroupMenu();
      break;
    case '3':
      Serial.println();
      Serial.println(F("==> Test All Instructions - Not Yet Implemented"));
      Serial.println(F("    This feature will run complete validation of all 698 instructions."));
      waitForKeyPress();
      break;
    case '4':
      processConfigurationMenu();
      break;
    case '5':
      processViewResultsMenu();
      break;
    case '6':
      processDiagnosticsMenu();
      break;
    case '7':
      processHelpMenu();
      break;
    case '0':
      Serial.println(F("\nExiting menu system..."));
      return;
  }
}

// Test Individual Instruction Menu
void displayTestIndividualMenu() {
  clearScreen();
  printHeader(F("TEST INDIVIDUAL INSTRUCTION > SELECT GROUP"));
  Serial.println();
  
  // Display all 12 groups
  for(uint8_t i = 0; i < 12; i++) {
    char buffer[40];
    strcpy_P(buffer, (char*)pgm_read_word(&(groupNames[i])));
    Serial.print(F("  "));
    if(i < 9) Serial.print(F(" ")); // Alignment for single digits
    Serial.print(i + 1);
    Serial.print(F(". "));
    Serial.println(buffer);
  }
  Serial.println(F("   0. Back to Main Menu"));
  Serial.println();
  Serial.print(F("Enter group number (0-12): "));
}

void displayInstructionGroupList(uint8_t groupNumber) {
  clearScreen();
  char buffer[40];
  strcpy_P(buffer, (char*)pgm_read_word(&(groupNames[groupNumber - 1])));
  
  Serial.print(F("TEST INDIVIDUAL INSTRUCTION > "));
  Serial.println(buffer);
  printSeparator();
  Serial.println();
  
  Serial.println(F("==> Instruction Database Not Yet Implemented"));
  Serial.println(F("    This will display all instructions in the selected group."));
  Serial.println(F("    You will be able to select an instruction by number to test it."));
  Serial.println();
  Serial.println(F("  0. Back to Group Menu"));
  Serial.println();
  Serial.print(F("Enter instruction number or 0 to go back: "));
}

void processTestIndividualMenu() {
  while(true) {
    displayTestIndividualMenu();
    uint16_t groupChoice = getNumber(12);
    
    if(groupChoice == 0) {
      return; // Back to main menu
    }
    
    g_selectedGroup = groupChoice;
    
    // Show instructions in selected group
    while(true) {
      displayInstructionGroupList(groupChoice);
      uint16_t instrChoice = getNumber(999); // Max instruction number placeholder
      
      if(instrChoice == 0) {
        break; // Back to group selection
      }
      
      Serial.println();
      Serial.println(F("==> Test Execution Not Yet Implemented"));
      Serial.println(F("    This will execute the selected instruction test."));
      waitForKeyPress();
    }
  }
}

// Test Instruction Group Menu
void displayTestGroupMenu() {
  clearScreen();
  printHeader(F("TEST INSTRUCTION GROUP"));
  Serial.println();
  
  for(uint8_t i = 0; i < 12; i++) {
    char buffer[40];
    strcpy_P(buffer, (char*)pgm_read_word(&(groupNames[i])));
    Serial.print(F("  "));
    if(i < 9) Serial.print(F(" ")); // Alignment
    Serial.print(i + 1);
    Serial.print(F(". "));
    Serial.println(buffer);
  }
  Serial.println(F("   0. Back to Main Menu"));
  Serial.println();
  Serial.print(F("Enter group number (0-12): "));
}

void processTestGroupMenu() {
  while(true) {
    displayTestGroupMenu();
    uint16_t choice = getNumber(12);
    
    if(choice == 0) {
      return; // Back to main menu
    }
    
    char buffer[40];
    strcpy_P(buffer, (char*)pgm_read_word(&(groupNames[choice - 1])));
    
    Serial.println();
    Serial.print(F("You selected: "));
    Serial.println(buffer);
    
    if(confirmAction(F("Run all tests in this group?"))) {
      Serial.println();
      Serial.println(F("==> Group Test Execution Not Yet Implemented"));
      Serial.println(F("    This will run all instruction tests in the selected group."));
      waitForKeyPress();
    }
  }
}

// Configuration Menu
void displayConfigurationMenu() {
  clearScreen();
  printHeader(F("CONFIGURATION > TEST SETTINGS"));
  Serial.println();
  
  Serial.print(F("  1. Test Mode: ["));
  Serial.print(g_config.thoroughMode ? F("THOROUGH") : F("QUICK"));
  Serial.println(F("] (Switch modes)"));
  
  Serial.print(F("  2. Clock Speed: ["));
  printFrequency(g_config.clockSpeed);
  Serial.println(F("] (Range: 100-3000 kHz)"));
  
  Serial.print(F("  3. Verbose Output: ["));
  Serial.print(g_config.verboseOutput ? F("ON") : F("OFF"));
  Serial.println(F("] (Toggle)"));
  
  Serial.print(F("  4. Random Seed: ["));
  if(g_config.randomSeed == 0) {
    Serial.print(F("AUTO"));
  } else {
    Serial.print(g_config.randomSeed);
  }
  Serial.println(F("] (Set seed)"));
  
  Serial.println(F("  5. Reset to Defaults"));
  Serial.println(F("  0. Back to Main Menu"));
  Serial.println();
  Serial.print(F("Enter selection (0-5): "));
}

void toggleTestMode() {
  g_config.thoroughMode = !g_config.thoroughMode;
  Serial.println();
  Serial.print(F("Test mode changed to: "));
  Serial.println(g_config.thoroughMode ? F("THOROUGH") : F("QUICK"));
  delay(1000);
}

void setClockSpeed() {
  Serial.println();
  uint32_t freq = getFrequency();
  
  // Validate and apply the frequency change
  if(freq >= CLOCK_FREQ_MIN && freq <= CLOCK_FREQ_MAX) {
    if(setClockFrequency(freq)) {
      g_config.clockSpeed = freq;
      Serial.print(F("Clock speed set to: "));
      printFrequency(freq);
      Serial.println();
    } else {
      Serial.println(F("ERROR: Failed to set clock frequency"));
    }
  } else {
    Serial.println(F("ERROR: Frequency out of range"));
  }
  delay(1000);
}

void toggleVerbose() {
  g_config.verboseOutput = !g_config.verboseOutput;
  Serial.println();
  Serial.print(F("Verbose output: "));
  Serial.println(g_config.verboseOutput ? F("ON") : F("OFF"));
  delay(1000);
}

void setRandomSeed() {
  Serial.println();
  Serial.println(F("Enter random seed (0 for auto, 1-65535): "));
  uint16_t seed = getNumber(65535);
  g_config.randomSeed = seed;
  Serial.print(F("Random seed set to: "));
  if(seed == 0) {
    Serial.println(F("AUTO"));
  } else {
    Serial.println(seed);
  }
  delay(1000);
}

void resetConfigToDefaults() {
  Serial.println();
  if(confirmAction(F("Reset all settings to defaults?"))) {
    g_config.thoroughMode = false;
    g_config.clockSpeed = 500000;
    g_config.verboseOutput = false;
    g_config.randomSeed = 0;
    Serial.println(F("Configuration reset to defaults."));
  } else {
    Serial.println(F("Reset cancelled."));
  }
  delay(1000);
}

void processConfigurationMenu() {
  while(true) {
    displayConfigurationMenu();
    char choice = getMenuChoice(0, 5);
    
    switch(choice) {
      case '1': toggleTestMode(); break;
      case '2': setClockSpeed(); break;
      case '3': toggleVerbose(); break;
      case '4': setRandomSeed(); break;
      case '5': resetConfigToDefaults(); break;
      case '0': return; // Back to main menu
    }
  }
}

// View Results Menu
void displayViewResultsMenu() {
  clearScreen();
  printHeader(F("VIEW RESULTS"));
  Serial.println();
  Serial.println(F("  1. Last Test Result [Not Implemented]"));
  Serial.println(F("  2. Last Group Summary [Not Implemented]"));
  Serial.println(F("  3. Full Test Report (All Instructions) [Not Implemented]"));
  Serial.println(F("  4. Test Statistics (Pass/Fail counts) [Not Implemented]"));
  Serial.println(F("  5. Failed Tests Only [Not Implemented]"));
  Serial.println(F("  6. Export Results (Serial dump) [Not Implemented]"));
  Serial.println(F("  0. Back to Main Menu"));
  Serial.println();
  Serial.print(F("Enter selection (0-6): "));
}

void processViewResultsMenu() {
  while(true) {
    displayViewResultsMenu();
    char choice = getMenuChoice(0, 6);
    
    if(choice == '0') return;
    
    Serial.println();
    Serial.println(F("==> Results Storage and Display Not Yet Implemented"));
    Serial.println(F("    This will show test results once testing functions are complete."));
    waitForKeyPress();
  }
}

// System Diagnostics Menu
void displayDiagnosticsMenu() {
  clearScreen();
  printHeader(F("SYSTEM DIAGNOSTICS"));
  Serial.println();
  Serial.println(F("  1. Hardware Status [IMPLEMENTED]"));
  Serial.println(F("  2. Run HAL Hardware Test [IMPLEMENTED]"));
  Serial.println(F("  3. Test Memory ISR [IMPLEMENTED]"));
  Serial.println(F("  4. Clock Control [IMPLEMENTED]"));
  Serial.println(F("  5. Pin Mapping Reference [Not Implemented]"));
  Serial.println(F("  6. Performance Tests [IMPLEMENTED]"));
  Serial.println(F("  7. Cache Size Testing [IMPLEMENTED]"));
  Serial.println(F("  0. Back to Main Menu"));
  Serial.println();
  Serial.print(F("Enter selection (0-7): "));
}

void displayHardwareStatus() {
  clearScreen();
  printHeader(F("HARDWARE STATUS"));
  Serial.println();
  
  // Clock Status
  Serial.println(F("CLOCK STATUS:"));
  Serial.print(F("  Current Frequency: "));
  printFrequency(getClockFrequency());
  Serial.println();
  Serial.print(F("  Clock Running: "));
  Serial.println(isClockRunning() ? F("YES") : F("NO"));
  Serial.println();
  
  // ISR Status
  Serial.println(F("ISR STATUS (Separate RD/WR ISRs):"));
  noInterrupts();
  uint32_t readCount = g_readIsrCount;
  uint32_t writeCount = g_writeIsrCount;
  uint16_t lastReadAddr = g_lastReadAddress;
  uint16_t lastWriteAddr = g_lastWriteAddress;
  uint32_t errorCount = g_isrErrorCount;
  uint32_t triggerCount = g_isrTriggerCount;
  interrupts();
  
  Serial.print(F("  ISR Triggers:    "));
  Serial.println(triggerCount);
  Serial.print(F("  Read ISR Calls:  "));
  Serial.println(readCount);
  Serial.print(F("  Write ISR Calls: "));
  Serial.println(writeCount);
  Serial.print(F("  Last Read Addr:  0x"));
  Serial.println(lastReadAddr, HEX);
  Serial.print(F("  Last Write Addr: 0x"));
  Serial.println(lastWriteAddr, HEX);
  Serial.print(F("  ISR Errors:      "));
  Serial.println(errorCount);
  Serial.println();
  
  // Memory Handler Status
  Serial.println(F("MEMORY HANDLER:"));
  Serial.print(F("  Initialized:     "));
  Serial.println(F("YES (INT5/INT4 enabled)"));
  Serial.print(F("  Program Base:    0x"));
  Serial.println(g_programBaseAddress, HEX);
  Serial.print(F("  Base Valid:      "));
  Serial.println(g_programBaseAddressValid ? F("YES") : F("NO"));
  Serial.println();
  
  // Z80 Status
  Serial.println(F("Z80 STATUS:"));
  Serial.print(F("  HALT Signal:     "));
  Serial.println(digitalRead(Z80_HALT) ? F("HIGH (running)") : F("LOW (halted)"));
  Serial.print(F("  WAIT Asserted:   "));
  Serial.println(g_waitAsserted ? F("YES") : F("NO"));
  Serial.println();
  
  Serial.println(F("Note: ISR counters reset on each test. Use diagnostics for live monitoring."));
}

void displayClockControlMenu() {
  clearScreen();
  printHeader(F("DIAGNOSTICS > CLOCK CONTROL"));
  
  // Display current clock status
  Serial.print(F("Current Status: "));
  printClockStatus();
  Serial.println();
  Serial.println();
  
  Serial.println(F("  1. View Detailed Clock Status"));
  Serial.println(F("  2. Start Clock"));
  Serial.println(F("  3. Stop Clock"));
  Serial.println(F("  4. Single Step (clock must be stopped)"));
  Serial.println(F("  5. Change Frequency"));
  Serial.println(F("  6. Test Clock Output (manual 1Hz toggle)"));
  Serial.println(F("  7. Preset: 100 KHz"));
  Serial.println(F("  8. Preset: 500 KHz"));
  Serial.println(F("  9. Preset: 1 MHz"));
  Serial.println(F("  0. Back to Diagnostics Menu"));
  Serial.println();
  Serial.print(F("Enter selection (0-9): "));
}

void processClockControlMenu() {
  while(true) {
    displayClockControlMenu();
    char choice = getMenuChoice(0, 9);
    
    Serial.println();
    
    switch(choice) {
      case '1': // View Detailed Clock Status
        {
          clearScreen();
          printHeader(F("CLOCK STATUS"));
          Serial.println();
          
          Serial.print(F("State: "));
          Serial.println(isClockRunning() ? F("RUNNING") : F("STOPPED"));
          
          Serial.print(F("Frequency: "));
          printFrequency(getClockFrequency());
          Serial.println();
          
          Serial.print(F("Frequency (Hz): "));
          Serial.print(getClockFrequency());
          Serial.println(F(" Hz"));
          
          Serial.print(F("Output Pin: "));
          Serial.println(F("11 (PB5/OC1A)"));
          
          Serial.print(F("Timer: "));
          Serial.println(F("Timer1 (Hardware PWM)"));
          
          Serial.print(F("Duty Cycle: "));
          Serial.println(F("50%"));
          
          Serial.println();
          Serial.println(F("Supported Range: 100 kHz - 3 MHz"));
          
          waitForKeyPress();
        }
        break;
        
      case '2': // Start Clock
        startClock();
        Serial.println(F("Clock started"));
        Serial.print(F("Status: "));
        printClockStatus();
        Serial.println();
        delay(1500);
        break;
        
      case '3': // Stop Clock
        stopClock();
        Serial.println(F("Clock stopped"));
        Serial.print(F("Status: "));
        printClockStatus();
        Serial.println();
        delay(1500);
        break;
        
      case '4': // Single Step
        if(singleStepClock()) {
          Serial.println(F("Single clock pulse generated"));
        } else {
          Serial.println(F("ERROR: Clock must be stopped for single-step mode"));
          Serial.println(F("Use option 3 to stop the clock first."));
        }
        delay(1500);
        break;
        
      case '5': // Change Frequency
        Serial.println();
        Serial.println(F("Enter frequency in kHz (100-3000): "));
        {
          uint32_t khz = getNumber(3000);
          if(khz >= 100 && khz <= 3000) {
            uint32_t freq = khz * 1000UL;
            if(setClockFrequency(freq)) {
              Serial.println(F("Frequency updated successfully"));
              Serial.print(F("New status: "));
              printClockStatus();
              Serial.println();
              // Also update config
              g_config.clockSpeed = freq;
            } else {
              Serial.println(F("ERROR: Failed to set frequency"));
            }
          } else {
            Serial.println(F("Frequency out of range (100-3000 kHz)"));
          }
        }
        delay(1500);
        break;
        
      case '6': // Test Clock Output
        Serial.println(F("Starting manual clock test (1 Hz for 10 seconds)"));
        Serial.println(F("Press any key to stop early..."));
        delay(1000);
        testClockOutput();
        Serial.println(F("Clock test complete. Returning to menu..."));
        delay(1500);
        break;
        
      case '7': // 100 KHz
        if(setClockFrequency(CLOCK_FREQ_100KHZ)) {
          Serial.println(F("Clock set to 100 KHz"));
          g_config.clockSpeed = CLOCK_FREQ_100KHZ;
        }
        delay(1500);
        break;
        
      case '8': // 500 KHz
        if(setClockFrequency(CLOCK_FREQ_500KHZ)) {
          Serial.println(F("Clock set to 500 KHz"));
          g_config.clockSpeed = CLOCK_FREQ_500KHZ;
        }
        delay(1500);
        break;
        
      case '9': // 1 MHz
        if(setClockFrequency(CLOCK_FREQ_1MHZ)) {
          Serial.println(F("Clock set to 1 MHz"));
          g_config.clockSpeed = CLOCK_FREQ_1MHZ;
        }
        delay(1500);
        break;
        
      case '0': // Back
        return;
    }
  }
}

void processDiagnosticsMenu() {
  while(true) {
    displayDiagnosticsMenu();
    char choice = getMenuChoice(0, 7);
    
    switch(choice) {
      case '1':
        Serial.println();
        displayHardwareStatus();
        waitForKeyPress();
        break;
      case '2':
        Serial.println();
        runHALTest();
        waitForKeyPress();
        break;
      case '3':
        Serial.println();
        runISRTest();
        waitForKeyPress();
        break;
      case '4':
        processClockControlMenu();
        break;
      case '5':
        Serial.println();
        Serial.println(F("==> Pin Mapping Reference Not Yet Implemented"));
        Serial.println(F("    This will display the Arduino-to-Z80 pin connections."));
        waitForKeyPress();
        break;
      case '6':
        Serial.println();
        runPerformanceBenchmark();
        waitForKeyPress();
        break;
      case '7':
        Serial.println();
        findOptimalCacheSize();
        waitForKeyPress();
        break;
      case '0':
        return;
    }
  }
}

// Help Menu
void displayHelpMenu() {
  clearScreen();
  printHeader(F("HELP"));
  Serial.println();
  Serial.println(F("  1. Quick Start Guide"));
  Serial.println(F("  2. Menu Navigation Help"));
  Serial.println(F("  3. Test Mode Explanations"));
  Serial.println(F("  4. Instruction Groups Reference"));
  Serial.println(F("  5. Troubleshooting Guide"));
  Serial.println(F("  6. Z80 Instruction Quick Reference"));
  Serial.println(F("  0. Back to Main Menu"));
  Serial.println();
  Serial.print(F("Enter selection (0-6): "));
}

void processHelpMenu() {
  while(true) {
    displayHelpMenu();
    char choice = getMenuChoice(0, 6);
    
    if(choice == '0') return;
    
    clearScreen();
    switch(choice) {
      case '1':
        printHeader(F("QUICK START GUIDE"));
        Serial.println();
        Serial.println(F("1. Select 'Test Individual Instruction' to test a single instruction"));
        Serial.println(F("2. Select 'Test Instruction Group' to test all instructions in a category"));
        Serial.println(F("3. Use 'Test Configuration' to switch between Quick and Thorough modes"));
        Serial.println(F("4. Quick Mode: Single random test per instruction (fast)"));
        Serial.println(F("5. Thorough Mode: All input combinations tested (comprehensive)"));
        Serial.println(F("6. View results after testing to see pass/fail status"));
        break;
      case '2':
        printHeader(F("MENU NAVIGATION"));
        Serial.println();
        Serial.println(F("- Enter numbers to make selections"));
        Serial.println(F("- Press 0 to go back to previous menu"));
        Serial.println(F("- Multi-digit numbers: Type digits and press Enter"));
        Serial.println(F("- Use Backspace to correct input errors"));
        break;
      case '3':
        printHeader(F("TEST MODES"));
        Serial.println();
        Serial.println(F("QUICK MODE:"));
        Serial.println(F("  - Tests each instruction with one random set of inputs"));
        Serial.println(F("  - Fast execution (seconds per group)"));
        Serial.println(F("  - Good for initial validation and debugging"));
        Serial.println();
        Serial.println(F("THOROUGH MODE:"));
        Serial.println(F("  - Tests all possible input combinations"));
        Serial.println(F("  - Comprehensive coverage (minutes to hours)"));
        Serial.println(F("  - Required for complete validation"));
        break;
      case '4':
        printHeader(F("INSTRUCTION GROUPS"));
        Serial.println();
        for(uint8_t i = 0; i < 12; i++) {
          char buffer[40];
          strcpy_P(buffer, (char*)pgm_read_word(&(groupNames[i])));
          Serial.print(i + 1);
          Serial.print(F(". "));
          Serial.println(buffer);
        }
        break;
      case '5':
        printHeader(F("TROUBLESHOOTING"));
        Serial.println();
        Serial.println(F("If tests fail:"));
        Serial.println(F("1. Check all Z80 connections"));
        Serial.println(F("2. Verify power supply is stable at 5V"));
        Serial.println(F("3. Run Hardware Self-Test in Diagnostics menu"));
        Serial.println(F("4. Try reducing clock frequency (use Clock Control)"));
        Serial.println(F("5. Check for shorts or open connections"));
        Serial.println(F("6. Use Clock Control > Single Step for debugging"));
        Serial.println(F("7. Use Clock Control > Test Output to verify clock signal"));
        break;
      case '6':
        printHeader(F("Z80 QUICK REFERENCE"));
        Serial.println();
        Serial.println(F("The Z80 has 698 documented instruction variations"));
        Serial.println(F("Organized into 12 functional groups"));
        Serial.println(F("See Instruction Groups Reference for categories"));
        break;
    }
    waitForKeyPress();
  }
}

// Display Helper Functions
// TODO: displayProgress() will be used by test execution framework - not yet called
void displayProgress(uint16_t current, uint16_t total, const char* itemName) {
  static unsigned long lastUpdate = 0;
  unsigned long now = millis();
  
  // Throttle updates to every 1 second
  if(now - lastUpdate >= 1000) {
    lastUpdate = now;
    
    uint8_t percent = (current * 100) / total;
    
    Serial.print(F("\rProgress: ["));
    
    // Progress bar (20 chars)
    uint8_t filled = (current * 20) / total;
    for(uint8_t i = 0; i < 20; i++) {
      Serial.print(i < filled ? F("#") : F("."));
    }
    
    Serial.print(F("] "));
    Serial.print(percent);
    Serial.print(F("% ("));
    Serial.print(current);
    Serial.print(F("/"));
    Serial.print(total);
    Serial.print(F(") - "));
    Serial.print(itemName);
    Serial.print(F("   ")); // Clear any trailing chars
  }
}

// Initialization and Main Loop
void initializeMenu() {
  // Initialize serial if not already done
  if(!Serial) {
    Serial.begin(115200);
    while(!Serial); // Wait for serial port
  }
  
  // Sync config with actual clock state
  g_config.clockSpeed = getClockFrequency();
  
  Serial.println();
  Serial.println(F("Z80 Validation Menu System Initialized"));
  Serial.print(F("Clock: "));
  printClockStatus();
  Serial.println();
  delay(1000);
}

void runMenu() {
  while(true) {
    processMainMenu();
  }
}
