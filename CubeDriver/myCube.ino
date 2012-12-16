

/*
* 5x5x5Cube.ino                        Date: 12/5/2012
* Author: jliang84@gmail.com
* Description: This code helps drive a 5x5x5 LED cube.
*/

int E0 = 13; // Goes to A0 on LS0
int E1 = 12; // Goes to A1 on LS0
int C0 = 3;

int ON = 255;
int OFF = 0;

// The following pins output to the LS1,LS2,LS3 to be decoded based on the which one is enabled from LS0
int LA0 = 11;
int LA1 = 10;
int LA2 = 9;

// The following ints are named so that output to enable the horizontal layers is easeir.
int L0 = 4;
int L1 = 5;
int L2 = 6;
int L3 = 7;
int L4 = 8;

int inByte = 0;
// Setup only run once when arduino is powered up
void setup() {                
  Serial.begin(9600);
  for(int pin = 3; pin < 14 ; pin++){
      pinMode(pin, OUTPUT);
  }
}

// Output Pin Y0 on LS0 is set to clear no lights lite up.
void clear(){
  analogWrite(E0, 0);
  analogWrite(E1, 0);
  zeroOFF();
}
// The following LS# functions enable the 3-8 decoders to decode input
void LS1(){
  analogWrite(E0, 255);
  analogWrite(E1, 0);
}
// This function enables LS2 to decode the inputs from pins 9,10,11
void LS2(){
  analogWrite(E0, 0);
  analogWrite(E1, 255);
}
//This function enables LS3 to decode the inputs from pins 9,10,11
void LS3(){
    analogWrite(E0, 255);
    analogWrite(E1, 255);
}

// The following function set the outputs to the 3-8 chip selected
// This sets pin Y0 output to High
void LLL(){
  analogWrite(LA0, 0);
  analogWrite(LA1, 0);
  analogWrite(LA2, 0);
}

//This sets pin Y1 output to HIGH
void HLL(){
  analogWrite(LA0, 255);
  analogWrite(LA1, 0);
  analogWrite(LA2, 0);
}
//This sets pin Y2 output to HIGH
void LHL(){
    analogWrite(LA0, 0);
    analogWrite(LA1, 255);
    analogWrite(LA2, 0);
}
// This sets pin Y3 output to HIGH
void HHL (){
  analogWrite(LA0, 255);
  analogWrite(LA1, 255);
  analogWrite(LA2, 0);
}
// This sets pin Y4 output to HIGH
void LLH (){
  analogWrite(LA0, 0);
  analogWrite(LA1, 0);
  analogWrite(LA2, 255);
}
// This sets pin Y5 output to HIGH
void HLH (){
  analogWrite(LA0, 255);
  analogWrite(LA1, 0);
  analogWrite(LA2, 255);
}
// This sets pin Y6 output to HIGH
void LHH (){
  analogWrite(LA0, 0);
  analogWrite(LA1, 255);
  analogWrite(LA2, 255);
}
// This sets pin Y7 output to HIGH
void HHH (){
  analogWrite(LA0, 255);
  analogWrite(LA1, 255);
  analogWrite(LA2, 255);
}

//This function uses an integer input to turn on/off a layer
void layerState(int layer, int state){
  switch (layer) {
    case 0:
      analogWrite(L0, state);
      break;
    case 1:
      analogWrite(L1, state);
      break;
    case 2: 
      analogWrite(L2, state);
      break;
    case 3:
      analogWrite(L3, state);
      break;
    case 4:
      analogWrite(L4, state);
      break;
    default:
      break;
  }
}

// This method will turn on a column by an integer input
// if column 0, must turn off column 0 also
void columnState (int col){
  if(col == 0){
    analogWrite(C0, 255);
  }
  else if (col > 0 && col <= 8){
    analogWrite(E0, 255);
    analogWrite(E1, 0);
    mySwitch(col);
  }
  else if( col > 8 && col <= 16){
    analogWrite(E0, 0);
    analogWrite(E1, 255);
    mySwitch(col - 8);
  }
  else if(col > 16 && col <= 24){
    analogWrite(E0, 255);
    analogWrite(E1, 255);
    mySwitch(col - 16);
  }
  //last bracket
}

void zeroOFF(){
  analogWrite(C0, 0);
}
void mySwitch(int col){
        switch(col){
      case 1:
             LLL();
             break;
      case 2:
             HLL();
             break;
      case 3:
             LHL();
             break;
      case 4:
             HHL();
             break;
      case 5:
             LLH();
             break;
      case 6:
             HLH();
             break;
      case 7:
             LHH();
             break;
      case 8:
             HHH();
             break;
      default:
             break;
    }
}
void allLayers(int state){
  for(int i = 0; i <= 4; i++){
    layerState(i, state);
  }
}

void inSequence(int time){
  LLL();
  HLL();
  LHL();
  HHL();
  LLH();
  HLH();
  LHH();
  HHH();
}
int getLayer(int led){
  if(led <= 25){
    return (led -1) / 25;
  }
  else if( led > 25){
    return (led -1) /25;
  }
}

int getCol(int led){
  if(led <= 25){
    return (led -1);
  }
  else{
    return (led -1) %25;
  }
}

void ledOnTime(int led, int time){
  int layer = getLayer(led);
  int col = getCol(led);
  layerState(layer, ON);
  
 if(col == 0){
   columnState(col);
   delay(time);
   zeroOFF();
 }
 else if(col > 0){
   columnState(col);
   delay(time);
 }
 layerState(layer, OFF);
}
void ledOn(int led){
  ledOnTime(led, 1);
}

void allRandom(int time){
  int led = random(1, 126);
  ledOnTime(led, time);
}

void layerRandom(int layer, int time){
  int led = 0;
  switch(layer){
    
    case 0:
      led = random(0,26);
      ledOnTime(led,time);
      break;
    case 1:
      led = random(26,51);
      ledOnTime(led,time);
      break; 
    case 2:
      led = random(51,76);
      ledOnTime(led,time);
      break; 
    case 3:
      led = random(76,101);
      ledOnTime(led,time);
      break; 
     case 4:
      led = random(101,126);
      ledOnTime(led,time);
      break;
     default:
      break;
  }
} 

void loop(){

  if(Serial.available()> 0){
    inByte = Serial.read();
    ledOn(inByte);
  }
}

