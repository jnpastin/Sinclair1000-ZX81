//Read a single line in from the serial port
void readLine(char* buffer, size_t bufferSize) {
  size_t idx = 0;
  while (true) {
    while (Serial.available() == 0) {
      delay(1);
    }
    char c = Serial.read();
    if (c == '\n' || c == '\r') {
      if (idx > 0) break;  // End on first newline after input
    } else {
      if (idx < bufferSize - 1) {
        buffer[idx++] = c;
      }
    }
  }
  buffer[idx] = '\0';  // Null-terminate
}

//Print PROGMEM string by pointer
void printProgmemString(const char* progmemPtr) {
  char buffer[30];
  strcpy_P(buffer, progmemPtr);
  Serial.print(buffer);
}

//Display the main menu
void showMainMenu() {
  while (true) {
    Serial.println(F("Z80 CPU Test Suite"));
    Serial.println();
    Serial.println(F("Please make a selection:"));
    Serial.println(F("1) Run all tests"));
    Serial.println(F("2) Run all tests for a specific group"));
    Serial.println(F("3) Run a specific test"));
    Serial.println(F("4) Exit"));

    char selectionBuffer[8];
    readLine(selectionBuffer, sizeof(selectionBuffer));
    int menuSelection = atoi(selectionBuffer);
    //char menuSelection = waitForSelection();

    switch (menuSelection) {
      case 1:
        runAllTests();
        break;
      case 2:
        showGroupMenu(runAllTestsForGroup);
        break;
      case 3:
        showGroupMenu([](InstructionGroup group) {
          showInstructionMenu(group);
        });
        break;
      case 4:
        Serial.println(F("Exiting..."));
        while (1) delay(1000);  // Halt
        break;
      default:
        Serial.println(F("Invalid selection. Try again."));
    }
    Serial.println();
  }
}

//Display the group menu
void showGroupMenu(void (*callback)(InstructionGroup)) {
  Serial.println(F("Select a group:"));
  for (int i = 0; i < GROUP_COUNT; ++i) {
    Serial.print(i + 1);
    Serial.print(F(") "));
    // Correct way: retrieve pointer from PROGMEM using memcpy_P!
    const char* groupNamePtr;
    memcpy_P(&groupNamePtr, &InstructionGroupNames[i], sizeof(groupNamePtr));
    printProgmemString(groupNamePtr);
    Serial.println();
  }
  char selectionBuffer[8];
  readLine(selectionBuffer, sizeof(selectionBuffer));
  int groupSelection = atoi(selectionBuffer) - 1;
  if (groupSelection >= 0 && groupSelection < GROUP_COUNT) {
    callback((InstructionGroup)groupSelection);
  } else {
    Serial.println(F("Invalid group selection."));
  }
}

// Limit max instructions per group to avoid large arrays on stack
#define MAX_INSTRUCTIONS_PER_GROUP 240
//Display the functions menu
void showInstructionMenu(InstructionGroup group) {
  Serial.print(F("Select an instruction in "));
  const char* groupNamePtr;
  memcpy_P(&groupNamePtr, &InstructionGroupNames[group], sizeof(groupNamePtr));
  printProgmemString(groupNamePtr);
  Serial.println();
  int count = 0;
  int indices[MAX_INSTRUCTIONS_PER_GROUP];
  for (size_t i = 0; i < numInstructions; ++i) {
    instructionDefinitionType inst;
    memcpy_P(&inst, &InstructionDefinitions[i], sizeof(inst));
    if (inst.group == group) {
      if (count < MAX_INSTRUCTIONS_PER_GROUP) {
        Serial.print(count + 1);
        Serial.print(F(") "));
        if (inst.mnemonic) {
          char buffer[24];
          strcpy_P(buffer, inst.mnemonic);
          Serial.print(buffer);
        }
        Serial.println();
        indices[count] = i;
        count++;
      }
    }
  }
  if (count == 0) {
    Serial.println(F("No instructions in this group."));
    return;
  }
  char selectionBuffer[8];
  readLine(selectionBuffer, sizeof(selectionBuffer));
  int instSelection = atoi(selectionBuffer) - 1;
  if (instSelection >= 0 && instSelection < count) {
    instructionDefinitionType inst;
    memcpy_P(&inst, &InstructionDefinitions[indices[instSelection]], sizeof(inst));
    runTest(inst);
  } else {
    Serial.println(F("Invalid instruction selection."));
  }
}

int findInstructionIndexByMnemonic(const char* targetMnemonic) {
    char buffer[32];
    for (int i = 0; i < numInstructions; ++i) {
        instructionDefinitionType inst;
        memcpy_P(&inst, &InstructionDefinitions[i], sizeof(inst));
        strcpy_P(buffer, inst.mnemonic); // copy mnemonic from PROGMEM
        if (strcmp(buffer, targetMnemonic) == 0) {
            return i; // Found!
        }
    }
    return -1; // Not found
}

void runAllTests() {
  Serial.println(F("Running all tests..."));

  for (size_t i = 0; i < numInstructions; ++i) {
    instructionDefinitionType inst;
    memcpy_P(&inst, &InstructionDefinitions[i], sizeof(inst));
    runTest(inst);
  }
}

void runAllTestsForGroup(InstructionGroup group) {
  Serial.print(F("Running all tests for group: "));
  const char* groupNamePtr;
  memcpy_P(&groupNamePtr, &InstructionGroupNames[group], sizeof(groupNamePtr));
  printProgmemString(groupNamePtr);
  Serial.println();

  for (size_t i = 0; i < numInstructions; ++i) {
    instructionDefinitionType inst;
    memcpy_P(&inst, &InstructionDefinitions[i], sizeof(inst));
    if (inst.group == group && inst.testFunction) {
      runTest(inst); // Call your runTest function for this instruction
    }
  }
  
}

void runTest(const instructionDefinitionType& inst) {
  Serial.print(F("Running test for instruction: "));
  char buffer[24];
  if (inst.mnemonic) {
    strcpy_P(buffer, inst.mnemonic);
    Serial.print(buffer);
  }
  
  if (inst.testFunction) {
    bool result = inst.testFunction();
  }
}
