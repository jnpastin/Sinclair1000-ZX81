/*
+-----------------------------------------+
| Timex Sinclair TS1016 DRAM Tester.      |
| Jeremy Pastin   10/25                    |
+-----------------------------------------+
*/


//
//Define some macros
//

//Directly modify the state of GPIO pins
//This is significantly faster than digitalRead or digitalWrite
#define SET_PIN(port, mask) ((port) |= (mask))
#define CLEAR_PIN(port, mask) ((port) &= ~(mask))

//Timing is pretty critical, this provides more granularity than delayMicroseconds
#define DELAY_CYCLES(n) __builtin_avr_delay_cycles(n)


//
//Define some constants
//

//Address pins are mostly on PORT D on the Arduino, with the exception of pin 8 (A6 on 4116) which is on PORT B
const uint8_t ADDR_PINS=7;
const uint8_t addrPins[ADDR_PINS] = {2, 3, 4, 5, 6, 7, 8};

//Control pins are all on PORT B, Define them and their respective bitmasks
const uint8_t Din  = 9;    
const uint8_t Din_Mask = 2;

const uint8_t RAS  = 10; 
const uint8_t RAS_Mask = 4;

const uint8_t CAS  = 11;   
const uint8_t CAS_Mask = 8;

const uint8_t WRITE = 12;   
const uint8_t WRITE_Mask = 16;

const uint8_t Dout = 13;   
const uint8_t Dout_Mask = 32;

const uint8_t ROWS=128;
const uint8_t COLS=128;
const uint8_t readDelay=4;

//
//Function prototyping
//
void setAddr(uint8_t addr);
void writeDRAM(uint8_t row, uint8_t col, uint8_t data);
uint8_t readDRAM(uint8_t row, uint8_t col);
uint8_t randomBit();


