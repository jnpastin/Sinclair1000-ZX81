#include <digitalWriteFast.h>


void writeSingleInstruction(byte* data, int numM1Cycles, int numReadCycles, int numWriteCycles, uint16_t* addresses);
void writeData(byte data);
uint16_t readDataBus(int delayTime);
uint16_t readAddressBus(int delayTime);

//
//Define some Macros
//

  //Delay by n ATMega2560 cycles, this should be ~1/16th of a Z80 cycle
  #define DELAY_CYCLES(n) __builtin_avr_delay_cycles(n)

  //Directly modify the state of GPIO pins
  //This is significantly faster than digitalRead or digitalWrite
  #define SET_PIN(port, mask) ((port) |= (mask))
  #define CLEAR_PIN(port, mask) ((port) &= ~(mask))

//

//
//Define Pins
//

  //Data Bus
  //Port L on ATMega2560
  const int D_0 = 49;
  const int D_1 = 48;
  const int D_2 = 47;
  const int D_3 = 46;
  const int D_4 = 45;
  const int D_5 = 44;
  const int D_6 = 43;
  const int D_7 = 42;
  const int DataBus[] = { D_0, D_1, D_2, D_3, D_4, D_5, D_6, D_7 };

  //Address Bus
  //Low Byte - Port A on ATMega2560
  //High Byte - Port C on ATMega2560
  const int A_0 = 22;
  const int A_1 = 23;
  const int A_2 = 24;
  const int A_3 = 25;
  const int A_4 = 26;
  const int A_5 = 27;
  const int A_6 = 28;
  const int A_7 = 29;
  const int A_8 = 37;
  const int A_9 = 36;
  const int A_10 = 35;
  const int A_11 = 34;
  const int A_12 = 33;
  const int A_13 = 32;
  const int A_14 = 31;
  const int A_15 = 30;
  const int AddressBus[] = { A_0, A_1, A_2, A_3, A_4, A_5, A_6, A_7, A_8, A_9, A_10, A_11, A_12, A_13, A_14, A_15 };

  //System Control
  //Port B on ATMega2560
  const int M1 = 12;    
  const int MREQ = 52;  
  const int IORQ = 51;  
  const int RD = 50;    
  const int WR = 10;    
  const int RFSH = 13;  
  const int HALT = 4;  

  //CPU Control
  //Port H on ATMega2560
  const int WAIT = 6;   
  const int INT = 17;   
  const int NMI = 16;   
  const int RESET = 9;  
  const int CLK = 7;    

  //CPU Bus Control
  //Port H on ATMega2560
  const int BUSRQ = 25;   
  const int BUSAK = 11;   


// 


//
//Define value bitmasks
//

  //Data Bus
  const int D_0_Mask = 1;
  const int D_1_Mask = 2;
  const int D_2_Mask = 4;
  const int D_3_Mask = 8;
  const int D_4_Mask = 16;
  const int D_5_Mask = 32;
  const int D_6_Mask = 64;
  const int D_7_Mask = 128;
  const int DataBusMask[] = { D_0_Mask, D_1_Mask, D_2_Mask, D_3_Mask, D_4_Mask, D_5_Mask, D_6_Mask, D_7_Mask };

  //Address Bus
  const int A_0_Mask = 1;
  const int A_1_Mask = 2;
  const int A_2_Mask = 4;
  const int A_3_Mask = 8;
  const int A_4_Mask = 16;
  const int A_5_Mask = 32;
  const int A_6_Mask = 64;
  const int A_7_Mask = 128;
  const int A_8_Mask = 256;
  const int A_9_Mask = 512;
  const int A_10_Mask = 1024;
  const int A_11_Mask = 2048;
  const int A_12_Mask = 4096;
  const int A_13_Mask = 8192;
  const int A_14_Mask = 16384;
  const int A_15_Mask = 32768;
  const int AddressBusMask[] = { A_0_Mask, A_1_Mask, A_2_Mask, A_3_Mask, A_4_Mask, A_5_Mask, A_6_Mask, A_7_Mask, A_8_Mask, A_9_Mask, A_10_Mask, A_11_Mask, A_12_Mask, A_13_Mask, A_14_Mask, A_15_Mask };
