//Read the address currently on the Address Bus
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
//  Serial.println(data,HEX);
  for (int i = 0; i < 8; i++) {
    pinMode(DataBus[i], OUTPUT);
    digitalWriteFast(DataBus[i], data & DataBusMask[i]);
//    Serial.println(data & DataBusMask[i],BIN);
  }
//  Serial.println("");
}

//Read the address currently on the Address Bus
uint16_t readDataBus(int delayTime) {

  //The data bus may need time to stabilize
  if (delayTime > 0) {
    delayMicroseconds(delayTime);
  }

  uint16_t curData = 0;
  //Read each line and add the bitmask for that line if it is high
  for (int i = 0; i < 8; i++) {
    if (digitalReadFast(DataBus[i])) {
      curData += DataBusMask[i];
    }
  }
  return curData;
}

void writeSingleInstruction(byte* data, int numM1Cycles, int numReadCycles, int numWriteCycles) {

byte curData=0;
  //Process the M1 cycles
  for (int i = 0; i < (numM1Cycles+numReadCycles+numWriteCycles); i++) {

    //There are three possible next cycles. M1, Read, Write

    //Execute all of the M1 cycles first
    if (numM1Cycles >= (i + 1)) {

      //We always have to wait for the second half of a T cycle to deactivate Wait
      while (digitalReadFast(CLK)) {
        delayMicroseconds(1);
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

      //M1 cycles have a DRAM refresh cycle buried in them, wait until it is over
      while (digitalReadFast(RFSH)) {
        delayMicroseconds(1);
      }
      while (!digitalReadFast(RFSH)) {
        delayMicroseconds(1);
      }

      //Tell the CPU to wait while we process everything
      digitalWriteFast(WAIT, LOW);

    // Then execute the Read cycles
    } else if (numReadCycles >= (i - numM1Cycles + 1)) {

      while (digitalReadFast(CLK)) {
        delayMicroseconds(1);
      }
      digitalWriteFast(WAIT, HIGH);

      //Wait for the next Read cycle to start
      while (digitalReadFast(RD) || digitalReadFast(MREQ)) {// || !digitalReadFast(M1) || !digitalReadFast(RFSH)) {
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

      //Wait until the end of the current Read Cycle
      while (!digitalReadFast(RD) || !digitalReadFast(MREQ)) {
        delayMicroseconds(1);
      }

      //Tell the CPU to wait while we process everything
      digitalWriteFast(WAIT, LOW);

    //Then execute the Write cycles
    } else if (numWriteCycles >= (i - numM1Cycles - numReadCycles + 1)) {
      
      while (digitalReadFast(CLK)) {
        delayMicroseconds(1);
      }
      digitalWriteFast(WAIT, HIGH);

      //Wait for the next Write cycle
      while (!digitalReadFast(RD) || digitalReadFast(MREQ) || !digitalReadFast(M1)) {
        delayMicroseconds(1);
      }

      //Tell the CPU to wait while data is being written
      digitalWriteFast(WAIT, LOW);

      //Read the Data lines
      curData=readDataBus(0);

      //Tell the CPU that we have read the data
      while (digitalReadFast(CLK)) {
        delayMicroseconds(1);
      }
      digitalWriteFast(WAIT, HIGH);

      //Write the retrieved data back to the passed array
      data[i]=curData;

      //Wait for the end of the Write cycle
      while (!digitalReadFast(WR) || !digitalReadFast(MREQ)){
        delayMicroseconds(1);
      }

      //Tell the CPU to wait while we process everything
      digitalWriteFast(WAIT, LOW);

    //If we get here, we must have a timing issue
    } else {
            Serial.print(F("In cycle: "));
      Serial.print(i);
      Serial.println(F(" which does not match"));

    }

    //Clean up by ensuring all of the data pins are set back to input
    for (int i = 0; i < 8; i++) {
      pinMode(DataBus[i], INPUT);
    }

    //Tell the CPU to wait while we process everything
    //digitalWriteFast(WAIT, LOW);
  }
}