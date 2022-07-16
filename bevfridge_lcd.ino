#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);


int relayPin = 6;
double high;  //set the high temp
double low;   //  set the low temp

int potL = 1;
int potH = 2;
double adjVal;
int potAdj = 3;

int relay = 0;
boolean lowReached = false;
boolean highReached = false;

const int numReadings = 100;

int readings[numReadings];
int readIndex = 0;
double total = 0;
double average = 0;


void setup() {
  // put your setup code here, to run once:
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, 0);
  Serial.begin(9600);
  
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Fridge control");
  delay(5000);
  lcd.clear();

  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }

}

void loop() {
  // set low and high
  low = map(analogRead(potL), 0, 1023, 0, 6);
  high = map(analogRead(potH), 0, 1023, 4, 12);

  adjVal = map(analogRead(potAdj), 0.0, 1023.0, -5.0, 5.0);


  //temperature
  total = total - readings[readIndex];
  readings[readIndex] = getTempC(0);  //derfine temp pin and read from function
  total = total + readings[readIndex];
  readIndex = readIndex + 1;
  if (readIndex >= numReadings) {
    readIndex = 0;
  }
  average = (total / numReadings) + adjVal;
  Serial.println(average);

  lcd.setCursor(0, 0);
  lcd.print("Temp:");
  lcd.setCursor(5, 0);
  lcd.print(average, 1);

  lcd.setCursor(0, 1);
  lcd.print("Adj:");
  lcd.setCursor(4, 1);
  lcd.print(adjVal, 1);

  lcd.setCursor(11, 0);
  lcd.print("L:");
  lcd.setCursor(13, 0);
  lcd.print(low, 1);

  lcd.setCursor(11, 1);
  lcd.print("H:");
  lcd.setCursor(13, 1);
  lcd.print(high, 1);


  //control
  if (average >= high && lowReached == false) {
    digitalWrite(relayPin, 1);
    lowReached = true;
    lcd.clear();
    lcd.setCursor(8, 1);
    lcd.print("ON");
  }

  if (average <= low && lowReached == true) {
    digitalWrite(relayPin, 0);
    lowReached = false;
    lcd.clear();
    lcd.setCursor(8, 1);
    lcd.print("  ");
      }

  delay(10);
  
}

////A function to get the temperature
double getTempC(int tempPin) {
  int adcReading = analogRead(tempPin);
  double tempC = (5.1 * adcReading * 100.0) / 1024.0;
  return tempC;
}
