#include <Wire.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// define some values used by the panel and buttons
int lcd_key     = 0;
int key  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5
//Varibles used for calculations
int ticks = 0, Speed = 0;
int potPin = A5; 
int transistorPin = 3; // PWM Pin 9 connected to the base of the transistor
int potValue = 150; // value returned from the potentiometer
 
int hallsensor = 2;  //The Hall effect sensor (HES) output of fan  connected to pin no 12 of Arduino due

//Defines the structure for multiple fans and their dividers
typedef struct{   

char fantype;
unsigned int fandiv; 

}  fanspec;

//Definitions of the fans
//This is the varible used to select the fan and it's divider,
//set 1 for unipole hall effect sensor
//and 2 for bipole hall effect sensor
fanspec fanspace[3]={{0,1},{1,2},{2,8}}; char fan = 0;

void pickrpm ()
//This is the interrupt subroutine that increments ticks counts for each HES response.
{ ticks++; }

//This is the setup function where the serial port is initialised,
//and the interrupt is attached and other pins initialized.

void setup()
{
   lcd.begin(16, 2);              // Inicializar el LCD
 lcd.setCursor(0,0);
 lcd.print("CENTRIFUGA");     // print a simple message
 
pinMode(transistorPin, OUTPUT);
pinMode(hallsensor, INPUT);
Serial.begin(9600);
attachInterrupt(0, pickrpm, FALLING); 
}
  
void loop ()
{ 
   lcd.setCursor(0,1);            // Cursor a linea 2, posicion 1
 
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
  // use that to control the transistor:
  if(potValue<150){potValue=150;}
  else if(potValue>254){potValue=254;}
  Serial.println("value: ");
  Serial.println(potValue);
  analogWrite(transistorPin, potValue);
    
ticks = 0;      // Make ticks zero before starting interrupts.

interrupts();    // or use sei(); to Enables interrupts
delay (1000);     //Wait 1 second
noInterrupts();  //  or use  cli();  to Disable interrupts

//Times sensorclicks (which is apprioxiamately the fequency the fan 
//is spinning at) by 60 seconds before dividing by the fan's divider
// this gives reasonable accuracy in upto few 10s of RPM
// for more accurate and fast measurements, other algorithms is required.
Speed = ((ticks * 60)/fanspace[fan].fandiv)/2;  

//Print calculated Speed at the serial port 
  Serial.print (Speed, DEC);    
  Serial.print ("RPM\r\n");
lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("CENTRIFUGA");  
lcd.setCursor(8,1);            // Cursor a linea 2, posicion 1
lcd.print("RPM");
lcd.setCursor(11,1); 
lcd.print(Speed);
// Converting integer Speed to an ASCII string of 4 characters padded left
  char  SpeedString[4];    // Buffer to store string of 4 chars + 0 termination
  sprintf(SpeedString, "%4d", Speed);   // change this to %3, 4 ,5 etc depending upon your max spee
}  // end of loop function


int read_LCD_buttons()  // Lee los botones y devuelve le valor pulsado
{
 key = analogRead(0);      // Leemos la puerta A0 
 // Mis botones dan:  0, 145, 329,507,743 ; Comprueba que tu shield devuelve valores parecidos
 // Y ahora los comparamos con un margen comodo
   Serial.println(key);  

 if (key > 1000) return btnNONE; //Si no se pulsa nada salimos (el 99% de las veces) 
 if (key < 50)   return btnRIGHT;  
 if (key < 250)  return btnUP; 
 if (key < 350)  return btnDOWN; 
 if (key < 420)  return btnLEFT; 
 if (key < 650)  return btnSELECT;  

 return btnNONE;  // when all others fail, return this...
}
