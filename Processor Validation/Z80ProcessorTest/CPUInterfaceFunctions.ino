//Read the address currently on the Address Bus
uint16_t readAddressBus(int delayTime) {

  //The address bus may need time to stabilize
  if (delayTime > 0) {
    delayMicroseconds(delayTime);
  }
  uint16_t curAddr = 0;

  //The low byte of the address bus is on PORT A
  //The high byte is on PORT C and needs to be shifted left by 8 positions
  curAddr=PINA + (PINC << 8);
  return curAddr;
}

//Set the Data lines
void writeData(byte data) {

  //Set all data lines to output
  DDRL = 0xFF;

  //Set all data lines simultaneously
  PORTL = data;
}

//Read the address currently on the Address Bus
uint16_t readDataBus(int delayTime) {

  //The data bus may need time to stabilize
  if (delayTime > 0) {
    delayMicroseconds(delayTime);
  }

  uint16_t curData = 0;

  //Set all data lines to input
  DDRL=0x00;

  //Read all data lines concurrently
  curData = PINL;

  return curData;
}

void writeSingleInstruction(byte* data, int numM1Cycles, int numReadCycles, int numWriteCycles, uint16_t* addresses) {

  setNextCycle(numM1Cycles, numReadCycles, numWriteCycles, 0);
  CLEAR_PIN(PORTH,WAIT_ATM);
  delayMicroseconds(1/2*period);

  //Process the M1 cycles
  for (int i = 0; i < (numM1Cycles+numReadCycles+numWriteCycles); i++) {
    //There are three possible next cycles. M1, Read, Write

    //Execute all of the M1 cycles first
    if (nextCycle == 1) {

      needsCycleEnd=0;

      while (currentCycle != 1) {
        delayMicroseconds(1);
      }

      //Disable interrupts so that nothing interfers with setting the data lines
      noInterrupts();

      //Set the data lines as outputs    
      DDRL = 0xFF;

      //Set the Data lines
      PORTL=data[i];

      //Determine what cycle is next
      setNextCycle(numM1Cycles, numReadCycles, numWriteCycles, i+1);

      needsCycleEnd = 1;

      interrupts();

      //Wait for the next M1 cycle to start
      while (currentCycle != 0) {
        delayMicroseconds(1);
      }
      DDRL=0xFF;
      PORTL = 0x00;

    // Then execute the Read cycles
    } else if (nextCycle == 2) {

      needsCycleEnd=0;
      
      while (currentCycle != 2) {
        delayMicroseconds(1);
      }
    
      //Disable interrupts so that nothing interfers with setting the data lines
      noInterrupts();

      //Set the data lines as outputs    
      DDRL = 0xFF;

      //Set the Data lines
      PORTL=data[i];

      //Capture the address
      addresses[i]=PINA+(PINC << 8);

      //Determine what cycle is next
      setNextCycle(numM1Cycles, numReadCycles, numWriteCycles, i+1);      

      needsCycleEnd = 1;

      interrupts();

      //Wait for the next Read cycle to start
      while (currentCycle != 0) {
        delayMicroseconds(1);
      }
      DDRL=0xFF;
      PORTL = 0x00;

    //Then execute the Write cycles
    } else if (nextCycle == 3) {
    
      needsCycleEnd=0;

      //This will be true when MREQ goes low while M1, RD, and WR are all still high.
      //This is necessary because WR is only low for one clock cycle, so we need to give the ISR time 
      //to enable WAIT      
      while (currentCycle != 3) {
        delayMicroseconds(1);
      }

      //Wait one clock cycle to ensure that we are in the actual WR cycle
      delayMicroseconds(period/2);

      //Disable interrupts so that nothing interfers with setting the data lines
      noInterrupts();

      //Set the data lines as inputs    
      DDRL = 0x00;

      //Read the Data lines
      data[i]=PINL;

      //Determine what cycle is next
      setNextCycle(numM1Cycles, numReadCycles, numWriteCycles, i+1);
      
      needsCycleEnd = 1;

      //Reenable interrupts
      interrupts();

      //Wait for the next Write cycle
      while (currentCycle != 0) {
        delayMicroseconds(1);
      }

      DDRL=0xFF;
      PORTL = 0x00;
    //If we get here, we must have a timing issue
    } else {
            Serial.print(F("In cycle: "));
      Serial.print(i);
      Serial.println(F(" which does not match"));

    }

  }
  interrupts();
  nextCycle=0;
}


void setNextCycle (int numM1Cycles, int numReadCycles, int numWriteCycles, int i) {

  if(numM1Cycles > (i)) {
    nextCycle = 1;
  } else if (numReadCycles > (i - numM1Cycles)) {
    nextCycle = 2;
  } else if (numWriteCycles > (i - numM1Cycles - numReadCycles)) {
    nextCycle = 3;
  } else  if (numM1Cycles+numReadCycles+numWriteCycles == (i)) {
    nextCycle = 0;
  } else {
    Serial.println ("Error in cycle detection");
    return;
  }

  return;
}

//Define the ISR, this needs to stay lean
void checkCycle() {

  //Get the current state and compare it to the lookup table
  int currentState = PINB;
  currentCycle = cycleTable[currentState];

  //Set WAIT accordingly
  if (currentCycle == nextCycle && needsCycleEnd == 0 && nextCycle != 0){
    CLEAR_PIN(PORTH, WAIT_ATM);
  } else if (currentCycle == 0 && needsCycleEnd == 1) {
    SET_PIN(PORTH, WAIT_ATM);
    needsCycleEnd = 0;
  } else {
    SET_PIN(PORTH, WAIT_ATM);
  }
}
