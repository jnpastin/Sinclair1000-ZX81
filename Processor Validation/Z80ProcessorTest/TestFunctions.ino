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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};
  
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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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

  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};
  byte outputData[] = {0x00, 0x00, 0x00};
  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 256; i++) {

    //Send the LD B,n command
    helper_ld_X_n('B', i);

    //Send the LD A,B command to move the data into the A register
    helper_ld_X_Y('A','B');

    //Read the data in the A register   
    for (int j=0; j<3; j++) {
      outputData[j]=0x00;
    }

    helper_ld_ptr_nn_a(outputData, addr);
   
    //Test if the Data Bus reads the expected value
    if (outputData[2] != i) {
      error = true;
      Serial.print(F("Expected: "));
      Serial.print(i,HEX);
      Serial.print(F(" got: "));
      Serial.println(outputData[2],HEX);
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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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

  char targetMnemonic[]="LD (HL),n";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));
  byte data[] = {inst.opCode, 0x00, 0x00};
  byte loadHLData[]={0x00, 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
    if (data[2] != lowByte(sentData) || addr[2] != i) {
      error=true;
      /*Serial.print(F("Expected data: "));
      Serial.print(lowByte(sentData),HEX);
      Serial.print(F(" got: "));
      Serial.print(data[2],HEX);
      Serial.print(F(" Expected address:"));
      Serial.print(i, HEX);
      Serial.print(F(" got: "));
      Serial.println(addr[2],HEX);*/

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
    if (data[2] != j || addr[2] != sentAddress) {
      error=true;
      Serial.print(F("Expected data: "));
      Serial.print(j,HEX);
      Serial.print(F(" got: "));
      Serial.print(data[2],HEX);
      Serial.print(F(" Expected address:"));
      Serial.print(sentAddress, HEX);
      Serial.print(F(" got: "));
      Serial.println(addr[2],HEX);
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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};


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
  //Assembled command: 0x7E
  //Expected behavior: A value is loaded into the memory address in the HL Register
  //Test methodology: Load Register HL with each possible address, load a random value into the memory address (HL).  Load Register HL with a random address, load each possible value into that memory address.
  //Success criteria: Verify that the returned data & address matches the loaded data & address.

  //Initialize some variables
  bool error = false;

  char targetMnemonic[]="LD A,(HL)";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));
  byte data[] = {inst.opCode, 0x00, 0x00};
  byte loadHLData[]={0x00, 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};
  uint16_t receivedAddr=0x00;


  //Test enough times to ensure that we have hit every possible address value with random data. 
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

    receivedAddr=addr[1];

    helper_ld_ptr_nn_a(data, addr);


    if (data[2] != lowByte(sentData) || receivedAddr != i) {
      error=true;
      Serial.print(F("Expected data: "));
      Serial.print(lowByte(sentData),HEX);
      Serial.print(F(" got: "));
      Serial.print(data[2],HEX);
      Serial.print(F(" Expected address: "));
      Serial.print(i, HEX);
      Serial.print(F(" got: "));
      Serial.println(receivedAddr,HEX);
    } 
    
    //Display progress
    if ((i % 1000) == 0) {
      Serial.print(F("."));
    }
    
  }
  
  //Test enough times to ensure that we have hit every possible data value with a random address. 
  for (int j=0; j<256; j++)
  {

    //Load a random address into 
    long sentAddress=random(65535);
    loadHLData[0]=lowByte(sentAddress);
    loadHLData[1]=highByte(sentAddress);
    helper_ld_XY_nn('H', 'L', loadHLData);

    data[1]=j;

    writeSingleInstruction(data, 1, 1, 0, addr);
    receivedAddr=addr[1];    
    helper_ld_ptr_nn_a(data, addr);


    if (data[2] != j || sentAddress != receivedAddr) {
      error=true;
      Serial.print(F("Expected data: "));
      Serial.print(lowByte(j),HEX);
      Serial.print(F(" got: "));
      Serial.print(data[2],HEX);
      Serial.print(F(" Expected address: "));
      Serial.print(sentAddress, HEX);
      Serial.print(F(" got: "));
      Serial.println(receivedAddr,HEX);
    } 
    
  }

  //Display status
  if (error) {
    Serial.println(F("Fail"));
  } else {
    Serial.println(F("Pass"));
  }
}
bool LD_B_PTR_HL() {
  //Mnemonic:  LD B,(HL)
  //Assembled command: 0x46
  //Expected behavior: A value is loaded into the memory address in the HL Register
  //Test methodology: Load Register HL with each possible address, load a random value into the memory address (HL).  Load Register HL with a random address, load each possible value into that memory address.
  //Success criteria: Verify that the returned data & address matches the loaded data & address.

  //Initialize some variables
  bool error = false;

  char targetMnemonic[]="LD B,(HL)";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));
  byte data[] = {inst.opCode, 0x00, 0x00};
  byte loadHLData[]={0x00, 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};
  uint16_t receivedAddr=0x00;


  //Test enough times to ensure that we have hit every possible address value with random data. 
  for (uint16_t i = 0; i < 65535; i++) {

    //Load the target address into the HL Register
    loadHLData[0]=lowByte(i);
    loadHLData[1]=highByte(i);
    helper_ld_XY_nn('H', 'L', loadHLData);

    //Generate random data
    long sentData=random(255);
    data[1]=lowByte(sentData);

    //Load that data into the B register
    writeSingleInstruction(data, 1, 1, 0, addr);
    receivedAddr=addr[1];

    helper_ld_X_Y('A', 'B');
    
    helper_ld_ptr_nn_a(data, addr);


    if (data[2] != lowByte(sentData) || receivedAddr != i) {
      error=true;
    } 
    
    //Display progress
    if ((i % 1000) == 0) {
      Serial.print(F("."));
    }
    
  }
  
  //Test enough times to ensure that we have hit every possible data value with a random address. 
  for (int j=0; j<256; j++)
  {

    //Load a random address into 
    long sentAddress=random(65535);
    loadHLData[0]=lowByte(sentAddress);
    loadHLData[1]=highByte(sentAddress);
    helper_ld_XY_nn('H', 'L', loadHLData);

    data[1]=j;

    writeSingleInstruction(data, 1, 1, 0, addr);
    
    receivedAddr=addr[1];

    helper_ld_X_Y('A', 'B');

    helper_ld_ptr_nn_a(data, addr);


    if (data[2] != j || sentAddress != receivedAddr) {
      error=true;
    }
    
  }

  //Display status
  if (error) {
    Serial.println(F("Fail"));
  } else {
    Serial.println(F("Pass"));
  }
}
bool LD_C_PTR_HL() {
    //Mnemonic:  LD C,(HL)
  //Assembled command: 0x4E
  //Expected behavior: A value is loaded into the memory address in the HL Register
  //Test methodology: Load Register HL with each possible address, load a random value into the memory address (HL).  Load Register HL with a random address, load each possible value into that memory address.
  //Success criteria: Verify that the returned data & address matches the loaded data & address.

  //Initialize some variables
  bool error = false;

  char targetMnemonic[]="LD C,(HL)";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));
  byte data[] = {inst.opCode, 0x00, 0x00};
  byte loadHLData[]={0x00, 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};
  uint16_t receivedAddr=0x00;


  //Test enough times to ensure that we have hit every possible address value with random data. 
  for (uint16_t i = 0; i < 65535; i++) {

    //Load the target address into the HL Register
    loadHLData[0]=lowByte(i);
    loadHLData[1]=highByte(i);
    helper_ld_XY_nn('H', 'L', loadHLData);

    //Generate random data
    long sentData=random(255);
    data[1]=lowByte(sentData);

    //Load that data into the B register
    writeSingleInstruction(data, 1, 1, 0, addr);
    receivedAddr=addr[1];

    helper_ld_X_Y('A', 'C');
    
    helper_ld_ptr_nn_a(data, addr);


    if (data[2] != lowByte(sentData) || receivedAddr != i) {
      error=true;
    } 
    
    //Display progress
    if ((i % 1000) == 0) {
      Serial.print(F("."));
    }
    
  }
  
  //Test enough times to ensure that we have hit every possible data value with a random address. 
  for (int j=0; j<256; j++)
  {

    //Load a random address into 
    long sentAddress=random(65535);
    loadHLData[0]=lowByte(sentAddress);
    loadHLData[1]=highByte(sentAddress);
    helper_ld_XY_nn('H', 'L', loadHLData);

    data[1]=j;

    writeSingleInstruction(data, 1, 1, 0, addr);
    
    receivedAddr=addr[1];

    helper_ld_X_Y('A', 'C');

    helper_ld_ptr_nn_a(data, addr);


    if (data[2] != j || sentAddress != receivedAddr) {
      error=true;
    }
    
  }

  //Display status
  if (error) {
    Serial.println(F("Fail"));
  } else {
    Serial.println(F("Pass"));
  }

}
bool LD_D_PTR_HL() {
    //Mnemonic:  LD D,(HL)
  //Assembled command: 0x56
  //Expected behavior: A value is loaded into the memory address in the HL Register
  //Test methodology: Load Register HL with each possible address, load a random value into the memory address (HL).  Load Register HL with a random address, load each possible value into that memory address.
  //Success criteria: Verify that the returned data & address matches the loaded data & address.

  //Initialize some variables
  bool error = false;

  char targetMnemonic[]="LD D,(HL)";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));
  byte data[] = {inst.opCode, 0x00, 0x00};
  byte loadHLData[]={0x00, 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};
  uint16_t receivedAddr=0x00;


  //Test enough times to ensure that we have hit every possible address value with random data. 
  for (uint16_t i = 0; i < 65535; i++) {

    //Load the target address into the HL Register
    loadHLData[0]=lowByte(i);
    loadHLData[1]=highByte(i);
    helper_ld_XY_nn('H', 'L', loadHLData);

    //Generate random data
    long sentData=random(255);
    data[1]=lowByte(sentData);

    //Load that data into the B register
    writeSingleInstruction(data, 1, 1, 0, addr);
    receivedAddr=addr[1];

    helper_ld_X_Y('A', 'D');
    
    helper_ld_ptr_nn_a(data, addr);


    if (data[2] != lowByte(sentData) || receivedAddr != i) {
      error=true;
    } 
    
    //Display progress
    if ((i % 1000) == 0) {
      Serial.print(F("."));
    }
    
  }
  
  //Test enough times to ensure that we have hit every possible data value with a random address. 
  for (int j=0; j<256; j++)
  {

    //Load a random address into 
    long sentAddress=random(65535);
    loadHLData[0]=lowByte(sentAddress);
    loadHLData[1]=highByte(sentAddress);
    helper_ld_XY_nn('H', 'L', loadHLData);

    data[1]=j;

    writeSingleInstruction(data, 1, 1, 0, addr);
    
    receivedAddr=addr[1];

    helper_ld_X_Y('A', 'D');

    helper_ld_ptr_nn_a(data, addr);


    if (data[2] != j || sentAddress != receivedAddr) {
      error=true;
    }
    
  }

  //Display status
  if (error) {
    Serial.println(F("Fail"));
  } else {
    Serial.println(F("Pass"));
  }

}
bool LD_E_PTR_HL() {
    //Mnemonic:  LD E,(HL)
  //Assembled command: 0x5E
  //Expected behavior: A value is loaded into the memory address in the HL Register
  //Test methodology: Load Register HL with each possible address, load a random value into the memory address (HL).  Load Register HL with a random address, load each possible value into that memory address.
  //Success criteria: Verify that the returned data & address matches the loaded data & address.

  //Initialize some variables
  bool error = false;

  char targetMnemonic[]="LD E,(HL)";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));
  byte data[] = {inst.opCode, 0x00, 0x00};
  byte loadHLData[]={0x00, 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};
  uint16_t receivedAddr=0x00;


  //Test enough times to ensure that we have hit every possible address value with random data. 
  for (uint16_t i = 0; i < 65535; i++) {

    //Load the target address into the HL Register
    loadHLData[0]=lowByte(i);
    loadHLData[1]=highByte(i);
    helper_ld_XY_nn('H', 'L', loadHLData);

    //Generate random data
    long sentData=random(255);
    data[1]=lowByte(sentData);

    //Load that data into the B register
    writeSingleInstruction(data, 1, 1, 0, addr);
    receivedAddr=addr[1];

    helper_ld_X_Y('A', 'E');
    
    helper_ld_ptr_nn_a(data, addr);


    if (data[2] != lowByte(sentData) || receivedAddr != i) {
      error=true;
    } 
    
    //Display progress
    if ((i % 1000) == 0) {
      Serial.print(F("."));
    }
    
  }
  
  //Test enough times to ensure that we have hit every possible data value with a random address. 
  for (int j=0; j<256; j++)
  {

    //Load a random address into 
    long sentAddress=random(65535);
    loadHLData[0]=lowByte(sentAddress);
    loadHLData[1]=highByte(sentAddress);
    helper_ld_XY_nn('H', 'L', loadHLData);

    data[1]=j;

    writeSingleInstruction(data, 1, 1, 0, addr);
    
    receivedAddr=addr[1];

    helper_ld_X_Y('A', 'E');

    helper_ld_ptr_nn_a(data, addr);


    if (data[2] != j || sentAddress != receivedAddr) {
      error=true;
    }
    
  }

  //Display status
  if (error) {
    Serial.println(F("Fail"));
  } else {
    Serial.println(F("Pass"));
  }
}
bool LD_H_PTR_HL() {
    //Mnemonic:  LD H,(HL)
  //Assembled command: 0x66
  //Expected behavior: A value is loaded into the memory address in the HL Register
  //Test methodology: Load Register HL with each possible address, load a random value into the memory address (HL).  Load Register HL with a random address, load each possible value into that memory address.
  //Success criteria: Verify that the returned data & address matches the loaded data & address.

  //Initialize some variables
  bool error = false;

  char targetMnemonic[]="LD H,(HL)";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));
  byte data[] = {inst.opCode, 0x00, 0x00};
  byte loadHLData[]={0x00, 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};
  uint16_t receivedAddr=0x00;


  //Test enough times to ensure that we have hit every possible address value with random data. 
  for (uint16_t i = 0; i < 65535; i++) {

    //Load the target address into the HL Register
    loadHLData[0]=lowByte(i);
    loadHLData[1]=highByte(i);
    helper_ld_XY_nn('H', 'L', loadHLData);

    //Generate random data
    long sentData=random(255);
    data[1]=lowByte(sentData);

    //Load that data into the B register
    writeSingleInstruction(data, 1, 1, 0, addr);
    receivedAddr=addr[1];

    helper_ld_X_Y('A', 'H');
    
    helper_ld_ptr_nn_a(data, addr);


    if (data[2] != lowByte(sentData) || receivedAddr != i) {
      error=true;
    } 
    
    //Display progress
    if ((i % 1000) == 0) {
      Serial.print(F("."));
    }
    
  }
  
  //Test enough times to ensure that we have hit every possible data value with a random address. 
  for (int j=0; j<256; j++)
  {

    //Load a random address into 
    long sentAddress=random(65535);
    loadHLData[0]=lowByte(sentAddress);
    loadHLData[1]=highByte(sentAddress);
    helper_ld_XY_nn('H', 'L', loadHLData);

    data[1]=j;

    writeSingleInstruction(data, 1, 1, 0, addr);
    
    receivedAddr=addr[1];

    helper_ld_X_Y('A', 'H');

    helper_ld_ptr_nn_a(data, addr);


    if (data[2] != j || sentAddress != receivedAddr) {
      error=true;
    }
    
  }

  //Display status
  if (error) {
    Serial.println(F("Fail"));
  } else {
    Serial.println(F("Pass"));
  }
}
bool LD_L_PTR_HL() {
  //Mnemonic:  LD L,(HL)
  //Assembled command: 0x6E
  //Expected behavior: A value is loaded into the memory address in the HL Register
  //Test methodology: Load Register HL with each possible address, load a random value into the memory address (HL).  Load Register HL with a random address, load each possible value into that memory address.
  //Success criteria: Verify that the returned data & address matches the loaded data & address.

  //Initialize some variables
  bool error = false;

  char targetMnemonic[]="LD L,(HL)";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));
  byte data[] = {inst.opCode, 0x00, 0x00};
  byte loadHLData[]={0x00, 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};
  uint16_t receivedAddr=0x00;


  //Test enough times to ensure that we have hit every possible address value with random data. 
  for (uint16_t i = 0; i < 65535; i++) {

    //Load the target address into the HL Register
    loadHLData[0]=lowByte(i);
    loadHLData[1]=highByte(i);
    helper_ld_XY_nn('H', 'L', loadHLData);

    //Generate random data
    long sentData=random(255);
    data[1]=lowByte(sentData);

    //Load that data into the B register
    writeSingleInstruction(data, 1, 1, 0, addr);
    receivedAddr=addr[1];

    helper_ld_X_Y('A', 'L');
    
    helper_ld_ptr_nn_a(data, addr);


    if (data[2] != lowByte(sentData) || receivedAddr != i) {
      error=true;
    } 
    
    //Display progress
    if ((i % 1000) == 0) {
      Serial.print(F("."));
    }
    
  }
  
  //Test enough times to ensure that we have hit every possible data value with a random address. 
  for (int j=0; j<256; j++)
  {

    //Load a random address into 
    long sentAddress=random(65535);
    loadHLData[0]=lowByte(sentAddress);
    loadHLData[1]=highByte(sentAddress);
    helper_ld_XY_nn('H', 'L', loadHLData);

    data[1]=j;

    writeSingleInstruction(data, 1, 1, 0, addr);
    
    receivedAddr=addr[1];

    helper_ld_X_Y('A', 'L');

    helper_ld_ptr_nn_a(data, addr);


    if (data[2] != j || sentAddress != receivedAddr) {
      error=true;
    }
    
  }

  //Display status
  if (error) {
    Serial.println(F("Fail"));
  } else {
    Serial.println(F("Pass"));
  }
}
bool LD_PTR_HL_A() {
  //Mnemonic:  LD (HL),A
  //Assembled command: 0x77
  //Expected behavior: A value is loaded into the memory address in the HL Register
  //Test methodology: Load Register HL with each possible address, load a random value into Register A.  Store the value in register A to the memory location in HL.  Load Register HL with a random address, load each possible value into that memory address via register A.
  //Success criteria: Verify that the returned data & address matches the loaded data & address.

  //Initialize some variables
  bool error = false;

  char targetMnemonic[]="LD (HL),A";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));
  byte data[] = {inst.opCode, 0x00, 0x00, 0x00};
  byte loadHLData[]={0x00, 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};
  uint16_t receivedAddr=0x00;


  //Test enough times to ensure that we have hit every possible address value with random data. 
  for (uint16_t i = 0; i < 65535; i++) {

    //Load the target address into the HL Register
    loadHLData[0]=lowByte(i);
    loadHLData[1]=highByte(i);
    helper_ld_XY_nn('H', 'L', loadHLData);

    //Generate random data
    long sentData=random(255);
    

    helper_ld_X_n('A', lowByte(sentData));

    //Load that data into the A register
    writeSingleInstruction(data, 1, 0, 1, addr);

    receivedAddr=addr[1];

    if (data[1] != lowByte(sentData) || receivedAddr != i) {
      error=true;
      Serial.print(F("Expected data: "));
      Serial.print(lowByte(sentData),HEX);
      Serial.print(F(" got: "));
      Serial.print(data[1],HEX);
      Serial.print(F(" Expected address: "));
      Serial.print(i, HEX);
      Serial.print(F(" got: "));
      Serial.println(receivedAddr,HEX);
    } 


    //Display progress
    if ((i % 1000) == 0) {
      Serial.print(F("."));
    }
    
  }
  
  //Test enough times to ensure that we have hit every possible data value with a random address. 
  for (int j=0; j<256; j++)
  {

    //Load a random address into 
    long sentAddress=random(65535);
    loadHLData[0]=lowByte(sentAddress);
    loadHLData[1]=highByte(sentAddress);
    helper_ld_XY_nn('H', 'L', loadHLData);


    helper_ld_X_n('A', j);
    writeSingleInstruction(data, 1, 0, 1, addr);
    receivedAddr=addr[1];    



    if (data[1] != j || sentAddress != receivedAddr) {
      error=true;
      Serial.print(F("Expected data: "));
      Serial.print(lowByte(j),HEX);
      Serial.print(F(" got: "));
      Serial.print(data[1],HEX);
      Serial.print(F(" Expected address: "));
      Serial.print(sentAddress, HEX);
      Serial.print(F(" got: "));
      Serial.println(receivedAddr,HEX);
    } 
    
  }

  //Display status
  if (error) {
    Serial.println(F("Fail"));
  } else {
    Serial.println(F("Pass"));
  }

}
bool LD_PTR_HL_B() {
  //Mnemonic:  LD (HL),B
  //Assembled command: 0x70
  //Expected behavior: A value is loaded into the memory address in the HL Register
  //Test methodology: Load Register HL with each possible address, load a random value into Register B.  Store the value in register B to the memory location in HL.  Load Register HL with a random address, load each possible value into that memory address via register B.
  //Success criteria: Verify that the returned data & address matches the loaded data & address.

  //Initialize some variables
  bool error = false;

  char targetMnemonic[]="LD (HL),B";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));
  byte data[] = {inst.opCode, 0x00, 0x00, 0x00};
  byte loadHLData[]={0x00, 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};
  uint16_t receivedAddr=0x00;


  //Test enough times to ensure that we have hit every possible address value with random data. 
  for (uint16_t i = 0; i < 65535; i++) {

    //Load the target address into the HL Register
    loadHLData[0]=lowByte(i);
    loadHLData[1]=highByte(i);
    helper_ld_XY_nn('H', 'L', loadHLData);

    //Generate random data
    long sentData=random(255);
    

    helper_ld_X_n('B', lowByte(sentData));

    //Load that data into the B register
    writeSingleInstruction(data, 1, 0, 1, addr);

    receivedAddr=addr[1];

    if (data[1] != lowByte(sentData) || receivedAddr != i) {
      error=true;
    } 

    //Display progress
    if ((i % 1000) == 0) {
      Serial.print(F("."));
    }
    
  }
  
  //Test enough times to ensure that we have hit every possible data value with a random address. 
  for (int j=0; j<256; j++)
  {

    //Load a random address into 
    long sentAddress=random(65535);
    loadHLData[0]=lowByte(sentAddress);
    loadHLData[1]=highByte(sentAddress);
    helper_ld_XY_nn('H', 'L', loadHLData);


    helper_ld_X_n('B', j);
    writeSingleInstruction(data, 1, 0, 1, addr);
    receivedAddr=addr[1];    



    if (data[1] != j || sentAddress != receivedAddr) {
      error=true;
    } 
    
  }

  //Display status
  if (error) {
    Serial.println(F("Fail"));
  } else {
    Serial.println(F("Pass"));
  }

}
bool LD_PTR_HL_C() {
  //Mnemonic:  LD (HL),C
  //Assembled command: 0x71
  //Expected behavior: A value is loaded into the memory address in the HL Register
  //Test methodology: Load Register HL with each possible address, load a random value into Register C.  Store the value in register C to the memory location in HL.  Load Register HL with a random address, load each possible value into that memory address via register C.
  //Success criteria: Verify that the returned data & address matches the loaded data & address.

  //Initialize some variables
  bool error = false;

  char targetMnemonic[]="LD (HL),C";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));
  byte data[] = {inst.opCode, 0x00, 0x00, 0x00};
  byte loadHLData[]={0x00, 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};
  uint16_t receivedAddr=0x00;


  //Test enough times to ensure that we have hit every possible address value with random data. 
  for (uint16_t i = 0; i < 65535; i++) {

    //Load the target address into the HL Register
    loadHLData[0]=lowByte(i);
    loadHLData[1]=highByte(i);
    helper_ld_XY_nn('H', 'L', loadHLData);

    //Generate random data
    long sentData=random(255);
    

    helper_ld_X_n('C', lowByte(sentData));

    //Load that data into the C register
    writeSingleInstruction(data, 1, 0, 1, addr);

    receivedAddr=addr[1];

    if (data[1] != lowByte(sentData) || receivedAddr != i) {
      error=true;
    } 

    //Display progress
    if ((i % 1000) == 0) {
      Serial.print(F("."));
    }
    
  }
  
  //Test enough times to ensure that we have hit every possible data value with a random address. 
  for (int j=0; j<256; j++)
  {

    //Load a random address into 
    long sentAddress=random(65535);
    loadHLData[0]=lowByte(sentAddress);
    loadHLData[1]=highByte(sentAddress);
    helper_ld_XY_nn('H', 'L', loadHLData);


    helper_ld_X_n('C', j);
    writeSingleInstruction(data, 1, 0, 1, addr);
    receivedAddr=addr[1];    



    if (data[1] != j || sentAddress != receivedAddr) {
      error=true;
    } 
    
  }

  //Display status
  if (error) {
    Serial.println(F("Fail"));
  } else {
    Serial.println(F("Pass"));
  }

}
bool LD_PTR_HL_D() {
  //Mnemonic:  LD (HL),D
  //Assembled command: 0x72
  //Expected behavior: A value is loaded into the memory address in the HL Register
  //Test methodology: Load Register HL with each possible address, load a random value into Register D.  Store the value in register D to the memory location in HL.  Load Register HL with a random address, load each possible value into that memory address via register D.
  //Success criteria: Verify that the returned data & address matches the loaded data & address.

  //Initialize some variables
  bool error = false;

  char targetMnemonic[]="LD (HL),D";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));
  byte data[] = {inst.opCode, 0x00, 0x00, 0x00};
  byte loadHLData[]={0x00, 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};
  uint16_t receivedAddr=0x00;


  //Test enough times to ensure that we have hit every possible address value with random data. 
  for (uint16_t i = 0; i < 65535; i++) {

    //Load the target address into the HL Register
    loadHLData[0]=lowByte(i);
    loadHLData[1]=highByte(i);
    helper_ld_XY_nn('H', 'L', loadHLData);

    //Generate random data
    long sentData=random(255);
    

    helper_ld_X_n('D', lowByte(sentData));

    //Load that data into the D register
    writeSingleInstruction(data, 1, 0, 1, addr);

    receivedAddr=addr[1];

    if (data[1] != lowByte(sentData) || receivedAddr != i) {
      error=true;
    } 

    //Display progress
    if ((i % 1000) == 0) {
      Serial.print(F("."));
    }
    
  }
  
  //Test enough times to ensure that we have hit every possible data value with a random address. 
  for (int j=0; j<256; j++)
  {

    //Load a random address into 
    long sentAddress=random(65535);
    loadHLData[0]=lowByte(sentAddress);
    loadHLData[1]=highByte(sentAddress);
    helper_ld_XY_nn('H', 'L', loadHLData);


    helper_ld_X_n('D', j);
    writeSingleInstruction(data, 1, 0, 1, addr);
    receivedAddr=addr[1];    



    if (data[1] != j || sentAddress != receivedAddr) {
      error=true;
    } 
    
  }

  //Display status
  if (error) {
    Serial.println(F("Fail"));
  } else {
    Serial.println(F("Pass"));
  }

}
bool LD_PTR_HL_E() {
  //Mnemonic:  LD (HL),E
  //Assembled command: 0x73
  //Expected behavior: A value is loaded into the memory address in the HL Register
  //Test methodology: Load Register HL with each possible address, load a random value into Register E.  Store the value in register E to the memory location in HL.  Load Register HL with a random address, load each possible value into that memory address via register E.
  //Success criteria: Verify that the returned data & address matches the loaded data & address.

  //Initialize some variables
  bool error = false;

  char targetMnemonic[]="LD (HL),E";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));
  byte data[] = {inst.opCode, 0x00, 0x00, 0x00};
  byte loadHLData[]={0x00, 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};
  uint16_t receivedAddr=0x00;


  //Test enough times to ensure that we have hit every possible address value with random data. 
  for (uint16_t i = 0; i < 65535; i++) {

    //Load the target address into the HL Register
    loadHLData[0]=lowByte(i);
    loadHLData[1]=highByte(i);
    helper_ld_XY_nn('H', 'L', loadHLData);

    //Generate random data
    long sentData=random(255);
    

    helper_ld_X_n('E', lowByte(sentData));

    //Load that data into the E register
    writeSingleInstruction(data, 1, 0, 1, addr);

    receivedAddr=addr[1];

    if (data[1] != lowByte(sentData) || receivedAddr != i) {
      error=true;

    } 

    //Display progress
    if ((i % 1000) == 0) {
      Serial.print(F("."));
    }
    
  }
  
  //Test enough times to ensure that we have hit every possible data value with a random address. 
  for (int j=0; j<256; j++)
  {

    //Load a random address into 
    long sentAddress=random(65535);
    loadHLData[0]=lowByte(sentAddress);
    loadHLData[1]=highByte(sentAddress);
    helper_ld_XY_nn('H', 'L', loadHLData);


    helper_ld_X_n('E', j);
    writeSingleInstruction(data, 1, 0, 1, addr);
    receivedAddr=addr[1];    



    if (data[1] != j || sentAddress != receivedAddr) {
      error=true;
    } 
    
  }

  //Display status
  if (error) {
    Serial.println(F("Fail"));
  } else {
    Serial.println(F("Pass"));
  }

}
bool LD_PTR_HL_H() {
  //Mnemonic:  LD (HL),H
  //Assembled command: 0x74
  //Expected behavior: A high byte value in the HL register into the memory address in the HL Register
  //Test methodology: Load Register HL with each possible address, store the value in register H to the memory location in HL. 
  //Success criteria: Verify that the returned data & address matches the loaded data & address.

  //Initialize some variables
  bool error = false;

  char targetMnemonic[]="LD (HL),H";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));
  byte data[] = {inst.opCode, 0x00, 0x00, 0x00};
  byte loadHLData[]={0x00, 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};
  uint16_t receivedAddr=0x00;


  //Test enough times to ensure that we have hit every possible address value
  for (uint16_t i = 0; i < 65535; i++) {

    //Load the target address into the HL Register
    loadHLData[0]=lowByte(i);
    loadHLData[1]=highByte(i);
    helper_ld_XY_nn('H', 'L', loadHLData);

    data[1]=highByte(i);

    //Load that data into the H register
    writeSingleInstruction(data, 1, 0, 1, addr);

    receivedAddr=addr[1];

    if (data[1] != highByte(i) || receivedAddr != i) {
      error=true;
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
bool LD_PTR_HL_L() {
  //Mnemonic:  LD (HL),L
  //Assembled command: 0x75
  //Expected behavior: A low byte value in the HL register into the memory address in the HL Register
  //Test methodology: Load Register HL with each possible address, store the value in register L to the memory location in HL. 
  //Success criteria: Verify that the returned data & address matches the loaded data & address.

  //Initialize some variables
  bool error = false;

  char targetMnemonic[]="LD (HL),L";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));
  byte data[] = {inst.opCode, 0x00, 0x00, 0x00};
  byte loadHLData[]={0x00, 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};
  uint16_t receivedAddr=0x00;


  //Test enough times to ensure that we have hit every possible address value
  for (uint16_t i = 0; i < 65535; i++) {

    //Load the target address into the HL Register
    loadHLData[0]=lowByte(i);
    loadHLData[1]=highByte(i);
    helper_ld_XY_nn('H', 'L', loadHLData);

    data[1]=lowByte(i);

    //Load that data into the L register
    writeSingleInstruction(data, 1, 0, 1, addr);

    receivedAddr=addr[1];

    if (data[1] != lowByte(i) || receivedAddr != i) {
      error=true;
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
bool LD_A_PTR_BC() {
  //Mnemonic:  LD A,(BC)
  //Assembled command: 0x0A
  //Expected behavior: A value is loaded into the memory address in the HL Register
  //Test methodology: Load Register HL with each possible address, load a random value into the memory address (HL).  Load Register HL with a random address, load each possible value into that memory address.
  //Success criteria: Verify that the returned data & address matches the loaded data & address.

  //Initialize some variables
  bool error = false;

  char targetMnemonic[]="LD A,(BC)";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));
  byte data[] = {inst.opCode, 0x00, 0x00};
  byte loadBCData[]={0x00, 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};
  uint16_t receivedAddr=0x00;


  //Test enough times to ensure that we have hit every possible address value with random data. 
  for (uint16_t i = 0; i < 65535; i++) {

    //Load the target address into the HL Register
    loadBCData[0]=lowByte(i);
    loadBCData[1]=highByte(i);
    helper_ld_XY_nn('B', 'C', loadBCData);

    //Generate random data
    long sentData=random(255);
    data[1]=lowByte(sentData);

    //Load that data into the A register
    writeSingleInstruction(data, 1, 1, 0, addr);

    receivedAddr=addr[1];

    helper_ld_ptr_nn_a(data, addr);


    if (data[2] != lowByte(sentData) || receivedAddr != i) {
      error=true;
      Serial.print(F("Expected data: "));
      Serial.print(lowByte(sentData),HEX);
      Serial.print(F(" got: "));
      Serial.print(data[2],HEX);
      Serial.print(F(" Expected address: "));
      Serial.print(i, HEX);
      Serial.print(F(" got: "));
      Serial.println(receivedAddr,HEX);
    } 
    
    //Display progress
    if ((i % 1000) == 0) {
      Serial.print(F("."));
    }
    
  }
  
  //Test enough times to ensure that we have hit every possible data value with a random address. 
  for (int j=0; j<256; j++)
  {

    //Load a random address into 
    long sentAddress=random(65535);
    loadBCData[0]=lowByte(sentAddress);
    loadBCData[1]=highByte(sentAddress);
    helper_ld_XY_nn('B', 'C', loadBCData);

    data[1]=j;

    writeSingleInstruction(data, 1, 1, 0, addr);
    receivedAddr=addr[1];    
    helper_ld_ptr_nn_a(data, addr);


    if (data[2] != j || sentAddress != receivedAddr) {
      error=true;
      Serial.print(F("Expected data: "));
      Serial.print(lowByte(j),HEX);
      Serial.print(F(" got: "));
      Serial.print(data[2],HEX);
      Serial.print(F(" Expected address: "));
      Serial.print(sentAddress, HEX);
      Serial.print(F(" got: "));
      Serial.println(receivedAddr,HEX);
    } 
    
  }

  //Display status
  if (error) {
    Serial.println(F("Fail"));
  } else {
    Serial.println(F("Pass"));
  }
}
bool LD_A_PTR_DE() {
  //Mnemonic:  LD A,(DE)
  //Assembled command: 0x1A
  //Expected behavior: A value is loaded into the memory address in the HL Register
  //Test methodology: Load Register HL with each possible address, load a random value into the memory address (HL).  Load Register HL with a random address, load each possible value into that memory address.
  //Success criteria: Verify that the returned data & address matches the loaded data & address.

  //Initialize some variables
  bool error = false;

  char targetMnemonic[]="LD A,(DE)";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));
  byte data[] = {inst.opCode, 0x00, 0x00};
  byte loadDEData[]={0x00, 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};
  uint16_t receivedAddr=0x00;


  //Test enough times to ensure that we have hit every possible address value with random data. 
  for (uint16_t i = 0; i < 65535; i++) {

    //Load the target address into the HL Register
    loadDEData[0]=lowByte(i);
    loadDEData[1]=highByte(i);
    helper_ld_XY_nn('D', 'E', loadDEData);

    //Generate random data
    long sentData=random(255);
    data[1]=lowByte(sentData);

    //Load that data into the A register
    writeSingleInstruction(data, 1, 1, 0, addr);

    receivedAddr=addr[1];

    helper_ld_ptr_nn_a(data, addr);


    if (data[2] != lowByte(sentData) || receivedAddr != i) {
      error=true;
      Serial.print(F("Expected data: "));
      Serial.print(lowByte(sentData),HEX);
      Serial.print(F(" got: "));
      Serial.print(data[2],HEX);
      Serial.print(F(" Expected address: "));
      Serial.print(i, HEX);
      Serial.print(F(" got: "));
      Serial.println(receivedAddr,HEX);
    } 
    
    //Display progress
    if ((i % 1000) == 0) {
      Serial.print(F("."));
    }
    
  }
  
  //Test enough times to ensure that we have hit every possible data value with a random address. 
  for (int j=0; j<256; j++)
  {

    //Load a random address into 
    long sentAddress=random(65535);
    loadDEData[0]=lowByte(sentAddress);
    loadDEData[1]=highByte(sentAddress);
    helper_ld_XY_nn('D', 'E', loadDEData);

    data[1]=j;

    writeSingleInstruction(data, 1, 1, 0, addr);
    receivedAddr=addr[1];    
    helper_ld_ptr_nn_a(data, addr);


    if (data[2] != j || sentAddress != receivedAddr) {
      error=true;
      Serial.print(F("Expected data: "));
      Serial.print(lowByte(j),HEX);
      Serial.print(F(" got: "));
      Serial.print(data[2],HEX);
      Serial.print(F(" Expected address: "));
      Serial.print(sentAddress, HEX);
      Serial.print(F(" got: "));
      Serial.println(receivedAddr,HEX);
    } 
    
  }

  //Display status
  if (error) {
    Serial.println(F("Fail"));
  } else {
    Serial.println(F("Pass"));
  }
}
bool LD_A_PTR_NN() {
  //Mnemonic:  LD A,(nn)
  //Assembled command: 0x3A
  //Expected behavior: The value in memory address nn is written to Register A
  //Test methodology: Load a random 8 bit value into Register A via a pointer to address nn, write the value of Register A to address nn verify.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte data[] = {0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};
  byte sentData=0x00;

  char targetMnemonic[]="LD A,(nn)";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 65535; i++) {

    sentData=lowByte(random(255));
    
    data[0]=inst.opCode;
    data[1]=lowByte(i);
    data[2]=highByte(i);
    data[3]=sentData;
    writeSingleInstruction(data, 1, 3, 0, addr);

    //Read the data in the A register   
    for (int j=0; j<4; j++) {
      data[j]=0x0;
    }
    data[0]=lowByte(i);
    data[1]=highByte(i);
    helper_ld_ptr_nn_a(data, addr);
  
    /*for (int j=0; j<4; j++){
      Serial.println(addr[j],HEX);
    }*/
    //Test if the Data Bus reads the expected value
    if (data[2] != sentData || addr[3] != i) {
      error = true;
      Serial.print(F("Expected data: "));
      Serial.print(sentData, HEX);
      Serial.print(F(" got: "));
      Serial.print(data[2], HEX);
      Serial.print(F(" Expected address: "));
      Serial.print(i, HEX);
      Serial.print(F(" got: "));
      Serial.println(addr[3],HEX);
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
bool LD_PTR_NN_A() {
  //Mnemonic:  LD (nn),A
  //Assembled command: 0x32
  //Expected behavior: The value in Register A is written to memory address nn
  //Test methodology: Load a random 8 bit value into Register A, write the value of Register A to address nn verify.  NOTE: This tests `LD A,n` for a random value as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte data[] = {0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};
  byte sentData=0x00;


  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 65535; i++) {

    sentData=lowByte(random(255));
    
    //Send the LD A,n command
    helper_ld_X_n('A', sentData);

    //Read the data in the A register   
    for (int j=0; j<4; j++) {
      data[j]=0x0;
    }
    data[0]=lowByte(i);
    data[1]=highByte(i);
    helper_ld_ptr_nn_a(data, addr);
  
    /*for (int j=0; j<4; j++){
      Serial.println(addr[j],HEX);
    }*/
    //Test if the Data Bus reads the expected value
    if (data[2] != sentData || addr[3] != i) {
      error = true;
      Serial.print(F("Expected data: "));
      Serial.print(sentData, HEX);
      Serial.print(F(" got: "));
      Serial.print(data[2], HEX);
      Serial.print(F(" Expected address: "));
      Serial.print(i, HEX);
      Serial.print(F(" got: "));
      Serial.println(addr[3],HEX);
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
bool LD_PTR_BC_A() {
  //Mnemonic:  LD (BC),A
  //Assembled command: 0x02
  //Expected behavior: The value in Register A is written to memory address stored in Register BC
  //Test methodology: Load a random 8 bit value into Register A, write the value of Register A to address in Register BC verify.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  char targetMnemonic[]="LD (BC),A";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));

  bool error = false;
  byte data[] = {0x00, 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};
  byte sentData=0x00;


  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 65535; i++) {

    sentData=lowByte(random(255));
    
    //Send the LD A,n command
    helper_ld_X_n('A', sentData);


    for (int j=0; j<4; j++) {
      data[j]=0x0;
    }

    //Write the address to the BC register
    data[0]=lowByte(i);
    data[1]=highByte(i);
    helper_ld_XY_nn('B', 'C', data);

    data[0]=inst.opCode;
    //Write the data in the A register to the memory address in the BC register
    writeSingleInstruction(data, 1, 0, 1, addr);


    //Test if the Data Bus reads the expected value
    if (data[1] != sentData || addr[1] != i) {
      error = true;
      Serial.print(F("Expected data: "));
      Serial.print(sentData, HEX);
      Serial.print(F(" got: "));
      Serial.print(data[1], HEX);
      Serial.print(F(" Expected address: "));
      Serial.print(i, HEX);
      Serial.print(F(" got: "));
      Serial.println(addr[1],HEX);
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
bool LD_PTR_DE_A() {
  //Mnemonic:  LD (DE),A
  //Assembled command: 0x12
  //Expected behavior: The value in Register A is written to memory address stored in Register DE
  //Test methodology: Load a random 8 bit value into Register A, write the value of Register A to address in Register DE verify.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  char targetMnemonic[]="LD (DE),A";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));

  bool error = false;
  byte data[] = {0x00, 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};
  byte sentData=0x00;


  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 65535; i++) {

    sentData=lowByte(random(255));
    
    //Send the LD A,n command
    helper_ld_X_n('A', sentData);


    for (int j=0; j<4; j++) {
      data[j]=0x0;
    }

    //Write the address to the DE register
    data[0]=lowByte(i);
    data[1]=highByte(i);
    helper_ld_XY_nn('D', 'E', data);

    data[0]=inst.opCode;
    //Write the data in the A register to the memory address in the DE register
    writeSingleInstruction(data, 1, 0, 1, addr);


    //Test if the Data Bus reads the expected value
    if (data[1] != sentData || addr[1] != i) {
      error = true;
      Serial.print(F("Expected data: "));
      Serial.print(sentData, HEX);
      Serial.print(F(" got: "));
      Serial.print(data[1], HEX);
      Serial.print(F(" Expected address: "));
      Serial.print(i, HEX);
      Serial.print(F(" got: "));
      Serial.println(addr[1],HEX);
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
bool LD_A_I() {
  //Mnemonic:  LD A,I
  //Assembled command: 0xED57
  //Expected behavior: The value in Register I is written to register A
  //Test methodology: Load a value into Register I via Register A, clear register A, load the value in Register I back into Register A.  Note: This relies on the LD I,A instruction
  //Success criteria: The value in register A matches the sent data

  

  //Initialize some variables
  char targetMnemonic[]="LD A,I";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));

  char targetMnemonic2[]="LD I,A";
  int instIndex2=findInstructionIndexByMnemonic(targetMnemonic2);
  instructionDefinitionType inst2;
  memcpy_P(&inst2, &InstructionDefinitions[instIndex2], sizeof(inst2));

  bool error = false;
  byte data[] = {0x00, 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};
  byte sentData=0x00;

  for (int i=0; i<256; i++){
    
    //Load the data pattern into the A Register
    sentData=i;
    helper_ld_X_n('A',sentData);

    //Execute the LD I,A instruction to put that info into the I registier
    data[0]=highByte(inst2.opCode);
    data[1]=lowByte(inst2.opCode);

    writeSingleInstruction(data,2,0,0,addr);

    //Clear out the A register
    helper_ld_X_n('A',0x00);

    //Load the contents of the I Register back into the A Register
    data[0]=highByte(inst.opCode);
    data[1]=lowByte(inst.opCode);

    writeSingleInstruction(data,2,0,0,addr);

    //Read the contents of the A Register
    for (int j=0; j<4;j++){
      data[j]=0x00;
    }
    helper_ld_ptr_nn_a(data, addr);

    if (data[2] != sentData){
      error=true;
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
bool LD_A_R() {
  //Mnemonic:  LD A,R
  //Assembled command: 0xED5F
  //Expected behavior: The value in Register R is written to register A
  //Test methodology: Load a value into Register R via Register A, clear register A, load the value in Register RI back into Register A.  Note: This relies on the LD R,A instruction
  //Success criteria: The value in register A matches the sent data

  

  //Initialize some variables
  char targetMnemonic[]="LD A,R";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));

  char targetMnemonic2[]="LD R,A";
  int instIndex2=findInstructionIndexByMnemonic(targetMnemonic2);
  instructionDefinitionType inst2;
  memcpy_P(&inst2, &InstructionDefinitions[instIndex2], sizeof(inst2));

  bool error = false;
  byte data[] = {0x00, 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};
  byte sentData=0x00;

  for (int i=0; i<256; i++){
    
    //Load the data pattern into the A Register
    sentData=i;
    helper_ld_X_n('A',sentData);

    //Execute the LD I,A instruction to put that info into the I registier
    data[0]=highByte(inst2.opCode);
    data[1]=lowByte(inst2.opCode);

    writeSingleInstruction(data,2,0,0,addr);

    //Clear out the A register
    helper_ld_X_n('A',0x00);

    //Load the contents of the I Register back into the A Register
    data[0]=highByte(inst.opCode);
    data[1]=lowByte(inst.opCode);

    writeSingleInstruction(data,2,0,0,addr);

    //Read the contents of the A Register
    for (int j=0; j<4;j++){
      data[j]=0x00;
    }
    helper_ld_ptr_nn_a(data, addr);

    //Handle the bit rollovers
    if (sentData>=0x7D && sentData <=0x7F){
      data[2]+=0x80;
    }
    if (sentData>=0xFD && sentData<=0xFF){
      sentData-=0x80;
    }

    //There are 3 clock cycles between the address set and the address retrieval, we expect the retrieved address to be three higher than what was sent.
    if (data[2] != sentData+3){
      error=true;
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
bool LD_I_A() {
  //Mnemonic:  LD I,A
  //Assembled command: 0xED47
  //Expected behavior: The value in Register A is written to register I
  //Test methodology: Load a value into the A register, then load the I register from the A register, Record the high byte of the refresh address, this should be the value of Register I
  //Success criteria: The value in register A matches the high byte of the refresh address

  //Initialize some variables
  char targetMnemonic[]="LD I,A";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));

  bool error = false;
  byte data[] = {0x00, 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};
  byte sentData=0x00;
  int prevVal=0;
  //Note that since testing this instruction involves working with the system during the refresh cycle, the `writeSingleInstruction` function cannot be used.  This instruction requires two M1 cycles.

  data[0]=highByte(inst.opCode);
  data[1]=lowByte(inst.opCode);

  for (int j=0; j<256; j++){
    sentData=j;
    helper_ld_X_n('A',sentData);

    for (int i=0; i<3; i++){
      //We always have to wait for the second half of a T cycle to deactivate Wait
      while (digitalReadFast(CLK)) {
        delayMicroseconds(period/2);
      }
      digitalWriteFast(WAIT, HIGH);

      //Wait for the next M1 cycle to start
      while (digitalReadFast(RD) || digitalReadFast(MREQ) || digitalReadFast(M1) || !digitalReadFast(RFSH)) {
        delayMicroseconds(1);
      }

      //Tell the CPU to wait while data is being written
      digitalWriteFast(WAIT, LOW);

      //Set the Data lines
      writeData(data[i]);

      //Tell the CPU that the data is valid
      while (digitalReadFast(CLK)) {
        delayMicroseconds(1);
      }
      digitalWriteFast(WAIT, HIGH);

      //If the clock is set to the 32MHz setting, the readAddressBus function takes longer than the refresh cycle.  There is no way to pause the refresh cycle, so we need to slow the clock
      /*if (prescaler==1){
        TCCR4B &= ~eraser;
        prescaler = 2;
        TCCR4B |= prescaler;
        prevVal=1;
      }*/

      //Wait for the refresh cycle to start
      while (digitalReadFast(RFSH)) {
        delayMicroseconds(1);
      }

      //Capture the address
      addr[i]=readAddressBus(0);

      //If we slowed the clock down, speed it back up
      /*if (prevVal == 1){
        TCCR4B &= ~eraser;
        prescaler = 1;
        TCCR4B |= prescaler;
        prevVal=0;
      }*/

      //Carry on with the normal M1 Cycle routine
      while (!digitalReadFast(RFSH)) {
        delayMicroseconds(1);
      }

      //Tell the CPU to wait while we process everything
      digitalWriteFast(WAIT, LOW);
    }

    if (highByte(addr[2]) != sentData){
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
bool LD_R_A() {
  //Mnemonic:  LD R,A
  //Assembled command: 0xED4F
  //Expected behavior: The value in Register A is written to register R
  //Test methodology: Load a value into the A register, then load the R register from the A register, Record the low byte of the refresh address, this should be the value of Register R
  //Success criteria: The value in register A matches the low byte of the refresh address

  //Initialize some variables
  char targetMnemonic[]="LD R,A";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));

  bool error = false;
  byte data[] = {0x00, 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};
  byte sentData=0x00;
  int prevVal=0;
  //Note that since testing this instruction involves working with the system during the refresh cycle, the `writeSingleInstruction` function cannot be used.  This instruction requires two M1 cycles.

  data[0]=highByte(inst.opCode);
  data[1]=lowByte(inst.opCode);

  for (int j=0; j<256; j++){
    sentData=j;
    helper_ld_X_n('A',sentData);

    for (int i=0; i<3; i++){
      //We always have to wait for the second half of a T cycle to deactivate Wait
      while (digitalReadFast(CLK)) {
        delayMicroseconds(period/2);
      }
      digitalWriteFast(WAIT, HIGH);

      //Wait for the next M1 cycle to start
      while (digitalReadFast(RD) || digitalReadFast(MREQ) || digitalReadFast(M1) || !digitalReadFast(RFSH)) {
        delayMicroseconds(1);
      }

      //Tell the CPU to wait while data is being written
      digitalWriteFast(WAIT, LOW);

      //Set the Data lines
      writeData(data[i]);

      //Tell the CPU that the data is valid
      while (digitalReadFast(CLK)) {
        delayMicroseconds(1);
      }
      digitalWriteFast(WAIT, HIGH);

      /*//If the clock is set to the 32MHz setting, the readAddressBus function takes longer than the refresh cycle.  There is no way to pause the refresh cycle, so we need to slow the clock
      if (prescaler==1){
        TCCR4B &= ~eraser;
        prescaler = 2;
        TCCR4B |= prescaler;
        prevVal=1;
      }*/

      //Wait for the refresh cycle to start
      while (digitalReadFast(RFSH)) {
        delayMicroseconds(1);
      }

      //Capture the address
      addr[i]=readAddressBus(0);

      /*//If we slowed the clock down, speed it back up
      if (prevVal == 1){
        TCCR4B &= ~eraser;
        prescaler = 1;
        TCCR4B |= prescaler;
        prevVal=0;
      }*/

      //Carry on with the normal M1 Cycle routine
      while (!digitalReadFast(RFSH)) {
        delayMicroseconds(1);
      }

      //Tell the CPU to wait while we process everything
      digitalWriteFast(WAIT, LOW);
    }

    if (lowByte(addr[2]) != sentData){
      error=true;
    }
     Serial.print(F("Expected: "));
      Serial.print(sentData,HEX);
      Serial.print(F(" got: "));
      Serial.println(lowByte(addr[2]),HEX);
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
bool LD_PTR_IX_PLUS_D_N() {
  //Mnemonic:  LD (IX+d),n
  //Assembled command: 0xDD36
  //Expected behavior: A value is loaded into the memory address in the IX Register offset by d
  //Test methodology: Load Register IX with a random value, load a random value into the memory address (IX+d), where d is every possible value in an 8 bit signed integer.  Load Register IX with a random address, load each possible value into that memory address, offset by a random value d
  //Success criteria: Verify that the returned data & address matches the loaded data & address.

  //Initialize some variables
  bool error = false;

  char targetMnemonic[]="LD (IX+d),n";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));
  byte data[] = {highByte(inst.opCode), lowByte(inst.opCode), 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


  //Test enough times to ensure that we have hit every possible address value with random data
  for (uint16_t i = 0; i < 256; i++) {

    int8_t d=i;
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'X', baseAddressBytes);

    long sentData=random(255);
    data[3]=lowByte(sentData);
    data[2]=d;

    //Load the data in the target address into the nn memory address
    writeSingleInstruction(data, 2, 2, 1, addr);

    if (data[4] != lowByte(sentData) || addr[4] != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((i % 16) == 0) {
      Serial.print(F("."));
    }
  }
  
  //Test enough times to ensure that we have hit every possible data value with a random address
  for (int j=0; j<256; j++)
  {

    int8_t d=random(255);
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD HL,nn command
    helper_ld_XY_nn('I', 'X', baseAddressBytes);

    long sentData=j;
    data[3]=lowByte(sentData);
    data[2]=d;

    //Load the data in the target address into the nn memory address
    writeSingleInstruction(data, 2, 2, 1, addr);

    if (data[4] != lowByte(sentData) || addr[4] != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((j % 16) == 0) {
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
bool LD_PTR_IY_PLUS_D_N() {
  //Mnemonic:  LD (IY+d),n
  //Assembled command: 0xFD36
  //Expected behavior: A value is loaded into the memory address in the IY Register offset by d
  //Test methodology: Load Register IY with a random value, load a random value into the memory address (IY+d), where d is every possible value in an 8 bit signed integer.  Load Register IY with a random address, load each possible value into that memory address, offset by a random value d
  //Success criteria: Verify that the returned data & address matches the loaded data & address.

  //Initialize some variables
  bool error = false;

  char targetMnemonic[]="LD (IY+d),n";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));
  byte data[] = {highByte(inst.opCode), lowByte(inst.opCode), 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


  //Test enough times to ensure that we have hit every possible address value with random data
  for (uint16_t i = 0; i < 256; i++) {

    int8_t d=i;
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'Y', baseAddressBytes);

    long sentData=random(255);
    data[3]=lowByte(sentData);
    data[2]=d;

    //Load the data in the target address into the nn memory address
    writeSingleInstruction(data, 2, 2, 1, addr);


    if (data[4] != lowByte(sentData) || addr[4] != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((i % 16) == 0) {
      Serial.print(F("."));
    }
  }
  
  //Test enough times to ensure that we have hit every possible data value with a random address
  for (int j=0; j<256; j++)
  {

    int8_t d=random(255);
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'Y', baseAddressBytes);

    long sentData=j;
    data[3]=lowByte(sentData);
    data[2]=d;

    //Load the data in the target address into the nn memory address
    writeSingleInstruction(data, 2, 2, 1, addr);

    if (data[4] != lowByte(sentData) || addr[4] != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((j % 16) == 0) {
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
bool LD_A_PTR_IX_PLUS_D() {
  //Mnemonic:  LD A,(IX+d)
  //Assembled command: 0xDD7E
  //Expected behavior: A value in the memory address in the IX Register offset by d is loaded into the A register
  //Test methodology: Load Register IX with a random value, load a random value into the memory address (IX+d), where d is every possible value in an 8 bit signed integer, load A register with the value.  Load Register IX with a random address, load each possible value into that memory address, offset by a random value d, load that value into the A register.
  //Success criteria: Verify that the returned data & address matches the loaded data & address.

  //Initialize some variables
  bool error = false;

  char targetMnemonic[]="LD A,(IX+d)";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));
  byte data[] = {highByte(inst.opCode), lowByte(inst.opCode), 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  byte rcvdData=0x00;
  uint16_t rcvdAddr=0x00;


  //Test enough times to ensure that we have hit every possible address value with random data
  for (uint16_t i = 0; i < 256; i++) {

    int8_t d=i;
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'X', baseAddressBytes);

    long sentData=random(255);
    data[3]=lowByte(sentData);
    data[2]=d;
    data[1]=lowByte(inst.opCode);
    data[0]=highByte(inst.opCode);

    //Load the data in the target address into the A Register
    writeSingleInstruction(data, 2, 2, 0, addr);

    //The address that was "read" comes back here
    rcvdAddr=addr[3];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    //Write the contents of the A register to memory
    helper_ld_ptr_nn_a(data, addr);

    //This is where we get the data back
    rcvdData=data[2];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    if (rcvdData != lowByte(sentData) || rcvdAddr != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((i % 16) == 0) {
      Serial.print(F("."));
    }
  }
  
  //Test enough times to ensure that we have hit every possible data value with a random address
  for (int j=0; j<256; j++)
  {

    int8_t d=random(255);
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'X', baseAddressBytes);

    long sentData=j;
    data[3]=lowByte(sentData);
    data[2]=d;
    data[1]=lowByte(inst.opCode);
    data[0]=highByte(inst.opCode);

    //Load the data in the target address into the A Register
    writeSingleInstruction(data, 2, 2, 0, addr);

    //The address that was "read" comes back here
    rcvdAddr=addr[3];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    //Write the contents of the A register to memory
    helper_ld_ptr_nn_a(data, addr);

    //This is where we get the data back
    rcvdData=data[2];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    if (rcvdData != lowByte(sentData) || rcvdAddr != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((j % 16) == 0) {
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
bool LD_B_PTR_IX_PLUS_D() {
  //Mnemonic:  LD B,(IX+d)
  //Assembled command: 0xDD46
  //Expected behavior: A value in the memory address in the IX Register offset by d is loaded into the A register
  //Test methodology: Load Register IX with a random value, load a random value into the memory address (IX+d), where d is every possible value in an 8 bit signed integer, load A register with the value.  Load Register IX with a random address, load each possible value into that memory address, offset by a random value d, load that value into the A register.
  //Success criteria: Verify that the returned data & address matches the loaded data & address.

  //Initialize some variables
  bool error = false;

  char targetMnemonic[]="LD B,(IX+d)";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));
  byte data[] = {highByte(inst.opCode), lowByte(inst.opCode), 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  byte rcvdData=0x00;
  uint16_t rcvdAddr=0x00;


  //Test enough times to ensure that we have hit every possible address value with random data
  for (uint16_t i = 0; i < 256; i++) {

    int8_t d=i;
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'X', baseAddressBytes);

    long sentData=random(255);
    data[3]=lowByte(sentData);
    data[2]=d;
    data[1]=lowByte(inst.opCode);
    data[0]=highByte(inst.opCode);

    //Load the data in the target address into the B Register
    writeSingleInstruction(data, 2, 2, 0, addr);

    //The address that was "read" comes back here
    rcvdAddr=addr[3];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    //Transfer the contents of the B register to the A register
    helper_ld_X_Y('A', 'B');

    //Write the contents of the A register to memory
    helper_ld_ptr_nn_a(data, addr);

    //This is where we get the data back
    rcvdData=data[2];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    if (rcvdData != lowByte(sentData) || rcvdAddr != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((i % 16) == 0) {
      Serial.print(F("."));
    }
  }
  
  //Test enough times to ensure that we have hit every possible data value with a random address
  for (int j=0; j<256; j++)
  {

    int8_t d=random(255);
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'X', baseAddressBytes);

    long sentData=j;
    data[3]=lowByte(sentData);
    data[2]=d;
    data[1]=lowByte(inst.opCode);
    data[0]=highByte(inst.opCode);

    //Load the data in the target address into the B Register
    writeSingleInstruction(data, 2, 2, 0, addr);

    //The address that was "read" comes back here
    rcvdAddr=addr[3];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    //Transfer the contents of the B register to the A register
    helper_ld_X_Y('A', 'B');

    //Write the contents of the A register to memory
    helper_ld_ptr_nn_a(data, addr);

    //This is where we get the data back
    rcvdData=data[2];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    if (rcvdData != lowByte(sentData) || rcvdAddr != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((j % 16) == 0) {
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
bool LD_C_PTR_IX_PLUS_D() {
  //Mnemonic:  LD C,(IX+d)
  //Assembled command: 0xDD4E
  //Expected behavior: A value in the memory address in the IX Register offset by d is loaded into the C register
  //Test methodology: Load Register IX with a random value, load a random value into the memory address (IX+d), where d is every possible value in an 8 bit signed integer, load C register with the value.  Load Register IX with a random address, load each possible value into that memory address, offset by a random value d, load that value into the C register.
  //Success criteria: Verify that the returned data & address matches the loaded data & address.

  //Initialize some variables
  bool error = false;

  char targetMnemonic[]="LD C,(IX+d)";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));
  byte data[] = {highByte(inst.opCode), lowByte(inst.opCode), 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  byte rcvdData=0x00;
  uint16_t rcvdAddr=0x00;


  //Test enough times to ensure that we have hit every possible address value with random data
  for (uint16_t i = 0; i < 256; i++) {

    int8_t d=i;
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'X', baseAddressBytes);

    long sentData=random(255);
    data[3]=lowByte(sentData);
    data[2]=d;
    data[1]=lowByte(inst.opCode);
    data[0]=highByte(inst.opCode);

    //Load the data in the target address into the C Register
    writeSingleInstruction(data, 2, 2, 0, addr);

    //The address that was "read" comes back here
    rcvdAddr=addr[3];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    //Transfer the contents of the C register to the A register
    helper_ld_X_Y('A', 'C');

    //Write the contents of the A register to memory
    helper_ld_ptr_nn_a(data, addr);

    //This is where we get the data back
    rcvdData=data[2];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    if (rcvdData != lowByte(sentData) || rcvdAddr != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((i % 16) == 0) {
      Serial.print(F("."));
    }
  }
  
  //Test enough times to ensure that we have hit every possible data value with a random address
  for (int j=0; j<256; j++)
  {

    int8_t d=random(255);
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'X', baseAddressBytes);

    long sentData=j;
    data[3]=lowByte(sentData);
    data[2]=d;
    data[1]=lowByte(inst.opCode);
    data[0]=highByte(inst.opCode);

    //Load the data in the target address into the C Register
    writeSingleInstruction(data, 2, 2, 0, addr);

    //The address that was "read" comes back here
    rcvdAddr=addr[3];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    //Transfer the contents of the C register to the A register
    helper_ld_X_Y('A', 'C');

    //Write the contents of the A register to memory
    helper_ld_ptr_nn_a(data, addr);

    //This is where we get the data back
    rcvdData=data[2];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    if (rcvdData != lowByte(sentData) || rcvdAddr != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((j % 16) == 0) {
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
bool LD_D_PTR_IX_PLUS_D() {
  //Mnemonic:  LD D,(IX+d)
  //Assembled command: 0xDD56
  //Expected behavior: A value in the memory address in the IX Register offset by d is loaded into the C register
  //Test methodology: Load Register IX with a random value, load a random value into the memory address (IX+d), where d is every possible value in an 8 bit signed integer, load D register with the value.  Load Register IX with a random address, load each possible value into that memory address, offset by a random value d, load that value into the D register.
  //Success criteria: Verify that the returned data & address matches the loaded data & address.

  //Initialize some variables
  bool error = false;

  char targetMnemonic[]="LD D,(IX+d)";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));
  byte data[] = {highByte(inst.opCode), lowByte(inst.opCode), 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  byte rcvdData=0x00;
  uint16_t rcvdAddr=0x00;


  //Test enough times to ensure that we have hit every possible address value with random data
  for (uint16_t i = 0; i < 256; i++) {

    int8_t d=i;
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'X', baseAddressBytes);

    long sentData=random(255);
    data[3]=lowByte(sentData);
    data[2]=d;
    data[1]=lowByte(inst.opCode);
    data[0]=highByte(inst.opCode);

    //Load the data in the target address into the D Register
    writeSingleInstruction(data, 2, 2, 0, addr);

    //The address that was "read" comes back here
    rcvdAddr=addr[3];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    //Transfer the contents of the D register to the A register
    helper_ld_X_Y('A', 'D');

    //Write the contents of the A register to memory
    helper_ld_ptr_nn_a(data, addr);

    //This is where we get the data back
    rcvdData=data[2];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    if (rcvdData != lowByte(sentData) || rcvdAddr != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((i % 16) == 0) {
      Serial.print(F("."));
    }
  }
  
  //Test enough times to ensure that we have hit every possible data value with a random address
  for (int j=0; j<256; j++)
  {

    int8_t d=random(255);
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'X', baseAddressBytes);

    long sentData=j;
    data[3]=lowByte(sentData);
    data[2]=d;
    data[1]=lowByte(inst.opCode);
    data[0]=highByte(inst.opCode);

    //Load the data in the target address into the D Register
    writeSingleInstruction(data, 2, 2, 0, addr);

    //The address that was "read" comes back here
    rcvdAddr=addr[3];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    //Transfer the contents of the D register to the A register
    helper_ld_X_Y('A', 'D');

    //Write the contents of the A register to memory
    helper_ld_ptr_nn_a(data, addr);

    //This is where we get the data back
    rcvdData=data[2];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    if (rcvdData != lowByte(sentData) || rcvdAddr != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((j % 16) == 0) {
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
bool LD_E_PTR_IX_PLUS_D() {
  //Mnemonic:  LD E,(IX+d)
  //Assembled command: 0xDD5E
  //Expected behavior: A value in the memory address in the IX Register offset by d is loaded into the E register
  //Test methodology: Load Register IX with a random value, load a random value into the memory address (IX+d), where d is every possible value in an 8 bit signed integer, load E register with the value.  Load Register IX with a random address, load each possible value into that memory address, offset by a random value d, load that value into the E register.
  //Success criteria: Verify that the returned data & address matches the loaded data & address.

  //Initialize some variables
  bool error = false;

  char targetMnemonic[]="LD E,(IX+d)";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));
  byte data[] = {highByte(inst.opCode), lowByte(inst.opCode), 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  byte rcvdData=0x00;
  uint16_t rcvdAddr=0x00;


  //Test enough times to ensure that we have hit every possible address value with random data
  for (uint16_t i = 0; i < 256; i++) {

    int8_t d=i;
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'X', baseAddressBytes);

    long sentData=random(255);
    data[3]=lowByte(sentData);
    data[2]=d;
    data[1]=lowByte(inst.opCode);
    data[0]=highByte(inst.opCode);

    //Load the data in the target address into the E Register
    writeSingleInstruction(data, 2, 2, 0, addr);

    //The address that was "read" comes back here
    rcvdAddr=addr[3];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    //Transfer the contents of the E register to the A register
    helper_ld_X_Y('A', 'E');

    //Write the contents of the A register to memory
    helper_ld_ptr_nn_a(data, addr);

    //This is where we get the data back
    rcvdData=data[2];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    if (rcvdData != lowByte(sentData) || rcvdAddr != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((i % 16) == 0) {
      Serial.print(F("."));
    }
  }
  
  //Test enough times to ensure that we have hit every possible data value with a random address
  for (int j=0; j<256; j++)
  {

    int8_t d=random(255);
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'X', baseAddressBytes);

    long sentData=j;
    data[3]=lowByte(sentData);
    data[2]=d;
    data[1]=lowByte(inst.opCode);
    data[0]=highByte(inst.opCode);

    //Load the data in the target address into the E Register
    writeSingleInstruction(data, 2, 2, 0, addr);

    //The address that was "read" comes back here
    rcvdAddr=addr[3];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    //Transfer the contents of the E register to the A register
    helper_ld_X_Y('A', 'E');

    //Write the contents of the A register to memory
    helper_ld_ptr_nn_a(data, addr);

    //This is where we get the data back
    rcvdData=data[2];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    if (rcvdData != lowByte(sentData) || rcvdAddr != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((j % 16) == 0) {
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
bool LD_H_PTR_IX_PLUS_D() {
  //Mnemonic:  LD H,(IX+d)
  //Assembled command: 0xDD66
  //Expected behavior: A value in the memory address in the IX Register offset by d is loaded into the H register
  //Test methodology: Load Register IX with a random value, load a random value into the memory address (IX+d), where d is every possible value in an 8 bit signed integer, load H register with the value.  Load Register IX with a random address, load each possible value into that memory address, offset by a random value d, load that value into the H register.
  //Success criteria: Verify that the returned data & address matches the loaded data & address.

  //Initialize some variables
  bool error = false;

  char targetMnemonic[]="LD H,(IX+d)";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));
  byte data[] = {highByte(inst.opCode), lowByte(inst.opCode), 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  byte rcvdData=0x00;
  uint16_t rcvdAddr=0x00;


  //Test enough times to ensure that we have hit every possible address value with random data
  for (uint16_t i = 0; i < 256; i++) {

    int8_t d=i;
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'X', baseAddressBytes);

    long sentData=random(255);
    data[3]=lowByte(sentData);
    data[2]=d;
    data[1]=lowByte(inst.opCode);
    data[0]=highByte(inst.opCode);

    //Load the data in the target address into the H Register
    writeSingleInstruction(data, 2, 2, 0, addr);

    //The address that was "read" comes back here
    rcvdAddr=addr[3];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    //Transfer the contents of the H register to the A register
    helper_ld_X_Y('A', 'H');

    //Write the contents of the A register to memory
    helper_ld_ptr_nn_a(data, addr);

    //This is where we get the data back
    rcvdData=data[2];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    if (rcvdData != lowByte(sentData) || rcvdAddr != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((i % 16) == 0) {
      Serial.print(F("."));
    }
  }
  
  //Test enough times to ensure that we have hit every possible data value with a random address
  for (int j=0; j<256; j++)
  {

    int8_t d=random(255);
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'X', baseAddressBytes);

    long sentData=j;
    data[3]=lowByte(sentData);
    data[2]=d;
    data[1]=lowByte(inst.opCode);
    data[0]=highByte(inst.opCode);

    //Load the data in the target address into the H Register
    writeSingleInstruction(data, 2, 2, 0, addr);

    //The address that was "read" comes back here
    rcvdAddr=addr[3];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    //Transfer the contents of the H register to the A register
    helper_ld_X_Y('A', 'H');

    //Write the contents of the A register to memory
    helper_ld_ptr_nn_a(data, addr);

    //This is where we get the data back
    rcvdData=data[2];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    if (rcvdData != lowByte(sentData) || rcvdAddr != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((j % 16) == 0) {
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
bool LD_L_PTR_IX_PLUS_D() {
  //Mnemonic:  LD L,(IX+d)
  //Assembled command: 0xDD6E
  //Expected behavior: A value in the memory address in the IX Register offset by d is loaded into the L register
  //Test methodology: Load Register IX with a random value, load a random value into the memory address (IX+d), where d is every possible value in an 8 bit signed integer, load L register with the value.  Load Register IX with a random address, load each possible value into that memory address, offset by a random value d, load that value into the L register.
  //Success criteria: Verify that the returned data & address matches the loaded data & address.

  //Initialize some variables
  bool error = false;

  char targetMnemonic[]="LD L,(IX+d)";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));
  byte data[] = {highByte(inst.opCode), lowByte(inst.opCode), 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  byte rcvdData=0x00;
  uint16_t rcvdAddr=0x00;


  //Test enough times to ensure that we have hit every possible address value with random data
  for (uint16_t i = 0; i < 256; i++) {

    int8_t d=i;
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'X', baseAddressBytes);

    long sentData=random(255);
    data[3]=lowByte(sentData);
    data[2]=d;
    data[1]=lowByte(inst.opCode);
    data[0]=highByte(inst.opCode);

    //Load the data in the target address into the L Register
    writeSingleInstruction(data, 2, 2, 0, addr);

    //The address that was "read" comes back here
    rcvdAddr=addr[3];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    //Transfer the contents of the L register to the A register
    helper_ld_X_Y('A', 'L');

    //Write the contents of the A register to memory
    helper_ld_ptr_nn_a(data, addr);

    //This is where we get the data back
    rcvdData=data[2];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    if (rcvdData != lowByte(sentData) || rcvdAddr != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((i % 16) == 0) {
      Serial.print(F("."));
    }
  }
  
  //Test enough times to ensure that we have hit every possible data value with a random address
  for (int j=0; j<256; j++)
  {

    int8_t d=random(255);
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'X', baseAddressBytes);

    long sentData=j;
    data[3]=lowByte(sentData);
    data[2]=d;
    data[1]=lowByte(inst.opCode);
    data[0]=highByte(inst.opCode);

    //Load the data in the target address into the L Register
    writeSingleInstruction(data, 2, 2, 0, addr);

    //The address that was "read" comes back here
    rcvdAddr=addr[3];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    //Transfer the contents of the L register to the A register
    helper_ld_X_Y('A', 'L');

    //Write the contents of the A register to memory
    helper_ld_ptr_nn_a(data, addr);

    //This is where we get the data back
    rcvdData=data[2];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    if (rcvdData != lowByte(sentData) || rcvdAddr != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((j % 16) == 0) {
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
bool LD_A_PTR_IY_PLUS_D() {
  //Mnemonic:  LD A,(IY+d)
  //Assembled command: 0xFF7E
  //Expected behavior: A value in the memory address in the IY Register offset by d is loaded into the A register
  //Test methodology: Load Register IY with a random value, load a random value into the memory address (IY+d), where d is every possible value in an 8 bit signed integer, load A register with the value.  Load Register IY with a random address, load each possible value into that memory address, offset by a random value d, load that value into the A register.
  //Success criteria: Verify that the returned data & address matches the loaded data & address.

  //Initialize some variables
  bool error = false;

  char targetMnemonic[]="LD A,(IY+d)";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));
  byte data[] = {highByte(inst.opCode), lowByte(inst.opCode), 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  byte rcvdData=0x00;
  uint16_t rcvdAddr=0x00;


  //Test enough times to ensure that we have hit every possible address value with random data
  for (uint16_t i = 0; i < 256; i++) {

    int8_t d=i;
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'Y', baseAddressBytes);

    long sentData=random(255);
    data[3]=lowByte(sentData);
    data[2]=d;
    data[1]=lowByte(inst.opCode);
    data[0]=highByte(inst.opCode);

    //Load the data in the target address into the A Register
    writeSingleInstruction(data, 2, 2, 0, addr);

    //The address that was "read" comes back here
    rcvdAddr=addr[3];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    //Write the contents of the A register to memory
    helper_ld_ptr_nn_a(data, addr);

    //This is where we get the data back
    rcvdData=data[2];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    if (rcvdData != lowByte(sentData) || rcvdAddr != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((i % 16) == 0) {
      Serial.print(F("."));
    }
  }
  
  //Test enough times to ensure that we have hit every possible data value with a random address
  for (int j=0; j<256; j++)
  {

    int8_t d=random(255);
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'Y', baseAddressBytes);

    long sentData=j;
    data[3]=lowByte(sentData);
    data[2]=d;
    data[1]=lowByte(inst.opCode);
    data[0]=highByte(inst.opCode);

    //Load the data in the target address into the A Register
    writeSingleInstruction(data, 2, 2, 0, addr);

    //The address that was "read" comes back here
    rcvdAddr=addr[3];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    //Write the contents of the A register to memory
    helper_ld_ptr_nn_a(data, addr);

    //This is where we get the data back
    rcvdData=data[2];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    if (rcvdData != lowByte(sentData) || rcvdAddr != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((j % 16) == 0) {
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
bool LD_B_PTR_IY_PLUS_D() {
  //Mnemonic:  LD B,(IY+d)
  //Assembled command: 0xFF46
  //Expected behavior: A value in the memory address in the IY Register offset by d is loaded into the A register
  //Test methodology: Load Register IY with a random value, load a random value into the memory address (IY+d), where d is every possible value in an 8 bit signed integer, load A register with the value.  Load Register IY with a random address, load each possible value into that memory address, offset by a random value d, load that value into the A register.
  //Success criteria: Verify that the returned data & address matches the loaded data & address.

  //Initialize some variables
  bool error = false;

  char targetMnemonic[]="LD B,(IY+d)";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));
  byte data[] = {highByte(inst.opCode), lowByte(inst.opCode), 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  byte rcvdData=0x00;
  uint16_t rcvdAddr=0x00;


  //Test enough times to ensure that we have hit every possible address value with random data
  for (uint16_t i = 0; i < 256; i++) {

    int8_t d=i;
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'Y', baseAddressBytes);

    long sentData=random(255);
    data[3]=lowByte(sentData);
    data[2]=d;
    data[1]=lowByte(inst.opCode);
    data[0]=highByte(inst.opCode);

    //Load the data in the target address into the B Register
    writeSingleInstruction(data, 2, 2, 0, addr);

    //The address that was "read" comes back here
    rcvdAddr=addr[3];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    //Transfer the contents of the B register to the A register
    helper_ld_X_Y('A', 'B');

    //Write the contents of the A register to memory
    helper_ld_ptr_nn_a(data, addr);

    //This is where we get the data back
    rcvdData=data[2];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    if (rcvdData != lowByte(sentData) || rcvdAddr != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((i % 16) == 0) {
      Serial.print(F("."));
    }
  }
  
  //Test enough times to ensure that we have hit every possible data value with a random address
  for (int j=0; j<256; j++)
  {

    int8_t d=random(255);
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'Y', baseAddressBytes);

    long sentData=j;
    data[3]=lowByte(sentData);
    data[2]=d;
    data[1]=lowByte(inst.opCode);
    data[0]=highByte(inst.opCode);

    //Load the data in the target address into the B Register
    writeSingleInstruction(data, 2, 2, 0, addr);

    //The address that was "read" comes back here
    rcvdAddr=addr[3];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    //Transfer the contents of the B register to the A register
    helper_ld_X_Y('A', 'B');

    //Write the contents of the A register to memory
    helper_ld_ptr_nn_a(data, addr);

    //This is where we get the data back
    rcvdData=data[2];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    if (rcvdData != lowByte(sentData) || rcvdAddr != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((j % 16) == 0) {
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
bool LD_C_PTR_IY_PLUS_D() {
  //Mnemonic:  LD C,(IY+d)
  //Assembled command: 0xFF4E
  //Expected behavior: A value in the memory address in the IY Register offset by d is loaded into the C register
  //Test methodology: Load Register IY with a random value, load a random value into the memory address (IY+d), where d is every possible value in an 8 bit signed integer, load C register with the value.  Load Register IY with a random address, load each possible value into that memory address, offset by a random value d, load that value into the C register.
  //Success criteria: Verify that the returned data & address matches the loaded data & address.

  //Initialize some variables
  bool error = false;

  char targetMnemonic[]="LD C,(IY+d)";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));
  byte data[] = {highByte(inst.opCode), lowByte(inst.opCode), 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  byte rcvdData=0x00;
  uint16_t rcvdAddr=0x00;


  //Test enough times to ensure that we have hit every possible address value with random data
  for (uint16_t i = 0; i < 256; i++) {

    int8_t d=i;
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'Y', baseAddressBytes);

    long sentData=random(255);
    data[3]=lowByte(sentData);
    data[2]=d;
    data[1]=lowByte(inst.opCode);
    data[0]=highByte(inst.opCode);

    //Load the data in the target address into the C Register
    writeSingleInstruction(data, 2, 2, 0, addr);

    //The address that was "read" comes back here
    rcvdAddr=addr[3];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    //Transfer the contents of the C register to the A register
    helper_ld_X_Y('A', 'C');

    //Write the contents of the A register to memory
    helper_ld_ptr_nn_a(data, addr);

    //This is where we get the data back
    rcvdData=data[2];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    if (rcvdData != lowByte(sentData) || rcvdAddr != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((i % 16) == 0) {
      Serial.print(F("."));
    }
  }
  
  //Test enough times to ensure that we have hit every possible data value with a random address
  for (int j=0; j<256; j++)
  {

    int8_t d=random(255);
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'Y', baseAddressBytes);

    long sentData=j;
    data[3]=lowByte(sentData);
    data[2]=d;
    data[1]=lowByte(inst.opCode);
    data[0]=highByte(inst.opCode);

    //Load the data in the target address into the C Register
    writeSingleInstruction(data, 2, 2, 0, addr);

    //The address that was "read" comes back here
    rcvdAddr=addr[3];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    //Transfer the contents of the C register to the A register
    helper_ld_X_Y('A', 'C');

    //Write the contents of the A register to memory
    helper_ld_ptr_nn_a(data, addr);

    //This is where we get the data back
    rcvdData=data[2];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    if (rcvdData != lowByte(sentData) || rcvdAddr != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((j % 16) == 0) {
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
bool LD_D_PTR_IY_PLUS_D() {
  //Mnemonic:  LD D,(IY+d)
  //Assembled command: 0xFF56
  //Expected behavior: A value in the memory address in the IY Register offset by d is loaded into the C register
  //Test methodology: Load Register IY with a random value, load a random value into the memory address (IY+d), where d is every possible value in an 8 bit signed integer, load D register with the value.  Load Register IY with a random address, load each possible value into that memory address, offset by a random value d, load that value into the D register.
  //Success criteria: Verify that the returned data & address matches the loaded data & address.

  //Initialize some variables
  bool error = false;

  char targetMnemonic[]="LD D,(IY+d)";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));
  byte data[] = {highByte(inst.opCode), lowByte(inst.opCode), 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  byte rcvdData=0x00;
  uint16_t rcvdAddr=0x00;


  //Test enough times to ensure that we have hit every possible address value with random data
  for (uint16_t i = 0; i < 256; i++) {

    int8_t d=i;
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'Y', baseAddressBytes);

    long sentData=random(255);
    data[3]=lowByte(sentData);
    data[2]=d;
    data[1]=lowByte(inst.opCode);
    data[0]=highByte(inst.opCode);

    //Load the data in the target address into the D Register
    writeSingleInstruction(data, 2, 2, 0, addr);

    //The address that was "read" comes back here
    rcvdAddr=addr[3];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    //Transfer the contents of the D register to the A register
    helper_ld_X_Y('A', 'D');

    //Write the contents of the A register to memory
    helper_ld_ptr_nn_a(data, addr);

    //This is where we get the data back
    rcvdData=data[2];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    if (rcvdData != lowByte(sentData) || rcvdAddr != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((i % 16) == 0) {
      Serial.print(F("."));
    }
  }
  
  //Test enough times to ensure that we have hit every possible data value with a random address
  for (int j=0; j<256; j++)
  {

    int8_t d=random(255);
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'Y', baseAddressBytes);

    long sentData=j;
    data[3]=lowByte(sentData);
    data[2]=d;
    data[1]=lowByte(inst.opCode);
    data[0]=highByte(inst.opCode);

    //Load the data in the target address into the D Register
    writeSingleInstruction(data, 2, 2, 0, addr);

    //The address that was "read" comes back here
    rcvdAddr=addr[3];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    //Transfer the contents of the D register to the A register
    helper_ld_X_Y('A', 'D');

    //Write the contents of the A register to memory
    helper_ld_ptr_nn_a(data, addr);

    //This is where we get the data back
    rcvdData=data[2];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    if (rcvdData != lowByte(sentData) || rcvdAddr != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((j % 16) == 0) {
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
bool LD_E_PTR_IY_PLUS_D() {
  //Mnemonic:  LD E,(IY+d)
  //Assembled command: 0xFF5E
  //Expected behavior: A value in the memory address in the IY Register offset by d is loaded into the E register
  //Test methodology: Load Register IY with a random value, load a random value into the memory address (IY+d), where d is every possible value in an 8 bit signed integer, load E register with the value.  Load Register IY with a random address, load each possible value into that memory address, offset by a random value d, load that value into the E register.
  //Success criteria: Verify that the returned data & address matches the loaded data & address.

  //Initialize some variables
  bool error = false;

  char targetMnemonic[]="LD E,(IY+d)";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));
  byte data[] = {highByte(inst.opCode), lowByte(inst.opCode), 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  byte rcvdData=0x00;
  uint16_t rcvdAddr=0x00;


  //Test enough times to ensure that we have hit every possible address value with random data
  for (uint16_t i = 0; i < 256; i++) {

    int8_t d=i;
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'Y', baseAddressBytes);

    long sentData=random(255);
    data[3]=lowByte(sentData);
    data[2]=d;
    data[1]=lowByte(inst.opCode);
    data[0]=highByte(inst.opCode);

    //Load the data in the target address into the E Register
    writeSingleInstruction(data, 2, 2, 0, addr);

    //The address that was "read" comes back here
    rcvdAddr=addr[3];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    //Transfer the contents of the E register to the A register
    helper_ld_X_Y('A', 'E');

    //Write the contents of the A register to memory
    helper_ld_ptr_nn_a(data, addr);

    //This is where we get the data back
    rcvdData=data[2];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    if (rcvdData != lowByte(sentData) || rcvdAddr != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((i % 16) == 0) {
      Serial.print(F("."));
    }
  }
  
  //Test enough times to ensure that we have hit every possible data value with a random address
  for (int j=0; j<256; j++)
  {

    int8_t d=random(255);
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'Y', baseAddressBytes);

    long sentData=j;
    data[3]=lowByte(sentData);
    data[2]=d;
    data[1]=lowByte(inst.opCode);
    data[0]=highByte(inst.opCode);

    //Load the data in the target address into the E Register
    writeSingleInstruction(data, 2, 2, 0, addr);

    //The address that was "read" comes back here
    rcvdAddr=addr[3];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    //Transfer the contents of the E register to the A register
    helper_ld_X_Y('A', 'E');

    //Write the contents of the A register to memory
    helper_ld_ptr_nn_a(data, addr);

    //This is where we get the data back
    rcvdData=data[2];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    if (rcvdData != lowByte(sentData) || rcvdAddr != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((j % 16) == 0) {
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
bool LD_H_PTR_IY_PLUS_D() {
  //Mnemonic:  LD H,(IY+d)
  //Assembled command: 0xFF66
  //Expected behavior: A value in the memory address in the IY Register offset by d is loaded into the H register
  //Test methodology: Load Register IY with a random value, load a random value into the memory address (IY+d), where d is every possible value in an 8 bit signed integer, load H register with the value.  Load Register IY with a random address, load each possible value into that memory address, offset by a random value d, load that value into the H register.
  //Success criteria: Verify that the returned data & address matches the loaded data & address.

  //Initialize some variables
  bool error = false;

  char targetMnemonic[]="LD H,(IY+d)";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));
  byte data[] = {highByte(inst.opCode), lowByte(inst.opCode), 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  byte rcvdData=0x00;
  uint16_t rcvdAddr=0x00;


  //Test enough times to ensure that we have hit every possible address value with random data
  for (uint16_t i = 0; i < 256; i++) {

    int8_t d=i;
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'Y', baseAddressBytes);

    long sentData=random(255);
    data[3]=lowByte(sentData);
    data[2]=d;
    data[1]=lowByte(inst.opCode);
    data[0]=highByte(inst.opCode);

    //Load the data in the target address into the H Register
    writeSingleInstruction(data, 2, 2, 0, addr);

    //The address that was "read" comes back here
    rcvdAddr=addr[3];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    //Transfer the contents of the H register to the A register
    helper_ld_X_Y('A', 'H');

    //Write the contents of the A register to memory
    helper_ld_ptr_nn_a(data, addr);

    //This is where we get the data back
    rcvdData=data[2];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    if (rcvdData != lowByte(sentData) || rcvdAddr != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((i % 16) == 0) {
      Serial.print(F("."));
    }
  }
  
  //Test enough times to ensure that we have hit every possible data value with a random address
  for (int j=0; j<256; j++)
  {

    int8_t d=random(255);
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'Y', baseAddressBytes);

    long sentData=j;
    data[3]=lowByte(sentData);
    data[2]=d;
    data[1]=lowByte(inst.opCode);
    data[0]=highByte(inst.opCode);

    //Load the data in the target address into the H Register
    writeSingleInstruction(data, 2, 2, 0, addr);

    //The address that was "read" comes back here
    rcvdAddr=addr[3];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    //Transfer the contents of the H register to the A register
    helper_ld_X_Y('A', 'H');

    //Write the contents of the A register to memory
    helper_ld_ptr_nn_a(data, addr);

    //This is where we get the data back
    rcvdData=data[2];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    if (rcvdData != lowByte(sentData) || rcvdAddr != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((j % 16) == 0) {
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
bool LD_L_PTR_IY_PLUS_D() {
  //Mnemonic:  LD L,(IY+d)
  //Assembled command: 0xFF6E
  //Expected behavior: A value in the memory address in the IY Register offset by d is loaded into the L register
  //Test methodology: Load Register IY with a random value, load a random value into the memory address (IY+d), where d is every possible value in an 8 bit signed integer, load L register with the value.  Load Register IY with a random address, load each possible value into that memory address, offset by a random value d, load that value into the L register.
  //Success criteria: Verify that the returned data & address matches the loaded data & address.

  //Initialize some variables
  bool error = false;

  char targetMnemonic[]="LD L,(IY+d)";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));
  byte data[] = {highByte(inst.opCode), lowByte(inst.opCode), 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  byte rcvdData=0x00;
  uint16_t rcvdAddr=0x00;


  //Test enough times to ensure that we have hit every possible address value with random data
  for (uint16_t i = 0; i < 256; i++) {

    int8_t d=i;
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'Y', baseAddressBytes);

    long sentData=random(255);
    data[3]=lowByte(sentData);
    data[2]=d;
    data[1]=lowByte(inst.opCode);
    data[0]=highByte(inst.opCode);

    //Load the data in the target address into the L Register
    writeSingleInstruction(data, 2, 2, 0, addr);

    //The address that was "read" comes back here
    rcvdAddr=addr[3];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    //Transfer the contents of the L register to the A register
    helper_ld_X_Y('A', 'L');

    //Write the contents of the A register to memory
    helper_ld_ptr_nn_a(data, addr);

    //This is where we get the data back
    rcvdData=data[2];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    if (rcvdData != lowByte(sentData) || rcvdAddr != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((i % 16) == 0) {
      Serial.print(F("."));
    }
  }
  
  //Test enough times to ensure that we have hit every possible data value with a random address
  for (int j=0; j<256; j++)
  {

    int8_t d=random(255);
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'Y', baseAddressBytes);

    long sentData=j;
    data[3]=lowByte(sentData);
    data[2]=d;
    data[1]=lowByte(inst.opCode);
    data[0]=highByte(inst.opCode);

    //Load the data in the target address into the L Register
    writeSingleInstruction(data, 2, 2, 0, addr);

    //The address that was "read" comes back here
    rcvdAddr=addr[3];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    //Transfer the contents of the L register to the A register
    helper_ld_X_Y('A', 'L');

    //Write the contents of the A register to memory
    helper_ld_ptr_nn_a(data, addr);

    //This is where we get the data back
    rcvdData=data[2];

    //Clear the data and address arrays to avoid weird interactions between the two functions
    for (int j=0; j<5; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    if (rcvdData != lowByte(sentData) || rcvdAddr != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((j % 16) == 0) {
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
bool LD_PTR_IX_PLUS_D_A() {
  //Mnemonic:  LD (IX+d),A
  //Assembled command: 0xDD77
  //Expected behavior: A value from Register A is loaded into the memory address in the IX Register offset by d
  //Test methodology: Load Register IX with a random value, load a random value into Register A.  Load the memory address (IX+d), where d is every possible value in an 8 bit signed integer with the value from Register A.  Load Register IX with a random address, load each possible value into register A.  Load that value into the memory address in IX, offset by a random value d
  //Success criteria: Verify that the returned data & address matches the loaded data & address.

  //Initialize some variables
  bool error = false;

  char targetMnemonic[]="LD (IX+d),A";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));
  byte data[] = {highByte(inst.opCode), lowByte(inst.opCode), 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


  //Test enough times to ensure that we have hit every possible address value with random data
  for (uint16_t i = 0; i < 256; i++) {

    int8_t d=i;
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'X', baseAddressBytes);

    long sentData=random(255);
    helper_ld_X_n('A',lowByte(sentData));

    data[2]=d;

    //Load the data in the A register into the memory address (IX+d)
    writeSingleInstruction(data, 2, 1, 1, addr);

    if (data[3] != lowByte(sentData) || addr[3] != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((i % 16) == 0) {
      Serial.print(F("."));
    }
  }
  
  //Test enough times to ensure that we have hit every possible data value with a random address
  for (int j=0; j<256; j++)
  {

    int8_t d=random(255);
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD HL,nn command
    helper_ld_XY_nn('I', 'X', baseAddressBytes);

    long sentData=j;
    helper_ld_X_n('A',lowByte(sentData));
    data[2]=d;

    //Load the data in Register A into the memory address (IX+d)
    writeSingleInstruction(data, 2, 1, 1, addr);

    if (data[3] != lowByte(sentData) || addr[3] != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((j % 16) == 0) {
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
bool LD_PTR_IX_PLUS_D_B() {
  //Mnemonic:  LD (IX+d),B
  //Assembled command: 0xDD70
  //Expected behavior: B value from Register B is loaded into the memory address in the IX Register offset by d
  //Test methodology: Load Register IX with a random value, load a random value into Register B.  Load the memory address (IX+d), where d is every possible value in an 8 bit signed integer with the value from Register B.  Load Register IX with a random address, load each possible value into register B.  Load that value into the memory address in IX, offset by a random value d
  //Success criteria: Verify that the returned data & address matches the loaded data & address.

  //Initialize some variables
  bool error = false;

  char targetMnemonic[]="LD (IX+d),B";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));
  byte data[] = {highByte(inst.opCode), lowByte(inst.opCode), 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


  //Test enough times to ensure that we have hit every possible address value with random data
  for (uint16_t i = 0; i < 256; i++) {

    int8_t d=i;
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'X', baseAddressBytes);

    long sentData=random(255);
    helper_ld_X_n('B',lowByte(sentData));

    data[2]=d;

    //Load the data in the B register into the memory address (IX+d)
    writeSingleInstruction(data, 2, 1, 1, addr);

    if (data[3] != lowByte(sentData) || addr[3] != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((i % 16) == 0) {
      Serial.print(F("."));
    }
  }
  
  //Test enough times to ensure that we have hit every possible data value with a random address
  for (int j=0; j<256; j++)
  {

    int8_t d=random(255);
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'X', baseAddressBytes);

    long sentData=j;
    helper_ld_X_n('B',lowByte(sentData));
    data[2]=d;

    //Load the data in Register B into the memory address (IX+d)
    writeSingleInstruction(data, 2, 1, 1, addr);

    if (data[3] != lowByte(sentData) || addr[3] != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((j % 16) == 0) {
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
bool LD_PTR_IX_PLUS_D_C() {
  //Mnemonic:  LD (IX+d),C
  //Assembled command: 0xDD71
  //Expected behavior: C value from Register C is loaded into the memory address in the IX Register offset by d
  //Test methodology: Load Register IX with a random value, load a random value into Register C.  Load the memory address (IX+d), where d is every possible value in an 8 bit signed integer with the value from Register C.  Load Register IX with a random address, load each possible value into register C.  Load that value into the memory address in IX, offset by a random value d
  //Success criteria: Verify that the returned data & address matches the loaded data & address.

  //Initialize some variables
  bool error = false;

  char targetMnemonic[]="LD (IX+d),C";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));
  byte data[] = {highByte(inst.opCode), lowByte(inst.opCode), 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


  //Test enough times to ensure that we have hit every possible address value with random data
  for (uint16_t i = 0; i < 256; i++) {

    int8_t d=i;
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'X', baseAddressBytes);

    long sentData=random(255);
    helper_ld_X_n('C',lowByte(sentData));

    data[2]=d;

    //Load the data in the C register into the memory address (IX+d)
    writeSingleInstruction(data, 2, 1, 1, addr);

    if (data[3] != lowByte(sentData) || addr[3] != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((i % 16) == 0) {
      Serial.print(F("."));
    }
  }
  
  //Test enough times to ensure that we have hit every possible data value with a random address
  for (int j=0; j<256; j++)
  {

    int8_t d=random(255);
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'X', baseAddressBytes);

    long sentData=j;
    helper_ld_X_n('C',lowByte(sentData));
    data[2]=d;

    //Load the data in Register C into the memory address (IX+d)
    writeSingleInstruction(data, 2, 1, 1, addr);

    if (data[3] != lowByte(sentData) || addr[3] != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((j % 16) == 0) {
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
bool LD_PTR_IX_PLUS_D_D() {
  //Mnemonic:  LD (IX+d),D
  //Assembled command: 0xDD72
  //Expected behavior: D value from Register D is loaded into the memory address in the IX Register offset by d
  //Test methodology: Load Register IX with a random value, load a random value into Register D.  Load the memory address (IX+d), where d is every possible value in an 8 bit signed integer with the value from Register D.  Load Register IX with a random address, load each possible value into register D.  Load that value into the memory address in IX, offset by a random value d
  //Success criteria: Verify that the returned data & address matches the loaded data & address.

  //Initialize some variables
  bool error = false;

  char targetMnemonic[]="LD (IX+d),D";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));
  byte data[] = {highByte(inst.opCode), lowByte(inst.opCode), 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


  //Test enough times to ensure that we have hit every possible address value with random data
  for (uint16_t i = 0; i < 256; i++) {

    int8_t d=i;
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'X', baseAddressBytes);

    long sentData=random(255);
    helper_ld_X_n('D',lowByte(sentData));

    data[2]=d;

    //Load the data in the D register into the memory address (IX+d)
    writeSingleInstruction(data, 2, 1, 1, addr);

    if (data[3] != lowByte(sentData) || addr[3] != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((i % 16) == 0) {
      Serial.print(F("."));
    }
  }
  
  //Test enough times to ensure that we have hit every possible data value with a random address
  for (int j=0; j<256; j++)
  {

    int8_t d=random(255);
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'X', baseAddressBytes);

    long sentData=j;
    helper_ld_X_n('D',lowByte(sentData));
    data[2]=d;

    //Load the data in Register D into the memory address (IX+d)
    writeSingleInstruction(data, 2, 1, 1, addr);

    if (data[3] != lowByte(sentData) || addr[3] != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((j % 16) == 0) {
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
bool LD_PTR_IX_PLUS_D_E() {
  //Mnemonic:  LD (IX+d),E
  //Assembled command: 0xDD73
  //Expected behavior: E value from Register E is loaded into the memory address in the IX Register offset by d
  //Test methodology: Load Register IX with a random value, load a random value into Register E.  Load the memory address (IX+d), where d is every possible value in an 8 bit signed integer with the value from Register E.  Load Register IX with a random address, load each possible value into register E.  Load that value into the memory address in IX, offset by a random value d
  //Success criteria: Verify that the returned data & address matches the loaded data & address.

  //Initialize some variables
  bool error = false;

  char targetMnemonic[]="LD (IX+d),E";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));
  byte data[] = {highByte(inst.opCode), lowByte(inst.opCode), 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


  //Test enough times to ensure that we have hit every possible address value with random data
  for (uint16_t i = 0; i < 256; i++) {

    int8_t d=i;
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'X', baseAddressBytes);

    long sentData=random(255);
    helper_ld_X_n('E',lowByte(sentData));

    data[2]=d;

    //Load the data in the E register into the memory address (IX+d)
    writeSingleInstruction(data, 2, 1, 1, addr);

    if (data[3] != lowByte(sentData) || addr[3] != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((i % 16) == 0) {
      Serial.print(F("."));
    }
  }
  
  //Test enough times to ensure that we have hit every possible data value with a random address
  for (int j=0; j<256; j++)
  {

    int8_t d=random(255);
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'X', baseAddressBytes);

    long sentData=j;
    helper_ld_X_n('E',lowByte(sentData));
    data[2]=d;

    //Load the data in Register E into the memory address (IX+d)
    writeSingleInstruction(data, 2, 1, 1, addr);

    if (data[3] != lowByte(sentData) || addr[3] != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((j % 16) == 0) {
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
bool LD_PTR_IX_PLUS_D_H() {
  //Mnemonic:  LD (IX+d),H
  //Assembled command: 0xDD74
  //Expected behavior: H value from Register H is loaded into the memory address in the IX Register offset by d
  //Test methodology: Load Register IX with a random value, load a random value into Register H.  Load the memory address (IX+d), where d is every possible value in an 8 bit signed integer with the value from Register H.  Load Register IX with a random address, load each possible value into register H.  Load that value into the memory address in IX, offset by a random value d
  //Success criteria: Verify that the returned data & address matches the loaded data & address.

  //Initialize some variables
  bool error = false;

  char targetMnemonic[]="LD (IX+d),H";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));
  byte data[] = {highByte(inst.opCode), lowByte(inst.opCode), 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


  //Test enough times to ensure that we have hit every possible address value with random data
  for (uint16_t i = 0; i < 256; i++) {

    int8_t d=i;
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'X', baseAddressBytes);

    long sentData=random(255);
    helper_ld_X_n('H',lowByte(sentData));

    data[2]=d;

    //Load the data in the H register into the memory address (IX+d)
    writeSingleInstruction(data, 2, 1, 1, addr);

    if (data[3] != lowByte(sentData) || addr[3] != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((i % 16) == 0) {
      Serial.print(F("."));
    }
  }
  
  //Test enough times to ensure that we have hit every possible data value with a random address
  for (int j=0; j<256; j++)
  {

    int8_t d=random(255);
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'X', baseAddressBytes);

    long sentData=j;
    helper_ld_X_n('H',lowByte(sentData));
    data[2]=d;

    //Load the data in Register H into the memory address (IX+d)
    writeSingleInstruction(data, 2, 1, 1, addr);

    if (data[3] != lowByte(sentData) || addr[3] != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((j % 16) == 0) {
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
bool LD_PTR_IX_PLUS_D_L() {
  //Mnemonic:  LD (IX+d),L
  //Assembled command: 0xDD75
  //Expected behavior: L value from Register L is loaded into the memory address in the IX Register offset by d
  //Test methodology: Load Register IX with a random value, load a random value into Register L.  Load the memory address (IX+d), where d is every possible value in an 8 bit signed integer with the value from Register L.  Load Register IX with a random address, load each possible value into register L.  Load that value into the memory address in IX, offset by a random value d
  //Success criteria: Verify that the returned data & address matches the loaded data & address.

  //Initialize some variables
  bool error = false;

  char targetMnemonic[]="LD (IX+d),L";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));
  byte data[] = {highByte(inst.opCode), lowByte(inst.opCode), 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


  //Test enough times to ensure that we have hit every possible address value with random data
  for (uint16_t i = 0; i < 256; i++) {

    int8_t d=i;
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'X', baseAddressBytes);

    long sentData=random(255);
    helper_ld_X_n('L',lowByte(sentData));

    data[2]=d;

    //Load the data in the L register into the memory address (IX+d)
    writeSingleInstruction(data, 2, 1, 1, addr);

    if (data[3] != lowByte(sentData) || addr[3] != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((i % 16) == 0) {
      Serial.print(F("."));
    }
  }
  
  //Test enough times to ensure that we have hit every possible data value with a random address
  for (int j=0; j<256; j++)
  {

    int8_t d=random(255);
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'X', baseAddressBytes);

    long sentData=j;
    helper_ld_X_n('L',lowByte(sentData));
    data[2]=d;

    //Load the data in Register L into the memory address (IX+d)
    writeSingleInstruction(data, 2, 1, 1, addr);

    if (data[3] != lowByte(sentData) || addr[3] != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((j % 16) == 0) {
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
bool LD_PTR_IY_PLUS_D_A() {
  //Mnemonic:  LD (IY+d),A
  //Assembled command: 0xFD77
  //Expected behavior: A value from Register A is loaded into the memory address in the IY Register offset by d
  //Test methodology: Load Register IY with a random value, load a random value into Register A.  Load the memory address (IY+d), where d is every possible value in an 8 bit signed integer with the value from Register A.  Load Register IY with a random address, load each possible value into register A.  Load that value into the memory address in IY, offset by a random value d
  //Success criteria: Verify that the returned data & address matches the loaded data & address.

  //Initialize some variables
  bool error = false;

  char targetMnemonic[]="LD (IY+d),A";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));
  byte data[] = {highByte(inst.opCode), lowByte(inst.opCode), 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


  //Test enough times to ensure that we have hit every possible address value with random data
  for (uint16_t i = 0; i < 256; i++) {

    int8_t d=i;
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'Y', baseAddressBytes);

    long sentData=random(255);
    helper_ld_X_n('A',lowByte(sentData));

    data[2]=d;

    //Load the data in the A register into the memory address (IY+d)
    writeSingleInstruction(data, 2, 1, 1, addr);

    if (data[3] != lowByte(sentData) || addr[3] != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((i % 16) == 0) {
      Serial.print(F("."));
    }
  }
  
  //Test enough times to ensure that we have hit every possible data value with a random address
  for (int j=0; j<256; j++)
  {

    int8_t d=random(255);
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD HL,nn command
    helper_ld_XY_nn('I', 'Y', baseAddressBytes);

    long sentData=j;
    helper_ld_X_n('A',lowByte(sentData));
    data[2]=d;

    //Load the data in Register A into the memory address (IY+d)
    writeSingleInstruction(data, 2, 1, 1, addr);

    if (data[3] != lowByte(sentData) || addr[3] != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((j % 16) == 0) {
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
bool LD_PTR_IY_PLUS_D_B() {
  //Mnemonic:  LD (IY+d),B
  //Assembled command: 0xFD70
  //Expected behavior: B value from Register B is loaded into the memory address in the IY Register offset by d
  //Test methodology: Load Register IY with a random value, load a random value into Register B.  Load the memory address (IY+d), where d is every possible value in an 8 bit signed integer with the value from Register B.  Load Register IY with a random address, load each possible value into register B.  Load that value into the memory address in IY, offset by a random value d
  //Success criteria: Verify that the returned data & address matches the loaded data & address.

  //Initialize some variables
  bool error = false;

  char targetMnemonic[]="LD (IY+d),B";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));
  byte data[] = {highByte(inst.opCode), lowByte(inst.opCode), 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


  //Test enough times to ensure that we have hit every possible address value with random data
  for (uint16_t i = 0; i < 256; i++) {

    int8_t d=i;
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'Y', baseAddressBytes);

    long sentData=random(255);
    helper_ld_X_n('B',lowByte(sentData));

    data[2]=d;

    //Load the data in the B register into the memory address (IY+d)
    writeSingleInstruction(data, 2, 1, 1, addr);

    if (data[3] != lowByte(sentData) || addr[3] != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((i % 16) == 0) {
      Serial.print(F("."));
    }
  }
  
  //Test enough times to ensure that we have hit every possible data value with a random address
  for (int j=0; j<256; j++)
  {

    int8_t d=random(255);
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'Y', baseAddressBytes);

    long sentData=j;
    helper_ld_X_n('B',lowByte(sentData));
    data[2]=d;

    //Load the data in Register B into the memory address (IY+d)
    writeSingleInstruction(data, 2, 1, 1, addr);

    if (data[3] != lowByte(sentData) || addr[3] != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((j % 16) == 0) {
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
bool LD_PTR_IY_PLUS_D_C() {
  //Mnemonic:  LD (IY+d),C
  //Assembled command: 0xFD71
  //Expected behavior: C value from Register C is loaded into the memory address in the IY Register offset by d
  //Test methodology: Load Register IY with a random value, load a random value into Register C.  Load the memory address (IY+d), where d is every possible value in an 8 bit signed integer with the value from Register C.  Load Register IY with a random address, load each possible value into register C.  Load that value into the memory address in IY, offset by a random value d
  //Success criteria: Verify that the returned data & address matches the loaded data & address.

  //Initialize some variables
  bool error = false;

  char targetMnemonic[]="LD (IY+d),C";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));
  byte data[] = {highByte(inst.opCode), lowByte(inst.opCode), 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


  //Test enough times to ensure that we have hit every possible address value with random data
  for (uint16_t i = 0; i < 256; i++) {

    int8_t d=i;
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'Y', baseAddressBytes);

    long sentData=random(255);
    helper_ld_X_n('C',lowByte(sentData));

    data[2]=d;

    //Load the data in the C register into the memory address (IY+d)
    writeSingleInstruction(data, 2, 1, 1, addr);

    if (data[3] != lowByte(sentData) || addr[3] != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((i % 16) == 0) {
      Serial.print(F("."));
    }
  }
  
  //Test enough times to ensure that we have hit every possible data value with a random address
  for (int j=0; j<256; j++)
  {

    int8_t d=random(255);
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'Y', baseAddressBytes);

    long sentData=j;
    helper_ld_X_n('C',lowByte(sentData));
    data[2]=d;

    //Load the data in Register C into the memory address (IY+d)
    writeSingleInstruction(data, 2, 1, 1, addr);

    if (data[3] != lowByte(sentData) || addr[3] != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((j % 16) == 0) {
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
bool LD_PTR_IY_PLUS_D_D() {
  //Mnemonic:  LD (IY+d),D
  //Assembled command: 0xFD72
  //Expected behavior: D value from Register D is loaded into the memory address in the IY Register offset by d
  //Test methodology: Load Register IY with a random value, load a random value into Register D.  Load the memory address (IY+d), where d is every possible value in an 8 bit signed integer with the value from Register D.  Load Register IY with a random address, load each possible value into register D.  Load that value into the memory address in IY, offset by a random value d
  //Success criteria: Verify that the returned data & address matches the loaded data & address.

  //Initialize some variables
  bool error = false;

  char targetMnemonic[]="LD (IY+d),D";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));
  byte data[] = {highByte(inst.opCode), lowByte(inst.opCode), 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


  //Test enough times to ensure that we have hit every possible address value with random data
  for (uint16_t i = 0; i < 256; i++) {

    int8_t d=i;
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'Y', baseAddressBytes);

    long sentData=random(255);
    helper_ld_X_n('D',lowByte(sentData));

    data[2]=d;

    //Load the data in the D register into the memory address (IY+d)
    writeSingleInstruction(data, 2, 1, 1, addr);

    if (data[3] != lowByte(sentData) || addr[3] != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((i % 16) == 0) {
      Serial.print(F("."));
    }
  }
  
  //Test enough times to ensure that we have hit every possible data value with a random address
  for (int j=0; j<256; j++)
  {

    int8_t d=random(255);
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'Y', baseAddressBytes);

    long sentData=j;
    helper_ld_X_n('D',lowByte(sentData));
    data[2]=d;

    //Load the data in Register D into the memory address (IY+d)
    writeSingleInstruction(data, 2, 1, 1, addr);

    if (data[3] != lowByte(sentData) || addr[3] != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((j % 16) == 0) {
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
bool LD_PTR_IY_PLUS_D_E() {
  //Mnemonic:  LD (IY+d),E
  //Assembled command: 0xFD73
  //Expected behavior: E value from Register E is loaded into the memory address in the IY Register offset by d
  //Test methodology: Load Register IY with a random value, load a random value into Register E.  Load the memory address (IY+d), where d is every possible value in an 8 bit signed integer with the value from Register E.  Load Register IY with a random address, load each possible value into register E.  Load that value into the memory address in IY, offset by a random value d
  //Success criteria: Verify that the returned data & address matches the loaded data & address.

  //Initialize some variables
  bool error = false;

  char targetMnemonic[]="LD (IY+d),E";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));
  byte data[] = {highByte(inst.opCode), lowByte(inst.opCode), 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


  //Test enough times to ensure that we have hit every possible address value with random data
  for (uint16_t i = 0; i < 256; i++) {

    int8_t d=i;
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'Y', baseAddressBytes);

    long sentData=random(255);
    helper_ld_X_n('E',lowByte(sentData));

    data[2]=d;

    //Load the data in the E register into the memory address (IY+d)
    writeSingleInstruction(data, 2, 1, 1, addr);

    if (data[3] != lowByte(sentData) || addr[3] != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((i % 16) == 0) {
      Serial.print(F("."));
    }
  }
  
  //Test enough times to ensure that we have hit every possible data value with a random address
  for (int j=0; j<256; j++)
  {

    int8_t d=random(255);
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'Y', baseAddressBytes);

    long sentData=j;
    helper_ld_X_n('E',lowByte(sentData));
    data[2]=d;

    //Load the data in Register E into the memory address (IY+d)
    writeSingleInstruction(data, 2, 1, 1, addr);

    if (data[3] != lowByte(sentData) || addr[3] != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((j % 16) == 0) {
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
bool LD_PTR_IY_PLUS_D_H() {
  //Mnemonic:  LD (IY+d),H
  //Assembled command: 0xFD74
  //Expected behavior: H value from Register H is loaded into the memory address in the IY Register offset by d
  //Test methodology: Load Register IY with a random value, load a random value into Register H.  Load the memory address (IY+d), where d is every possible value in an 8 bit signed integer with the value from Register H.  Load Register IY with a random address, load each possible value into register H.  Load that value into the memory address in IY, offset by a random value d
  //Success criteria: Verify that the returned data & address matches the loaded data & address.

  //Initialize some variables
  bool error = false;

  char targetMnemonic[]="LD (IY+d),H";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));
  byte data[] = {highByte(inst.opCode), lowByte(inst.opCode), 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


  //Test enough times to ensure that we have hit every possible address value with random data
  for (uint16_t i = 0; i < 256; i++) {

    int8_t d=i;
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'Y', baseAddressBytes);

    long sentData=random(255);
    helper_ld_X_n('H',lowByte(sentData));

    data[2]=d;

    //Load the data in the H register into the memory address (IY+d)
    writeSingleInstruction(data, 2, 1, 1, addr);

    if (data[3] != lowByte(sentData) || addr[3] != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((i % 16) == 0) {
      Serial.print(F("."));
    }
  }
  
  //Test enough times to ensure that we have hit every possible data value with a random address
  for (int j=0; j<256; j++)
  {

    int8_t d=random(255);
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'Y', baseAddressBytes);

    long sentData=j;
    helper_ld_X_n('H',lowByte(sentData));
    data[2]=d;

    //Load the data in Register H into the memory address (IY+d)
    writeSingleInstruction(data, 2, 1, 1, addr);

    if (data[3] != lowByte(sentData) || addr[3] != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((j % 16) == 0) {
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
bool LD_PTR_IY_PLUS_D_L() {
  //Mnemonic:  LD (IY+d),L
  //Assembled command: 0xFD75
  //Expected behavior: L value from Register L is loaded into the memory address in the IY Register offset by d
  //Test methodology: Load Register IY with a random value, load a random value into Register L.  Load the memory address (IY+d), where d is every possible value in an 8 bit signed integer with the value from Register L.  Load Register IY with a random address, load each possible value into register L.  Load that value into the memory address in IY, offset by a random value d
  //Success criteria: Verify that the returned data & address matches the loaded data & address.

  //Initialize some variables
  bool error = false;

  char targetMnemonic[]="LD (IY+d),L";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));
  byte data[] = {highByte(inst.opCode), lowByte(inst.opCode), 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


  //Test enough times to ensure that we have hit every possible address value with random data
  for (uint16_t i = 0; i < 256; i++) {

    int8_t d=i;
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'Y', baseAddressBytes);

    long sentData=random(255);
    helper_ld_X_n('L',lowByte(sentData));

    data[2]=d;

    //Load the data in the L register into the memory address (IY+d)
    writeSingleInstruction(data, 2, 1, 1, addr);

    if (data[3] != lowByte(sentData) || addr[3] != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((i % 16) == 0) {
      Serial.print(F("."));
    }
  }
  
  //Test enough times to ensure that we have hit every possible data value with a random address
  for (int j=0; j<256; j++)
  {

    int8_t d=random(255);
    uint16_t baseAddr=random(65535);
    byte baseAddressBytes[] = {lowByte(baseAddr), highByte(baseAddr)};
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'Y', baseAddressBytes);

    long sentData=j;
    helper_ld_X_n('L',lowByte(sentData));
    data[2]=d;

    //Load the data in Register L into the memory address (IY+d)
    writeSingleInstruction(data, 2, 1, 1, addr);

    if (data[3] != lowByte(sentData) || addr[3] != baseAddr+d) {
      error=true;
    } 
    
    //Display progress
    if ((j % 16) == 0) {
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
bool LD_BC_NN() {
  //Mnemonic:  LD BC,nn
  //Assembled command: 0x01
  //Expected behavior: A value is loaded into Register BC
  //Test methodology: Load all possible 16 bit values into Register BC, retrieve the value of Register BC to verify.  NOTE: This tests `LD (nn),BC` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte data[] = {0x00, 0x00, 0x00, 0x00};


  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 65535; i++) {

    data[0]=lowByte(i);
    data[1]=highByte(i);
    //Send the LD A,n command
    helper_ld_XY_nn('B', 'C', data);

    //Read the data in the HL register   
    for (int j=0; j<4; j++) {
      data[j]=0x0;
    }
    helper_ld_ptr_nn_XY('B', 'C', data);

   
    //Test if the Data Bus reads the expected value
    if (data[2] != lowByte(i) || data[3] != highByte(i)) {
      error = true;
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
bool LD_DE_NN() {
  //Mnemonic:  LD DE,nn
  //Assembled command: 0x11
  //Expected behavior: A value is loaded into Register DE
  //Test methodology: Load all possible 16 bit values into Register DE, retrieve the value of Register DE to verify.  NOTE: This tests `LD (nn),DE` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte data[] = {0x00, 0x00, 0x00, 0x00};


  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 65535; i++) {

    data[0]=lowByte(i);
    data[1]=highByte(i);
    //Send the LD A,n command
    helper_ld_XY_nn('D', 'E', data);

    //Read the data in the HL register   
    for (int j=0; j<4; j++) {
      data[j]=0x0;
    }
    helper_ld_ptr_nn_XY('D', 'E', data);

   
    //Test if the Data Bus reads the expected value
    if (data[2] != lowByte(i) || data[3] != highByte(i)) {
      error = true;
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
bool LD_HL_NN() {
  //Mnemonic:  LD HL,nn
  //Assembled command: 0x21
  //Expected behavior: A value is loaded into Register HL
  //Test methodology: Load all possible 16 bit values into Register HL, retrieve the value of Register HL to verify.  NOTE: This tests `LD (nn),HL` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte data[] = {0x00, 0x00, 0x00, 0x00};


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
    helper_ld_ptr_nn_XY('H', 'L', data);

   
    //Test if the Data Bus reads the expected value
    if (data[2] != lowByte(i) || data[3] != highByte(i)) {
      error = true;
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
bool LD_SP_NN() {
  //Mnemonic:  LD SP,nn
  //Assembled command: 0x31
  //Expected behavior: A value is loaded into Register SP
  //Test methodology: Load all possible 16 bit values into Register SP, retrieve the value of Register SP to verify.  NOTE: This tests `LD (nn),SP` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte data[] = {0x00, 0x00, 0x00, 0x00};


  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 65535; i++) {

    data[0]=lowByte(i);
    data[1]=highByte(i);
    //Send the LD A,n command
    helper_ld_XY_nn('S', 'P', data);

    //Read the data in the HL register   
    for (int j=0; j<4; j++) {
      data[j]=0x0;
    }
    helper_ld_ptr_nn_XY('S', 'P', data);

   
    //Test if the Data Bus reads the expected value
    if (data[2] != lowByte(i) || data[3] != highByte(i)) {
      error = true;
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
bool LD_IX_NN() {
  //Mnemonic:  LD IX,nn
  //Assembled command: 0xDD21
  //Expected behavior: A value is loaded into Register IX
  //Test methodology: Load all possible 16 bit values into Register IX, retrieve the value of Register IX to verify.  NOTE: This tests `LD (nn),IX` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte data[] = {0x00, 0x00, 0x00, 0x00};


  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 65535; i++) {

    data[0]=lowByte(i);
    data[1]=highByte(i);
    //Send the LD A,n command
    helper_ld_XY_nn('I', 'X', data);

    //Read the data in the HL register   
    for (int j=0; j<4; j++) {
      data[j]=0x0;
    }
    helper_ld_ptr_nn_XY('I', 'X', data);

   
    //Test if the Data Bus reads the expected value
    if (data[2] != lowByte(i) || data[3] != highByte(i)) {
      error = true;
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
bool LD_IY_NN() {
  //Mnemonic:  LD IY,nn
  //Assembled command: 0xDD21
  //Expected behavior: A value is loaded into Register IY
  //Test methodology: Load all possible 16 bit values into Register IY, retrieve the value of Register IY to verify.  NOTE: This tests `LD (nn),IY` for a single address as well.
  //Success criteria: Verify that the returned data matches the loaded data.

  //Initialize some variables
  bool error = false;
  byte data[] = {0x00, 0x00, 0x00, 0x00};


  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 65535; i++) {

    data[0]=lowByte(i);
    data[1]=highByte(i);
    //Send the LD A,n command
    helper_ld_XY_nn('I', 'Y', data);

    //Read the data in the HL register   
    for (int j=0; j<4; j++) {
      data[j]=0x0;
    }
    helper_ld_ptr_nn_XY('I', 'Y', data);

   
    //Test if the Data Bus reads the expected value
    if (data[2] != lowByte(i) || data[3] != highByte(i)) {
      error = true;
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
bool LD_BC_PTR_NN() {
  //Mnemonic:  LD BC,(nn)
  //Assembled command: 0xED4B
  //Expected behavior: A value is loaded into Register BC from memory address (nn)
  //Test methodology: Load a random value into Register BC.  The value should be "read" from memory address (nn), repeat for all memory addresses.  Secondly, load each possible value into BC from a random memory address (nn).
  //Success criteria: Verify that the returned data matches the loaded data and that the source address matches (nn).

  //Initialize some variables
  bool error = false;
  byte data[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  uint16_t sentData=0x00;
  uint16_t rcvdData=0x00;
  uint16_t sentAddr=0x00;
  uint16_t rcvdAddr=0x00;

  char targetMnemonic[]="LD BC,(nn)";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));

  Serial.print(F(" (this will take several minutes)"));
  //Test enough times to ensure that we have hit every possible address value
  for (unsigned int i = 0; i < 65535; i++) {

    sentData=random(65535);

    data[0]=highByte(inst.opCode);
    data[1]=lowByte(inst.opCode);
    data[2]=lowByte(i);
    data[3]=highByte(i);
    data[4]=lowByte(sentData);
    data[5]=highByte(sentData);

    writeSingleInstruction(data, 2, 4, 0, addr);
    rcvdAddr=addr[4];
    
    helper_ld_ptr_nn_XY('B', 'C', data);

    //Test if the Data Bus and the written address read the expected value
    if (data[2] != lowByte(sentData) || data[3] != highByte(sentData) || rcvdAddr != i) {
      error = true;
    }

    //Display progress
    if ((i % 2000) == 0) {
      Serial.print(F("."));
    }
  }


  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 65535; i++) {

    sentAddr=random(65535);

    data[0]=highByte(inst.opCode);
    data[1]=lowByte(inst.opCode);
    data[2]=lowByte(sentAddr);
    data[3]=highByte(sentAddr);
    data[4]=lowByte(i);
    data[5]=highByte(i);

    writeSingleInstruction(data, 2, 4, 0, addr);
    rcvdAddr=addr[4];
    
    helper_ld_ptr_nn_XY('B', 'C', data);

    //Test if the Data Bus reads the expected value
    if (data[2] != lowByte(i) || data[3] != highByte(i) || rcvdAddr != sentAddr) {
      error = true;
    }

    //Display progress
    if ((i % 2000) == 0) {
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
bool LD_DE_PTR_NN() {
  //Mnemonic:  LD DE,(nn)
  //Assembled command: 0xED5B
  //Expected behavior: A value is loaded into Register DE from memory address (nn)
  //Test methodology: Load a random value into Register DE.  The value should be "read" from memory address (nn), repeat for all memory addresses.  Secondly, load each possible value into DE from a random memory address (nn).
  //Success criteria: Verify that the returned data matches the loaded data and that the source address matches (nn).

  //Initialize some variables
  bool error = false;
  byte data[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  uint16_t sentData=0x00;
  uint16_t rcvdData=0x00;
  uint16_t sentAddr=0x00;
  uint16_t rcvdAddr=0x00;

  char targetMnemonic[]="LD DE,(nn)";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));

  Serial.print(F(" (this will take several minutes)"));
  //Test enough times to ensure that we have hit every possible address value
  for (unsigned int i = 0; i < 65535; i++) {

    sentData=random(65535);

    data[0]=highByte(inst.opCode);
    data[1]=lowByte(inst.opCode);
    data[2]=lowByte(i);
    data[3]=highByte(i);
    data[4]=lowByte(sentData);
    data[5]=highByte(sentData);

    writeSingleInstruction(data, 2, 4, 0, addr);
    rcvdAddr=addr[4];
    
    helper_ld_ptr_nn_XY('D', 'E', data);

    //Test if the Data Bus and the written address read the expected value
    if (data[2] != lowByte(sentData) || data[3] != highByte(sentData) || rcvdAddr != i) {
      error = true;
    }

    //Display progress
    if ((i % 2000) == 0) {
      Serial.print(F("."));
    }
  }


  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 65535; i++) {

    sentAddr=random(65535);

    data[0]=highByte(inst.opCode);
    data[1]=lowByte(inst.opCode);
    data[2]=lowByte(sentAddr);
    data[3]=highByte(sentAddr);
    data[4]=lowByte(i);
    data[5]=highByte(i);

    writeSingleInstruction(data, 2, 4, 0, addr);
    rcvdAddr=addr[4];
    
    helper_ld_ptr_nn_XY('D', 'E', data);

    //Test if the Data Bus reads the expected value
    if (data[2] != lowByte(i) || data[3] != highByte(i) || rcvdAddr != sentAddr) {
      error = true;
    }

    //Display progress
    if ((i % 2000) == 0) {
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
bool LD_HL_PTR_NN() {
  //Mnemonic:  LD HL,(nn)
  //Assembled command: 0x2A
  //Expected behavior: A value is loaded into Register BC from memory address (nn)
  //Test methodology: Load a random value into Register BC.  The value should be "read" from memory address (nn), repeat for all memory addresses.  Secondly, load each possible value into BC from a random memory address (nn).
  //Success criteria: Verify that the returned data matches the loaded data and that the source address matches (nn).

  //Initialize some variables
  bool error = false;
  byte data[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  uint16_t sentData=0x00;
  uint16_t rcvdData=0x00;
  uint16_t sentAddr=0x00;
  uint16_t rcvdAddr=0x00;

  char targetMnemonic[]="LD HL,(nn)";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));

  Serial.print(F(" (this will take several minutes)"));

  //Test enough times to ensure that we have hit every possible address value
  for (unsigned int i = 0; i < 65535; i++) {

    sentData=random(65535);

    data[0]=inst.opCode;
    data[1]=lowByte(i);
    data[2]=highByte(i);
    data[3]=lowByte(sentData);
    data[4]=highByte(sentData);

    writeSingleInstruction(data, 1, 4, 0, addr);
    rcvdAddr=addr[3];

    for (int j=0; j<6; j++){
      data[j]=0;
    }
   
    for (int j=0; j<6; j++){
      addr[j]=0;
    }

    helper_ld_ptr_nn_XY('H', 'L', data);
    if (data[2] != lowByte(sentData) || data[3] != highByte(sentData) || rcvdAddr != i) {
      error = true;
      Serial.println(F("Fail in address testing."));    
      Serial.print(data[4],HEX);
      Serial.print(F(" "));
      Serial.print(highByte(sentData),HEX);
      Serial.print(F(" "));
      Serial.print(data[3],HEX);
      Serial.print(F(" "));
      Serial.print(lowByte(sentData),HEX);
      Serial.print(F(" "));
      Serial.print(i,HEX);
      Serial.print(F(" "));
      Serial.println(rcvdAddr,HEX);

    }

    //Display progress
    if ((i % 2000) == 0) {
      Serial.print(F("."));
    }
  }


  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 65535; i++) {

    sentAddr=random(65535);

    data[0]=inst.opCode;
    data[1]=lowByte(sentAddr);
    data[2]=highByte(sentAddr);
    data[3]=lowByte(i);
    data[4]=highByte(i);

    writeSingleInstruction(data, 1, 4, 0, addr);
    rcvdAddr=addr[3];
    
    for (int j=0; j<6; j++){
      data[j]=0;
    }
    for (int j=0; j<6; j++){
      addr[j]=0;
    }
    helper_ld_ptr_nn_XY('H', 'L', data);

    //Test if the Data Bus reads the expected value
    if (data[2] != lowByte(i) || data[3] != highByte(i) || rcvdAddr != sentAddr) {
      error = true;
      Serial.println(F("Fail in data testing."));
    }

    //Display progress
    if ((i % 2000) == 0) {
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
bool LD_SP_PTR_NN() {
  //Mnemonic:  LD BC,(nn)
  //Assembled command: 0xED7B
  //Expected behavior: A value is loaded into Register SP from memory address (nn)
  //Test methodology: Load a random value into Register SP.  The value should be "read" from memory address (nn), repeat for all memory addresses.  Secondly, load each possible value into SP from a random memory address (nn).
  //Success criteria: Verify that the returned data matches the loaded data and that the source address matches (nn).

  //Initialize some variables
  bool error = false;
  byte data[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  uint16_t sentData=0x00;
  uint16_t rcvdData=0x00;
  uint16_t sentAddr=0x00;
  uint16_t rcvdAddr=0x00;

  char targetMnemonic[]="LD SP,(nn)";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));

  Serial.print(F(" (this will take several minutes)"));
  //Test enough times to ensure that we have hit every possible address value
  for (unsigned int i = 0; i < 65535; i++) {

    sentData=random(65535);

    data[0]=highByte(inst.opCode);
    data[1]=lowByte(inst.opCode);
    data[2]=lowByte(i);
    data[3]=highByte(i);
    data[4]=lowByte(sentData);
    data[5]=highByte(sentData);

    writeSingleInstruction(data, 2, 4, 0, addr);
    rcvdAddr=addr[4];
    
    helper_ld_ptr_nn_XY('S', 'P', data);

    //Test if the Data Bus and the written address read the expected value
    if (data[2] != lowByte(sentData) || data[3] != highByte(sentData) || rcvdAddr != i) {
      error = true;
    }

    //Display progress
    if ((i % 2000) == 0) {
      Serial.print(F("."));
    }
  }


  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 65535; i++) {

    sentAddr=random(65535);

    data[0]=highByte(inst.opCode);
    data[1]=lowByte(inst.opCode);
    data[2]=lowByte(sentAddr);
    data[3]=highByte(sentAddr);
    data[4]=lowByte(i);
    data[5]=highByte(i);

    writeSingleInstruction(data, 2, 4, 0, addr);
    rcvdAddr=addr[4];
    
    helper_ld_ptr_nn_XY('S', 'P', data);

    //Test if the Data Bus reads the expected value
    if (data[2] != lowByte(i) || data[3] != highByte(i) || rcvdAddr != sentAddr) {
      error = true;
    }

    //Display progress
    if ((i % 2000) == 0) {
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
bool LD_IX_PTR_NN() {
  //Mnemonic:  LD IX,(nn)
  //Assembled command: 0xDD2A
  //Expected behavior: A value is loaded into Register IX from memory address (nn)
  //Test methodology: Load a random value into Register IX.  The value should be "read" from memory address (nn), repeat for all memory addresses.  Secondly, load each possible value into IX from a random memory address (nn).
  //Success criteria: Verify that the returned data matches the loaded data and that the source address matches (nn).

  //Initialize some variables
  bool error = false;
  byte data[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  uint16_t sentData=0x00;
  uint16_t rcvdData=0x00;
  uint16_t sentAddr=0x00;
  uint16_t rcvdAddr=0x00;

  char targetMnemonic[]="LD IX,(nn)";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));

  Serial.print(F(" (this will take several minutes)"));
  //Test enough times to ensure that we have hit every possible address value
  for (unsigned int i = 0; i < 65535; i++) {

    sentData=random(65535);

    data[0]=highByte(inst.opCode);
    data[1]=lowByte(inst.opCode);
    data[2]=lowByte(i);
    data[3]=highByte(i);
    data[4]=lowByte(sentData);
    data[5]=highByte(sentData);

    writeSingleInstruction(data, 2, 4, 0, addr);
    rcvdAddr=addr[4];
    
    helper_ld_ptr_nn_XY('I', 'X', data);

    //Test if the Data Bus and the written address read the expected value
    if (data[2] != lowByte(sentData) || data[3] != highByte(sentData) || rcvdAddr != i) {
      error = true;
    }

    //Display progress
    if ((i % 2000) == 0) {
      Serial.print(F("."));
    }
  }


  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 65535; i++) {

    sentAddr=random(65535);

    data[0]=highByte(inst.opCode);
    data[1]=lowByte(inst.opCode);
    data[2]=lowByte(sentAddr);
    data[3]=highByte(sentAddr);
    data[4]=lowByte(i);
    data[5]=highByte(i);

    writeSingleInstruction(data, 2, 4, 0, addr);
    rcvdAddr=addr[4];
    
    helper_ld_ptr_nn_XY('I', 'X', data);

    //Test if the Data Bus reads the expected value
    if (data[2] != lowByte(i) || data[3] != highByte(i) || rcvdAddr != sentAddr) {
      error = true;
    }

    //Display progress
    if ((i % 2000) == 0) {
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
bool LD_IY_PTR_NN() {
  //Mnemonic:  LD IY,(nn)
  //Assembled command: 0xFD2A
  //Expected behavior: A value is loaded into Register IY from memory address (nn)
  //Test methodology: Load a random value into Register IY.  The value should be "read" from memory address (nn), repeat for all memory addresses.  Secondly, load each possible value into IY from a random memory address (nn).
  //Success criteria: Verify that the returned data matches the loaded data and that the source address matches (nn).

  //Initialize some variables
  bool error = false;
  byte data[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  uint16_t sentData=0x00;
  uint16_t rcvdData=0x00;
  uint16_t sentAddr=0x00;
  uint16_t rcvdAddr=0x00;

  char targetMnemonic[]="LD IY,(nn)";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));

  Serial.print(F(" (this will take several minutes)"));
  //Test enough times to ensure that we have hit every possible address value
  for (unsigned int i = 0; i < 65535; i++) {

    sentData=random(65535);

    data[0]=highByte(inst.opCode);
    data[1]=lowByte(inst.opCode);
    data[2]=lowByte(i);
    data[3]=highByte(i);
    data[4]=lowByte(sentData);
    data[5]=highByte(sentData);

    writeSingleInstruction(data, 2, 4, 0, addr);
    rcvdAddr=addr[4];
    
    helper_ld_ptr_nn_XY('I', 'Y', data);

    //Test if the Data Bus and the written address read the expected value
    if (data[2] != lowByte(sentData) || data[3] != highByte(sentData) || rcvdAddr != i) {
      error = true;
    }

    //Display progress
    if ((i % 2000) == 0) {
      Serial.print(F("."));
    }
  }


  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 65535; i++) {

    sentAddr=random(65535);

    data[0]=highByte(inst.opCode);
    data[1]=lowByte(inst.opCode);
    data[2]=lowByte(sentAddr);
    data[3]=highByte(sentAddr);
    data[4]=lowByte(i);
    data[5]=highByte(i);

    writeSingleInstruction(data, 2, 4, 0, addr);
    rcvdAddr=addr[4];
    
    helper_ld_ptr_nn_XY('I', 'Y', data);

    //Test if the Data Bus reads the expected value
    if (data[2] != lowByte(i) || data[3] != highByte(i) || rcvdAddr != sentAddr) {
      error = true;
    }

    //Display progress
    if ((i % 2000) == 0) {
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
bool LD_PTR_NN_BC() {
  //Mnemonic:  LD (nn),BC
  //Assembled command: 0xED43
  //Expected behavior: A value is read from Register BC into a specified memory address
  //Test methodology: Load a random value into Register BC, write the value of Register BC to each memory address to verify.  Note:  All possible data values were tested in the LD BC,nn test
  //Success criteria: Verify that the returned data matches the loaded data and that the written address matches the target address.

  //Initialize some variables
  bool error = false;
  byte data[] = {0x00, 0x00, 0x00, 0x00};
  uint16_t sentData=0x00;

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 65535; i++) {

    sentData=random(65535);
    data[0]=lowByte(sentData);
    data[1]=highByte(sentData);
    //Send the LD A,n command
    helper_ld_XY_nn('B', 'C', data);

    //Read the data in the HL register   
    for (int j=0; j<4; j++) {
      data[j]=0x0;
    }
    data[0]=lowByte(i);
    data[1]=highByte(i);

    helper_ld_ptr_nn_XY('B', 'C', data);

   
    //Test if the Data Bus reads the expected value
    if (data[0] != lowByte(i) || data[1] != highByte(i) || data[2] != lowByte(sentData) || data[3] != highByte(sentData)) {
      error = true;
      Serial.print(F("Expected address: "));
      Serial.print(i,HEX);
      Serial.print(F(" got: "));
      Serial.print(data[1],HEX);
      Serial.print(data[0],HEX);
      Serial.print(F(" Expected data: "));
      Serial.print(sentData,HEX);
      Serial.print(F(" got: "));
      Serial.print(data[3],HEX);
      Serial.print(data[2],HEX);
      Serial.println(F(""));
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
bool LD_PTR_NN_DE() {
  //Mnemonic:  LD (nn),DE
  //Assembled command: 0xED53
  //Expected behavior: A value is read from Register DE into a specified memory address
  //Test methodology: Load a random value into Register DE, write the value of Register DE to each memory address to verify.  Note:  All possible data values were tested in the LD DE,nn test
  //Success criteria: Verify that the returned data matches the loaded data and that the written address matches the target address.

  //Initialize some variables
  bool error = false;
  byte data[] = {0x00, 0x00, 0x00, 0x00};
  uint16_t sentData=0x00;

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 65535; i++) {

    sentData=random(65535);
    data[0]=lowByte(sentData);
    data[1]=highByte(sentData);
    //Send the LD XY,nn command
    helper_ld_XY_nn('D', 'E', data);

    //Read the data in the DE register   
    for (int j=0; j<4; j++) {
      data[j]=0x0;
    }
    data[0]=lowByte(i);
    data[1]=highByte(i);

    helper_ld_ptr_nn_XY('D', 'E', data);

   
    //Test if the Data Bus reads the expected value
    if (data[0] != lowByte(i) || data[1] != highByte(i) || data[2] != lowByte(sentData) || data[3] != highByte(sentData)) {
      error = true;
      Serial.print(F("Expected address: "));
      Serial.print(i,HEX);
      Serial.print(F(" got: "));
      Serial.print(data[1],HEX);
      Serial.print(data[0],HEX);
      Serial.print(F(" Expected data: "));
      Serial.print(sentData,HEX);
      Serial.print(F(" got: "));
      Serial.print(data[3],HEX);
      Serial.print(data[2],HEX);
      Serial.println(F(""));
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
bool LD_PTR_NN_HL() {
  //Mnemonic:  LD (nn),HL
  //Assembled command: 0x22
  //Expected behavior: A value is read from Register HL into a specified memory address
  //Test methodology: Load a random value into Register HL, write the value of Register BC to each memory address to verify.  Note:  All possible data values were tested in the LD HL,nn test
  //Success criteria: Verify that the returned data matches the loaded data and that the written address matches the target address.

  //Initialize some variables
  bool error = false;
  byte data[] = {0x00, 0x00, 0x00, 0x00};
  uint16_t sentData=0x00;

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 65535; i++) {

    sentData=random(65535);
    data[0]=lowByte(sentData);
    data[1]=highByte(sentData);
    //Send the LD XY,nn command
    helper_ld_XY_nn('H', 'L', data);

    //Read the data at the address in the HL register   
    for (int j=0; j<4; j++) {
      data[j]=0x0;
    }
    data[0]=lowByte(i);
    data[1]=highByte(i);

    /*Serial.print(F("Sent Address: "));
    Serial.print(i,HEX);
    Serial.print(F(" Sent Data: "));
    Serial.println(sentData,HEX);*/
    helper_ld_ptr_nn_XY('H', 'L', data);

   
    //Test if the Data Bus reads the expected value
    if (data[0] != lowByte(i) || data[1] != highByte(i) || data[2] != lowByte(sentData) || data[3] != highByte(sentData)) {
      error = true;
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
bool LD_PTR_NN_SP() {
  //Mnemonic:  LD (nn),SP
  //Assembled command: 0xED73
  //Expected behavior: A value is read from Register SP into a specified memory address
  //Test methodology: Load a random value into Register SP, write the value of Register SP to each memory address to verify.  Note:  All possible data values were tested in the LD SP,nn test
  //Success criteria: Verify that the returned data matches the loaded data and that the written address matches the target address.

  //Initialize some variables
  bool error = false;
  byte data[] = {0x00, 0x00, 0x00, 0x00};
  uint16_t sentData=0x00;

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 65535; i++) {

    sentData=random(65535);
    data[0]=lowByte(sentData);
    data[1]=highByte(sentData);
    //Send the LD XY,nn command
    helper_ld_XY_nn('S', 'P', data);

    //Read the data in the SP register   
    for (int j=0; j<4; j++) {
      data[j]=0x0;
    }
    data[0]=lowByte(i);
    data[1]=highByte(i);

    helper_ld_ptr_nn_XY('S', 'P', data);

   
    //Test if the Data Bus reads the expected value
    if (data[0] != lowByte(i) || data[1] != highByte(i) || data[2] != lowByte(sentData) || data[3] != highByte(sentData)) {
      error = true;
      Serial.print(F("Expected address: "));
      Serial.print(i,HEX);
      Serial.print(F(" got: "));
      Serial.print(data[1],HEX);
      Serial.print(data[0],HEX);
      Serial.print(F(" Expected data: "));
      Serial.print(sentData,HEX);
      Serial.print(F(" got: "));
      Serial.print(data[3],HEX);
      Serial.print(data[2],HEX);
      Serial.println(F(""));
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
bool LD_PTR_NN_IX() {
  //Mnemonic:  LD (nn),IX
  //Assembled command: 0xDD22
  //Expected behavior: A value is read from Register IX into a specified memory address
  //Test methodology: Load a random value into Register IX, write the value of Register IX to each memory address to verify.  Note:  All possible data values were tested in the LD IX,nn test
  //Success criteria: Verify that the returned data matches the loaded data and that the written address matches the target address.

  //Initialize some variables
  bool error = false;
  byte data[] = {0x00, 0x00, 0x00, 0x00};
  uint16_t sentData=0x00;

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 65535; i++) {

    sentData=random(65535);
    data[0]=lowByte(sentData);
    data[1]=highByte(sentData);
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'X', data);

    //Read the data in the IX register   
    for (int j=0; j<4; j++) {
      data[j]=0x0;
    }
    data[0]=lowByte(i);
    data[1]=highByte(i);

    helper_ld_ptr_nn_XY('I', 'X', data);

   
    //Test if the Data Bus reads the expected value
    if (data[0] != lowByte(i) || data[1] != highByte(i) || data[2] != lowByte(sentData) || data[3] != highByte(sentData)) {
      error = true;
      Serial.print(F("Expected address: "));
      Serial.print(i,HEX);
      Serial.print(F(" got: "));
      Serial.print(data[1],HEX);
      Serial.print(data[0],HEX);
      Serial.print(F(" Expected data: "));
      Serial.print(sentData,HEX);
      Serial.print(F(" got: "));
      Serial.print(data[3],HEX);
      Serial.print(data[2],HEX);
      Serial.println(F(""));
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
bool LD_PTR_NN_IY() {
  //Mnemonic:  LD (nn),IY
  //Assembled command: 0xFD22
  //Expected behavior: A value is read from Register IY into a specified memory address
  //Test methodology: Load a random value into Register IY, write the value of Register IY to each memory address to verify.  Note:  All possible data values were tested in the LD IY,nn test
  //Success criteria: Verify that the returned data matches the loaded data and that the written address matches the target address.

  //Initialize some variables
  bool error = false;
  byte data[] = {0x00, 0x00, 0x00, 0x00};
  uint16_t sentData=0x00;

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 65535; i++) {

    sentData=random(65535);
    data[0]=lowByte(sentData);
    data[1]=highByte(sentData);
    //Send the LD XY,nn command
    helper_ld_XY_nn('I', 'Y', data);

    //Read the data in the IY register   
    for (int j=0; j<4; j++) {
      data[j]=0x0;
    }
    data[0]=lowByte(i);
    data[1]=highByte(i);
    data[2]=lowByte(sentData);
    data[3]=highByte(sentData);

    helper_ld_ptr_nn_XY('I', 'Y', data);

   
    //Test if the Data Bus reads the expected value
    if (data[0] != lowByte(i) || data[1] != highByte(i) || data[2] != lowByte(sentData) || data[3] != highByte(sentData)) {
      error = true;
      Serial.print(F("Expected address: "));
      Serial.print(i,HEX);
      Serial.print(F(" got: "));
      Serial.print(data[1],HEX);
      Serial.print(data[0],HEX);
      Serial.print(F(" Expected data: "));
      Serial.print(sentData,HEX);
      Serial.print(F(" got: "));
      Serial.print(data[3],HEX);
      Serial.print(data[2],HEX);
      Serial.println(F(""));
      Serial.println(F("Debug Data: "));
      for (int l=0; l<6; l++){
        Serial.print(debugData[l],HEX);
        Serial.print(F(" "));
      }
      Serial.println(F(""));
      Serial.println(F("Debug Address: "));
      for (int l=0; l<6; l++){
        Serial.print(debugAddr[l],HEX);
        Serial.print(F(" "));
      }
      Serial.println(F(""));
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
bool LD_SP_HL() {
  //Mnemonic:  LD SP,HL
  //Assembled command: 0xFD23
  //Expected behavior: A value is read from Register IY into a specified memory address
  //Test methodology: Load a random value into Register IYDE, write the value of Register IY to each memory address to verify.  Note:  All possible data values were tested in the LD IY,nn test
  //Success criteria: Verify that the returned data matches the loaded data and that the written address matches the target address.

  //Initialize some variables
  bool error = false;
  byte data[] = {0x00, 0x00, 0x00, 0x00};
  uint16_t sentData=0x00;

  //Test enough times to ensure that we have hit every possible data value
  for (unsigned int i = 0; i < 65535; i++) {

    sentData=random(65535);
    data[0]=lowByte(sentData);
    data[1]=highByte(sentData);
    //Send the LD XY,nn command
    helper_ld_XY_nn('H', 'L', data);

    //Read the data in the IY register   
    for (int j=0; j<4; j++) {
      data[j]=0x0;
    }
    data[0]=lowByte(i);
    data[1]=highByte(i);
    data[2]=lowByte(sentData);
    data[3]=highByte(sentData);

    helper_ld_ptr_nn_XY('H', 'L', data);

   
    //Test if the Data Bus reads the expected value
    if (data[0] != lowByte(i) || data[1] != highByte(i) || data[2] != lowByte(sentData) || data[3] != highByte(sentData)) {
      error = true;
      Serial.print(F("Expected address: "));
      Serial.print(i,HEX);
      Serial.print(F(" got: "));
      Serial.print(data[1],HEX);
      Serial.print(data[0],HEX);
      Serial.print(F(" Expected data: "));
      Serial.print(sentData,HEX);
      Serial.print(F(" got: "));
      Serial.print(data[3],HEX);
      Serial.print(data[2],HEX);
      Serial.println(F(""));
      Serial.println(F("Debug Data: "));
      for (int l=0; l<6; l++){
        Serial.print(debugData[l],HEX);
        Serial.print(F(" "));
      }
      Serial.println(F(""));
      Serial.println(F("Debug Address: "));
      for (int l=0; l<6; l++){
        Serial.print(debugAddr[l],HEX);
        Serial.print(F(" "));
      }
      Serial.println(F(""));
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
bool LD_SP_IX() {
  Serial.println(F("...Test not written"));
}
bool LD_SP_IY() {
  Serial.println(F("...Test not written"));
}
bool POP_AF() {
  Serial.println(F("...Test not written"));
}
bool POP_BC() {
  Serial.println(F("...Test not written"));
}
bool POP_DE() {
  Serial.println(F("...Test not written"));
}
bool POP_HL() {
  Serial.println(F("...Test not written"));
}
bool POP_IX() {
  Serial.println(F("...Test not written"));
}
bool POP_IY() {
  Serial.println(F("...Test not written"));
}
bool PUSH_AF() {
  Serial.println(F("...Test not written"));
}
bool PUSH_BC() {
  Serial.println(F("...Test not written"));
}
bool PUSH_DE() {
  Serial.println(F("...Test not written"));
}
bool PUSH_HL() {
  Serial.println(F("...Test not written"));
}
bool PUSH_IX() {
  Serial.println(F("...Test not written"));
}
bool PUSH_IY() {
  Serial.println(F("...Test not written"));
}
bool EX_AF_AF_PRIME() {
  Serial.println(F("...Test not written"));
}
bool EXX() {
  Serial.println(F("...Test not written"));
}
bool EX_DE_HL() {
  Serial.println(F("...Test not written"));
}
bool EX_PTR_SP_HL() {
  Serial.println(F("...Test not written"));
}
bool EX_PTR_SP_IX() {
  Serial.println(F("...Test not written"));
}
bool EX_PTR_SP_IY() {
  Serial.println(F("...Test not written"));
}
bool LDI() {
  Serial.println(F("...Test not written"));
}
bool LDIR() {
  Serial.println(F("...Test not written"));
}
bool LDD() {
  Serial.println(F("...Test not written"));
}
bool LDDR() {
  Serial.println(F("...Test not written"));
}
bool CPI() {
  Serial.println(F("...Test not written"));
}
bool CPIR() {
  Serial.println(F("...Test not written"));
}
bool CPD() {
  Serial.println(F("...Test not written"));
}
bool CPDR() {
  Serial.println(F("...Test not written"));
}
bool ADD_A_A() {
  Serial.println(F("...Test not written"));
}
bool ADD_A_B() {
  Serial.println(F("...Test not written"));
}
bool ADD_A_C() {
  Serial.println(F("...Test not written"));
}
bool ADD_A_D() {
  Serial.println(F("...Test not written"));
}
bool ADD_A_E() {
  Serial.println(F("...Test not written"));
}
bool ADD_A_H() {
  Serial.println(F("...Test not written"));
}
bool ADD_A_L() {
  Serial.println(F("...Test not written"));
}
bool ADD_A_PTR_HL() {
  Serial.println(F("...Test not written"));
}
bool ADD_A_PTR_IX_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool ADD_A_PTR_IY_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool ADD_A_N() {
  Serial.println(F("...Test not written"));
}
bool ADC_A_A() {
  Serial.println(F("...Test not written"));
}
bool ADC_A_B() {
  Serial.println(F("...Test not written"));
}
bool ADC_A_C() {
  Serial.println(F("...Test not written"));
}
bool ADC_A_D() {
  Serial.println(F("...Test not written"));
}
bool ADC_A_E() {
  Serial.println(F("...Test not written"));
}
bool ADC_A_H() {
  Serial.println(F("...Test not written"));
}
bool ADC_A_L() {
  Serial.println(F("...Test not written"));
}
bool ADC_A_PTR_HL() {
  Serial.println(F("...Test not written"));
}
bool ADC_A_PTR_IX_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool ADC_A_PTR_IY_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool ADC_A_N() {
  Serial.println(F("...Test not written"));
}
bool SUB_A_A() {
  Serial.println(F("...Test not written"));
}
bool SUB_A_B() {
  Serial.println(F("...Test not written"));
}
bool SUB_A_C() {
  Serial.println(F("...Test not written"));
}
bool SUB_A_D() {
  Serial.println(F("...Test not written"));
}
bool SUB_A_E() {
  Serial.println(F("...Test not written"));
}
bool SUB_A_H() {
  Serial.println(F("...Test not written"));
}
bool SUB_A_L() {
  Serial.println(F("...Test not written"));
}
bool SUB_A_PTR_HL() {
  Serial.println(F("...Test not written"));
}
bool SUB_A_PTR_IX_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool SUB_A_PTR_IY_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool SUB_A_N() {
  Serial.println(F("...Test not written"));
}
bool SBC_A_A() {
  Serial.println(F("...Test not written"));
}
bool SBC_A_B() {
  Serial.println(F("...Test not written"));
}
bool SBC_A_C() {
  Serial.println(F("...Test not written"));
}
bool SBC_A_D() {
  Serial.println(F("...Test not written"));
}
bool SBC_A_E() {
  Serial.println(F("...Test not written"));
}
bool SBC_A_H() {
  Serial.println(F("...Test not written"));
}
bool SBC_A_L() {
  Serial.println(F("...Test not written"));
}
bool SBC_A_PTR_HL() {
  Serial.println(F("...Test not written"));
}
bool SBC_A_PTR_IX_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool SBC_A_PTR_IY_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool SBC_A_N() {
  Serial.println(F("...Test not written"));
}
bool AND_A() {
  Serial.println(F("...Test not written"));
}
bool AND_B() {
  Serial.println(F("...Test not written"));
}
bool AND_C() {
  Serial.println(F("...Test not written"));
}
bool AND_D() {
  Serial.println(F("...Test not written"));
}
bool AND_E() {
  Serial.println(F("...Test not written"));
}
bool AND_H() {
  Serial.println(F("...Test not written"));
}
bool AND_L() {
  Serial.println(F("...Test not written"));
}
bool AND_PTR_HL() {
  Serial.println(F("...Test not written"));
}
bool AND_PTR_IX_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool AND_PTR_IY_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool AND_N() {
  Serial.println(F("...Test not written"));
}
bool XOR_A() {
  Serial.println(F("...Test not written"));
}
bool XOR_B() {
  Serial.println(F("...Test not written"));
}
bool XOR_C() {
  Serial.println(F("...Test not written"));
}
bool XOR_D() {
  Serial.println(F("...Test not written"));
}
bool XOR_E() {
  Serial.println(F("...Test not written"));
}
bool XOR_H() {
  Serial.println(F("...Test not written"));
}
bool XOR_L() {
  Serial.println(F("...Test not written"));
}
bool XOR_PTR_HL() {
  Serial.println(F("...Test not written"));
}
bool XOR_PTR_IX_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool XOR_PTR_IY_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool XOR_N() {
  Serial.println(F("...Test not written"));
}
bool OR_A() {
  Serial.println(F("...Test not written"));
}
bool OR_B() {
  Serial.println(F("...Test not written"));
}
bool OR_C() {
  Serial.println(F("...Test not written"));
}
bool OR_D() {
  Serial.println(F("...Test not written"));
}
bool OR_E() {
  Serial.println(F("...Test not written"));
}
bool OR_H() {
  Serial.println(F("...Test not written"));
}
bool OR_L() {
  Serial.println(F("...Test not written"));
}
bool OR_PTR_HL() {
  Serial.println(F("...Test not written"));
}
bool OR_PTR_IX_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool OR_PTR_IY_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool OR_N() {
  Serial.println(F("...Test not written"));
}
bool CP_A() {
  Serial.println(F("...Test not written"));
}
bool CP_B() {
  Serial.println(F("...Test not written"));
}
bool CP_C() {
  Serial.println(F("...Test not written"));
}
bool CP_D() {
  Serial.println(F("...Test not written"));
}
bool CP_E() {
  Serial.println(F("...Test not written"));
}
bool CP_H() {
  Serial.println(F("...Test not written"));
}
bool CP_L() {
  Serial.println(F("...Test not written"));
}
bool CP_PTR_HL() {
  Serial.println(F("...Test not written"));
}
bool CP_PTR_IX_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool CP_PTR_IY_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool CP_N() {
  Serial.println(F("...Test not written"));
}
bool INC_A() {
  Serial.println(F("...Test not written"));
}
bool INC_B() {
  Serial.println(F("...Test not written"));
}
bool INC_C() {
  Serial.println(F("...Test not written"));
}
bool INC_D() {
  Serial.println(F("...Test not written"));
}
bool INC_E() {
  Serial.println(F("...Test not written"));
}
bool INC_H() {
  Serial.println(F("...Test not written"));
}
bool INC_L() {
  Serial.println(F("...Test not written"));
}
bool INC_PTR_HL() {
  Serial.println(F("...Test not written"));
}
bool INC_PTR_IX_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool INC_PTR_IY_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool DEC_A() {
  Serial.println(F("...Test not written"));
}
bool DEC_B() {
  Serial.println(F("...Test not written"));
}
bool DEC_C() {
  Serial.println(F("...Test not written"));
}
bool DEC_D() {
  Serial.println(F("...Test not written"));
}
bool DEC_E() {
  Serial.println(F("...Test not written"));
}
bool DEC_H() {
  Serial.println(F("...Test not written"));
}
bool DEC_L() {
  Serial.println(F("...Test not written"));
}
bool DEC_PTR_HL() {
  Serial.println(F("...Test not written"));
}
bool DEC_PTR_IX_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool DEC_PTR_IY_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool DAA() {
  Serial.println(F("...Test not written"));
}
bool CPL() {
  Serial.println(F("...Test not written"));
}
bool NEG() {
  Serial.println(F("...Test not written"));
}
bool CCF() {
  Serial.println(F("...Test not written"));
}
bool SCF() {
  Serial.println(F("...Test not written"));
}
bool ADD_HL_BC() {
  Serial.println(F("...Test not written"));
}
bool ADD_HL_DE() {
  Serial.println(F("...Test not written"));
}
bool ADD_HL_HL() {
  Serial.println(F("...Test not written"));
}
bool ADD_HL_SP() {
  Serial.println(F("...Test not written"));
}
bool ADD_IX_BC() {
  Serial.println(F("...Test not written"));
}
bool ADD_IX_DE() {
  Serial.println(F("...Test not written"));
}
bool ADD_IX_SP() {
  Serial.println(F("...Test not written"));
}
bool ADD_IX_IX() {
  Serial.println(F("...Test not written"));
}
bool ADD_IY_BC() {
  Serial.println(F("...Test not written"));
}
bool ADD_IY_DE() {
  Serial.println(F("...Test not written"));
}
bool ADD_IY_SP() {
  Serial.println(F("...Test not written"));
}
bool ADD_IY_IY() {
  Serial.println(F("...Test not written"));
}
bool ADC_HL_BC() {
  Serial.println(F("...Test not written"));
}
bool ADC_HL_DE() {
  Serial.println(F("...Test not written"));
}
bool ADC_HL_HL() {
  Serial.println(F("...Test not written"));
}
bool ADC_HL_SP() {
  Serial.println(F("...Test not written"));
}
bool SBC_HL_BC() {
  Serial.println(F("...Test not written"));
}
bool SBC_HL_DE() {
  Serial.println(F("...Test not written"));
}
bool SBC_HL_HL() {
  Serial.println(F("...Test not written"));
}
bool SBC_HL_SP() {
  Serial.println(F("...Test not written"));
}
bool INC_BC() {
  Serial.println(F("...Test not written"));
}
bool INC_DE() {
  Serial.println(F("...Test not written"));
}
bool INC_HL() {
  Serial.println(F("...Test not written"));
}
bool INC_SP() {
  Serial.println(F("...Test not written"));
}
bool INC_IX() {
  Serial.println(F("...Test not written"));
}
bool INC_IY() {
  Serial.println(F("...Test not written"));
}
bool DEC_BC() {
  Serial.println(F("...Test not written"));
}
bool DEC_DE() {
  Serial.println(F("...Test not written"));
}
bool DEC_HL() {
  Serial.println(F("...Test not written"));
}
bool DEC_SP() {
  Serial.println(F("...Test not written"));
}
bool DEC_IX() {
  Serial.println(F("...Test not written"));
}
bool DEC_IY() {
  Serial.println(F("...Test not written"));
}
bool RLC_A() {
  Serial.println(F("...Test not written"));
}
bool RLC_B() {
  Serial.println(F("...Test not written"));
}
bool RLC_C() {
  Serial.println(F("...Test not written"));
}
bool RLC_D() {
  Serial.println(F("...Test not written"));
}
bool RLC_E() {
  Serial.println(F("...Test not written"));
}
bool RLC_H() {
  Serial.println(F("...Test not written"));
}
bool RLC_L() {
  Serial.println(F("...Test not written"));
}
bool RLC_PTR_HL() {
  Serial.println(F("...Test not written"));
}
bool RLC_PTR_IX_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool RLC_PTR_IY_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool RRC_A() {
  Serial.println(F("...Test not written"));
}
bool RRC_B() {
  Serial.println(F("...Test not written"));
}
bool RRC_C() {
  Serial.println(F("...Test not written"));
}
bool RRC_D() {
  Serial.println(F("...Test not written"));
}
bool RRC_E() {
  Serial.println(F("...Test not written"));
}
bool RRC_H() {
  Serial.println(F("...Test not written"));
}
bool RRC_L() {
  Serial.println(F("...Test not written"));
}
bool RRC_PTR_HL() {
  Serial.println(F("...Test not written"));
}
bool RRC_PTR_IX_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool RRC_PTR_IY_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool RL_A() {
  Serial.println(F("...Test not written"));
}
bool RL_B() {
  Serial.println(F("...Test not written"));
}
bool RL_C() {
  Serial.println(F("...Test not written"));
}
bool RL_D() {
  Serial.println(F("...Test not written"));
}
bool RL_E() {
  Serial.println(F("...Test not written"));
}
bool RL_H() {
  Serial.println(F("...Test not written"));
}
bool RL_L() {
  Serial.println(F("...Test not written"));
}
bool RL_PTR_HL() {
  Serial.println(F("...Test not written"));
}
bool RL_PTR_IX_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool RL_PTR_IY_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool RR_A() {
  Serial.println(F("...Test not written"));
}
bool RR_B() {
  Serial.println(F("...Test not written"));
}
bool RR_C() {
  Serial.println(F("...Test not written"));
}
bool RR_D() {
  Serial.println(F("...Test not written"));
}
bool RR_E() {
  Serial.println(F("...Test not written"));
}
bool RR_H() {
  Serial.println(F("...Test not written"));
}
bool RR_L() {
  Serial.println(F("...Test not written"));
}
bool RR_PTR_HL() {
  Serial.println(F("...Test not written"));
}
bool RR_PTR_IX_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool RR_PTR_IY_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool SLA_A() {
  Serial.println(F("...Test not written"));
}
bool SLA_B() {
  Serial.println(F("...Test not written"));
}
bool SLA_C() {
  Serial.println(F("...Test not written"));
}
bool SLA_D() {
  Serial.println(F("...Test not written"));
}
bool SLA_E() {
  Serial.println(F("...Test not written"));
}
bool SLA_H() {
  Serial.println(F("...Test not written"));
}
bool SLA_L() {
  Serial.println(F("...Test not written"));
}
bool SLA_PTR_HL() {
  Serial.println(F("...Test not written"));
}
bool SLA_PTR_IX_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool SLA_PTR_IY_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool SRA_A() {
  Serial.println(F("...Test not written"));
}
bool SRA_B() {
  Serial.println(F("...Test not written"));
}
bool SRA_C() {
  Serial.println(F("...Test not written"));
}
bool SRA_D() {
  Serial.println(F("...Test not written"));
}
bool SRA_E() {
  Serial.println(F("...Test not written"));
}
bool SRA_H() {
  Serial.println(F("...Test not written"));
}
bool SRA_L() {
  Serial.println(F("...Test not written"));
}
bool SRA_PTR_HL() {
  Serial.println(F("...Test not written"));
}
bool SRA_PTR_IX_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool SRA_PTR_IY_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool SLL_A() {
  Serial.println(F("...Test not written"));
}
bool SLL_B() {
  Serial.println(F("...Test not written"));
}
bool SLL_C() {
  Serial.println(F("...Test not written"));
}
bool SLL_D() {
  Serial.println(F("...Test not written"));
}
bool SLL_E() {
  Serial.println(F("...Test not written"));
}
bool SLL_H() {
  Serial.println(F("...Test not written"));
}
bool SLL_L() {
  Serial.println(F("...Test not written"));
}
bool SLL_PTR_HL() {
  Serial.println(F("...Test not written"));
}
bool SLL_PTR_IX_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool SLL_PTR_IY_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool SRL_A() {
  Serial.println(F("...Test not written"));
}
bool SRL_B() {
  Serial.println(F("...Test not written"));
}
bool SRL_C() {
  Serial.println(F("...Test not written"));
}
bool SRL_D() {
  Serial.println(F("...Test not written"));
}
bool SRL_E() {
  Serial.println(F("...Test not written"));
}
bool SRL_H() {
  Serial.println(F("...Test not written"));
}
bool SRL_L() {
  Serial.println(F("...Test not written"));
}
bool SRL_PTR_HL() {
  Serial.println(F("...Test not written"));
}
bool SRL_PTR_IX_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool SRL_PTR_IY_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool RLD() {
  Serial.println(F("...Test not written"));
}
bool RRD() {
  Serial.println(F("...Test not written"));
}
bool RLCA() {
  Serial.println(F("...Test not written"));
}
bool RRCA() {
  Serial.println(F("...Test not written"));
}
bool RLA() {
  Serial.println(F("...Test not written"));
}
bool RRA() {
  Serial.println(F("...Test not written"));
}
bool BIT_0_A() {
  Serial.println(F("...Test not written"));
}
bool BIT_0_B() {
  Serial.println(F("...Test not written"));
}
bool BIT_0_C() {
  Serial.println(F("...Test not written"));
}
bool BIT_0_D() {
  Serial.println(F("...Test not written"));
}
bool BIT_0_E() {
  Serial.println(F("...Test not written"));
}
bool BIT_0_H() {
  Serial.println(F("...Test not written"));
}
bool BIT_0_L() {
  Serial.println(F("...Test not written"));
}
bool BIT_0_PTR_HL() {
  Serial.println(F("...Test not written"));
}
bool BIT_0_PTR_IX_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool BIT_0_PTR_IY_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool BIT_1_A() {
  Serial.println(F("...Test not written"));
}
bool BIT_1_B() {
  Serial.println(F("...Test not written"));
}
bool BIT_1_C() {
  Serial.println(F("...Test not written"));
}
bool BIT_1_D() {
  Serial.println(F("...Test not written"));
}
bool BIT_1_E() {
  Serial.println(F("...Test not written"));
}
bool BIT_1_H() {
  Serial.println(F("...Test not written"));
}
bool BIT_1_L() {
  Serial.println(F("...Test not written"));
}
bool BIT_1_PTR_HL() {
  Serial.println(F("...Test not written"));
}
bool BIT_1_PTR_IX_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool BIT_1_PTR_IY_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool BIT_2_A() {
  Serial.println(F("...Test not written"));
}
bool BIT_2_B() {
  Serial.println(F("...Test not written"));
}
bool BIT_2_C() {
  Serial.println(F("...Test not written"));
}
bool BIT_2_D() {
  Serial.println(F("...Test not written"));
}
bool BIT_2_E() {
  Serial.println(F("...Test not written"));
}
bool BIT_2_H() {
  Serial.println(F("...Test not written"));
}
bool BIT_2_L() {
  Serial.println(F("...Test not written"));
}
bool BIT_2_PTR_HL() {
  Serial.println(F("...Test not written"));
}
bool BIT_2_PTR_IX_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool BIT_2_PTR_IY_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool BIT_3_A() {
  Serial.println(F("...Test not written"));
}
bool BIT_3_B() {
  Serial.println(F("...Test not written"));
}
bool BIT_3_C() {
  Serial.println(F("...Test not written"));
}
bool BIT_3_D() {
  Serial.println(F("...Test not written"));
}
bool BIT_3_E() {
  Serial.println(F("...Test not written"));
}
bool BIT_3_H() {
  Serial.println(F("...Test not written"));
}
bool BIT_3_L() {
  Serial.println(F("...Test not written"));
}
bool BIT_3_PTR_HL() {
  Serial.println(F("...Test not written"));
}
bool BIT_3_PTR_IX_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool BIT_3_PTR_IY_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool BIT_4_A() {
  Serial.println(F("...Test not written"));
}
bool BIT_4_B() {
  Serial.println(F("...Test not written"));
}
bool BIT_4_C() {
  Serial.println(F("...Test not written"));
}
bool BIT_4_D() {
  Serial.println(F("...Test not written"));
}
bool BIT_4_E() {
  Serial.println(F("...Test not written"));
}
bool BIT_4_H() {
  Serial.println(F("...Test not written"));
}
bool BIT_4_L() {
  Serial.println(F("...Test not written"));
}
bool BIT_4_PTR_HL() {
  Serial.println(F("...Test not written"));
}
bool BIT_4_PTR_IX_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool BIT_4_PTR_IY_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool BIT_5_A() {
  Serial.println(F("...Test not written"));
}
bool BIT_5_B() {
  Serial.println(F("...Test not written"));
}
bool BIT_5_C() {
  Serial.println(F("...Test not written"));
}
bool BIT_5_D() {
  Serial.println(F("...Test not written"));
}
bool BIT_5_E() {
  Serial.println(F("...Test not written"));
}
bool BIT_5_H() {
  Serial.println(F("...Test not written"));
}
bool BIT_5_L() {
  Serial.println(F("...Test not written"));
}
bool BIT_5_PTR_HL() {
  Serial.println(F("...Test not written"));
}
bool BIT_5_PTR_IX_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool BIT_5_PTR_IY_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool BIT_6_A() {
  Serial.println(F("...Test not written"));
}
bool BIT_6_B() {
  Serial.println(F("...Test not written"));
}
bool BIT_6_C() {
  Serial.println(F("...Test not written"));
}
bool BIT_6_D() {
  Serial.println(F("...Test not written"));
}
bool BIT_6_E() {
  Serial.println(F("...Test not written"));
}
bool BIT_6_H() {
  Serial.println(F("...Test not written"));
}
bool BIT_6_L() {
  Serial.println(F("...Test not written"));
}
bool BIT_6_PTR_HL() {
  Serial.println(F("...Test not written"));
}
bool BIT_6_PTR_IX_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool BIT_6_PTR_IY_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool BIT_7_A() {
  Serial.println(F("...Test not written"));
}
bool BIT_7_B() {
  Serial.println(F("...Test not written"));
}
bool BIT_7_C() {
  Serial.println(F("...Test not written"));
}
bool BIT_7_D() {
  Serial.println(F("...Test not written"));
}
bool BIT_7_E() {
  Serial.println(F("...Test not written"));
}
bool BIT_7_H() {
  Serial.println(F("...Test not written"));
}
bool BIT_7_L() {
  Serial.println(F("...Test not written"));
}
bool BIT_7_PTR_HL() {
  Serial.println(F("...Test not written"));
}
bool BIT_7_PTR_IX_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool BIT_7_PTR_IY_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool RES_0_A() {
  Serial.println(F("...Test not written"));
}
bool RES_0_B() {
  Serial.println(F("...Test not written"));
}
bool RES_0_C() {
  Serial.println(F("...Test not written"));
}
bool RES_0_D() {
  Serial.println(F("...Test not written"));
}
bool RES_0_E() {
  Serial.println(F("...Test not written"));
}
bool RES_0_H() {
  Serial.println(F("...Test not written"));
}
bool RES_0_L() {
  Serial.println(F("...Test not written"));
}
bool RES_0_PTR_HL() {
  Serial.println(F("...Test not written"));
}
bool RES_0_PTR_IX_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool RES_0_PTR_IY_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool RES_1_A() {
  Serial.println(F("...Test not written"));
}
bool RES_1_B() {
  Serial.println(F("...Test not written"));
}
bool RES_1_C() {
  Serial.println(F("...Test not written"));
}
bool RES_1_D() {
  Serial.println(F("...Test not written"));
}
bool RES_1_E() {
  Serial.println(F("...Test not written"));
}
bool RES_1_H() {
  Serial.println(F("...Test not written"));
}
bool RES_1_L() {
  Serial.println(F("...Test not written"));
}
bool RES_1_PTR_HL() {
  Serial.println(F("...Test not written"));
}
bool RES_1_PTR_IX_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool RES_1_PTR_IY_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool RES_2_A() {
  Serial.println(F("...Test not written"));
}
bool RES_2_B() {
  Serial.println(F("...Test not written"));
}
bool RES_2_C() {
  Serial.println(F("...Test not written"));
}
bool RES_2_D() {
  Serial.println(F("...Test not written"));
}
bool RES_2_E() {
  Serial.println(F("...Test not written"));
}
bool RES_2_H() {
  Serial.println(F("...Test not written"));
}
bool RES_2_L() {
  Serial.println(F("...Test not written"));
}
bool RES_2_PTR_HL() {
  Serial.println(F("...Test not written"));
}
bool RES_2_PTR_IX_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool RES_2_PTR_IY_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool RES_3_A() {
  Serial.println(F("...Test not written"));
}
bool RES_3_B() {
  Serial.println(F("...Test not written"));
}
bool RES_3_C() {
  Serial.println(F("...Test not written"));
}
bool RES_3_D() {
  Serial.println(F("...Test not written"));
}
bool RES_3_E() {
  Serial.println(F("...Test not written"));
}
bool RES_3_H() {
  Serial.println(F("...Test not written"));
}
bool RES_3_L() {
  Serial.println(F("...Test not written"));
}
bool RES_3_PTR_HL() {
  Serial.println(F("...Test not written"));
}
bool RES_3_PTR_IX_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool RES_3_PTR_IY_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool RES_4_A() {
  Serial.println(F("...Test not written"));
}
bool RES_4_B() {
  Serial.println(F("...Test not written"));
}
bool RES_4_C() {
  Serial.println(F("...Test not written"));
}
bool RES_4_D() {
  Serial.println(F("...Test not written"));
}
bool RES_4_E() {
  Serial.println(F("...Test not written"));
}
bool RES_4_H() {
  Serial.println(F("...Test not written"));
}
bool RES_4_L() {
  Serial.println(F("...Test not written"));
}
bool RES_4_PTR_HL() {
  Serial.println(F("...Test not written"));
}
bool RES_4_PTR_IX_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool RES_4_PTR_IY_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool RES_5_A() {
  Serial.println(F("...Test not written"));
}
bool RES_5_B() {
  Serial.println(F("...Test not written"));
}
bool RES_5_C() {
  Serial.println(F("...Test not written"));
}
bool RES_5_D() {
  Serial.println(F("...Test not written"));
}
bool RES_5_E() {
  Serial.println(F("...Test not written"));
}
bool RES_5_H() {
  Serial.println(F("...Test not written"));
}
bool RES_5_L() {
  Serial.println(F("...Test not written"));
}
bool RES_5_PTR_HL() {
  Serial.println(F("...Test not written"));
}
bool RES_5_PTR_IX_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool RES_5_PTR_IY_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool RES_6_A() {
  Serial.println(F("...Test not written"));
}
bool RES_6_B() {
  Serial.println(F("...Test not written"));
}
bool RES_6_C() {
  Serial.println(F("...Test not written"));
}
bool RES_6_D() {
  Serial.println(F("...Test not written"));
}
bool RES_6_E() {
  Serial.println(F("...Test not written"));
}
bool RES_6_H() {
  Serial.println(F("...Test not written"));
}
bool RES_6_L() {
  Serial.println(F("...Test not written"));
}
bool RES_6_PTR_HL() {
  Serial.println(F("...Test not written"));
}
bool RES_6_PTR_IX_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool RES_6_PTR_IY_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool RES_7_A() {
  Serial.println(F("...Test not written"));
}
bool RES_7_B() {
  Serial.println(F("...Test not written"));
}
bool RES_7_C() {
  Serial.println(F("...Test not written"));
}
bool RES_7_D() {
  Serial.println(F("...Test not written"));
}
bool RES_7_E() {
  Serial.println(F("...Test not written"));
}
bool RES_7_H() {
  Serial.println(F("...Test not written"));
}
bool RES_7_L() {
  Serial.println(F("...Test not written"));
}
bool RES_7_PTR_HL() {
  Serial.println(F("...Test not written"));
}
bool RES_7_PTR_IX_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool RES_7_PTR_IY_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool SET_0_A() {
  Serial.println(F("...Test not written"));
}
bool SET_0_B() {
  Serial.println(F("...Test not written"));
}
bool SET_0_C() {
  Serial.println(F("...Test not written"));
}
bool SET_0_D() {
  Serial.println(F("...Test not written"));
}
bool SET_0_E() {
  Serial.println(F("...Test not written"));
}
bool SET_0_H() {
  Serial.println(F("...Test not written"));
}
bool SET_0_L() {
  Serial.println(F("...Test not written"));
}
bool SET_0_PTR_HL() {
  Serial.println(F("...Test not written"));
}
bool SET_0_PTR_IX_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool SET_0_PTR_IY_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool SET_1_A() {
  Serial.println(F("...Test not written"));
}
bool SET_1_B() {
  Serial.println(F("...Test not written"));
}
bool SET_1_C() {
  Serial.println(F("...Test not written"));
}
bool SET_1_D() {
  Serial.println(F("...Test not written"));
}
bool SET_1_E() {
  Serial.println(F("...Test not written"));
}
bool SET_1_H() {
  Serial.println(F("...Test not written"));
}
bool SET_1_L() {
  Serial.println(F("...Test not written"));
}
bool SET_1_PTR_HL() {
  Serial.println(F("...Test not written"));
}
bool SET_1_PTR_IX_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool SET_1_PTR_IY_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool SET_2_A() {
  Serial.println(F("...Test not written"));
}
bool SET_2_B() {
  Serial.println(F("...Test not written"));
}
bool SET_2_C() {
  Serial.println(F("...Test not written"));
}
bool SET_2_D() {
  Serial.println(F("...Test not written"));
}
bool SET_2_E() {
  Serial.println(F("...Test not written"));
}
bool SET_2_H() {
  Serial.println(F("...Test not written"));
}
bool SET_2_L() {
  Serial.println(F("...Test not written"));
}
bool SET_2_PTR_HL() {
  Serial.println(F("...Test not written"));
}
bool SET_2_PTR_IX_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool SET_2_PTR_IY_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool SET_3_A() {
  Serial.println(F("...Test not written"));
}
bool SET_3_B() {
  Serial.println(F("...Test not written"));
}
bool SET_3_C() {
  Serial.println(F("...Test not written"));
}
bool SET_3_D() {
  Serial.println(F("...Test not written"));
}
bool SET_3_E() {
  Serial.println(F("...Test not written"));
}
bool SET_3_H() {
  Serial.println(F("...Test not written"));
}
bool SET_3_L() {
  Serial.println(F("...Test not written"));
}
bool SET_3_PTR_HL() {
  Serial.println(F("...Test not written"));
}
bool SET_3_PTR_IX_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool SET_3_PTR_IY_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool SET_4_A() {
  Serial.println(F("...Test not written"));
}
bool SET_4_B() {
  Serial.println(F("...Test not written"));
}
bool SET_4_C() {
  Serial.println(F("...Test not written"));
}
bool SET_4_D() {
  Serial.println(F("...Test not written"));
}
bool SET_4_E() {
  Serial.println(F("...Test not written"));
}
bool SET_4_H() {
  Serial.println(F("...Test not written"));
}
bool SET_4_L() {
  Serial.println(F("...Test not written"));
}
bool SET_4_PTR_HL() {
  Serial.println(F("...Test not written"));
}
bool SET_4_PTR_IX_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool SET_4_PTR_IY_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool SET_5_A() {
  Serial.println(F("...Test not written"));
}
bool SET_5_B() {
  Serial.println(F("...Test not written"));
}
bool SET_5_C() {
  Serial.println(F("...Test not written"));
}
bool SET_5_D() {
  Serial.println(F("...Test not written"));
}
bool SET_5_E() {
  Serial.println(F("...Test not written"));
}
bool SET_5_H() {
  Serial.println(F("...Test not written"));
}
bool SET_5_L() {
  Serial.println(F("...Test not written"));
}
bool SET_5_PTR_HL() {
  Serial.println(F("...Test not written"));
}
bool SET_5_PTR_IX_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool SET_5_PTR_IY_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool SET_6_A() {
  Serial.println(F("...Test not written"));
}
bool SET_6_B() {
  Serial.println(F("...Test not written"));
}
bool SET_6_C() {
  Serial.println(F("...Test not written"));
}
bool SET_6_D() {
  Serial.println(F("...Test not written"));
}
bool SET_6_E() {
  Serial.println(F("...Test not written"));
}
bool SET_6_H() {
  Serial.println(F("...Test not written"));
}
bool SET_6_L() {
  Serial.println(F("...Test not written"));
}
bool SET_6_PTR_HL() {
  Serial.println(F("...Test not written"));
}
bool SET_6_PTR_IX_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool SET_6_PTR_IY_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool SET_7_A() {
  Serial.println(F("...Test not written"));
}
bool SET_7_B() {
  Serial.println(F("...Test not written"));
}
bool SET_7_C() {
  Serial.println(F("...Test not written"));
}
bool SET_7_D() {
  Serial.println(F("...Test not written"));
}
bool SET_7_E() {
  Serial.println(F("...Test not written"));
}
bool SET_7_H() {
  Serial.println(F("...Test not written"));
}
bool SET_7_L() {
  Serial.println(F("...Test not written"));
}
bool SET_7_PTR_HL() {
  Serial.println(F("...Test not written"));
}
bool SET_7_PTR_IX_PLUS_D() {
  Serial.println(F("...Test not written"));
}
bool SET_7_PTR_IY_PLUS_D() {
  Serial.println(F("...Test not written"));
}
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

  //Get the instruction information
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));

  
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
bool JP_C_NN() {
  Serial.println(F("...Test not written"));
}
bool JP_NC_NN() {
  Serial.println(F("...Test not written"));
}
bool JP_Z_NN() {
  Serial.println(F("...Test not written"));
}
bool JP_NZ_NN() {
  Serial.println(F("...Test not written"));
}
bool JP_PE_NN() {
  Serial.println(F("...Test not written"));
}
bool JP_PO_NN() {
  Serial.println(F("...Test not written"));
}
bool JP_M_NN() {
  Serial.println(F("...Test not written"));
}
bool JP_P_NN() {
  Serial.println(F("...Test not written"));
}
bool JR_E() {
  Serial.println(F("...Test not written"));
}
bool JR_C_E() {
  Serial.println(F("...Test not written"));
}
bool JR_NC_E() {
  Serial.println(F("...Test not written"));
}
bool JR_Z_E() {
  Serial.println(F("...Test not written"));
}
bool JR_NZ_E() {
  Serial.println(F("...Test not written"));
}
bool JP_PTR_HL() {
  Serial.println(F("...Test not written"));
}
bool JP_PTR_IX() {
  Serial.println(F("...Test not written"));
}
bool JP_PTR_IY() {
  Serial.println(F("...Test not written"));
}
bool CALL_NN() {
  Serial.println(F("...Test not written"));
}
bool CALL_C_NN() {
  Serial.println(F("...Test not written"));
}
bool CALL_NC_NN() {
  Serial.println(F("...Test not written"));
}
bool CALL_Z_NN() {
  Serial.println(F("...Test not written"));
}
bool CALL_NZ_NN() {
  Serial.println(F("...Test not written"));
}
bool CALL_PE_NN() {
  Serial.println(F("...Test not written"));
}
bool CALL_PO_NN() {
  Serial.println(F("...Test not written"));
}
bool CALL_M_NN() {
  Serial.println(F("...Test not written"));
}
bool CALL_P_NN() {
  Serial.println(F("...Test not written"));
}
bool DJNZ_E() {
  Serial.println(F("...Test not written"));
}
bool RET() {
  Serial.println(F("...Test not written"));
}
bool RET_C() {
  Serial.println(F("...Test not written"));
}
bool RET_NC() {
  Serial.println(F("...Test not written"));
}
bool RET_Z() {
  Serial.println(F("...Test not written"));
}
bool RET_NZ() {
  Serial.println(F("...Test not written"));
}
bool RET_PE() {
  Serial.println(F("...Test not written"));
}
bool RET_PO() {
  Serial.println(F("...Test not written"));
}
bool RET_M() {
  Serial.println(F("...Test not written"));
}
bool RET_P() {
  Serial.println(F("...Test not written"));
}
bool RET_I() {
  Serial.println(F("...Test not written"));
}
bool RET_N() {
  Serial.println(F("...Test not written"));
}
bool RST_0() {
  Serial.println(F("...Test not written"));
}
bool RST_8() {
  Serial.println(F("...Test not written"));
}
bool RST_16() {
  Serial.println(F("...Test not written"));
}
bool RST_24() {
  Serial.println(F("...Test not written"));
}
bool RST_32() {
  Serial.println(F("...Test not written"));
}
bool RST_40() {
  Serial.println(F("...Test not written"));
}
bool RST_48() {
  Serial.println(F("...Test not written"));
}
bool RST_56() {
  Serial.println(F("...Test not written"));
}
bool IN_A_PTR_N() {
  Serial.println(F("...Test not written"));
}
bool IN_A_PTR_C() {
  Serial.println(F("...Test not written"));
}
bool IN_B_PTR_C() {
  Serial.println(F("...Test not written"));
}
bool IN_C_PTR_C() {
  Serial.println(F("...Test not written"));
}
bool IN_D_PTR_C() {
  Serial.println(F("...Test not written"));
}
bool IN_E_PTR_C() {
  Serial.println(F("...Test not written"));
}
bool IN_H_PTR_C() {
  Serial.println(F("...Test not written"));
}
bool IN_L_PTR_C() {
  Serial.println(F("...Test not written"));
}
bool INI() {
  Serial.println(F("...Test not written"));
}
bool INIR() {
  Serial.println(F("...Test not written"));
}
bool IND() {
  Serial.println(F("...Test not written"));
}
bool INDR() {
  Serial.println(F("...Test not written"));
}
bool OUT_PTR_N_A() {
  Serial.println(F("...Test not written"));
}
bool OUT_PTR_C_A() {
  Serial.println(F("...Test not written"));
}
bool OUT_PTR_C_B() {
  Serial.println(F("...Test not written"));
}
bool OUT_PTR_C_C() {
  Serial.println(F("...Test not written"));
}
bool OUT_PTR_C_D() {
  Serial.println(F("...Test not written"));
}
bool OUT_PTR_C_E() {
  Serial.println(F("...Test not written"));
}
bool OUT_PTR_C_H() {
  Serial.println(F("...Test not written"));
}
bool OUT_PTR_C_L() {
  Serial.println(F("...Test not written"));
}
bool OUTI() {
  Serial.println(F("...Test not written"));
}
bool OUTIR() {
  Serial.println(F("...Test not written"));
}
bool OUTD() {
  Serial.println(F("...Test not written"));
}
bool OUTDR() {
  Serial.println(F("...Test not written"));
}
bool HALT_() {
  Serial.println(F("...Test not written"));
}
bool DI() {
  Serial.println(F("...Test not written"));
}
bool EI() {
  Serial.println(F("...Test not written"));
}
bool IM0() {
  Serial.println(F("...Test not written"));
}
bool IM1() {
  Serial.println(F("...Test not written"));
}
bool IM2() {
  Serial.println(F("...Test not written"));
}
