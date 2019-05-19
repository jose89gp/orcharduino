#include <Arduino.h>
#include <SR04.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal.h>

#define TRIG_PIN 13
#define ECHO_PIN 12
#define DHTPIN 2
#define DHTTYPE    DHT11 
#define RED 3
#define GREEN 5
#define BLUE 6

SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
long a;
long humidity;
long temperature;
DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS = 500;
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(0,7,8,9,10,11);


void setup() {
  pinMode(RED,OUTPUT);
  pinMode(GREEN,OUTPUT);
  pinMode(BLUE,OUTPUT);

  digitalWrite(RED,LOW);
  digitalWrite(GREEN, HIGH);
  digitalWrite(BLUE,HIGH);
  
   //Serial.begin(9600);
   dht.begin();
   // Print temperature sensor details.
   sensor_t sensor;
   dht.temperature().getSensor(&sensor);
   // Print humidity sensor details.
   dht.humidity().getSensor(&sensor);
   // set up the LCD's number of columns and rows:
   lcd.begin(16, 2);
   // Print a message to the LCD.
   lcd.print("Initialiting...");
}

void loop() {

   a=sr04.Distance();
   Serial.print(a);
   Serial.println("cm");
   delay(delayMS); 
   analogWrite(RED,rand()*255);
   analogWrite(GREEN,rand()*255);
   analogWrite(BLUE,rand()*255);
   sensors_event_t event;
   dht.temperature().getEvent(&event);
   if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
   }
   else {
    Serial.print(F("Temperature: "));
    temperature = event.temperature;
    Serial.print(temperature);
    Serial.println(F("°C"));
   }
   // Get humidity event and print its value.
   dht.humidity().getEvent(&event);
   if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    Serial.print(F("Humidity: "));
    humidity = event.relative_humidity;
    Serial.print(humidity);
    Serial.println(F("%"));
  }
 
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T=");
  lcd.print(temperature);
  lcd.print(F(" C"));

  lcd.print("   ");

  lcd.print("H=");
  lcd.print(temperature);
  lcd.print(F(" %"));

  lcd.setCursor(0,1); 
  lcd.print("D=");
  lcd.print(a);
  lcd.print("cm");

  lcd.print("   ");
  lcd.print(analogRead(0));
}