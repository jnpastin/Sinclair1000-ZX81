/*
+-----------------------------------------+
| Timex Sinclair 1000/ZX81 ROM Tester     |
| Jeremy Pastin   7/25                    |
+-----------------------------------------+
*/


//Define chip control pins
const int CS=5;
const int OE=4;
const int WE=3;

//Define RAM I/O pins
const int IO_1=13;
const int IO_2=12;
const int IO_3=11;
const int IO_4=10;
const int IO_5=9;
const int IO_6=8;
const int IO_7=7;
const int IO_8=6;

//Define program control pins
const int AddressSetEnable=2;
const int AddressHoldEnable=A2;
const int Halt=A0;
const int ManualStep=A1;

//Define RAM I/O pin bitmasks 
const int IO_1_Mask=1;     //binary 00000001
const int IO_2_Mask=2;     //binary 00000010
const int IO_3_Mask=4;     //binary 00000100
const int IO_4_Mask=8;     //binary 00001000
const int IO_5_Mask=16;    //binary 00010000
const int IO_6_Mask=32;    //binary 00100000
const int IO_7_Mask=64;    //binary 01000000
const int IO_8_Mask=128;   //binary 10000000 


void setup() {

  //Define vairables 
  int currentValue=0;

  //Set pin modes.  Note that the IO pins will be changing modes, and set later
  pinMode(CS, OUTPUT);
  pinMode(OE, OUTPUT);
  pinMode(WE, OUTPUT);
  pinMode(AddressSetEnable, OUTPUT);
  pinMode(AddressHoldEnable, INPUT);
  pinMode(Halt, INPUT);
  pinMode(ManualStep, INPUT);

  //Start the serial terminal.  This is necessary to output the contents of the ROM.
  Serial.begin(115200);
  
  //Count through all 2K addresses
  for (int i=0; i<2048; i++) {

    //Cycle through all possible 8 bit combinations
    for (int j=0; j<256; j++){

      //If the system is halted, wait for a manual step through
      if (digitalRead(Halt) == HIGH) {
        while (digitalRead(ManualStep) == LOW) {
          delay (1);
        }
      }

      //Reset the current value to 0
      currentValue=0;

      //Tell the other unit to start setting the address bits
      digitalWrite(AddressSetEnable, HIGH);

      //Wait for the other unit to confirm that its has set all of the address pins
      while (digitalRead(AddressHoldEnable) == LOW) {
        delay (1);
      }

      //Set the I/O pins to OUTPUT
      pinMode(IO_1, OUTPUT);
      pinMode(IO_2, OUTPUT);
      pinMode(IO_3, OUTPUT);
      pinMode(IO_4, OUTPUT);
      pinMode(IO_5, OUTPUT);
      pinMode(IO_6, OUTPUT);
      pinMode(IO_7, OUTPUT);
      pinMode(IO_8, OUTPUT);

      //For each I/O bit, do a bitwise AND with the mask and the current target address to get the intended state for that bit, then set it accordingly.
      //Yes, doing it this way is inelegant, but my focus is on readability
      if (j & IO_1_Mask){
        digitalWrite(IO_1, HIGH);
      } else {
        digitalWrite(IO_1, LOW);
      }

      if (j & IO_2_Mask){
        digitalWrite(IO_2, HIGH);
      } else {
        digitalWrite(IO_2, LOW);
      }

      if (j & IO_3_Mask){
        digitalWrite(IO_3, HIGH);
      } else {
        digitalWrite(IO_3, LOW);
      }
      
      if (j & IO_4_Mask){
        digitalWrite(IO_4, HIGH);
      } else {
        digitalWrite(IO_4, LOW);
      }
      
      if (j & IO_5_Mask){
        digitalWrite(IO_5, HIGH);
      } else {
        digitalWrite(IO_5, LOW);
      }
      
      if (j & IO_6_Mask){
        digitalWrite(IO_6, HIGH);
      } else {
        digitalWrite(IO_6, LOW);
      }
      
      if (j & IO_7_Mask){
        digitalWrite(IO_7, HIGH);
      } else {
        digitalWrite(IO_7, LOW);
      }
      
      if (j & IO_8_Mask){
        digitalWrite(IO_8, HIGH);
      } else {
        digitalWrite(IO_8, LOW);
      }

      //Set the control pins for writing.  Note that they are active LOW.
      digitalWrite(CS, LOW);
      digitalWrite(OE, HIGH);
      digitalWrite(WE, LOW);

      //Wait for the chip to complete the write
      delay(1);

      //Set the control pins for reading
      digitalWrite(CS, LOW);
      digitalWrite(OE, LOW);
      digitalWrite(WE, HIGH);

      //Wait for the chip to become ready
      delay(1);

      //Set the I/O pins to INPUT
      pinMode(IO_1, INPUT);
      pinMode(IO_2, INPUT);
      pinMode(IO_3, INPUT);
      pinMode(IO_4, INPUT);
      pinMode(IO_5, INPUT);
      pinMode(IO_6, INPUT);
      pinMode(IO_7, INPUT);
      pinMode(IO_8, INPUT);

      //For each I/O bit, if it is set, add the mask to the current Value
      if (digitalRead(IO_1) == HIGH) { 
        currentValue+=IO_1_Mask;
      }

      if (digitalRead(IO_2) == HIGH) { 
        currentValue+=IO_2_Mask;
      }

      if (digitalRead(IO_3) == HIGH) { 
        currentValue+=IO_3_Mask;
      }

      if (digitalRead(IO_4) == HIGH) { 
        currentValue+=IO_4_Mask;
      }

      if (digitalRead(IO_5) == HIGH) { 
        currentValue+=IO_5_Mask;
      }

      if (digitalRead(IO_6) == HIGH) { 
        currentValue+=IO_6_Mask;
      }

      if (digitalRead(IO_7) == HIGH) { 
        currentValue+=IO_7_Mask;
      }

      if (digitalRead(IO_8) == HIGH) { 
        currentValue+=IO_8_Mask;
      }

      //Display errors or a progress dot
      if (j != currentValue ){
        Serial.print("Memory error on word: ");
        Serial.print(i);
        Serial.print(".  Expected ");
        Serial.print(j);
        Serial.print(" but received ");
        Serial.println(currentValue);
      } else {
        Serial.print(".");
        if ((j)%128 == 0){
          Serial.println("");
        }
      }

      //Tell the other unit that the cycle has ended
      digitalWrite(AddressSetEnable, LOW);

      //Wait for the other unit to ACK by setting the Hold Enable LOW
      while (digitalRead(AddressHoldEnable) == HIGH){
        delay(1);
      }

      //If the system is halted, ensure that a button press only allows one cycle
      if (digitalRead(Halt) == HIGH) {
        while (digitalRead(ManualStep) == HIGH) {
          delay (1);
        }
      }

      //Disable the chip
      digitalWrite(CS, HIGH);
      digitalWrite(OE, HIGH);
      digitalWrite(WE, HIGH);
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