void setup() {

  //Start the serial port
  Serial.begin(115200);

  //Configure pins
  for (uint8_t i = 0; i < ADDR_PINS; i++) {
    pinMode(addrPins[i], OUTPUT);
    digitalWrite(addrPins[i], LOW);
  }
  pinMode(Din, OUTPUT);
  pinMode(Dout, INPUT);
  pinMode(RAS, OUTPUT);
  pinMode(CAS, OUTPUT);
  pinMode(WRITE, OUTPUT);

  digitalWrite(RAS, HIGH);
  digitalWrite(CAS, HIGH);
  digitalWrite(WRITE, HIGH);

  //Print the title
  Serial.println(F("Timex Sinclair TS1016 - 4116 DRAM Full Test Suite: "));
  Serial.println();

  //Initialize some variables
  char dataPatterns[4][25]={"all zeroes", "all ones", "random data", "toggling returned data"};
  uint16_t errorCount=0;
  uint16_t successCount=0;
  uint8_t sentData=0;

  //Initialize the chip.  A minimum of 8 cycles are required
  for (uint8_t j=0; j<10; j++) {
    setAddr(j);
    CLEAR_PIN(PORTB,RAS_Mask);
    delayMicroseconds(1);
    SET_PIN(PORTB,RAS_Mask);
    delay(1);
  }

  //
  //Test Early Writes and Reads
  //
  for (uint8_t k=0; k<3; k++){
    errorCount=0;
    successCount=0;
    Serial.print (F("Testing Early Write and Read with "));
    Serial.print (dataPatterns[k]);
    for (uint8_t i=0; i<ROWS; i++){
      for (uint8_t j=0; j<COLS; j++){
        switch (k) {
          case 0:
            sentData=0;
            break;
          case 1:
            sentData=1;
            break;
          case 2:
            sentData=randomBit();
            break;
        }
        writeDRAM(i, j, sentData);
        uint8_t retrievedData=readDRAM(i, j);
        if (sentData == retrievedData){
          successCount++;
        }else {
          errorCount++;
        }
      }
      if (i%4==0){
        Serial.print(F("."));
      }
    }
    if (successCount == 16384 && errorCount == 0 ){
      Serial.println (F("Pass."));
    } else {
      Serial.print (F("Fail.  Error Count: "));
      Serial.println (errorCount);
    }
  }

  Serial.println();

  //
  //Test Read-Modify-Write cycle
  //
  for (uint8_t k=0; k<4; k++){
    errorCount=0;
    successCount=0;
    Serial.print (F("Testing Read-Modify-Write with "));
    Serial.print (dataPatterns[k]);
    for (uint8_t i=0; i<ROWS; i++){
      for (uint8_t j=0; j<COLS; j++){
        bool toggle=false;
        switch (k) {
          case 0:
            sentData=0;
            break;
          case 1:
            sentData=1;
            break;
          case 2:
            sentData=randomBit();
            break;
          case 3:
            toggle=true;
            //This will ultimately get ignored
            sentData=0;
            break;
        }

        //First populate the memory with random data
        uint8_t initialData=randomBit();
        writeDRAM(i, j, initialData);

        //Perform the Read-Modify-Write Cycle
        uint8_t initialRead=readModifyWriteDRAM(i,j,sentData,toggle);        

        //Perform a Read to validate the previous Write
        uint8_t finalRead=readDRAM(i, j);

        if (initialData == initialRead){
          if (toggle){
            if (finalRead == initialData ^ 1){
              successCount++;
            } else {
              errorCount++;
            }
          } else if (finalRead == sentData){
            successCount++;
          } else {
            errorCount++;
          }
        } else {
          errorCount++;
        }
      }
      if (i%4==0){
        Serial.print(F("."));
      }
    }
    if (successCount == 16384 && errorCount == 0 ){
      Serial.println (F("Pass."));
    } else {
      Serial.print (F("Fail.  Error Count: "));
      Serial.println (errorCount);
    }
  }

  Serial.println();

  //
  //Test Page Writes and Reads along with RAS-Only Refreshes
  //
  for (uint8_t k=0; k<3; k++){
    errorCount=0;
    successCount=0;
    Serial.print (F("Testing Page Write, Page Read, and Refresh with "));
    Serial.print (dataPatterns[k]);
    for (uint8_t i=0; i<ROWS; i++){
      uint8_t inputData[16];
      switch (k) {
        case 0:
          for (uint8_t j=0; j<16; j++){
            inputData[j]=0;
          }
          break;
        case 1:
          for (uint8_t j=0; j<16; j++){
            inputData[j]=256;
          }
          break;
        case 2:
          for (uint8_t j=0; j<16; j++){
            inputData[j]=(uint8_t)random(0,256);
          }
          break;
      }

      //Write a whole page worth of data
      writePageDRAM(i, inputData);
      
      //Test RAS Only Refreshes
      for (uint8_t j=0; j<10; j++) {
        setAddr(i);
        CLEAR_PIN(PORTB,RAS_Mask);
        delayMicroseconds(1);
        SET_PIN(PORTB,RAS_Mask);
        delay(1);
      }

      //Read back the page worth of data
      uint8_t retrievedData[16];
      readPageDRAM(i, retrievedData);


      for (uint8_t j=0; j<16; j++)
        if (inputData[j] == retrievedData[j]){
        successCount++;
        }else {
        errorCount++;
        }
      if (i%4==0){
        Serial.print(F("."));
      }
    }
    if (successCount == 2048 && errorCount == 0 ){
      Serial.println (F("Pass."));
    } else {
      Serial.print (F("Fail.  Error Count: "));
      Serial.println (errorCount);
    }
  }
  Serial.println();
  Serial.println(F("Test Complete!"));
}

void loop() {
  //Nothing to do here
}

//Generate a random 1 or 0
uint8_t randomBit() {
  if (random(0, 1000) < 500) {
    return 0;
  } else {
    return 1; 
  }
}

//Populate the address pins with a 7 bit value
void setAddr(uint8_t addr) {

  //Write the first 6 bits of the address to PORT D pins 2-7
  PORTD = (PORTD & 0x03) | (addr << 2);

  //Write the 7th bit to PORT B pin 1
  PORTB = (PORTB & 0xFE) | (addr >> 6);

}

//Perform an early write
void writeDRAM(uint8_t row, uint8_t col, uint8_t data) {
  //In this situation, WRITE goes low before CAS.  The chip will reference the setup/hold times to CAS going low

  //Din was likely set to high Z, set it back to output
  pinMode(Din, OUTPUT);

  //Set the Din pin
  if (data){
    SET_PIN(PORTB, Din_Mask);
  } else {
    CLEAR_PIN(PORTB, Din_Mask);
  }

  //Latch in the row address
  setAddr(row);
  CLEAR_PIN(PORTB, RAS_Mask);

  //Activate Write Enable
  CLEAR_PIN(PORTB, WRITE_Mask);

  //Latch in the column address, data will be sampled when CAS goes low
  setAddr(col);
  CLEAR_PIN(PORTB, CAS_Mask);

  //Deactivate all control pins
  SET_PIN(PORTB, (WRITE_Mask+RAS_Mask+CAS_Mask));

  //Set the memory's input pin to high-Z during reading
  pinMode(Din, INPUT);     
}

