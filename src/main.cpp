#include <Arduino.h>
#include <SR04.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal.h>

#define TRIG_PIN 13
#define ECHO_PIN 12
#define DHTPIN 2
#define DHTTYPE DHT11
#define RED 3
#define GREEN 5
#define BLUE 6

//Distance sensor variable initializing
SR04 sr04 = SR04(ECHO_PIN, TRIG_PIN);
long wtank_distance;
float wtank_volume;
float wtank_radius = 0.2;
float wtank_height = 0.3;
float wtank_maxvolume = wtank_height * PI * wtank_radius * wtank_radius * 1000;

//Temp&Hum sensor variable initializing
long humidity;
long temperature;
DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS = 500;

// LCD variable initializing
LiquidCrystal lcd(0, 7, 8, 9, 10, 11);

void setup()
{
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  digitalWrite(RED, LOW);
  digitalWrite(GREEN, HIGH);
  digitalWrite(BLUE, HIGH);

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

void loop()
{
  measWaterLevel();
  measTempHumid();
  delay(delayMS);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T=");
  lcd.print(temperature);
  lcd.print(F(" C"));

  lcd.print("   ");

  lcd.print("H=");
  lcd.print(temperature);
  lcd.print(F(" %"));

  lcd.setCursor(0, 1);
  lcd.print("D=");
  lcd.print(wtank_distance);
  lcd.print("cm");

  lcd.print("   ");
  lcd.print(analogRead(0));
  if (analogRead(0) > 700)
  {
    analogWrite(RED, 255);
    analogWrite(GREEN, 0);
    analogWrite(BLUE, 0);
  }
  if (analogRead(0) < 700)
  {
    analogWrite(RED, 0);
    analogWrite(GREEN, 255);
    analogWrite(BLUE, 0); /* code */
  }
}

void measWaterLevel()
{
  //Get distance measure from sensor
  wtank_distance = sr04.Distance();
  Serial.print(wtank_distance);
  Serial.println("cm");

  //Transform distance to remaining litres
  wtank_volume = wtank_maxvolume - (PI * wtank_radius * wtank_radius * wtank_distance);
  Serial.print("Remaining water: ");
  Serial.println(wtank_volume);
}

void measTempHumid()
{
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature))
  {
    Serial.println(F("Error reading temperature!"));
  }
  else
  {
    Serial.print(F("Temperature: "));
    temperature = event.temperature;
    Serial.print(temperature);
    Serial.println(F("°C"));
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity))
  {
    Serial.println(F("Error reading humidity!"));
  }
  else
  {
    Serial.print(F("Humidity: "));
    humidity = event.relative_humidity;
    Serial.print(humidity);
    Serial.println(F("%"));
  }
}