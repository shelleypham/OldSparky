/*********************************************************************************************************
* OLD SPARKY: Immersive Electric Chair Virtual Reality Project
* http://oldsparky.warpedimagination.com/
*
* ***************************************************************
*  This is the Arduino code that was used for the VR experience.
* ***************************************************************
*
* FIRST EXHIBITION: SoCal VR Conference & Expo | August 2015 | Irvine, California
* EXECUTION GALLERY: http://goo.gl/Lok031
* PROJECT HISTORY: http://goo.gl/baJtB2
*
* MEDIA + PRESS:
* OC Register: http://goo.gl/31YwqY
* Realidyne: http://goo.gl/v8L1vP
* VR Scout: http://goo.gl/7N7aex 
*
* Special thanks to KOR-FX who provided the haptics gaming vest
*
* CREDITS:
* AARON GUTIERREZ
* ANDREW JOHNSON
* ARMAN BASTANI - Oval Integration
* DAVID GREEN - Warped Imagination
* DYLAN WATKINS - Monster VR
* KENDRICK PARKS
* MATTHEW KINNEY - Monster VR
* SHELLEY PHAM - Oval Integration
*********************************************************************************************************/
#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

/* Set color of the LCD */
int colorR, colorG, colorB;

/* LIGHT AND SHOCKER PIN LOCATION */
const int pinButton = 2;
const int pinLed = 6; //pwm

/* FOR SERIAL PURPOSES */
byte byteRead = 'c';

/* BUTTON STATUS */
bool button = false;

/* PLACEHOLDERS for SHOCK LEVELS */
int const shockLow = 200, shockMed = 40, shockHig = 25;

/*********************************************************************************************************
* INITIAL SET UP FOR LCD AND SHOCKER
*********************************************************************************************************/
void setup()
{
  // set up light and shocker
  pinMode(pinButton, INPUT);
  pinMode(pinLed, OUTPUT); 

  analogWrite(pinLed, 255); // TURN OFF SHOCKER
  
  lcd.begin(16, 2); // set up the LCD's number of columns and rows:

  // Print a message to the LCD.
  lcd.print("Welcome to");
  lcd.setCursor(0, 1);
  lcd.print(" www.SoCalVR.com");
  delay(1000);
}

/*********************************************************************************************************
* WHERE THE MAGIC HAPPENS
*********************************************************************************************************/
void loop()
{
  analogWrite(pinLed, 255); // TURN OFF Shocker
  
  //STANDBY: green screen
  colorR = 0;
  colorG = 255;
  colorB = 0;
  
  // set the cursor to column 0, line 0
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 0);
  lcd.print("PENANCE         ");
  delay(1000);
  lcd.setRGB(colorR, colorG, colorB);
  
  Serial.begin(9600);
  if (Serial.available() > 0) {
    byteRead = Serial.read(); // read the most recent input by user
    if (byteRead == 'H' || byteRead == 'L' || byteRead == 'M') {
        lcd.setRGB(255, 0, 0); // screen turns RED when ready to press    

        // TURN ON Red Light on Knife Switch
        
        // enter the waiting phase for the Knife Switch
        // print a message to the LCD.
        lcd.setCursor(0, 0);
        if (byteRead == 'H') { lcd.print("SWITCH High!"); }
        if (byteRead == 'M') { lcd.print("SWITCH Med !"); }
        if (byteRead == 'L') { lcd.print("SWITCH Low !"); }
        
        /***************************************************************
         * When button is pressed, prints out "X" on serial output.
         * LCD turns GREEN to indicate that it's been switched, then
         * shocker activates
         **************************************************************/
        while (button == false) {
          if (digitalRead(pinButton))                    
          {
            // TURN OFF Red Light on Knife Switch
            // TURN ON Strobe and Blue Lights EL wire

            Serial.println("X");
            
            // once button is pressed, change LCD to blue
            colorR = 0;
            colorG = 0;
            colorB = 255;
            lcd.setRGB(colorR, colorG, colorB);
            
            // enter the waiting phase for the Knife Switch
            // print a message to the LCD.
            lcd.setCursor(0, 0);
            if (byteRead == 'H') { lcd.print("Electricity High"); }
            if (byteRead == 'M') { lcd.print("Electricity Med!"); }
            if (byteRead == 'L') { lcd.print("Electricity Low!"); }
            
            // output on Shocker
            if (byteRead == 'H') { analogWrite(pinLed, shockHig); }
            if (byteRead == 'M') { analogWrite(pinLed, shockMed); }
            if (byteRead == 'L') { analogWrite(pinLed, shockLow); }
            
            delay(2000);
            
            analogWrite(pinLed, 255); // TURN OFF SHOCKER

            // TURN OFF Strobe and Blue Lights EL wire           
            
            break; // break out of while-loop
          }
        }
    }
  }
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
