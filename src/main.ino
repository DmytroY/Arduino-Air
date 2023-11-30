#include <Arduino.h>
#include <UTFT.h>                         //connect Libriary for use TFT

// main class constructors

UTFT    myGLCD(CTE32_R2, 38, 39, 40, 41);   //Type of TFT

extern uint8_t BigFont[];          // Declare which fonts we will be using
extern uint8_t SmallFont[];        
extern uint8_t SevenSegNumFont[];  

//---- pins ----//
#define PPM2  A0  // PPM 2.5mkm output
#define PPM1  A1  // PPM 1mkm output

//--- variables ---//
unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 60000;
unsigned long low = 0;
float ratio = 0;
float concentration = 0;
int result[320];
int i;

void setup() {
  Serial.begin(9600);
  Serial.println("Serial.begin(9600)");

  pinMode(PPM1, INPUT);
  pinMode(PPM2, INPUT);

  // Initialisation of LCD
  myGLCD.InitLCD(LANDSCAPE);              
  myGLCD.clrScr();
  myGLCD.setBackColor(0, 0, 0);    
  myGLCD.setColor(255, 255, 255);

  for (i = 0; i < 320; i++) {
    result[i] = 0;
  }
  
  Serial.println("setup routine done");
}

void loop() {
  low = low + pulseIn(PPM1, LOW);

  if ((millis()-starttime) > sampletime_ms)
  {
    ratio = low/(sampletime_ms*10.00);  // percentage 0=>100
 
    //concentration = 1.1 * pow(ratio, 3)-3.8 * pow(ratio, 2) + 520 * ratio + 0.62; // using spec sheet curve
    concentration = 116.7 * ratio; // in mkg/sq.m.
    //Serial.print(lowpulseoccupancy);
    // Serial.print(",");
    Serial.println(ratio);  
    
// 
    myGLCD.clrScr();

    // print hour scale
    myGLCD.setFont(BigFont);
    myGLCD.setColor(120, 120, 120);
    for (i = 1; i < 6; i++){
      myGLCD.drawLine(320 - i * 60, 220, 320 - i * 60, 223);
      myGLCD.printNumI(i, 312 - i * 60 , 224);
      myGLCD.print("h", 324 - i * 60 , 224);
    }

    //draw data
    myGLCD.setColor(255, 255, 255);
    for (i = 0; i < 319; i++) {
      result[i] = result[i+1];
      myGLCD.drawLine(i, 220, i, 220 - result[i] );
    }
    result[319] = concentration/10;
    myGLCD.drawLine(319, 220, 319, 220 - result[319] );

    // print concentration scale
    myGLCD.setFont(SmallFont);
    for (i = 0; i < 210; i = i+10){
      if (i%100){
        myGLCD.setColor(120, 120, 120);
      } else {
        myGLCD.setColor(255, 255, 255);
      }
      myGLCD.drawLine(15, 220 - i, 319, 220 - i);
      myGLCD.printNumI(i * 10, 0, 214 - i);
    }

    myGLCD.setFont(SevenSegNumFont);
    if(concentration < 100){
      myGLCD.setColor(125, 255, 125);    
    } else if(concentration > 200){
      myGLCD.setColor(255, 125, 125);
    } else {
      myGLCD.setColor(255, 255, 125);
    }
    
    myGLCD.printNumI(concentration,CENTER, 50);
    low = 0;
    starttime = millis();
  }
  
}
