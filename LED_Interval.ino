

const int red =  21;
const int yellow = 22;
const int green = 23;

long startTime = 0;

long interval = 8000;


void setup() {
  
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);
}

void loop() {
 
  unsigned long currentTime = millis();
  unsigned long elapsedTime = currentTime - startTime;


  if (elapsedTime > interval) {
    startTime = currentTime;
  }

  if (elapsedTime < 3000){
    digitalWrite(red,LOW);
    digitalWrite(yellow,LOW);
    digitalWrite(green,HIGH);
  }

  if (elapsedTime > 3000&&elapsedTime < 5000){
    digitalWrite(red,LOW);
    digitalWrite(yellow,HIGH);
    digitalWrite(green,LOW);
  }

  if (elapsedTime > 6000){
    digitalWrite(red,HIGH);
    digitalWrite(yellow,LOW);
    digitalWrite(green,LOW);
  }

}
