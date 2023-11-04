#include <Arduino.h>
#include <UTFT.h>                         //connect Libriary for use TFT

// main class constructors

UTFT    myGLCD(CTE32_R2, 38, 39, 40, 41);   //Type of TFT

extern uint8_t BigFont[];          // Declare which fonts we will be using
extern uint8_t SmallFont[];        
extern uint8_t SevenSegNumFont[];  
int i = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Serial.begin(9600)");

  // Initialisation of LCD
  myGLCD.InitLCD(LANDSCAPE);              
  myGLCD.clrScr();
  myGLCD.setBackColor(0, 0, 0);    
  myGLCD.setFont(SevenSegNumFont);
  Serial.println("setup routine done");
}

void loop() {
  Serial.print("i = ");
  Serial.println(i);

  
  myGLCD.clrScr();  
  myGLCD.setColor((i%3)*122, ((i+1)%3)*122, ((i+2)%3)*122);
  // myGLCD.setColor(255, 255, 255);
  // myGLCD.fillRect(0, 0, 319, 13);

  myGLCD.printNumI(i, CENTER, 70);
  delay(1000);
  i++;
}
