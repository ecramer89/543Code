const int TENTACLE_A = 0;
const int TENTACLE_B = 1;

const int RED = 0;
const int GREEN = 1;
const int BLUE = 2;

const int FIRST = 0;
const int SECOND = 1;
const int THIRD = 2;
const int FOURTH = 3;
const int num_tentacles = 2;
const int num_pins_per_tentacle = 4;


/*int A_ROWS[] = {2, 3, 1, 0};
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
  };*/


int ROWS[num_tentacles][num_pins_per_tentacle] = {
  {2, 3, 1, 0},
  {3, 0, 1, 2}
};

int COLORS[num_tentacles][num_pins_per_tentacle][3] = {
  {
    {0, 1, 3},
    {2, 0, 1},
    {3, 2, 0},
    {1, 3, 2}
  },
  {
    {0, 1, 2},
    {1, 2, 3},
    {2, 3, 0},
    {3, 0, 1}
  }

};



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

//FIRST ARRAY= tentacle A.
//SECOND ARRAY=tentacle B.
int user_defined_pattern[num_tentacles][pattern_length][num_pattern_component_dimensions] = {
  { //row | color | duration | on or off. (don't need to enter a meaningful row or colour if you want a section to be off, but do need to enter a duration)
    {FIRST, RED, 600, ON},
    {SECOND, GREEN, 600, ON},
    {OFF, OFF, 600, OFF},
    {THIRD, BLUE, 600, ON}
  },
  {
    {THIRD, BLUE, 200, ON},
    {FOURTH, GREEN, 3000, ON},
    {FOURTH, RED, 200, ON},
    {OFF, OFF, 600, OFF}
  }
};


int index_of_current_pattern_component[num_tentacles] = {0, 0};
int curr_pattern_component[num_tentacles][num_pattern_component_dimensions] = {
  {0, 1, default_duration_of_pattern_component, ON},
  {0, 1, default_duration_of_pattern_component, ON}
};




//catode index, +1%4=red, +2%4=green, +3%4=blue anodes.
int pattern[num_tentacles][pattern_length][num_pattern_component_dimensions] = {
  {
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
  },

  {
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
  }

};



void setup() {
  interpretAndStoreUserDefinedPattern();
  disconnectAllPins();
}

void interpretAndStoreUserDefinedPattern() {
  for (int i = 0; i < num_tentacles; i++) {
    for (int j = 0; j < pattern_length; j++) {
      int user_defined_row = user_defined_pattern[i][j][INDEX_OF_CATHODE_PIN];
      int user_defined_color = user_defined_pattern[i][j][INDEX_OF_ANODE_PIN];
      int user_defined_duration = user_defined_pattern[i][j][INDEX_OF_DURATION];
      int user_defined_instruction = user_defined_pattern[i][j][INDEX_OF_INSTRUCTION];

      int cathode_given_row_tentacle = ROWS[i][user_defined_row];
      int anode_given_row_tentacle_color = COLORS[i][user_defined_row][user_defined_color];

      pattern[i][j][INDEX_OF_INSTRUCTION] = user_defined_instruction;
      pattern[i][j][INDEX_OF_DURATION] = user_defined_duration;
      //in the user defined pattern, I wanted to just write the "OFF" value for each dimension. This conditional
      //just prevents me from writing the value of OFF to the anode and cathode pin indices.
      //
      if (user_defined_instruction == OFF) {
        pattern[i][j][INDEX_OF_CATHODE_PIN] = curr_pattern_component[num_tentacles][INDEX_OF_CATHODE_PIN];
        pattern[i][j][INDEX_OF_ANODE_PIN] = curr_pattern_component[num_tentacles][INDEX_OF_ANODE_PIN];
      }
      else {
        pattern[i][j][INDEX_OF_CATHODE_PIN] = cathode_given_row_tentacle;
        pattern[i][j][INDEX_OF_ANODE_PIN] = anode_given_row_tentacle_color;

      }
    }
  }
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
