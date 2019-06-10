#include <Arduino.h>
#include <SR04.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal.h>

#define TRIG_PIN 5
#define ECHO_PIN 4
#define DHTPIN 2
#define DHTTYPE DHT11
#define SOILRELE 1
#define PUMPRELE 3
#define SOILSENSOR 0

//Variable timer depending on dry level
int long_timer = 60*30;
int short_timer = 2;
int timer = long_timer;

//Distance sensor variable initializing
SR04 sr04 = SR04(ECHO_PIN, TRIG_PIN);
long wtank_distance;
float wtank_volume;
float wtank_radius = 0.125;
float wtank_height = 0.17;
float wtank_maxvolume = wtank_height * PI * wtank_radius * wtank_radius * 1000;
float wtank_minvol = 1.5;

//Temp&Hum sensor variable initializing
long humidity;
long temperature;
long temp_limit =23;
long temp_prev;
DHT_Unified dht(DHTPIN, DHTTYPE);

// LCD variable initializing
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

//Soil moisture sensor variable initializing
int drysoillimit = 50;
float soilmoisturelevel = 1024;

void setup()
{
  pinMode(SOILRELE, OUTPUT);
  pinMode(PUMPRELE, OUTPUT);
  digitalWrite(SOILRELE, HIGH);
  digitalWrite(PUMPRELE, HIGH);

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
void measWaterLevel()
{
  delay(200);
  //Get distance measure from sensor
  wtank_distance = sr04.Distance();

  //Transform distance to remaining litres
  wtank_volume = wtank_maxvolume - (PI * wtank_radius * wtank_radius * wtank_distance/100)*1000;
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

void lcdUpdate()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T=");
  lcd.print(temperature);
  lcd.print(F(" C"));

  lcd.print("   ");

  lcd.print("H=");
  lcd.print(temperature);
  lcd.print(F("%"));

  lcd.setCursor(0, 1);
  lcd.print("V=");
  lcd.print(wtank_volume);
  lcd.print("L");

  lcd.print(" S=");
  lcd.print(soilmoisturelevel);
  lcd.print("%");
}

void measSoilMoisture()
{
  digitalWrite(SOILRELE, LOW);
  delay(200);
  soilmoisturelevel = analogRead(SOILSENSOR);
  soilmoisturelevel = abs(((((soilmoisturelevel - 112) / 850)) - 1) * 100);
  digitalWrite(SOILRELE, HIGH);
}

void loop()
{
  //Start measure loop
  measWaterLevel();
  measTempHumid();
  measSoilMoisture();

  //First of all, check if water is available to avoid breaking the pump.
  if (wtank_volume > wtank_minvol && temperature < temp_limit)
  {
    //If soil is wet
    if (soilmoisturelevel >= drysoillimit)
    {
      //Switch off water pump
      digitalWrite(PUMPRELE, HIGH);
      //Set timer to long period between measurements
      timer = long_timer;
    }

    //If soil is dry
    else if (soilmoisturelevel < drysoillimit)
    {
      //Check if water is available in tank before activating pump
      if (wtank_volume > wtank_minvol)
      {
        //Switch on water pump
        digitalWrite(PUMPRELE, LOW);
        //Set timer to short period between measurements
        timer = short_timer;
      }
    }
  }

  //Update LCD with new data
  lcdUpdate();

  //Wait for new measurement
  for (size_t i = 0; i < timer; i++)
  {
    delay(1000);
  }
}