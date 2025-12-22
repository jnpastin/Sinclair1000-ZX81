/*
 * menu.h - Serial Menu System for Z80 Validation
 * 
 * Hierarchical number-based menu interface for test control and configuration.
 * All static strings stored in PROGMEM to preserve RAM for Z80 memory emulation.
 */

#ifndef MENU_H
#define MENU_H

#include <Arduino.h>
#include <avr/pgmspace.h>
#include "clock.h"  // Include clock module for hardware control

// Configuration Structure (stored in RAM, reset on power cycle)
struct TestConfiguration {
  bool thoroughMode;      // false = Quick, true = Thorough
  uint32_t clockSpeed;    // Clock frequency in Hz
  bool verboseOutput;     // Detailed execution logs
  uint32_t randomSeed;    // Random seed (0 = auto)
};

// Global configuration
extern TestConfiguration g_config;

// Menu Display Functions
void displayMainMenu();
void displayTestIndividualMenu();
void displayInstructionGroupList(uint8_t groupNumber);
void displayTestGroupMenu();
void displayConfigurationMenu();
void displayViewResultsMenu();
void displayDiagnosticsMenu();
void displayClockControlMenu();
void displayHelpMenu();

// Menu Processing Functions
void processMainMenu();
void processTestIndividualMenu();
void processTestGroupMenu();
void processConfigurationMenu();
void processViewResultsMenu();
void processDiagnosticsMenu();
void processClockControlMenu();
void processHelpMenu();

// Input Functions
char getMenuChoice(char minOption, char maxOption);
uint16_t getNumber(uint16_t max);
uint32_t getFrequency();
bool confirmAction(const __FlashStringHelper* message);

// Utility Functions
void clearScreen();
void printHeader(const __FlashStringHelper* title);
void printSeparator();
void waitForKeyPress();
void printConfigStatus();
void printFrequency(uint32_t freq);
void printClockStatus();

// Display Helper Functions (TODO: Implement when test framework ready)
void displayInstructionDetails(uint16_t instructionIndex);
void displayTestResult(const char* instruction, bool passed);
void displayGroupSummary(uint8_t groupNumber, uint16_t passed, uint16_t failed);
void displayProgress(uint16_t current, uint16_t total, const char* itemName);

// Configuration Functions
void toggleTestMode();
void setClockSpeed();
void toggleVerbose();
void setRandomSeed();
void resetConfigToDefaults();

// Main menu entry point
void initializeMenu();
void runMenu();

#endif // MENU_H