//

//
//Define ATMega2560 port bitmasks
//
  //Port L (Data Bus)
  const int D_0_ATM = 1;
  const int D_1_ATM = 2;
  const int D_2_ATM = 4;
  const int D_3_ATM = 8;
  const int D_4_ATM = 16;
  const int D_5_ATM = 32;
  const int D_6_ATM = 64;
  const int D_7_ATM = 128;

  //Port A (Address Bus - Low Byte)
  const int A_0_ATM = 1;
  const int A_1_ATM = 2;
  const int A_2_ATM = 4;
  const int A_3_ATM = 8;
  const int A_4_ATM = 16;
  const int A_5_ATM = 32;
  const int A_6_ATM = 64;
  const int A_7_ATM = 128;

  //Port C (Address Bus - High Byte)
  const int A_8_ATM = 1;
  const int A_9_ATM = 2;
  const int A_10_ATM = 4;
  const int A_11_ATM = 8;
  const int A_12_ATM = 16;
  const int A_13_ATM = 32;
  const int A_14_ATM = 64;
  const int A_15_ATM = 128;

  //Port B (System Control)
  const int HALT_ATM = 1;
  const int MREQ_ATM = 2;
  const int IORQ_ATM = 4;
  const int RD_ATM = 8;
  const int WR_ATM = 16;
  const int BUSAK_ATM = 32;
  const int M1_ATM = 64;
  const int RFSH_ATM = 128;

  //Port H (CPU Control)
  const int INT_ATM = 1;
  const int NMI_ATM = 2;
  //Port H Pin 2 is not exposed
  const int WAIT_ATM = 8;
  const int CLK_ATM = 16;
  const int BUSRQ_ATM = 32;
  const int RESET_ATM = 64;


  //Some timing cycle bitmasks
  //These are to bitwise AND with the appropriate ports to get the correct signals
  // eg: if (PINH & M1_Cycle == M1_Cycle_Target) {...}

    //OpCode Fetch portion of the M1 Cycle is when M1, MREQ, and RD are LOW with RFSH being HIGH
    const int M1_Cycle = MREQ_ATM + RD_ATM + M1_ATM + RFSH_ATM;
    const int M1_Cycle_Target = RFSH_ATM;

    //Refresh portion of the M1 Cycle is when MREQ and RFSH are LOW with M1 and RD being HIGH
    const int RFSH_Cycle = MREQ_ATM + RFSH_ATM + M1_ATM;
    const int RFSH_Cycle_Target = M1_ATM;

    //Read Cycle is when MREQ and RD are LOW with M1 being HIGH
    const int RD_Cycle = MREQ_ATM + RD_ATM + M1_ATM;
    const int RD_Cycle_Target = M1_ATM;

    //Write Cycle is when MREQ and WR are LOW 
    const int WR_Cycle = MREQ_ATM + WR_ATM;
    const int WR_Cycle_Target = 0;

    //

//

volatile int currentCycle=0;
volatile int currentCycleTarget=0;

ISR(PCINT0_vect){
  if ((PINB & currentCycle) == currentCycleTarget ){
    CLEAR_PIN(PORTH, WAIT_ATM);
  }
}

void activateWait();
void deactivateWait();

