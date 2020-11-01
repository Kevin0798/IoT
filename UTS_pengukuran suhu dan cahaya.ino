#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

#define DHTTYPE DHT11
#define DHTPIN 25
#define ldrPin 39
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

char auth[] = "7IFMbE-Sd_BPaaO2ntWDmb9Qb3IWYGnj";
char ssid[] = "KevinHB";
char pass[] = "ShadowTwice0798";

WidgetLED luxIndikator(V6);
WidgetLED indSuhu(V7);

int tombol = V8;
float sensorValue;
float lux = 0.00;
int suhuSlider = V5;
int luxSlider = V4;

BLYNK_WRITE(V5) // Slider Suhu dan V5 adalah nomor dari virtual pin di aplikasi
{
  suhuSlider = param.asInt();
  Serial.println(suhuSlider);  
}

BLYNK_WRITE(V8) // tombol start/stop sistem
{
  tombol = param.asInt();
  Serial.println(tombol);  
}

BLYNK_WRITE(V4) // slider lux
{
  luxSlider = param.asInt();
  Serial.println(luxSlider);  
}

void sendSensor() // pemanggilan fungsi
{
  
  if(tombol == 1){ 
    suhu();
    luxFunction();
  }
  
}

void suhu(){
      
  float t = dht.readTemperature();
  
  if(t <= suhuSlider){
    digitalWrite(23, HIGH); // lampu fisik hijau
    Blynk.setProperty(V7, "color", "#2EFE00"); // Hijau
    indSuhu.on(); // lampu virtual dari aplikasi     
  }
  if(t > suhuSlider){
    Blynk.setProperty(V7, "color", "#FE0000"); // Merah
    digitalWrite(23, LOW); // lampu fisik hijau
    indSuhu.on(); // lampu virtual dari aplikasi
  }
  if (isnan(t)) { // kondisi untuk dht11 jika tidak terhubung
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Blynk.virtualWrite(V1, t); // Grafik SUHU(super chart)
  Blynk.virtualWrite(V3, t); // label Temperatur
}

void luxFunction(){
  
  sensorValue = analogRead(ldrPin); // membaca nilai atau value dari pin
  Serial.println(sensorValue);
  
  lux= (0.009758*sensorValue) + 10;

  if(lux < luxSlider){
    Serial.println("On");
    digitalWrite(22, HIGH);
    Blynk.setProperty(V6, "color", "#2EFE00"); // hijau
    luxIndikator.on();
  }
  if(lux > luxSlider){
    Serial.println("Off");
    digitalWrite(22, LOW);
    Blynk.setProperty(V6, "color", "#FE0000"); // merah
    luxIndikator.on();
  }
  Blynk.virtualWrite(V0, lux); // grafik lux(superchart)
  Blynk.virtualWrite(V2, lux); // label Kecerahan
  
}

void setup()
{
  Serial.begin(115200);
  
  dht.begin();
  timer.setInterval(1000L, sendSensor);  // pemanggilan fungsi atau start sistem

  // pengecekan wifi jika terhubung
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  int wifi_ctr = 0;
  pinMode(22, OUTPUT);
  pinMode(23, OUTPUT);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");  
 
  Blynk.begin(auth, ssid, pass, IPAddress(192,168,100,46),8080);
  
}

void loop()
{
  Blynk.run();
  timer.run();
}
