const int RED = 0;
const int GREEN = 1;
const int BLUE = 2;

const int FIRST = 0;
const int SECOND = 1;
const int THIRD = 2;
const int FOURTH = 3;

int A_ROWS[] = {2, 3, 1, 0};
int A_COLORS[][3] = {
  {0, 1, 3},
  {2, 0, 1},
  {3, 2, 0},
  {1, 3, 2}
};

int B_ROWS[] = {3, 0, 1, 2};
int B_COLORS[][3] = {
  {0, 1, 2},
  {1, 2, 3},
  {2, 3, 0},
  {3, 0, 1}
};


const int num_tentacles = 2;
const int num_pins_per_tentacle = 4;
int default_duration_of_pattern_component = 200;
int pins[num_tentacles][num_pins_per_tentacle] = {
  {1, 2, 3, 5},
  {8, 9, 10, 11}

};

int duration_of_pattern_component[num_tentacles] = {0, 0};
unsigned long start_time_of_pattern_component[num_tentacles] = {0, 0};

const int num_pattern_component_dimensions = 4;
const int INDEX_OF_CATHODE_PIN = 0;
const int INDEX_OF_ANODE_PIN = 1;
const int INDEX_OF_DURATION = 2;
const int INDEX_OF_INSTRUCTION = 3;

const int OFF = -12;
const int ON = 14;
const int LOOP = 22;

const int pattern_length = 4;
int index_of_current_pattern_component[num_tentacles] = {0, 0};
int curr_pattern_component[num_tentacles][num_pattern_component_dimensions] = {
  {0, 1, default_duration_of_pattern_component, ON},
  {0, 1, default_duration_of_pattern_component, ON}
};


//catode index, +1%4=red, +2%4=green, +3%4=blue anodes.
int pattern[num_tentacles][pattern_length][num_pattern_component_dimensions] = {
  {
    {A_ROWS[FIRST], A_COLORS[FIRST][GREEN], 600, ON},
    {A_ROWS[SECOND], A_COLORS[SECOND][GREEN], 600, OFF},
    {A_ROWS[THIRD], A_COLORS[THIRD][GREEN], 600, ON},
    {A_ROWS[FOURTH], A_COLORS[FOURTH][GREEN], 600, OFF}
  },

  {
    {B_ROWS[FIRST], B_COLORS[FIRST][GREEN], 600, OFF},
    {B_ROWS[SECOND], B_COLORS[SECOND][GREEN], 600, ON},
    {B_ROWS[THIRD], B_COLORS[THIRD][GREEN], 600, OFF},
    {B_ROWS[FOURTH], B_COLORS[FOURTH][GREEN], 600, ON}
  }

};



void setup() {

  disconnectAllPins();
}

void disconnectAllPins() {
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
  unsigned long start_time_of_pattern_component_of_this_tentacle_current_pattern_component = start_time_of_pattern_component[index_of_tentacle];
  int duration_of_this_tentacle_current_pattern_component = duration_of_pattern_component[index_of_tentacle];
  return abs(curr_time - start_time_of_pattern_component_of_this_tentacle_current_pattern_component) >= duration_of_this_tentacle_current_pattern_component;
}

int getActualPinValue(int index_of_tentacle, int pin_type_index) {
  int index_of_pin = curr_pattern_component[index_of_tentacle][pin_type_index];
  int actual_pin_value = pins[index_of_tentacle][index_of_pin];
  return actual_pin_value;
}

void disconnectCurrentAnode(int index_of_tentacle) {
  int pin = getActualPinValue(index_of_tentacle, INDEX_OF_ANODE_PIN);
  disconnect(pin);
}


void disconnectCurrentCathode(int index_of_tentacle) {
  int pin = getActualPinValue(index_of_tentacle, INDEX_OF_CATHODE_PIN);
  disconnect(pin);
}



void setupNextPatternComponent(int index_of_tentacle) {
  setIndexOfNextPatternComponent(index_of_tentacle);
  setNextPatternComponent(index_of_tentacle);


  if (on(index_of_tentacle)) {
    groundCathodeOfCurrentPatternComponent(index_of_tentacle);
    lightupAnodeOfCurrentPatternCathode(index_of_tentacle);
  }


  setCurrentPatternComponentDuration(index_of_tentacle);
  start_time_of_pattern_component[index_of_tentacle] = millis();
}

bool on(int index_of_tentacle) {
  int instruction = curr_pattern_component[index_of_tentacle][INDEX_OF_INSTRUCTION];
  return instruction != OFF;
}


//in c one cannot assign declared arrays to indices in a multidimensional array.
//to get around issue, just assign each dimension separately.
void setNextPatternComponent(int index_of_tentacle) {
  for (int i = 0; i < num_pattern_component_dimensions; i++) {
    curr_pattern_component[index_of_tentacle][i] = pattern[index_of_tentacle][index_of_current_pattern_component[index_of_tentacle]][i];
  }
}

void groundCathodeOfCurrentPatternComponent(int index_of_tentacle) {
  int pin = getActualPinValue(index_of_tentacle, INDEX_OF_CATHODE_PIN);
  makeGround(pin);
}


void lightupAnodeOfCurrentPatternCathode(int index_of_tentacle) {
  int pin = getActualPinValue(index_of_tentacle, INDEX_OF_ANODE_PIN);

  lightUp(pin);
}



void setCurrentPatternComponentDuration(int index_of_tentacle) {
  duration_of_pattern_component[index_of_tentacle] = curr_pattern_component[index_of_tentacle][INDEX_OF_DURATION];
}


void setIndexOfNextPatternComponent(int index_of_tentacle) {
  int current_index = index_of_current_pattern_component[index_of_tentacle];

  if (current_index < pattern_length - 1) {
    index_of_current_pattern_component[index_of_tentacle] = current_index + 1;

  }
  else {
    index_of_current_pattern_component[index_of_tentacle] = 0;
  }
}

void makeGround(int groundPin) {
  pinMode(groundPin, OUTPUT);
  Serial.println(groundPin);
  digitalWrite(groundPin, LOW);
}

void lightUp(int anodePin) {
  pinMode(anodePin, OUTPUT);

  //Serial.println("anode: "+anodePin);
  digitalWrite(anodePin, HIGH);
}





void disconnect(int pin) {
  pinMode(pin, INPUT);
}
