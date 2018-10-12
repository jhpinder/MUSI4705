#include<LiquidCrystal.h>

LiquidCrystal lcd(2,3,4,5,6,7);   //rs,e,d4,d5,d6,d7

int Vin=5;        //voltage at 5V pin of arduino
float Vout=0;     //voltage at A0 pin of arduino
float R1=5100;    //value of known resistance
float R2=0;       //value of unknown resistance
int a2d_data=0;    
float buffer=0;            

void setup() 
{
 lcd.begin(16,2);
}

void loop()
{
  a2d_data=analogRead(A0);
  if(a2d_data)
  {
    buffer=a2d_data*Vin;
    Vout=(buffer)/1024.0;
    buffer=Vout/(Vin-Vout); 
    R2=R1*buffer;

    lcd.setCursor(4,0);
    lcd.print("Root Note:");

    if(R2>9850 && R2<10100) {
      lcd.setCursor(0,1);
      lcd.print("C");
    }

    else if(R2>1900 && R2<2100) {
      lcd.setCursor(0,1);
      lcd.print("D");
    }

    else {
      lcd.setCursor(0,1);
      lcd.print(" ");
    }

    //lcd.setCursor(0,1);
    //lcd.print("R (ohm) = ");
    //lcd.print(R2);
    
    delay(100);
  }
}
