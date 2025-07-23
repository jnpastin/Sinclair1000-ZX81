// --- Test Functions (Stubs) ---
void runAllTests() {
  Serial.println(F("Running all tests..."));
  // ... your test logic here
}

void runAllTestsForGroup(InstructionGroup group) {
  Serial.print(F("Running all tests for group: "));
  const char* groupNamePtr;
  memcpy_P(&groupNamePtr, &InstructionGroupNames[group], sizeof(groupNamePtr));
  printProgmemString(groupNamePtr);
  Serial.println();
  // ... your test logic here
}

void runTest(const instructionDefinitionType& inst) {
  Serial.print(F("Running test for instruction: "));
  if (inst.mnemonic) {
    char buffer[24];
    strcpy_P(buffer, inst.mnemonic);
    Serial.print(buffer);
  }
  Serial.println();
  // ... your test logic here
}


bool NOP() {
  //Mnemonic:  NOP
  //Assembled command: 0x00
  //Expected behavior: CPU performs no operation, PC advances
  //Test methodology: Run a NOP until all 16 bits of the address space have been cycled through
  //Success criteria: Verify that the address bus reads one higher than prior to the execution of the instruction

  //Initialize some data
  bool error = false;
  byte data[] = { 0x0 };
  uint16_t curAddr = 0;
  int delayTime = 5;

  Serial.print(F("Testing NOP "));
  for (unsigned int i = 0; i < 65535; i++) {

    //This instruction only takes a single M1 cycle
    writeSingleInstruction(data, 1, 0, 0);

    //Wait for the refresh cycle to end
    while (!digitalReadFast(RFSH)) {
      delayMicroseconds(0.1);
    }

    //Read the address bus, delaying long enough for the address bus to settl out after the refresh
    curAddr = readAddressBus(delayTime);

    //Test if the address bus reads the expected value
    if (curAddr != i + 1) {
      error = true;
    }

    //Display progress
    if ((curAddr % 1000) == 0) {
      Serial.print(F("."));
    }
  }

  //Display status
  if (error) {
    Serial.println(F("Fail"));
  } else {
    Serial.println(F("Pass"));
  }
}