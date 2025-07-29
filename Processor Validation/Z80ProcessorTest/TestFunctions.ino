bool NOP() {
  //Mnemonic:  NOP
  //Assembled command: 0x00
  //Expected behavior: CPU performs no operation, PC advances
  //Test methodology: Run a NOP until all 16 bits of the address space have been cycled through
  //Success criteria: Verify that the address bus reads one higher than prior to the execution of the instruction

  //Initialize some variables
  bool error = false;
  byte data[] = { 0x0 };
  uint16_t prevAddr = 0;
  uint16_t curAddr = 0;
  uint16_t addr[]={0x00};
  char targetMnemonic[]="NOP";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  char buffer[24];

  //Get the instruction information
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));

  strcpy_P(buffer, inst.mnemonic);
  Serial.print(F("Testing "));
  Serial.print(buffer);

  //Test enough times to ensure that we have hit every addressable value
  for (unsigned int i = 0; i < 65535; i++) {

    //Read the current address on the bus
    prevAddr = readAddressBus(0);

    //This instruction only takes a single M1 cycle
    writeSingleInstruction(data, 1, 0, 0, addr);

    //Wait for the refresh cycle to end
    while (!digitalReadFast(RFSH)) {
      delayMicroseconds(0.1);
    }

    //Read the address bus, delaying long enough for the address bus to settle out after the refresh
    curAddr = readAddressBus(period/2);

    //Test if the address bus reads the expected value
    if (curAddr != prevAddr + 1) {
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
bool LD_A_N() {
  //Mnemonic:  LD A,n
  //Assembled command: 0x3E
  //Expected behavior: A value is loaded into Register A
  //Test methodology: Load all possible 8 bit values into Register A, retrieve the value of Register A to verify.  NOTE: This tests `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte data[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD A,n"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD A,n command
    helper_ld_X_n('A', i);

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      data[j]=0x0;
    }
    helper_ld_ptr_nn_a(data, addr);

   
    //Test if the Data Bus reads the expected value
    if (data[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_B_N() {
  //Mnemonic:  LD B,n
  //Assembled command: 0x06
  //Expected behavior: A value is loaded into Register B
  //Test methodology: Load all possible 8 bit values into Register B, load Register A with the contents of Register B, retrieve the value of Register A to verify.  NOTE: This tests `LD A,B` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD B,n"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD B,n command
    helper_ld_X_n('B', i);

    //Send the LD A,B command to move the data into the A register
    helper_ld_X_Y('A','B');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
      Serial.print(F("Expected: "));
      Serial.print(i);
      Serial.print(F(" got: "));
      Serial.println(outputData[2]);
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_C_N() {
  //Mnemonic:  LD C,n
  //Assembled command: 0x0E
  //Expected behavior: A value is loaded into Register C
  //Test methodology: Load all possible 8 bit values into Register C, load Register A with the contents of Register C, retrieve the value of Register A to verify.  NOTE: This tests `LD A,C` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD C,n"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD C,n command
    helper_ld_X_n('C', i);

    //Send the LD A,C command to move the data into the A register
    helper_ld_X_Y('A','C');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_D_N() {
  //Mnemonic:  LD D,n
  //Assembled command: 0x16
  //Expected behavior: A value is loaded into Register D
  //Test methodology: Load all possible 8 bit values into Register D, load Register A with the contents of Register D, retrieve the value of Register A to verify.  NOTE: This tests `LD A,D` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD D,n"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD D,n command
    helper_ld_X_n('D', i);

    //Send the LD A,D command to move the data into the A register
    helper_ld_X_Y('A','D');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_E_N() {
  //Mnemonic:  LD E,n
  //Assembled command: 0x1E
  //Expected behavior: A value is loaded into Register E
  //Test methodology: Load all possible 8 bit values into Register E, load Register A with the contents of Register E, retrieve the value of Register A to verify.  NOTE: This tests `LD A,E` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD E,n"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD C,n command
    helper_ld_X_n('E', i);

    //Send the LD A,C command to move the data into the A register
    helper_ld_X_Y('A','E');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_H_N() {
  //Mnemonic:  LD H,n
  //Assembled command: 0x26
  //Expected behavior: A value is loaded into Register H
  //Test methodology: Load all possible 8 bit values into Register H, load Register A with the contents of Register H, retrieve the value of Register A to verify.  NOTE: This tests `LD A,H` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD H,n"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD H,n command
    helper_ld_X_n('H', i);

    //Send the LD A,H command to move the data into the A register
    helper_ld_X_Y('A','H');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_L_N() {
  //Mnemonic:  LD L,n
  //Assembled command: 0x2E
  //Expected behavior: A value is loaded into Register L
  //Test methodology: Load all possible 8 bit values into Register L, load Register A with the contents of Register L, retrieve the value of Register A to verify.  NOTE: This tests `LD A,L` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD L,n"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD L,n command
    helper_ld_X_n('L', i);

    //Send the LD A,L command to move the data into the A register
    helper_ld_X_Y('A','L');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_PTR_HL_N() {
  //Mnemonic:  LD (HL),n
  //Assembled command: 0x36
  //Expected behavior: A value is loaded into the memory address in the HL Register
  //Test methodology: Load Register HL with each possible address, load a random value into the memory address (HL).  Load Register HL with a random address, load each possible value into that memory address.
  //Success criteria: Verify that the returned data & address matches the loaded data & address.

  //Initialize some variables
  bool error = false;
  char buffer[24];

  char targetMnemonic[]="LD (HL),n";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));
  byte data[] = {inst.opCode, 0x00, 0x00};
  byte loadHLData[]={0x00, 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00};

  Serial.print(F("Testing LD HL,n"));

  //Test enough times to ensure that we have hit every possible address value with random data
  for (uint16_t i = 0; i < 65535; i++) {

    loadHLData[0]=lowByte(i);
    loadHLData[1]=highByte(i);
    //Send the LD HL,nn command
    helper_ld_XY_nn('H', 'L', loadHLData);

    long sentData=random(255);
    data[1]=lowByte(sentData);

    //Load the target address into the HL Register

    writeSingleInstruction(data, 1, 1, 1, addr);
    if (data[2] != lowByte(sentData) || addr != i) {
      error=true;
    } 
    
    //Display progress
    if ((i % 1000) == 0) {
      Serial.print(F("."));
    }
    
  }
  
  //Test enough times to ensure that we have hit every possible data value with a random address
  for (int j=0; j<256; j++)
  {

    long sentAddress=random(65535);
    loadHLData[0]=lowByte(sentAddress);
    loadHLData[1]=highByte(sentAddress);
    //Send the LD HL,nn command
    helper_ld_XY_nn('H', 'L', loadHLData);

    data[1]=j;

    //Load the target address into the HL Register

    writeSingleInstruction(data, 1, 1, 1, addr);
    if (data[1] != j || addr != sentAddress) {
      error=true;
    }
        //Display progress
    if ((j % 8) == 0) {
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
bool LD_A_A() {
  //Mnemonic:  LD A,A
  //Assembled command: 0x7f
  //Expected behavior: A value in Register A is moved to Register A
  //Test methodology: Load all possible 8 bit values into Register A, load Register A with the contents of Register A, retrieve the value of Register A to verify.  NOTE: This tests `LD A,n` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD A,A"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD A,n command
    helper_ld_X_n('A', i);

    //Send the LD A,A command to move the data into the A register
    helper_ld_X_Y('A','A');

    //Send the LD A,A command to move the data into the A register
    helper_ld_X_Y('A','A');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_B_A() {
  //Mnemonic:  LD B,A
  //Assembled command: 0x47
  //Expected behavior: A value in Register A is moved to Register B
  //Test methodology: Load all possible 8 bit values into Register A, load Register B with the contents of Register A, retrieve the value of Register B to verify.  NOTE: This tests `LD A,n` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD B,A"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD A,n command
    helper_ld_X_n('A', i);

    //Send the LD B,A command to move the data into the B register
    helper_ld_X_Y('B','A');

    //Send the LD B,A command to move the data into the A register
    helper_ld_X_Y('A','B');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_C_A() {
  //Mnemonic:  LD C,A
  //Assembled command: 0x4f
  //Expected behavior: A value in Register A is moved to Register C
  //Test methodology: Load all possible 8 bit values into Register A, load Register C with the contents of Register A, retrieve the value of Register C to verify.  NOTE: This tests `LD A,n` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD C,A"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD A,n command
    helper_ld_X_n('A', i);

    //Send the LD C,A command to move the data into the C register
    helper_ld_X_Y('C','A');

    //Send the LD C,A command to move the data into the A register
    helper_ld_X_Y('A','C');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_D_A() {
  //Mnemonic:  LD D,A
  //Assembled command: 0x57
  //Expected behavior: A value in Register A is moved to Register D
  //Test methodology: Load all possible 8 bit values into Register A, load Register D with the contents of Register A, retrieve the value of Register D to verify.  NOTE: This tests `LD A,n` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD D,A"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD A,n command
    helper_ld_X_n('A', i);

    //Send the LD D,A command to move the data into the D register
    helper_ld_X_Y('D','A');

    //Send the LD D,A command to move the data into the A register
    helper_ld_X_Y('A','D');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_E_A() {
  //Mnemonic:  LD E,A
  //Assembled command: 0x5f
  //Expected behavior: A value in Register A is moved to Register E
  //Test methodology: Load all possible 8 bit values into Register A, load Register E with the contents of Register A, retrieve the value of Register E to verify.  NOTE: This tests `LD A,n` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD E,A"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD A,n command
    helper_ld_X_n('A', i);

    //Send the LD E,A command to move the data into the E register
    helper_ld_X_Y('E','A');

    //Send the LD E,A command to move the data into the A register
    helper_ld_X_Y('A','E');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_H_A() {
  //Mnemonic:  LD H,A
  //Assembled command: 0x67
  //Expected behavior: A value in Register A is moved to Register H
  //Test methodology: Load all possible 8 bit values into Register A, load Register H with the contents of Register A, retrieve the value of Register H to verify.  NOTE: This tests `LD A,n` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD H,A"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD A,n command
    helper_ld_X_n('A', i);

    //Send the LD H,A command to move the data into the H register
    helper_ld_X_Y('H','A');

    //Send the LD H,A command to move the data into the A register
    helper_ld_X_Y('A','H');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_L_A() {
  //Mnemonic:  LD L,A
  //Assembled command: 0x6f
  //Expected behavior: A value in Register A is moved to Register L
  //Test methodology: Load all possible 8 bit values into Register A, load Register L with the contents of Register A, retrieve the value of Register L to verify.  NOTE: This tests `LD A,n` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD L,A"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD A,n command
    helper_ld_X_n('A', i);

    //Send the LD L,A command to move the data into the L register
    helper_ld_X_Y('L','A');

    //Send the LD L,A command to move the data into the A register
    helper_ld_X_Y('A','L');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_A_B() {
  //Mnemonic:  LD A,B
  //Assembled command: 0x78
  //Expected behavior: A value in Register B is moved to Register A
  //Test methodology: Load all possible 8 bit values into Register B, load Register A with the contents of Register B, retrieve the value of Register A to verify.  NOTE: This tests `LD B,n` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD A,B"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD B,n command
    helper_ld_X_n('B', i);

    //Send the LD A,B command to move the data into the A register
    helper_ld_X_Y('A','B');

    //Send the LD A,A command to move the data into the A register
    helper_ld_X_Y('A','A');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_B_B() {
  //Mnemonic:  LD B,B
  //Assembled command: 0x40
  //Expected behavior: A value in Register B is moved to Register B
  //Test methodology: Load all possible 8 bit values into Register B, load Register B with the contents of Register B, retrieve the value of Register B to verify.  NOTE: This tests `LD B,n` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD B,B"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD B,n command
    helper_ld_X_n('B', i);

    //Send the LD B,B command to move the data into the B register
    helper_ld_X_Y('B','B');

    //Send the LD B,A command to move the data into the A register
    helper_ld_X_Y('A','B');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_C_B() {
  //Mnemonic:  LD C,B
  //Assembled command: 0x48
  //Expected behavior: A value in Register B is moved to Register C
  //Test methodology: Load all possible 8 bit values into Register B, load Register C with the contents of Register B, retrieve the value of Register C to verify.  NOTE: This tests `LD B,n` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD C,B"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD B,n command
    helper_ld_X_n('B', i);

    //Send the LD C,B command to move the data into the C register
    helper_ld_X_Y('C','B');

    //Send the LD C,A command to move the data into the A register
    helper_ld_X_Y('A','C');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_D_B() {
  //Mnemonic:  LD D,B
  //Assembled command: 0x50
  //Expected behavior: A value in Register B is moved to Register D
  //Test methodology: Load all possible 8 bit values into Register B, load Register D with the contents of Register B, retrieve the value of Register D to verify.  NOTE: This tests `LD B,n` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD D,B"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD B,n command
    helper_ld_X_n('B', i);

    //Send the LD D,B command to move the data into the D register
    helper_ld_X_Y('D','B');

    //Send the LD D,A command to move the data into the A register
    helper_ld_X_Y('A','D');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_E_B() {
  //Mnemonic:  LD E,B
  //Assembled command: 0x58
  //Expected behavior: A value in Register B is moved to Register E
  //Test methodology: Load all possible 8 bit values into Register B, load Register E with the contents of Register B, retrieve the value of Register E to verify.  NOTE: This tests `LD B,n` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD E,B"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD B,n command
    helper_ld_X_n('B', i);

    //Send the LD E,B command to move the data into the E register
    helper_ld_X_Y('E','B');

    //Send the LD E,A command to move the data into the A register
    helper_ld_X_Y('A','E');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_H_B() {
  //Mnemonic:  LD H,B
  //Assembled command: 0x60
  //Expected behavior: A value in Register B is moved to Register H
  //Test methodology: Load all possible 8 bit values into Register B, load Register H with the contents of Register B, retrieve the value of Register H to verify.  NOTE: This tests `LD B,n` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD H,B"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD B,n command
    helper_ld_X_n('B', i);

    //Send the LD H,B command to move the data into the H register
    helper_ld_X_Y('H','B');

    //Send the LD H,A command to move the data into the A register
    helper_ld_X_Y('A','H');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_L_B() {
  //Mnemonic:  LD L,B
  //Assembled command: 0x68
  //Expected behavior: A value in Register B is moved to Register L
  //Test methodology: Load all possible 8 bit values into Register B, load Register L with the contents of Register B, retrieve the value of Register L to verify.  NOTE: This tests `LD B,n` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD L,B"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD B,n command
    helper_ld_X_n('B', i);

    //Send the LD L,B command to move the data into the L register
    helper_ld_X_Y('L','B');

    //Send the LD L,A command to move the data into the A register
    helper_ld_X_Y('A','L');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_A_C() {
  //Mnemonic:  LD A,C
  //Assembled command: 0x79
  //Expected behavior: A value in Register C is moved to Register A
  //Test methodology: Load all possible 8 bit values into Register C, load Register A with the contents of Register C, retrieve the value of Register A to verify.  NOTE: This tests `LD C,n` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD A,C"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD C,n command
    helper_ld_X_n('C', i);

    //Send the LD A,C command to move the data into the A register
    helper_ld_X_Y('A','C');

    //Send the LD A,A command to move the data into the A register
    helper_ld_X_Y('A','A');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_B_C() {
  //Mnemonic:  LD B,C
  //Assembled command: 0x41
  //Expected behavior: A value in Register C is moved to Register B
  //Test methodology: Load all possible 8 bit values into Register C, load Register B with the contents of Register C, retrieve the value of Register B to verify.  NOTE: This tests `LD C,n` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD B,C"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD C,n command
    helper_ld_X_n('C', i);

    //Send the LD B,C command to move the data into the B register
    helper_ld_X_Y('B','C');

    //Send the LD B,A command to move the data into the A register
    helper_ld_X_Y('A','B');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_C_C() {
  //Mnemonic:  LD C,C
  //Assembled command: 0x49
  //Expected behavior: A value in Register C is moved to Register C
  //Test methodology: Load all possible 8 bit values into Register C, load Register C with the contents of Register C, retrieve the value of Register C to verify.  NOTE: This tests `LD C,n` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD C,C"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD C,n command
    helper_ld_X_n('C', i);

    //Send the LD C,C command to move the data into the C register
    helper_ld_X_Y('C','C');

    //Send the LD C,A command to move the data into the A register
    helper_ld_X_Y('A','C');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_D_C() {
  //Mnemonic:  LD D,C
  //Assembled command: 0x51
  //Expected behavior: A value in Register C is moved to Register D
  //Test methodology: Load all possible 8 bit values into Register C, load Register D with the contents of Register C, retrieve the value of Register D to verify.  NOTE: This tests `LD C,n` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD D,C"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD C,n command
    helper_ld_X_n('C', i);

    //Send the LD D,C command to move the data into the D register
    helper_ld_X_Y('D','C');

    //Send the LD D,A command to move the data into the A register
    helper_ld_X_Y('A','D');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_E_C() {
  //Mnemonic:  LD E,C
  //Assembled command: 0x59
  //Expected behavior: A value in Register C is moved to Register E
  //Test methodology: Load all possible 8 bit values into Register C, load Register E with the contents of Register C, retrieve the value of Register E to verify.  NOTE: This tests `LD C,n` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD E,C"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD C,n command
    helper_ld_X_n('C', i);

    //Send the LD E,C command to move the data into the E register
    helper_ld_X_Y('E','C');

    //Send the LD E,A command to move the data into the A register
    helper_ld_X_Y('A','E');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_H_C() {
  //Mnemonic:  LD H,C
  //Assembled command: 0x61
  //Expected behavior: A value in Register C is moved to Register H
  //Test methodology: Load all possible 8 bit values into Register C, load Register H with the contents of Register C, retrieve the value of Register H to verify.  NOTE: This tests `LD C,n` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD H,C"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD C,n command
    helper_ld_X_n('C', i);

    //Send the LD H,C command to move the data into the H register
    helper_ld_X_Y('H','C');

    //Send the LD H,A command to move the data into the A register
    helper_ld_X_Y('A','H');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_L_C() {
  //Mnemonic:  LD L,C
  //Assembled command: 0x69
  //Expected behavior: A value in Register C is moved to Register L
  //Test methodology: Load all possible 8 bit values into Register C, load Register L with the contents of Register C, retrieve the value of Register L to verify.  NOTE: This tests `LD C,n` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD L,C"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD C,n command
    helper_ld_X_n('C', i);

    //Send the LD L,C command to move the data into the L register
    helper_ld_X_Y('L','C');

    //Send the LD L,A command to move the data into the A register
    helper_ld_X_Y('A','L');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_A_D() {
  //Mnemonic:  LD A,D
  //Assembled command: 0x7a
  //Expected behavior: A value in Register D is moved to Register A
  //Test methodology: Load all possible 8 bit values into Register D, load Register A with the contents of Register D, retrieve the value of Register A to verify.  NOTE: This tests `LD D,n` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD A,D"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD D,n command
    helper_ld_X_n('D', i);

    //Send the LD A,D command to move the data into the A register
    helper_ld_X_Y('A','D');

    //Send the LD A,A command to move the data into the A register
    helper_ld_X_Y('A','A');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_B_D() {
  //Mnemonic:  LD B,D
  //Assembled command: 0x42
  //Expected behavior: A value in Register D is moved to Register B
  //Test methodology: Load all possible 8 bit values into Register D, load Register B with the contents of Register D, retrieve the value of Register B to verify.  NOTE: This tests `LD D,n` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD B,D"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD D,n command
    helper_ld_X_n('D', i);

    //Send the LD B,D command to move the data into the B register
    helper_ld_X_Y('B','D');

    //Send the LD B,A command to move the data into the A register
    helper_ld_X_Y('A','B');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_C_D() {
  //Mnemonic:  LD C,D
  //Assembled command: 0x4a
  //Expected behavior: A value in Register D is moved to Register C
  //Test methodology: Load all possible 8 bit values into Register D, load Register C with the contents of Register D, retrieve the value of Register C to verify.  NOTE: This tests `LD D,n` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD C,D"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD D,n command
    helper_ld_X_n('D', i);

    //Send the LD C,D command to move the data into the C register
    helper_ld_X_Y('C','D');

    //Send the LD C,A command to move the data into the A register
    helper_ld_X_Y('A','C');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_D_D() {
  //Mnemonic:  LD D,D
  //Assembled command: 0x52
  //Expected behavior: A value in Register D is moved to Register D
  //Test methodology: Load all possible 8 bit values into Register D, load Register D with the contents of Register D, retrieve the value of Register D to verify.  NOTE: This tests `LD D,n` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD D,D"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD D,n command
    helper_ld_X_n('D', i);

    //Send the LD D,D command to move the data into the D register
    helper_ld_X_Y('D','D');

    //Send the LD D,A command to move the data into the A register
    helper_ld_X_Y('A','D');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_E_D() {
  //Mnemonic:  LD E,D
  //Assembled command: 0x5a
  //Expected behavior: A value in Register D is moved to Register E
  //Test methodology: Load all possible 8 bit values into Register D, load Register E with the contents of Register D, retrieve the value of Register E to verify.  NOTE: This tests `LD D,n` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD E,D"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD D,n command
    helper_ld_X_n('D', i);

    //Send the LD E,D command to move the data into the E register
    helper_ld_X_Y('E','D');

    //Send the LD E,A command to move the data into the A register
    helper_ld_X_Y('A','E');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_H_D() {
  //Mnemonic:  LD H,D
  //Assembled command: 0x62
  //Expected behavior: A value in Register D is moved to Register H
  //Test methodology: Load all possible 8 bit values into Register D, load Register H with the contents of Register D, retrieve the value of Register H to verify.  NOTE: This tests `LD D,n` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD H,D"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD D,n command
    helper_ld_X_n('D', i);

    //Send the LD H,D command to move the data into the H register
    helper_ld_X_Y('H','D');

    //Send the LD H,A command to move the data into the A register
    helper_ld_X_Y('A','H');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_L_D() {
  //Mnemonic:  LD L,D
  //Assembled command: 0x6a
  //Expected behavior: A value in Register D is moved to Register L
  //Test methodology: Load all possible 8 bit values into Register D, load Register L with the contents of Register D, retrieve the value of Register L to verify.  NOTE: This tests `LD D,n` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD L,D"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD D,n command
    helper_ld_X_n('D', i);

    //Send the LD L,D command to move the data into the L register
    helper_ld_X_Y('L','D');

    //Send the LD L,A command to move the data into the A register
    helper_ld_X_Y('A','L');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_A_E() {
  //Mnemonic:  LD A,E
  //Assembled command: 0x7b
  //Expected behavior: A value in Register E is moved to Register A
  //Test methodology: Load all possible 8 bit values into Register E, load Register A with the contents of Register E, retrieve the value of Register A to verify.  NOTE: This tests `LD E,n` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD A,E"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD E,n command
    helper_ld_X_n('E', i);

    //Send the LD A,E command to move the data into the A register
    helper_ld_X_Y('A','E');

    //Send the LD A,A command to move the data into the A register
    helper_ld_X_Y('A','A');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_B_E() {
  //Mnemonic:  LD B,E
  //Assembled command: 0x43
  //Expected behavior: A value in Register E is moved to Register B
  //Test methodology: Load all possible 8 bit values into Register E, load Register B with the contents of Register E, retrieve the value of Register B to verify.  NOTE: This tests `LD E,n` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD B,E"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD E,n command
    helper_ld_X_n('E', i);

    //Send the LD B,E command to move the data into the B register
    helper_ld_X_Y('B','E');

    //Send the LD B,A command to move the data into the A register
    helper_ld_X_Y('A','B');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_C_E() {
  //Mnemonic:  LD C,E
  //Assembled command: 0x4b
  //Expected behavior: A value in Register E is moved to Register C
  //Test methodology: Load all possible 8 bit values into Register E, load Register C with the contents of Register E, retrieve the value of Register C to verify.  NOTE: This tests `LD E,n` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD C,E"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD E,n command
    helper_ld_X_n('E', i);

    //Send the LD C,E command to move the data into the C register
    helper_ld_X_Y('C','E');

    //Send the LD C,A command to move the data into the A register
    helper_ld_X_Y('A','C');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_D_E() {
  //Mnemonic:  LD D,E
  //Assembled command: 0x53
  //Expected behavior: A value in Register E is moved to Register D
  //Test methodology: Load all possible 8 bit values into Register E, load Register D with the contents of Register E, retrieve the value of Register D to verify.  NOTE: This tests `LD E,n` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD D,E"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD E,n command
    helper_ld_X_n('E', i);

    //Send the LD D,E command to move the data into the D register
    helper_ld_X_Y('D','E');

    //Send the LD D,A command to move the data into the A register
    helper_ld_X_Y('A','D');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_E_E() {
  //Mnemonic:  LD E,E
  //Assembled command: 0x5b
  //Expected behavior: A value in Register E is moved to Register E
  //Test methodology: Load all possible 8 bit values into Register E, load Register E with the contents of Register E, retrieve the value of Register E to verify.  NOTE: This tests `LD E,n` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD E,E"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD E,n command
    helper_ld_X_n('E', i);

    //Send the LD E,E command to move the data into the E register
    helper_ld_X_Y('E','E');

    //Send the LD E,A command to move the data into the A register
    helper_ld_X_Y('A','E');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_H_E() {
  //Mnemonic:  LD H,E
  //Assembled command: 0x63
  //Expected behavior: A value in Register E is moved to Register H
  //Test methodology: Load all possible 8 bit values into Register E, load Register H with the contents of Register E, retrieve the value of Register H to verify.  NOTE: This tests `LD E,n` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD H,E"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD E,n command
    helper_ld_X_n('E', i);

    //Send the LD H,E command to move the data into the H register
    helper_ld_X_Y('H','E');

    //Send the LD H,A command to move the data into the A register
    helper_ld_X_Y('A','H');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_L_E() {
  //Mnemonic:  LD L,E
  //Assembled command: 0x6b
  //Expected behavior: A value in Register E is moved to Register L
  //Test methodology: Load all possible 8 bit values into Register E, load Register L with the contents of Register E, retrieve the value of Register L to verify.  NOTE: This tests `LD E,n` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD L,E"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD E,n command
    helper_ld_X_n('E', i);

    //Send the LD L,E command to move the data into the L register
    helper_ld_X_Y('L','E');

    //Send the LD L,A command to move the data into the A register
    helper_ld_X_Y('A','L');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_A_H() {
  //Mnemonic:  LD A,H
  //Assembled command: 0x7c
  //Expected behavior: A value in Register H is moved to Register A
  //Test methodology: Load all possible 8 bit values into Register H, load Register A with the contents of Register H, retrieve the value of Register A to verify.  NOTE: This tests `LD H,n` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD A,H"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD H,n command
    helper_ld_X_n('H', i);

    //Send the LD A,H command to move the data into the A register
    helper_ld_X_Y('A','H');

    //Send the LD A,A command to move the data into the A register
    helper_ld_X_Y('A','A');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_B_H() {
  //Mnemonic:  LD B,H
  //Assembled command: 0x44
  //Expected behavior: A value in Register H is moved to Register B
  //Test methodology: Load all possible 8 bit values into Register H, load Register B with the contents of Register H, retrieve the value of Register B to verify.  NOTE: This tests `LD H,n` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD B,H"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD H,n command
    helper_ld_X_n('H', i);

    //Send the LD B,H command to move the data into the B register
    helper_ld_X_Y('B','H');

    //Send the LD B,A command to move the data into the A register
    helper_ld_X_Y('A','B');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_C_H() {
  //Mnemonic:  LD C,H
  //Assembled command: 0x4c
  //Expected behavior: A value in Register H is moved to Register C
  //Test methodology: Load all possible 8 bit values into Register H, load Register C with the contents of Register H, retrieve the value of Register C to verify.  NOTE: This tests `LD H,n` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD C,H"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD H,n command
    helper_ld_X_n('H', i);

    //Send the LD C,H command to move the data into the C register
    helper_ld_X_Y('C','H');

    //Send the LD C,A command to move the data into the A register
    helper_ld_X_Y('A','C');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_D_H() {
  //Mnemonic:  LD D,H
  //Assembled command: 0x54
  //Expected behavior: A value in Register H is moved to Register D
  //Test methodology: Load all possible 8 bit values into Register H, load Register D with the contents of Register H, retrieve the value of Register D to verify.  NOTE: This tests `LD H,n` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD D,H"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD H,n command
    helper_ld_X_n('H', i);

    //Send the LD D,H command to move the data into the D register
    helper_ld_X_Y('D','H');

    //Send the LD D,A command to move the data into the A register
    helper_ld_X_Y('A','D');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_E_H() {
  //Mnemonic:  LD E,H
  //Assembled command: 0x5c
  //Expected behavior: A value in Register H is moved to Register E
  //Test methodology: Load all possible 8 bit values into Register H, load Register E with the contents of Register H, retrieve the value of Register E to verify.  NOTE: This tests `LD H,n` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD E,H"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD H,n command
    helper_ld_X_n('H', i);

    //Send the LD E,H command to move the data into the E register
    helper_ld_X_Y('E','H');

    //Send the LD E,A command to move the data into the A register
    helper_ld_X_Y('A','E');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_H_H() {
  //Mnemonic:  LD H,H
  //Assembled command: 0x64
  //Expected behavior: A value in Register H is moved to Register H
  //Test methodology: Load all possible 8 bit values into Register H, load Register H with the contents of Register H, retrieve the value of Register H to verify.  NOTE: This tests `LD H,n` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD H,H"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD H,n command
    helper_ld_X_n('H', i);

    //Send the LD H,H command to move the data into the H register
    helper_ld_X_Y('H','H');

    //Send the LD H,A command to move the data into the A register
    helper_ld_X_Y('A','H');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_L_H() {
  //Mnemonic:  LD L,H
  //Assembled command: 0x6c
  //Expected behavior: A value in Register H is moved to Register L
  //Test methodology: Load all possible 8 bit values into Register H, load Register L with the contents of Register H, retrieve the value of Register L to verify.  NOTE: This tests `LD H,n` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD L,H"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD H,n command
    helper_ld_X_n('H', i);

    //Send the LD L,H command to move the data into the L register
    helper_ld_X_Y('L','H');

    //Send the LD L,A command to move the data into the A register
    helper_ld_X_Y('A','L');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_A_L() {
  //Mnemonic:  LD A,L
  //Assembled command: 0x7d
  //Expected behavior: A value in Register L is moved to Register A
  //Test methodology: Load all possible 8 bit values into Register L, load Register A with the contents of Register L, retrieve the value of Register A to verify.  NOTE: This tests `LD L,n` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD A,L"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD L,n command
    helper_ld_X_n('L', i);

    //Send the LD A,L command to move the data into the A register
    helper_ld_X_Y('A','L');

    //Send the LD A,A command to move the data into the A register
    helper_ld_X_Y('A','A');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_B_L() {
  //Mnemonic:  LD B,L
  //Assembled command: 0x45
  //Expected behavior: A value in Register L is moved to Register B
  //Test methodology: Load all possible 8 bit values into Register L, load Register B with the contents of Register L, retrieve the value of Register B to verify.  NOTE: This tests `LD L,n` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD B,L"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD L,n command
    helper_ld_X_n('L', i);

    //Send the LD B,L command to move the data into the B register
    helper_ld_X_Y('B','L');

    //Send the LD B,A command to move the data into the A register
    helper_ld_X_Y('A','B');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_C_L() {
  //Mnemonic:  LD C,L
  //Assembled command: 0x4d
  //Expected behavior: A value in Register L is moved to Register C
  //Test methodology: Load all possible 8 bit values into Register L, load Register C with the contents of Register L, retrieve the value of Register C to verify.  NOTE: This tests `LD L,n` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD C,L"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD L,n command
    helper_ld_X_n('L', i);

    //Send the LD C,L command to move the data into the C register
    helper_ld_X_Y('C','L');

    //Send the LD C,A command to move the data into the A register
    helper_ld_X_Y('A','C');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_D_L() {
  //Mnemonic:  LD D,L
  //Assembled command: 0x55
  //Expected behavior: A value in Register L is moved to Register D
  //Test methodology: Load all possible 8 bit values into Register L, load Register D with the contents of Register L, retrieve the value of Register D to verify.  NOTE: This tests `LD L,n` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD D,L"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD L,n command
    helper_ld_X_n('L', i);

    //Send the LD D,L command to move the data into the D register
    helper_ld_X_Y('D','L');

    //Send the LD D,A command to move the data into the A register
    helper_ld_X_Y('A','D');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_E_L() {
  //Mnemonic:  LD E,L
  //Assembled command: 0x5d
  //Expected behavior: A value in Register L is moved to Register E
  //Test methodology: Load all possible 8 bit values into Register L, load Register E with the contents of Register L, retrieve the value of Register E to verify.  NOTE: This tests `LD L,n` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD E,L"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD L,n command
    helper_ld_X_n('L', i);

    //Send the LD E,L command to move the data into the E register
    helper_ld_X_Y('E','L');

    //Send the LD E,A command to move the data into the A register
    helper_ld_X_Y('A','E');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_H_L() {
  //Mnemonic:  LD H,L
  //Assembled command: 0x65
  //Expected behavior: A value in Register L is moved to Register H
  //Test methodology: Load all possible 8 bit values into Register L, load Register H with the contents of Register L, retrieve the value of Register H to verify.  NOTE: This tests `LD L,n` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD H,L"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD L,n command
    helper_ld_X_n('L', i);

    //Send the LD H,L command to move the data into the H register
    helper_ld_X_Y('H','L');

    //Send the LD H,A command to move the data into the A register
    helper_ld_X_Y('A','H');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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
bool LD_L_L() {
  //Mnemonic:  LD L,L
  //Assembled command: 0x6d
  //Expected behavior: A value in Register L is moved to Register L
  //Test methodology: Load all possible 8 bit values into Register L, load Register L with the contents of Register L, retrieve the value of Register L to verify.  NOTE: This tests `LD L,n` and `LD (nn),A` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte outputData[] = {0x00, 0x00, 0x00};
  char buffer[24];
  uint16_t addr[]={0x00};

  Serial.print(F("Testing LD L,L"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD L,n command
    helper_ld_X_n('L', i);

    //Send the LD L,L command to move the data into the L register
    helper_ld_X_Y('L','L');

    //Send the LD L,A command to move the data into the A register
    helper_ld_X_Y('A','L');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x0;
    }
    helper_ld_ptr_nn_a(outputData, addr);

   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
    }

    //Display progress
    if ((i % 8) == 0) {
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

bool LD_A_PTR_HL() {
  //Mnemonic:  LD A,(HL)
  //Assembled command: 0x36
  //Expected behavior: A value is loaded into the memory address in the HL Register
  //Test methodology: Load Register HL with each possible address, load a random value into the memory address (HL).  Load Register HL with a random address, load each possible value into that memory address.
  //Success criteria: Verify that the returned data & address matches the loaded data & address.

  //Initialize some variables
  bool error = false;
  char buffer[24];

  char targetMnemonic[]="LD A,(HL)";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));
  byte data[] = {inst.opCode, 0x00, 0x00};
  byte loadHLData[]={0x00, 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};

  Serial.print(F("Testing A,(HL)"));

  //Test enough times to ensure that we have hit every possible address value with random data.  We are not explicitly verifying the address read, just the data returned.  This will only be consistently correct if the address being read is correct.
  for (uint16_t i = 0; i < 65535; i++) {

    //Load the target address into the HL Register
    loadHLData[0]=lowByte(i);
    loadHLData[1]=highByte(i);
    helper_ld_XY_nn('H', 'L', loadHLData);

    //Generate random data
    long sentData=random(255);
    data[1]=lowByte(sentData);

    //Load that data into the A register
    writeSingleInstruction(data, 1, 1, 0, addr);
    
    helper_ld_ptr_nn_a(data, addr);


    if (data[2] != lowByte(sentData)) {
      error=true;
      Serial.print(F("Expected data: "));
      Serial.print(lowByte(sentData),HEX);
      Serial.print(F(" got: "));
      Serial.println(data[2],HEX);
    } 
    
    //Display progress
    if ((i % 1000) == 0) {
      Serial.print(F("."));
    }
    
  }
  
  //Test enough times to ensure that we have hit every possible data value with a random address.  We are not explicitly verifying the address read, just the data returned.  This will only be consistently correct if the address being read is correct.
  for (int j=0; j<256; j++)
  {

    //Load a random address into 
    long sentAddress=random(65535);
    loadHLData[0]=lowByte(sentAddress);
    loadHLData[1]=highByte(sentAddress);
    helper_ld_XY_nn('H', 'L', loadHLData);

    data[1]=j;

    writeSingleInstruction(data, 1, 1, 0, addr);
    
    helper_ld_ptr_nn_a(data, addr);


    if (data[2] != j) {
      error=true;
      Serial.print(F("Expected data: "));
      Serial.print(lowByte(j),HEX);
      Serial.print(F(" got: "));
      Serial.println(data[2],HEX);
    } 
    
  }

  //Display status
  if (error) {
    Serial.println(F("Fail"));
  } else {
    Serial.println(F("Pass"));
  }
}
bool LD_B_PTR_HL() {}
bool LD_C_PTR_HL() {}
bool LD_D_PTR_HL() {}
bool LD_E_PTR_HL() {}
bool LD_H_PTR_HL() {}
bool LD_L_PTR_HL() {}
bool LD_PTR_HL_A() {}
bool LD_PTR_HL_B() {}
bool LD_PTR_HL_C() {}
bool LD_PTR_HL_D() {}
bool LD_PTR_HL_E() {}
bool LD_PTR_HL_H() {}
bool LD_PTR_HL_L() {}
bool LD_A_PTR_BC() {}
bool LD_A_PTR_DE() {}
bool LD_A_PTR_NN() {}
bool LD_PTR_NN_A() {}
bool LD_PTR_BC_A() {}
bool LD_PTR_DE_A() {}
bool LD_A_I() {}
bool LD_A_R() {}
bool LD_I_A() {}
bool LD_R_A() {}
bool LD_PTR_IX_PLUS_D_N() {}
bool LD_PTR_IY_PLUS_D_N() {}
bool LD_A_PTR_IX_PLUS_D() {}
bool LD_B_PTR_IX_PLUS_D() {}
bool LD_C_PTR_IX_PLUS_D() {}
bool LD_D_PTR_IX_PLUS_D() {}
bool LD_E_PTR_IX_PLUS_D() {}
bool LD_H_PTR_IX_PLUS_D() {}
bool LD_L_PTR_IX_PLUS_D() {}
bool LD_A_PTR_IY_PLUS_D() {}
bool LD_B_PTR_IY_PLUS_D() {}
bool LD_C_PTR_IY_PLUS_D() {}
bool LD_D_PTR_IY_PLUS_D() {}
bool LD_E_PTR_IY_PLUS_D() {}
bool LD_H_PTR_IY_PLUS_D() {}
bool LD_L_PTR_IY_PLUS_D() {}
bool LD_PTR_IX_PLUS_D_A() {}
bool LD_PTR_IX_PLUS_D_B() {}
bool LD_PTR_IX_PLUS_D_C() {}
bool LD_PTR_IX_PLUS_D_D() {}
bool LD_PTR_IX_PLUS_D_E() {}
bool LD_PTR_IX_PLUS_D_H() {}
bool LD_PTR_IX_PLUS_D_L() {}
bool LD_PTR_IY_PLUS_D_A() {}
bool LD_PTR_IY_PLUS_D_B() {}
bool LD_PTR_IY_PLUS_D_C() {}
bool LD_PTR_IY_PLUS_D_D() {}
bool LD_PTR_IY_PLUS_D_E() {}
bool LD_PTR_IY_PLUS_D_H() {}
bool LD_PTR_IY_PLUS_D_L() {}
bool LD_BC_NN() {}
bool LD_DE_NN() {}
bool LD_HL_NN() {
  //Mnemonic:  LD HL,nn
  //Assembled command: 0x21
  //Expected behavior: A value is loaded into Register HL
  //Test methodology: Load all possible 16 bit values into Register HL, retrieve the value of Register HL to verify.  NOTE: This tests `LD (nn),HL` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte data[] = {0x00, 0x00, 0x00, 0x00};
  char buffer[24];

  Serial.print(F("Testing LD HL,nn"));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 65535; i++) {

    data[0]=lowByte(i);
    data[1]=highByte(i);
    //Send the LD A,n command
    helper_ld_XY_nn('H', 'L', data);

    //Read the data in the HL register   
    for (int j=0; j<4; j++) {
      data[j]=0x0;
    }
    helper_ld_ptr_nn_hl(data);

   
    //Test if the Data Bus reads the expected value
    if (data[2] != lowByte(i) || data[3] != highByte(i)) {
      error = true;
      Serial.print(F("Expected Low Byte: "));
      Serial.print(lowByte(i));
      Serial.print(F(" got "));
      Serial.print(data[2]);
      Serial.print(F(" Expected High Byte: "));
      Serial.print(highByte(i));
      Serial.print(F(" got "));
      Serial.println(data[3]);
    }

    //Display progress
    if ((i % 1000) == 0) {
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
bool LD_SP_NN() {}
bool LD_IX_NN() {}
bool LD_IY_NN() {}
bool LD_BC_PTR_NN() {}
bool LD_DE_PTR_NN() {}
bool LD_HL_PTR_NN() {}
bool LD_SP_PTR_NN() {}
bool LD_IX_PTR_NN() {}
bool LD_IY_PTR_NN() {}
bool LD_PTR_NN_BC() {}
bool LD_PTR_NN_DE() {}
bool LD_PTR_NN_HL() {}
bool LD_PTR_NN_SP() {}
bool LD_PTR_NN_IX() {}
bool LD_PTR_NN_IY() {}
bool LD_SP_HL() {}
bool LD_SP_IX() {}
bool LD_SP_IY() {}
bool POP_AF() {}
bool POP_BC() {}
bool POP_DE() {}
bool POP_HL() {}
bool POP_IX() {}
bool POP_IY() {}
bool PUSH_AF() {}
bool PUSH_BC() {}
bool PUSH_DE() {}
bool PUSH_HL() {}
bool PUSH_IX() {}
bool PUSH_IY() {}
bool EX_AF_AF_PRIME() {}
bool EXX() {}
bool EX_DE_HL() {}
bool EX_PTR_SP_HL() {}
bool EX_PTR_SP_IX() {}
bool EX_PTR_SP_IY() {}
bool LDI() {}
bool LDIR() {}
bool LDD() {}
bool LDDR() {}
bool CPI() {}
bool CPIR() {}
bool CPD() {}
bool CPDR() {}
bool ADD_A_A() {}
bool ADD_A_B() {}
bool ADD_A_C() {}
bool ADD_A_D() {}
bool ADD_A_E() {}
bool ADD_A_H() {}
bool ADD_A_L() {}
bool ADD_A_PTR_HL() {}
bool ADD_A_PTR_IX_PLUS_D() {}
bool ADD_A_PTR_IY_PLUS_D() {}
bool ADD_A_N() {}
bool ADC_A_A() {}
bool ADC_A_B() {}
bool ADC_A_C() {}
bool ADC_A_D() {}
bool ADC_A_E() {}
bool ADC_A_H() {}
bool ADC_A_L() {}
bool ADC_A_PTR_HL() {}
bool ADC_A_PTR_IX_PLUS_D() {}
bool ADC_A_PTR_IY_PLUS_D() {}
bool ADC_A_N() {}
bool SUB_A_A() {}
bool SUB_A_B() {}
bool SUB_A_C() {}
bool SUB_A_D() {}
bool SUB_A_E() {}
bool SUB_A_H() {}
bool SUB_A_L() {}
bool SUB_A_PTR_HL() {}
bool SUB_A_PTR_IX_PLUS_D() {}
bool SUB_A_PTR_IY_PLUS_D() {}
bool SUB_A_N() {}
bool SBC_A_A() {}
bool SBC_A_B() {}
bool SBC_A_C() {}
bool SBC_A_D() {}
bool SBC_A_E() {}
bool SBC_A_H() {}
bool SBC_A_L() {}
bool SBC_A_PTR_HL() {}
bool SBC_A_PTR_IX_PLUS_D() {}
bool SBC_A_PTR_IY_PLUS_D() {}
bool SBC_A_N() {}
bool AND_A() {}
bool AND_B() {}
bool AND_C() {}
bool AND_D() {}
bool AND_E() {}
bool AND_H() {}
bool AND_L() {}
bool AND_PTR_HL() {}
bool AND_PTR_IX_PLUS_D() {}
bool AND_PTR_IY_PLUS_D() {}
bool AND_N() {}
bool XOR_A() {}
bool XOR_B() {}
bool XOR_C() {}
bool XOR_D() {}
bool XOR_E() {}
bool XOR_H() {}
bool XOR_L() {}
bool XOR_PTR_HL() {}
bool XOR_PTR_IX_PLUS_D() {}
bool XOR_PTR_IY_PLUS_D() {}
bool XOR_N() {}
bool OR_A() {}
bool OR_B() {}
bool OR_C() {}
bool OR_D() {}
bool OR_E() {}
bool OR_H() {}
bool OR_L() {}
bool OR_PTR_HL() {}
bool OR_PTR_IX_PLUS_D() {}
bool OR_PTR_IY_PLUS_D() {}
bool OR_N() {}
bool CP_A() {}
bool CP_B() {}
bool CP_C() {}
bool CP_D() {}
bool CP_E() {}
bool CP_H() {}
bool CP_L() {}
bool CP_PTR_HL() {}
bool CP_PTR_IX_PLUS_D() {}
bool CP_PTR_IY_PLUS_D() {}
bool CP_N() {}
bool INC_A() {}
bool INC_B() {}
bool INC_C() {}
bool INC_D() {}
bool INC_E() {}
bool INC_H() {}
bool INC_L() {}
bool INC_PTR_HL() {}
bool INC_PTR_IX_PLUS_D() {}
bool INC_PTR_IY_PLUS_D() {}
bool DEC_A() {}
bool DEC_B() {}
bool DEC_C() {}
bool DEC_D() {}
bool DEC_E() {}
bool DEC_H() {}
bool DEC_L() {}
bool DEC_PTR_HL() {}
bool DEC_PTR_IX_PLUS_D() {}
bool DEC_PTR_IY_PLUS_D() {}
bool DAA() {}
bool CPL() {}
bool NEG() {}
bool CCF() {}
bool SCF() {}
bool ADD_HL_BC() {}
bool ADD_HL_DE() {}
bool ADD_HL_HL() {}
bool ADD_HL_SP() {}
bool ADD_IX_BC() {}
bool ADD_IX_DE() {}
bool ADD_IX_SP() {}
bool ADD_IX_IX() {}
bool ADD_IY_BC() {}
bool ADD_IY_DE() {}
bool ADD_IY_SP() {}
bool ADD_IY_IY() {}
bool ADC_HL_BC() {}
bool ADC_HL_DE() {}
bool ADC_HL_HL() {}
bool ADC_HL_SP() {}
bool SBC_HL_BC() {}
bool SBC_HL_DE() {}
bool SBC_HL_HL() {}
bool SBC_HL_SP() {}
bool INC_BC() {}
bool INC_DE() {}
bool INC_HL() {}
bool INC_SP() {}
bool INC_IX() {}
bool INC_IY() {}
bool DEC_BC() {}
bool DEC_DE() {}
bool DEC_HL() {}
bool DEC_SP() {}
bool DEC_IX() {}
bool DEC_IY() {}
bool RLC_A() {}
bool RLC_B() {}
bool RLC_C() {}
bool RLC_D() {}
bool RLC_E() {}
bool RLC_H() {}
bool RLC_L() {}
bool RLC_PTR_HL() {}
bool RLC_PTR_IX_PLUS_D() {}
bool RLC_PTR_IY_PLUS_D() {}
bool RRC_A() {}
bool RRC_B() {}
bool RRC_C() {}
bool RRC_D() {}
bool RRC_E() {}
bool RRC_H() {}
bool RRC_L() {}
bool RRC_PTR_HL() {}
bool RRC_PTR_IX_PLUS_D() {}
bool RRC_PTR_IY_PLUS_D() {}
bool RL_A() {}
bool RL_B() {}
bool RL_C() {}
bool RL_D() {}
bool RL_E() {}
bool RL_H() {}
bool RL_L() {}
bool RL_PTR_HL() {}
bool RL_PTR_IX_PLUS_D() {}
bool RL_PTR_IY_PLUS_D() {}
bool RR_A() {}
bool RR_B() {}
bool RR_C() {}
bool RR_D() {}
bool RR_E() {}
bool RR_H() {}
bool RR_L() {}
bool RR_PTR_HL() {}
bool RR_PTR_IX_PLUS_D() {}
bool RR_PTR_IY_PLUS_D() {}
bool SLA_A() {}
bool SLA_B() {}
bool SLA_C() {}
bool SLA_D() {}
bool SLA_E() {}
bool SLA_H() {}
bool SLA_L() {}
bool SLA_PTR_HL() {}
bool SLA_PTR_IX_PLUS_D() {}
bool SLA_PTR_IY_PLUS_D() {}
bool SRA_A() {}
bool SRA_B() {}
bool SRA_C() {}
bool SRA_D() {}
bool SRA_E() {}
bool SRA_H() {}
bool SRA_L() {}
bool SRA_PTR_HL() {}
bool SRA_PTR_IX_PLUS_D() {}
bool SRA_PTR_IY_PLUS_D() {}
bool SLL_A() {}
bool SLL_B() {}
bool SLL_C() {}
bool SLL_D() {}
bool SLL_E() {}
bool SLL_H() {}
bool SLL_L() {}
bool SLL_PTR_HL() {}
bool SLL_PTR_IX_PLUS_D() {}
bool SLL_PTR_IY_PLUS_D() {}
bool SRL_A() {}
bool SRL_B() {}
bool SRL_C() {}
bool SRL_D() {}
bool SRL_E() {}
bool SRL_H() {}
bool SRL_L() {}
bool SRL_PTR_HL() {}
bool SRL_PTR_IX_PLUS_D() {}
bool SRL_PTR_IY_PLUS_D() {}
bool RLD() {}
bool RRD() {}
bool RLCA() {}
bool RRCA() {}
bool RLA() {}
bool RRA() {}
bool BIT_0_A() {}
bool BIT_0_B() {}
bool BIT_0_C() {}
bool BIT_0_D() {}
bool BIT_0_E() {}
bool BIT_0_H() {}
bool BIT_0_L() {}
bool BIT_0_PTR_HL() {}
bool BIT_0_PTR_IX_PLUS_D() {}
bool BIT_0_PTR_IY_PLUS_D() {}
bool BIT_1_A() {}
bool BIT_1_B() {}
bool BIT_1_C() {}
bool BIT_1_D() {}
bool BIT_1_E() {}
bool BIT_1_H() {}
bool BIT_1_L() {}
bool BIT_1_PTR_HL() {}
bool BIT_1_PTR_IX_PLUS_D() {}
bool BIT_1_PTR_IY_PLUS_D() {}
bool BIT_2_A() {}
bool BIT_2_B() {}
bool BIT_2_C() {}
bool BIT_2_D() {}
bool BIT_2_E() {}
bool BIT_2_H() {}
bool BIT_2_L() {}
bool BIT_2_PTR_HL() {}
bool BIT_2_PTR_IX_PLUS_D() {}
bool BIT_2_PTR_IY_PLUS_D() {}
bool BIT_3_A() {}
bool BIT_3_B() {}
bool BIT_3_C() {}
bool BIT_3_D() {}
bool BIT_3_E() {}
bool BIT_3_H() {}
bool BIT_3_L() {}
bool BIT_3_PTR_HL() {}
bool BIT_3_PTR_IX_PLUS_D() {}
bool BIT_3_PTR_IY_PLUS_D() {}
bool BIT_4_A() {}
bool BIT_4_B() {}
bool BIT_4_C() {}
bool BIT_4_D() {}
bool BIT_4_E() {}
bool BIT_4_H() {}
bool BIT_4_L() {}
bool BIT_4_PTR_HL() {}
bool BIT_4_PTR_IX_PLUS_D() {}
bool BIT_4_PTR_IY_PLUS_D() {}
bool BIT_5_A() {}
bool BIT_5_B() {}
bool BIT_5_C() {}
bool BIT_5_D() {}
bool BIT_5_E() {}
bool BIT_5_H() {}
bool BIT_5_L() {}
bool BIT_5_PTR_HL() {}
bool BIT_5_PTR_IX_PLUS_D() {}
bool BIT_5_PTR_IY_PLUS_D() {}
bool BIT_6_A() {}
bool BIT_6_B() {}
bool BIT_6_C() {}
bool BIT_6_D() {}
bool BIT_6_E() {}
bool BIT_6_H() {}
bool BIT_6_L() {}
bool BIT_6_PTR_HL() {}
bool BIT_6_PTR_IX_PLUS_D() {}
bool BIT_6_PTR_IY_PLUS_D() {}
bool BIT_7_A() {}
bool BIT_7_B() {}
bool BIT_7_C() {}
bool BIT_7_D() {}
bool BIT_7_E() {}
bool BIT_7_H() {}
bool BIT_7_L() {}
bool BIT_7_PTR_HL() {}
bool BIT_7_PTR_IX_PLUS_D() {}
bool BIT_7_PTR_IY_PLUS_D() {}
bool RES_0_A() {}
bool RES_0_B() {}
bool RES_0_C() {}
bool RES_0_D() {}
bool RES_0_E() {}
bool RES_0_H() {}
bool RES_0_L() {}
bool RES_0_PTR_HL() {}
bool RES_0_PTR_IX_PLUS_D() {}
bool RES_0_PTR_IY_PLUS_D() {}
bool RES_1_A() {}
bool RES_1_B() {}
bool RES_1_C() {}
bool RES_1_D() {}
bool RES_1_E() {}
bool RES_1_H() {}
bool RES_1_L() {}
bool RES_1_PTR_HL() {}
bool RES_1_PTR_IX_PLUS_D() {}
bool RES_1_PTR_IY_PLUS_D() {}
bool RES_2_A() {}
bool RES_2_B() {}
bool RES_2_C() {}
bool RES_2_D() {}
bool RES_2_E() {}
bool RES_2_H() {}
bool RES_2_L() {}
bool RES_2_PTR_HL() {}
bool RES_2_PTR_IX_PLUS_D() {}
bool RES_2_PTR_IY_PLUS_D() {}
bool RES_3_A() {}
bool RES_3_B() {}
bool RES_3_C() {}
bool RES_3_D() {}
bool RES_3_E() {}
bool RES_3_H() {}
bool RES_3_L() {}
bool RES_3_PTR_HL() {}
bool RES_3_PTR_IX_PLUS_D() {}
bool RES_3_PTR_IY_PLUS_D() {}
bool RES_4_A() {}
bool RES_4_B() {}
bool RES_4_C() {}
bool RES_4_D() {}
bool RES_4_E() {}
bool RES_4_H() {}
bool RES_4_L() {}
bool RES_4_PTR_HL() {}
bool RES_4_PTR_IX_PLUS_D() {}
bool RES_4_PTR_IY_PLUS_D() {}
bool RES_5_A() {}
bool RES_5_B() {}
bool RES_5_C() {}
bool RES_5_D() {}
bool RES_5_E() {}
bool RES_5_H() {}
bool RES_5_L() {}
bool RES_5_PTR_HL() {}
bool RES_5_PTR_IX_PLUS_D() {}
bool RES_5_PTR_IY_PLUS_D() {}
bool RES_6_A() {}
bool RES_6_B() {}
bool RES_6_C() {}
bool RES_6_D() {}
bool RES_6_E() {}
bool RES_6_H() {}
bool RES_6_L() {}
bool RES_6_PTR_HL() {}
bool RES_6_PTR_IX_PLUS_D() {}
bool RES_6_PTR_IY_PLUS_D() {}
bool RES_7_A() {}
bool RES_7_B() {}
bool RES_7_C() {}
bool RES_7_D() {}
bool RES_7_E() {}
bool RES_7_H() {}
bool RES_7_L() {}
bool RES_7_PTR_HL() {}
bool RES_7_PTR_IX_PLUS_D() {}
bool RES_7_PTR_IY_PLUS_D() {}
bool SET_0_A() {}
bool SET_0_B() {}
bool SET_0_C() {}
bool SET_0_D() {}
bool SET_0_E() {}
bool SET_0_H() {}
bool SET_0_L() {}
bool SET_0_PTR_HL() {}
bool SET_0_PTR_IX_PLUS_D() {}
bool SET_0_PTR_IY_PLUS_D() {}
bool SET_1_A() {}
bool SET_1_B() {}
bool SET_1_C() {}
bool SET_1_D() {}
bool SET_1_E() {}
bool SET_1_H() {}
bool SET_1_L() {}
bool SET_1_PTR_HL() {}
bool SET_1_PTR_IX_PLUS_D() {}
bool SET_1_PTR_IY_PLUS_D() {}
bool SET_2_A() {}
bool SET_2_B() {}
bool SET_2_C() {}
bool SET_2_D() {}
bool SET_2_E() {}
bool SET_2_H() {}
bool SET_2_L() {}
bool SET_2_PTR_HL() {}
bool SET_2_PTR_IX_PLUS_D() {}
bool SET_2_PTR_IY_PLUS_D() {}
bool SET_3_A() {}
bool SET_3_B() {}
bool SET_3_C() {}
bool SET_3_D() {}
bool SET_3_E() {}
bool SET_3_H() {}
bool SET_3_L() {}
bool SET_3_PTR_HL() {}
bool SET_3_PTR_IX_PLUS_D() {}
bool SET_3_PTR_IY_PLUS_D() {}
bool SET_4_A() {}
bool SET_4_B() {}
bool SET_4_C() {}
bool SET_4_D() {}
bool SET_4_E() {}
bool SET_4_H() {}
bool SET_4_L() {}
bool SET_4_PTR_HL() {}
bool SET_4_PTR_IX_PLUS_D() {}
bool SET_4_PTR_IY_PLUS_D() {}
bool SET_5_A() {}
bool SET_5_B() {}
bool SET_5_C() {}
bool SET_5_D() {}
bool SET_5_E() {}
bool SET_5_H() {}
bool SET_5_L() {}
bool SET_5_PTR_HL() {}
bool SET_5_PTR_IX_PLUS_D() {}
bool SET_5_PTR_IY_PLUS_D() {}
bool SET_6_A() {}
bool SET_6_B() {}
bool SET_6_C() {}
bool SET_6_D() {}
bool SET_6_E() {}
bool SET_6_H() {}
bool SET_6_L() {}
bool SET_6_PTR_HL() {}
bool SET_6_PTR_IX_PLUS_D() {}
bool SET_6_PTR_IY_PLUS_D() {}
bool SET_7_A() {}
bool SET_7_B() {}
bool SET_7_C() {}
bool SET_7_D() {}
bool SET_7_E() {}
bool SET_7_H() {}
bool SET_7_L() {}
bool SET_7_PTR_HL() {}
bool SET_7_PTR_IX_PLUS_D() {}
bool SET_7_PTR_IY_PLUS_D() {}
bool JP_NN() {
  //Mnemonic:  JP nn
  //Assembled command: 0xC3 n n
  //Expected behavior: PC is set to value nn
  //Test methodology: Run a JP that targets each possible address
  //Success criteria: Verify that the address bus reads the target address

  //Initialize some variables
  bool error = false;
  uint16_t targetAddr=0;
  uint16_t curAddr = 0;

  uint16_t addr[]={0x00, 0x00, 0x00};
  char targetMnemonic[]="JP nn";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  char buffer[24];

  //Get the instruction information
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));

  strcpy_P(buffer, inst.mnemonic);
  Serial.print(F("Testing "));
  Serial.print(buffer);
  
  byte data[] = {inst.opCode, lowByte(targetAddr), highByte(targetAddr) };

  //Test enough times to ensure that we have hit every addressable value
  for (unsigned int i = 0; i < 65535; i++) {

    //This instruction requires one M1 cycle and two Read cycles
    writeSingleInstruction(data, 1, 2, 0, addr);

    //Read the address bus, delaying long enough for the address bus to settle out
    curAddr = readAddressBus(period);

    //Test if the address bus reads the expected value
    if (curAddr != targetAddr) {
      error = true;
    }

    //Display progress
    if ((i % 1000) == 0) {
      Serial.print(F("."));
    }

    //Update the target address for the next iteration
    targetAddr++;
    data[1] = lowByte(targetAddr);
    data[2] = highByte(targetAddr);
  }

  //Display status
  if (error) {
    Serial.println(F("Fail"));
  } else {
    Serial.println(F("Pass"));
  }

}
bool JP_C_NN() {}
bool JP_NC_NN() {}
bool JP_Z_NN() {}
bool JP_NZ_NN() {}
bool JP_PE_NN() {}
bool JP_PO_NN() {}
bool JP_M_NN() {}
bool JP_P_NN() {}
bool JR_E() {}
bool JR_C_E() {}
bool JR_NC_E() {}
bool JR_Z_E() {}
bool JR_NZ_E() {}
bool JP_PTR_HL() {}
bool JP_PTR_IX() {}
bool JP_PTR_IY() {}
bool CALL_NN() {}
bool CALL_C_NN() {}
bool CALL_NC_NN() {}
bool CALL_Z_NN() {}
bool CALL_NZ_NN() {}
bool CALL_PE_NN() {}
bool CALL_PO_NN() {}
bool CALL_M_NN() {}
bool CALL_P_NN() {}
bool DJNZ_E() {}
bool RET() {}
bool RET_C() {}
bool RET_NC() {}
bool RET_Z() {}
bool RET_NZ() {}
bool RET_PE() {}
bool RET_PO() {}
bool RET_M() {}
bool RET_P() {}
bool RET_I() {}
bool RET_N() {}
bool RST_0() {}
bool RST_8() {}
bool RST_16() {}
bool RST_24() {}
bool RST_32() {}
bool RST_40() {}
bool RST_48() {}
bool RST_56() {}
bool IN_A_PTR_N() {}
bool IN_A_PTR_C() {}
bool IN_B_PTR_C() {}
bool IN_C_PTR_C() {}
bool IN_D_PTR_C() {}
bool IN_E_PTR_C() {}
bool IN_H_PTR_C() {}
bool IN_L_PTR_C() {}
bool INI() {}
bool INIR() {}
bool IND() {}
bool INDR() {}
bool OUT_PTR_N_A() {}
bool OUT_PTR_C_A() {}
bool OUT_PTR_C_B() {}
bool OUT_PTR_C_C() {}
bool OUT_PTR_C_D() {}
bool OUT_PTR_C_E() {}
bool OUT_PTR_C_H() {}
bool OUT_PTR_C_L() {}
bool OUTI() {}
bool OUTIR() {}
bool OUTD() {}
bool OUTDR() {}
bool HALT_() {}
bool DI() {}
bool EI() {}
bool IM0() {}
bool IM1() {}
bool IM2() {}
