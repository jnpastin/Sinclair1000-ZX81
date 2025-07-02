//Define address pins
const int A_0=7;
const int A_1=6;
const int A_2=5;
const int A_3=4;
const int A_4=3;
const int A_5=2;
const int A_6=A1;
const int A_7=A0;
const int A_8=8;
const int A_9=9;
const int A_10=10;

//Define control pins
const int AddressSetEnable=11;
const int AddressHoldEnable=12;

//Define address pin bitmasks
const int A_0_Mask=1;     //binary 0000000000001
const int A_1_Mask=2;     //binary 0000000000010
const int A_2_Mask=4;     //binary 0000000000100
const int A_3_Mask=8;     //binary 0000000001000
const int A_4_Mask=16;    //binary 0000000010000
const int A_5_Mask=32;    //binary 0000000100000
const int A_6_Mask=64;    //binary 0000001000000
const int A_7_Mask=128;   //binary 0000010000000
const int A_8_Mask=256;   //binary 0000100000000
const int A_9_Mask=512;   //binary 0001000000000
const int A_10_Mask=1024; //binary 0010000000000



void setup() {

  //Set pin modes
  pinMode(A_0, OUTPUT);
  pinMode(A_1, OUTPUT);
  pinMode(A_2, OUTPUT);
  pinMode(A_3, OUTPUT);
  pinMode(A_4, OUTPUT);
  pinMode(A_5, OUTPUT);
  pinMode(A_6, OUTPUT);
  pinMode(A_7, OUTPUT);
  pinMode(A_8, OUTPUT);
  pinMode(A_9, OUTPUT);
  pinMode(A_10, OUTPUT);
  pinMode(AddressSetEnable, INPUT);
  pinMode(AddressHoldEnable, OUTPUT);


  //Count through all 8K addresses
  for (int i=0; i<8192; i++){


    //Tell the other unit that we are not ready yet
    digitalWrite(AddressHoldEnable, LOW);

    //Wait for the other unit to tell us that it is ready
    while (digitalRead(AddressSetEnable) == LOW) {
      delay(1);
    }

    //For each address bit, do a bitwise AND with the mask and the current target address to get the intended state for that bit, then set it accordingly.
    if (i & A_10_Mask){
      digitalWrite(A_10, HIGH);
    } else {
      digitalWrite(A_10, LOW);
    }
    if (i & A_9_Mask){
      digitalWrite(A_9, HIGH);
    } else {
      digitalWrite(A_9, LOW);
    }
    if (i & A_8_Mask){
      digitalWrite(A_8, HIGH);
    } else {
      digitalWrite(A_8, LOW);
    }
    if (i & A_7_Mask){
      digitalWrite(A_7, HIGH);
    } else {
      digitalWrite(A_7, LOW);
    }
    if (i & A_6_Mask){
      digitalWrite(A_6, HIGH);
    } else {
      digitalWrite(A_6, LOW);
    }
    if (i & A_5_Mask){
      digitalWrite(A_5, HIGH);
    } else {
      digitalWrite(A_5, LOW);
    }
    if (i & A_4_Mask){
      digitalWrite(A_4, HIGH);
    } else {
      digitalWrite(A_4, LOW);
    }
    if (i & A_3_Mask){
      digitalWrite(A_3, HIGH);
    } else {
      digitalWrite(A_3, LOW);
    }
    if (i & A_2_Mask){
      digitalWrite(A_2, HIGH);
    } else {
      digitalWrite(A_2, LOW);
    }
    if (i & A_1_Mask){
      digitalWrite(A_1, HIGH);
    } else {
      digitalWrite(A_1, LOW);
    }
    if (i & A_0_Mask){
      digitalWrite(A_0, HIGH);
    } else {
      digitalWrite(A_0, LOW);
    }
  
    //Tell the other unit that our address pins are set correctly
    digitalWrite(AddressHoldEnable, HIGH);

    //Wait for the other unit to tell us that it is done with the previous cycle
    while (digitalRead(AddressSetEnable) == HIGH){
      delay(1);
    }
  }
}

void loop() {
}
