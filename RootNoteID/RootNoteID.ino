#include<LiquidCrystal.h>

LiquidCrystal lcd(2,3,4,5,6,7);   //rs,e,d4,d5,d6,d7

int Vin=5;        //voltage at 5V pin of arduino
float Vout=0;     //voltage at A0 pin of arduino
float R1=2200;    //value of known resistance
float R2=0;       //value of unknown resistance
int a2d_data=0;    
float buffer=0;            

void setup() 
{
 
 Serial.begin(9600);
 //lcd.begin(16,2);
}

void loop()
{
    a2d_data=analogRead(A1);
    buffer=a2d_data*Vin;
    Vout=(buffer)/1024.0;
    buffer=Vout/(Vin-Vout); 
    R2=R1*buffer;

    Serial.println(R2);

   //lcd.setCursor(0,0);
    //lcd.print("Note Value:");

    //if(R2>9000 && R2<10000) {
      //lcd.setCursor(0,1);
      //lcd.print("C");
    //}

    //else if(R2>1900 && R2<2000) {
      //lcd.setCursor(0,1);
      //lcd.print("D");
    //}
    //else if(R2>0) {
      //lcd.setCursor(0,1);
      //lcd.print(" ");

    //}
    //else {
      //lcd.setCursor(0,1);
      //lcd.print(R2);
    delay(100);
}
