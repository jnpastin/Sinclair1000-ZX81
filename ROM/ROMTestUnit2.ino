//Define address pins
const int A_11=4;
const int A_12=5;

//Define Chip Select pin
const int CS=3;

//Define ROM output pins
const int O_0=6;
const int O_1=7;
const int O_2=8;
const int O_3=9;
const int O_4=10;
const int O_5=11;
const int O_6=12;
const int O_7=13;

//Define control pins
const int AddressSetEnable=2;
const int AddressHoldEnable=A0;
const int Halt=A1;
const int ManualStep=A2;

//Define address pin bitmasks
const int A_11_Mask=2048; //binary 0100000000000
const int A_12_Mask=4096; //binary 1000000000000

//Define ROM output pin bitmasks 
const int O_0_Mask=1;     //binary 00000001
const int O_1_Mask=2;     //binary 00000010
const int O_2_Mask=4;     //binary 00000100
const int O_3_Mask=8;     //binary 00001000
const int O_4_Mask=16;    //binary 00010000
const int O_5_Mask=32;    //binary 00100000
const int O_6_Mask=64;    //binary 01000000
const int O_7_Mask=128;   //binary 10000000 



void setup() {
 
  int currentValue;

  //Set pin modes
  pinMode(A_11, OUTPUT);
  pinMode(A_12, OUTPUT);
  pinMode(CS, OUTPUT);
  pinMode(O_0, INPUT);
  pinMode(O_1, INPUT);
  pinMode(O_2, INPUT);
  pinMode(O_3, INPUT);
  pinMode(O_4, INPUT);
  pinMode(O_5, INPUT);
  pinMode(O_6, INPUT);
  pinMode(O_7, INPUT);
  pinMode(AddressSetEnable, OUTPUT);
  pinMode(AddressHoldEnable, INPUT);
  pinMode(Halt, INPUT);
  pinMode(ManualStep, INPUT);

  Serial.begin(115200);

  //Count through all 8K addresses
  for (int i=0; i<8192; i++) {

    if (digitalRead(Halt) == HIGH) {
      while (digitalRead(ManualStep) == LOW) {
        delay (1);
      }
    }

    //Reset the current value to 0
    currentValue=0;

    //Tell the other unit to start setting the address bits
    digitalWrite(AddressSetEnable, HIGH);

    //For each address bit, do a bitwise AND with the mask and the current target address to get the intended state for that bit, then set it accordingly.
    if (i & A_11_Mask){
      digitalWrite(A_11, HIGH);
    } else {
      digitalWrite(A_11, LOW);
    }
    if (i & A_12_Mask){
      digitalWrite(A_12, HIGH);
    } else {
      digitalWrite(A_12, LOW);
    }

    //Wait for the other unit to confirm that its has set all of the address pins
    while (digitalRead(AddressHoldEnable) == LOW) {
      delay (1);
    }

    //Set the Chip Select pin
    digitalWrite(CS, LOW);

    //Wait for the chip to become ready
    delay(1);

    //For each output bit, if it is set, add the mask to the current Value
    if (digitalRead(O_0) == HIGH) { 
      currentValue+=O_0_Mask;
    }
    if (digitalRead(O_1) == HIGH) { 
      currentValue+=O_1_Mask;
    }
    if (digitalRead(O_2) == HIGH) { 
      currentValue+=O_2_Mask;
    }
    if (digitalRead(O_3) == HIGH) { 
      currentValue+=O_3_Mask;
    }
    if (digitalRead(O_4) == HIGH) { 
      currentValue+=O_4_Mask;
    }
    if (digitalRead(O_5) == HIGH) { 
      currentValue+=O_5_Mask;
    }
    if (digitalRead(O_6) == HIGH) { 
      currentValue+=O_6_Mask;
    }
    if (digitalRead(O_7) == HIGH) { 
      currentValue+=O_7_Mask;
    }

    //Store the current value
    if (currentValue<16){
      Serial.print("0");
    }
    //Serial.print(highByte(currentValue), HEX);
    Serial.print(currentValue, HEX);



    //Tell the other unit that the cycle has ended
    digitalWrite(AddressSetEnable, LOW);

    //Wait for the other unit to ACK by setting the Hold Enable LOW
    while (digitalRead(AddressHoldEnable) == HIGH){
      delay(1);
    }

    if (digitalRead(Halt) == HIGH) {
      while (digitalRead(ManualStep) == HIGH) {
        delay (1);
      }
    }
    digitalWrite(CS, HIGH);
  }
}

void loop() {
  
}
