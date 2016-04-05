const int num_tentacles = 2;
const int num_pins_per_tentacle = 4;
int default_delay_between_colors = 400;
int pins[num_tentacles][num_pins_per_tentacle] = {
  {1, 2, 3, 4},
  {8, 9, 10, 11}
};

int delay_between_colors[num_tentacles] = {0, 0};
unsigned long start_time[num_tentacles] = {0, 0};



const int num_pattern_component_dimensions = 3;
const int INDEX_OF_CATHODE_PIN = 0;
const int INDEX_OF_ANODE_PIN = 1;
const int INDEX_OF_DURATION = 2;

const int pattern_length = 4;
int index_of_current_pattern_component[num_tentacles] = {0, 0};
int curr_pattern_component[num_tentacles][num_pattern_component_dimensions] = {
  {0, 1, default_delay_between_colors},
  {0, 1, default_delay_between_colors}
};

int pattern[num_tentacles][pattern_length][num_pattern_component_dimensions] = {
  {
    {3, 0, 10},
    {3, 0, 10},
    {3, 0, 10},
    {1, 0, 10}
  },

  {
    {3, 0, 10},
    {2, 1, 10},
    {0, 3, 10},
    {1, 2, 10}
  }

};



void setup() {
  for (int j = 0; j < num_tentacles; j++) {
    // put your setup code here, to run once:
    for (int i = 0; i < num_pins_per_tentacle; i++) {
      pinMode(pins[j][i], INPUT);
    }
  }
}



void updateTentaclePatternComponentIfNecessary(int index_of_tentacle) {
  unsigned long current_time = millis();
  if (timeToSwitchPatternComponent(current_time, index_of_tentacle)) {
    disconnectCurrentAnode(index_of_tentacle);
    disconnectCurrentCathode(index_of_tentacle);
    setupNextPatternComponent(index_of_tentacle);
  }
}



void loop() {
  for (int i = 0; i < num_tentacles; i++) {
    updateTentaclePatternComponentIfNecessary(i);
  }
}


bool timeToSwitchPatternComponent(unsigned long curr_time, int index_of_tentacle) {
  return abs(curr_time - start_time[index_of_tentacle]) >= delay_between_colors[index_of_tentacle];
}


void disconnectCurrentAnode(int index_of_tentacle) {
  int anodePin = getActualPinValueGivenIndicesOfPinAndOfTentacle(INDEX_OF_ANODE_PIN, index_of_tentacle); //pins[index_of_tentacle][curr_pattern_component[index_of_tentacle][INDEX_OF_ANODE_PIN]];
  disconnect(anodePin);
}


void disconnectCurrentCathode(int index_of_tentacle) {
  int groundPin = getActualPinValueGivenIndicesOfPinAndOfTentacle(INDEX_OF_CATHODE_PIN, index_of_tentacle); //pins[index_of_tentacle][curr_pattern_component[index_of_tentacle][INDEX_OF_CATHODE_PIN]];
  disconnect(groundPin);
}



void setupNextPatternComponent(int index_of_tentacle) {
  setIndexOfNextPatternComponent(index_of_tentacle);
  setNextPatternComponent(index_of_tentacle);
  groundCathodeOfCurrentPatternComponent(index_of_tentacle);
  lightupAnodeOfCurrentPatternCathode(index_of_tentacle);
  setCurrentPatternComponentDuration(index_of_tentacle);
  start_time[index_of_tentacle] = millis();
}

//in c one cannot assign declared arrays to indices in a multidimensional array.
//to get around issue, just assign each dimension separately.
void setNextPatternComponent(int index_of_tentacle) {
  curr_pattern_component[index_of_tentacle][INDEX_OF_CATHODE_PIN] = pattern[index_of_tentacle][index_of_current_pattern_component[index_of_tentacle]][INDEX_OF_CATHODE_PIN];
  curr_pattern_component[index_of_tentacle][INDEX_OF_ANODE_PIN] = pattern[index_of_tentacle][index_of_current_pattern_component[index_of_tentacle]][INDEX_OF_ANODE_PIN];
  curr_pattern_component[index_of_tentacle][INDEX_OF_DURATION] = pattern[index_of_tentacle][index_of_current_pattern_component[index_of_tentacle]][INDEX_OF_DURATION];
}

void groundCathodeOfCurrentPatternComponent(int index_of_tentacle) {
  int ground_pin = getActualPinValueGivenIndicesOfPinAndOfTentacle(INDEX_OF_CATHODE_PIN, index_of_tentacle); //pins[index_of_tentacle][curr_pattern_component[index_of_tentacle][INDEX_OF_CATHODE_PIN]];
  makeGround(ground_pin);
}


void lightupAnodeOfCurrentPatternCathode(int index_of_tentacle) {
  int anode_pin = getActualPinValueGivenIndicesOfPinAndOfTentacle(INDEX_OF_ANODE_PIN, index_of_tentacle); //pins[index_of_tentacle][curr_pattern_component[index_of_tentacle][INDEX_OF_ANODE_PIN]];
  lightUp(anode_pin);
}


void setCurrentPatternComponentDuration(int index_of_tentacle) {
  delay_between_colors[index_of_tentacle] = curr_pattern_component[index_of_tentacle][INDEX_OF_DURATION];
}


void setIndexOfNextPatternComponent(int index_of_tentacle) {
  if (index_of_current_pattern_component[index_of_tentacle] < pattern_length) {
    index_of_current_pattern_component[index_of_tentacle] = index_of_current_pattern_component[index_of_tentacle] + 1;
  }
  else {
    index_of_current_pattern_component[index_of_tentacle] = 0;
  }
}


int getActualPinValueGivenIndicesOfPinAndOfTentacle(int index_of_pin, int index_of_tentacle) {
  return pins[index_of_tentacle][curr_pattern_component[index_of_tentacle][index_of_pin]];
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
