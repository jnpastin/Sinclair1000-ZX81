  //Directly modify the state of GPIO pins
  //This is significantly faster than digitalRead or digitalWrite
  #define SET_PIN(port, mask) ((port) |= (mask))
  #define CLEAR_PIN(port, mask) ((port) &= ~(mask))
//
//Address pins are mostly on PORT D on the Arduino, with the exception of pin 8 (A6 on 4116) which is on PORT B
const int ADDR_PINS=7;
const int addrPins[ADDR_PINS] = {2, 3, 4, 5, 6, 7, 8};

//Control pins are all on PORT B, Define them and their respective bitmasks
const int Din  = 9;    
const int Din_Mask = 2;

const int RAS  = 10; 
const int RAS_Mask = 4;

const int CAS  = 11;   
const int CAS_Mask = 8;

const int WRITE = 12;   
const int WRITE_Mask = 16;

const int Dout = 13;   
const int Dout_Mask = 32;

const int ROWS=128;
const int COLS=128;

// Buffer holds expected values for random pattern tests
uint8_t randBuffer[ROWS][COLS];


//Function prototyping
void setAddr(uint8_t addr);
void writeDRAM(uint8_t row, uint8_t col, uint8_t data);
uint8_t readDRAM(uint8_t row, uint8_t col);
uint8_t randomBit();


void setup() {
  Serial.begin(115200);
  for (int i = 0; i < ADDR_PINS; i++) {
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

  Serial.println("4116 DRAM Full Test Suite: ");
  randomSeed(micros());
  delay(1000);

  int errorCount=0;
  int successCount=0;
  for (int i=0; i<128; i++){
    uint8_t currData=1;
    writeDRAM(i, i, currData);
    uint8_t retrievedValue=readDRAM(i, i);
    if (currData == retrievedValue){
      successCount++;
    }else {
      errorCount++;
      /*Serial.print(" For Row: ");
      Serial.print(i);
      Serial.print(" and Col: ");
      Serial.print(i);
      Serial.print(" Expected: ");
      Serial.print(currData);
      Serial.print(" Received: ");
      Serial.println (retrievedValue);*/
    }
  }


  Serial.print("Successes: ");
  Serial.print(successCount);
  Serial.print(" Failures: ");
  Serial.println(errorCount);
}

void loop() {
  // put your main code here, to run repeatedly:

}

void setAddr(uint8_t addr) {

  //Write the first 6 bits of the address to PORT D pins 2-7
  PORTD = (PORTD & 0x03) | (addr << 2);

  //Write the 7th bit to PORT B pin 1
  PORTB = (PORTB & 0xFE) | (addr >> 6);

}

void writeDRAM(uint8_t row, uint8_t col, uint8_t data) {
  //This assumes a standard "early" write cycle
  //In this situation, WRITE goes low before CAS.  The chip will reference the setup/hold times to CAS going low

  //Din was likely set to high Z, set it back to output
  pinMode(Din, OUTPUT);

  //Set the data pin
  if (data){
    SET_PIN(PORTB, Din_Mask);
  } else {
    CLEAR_PIN(PORTB, Din_Mask);
  }


  //Latch in the row address
  setAddr(row);
  CLEAR_PIN(PORTB, RAS_Mask);

  //activate Write Enable
  CLEAR_PIN(PORTB, WRITE_Mask);

  //Latch in the column address, data will be sampled when CAS goes low
  setAddr(col);
  CLEAR_PIN(PORTB, CAS_Mask);
  delayMicroseconds(1);

  //Deactivate all control pins
  SET_PIN(PORTB, (WRITE_Mask+RAS_Mask+CAS_Mask));
}

uint8_t readDRAM(uint8_t row, uint8_t col) {

  //Set the memory's input pin to high-Z during reading
  pinMode(Din, INPUT);      // Din should be high-Z when reading

  //Ensure that the Write pin is high
  SET_PIN(PORTB, WRITE_Mask);

  //Latch in the row address
  setAddr(row);
  CLEAR_PIN(PORTB, RAS_Mask);

  //Latch in the column address
  setAddr(col);
  CLEAR_PIN(PORTB, CAS_Mask);
  delayMicroseconds(1);


  uint8_t val = (PINB & Dout_Mask)<<5;

  //Deactivate control pins
  SET_PIN(PORTB, (RAS_Mask+CAS_Mask));
  return val;
}

uint8_t randomBit() {
  if (random(0, 1000) < 500) {
    return 0;
  } else {
    return 1; 
  }
}