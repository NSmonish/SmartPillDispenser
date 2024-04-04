#include <Wire.h>
#include <LiquidCrystal.h>
#include "RTClib.h"
#include <Servo.h>

RTC_DS1307 RTC;
Servo S;
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

const int ledPin=A2;
const int buzzerPin=A3;
const int motorPin1=2;
const int motorPin2=3;
const int motorPin3=4;
const int motorPin4=5;
const int motorSpeed=10;
const int buttonPin1=6;
const int buttonPin2=7;
const int buttonPin3=A1;
int buttonState1=0;
int buttonState2=0;
int buttonState3=0;
int hrscount=0;
int minscount=0;
int prestate1=0;
int prestate2=0;
int prestate3=0;
int pos;
boolean stop = false;
boolean cutoff = false;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  RTC.begin();
  lcd.begin(16, 2);
  S.attach(A0);
  pinMode(ledPin,OUTPUT);
  pinMode(buzzerPin,OUTPUT);
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);
  int motorcount=0;
  if(! RTC.isrunning())
  {
    Serial.println("RTC is not running!");
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
}

void loop() {
  DateTime now = RTC.now();
  button();
  LCD();
  Alarm(now);
  Stepper(now);
  Servo_mot(now);
  delay(100);
  lcd.clear();
}

void Alarm(DateTime now) {
  if(now.hour()==(hrscount%24) && now.minute()==(minscount%60))
  {
    digitalWrite(ledPin, HIGH); 
    delay(1000);
    digitalWrite(ledPin, LOW);
    delay(1000);
    tone(buzzerPin, 1000);
    delay(1000);
    noTone(buzzerPin);
    delay(1000); 
  }
}

void Servo_mot(DateTime now) {
  if (now.hour()==(hrscount%24) && now.minute()==(minscount%60)) {
  if (cutoff==false) {
    for (pos = 0; pos <= 90; pos += 1) {
        S.write(pos);
        delay(15);
      }

    Serial.println(cutoff);
    cutoff++;
  }
  }
}

void Stepper(DateTime now) {
  if(now.hour()==(hrscount%24) && now.minute()==((minscount+1)%60))
  {
    if (stop==false) {
    for(int i = 0; i <48; i++) 
      clockwise();
    stop=true;
    }
  }
}

void LCD() {
  lcd.setCursor(0, 0);
  lcd.print("Smart Pill Dispenser");
  lcd.setCursor(0, 1);
  lcd.print("Set Time: ");
  lcd.print(hrscount%24);
  lcd.print(":");
  lcd.print(minscount)%60;
}

void button(){
  buttonState1=digitalRead(buttonPin1);
  buttonState2=digitalRead(buttonPin2);
  buttonState3=digitalRead(buttonPin3);
  if(buttonState1 == HIGH && prestate1==0) {
    hrscount++;
    prestate1=1;
  }
  else if (buttonState1 == LOW)
  {
    prestate1=0;
  }
  if(buttonState2 == HIGH && prestate2==0) {
    minscount++;
    prestate2=1;
  }
  else if (buttonState2 == LOW)
  {
    prestate2=0;
  }
  if(buttonState3 == HIGH && prestate3==0) {
    for(int i = 0; i <43; i++) 
      clockwise();
    prestate3=1;
  }
  else if (buttonState3 == LOW)
  {
    prestate3=0;
  }
}

void clockwise(){
 // 1
 digitalWrite(motorPin4, HIGH);
 digitalWrite(motorPin3, LOW);
 digitalWrite(motorPin2, LOW);
 digitalWrite(motorPin1, LOW);
 delay(motorSpeed);
 // 2
 digitalWrite(motorPin4, HIGH);
 digitalWrite(motorPin3, HIGH);
 digitalWrite(motorPin2, LOW);
 digitalWrite(motorPin1, LOW);
 delay (motorSpeed);
 // 3
 digitalWrite(motorPin4, LOW);
 digitalWrite(motorPin3, HIGH);
 digitalWrite(motorPin2, LOW);
 digitalWrite(motorPin1, LOW);
 delay(motorSpeed);
 // 4
 digitalWrite(motorPin4, LOW);
 digitalWrite(motorPin3, HIGH);
 digitalWrite(motorPin2, HIGH);
 digitalWrite(motorPin1, LOW);
 delay(motorSpeed);
 // 5
 digitalWrite(motorPin4, LOW);
 digitalWrite(motorPin3, LOW);
 digitalWrite(motorPin2, HIGH);
 digitalWrite(motorPin1, LOW);
 delay(motorSpeed);
 // 6
 digitalWrite(motorPin4, LOW);
 digitalWrite(motorPin3, LOW);
 digitalWrite(motorPin2, HIGH);
 digitalWrite(motorPin1, HIGH);
 delay (motorSpeed);
 // 7
 digitalWrite(motorPin4, LOW);
 digitalWrite(motorPin3, LOW);
 digitalWrite(motorPin2, LOW);
 digitalWrite(motorPin1, HIGH);
 delay(motorSpeed);
 // 8
 digitalWrite(motorPin4, HIGH);
 digitalWrite(motorPin3, LOW);
 digitalWrite(motorPin2, LOW);
 digitalWrite(motorPin1, HIGH);
 delay(motorSpeed);
}
