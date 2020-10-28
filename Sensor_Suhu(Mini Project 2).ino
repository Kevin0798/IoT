#include <Adafruit_Sensor.h>
#include <DHT.h>

#define DHTPIN 25
#define DHTTYPE DHT11
#define LED_TOO_COLD 23
#define LED_PERFECT 22
#define LED_TOO_HOT 21
DHT dht(DHTPIN, DHTTYPE);

int tombolPin = 19;

int buttonNew;
int buttonOld = 1;
int flag = 0;
float t;
int dt = 100;

void setup() {
  pinMode(tombolPin, INPUT);
  pinMode (23 , OUTPUT);
  pinMode (22 , OUTPUT);
  pinMode (21 , OUTPUT);
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  
  buttonNew = digitalRead(tombolPin);
  if(buttonOld == 0 && buttonNew == 1){
      flag = 1;
  }
  
  if(flag == 1){
    delay(2000);
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    //float f = dht.readTemperature(true);
    if (isnan(t)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
      }
      Serial.print("Humidity: ");
      Serial.print(h);
      Serial.print(" %\t");
      Serial.print("Temperature: ");
      Serial.print(t);
      Serial.println(" *C ");
      
      if (t <= 35) {
        Serial.println("Normal");
        digitalWrite(23, HIGH);
        digitalWrite(22, LOW);  
        digitalWrite(21, LOW);
      }
      if (t >=35.00 && t <= 50.00) {
        Serial.println("Hangat");
        digitalWrite(21, LOW);
        digitalWrite(22, HIGH);
        digitalWrite(23, HIGH);
      }
      if (t >= 50.00) {
        Serial.println("Panas");
        digitalWrite(21, HIGH);
        digitalWrite(22, HIGH);
        digitalWrite(23, HIGH);
        
      }
  }
  buttonOld = buttonNew;
  delay(dt);
}
