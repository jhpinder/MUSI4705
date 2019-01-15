#include <Adafruit_NeoPixel.h>

//board pin arrays
int majPins[][8] ={ {2,8,22,28,34,40,46,52},
                    {4,10,24,30,36,42,48,14},
                    {6,12,26,32,38,44,50,16},
                  };
int minPins[][8] ={ {3,9,23,29,35,41,47,53},
                    {5,11,25,31,37,43,49,15},
                    {7,13,27,33,39,45,51,17},
                  };
                 

uint32_t maj3 = pix1.Color(255,162,193);
uint32_t min3 = pix1.Color(0,147,187);

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
//initialize NeoPixel
pinMode(A12,OUTPUT);
pinMode(A13,OUTPUT);
pinMode(A14,OUTPUT);
pinMode(A15,OUTPUT);
Adafruit_NeoPixel pix0 = Adafruit_NeoPixel(7, A12, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel pix1 = Adafruit_NeoPixel(8, A13, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel pix2 = Adafruit_NeoPixel(8, A14, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel pix3 = Adafruit_NeoPixel(8, A15, NEO_RGB + NEO_KHZ800);

pix0.begin();
pix0.setBrightness(50);
pix0.show();
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

//for loop to sequence one pass through 8 steps
//for (int seqNo = 0; seqNo < 2;) {
  int seqNo=0;
  int currentRoot = 60;
  if (seqNo == 0) {
    int currentRoot = 60;//could be variable with input device
  }

  else {
    
    //ohmmeter variables
    int Vin=5;
    float Vout=0;
    float R1=2200;//knownvalue
    float R2=0;//testvalue
    int a2d_data=0;
    float buffer=0;
    
    //resistorID code
    a2d_data=analogRead(A0);//will need to be replaced with appropriate pin
    buffer=a2d_data*Vin;
    Vout=(buffer)/1024.0;
    buffer=Vout/(Vin-Vout); 
    R2=R1*buffer;
    
    //Serial.println(R2);

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
  for (int i=0;i<3;i++)
    {
      int currentSel = intervalArr[i];
      
      if (currentSel!= 0)
      {
        int add = finalString[count-1].toInt()+intervalArr[i];
        finalString[count] = String(add);
        output = output + " " + finalString[count];
        count = count + 1;
      }

 
    }
  //NeoPixel time
  if (seqNo == 0) { //turning off last col of pixels
    pix0.setPixelColor(1,0,0,0);
    pix1.setPixelColor(1,0,0,0);
    pix2.setPixelColor(1,0,0,0);
    pix3.setPixelColor(1,0,0,0);
  } else {
    pix0.setPixelColor(seqNo - 1,0,0,0);
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
    pix1.setPixelColor(seqNo,255,255,255);
  }
  
  if (intervalArr[1] == 3) {
    pix2.setPixelColor(seqNo,min3);
  } else if (intervalArr[1] == 4) {
    pix2.setPixelColor(seqNo,maj3);
  } else if (intervalArr[1] == 0) {
    pix2.setPixelColor(seqNo,255,255,255);
  }

  if (intervalArr[2] == 3) {
    pix3.setPixelColor(seqNo,min3);
  } else if (intervalArr[2] == 4) {
    pix3.setPixelColor(seqNo,maj3);
  } else if (intervalArr[2] == 0) {
    pix3.setPixelColor(seqNo,255,255,255);
  }
  
  pix0.show();
  pix1.show();
  pix2.show();
  pix3.show();
  
  Serial.println(output);
  delay(500);
    
    
                         


//} 




}


  







