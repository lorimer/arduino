#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

#define trigPin 13
#define echoPin 12



void setup()
{
 Serial.begin (9600);
 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);
 lcd.init();                      // initialize the lcd 
 lcd.backlight();
}
void loop()
{
 long duration, distance;
 digitalWrite(trigPin, LOW);  // Added this line
 delayMicroseconds(2); // Added this line
 digitalWrite(trigPin, HIGH);
//  delayMicroseconds(1000); - Removed this line
 delayMicroseconds(10); // Added this line
 digitalWrite(trigPin, LOW);
 duration = pulseIn(echoPin, HIGH);
 distance = (duration/2) / 29.1;
 
 // Print a message to the LCD.
 lcd.clear();
 lcd.setCursor(0,0); 
 lcd.print(distance); 
 delay(500);
}
