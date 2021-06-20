#include<Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F,20,4);

char keys[4][3]={
 {'1','2','3'},
 {'4','5','6'},
 {'7','8','9'},
 {'*','0','#'}};
 
byte rowPin[4]={6,7,8,9};
byte colPin[3]={3,4,5};

String password = "1019";  // The Password.
int position = 0;

int wrong = 0; // Variable to calculate the wrong inputs.

int redPin = 10;
int greenPin = 11;
int buzzer = 12;
 
Keypad keypad=Keypad(makeKeymap(keys),rowPin,colPin,4,3);

int total = 0; // Variable to determine the number of wrong attempts.

// ------------Thermal Code--------------
int val;
int tempPin = 1;
// ------------Thermal End---------------

//------------Ultrasonic Code------------
const int echoPin = 2; // Echo Pin of Ultrasonic Sensor
const int pingPin = 3; // Trigger Pin of Ultrasonic Sensor
//------------Ultrasonic End-------------

void setup()
{
  
 pinMode(redPin,OUTPUT);
 pinMode(greenPin,OUTPUT);
 pinMode(buzzer, OUTPUT);
 
lcd.begin(); //lcd startup
lcd.begin();
lcd.backlight();
//lcd.print("     4x3 Keypad     ");
//lcd.setCursor(0,1);
//lcd.print("   Locking System   ");
//lcd.setCursor(0,2);
//lcd.print("         By:        ");
//lcd.setCursor(0,3);
lcd.print("Welcome!");
lcd.setCursor(0,1);
lcd.print("Enter Pin To Unlock :)");
lcd.setCursor(0,2);
delay(2000);
lcd.clear();
setLocked(true);
delay(20);

// ------------Thermal Code--------------
Serial.begin (9600);
// ------------Thermal End---------------

//------------Ultrasonic Code------------
pinMode(pingPin, OUTPUT); // initialising pin 3 as output
pinMode(echoPin, INPUT); // initialising pin 2 as input
//------------Ultrasonic End-------------

}

void loop()
{

// ------------Thermal Code--------------
  val = analogRead(tempPin);
  float mv = ( val / 1024.0) * 5000;
  float cel = mv / 10;
  //Serial.print ("Temp is: ");
  Serial.println (cel);
  //Serial.print (" * C");
  //Serial.println ();
  //delay(1000);
// ------------Thermal End---------------

//------------Ultrasonic Code------------
  long duration, inches, cm;
  
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(10);
  
  digitalWrite(pingPin, LOW);
  
  duration = pulseIn(echoPin, HIGH); // using pulsin function to determine total time
  inches = microsecondsToInches(duration); // calling method
  cm = microsecondsToCentimeters(duration); // calling method

  if (inches < 20) {
  float a = inches;
  //String sth = String("Someone Is Near at: ") + a + " inch";
  Serial.println(a);
  //Serial.println(" inch");
  //Serial.println();
  delay (500);
  }
  //Serial.print("in, ");
  //Serial.print(cm);
  //Serial.print("cm");
  //Serial.println();
  
  //delay(100);
//------------Ultrasonic End-------------

  
  //lcd.clear();
  //lcd.print(" Enter Unlock Pin: ");
  delay(100);
  
 char pressed=keypad.getKey();
 String key[4];
  
 if(pressed)
 {
  //lcd.clear();
  //lcd.print(" Enter Unlock Pin: ");
  lcd.setCursor(position,2);
  lcd.print(pressed);
  delay(100);
    if(pressed == '*' || pressed == '#')
      {
          position = 0;
          setLocked(true);
          lcd.clear();
      }

    else if(pressed == password[position])
      {
          key[position]=pressed;
          position++;
      }
 
    else if (pressed != password[position] )
      {
          wrong++;
          position ++;
      }

    if(position == 4){
          if( wrong >0)
            {
                total++;
                wrong = 0;
                position = 0;
                lcd.clear();
                lcd.setCursor(0,2);
                lcd.print("    Wrong Pass! Try Again!!     ");
                lcd.clear();
                delay(500);
                setLocked(true);
            }

          else if(position == 4 && wrong == 0)
            {
                position = 0;
                wrong = 0;
                lcd.clear();
                lcd.setCursor(0,1);
                lcd.print("      Welcome!     ");
                lcd.setCursor(5,2);
                lcd.print(" Door Open");
                setLocked(false);
                lcd.clear();
                delay(500);
            }

             if(total ==4)
            {
                total=0;
                buzzer_beep();
                delay(500);
            }

        }

   }

   
}




long microsecondsToInches(long microseconds) // method to covert microsec to inches 
{
 return microseconds / 74 / 2;
}
long microsecondsToCentimeters(long microseconds) // method to covert microsec to centimeters
{
   return microseconds / 29 / 2;
}


void setLocked(int locked)
  {
    if (locked)
      {
          digitalWrite(redPin, HIGH);
          digitalWrite(greenPin, LOW);
          delay(1000);
      }
    else
      {
          digitalWrite(redPin, LOW);
          digitalWrite(greenPin, HIGH);
          delay(1000);
      }
  }
void buzzer_beep()
{
   lcd.clear();
   lcd.setCursor(0,1);
   lcd.print("    WARNING  !!!!   ");
   lcd.setCursor(0,2);
   lcd.print("   Access Denied");
    while(1)
   {
   digitalWrite(buzzer,HIGH);
   delay(1000);
   digitalWrite(buzzer,LOW);
   delay(1000);
   }
}
