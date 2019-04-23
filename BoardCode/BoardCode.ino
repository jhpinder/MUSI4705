#include <Adafruit_NeoPixel.h>

//board pin arrays
int majPins[][8] ={ {2,8,22,28,34,40,46,52},
                    {4,10,24,30,36,42,48,A10},
                    {6,12,26,32,38,44,50,A12},
                  };

                  
int minPins[][8] ={ {3,9,23,29,35,41,47,53},
                    {5,11,25,31,37,43,49,A9},
                    {7,13,27,33,39,45,51,A11},
                  };
                 
//Adafruit_NeoPixel pix0 = Adafruit_NeoPixel(7, A12, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel pix1 = Adafruit_NeoPixel(8, A13, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel pix2 = Adafruit_NeoPixel(8, A14, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel pix3 = Adafruit_NeoPixel(8, A15, NEO_RGB + NEO_KHZ800);

uint32_t maj3 = pix1.Color(0,255,0);
uint32_t min3 = pix1.Color(0,0,255);

int colNo = 8 ;
int tempo;
int inst;

bool isEmpty = false;
int dumpCD = 0;
int lastR = 60;
int lastNotes[] = {0,0,0};
String currOut;
String lastOut;
String lastColOut;

void setup() {

Serial.begin(9600);

//initializing all maj pins
for (int thismajRow=0; thismajRow < 3; thismajRow++) {  
  for (int thismajPin = 0; thismajPin < 8; thismajPin++) {
    pinMode(majPins[thismajRow][thismajPin], INPUT_PULLUP);
    //Serial.print("Pin ");
    //Serial.print(majPins[thismajRow][thismajPin]);
    //Serial.println(" activated!");
  }
}
//initializing all min pins  
for (int thisminRow=0; thisminRow <3; thisminRow++) {  
  for (int thisminPin = 0; thisminPin < 8; thisminPin++) {
    pinMode(minPins[thisminRow][thisminPin], INPUT_PULLUP);
    //Serial.print("Pin ");
    //Serial.print(minPins[thisminRow][thisminPin]);
    //Serial.println(" activated!");
  }
}
pinMode(A7,INPUT);
pinMode(A8,INPUT);

//initialize NeoPixel pins
//pinMode(A12,OUTPUT);
pinMode(A13,OUTPUT);
pinMode(A14,OUTPUT);
pinMode(A15,OUTPUT);

//dump button
pinMode(15,INPUT_PULLUP);

pinMode(14,INPUT_PULLUP);
pinMode(16,INPUT_PULLUP);

//pix0.begin();
//pix0.setBrightness(50);
//pix0.show();
pix1.begin();
pix1.setBrightness(50);
pix1.show();
pix2.begin();
pix2.setBrightness(50);
pix2.show();
pix3.begin();
pix3.setBrightness(50);
pix3.show();

}

