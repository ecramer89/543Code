int pins[4] = {1, 2, 3, 4};
int delay_between_colors = 400;
unsigned long start_time;



const int num_pattern_component_dimensions = 3;
const int INDEX_OF_CATHODE_PIN = 0;
const int INDEX_OF_ANODE_PIN = 1;
const int INDEX_OF_DURATION = 2;

const int pattern_length = 4;
int index_of_current_pattern_component;
int *curr_pattern_component;
int pattern[pattern_length][num_pattern_component_dimensions] = {
  {2, 3, 200},
  {2, 1, 200},
  {3, 2, 200},
  {2, 3, 200}
};



void setup() {
  int num_pins = sizeof(pins) / sizeof(pins[0]);
  // put your setup code here, to run once:
  for (int i = 0; i < num_pins; i++) {
    pinMode(pins[i], INPUT);
  }
}



void loop() {
  unsigned long current_time = millis();
  if (timeToSwitchPatternComponent(current_time)) {
    disconnectCurrentAnode();
    disconnectCurrentCathode();
    setupNextPatternComponent();
  }
}


bool timeToSwitchPatternComponent(unsigned long curr_time) {
  return abs(curr_time - start_time) >= delay_between_colors;
}


void disconnectCurrentAnode() {
  int anodePin = pins[curr_pattern_component[INDEX_OF_ANODE_PIN]];
  disconnect(anodePin);
}


void disconnectCurrentCathode() {
  int groundPin = pins[curr_pattern_component[INDEX_OF_CATHODE_PIN]];
  disconnect(groundPin);
}



void setupNextPatternComponent() {
  setIndexOfNextPatternComponent();
  setNextPatternComponent();
  groundCathodeOfCurrentPatternComponent();
  lightupAnodeOfCurrentPatternCathode();
  setCurrentPatternComponentDuration();
  start_time = millis();
}

void setNextPatternComponent(){
  curr_pattern_component=pattern[index_of_current_pattern_component];
}

void groundCathodeOfCurrentPatternComponent() {
  int ground_pin = pins[curr_pattern_component[INDEX_OF_CATHODE_PIN]];
  makeGround(ground_pin);
}


void lightupAnodeOfCurrentPatternCathode() {
  int anode_pin = pins[curr_pattern_component[INDEX_OF_ANODE_PIN]];
  lightUp(anode_pin);
}


void setCurrentPatternComponentDuration() {
  delay_between_colors = curr_pattern_component[INDEX_OF_DURATION];
}


void setIndexOfNextPatternComponent() {
  if (index_of_current_pattern_component < pattern_length) {
    index_of_current_pattern_component++;
  }
  else {
    index_of_current_pattern_component = 0;
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
