#include <Arduino.h>
#include <SR04.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define TRIG_PIN 13
#define ECHO_PIN 12
#define DHTPIN 2 
#define DHTTYPE    DHT11 
SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
long a;
DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS = 2000;


void setup() {
   Serial.begin(9600);
   dht.begin();
   // Print temperature sensor details.
   sensor_t sensor;
   dht.temperature().getSensor(&sensor);
   // Print humidity sensor details.
   dht.humidity().getSensor(&sensor);
}

void loop() {

   a=sr04.Distance();
   Serial.print(a);
   Serial.println("cm");
   delay(delayMS); 
   
   sensors_event_t event;
   dht.temperature().getEvent(&event);
   if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
   }
   else {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
   }
   // Get humidity event and print its value.
   dht.humidity().getEvent(&event);
   if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
  }
}