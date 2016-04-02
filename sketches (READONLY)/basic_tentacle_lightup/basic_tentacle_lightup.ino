
int pins[4] = {5,6,7,8};
int numPins;
int delay_between_colors=500;
int delay_between_leds=500;

void setup() {
  numPins = sizeof(pins) / sizeof(pins[0]);
  // put your setup code here, to run once:
  for (int i = 0; i < numPins; i++) {
    pinMode(pins[i], INPUT);
  }
}


void makeGround(int groundPin) {
  pinMode(groundPin, OUTPUT);
  digitalWrite(groundPin, LOW);
}

void lightUp(int anodePin) {
  pinMode(anodePin, OUTPUT);
  digitalWrite(anodePin, HIGH);
}

void disconnect(int pin) {
  pinMode(pin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < numPins; i++) {
    int groundPin = pins[i];
    makeGround(groundPin);
    int j = i + 1;
    for (int l = 0; l < numPins - 1; l++) {
      if (j >= numPins) {
        j = 0;
      }
      int anodePin = pins[j];
      lightUp(anodePin);
      delay(delay_between_colors);
      disconnect(anodePin);
      j++;
    }
    disconnect(groundPin);
    delay(delay_between_leds);
  }

}