void setup() {

  Serial.begin(115200);
  //Set up the clock
  //

  pinMode( CLK, OUTPUT );
  pinMode(WAIT,OUTPUT);

  //WGM=15 Fast PWM, TOP=OCR1A, BOTTOM=0
  //prescaler = /1 (16MHz clock rate)
  TCCR4A = _BV(COM1B1) | _BV(WGM11) | _BV(WGM10);
  TCCR4B = _BV(WGM13) | _BV(WGM12) | _BV(CS10);
  OCR4A = 15;
  OCR4B = 7; 

//Enable Pin Change Interrupts on Port B
PCICR|=B00000001;

//Enable pins for pin change interrupts
PCMSK0|=M1_ATM|RFSH_ATM|RD_ATM|WR_ATM|MREQ_ATM;

//Data Bus
  //These can be either input or output, initializing as input
  pinMode(D_0, INPUT);
  pinMode(D_1, INPUT);
  pinMode(D_2, INPUT);
  pinMode(D_3, INPUT);
  pinMode(D_4, INPUT);
  pinMode(D_5, INPUT);
  pinMode(D_6, INPUT);
  pinMode(D_7, INPUT);

  //Address Bus
  pinMode(A_0, INPUT);
  pinMode(A_1, INPUT);
  pinMode(A_2, INPUT);
  pinMode(A_3, INPUT);
  pinMode(A_4, INPUT);
  pinMode(A_5, INPUT);
  pinMode(A_6, INPUT);
  pinMode(A_7, INPUT);
  pinMode(A_8, INPUT);
  pinMode(A_9, INPUT);
  pinMode(A_10, INPUT);
  pinMode(A_11, INPUT);
  pinMode(A_12, INPUT);
  pinMode(A_13, INPUT);
  pinMode(A_14, INPUT);
  pinMode(A_15, INPUT);

  //System Control
  pinMode(M1, INPUT);
  pinMode(MREQ, INPUT);
  pinMode(IORQ, INPUT);
  pinMode(RD, INPUT);
  pinMode(WR, INPUT);
  pinMode(RFSH, INPUT);

  //CPU Control
  pinMode(HALT, INPUT);
  pinMode(WAIT, OUTPUT);
  pinMode(INT, OUTPUT);
  pinMode(NMI, OUTPUT);
  pinMode(RESET, OUTPUT);
  pinMode(CLK, OUTPUT);

  //CPU Bus Control
  pinMode(BUSRQ, OUTPUT);
  pinMode(BUSAK, INPUT);

  //Initialize pins to CPU
  digitalWriteFast(WAIT, HIGH);
  digitalWriteFast(INT, HIGH);
  digitalWriteFast(NMI, HIGH);
  digitalWriteFast(RESET, HIGH);
  digitalWriteFast(BUSRQ, HIGH);
   //Set the CPU Control Pins
  digitalWriteFast(INT, HIGH);
  digitalWriteFast(NMI, HIGH);
  digitalWriteFast(BUSRQ, HIGH);

  //
  //Issue a reset to the CPU
  //
  
  //CLEAR_PIN(PINH, WAIT_ATM);
  digitalWriteFast(RESET, LOW);
  delay(10);
  digitalWriteFast(RESET, HIGH);


  Serial.println("Starting");
  //Write '0x55' to the A register
  uint16_t addr[]={0x00, 0x00, 0x00, 0x00};
  byte localData[]={0x3E,0x55,0x00,0x00};
  bool error=false;


  for (int j=0; j<256
  ; j++){

    localData[0]=0x3E;
    localData[1]=j;
    writeSingleInstruction(localData, 1, 1, 0, addr);
    
    for (int i=0; i<4; i++){
      localData[i]=0x00;
    }
    
    localData[0]=0x32;
    writeSingleInstruction(localData, 1, 2, 1, addr);

    /*if (localData[3] != j){
      error=true;
    }
    Serial.print("Sent: ");
    Serial.print(j,HEX);
    Serial.print(" got: ");
    Serial.print(localData[3]);*/
  }
  if (error){
    Serial.println("Fail");
  } else {
    Serial.println("Pass");
  }
 
  digitalWriteFast(WAIT,LOW);
  

}