void loop() {
  
  lastOut = currOut;
  currOut = "";
  //Serial.println(lastOut);
  

//for loop to sequence one pass through 8 steps
for (int seqNo = 0; seqNo < colNo; seqNo++) {
  if (digitalRead(16)) {
    return;
  }
  if (!digitalRead(15)) {
    dump();
    return;
  }
  tempo = map(analogRead(A7), 0, 1023, 100, 1000);
  inst = map(analogRead(A8), 0, 1023, 0, 5);
  
  int currentRoot = 60;
  //ohmmeter variables
  int Vin=5;
  float Vout=0;
  float R1=470;//knownvalue
  float R2=0;//testvalue
  int a2d_data=0;
  float buffer=0;
    
  //resistorID code
  a2d_data=analogRead(seqNo-1);//will need to be replaced with appropriate pin
  buffer=a2d_data*Vin;
  Vout=(buffer)/1024.0;
  buffer=Vout/(Vin-Vout); 
  R2=R1*buffer;

  isEmpty = false;
  if (R2 > 30 && R2 < 65) //1
  {
    currentRoot = 60;
  }
  
  else if (R2 > 135 && R2 < 170) //2
  {
    currentRoot = 62;
  }

  else if (R2 > 250 && R2 < 285) //3
  {
    if (digitalRead(14)) {
    currentRoot = 64;
    } else {
      currentRoot = 63;
    }
  }

  else if (R2 > 310 && R2 < 340) //4
  {
    currentRoot = 65;
  }

  else if (R2 > 490 && R2 < 525) //5
  {
    currentRoot = 67;
  }

  else if (R2 > 655 && R2 < 695) //6
  {
    if (digitalRead(14)) {
    currentRoot = 68;
    } else {
      currentRoot = 69;
    }
  }

  else if (R2 > 1890 && R2 < 2150) //7
  {
    if (digitalRead(14)) {
    currentRoot = 70;
    } else {
      currentRoot = 71;
    }
  }


  else if (analogRead(seqNo - 1) > 900)
  {
    currentRoot = 0;
    if (seqNo != 0) {
      isEmpty = true;
    }
  }
  if (seqNo == 0) {
    currentRoot = 60;
  }

  
  //array containing binary read from columns, laid out maj,min in the order they appear on the board
  int switchState[][2] ={ {digitalRead(majPins[0][seqNo]),digitalRead(minPins[0][seqNo])},
                          {digitalRead(majPins[1][seqNo]),digitalRead(minPins[1][seqNo])},
                          {digitalRead(majPins[2][seqNo]),digitalRead(minPins[2][seqNo])},
                        };
  //array of semitone values, major and minor, subtracting 1 and multiplying by (-) to correct bit flip wiring issue
  int semitoneArr[][2] = { {((switchState[0][0]-1)*-4),((switchState[0][1]-1)*-3)},
                           {((switchState[1][0]-1)*-4),((switchState[1][1]-1)*-3)},
                           {((switchState[2][0]-1)*-4),((switchState[2][1]-1)*-3)},
                         };
  //single dimension array of column semitone values
  int intervalArr[] = {(semitoneArr[0][0]+semitoneArr[0][1]),
                       (semitoneArr[1][0]+semitoneArr[1][1]),
                       (semitoneArr[2][0]+semitoneArr[2][1])};

  //semitone values to be added from the root, bottom to top, ignoring empty positions
  String finalString[4] = {String(currentRoot)};
  String output = String(currentRoot);
  int count = 1;
  int zerocount = 0;
  for (int i=0;i<3;i++) {
      int currentSel = intervalArr[i];
        lastNotes[i] = currentSel;
      if (currentSel!= 0)
      {
        int add = finalString[count-1].toInt()+intervalArr[i];
        finalString[count] = String(add);
        output = output + " " + finalString[count];
        count = count + 1;
      } else {
        zerocount++;
      }

  } 

    currOut = currOut + " " + output;
    for (int i = 0; i < zerocount; i++) {
      currOut = currOut + " 00";
      output = output + " 00";
    }
     if (isEmpty) {
    output = lastColOut;
    //currOut = lastColOut;
     }
     String outNoInst = output;
    output = output + " " + inst;
 
  //NeoPixel time
  if (seqNo == 0) { //turning off last col of pixels
    //pix0.setPixelColor(colNo - 1,0,0,0);
    pix1.setPixelColor(colNo - 1,0,0,0);
    pix2.setPixelColor(colNo - 1,0,0,0);
    pix3.setPixelColor(colNo - 1,0,0,0);
  } else {
    //pix0.setPixelColor(seqNo - 1,0,0,0);
    pix1.setPixelColor(seqNo - 1,0,0,0);
    pix2.setPixelColor(seqNo - 1,0,0,0);
    pix3.setPixelColor(seqNo - 1,0,0,0);
  }

  //turning on this col of pixels
  if (intervalArr[0] == 3) {
    pix1.setPixelColor(seqNo,min3);
  } else if (intervalArr[0] == 4) {
    pix1.setPixelColor(seqNo,maj3);
  } else if (intervalArr[0] == 0) {
    pix1.setPixelColor(seqNo,240,255,200);
  }
  
  if (intervalArr[1] == 3) {
    pix2.setPixelColor(seqNo,min3);
  } else if (intervalArr[1] == 4) {
    pix2.setPixelColor(seqNo,maj3);
  } else if (intervalArr[1] == 0) {
    pix2.setPixelColor(seqNo,240,255,200);
  }

  if (intervalArr[2] == 3) {
    pix3.setPixelColor(seqNo,min3);
  } else if (intervalArr[2] == 4) {
    pix3.setPixelColor(seqNo,maj3);
  } else if (intervalArr[2] == 0) {
    pix3.setPixelColor(seqNo,240,255,200);
  }
  
  //pix0.show();
  pix1.show();
  pix2.show();
  pix3.show();
  
  Serial.println(output);
  delay(tempo);
    dumpCD++;
    lastR = currentRoot;
    lastColOut = outNoInst;
                         
}


} 




void dump() {
//  if (dumpCD < 8) {
//    return;
//  }
  String output = "0 0 0 0 0 1 " + lastOut;
  Serial.println(output);
  delay(2000);
}


  









