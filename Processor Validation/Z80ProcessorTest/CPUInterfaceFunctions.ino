uint16_t readAddressBus(int delayTime) {

  //The address bus may need time to stabilize
  if (delayTime > 0) {
    delayMicroseconds(delayTime);
  }

  uint16_t curAddr = 0;

  //Read each line and add the bitmask for that line if it is high
  for (int i = 0; i < 16; i++) {
    if (digitalReadFast(AddressBus[i])) {
      curAddr += AddressBusMask[i];
    }
  }
  return curAddr;
}

void writeData(byte data) {
  //Write the bitwise AND of the data byte and each bitmask onto the appropriate pins
  for (int i = 0; i < 8; i++) {
    pinMode(DataBus[i], OUTPUT);
    digitalWriteFast(DataBus[i], data & DataBusMask[i]);
  }
  //Serial.println("");
}

void writeSingleInstruction(byte data[], int numM1Cycles, int numReadCycles, int numWriteCycles) {

  //Process the M1 cycles
  for (int i = 0; i < numM1Cycles; i++) {
    digitalWriteFast(WAIT, HIGH);

    //Wait for the CPU to be ready
    while (digitalReadFast(RD) || digitalReadFast(MREQ) || digitalReadFast(M1) || !digitalReadFast(RFSH)) {
      delayMicroseconds(1);
    }

    //Tell the CPU to write data
    digitalWriteFast(WAIT, LOW);

    //Set the Data lines
    writeData(data[i]);


    //Tell the CPU that the data is valid
    digitalWriteFast(WAIT, HIGH);


    while (digitalReadFast(RFSH)) {
      delayMicroseconds(1);
    }

    for (int i = 0; i < 8; i++) {
      pinMode(DataBus[i], INPUT);
    }

    //Wait for the end of the cycle, there are three possible next cycles
    if (numM1Cycles > (i + 1)) {
      //Wait for the next M1 cycle
      while (digitalReadFast(RD) || digitalReadFast(MREQ) || digitalReadFast(M1) || !digitalReadFast(RFSH)) {
        delayMicroseconds(1);
      }
    } else if (numReadCycles > (i - numM1Cycles + 1)) {
      //Wait for the next Read cycle
      while (digitalReadFast(RD) || digitalReadFast(MREQ) || !digitalReadFast(M1)) {
        delayMicroseconds(1);
      }
    } else if (numWriteCycles > (i - numM1Cycles - numReadCycles + 1)) {
      //Wait for the next Read cycle
      while (digitalReadFast(WR) || digitalReadFast(MREQ) || !digitalReadFast(M1)) {
        delayMicroseconds(1);
      }
    }
    digitalWriteFast(WAIT, LOW);
  }
}