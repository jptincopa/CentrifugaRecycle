//Programa para medicion de RPM y mostrar en LCD

#include <Wire.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int lcd_key     = 0;
int key  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

int ticks = 0, Speed = 0;
int potPin = A5; 
int transistorPin = 3; 
int potValue = 150; 
 
int hallsensor = 2;  

typedef struct{   

char fantype;
unsigned int fandiv; 

}  fanspec;

fanspec fanspace[3]={{0,1},{1,2},{2,8}}; char fan = 0;

void pickrpm ()

{ ticks++; }

void setup()
{
   lcd.begin(16, 2);              // Inicializar el LCD
 lcd.setCursor(0,0);
 lcd.print("CENTRIFUGA");     
 
pinMode(transistorPin, OUTPUT);
pinMode(hallsensor, INPUT);
Serial.begin(9600);
attachInterrupt(0, pickrpm, FALLING); 
}
  
void loop ()
{ 
   lcd.setCursor(0,1);            
 
   lcd_key = read_LCD_buttons();
  
   if( lcd_key == btnRIGHT)
       {lcd.print("RIGHT ");}
   else if ( lcd_key == btnLEFT )
       {lcd.print("LEFT   ");}
   else if ( lcd_key == btnUP)
       {lcd.print("UP    ");
       potValue=potValue+15;
       }
   else if ( lcd_key == btnDOWN)
       {
       potValue=potValue-15;
       lcd.print("DOWN  ");}
   else if ( lcd_key == btnSELECT)
       {lcd.print("SELECT");}
   else if ( lcd_key == btnNONE)
       {lcd.print("Menu  ");}
  
 //potValue = analogRead(potPin) / 4;

 
  if(potValue<150){potValue=150;}
  else if(potValue>254){potValue=254;}
  Serial.println("value: ");
  Serial.println(potValue);
  analogWrite(transistorPin, potValue);
    
ticks = 0;      

interrupts();    
delay (1000);     
noInterrupts();  

Speed = ((ticks * 60)/fanspace[fan].fandiv)/2;  

  Serial.print (Speed, DEC);    
  Serial.print ("RPM\r\n");
lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("CENTRIFUGA");  
lcd.setCursor(8,1);            
lcd.print("RPM");
lcd.setCursor(11,1); 
lcd.print(Speed);

 char  SpeedString[4];    
  sprintf(SpeedString, "%4d", Speed);   
}  


int read_LCD_buttons()  
{
 key = analogRead(0);      
   Serial.println(key);  

 if (key > 1000) return btnNONE;  
 if (key < 50)   return btnRIGHT;  
 if (key < 250)  return btnUP; 
 if (key < 350)  return btnDOWN; 
 if (key < 420)  return btnLEFT; 
 if (key < 650)  return btnSELECT;  

 return btnNONE;  
}
