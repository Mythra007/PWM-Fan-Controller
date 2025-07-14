#include <Wire.h>

#include <Servo.h>

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); 
Servo fanServo;


const int tempPin = A0;       
const int fanPin = A1;        
const int heaterPin = 10;     
const int buzzerPin = 11; 
const int servoPin = 9;

const int btn1Pin = 6;  
const int btn2Pin = 7;  
const int btn3Pin = 8;  


const float lowTempThreshold = 10.0;   
const float highTempThreshold = 35.0; 
const float coldAlert = 0.0;          

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  
  fanServo.attach(servoPin);

  pinMode(tempPin, INPUT);
  pinMode(fanPin, OUTPUT);
  pinMode(heaterPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  
  pinMode(btn1Pin, INPUT_PULLUP);
  pinMode(btn2Pin, INPUT_PULLUP);
  pinMode(btn3Pin, INPUT_PULLUP);
}

void loop() {
  
  bool btn1 = digitalRead(btn1Pin) == LOW;
  bool btn2 = digitalRead(btn2Pin) == LOW;
  bool btn3 = digitalRead(btn3Pin) == LOW;

  int manualLevel = 0;

  if (btn1) manualLevel = 1;
  else if (btn2) manualLevel = 3;
  else if (btn3) manualLevel = 5;

  int fanSpeed;
  int angle;
  float temperatureC = 0;

  if (manualLevel > 0) {
    
    fanSpeed = map(manualLevel, 0, 5, 0, 255);
    angle = manualLevel * 18;

    analogWrite(fanPin, fanSpeed);
    fanServo.write(angle);
    digitalWrite(heaterPin, LOW);
    noTone(buzzerPin);

    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Manual Fan L:");
    lcd.print(manualLevel);
    lcd.setCursor(0, 1);
    lcd.print("Speed: ");
    lcd.print(fanSpeed);
    delay(2000);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Manual Override");
    lcd.setCursor(0, 1);
    lcd.print("Servo: ");
    lcd.print(angle);
    delay(2000);

    Serial.print("Manual Override | Fan Level: ");
    Serial.print(manualLevel);
    Serial.print(" | Fan Speed: ");
    Serial.print(fanSpeed);
    Serial.print(" | Servo: ");
    Serial.println(angle);
  } else {
   
    int sensorValue = analogRead(tempPin);
    float voltage = sensorValue * (5.0 / 1023.0);
    temperatureC = (voltage - 0.5) * 100.0;

    fanSpeed = map(temperatureC, 24, 50, 0, 255);
    fanSpeed = constrain(fanSpeed, 0, 255);
    analogWrite(fanPin, fanSpeed);

    float fanVoltage = fanSpeed * (5.0 / 255.0);

    int level = (int)(fanVoltage);  
    angle = level * 18;

    if (temperatureC > 24) {
      fanServo.write(angle);
    } else {
      fanServo.write(0);
    }

    
    if (temperatureC < lowTempThreshold){
      digitalWrite(heaterPin, HIGH);
    } else {
      digitalWrite(heaterPin, LOW);
    }

    
    if (temperatureC < 10) {
      int freq = map((int)temperatureC, 0, 10, 800, 300);
      tone(buzzerPin, freq);
    } else if (temperatureC > 35) {
      int freq = map((int)temperatureC, 35, 50, 1000, 2000);
      tone(buzzerPin, freq);
    } else {
      noTone(buzzerPin);
    }

    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temperatureC);
    lcd.setCursor(0, 1);
    lcd.print("Fan:");
    lcd.print(fanSpeed);
    lcd.print(" L:");
    lcd.print(level);
    delay(2000);

    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Status:");
    lcd.setCursor(0, 1);
    if (temperatureC > highTempThreshold){
      lcd.print("HOT ");
    } else if (temperatureC < coldAlert) {
      lcd.print("FREEZE ");
    } else if (temperatureC < lowTempThreshold){
      lcd.print("Heating...");
    } else if (fanSpeed > 0){
      lcd.print("Cooling...");
    } else {
      lcd.print("Normal");
    }
    delay(2000);

    
    Serial.print("Temp: ");
    Serial.print(temperatureC);
    Serial.print(" °C | Fan: ");
    Serial.print(fanSpeed);
    Serial.print(" | Level: ");
    Serial.print(level);
    Serial.print(" | Heater: ");
    Serial.print(digitalRead(heaterPin));
    Serial.print(" | Buzzer: ");
    Serial.println(digitalRead(buzzerPin));
  }

  delay(1000);
}