void loop() {



}
void writeSingleInstruction(byte* data, int numM1Cycles, int numReadCycles, int numWriteCycles, uint16_t* addresses) {
  byte curData=0;
  uint16_t curAddr=0;
  
  //Process the M1 cycles
  for (int i = 0; i < (numM1Cycles+numReadCycles+numWriteCycles); i++) {
    //There are three possible next cycles. M1, Read, Write

    while ((PINB & M1_Cycle) != M1_Cycle_Target){
      DELAY_CYCLES(1);
    }


    //Execute all of the M1 cycles first
    if (numM1Cycles >= (i + 1)) {

      //Prepare for the next interrupt
      currentCycle=M1_Cycle;
      currentCycleTarget=M1_Cycle_Target;

      //Delay until the interrupt triggers and activates wait.  This indicates the beginning of the M1 Cycle
      do{
        DELAY_CYCLES(1);
      }while(PINH & WAIT_ATM);
    Serial.println("looking for M1");
      //Set the data lines to outputs
      DDRL=0xFF;
      //Set the Data lines
      PORTL=data[i];

      //Prepare for the Refresh Cycle
      currentCycle=RD_Cycle;
      currentCycleTarget=RD_Cycle_Target;
    //Serial.println("looking for RFSH");
      //Deactivate wait
      SET_PIN(PORTH, WAIT_ATM);

      //Delay until the interrupt triggers and activates wait.  This indicates the end of the Refresh Cycle
/*      while(PINH &WAIT_ATM) {
        DELAY_CYCLES(1);
      }

      Serial.println("Matched");
*/
    // Then execute the Read cycles
    } else if (numReadCycles >= (i - numM1Cycles + 1)) {

      //Prepare for the Read Cycle
      currentCycle=RD_Cycle;
      currentCycleTarget=RD_Cycle_Target;

      //Delay until the interrupt triggers and activates wait.  This indicates the beginning of the Read Cycle.
      do{
        DELAY_CYCLES(1);
      }while(PINH & WAIT_ATM);

      //Set the data lines to outputs
      DDRL=0xFF;
      //Set the Data lines
      PORTL = data[i];
      
      //Capture the memory address being read
      addresses[i]=PINA + (PINC << 8);

      //Deactivate Wait
      SET_PIN(PORTH, WAIT_ATM);

    //Then execute the Write cycles
    } else if (numWriteCycles >= (i - numM1Cycles - numReadCycles + 1)) {
    
      //Prepare for the Write Cycle
      currentCycle=WR_Cycle;
      currentCycleTarget=WR_Cycle_Target;

      //Delay until the interrupt triggers and activates wait.  This indicates the beginning of the Write Cycle.
      do{
        DELAY_CYCLES(1);
      }while(PINH & WAIT_ATM);

      //Set the data lines to outputs
      DDRL=0x00;
      //Read the Data Bus and the Address Bus
      data[i]=PINL;
      addresses[i]=PINA + (PINC << 8);


      //Tell the CPU that we have read the data
      SET_PIN(PORTH, WAIT_ATM);

    //If we get here, we must have a timing issue
    } else {
            Serial.print(F("In cycle: "));
      Serial.print(i);
      Serial.println(F(" which does not match"));

    }
  }
}

//Read the address currently on the Address Bus
uint16_t readAddressBus(int delayTime) {

  //The address bus may need time to stabilize
  if (delayTime > 0) {
    DELAY_CYCLES(delayTime);
  }
  bool value;
  uint16_t curAddr = 0;

  curAddr = PINA + (PINC << 8);
  return curAddr;
}

void writeData(byte data) {
  //Write the bitwise AND of the data byte and each bitmask onto the appropriate pins
  PORTL = data;
}

//Read the address currently on the Address Bus
uint16_t readDataBus(int delayTime) {

  //The data bus may need time to stabilize
  if (delayTime > 0) {
    DELAY_CYCLES(delayTime);
  }

  uint16_t curData = 0;
  curData=PINL;
  return curData;
}

void activateWait(){
  if ((PINB & currentCycle) == currentCycleTarget ){
    CLEAR_PIN(PORTH, WAIT_ATM);
  }
  Serial.println("In ISR");
}

void deactivateWait(){
  if ((PINB & currentCycle) == currentCycleTarget ){
    SET_PIN(PORTH, WAIT_ATM);
  }
}
