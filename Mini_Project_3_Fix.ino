#include <WiFi.h>
#include <MQTT.h>
#include <ESP32Servo.h>
#include <MQTT.h>
#include <MQTTClient.h>
#define ldrPin 36

const char ssid[] = "BADBOYS";
const char pass[] = "bayardulu365";
String namaBoard = "ESP32";

WiFiClient net;
MQTTClient client;
Servo mtrServo;

int servoPin = 23;
int posisiServo = 0;
int sensorValue;

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.print("\nconnecting...");
  while (!client.connect("ESP32", "58da1246", "31be56e9906c78fd")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  client.subscribe("/test");
  // client.unsubscribe("/hello");
}

void messageReceived(String &topic, String &payload) {

  Serial.println("incoming: " + topic + " - " + payload);

  if (topic == "/test") {
    if (payload == "1" && sensorValue >= 200) {
      for (posisiServo = 0; posisiServo <= 360; posisiServo += 1) { // goes from 0 degrees to 180 degrees
 
        Serial.println("Servo is run to 180 degrees");
        mtrServo.write(posisiServo);
      }
      client.publish("/servomessage", "Servo is run to 180 degrees");

    }
    else if (payload == "0" && sensorValue <= 2000) {
      for (posisiServo = 360; posisiServo >= 0; posisiServo -= 1) { // goes from 180 degrees to 0 degrees
        Serial.println("Servo is run to 0 degrees");
        mtrServo.write(posisiServo);    // tell servo to go to position in variable 'posisiServo'
      }
      client.publish("/servomessage", "Servo is run 0 degrees");
    
    }
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  mtrServo.setPeriodHertz(50);    // set periodHertz
  mtrServo.attach(servoPin, 500, 2400); // setup pin
  client.begin("broker.shiftr.io", net);
  client.onMessage(messageReceived);
  
  connect();
}

void loop() {
  sensorValue = analogRead(ldrPin);
  Serial.println(sensorValue);
  client.loop();
  delay(10);  // <- fixes some issues with WiFi stability

  if (!client.connected()) {
    connect();
  }

}
