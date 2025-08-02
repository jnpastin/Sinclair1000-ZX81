//Helper function to load register with an 8 bit value 
//Expects a single byte data value
void helper_ld_X_n(char targetRegister, byte data)
{
  char targetMnemonic[]="LD X,n";
  targetMnemonic[3]=targetRegister;
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));

  byte localData[]={inst.opCode, data};
 uint16_t addr[]={0x00, 0x00, 0x00, 0x00};

  writeSingleInstruction(localData, 1, 1, 0, addr);

}

//Helper function to load register with an 16 bit value 
//Expects a two byte data value
void helper_ld_XY_nn(char targetRegister, char targetRegister2, byte* data)
{
  char targetMnemonic[]="LD XY,nn";
  targetMnemonic[3]=targetRegister;
  targetMnemonic[4]=targetRegister2;
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));
  byte localData[]={0x00, 0x00, 0x00, 0x00};
  int numM1Cycles;

  if (highByte(inst.opCode) != 0){
    localData[0]=highByte(inst.opCode);
    localData[1]=lowByte(inst.opCode);
    localData[2]=data[0];
    localData[3]=data[1];
    numM1Cycles=2;
  } else {
    localData[0]=inst.opCode;
    localData[1]=data[0];
    localData[2]=data[1];
    numM1Cycles=1;
  }

  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};
  writeSingleInstruction(localData, numM1Cycles, 2, 0, addr);
}

//Helper function to load register with the contents of another register
//Expects a single byte data value
void helper_ld_X_Y(char targetRegister, char sourceRegister)
{
  char targetMnemonic[]="LD X,Y";
  targetMnemonic[3]=targetRegister;
  targetMnemonic[5]=sourceRegister;
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));

  uint16_t addr[]={0x00,};
  byte localData[]={inst.opCode};
  writeSingleInstruction(localData, 1, 0, 0, addr);
}


//Helper function to load memory address nn witht the contents of the A register
void helper_ld_ptr_nn_a(byte* data, uint16_t* addresses)
{
  //Expects a byte array consisting of:
  // [0] = Address Low Byte
  // [1] = Address High Byte
  // [2] = Data Byte to overwrite with the retrieved data
  char targetMnemonic[]="LD (nn),A";
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));

  byte localData[]={inst.opCode, data[0], data[1], data[2]};

  writeSingleInstruction(localData, 1, 2, 1, addresses);

  data[2]=localData[3];
}

//Helper function to load memory address nn with the contents of the HL register
void helper_ld_ptr_nn_XY(char targetRegister, char targetRegister2, byte* data)
{
  //Expects a byte array consisting of:
  // [0] = Address Low Byte
  // [1] = Address High Byte
  // [2] = Low Data Byte to overwrite with the retrieved data
  // [3] = High Data Byte to overwrite with the retrieved data
  char targetMnemonic[]="LD (nn),XY";
  targetMnemonic[8]=targetRegister;
  targetMnemonic[9]=targetRegister2;
  byte localData[]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  int numM1Cycles;
  int instIndex=findInstructionIndexByMnemonic(targetMnemonic);
  instructionDefinitionType inst;
  memcpy_P(&inst, &InstructionDefinitions[instIndex], sizeof(inst));

  if (highByte(inst.opCode) != 0 ){
    localData[0]=highByte(inst.opCode);
    localData[1]=lowByte(inst.opCode);
    localData[2]=data[0];
    localData[3]=data[1];
    localData[4]=data[2];
    numM1Cycles=2;
  } else {
    localData[0]=inst.opCode;
    localData[1]=data[0];
    localData[2]=data[1];
    localData[3]=data[2];
    numM1Cycles=1;
  }
  /*Serial.println(targetMnemonic);
  Serial.println(inst.opCode,HEX);
  for (int i=0; i<6; i++){
    Serial.println (localData[i],HEX);
  }*/
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};
  writeSingleInstruction(localData, numM1Cycles, 2, 2, addr);



  
  if (highByte(inst.opCode) != 0){
    data[2]=localData[4];
    data[3]=localData[5];
  } else {
    data[2]=localData[3];
    data[3]=localData[4];
  }
  
}
