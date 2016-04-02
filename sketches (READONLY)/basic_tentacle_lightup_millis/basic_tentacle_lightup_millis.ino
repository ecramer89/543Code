int pins[4]={1,2,3,4};

int num_cathodes;
int num_anodes_per_cathode = 3;
int delay_between_colors = 400;



int index_of_current_cathode;
int index_of_current_anode;
int num_anodes_lit_up;
unsigned long start_time;

void setup() {
  num_cathodes = sizeof(pins) / sizeof(pins[0]);
  // put your setup code here, to run once:
  for (int i = 0; i < num_cathodes; i++) {
    pinMode(pins[i], INPUT);
  }
}


bool timeToSwitchColor(unsigned long curr_time) {

  return abs(curr_time - start_time) >= delay_between_colors;

}


void disconnectCurrentAnode() {
  int anodePin = pins[index_of_current_anode];
  disconnect(anodePin);
}

bool isANextAnodeForCurrentCathode() {
  return num_anodes_lit_up <= num_anodes_per_cathode;
}

void setNextAnode() {
  if (index_of_current_anode < num_cathodes) {
    index_of_current_anode++;
  }
  else {
    index_of_current_anode = 0;
  }
}

void lightupCurrentAnode() {
  int anodePin = pins[index_of_current_anode];
  lightUp(anodePin);
}


void switchCathode() {
  disconnectCurrentCathode();
  setNextCathode();
  groundCurrentCathode();
  setFirstAnodeForNewCathode();
  lightupCurrentAnode();
  num_anodes_lit_up = 1;
  start_time = millis();
}

void setNextCathode() {
  if (index_of_current_cathode < num_cathodes)
    index_of_current_cathode++;
  else index_of_current_cathode = 0;
}


void groundCurrentCathode() {
  int groundPin = pins[index_of_current_cathode];
  makeGround(groundPin);
}


void disconnectCurrentCathode() {
  int groundPin = pins[index_of_current_cathode];
  disconnect(groundPin);
}


void setFirstAnodeForNewCathode() {
  if (index_of_current_cathode == num_cathodes - 1) {
    index_of_current_anode = 0;
  } else index_of_current_anode = index_of_current_cathode + 1;
}




void switchAnode() {
  setNextAnode();
  lightupCurrentAnode();
  num_anodes_lit_up++;
  start_time = millis();
}


void loop() {
  unsigned long current_time = millis();
  if (timeToSwitchColor(current_time)) {
    disconnectCurrentAnode();
    if (isANextAnodeForCurrentCathode()) {
      switchAnode();
    }
    else {//new cathode.
      switchCathode();
    }

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