//Perform a read 
uint8_t readDRAM(uint8_t row, uint8_t col) {

  //Ensure that the Write pin is high
  SET_PIN(PORTB, WRITE_Mask);

  //Latch in the row address
  setAddr(row);
  CLEAR_PIN(PORTB, RAS_Mask);

  //Latch in the column address
  setAddr(col);
  CLEAR_PIN(PORTB, CAS_Mask);

  //Delay a small amount to allow the memory to properly set the data
  DELAY_CYCLES(readDelay);

  //Read the data
  uint8_t val = ((PINB & Dout_Mask)>>5);

  //Deactivate control pins
  SET_PIN(PORTB, (RAS_Mask+CAS_Mask));
  
  return val;
}

//Perform a Read-Modify-Write Cycle
uint8_t readModifyWriteDRAM(uint8_t row, uint8_t col, uint8_t data, bool toggle) {
  //In this situation, CAS goes low before WRITE.  The chip will reference the setup/hold times to WRITE going low

  //Din was likely set to high Z, set it back to output
  pinMode(Din, OUTPUT);


  //Latch in the row address
  setAddr(row);
  CLEAR_PIN(PORTB, RAS_Mask);

  //Latch in the column address, data will be sampled when CAS goes low
  setAddr(col);
  CLEAR_PIN(PORTB, CAS_Mask);

  //Delay a small amount to allow the memory to properly set the data
  DELAY_CYCLES(readDelay);

  // Read the data
  uint8_t val = ((PINB & Dout_Mask) >> 5);

  // Set the Din pin
  if (toggle) {
    // Invert val and set Din accordingly
    uint8_t result = val ^ 1;

    // Set or clear Din using bitwise masking
    PORTB = (PORTB & ~Din_Mask) | (result ? Din_Mask : 0);
  } else {
        // Set Din based on 'data'
    PORTB = (PORTB & ~Din_Mask) | (data ? Din_Mask : 0);
  }

  //Activate Write Enable, Data will be written 
  CLEAR_PIN(PORTB, WRITE_Mask);

  //Deactivate all control pins
  SET_PIN(PORTB, (WRITE_Mask+RAS_Mask+CAS_Mask));

  //Set the memory's input pin to high-Z during reading
  pinMode(Din, INPUT); 

  return val;    
}

//Perform a page write
void writePageDRAM(uint8_t row, uint8_t* data) {
  //In this situation RAS stays low, while each column is strobed individually

  //Din was likely set to high Z, set it back to output
  pinMode(Din, OUTPUT);


  //Latch in the row address
  setAddr(row);
  CLEAR_PIN(PORTB, RAS_Mask);

  //The entire row is set sequentially without RAS deactivating
  for (uint8_t i=0; i<COLS; i++){

    //Set the data pin
    uint8_t byte=i/8;
    uint8_t bit=i%8;
    uint8_t bitValue=(data[byte]>>bit) & 1;

    if (bitValue)
    {
      SET_PIN(PORTB, Din_Mask);
    } else {
      CLEAR_PIN(PORTB, Din_Mask);
    }

    //Latch in the column address
    setAddr(i);

    //Activate Write Enable and CAS
    CLEAR_PIN(PORTB, WRITE_Mask + CAS_Mask);

    //Deactivate Write Enable and CAS
    SET_PIN(PORTB, (WRITE_Mask+CAS_Mask));
  }

  //Deactivate RAS
  SET_PIN(PORTB,RAS_Mask);

  //Set the memory's input pin to high-Z during reading
  pinMode(Din, INPUT);     
}

//Perform a page read 
void readPageDRAM(uint8_t row, uint8_t* data) {

  //Ensure that the Write pin is high
  SET_PIN(PORTB, WRITE_Mask);

  //Latch in the row address
  setAddr(row);
  CLEAR_PIN(PORTB, RAS_Mask);

  //The entire row is read sequentially without RAS deactivating
  for (uint8_t i=0; i<COLS; i++){

    uint8_t byte=i/8;
    uint8_t bit=i%8;

    //Latch in the column address
    setAddr(i);
    CLEAR_PIN(PORTB, CAS_Mask);

    //Delay a small amount to allow the memory to properly set the data
    DELAY_CYCLES(readDelay);

    //Read the data
    data[byte] = (data[byte] & ~(1 << bit)) | (((PINB & Dout_Mask)>>5)<<bit);

    //Deactivate CAS
    SET_PIN(PORTB, CAS_Mask);
  }

  //Deactivate RAS
  SET_PIN(PORTB, RAS_Mask)  ;
}