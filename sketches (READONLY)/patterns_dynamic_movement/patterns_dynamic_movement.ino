const int TENTACLE_A = 0;
const int TENTACLE_B = 1;

const int RED = 0;
const int GREEN = 1;
const int BLUE = 2;

const int FIRST = 0;
const int SECOND = 1;
const int THIRD = 2;
const int FOURTH = 3;

const int OFF = -12;
const int ON = 14;

const int num_tentacles = 2;
const int num_pins_per_tentacle = 4;
const int num_within_pattern_variants = 2;
const int num_pattern_component_dimensions = 3;

const int pattern_length = 8;


/*Sample patterns*/
const int NUM_PATTERNS=3;
const int ZIG_ZAG = 0;
const int LAP = 1;
const int MANHATTAN_WALK = 2;

int pattern_to_use = LAP; //infants-->sensitive to changes in patterns of movement. prefer to look at coherent rather than incoherent (random) motion patterns
int color_to_use = RED; //infants-->sensitive to colour
int speed_to_use = 100; //infants--> sensitive to motion velocity

int ZIG_ZAG_PATTERN[num_tentacles][pattern_length][num_within_pattern_variants] = {
  { //row | color | duration | on or off. (don't need to enter a meaningful row or colour if you want a section to be off, but do need to enter a duration)
    {FIRST, ON},
    {OFF, OFF},
    {THIRD, ON},
    {OFF, OFF},

    {FOURTH, ON},
    {OFF, OFF},
    {SECOND, ON},
    {OFF, OFF}
  },
  {
    {OFF, OFF},
    {SECOND, ON},
    {OFF, OFF},
    {FOURTH, ON},

    {OFF, OFF},
    {THIRD, ON},
    {OFF, OFF},
    {FIRST, ON}
  }
};


int LAP_PATTERN[num_tentacles][pattern_length][num_within_pattern_variants] = {
  { //row | color | duration | on or off. (don't need to enter a meaningful row or colour if you want a section to be off, but do need to enter a duration)
    {FIRST, ON},
    {SECOND, ON},
    {THIRD, ON},
    {FOURTH, ON},

    {FOURTH, OFF},
    {OFF, OFF},
    {SECOND, OFF},
    {OFF, OFF}
  },
  {
    {OFF, OFF},
    {SECOND, OFF},
    {OFF, OFF},
    {FOURTH, OFF},

    {FOURTH, ON},
    {THIRD, ON},
    {SECOND, ON},
    {FIRST, ON}
  }
};



int MANHATTAN_WALK_PATTERN[num_tentacles][pattern_length][num_within_pattern_variants] = {
  { //row | color | duration | on or off. (don't need to enter a meaningful row or colour if you want a section to be off, but do need to enter a duration)
    {FIRST, ON},
    {SECOND, ON},
    {THIRD, OFF},
    {FOURTH, OFF},

    {THIRD, ON},
    {FOURTH, ON},
    {SECOND, OFF},
    {THIRD, OFF}
  },
  {
    {OFF, OFF},
    {OFF, OFF},
    {SECOND, ON},
    {THIRD, ON},

    {FOURTH, OFF},
    {FOURTH, OFF},
    {FOURTH, ON},
    {FOURTH, ON}
  }



};


//indices of cathodes for each row.
int CATHODES_PER_ROW[num_tentacles][num_pins_per_tentacle] = {
  {2, 3, 1, 0},
  {3, 0, 1, 2}
};

//indices of anodes (R,G,B) in each row. (Notice:
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




int pins[num_tentacles][num_pins_per_tentacle] = {
  {1, 2, 3, 5},
  {8, 9, 10, 11}

};

int duration_of_pattern_component[num_tentacles] = {0, 0};
unsigned long start_time_of_pattern_component[num_tentacles] = {0, 0};

const int INDEX_OF_USER_DEFINED_CATHODE_PIN = 0;
const int INDEX_OF_USER_DEFINED_INSTRUCTION = 1;

const int INDEX_OF_CATHODE_PIN = 0;
const int INDEX_OF_ROW = 1;
const int INDEX_OF_INSTRUCTION = 2;


int index_of_current_pattern_component[num_tentacles] = {0, 0};
int curr_pattern_component[num_tentacles][num_pattern_component_dimensions] = {
  {0, 1, ON},
  {0, 1, ON}
};




//catode index, +1%4=red, +2%4=green, +3%4=blue anodes.
int pattern[num_tentacles][pattern_length][num_pattern_component_dimensions] = {
  {
    {0, 0, OFF},
    {0, 0, OFF},
    {0, 0, OFF},
    {0, 0, OFF},
    {0, 0, OFF},
    {0, 0, OFF},
    {0, 0, OFF},
    {0, 0, OFF}
  },

  {
    {0, 0, OFF},
    {0, 0, OFF},
    {0, 0, OFF},
    {0, 0, OFF},
    {0, 0, OFF},
    {0, 0, OFF},
    {0, 0, OFF},
    {0, 0, OFF}
  }

};



void setup() {
storeNewPatternValues();
}


void switchPattern(int new_pattern){
  pattern_to_use=new_pattern;
  storeNewPatternValues();
  delay(60);
  
}

void storeNewPatternValues(){
   switch (pattern_to_use) {
    case ZIG_ZAG:
      setPattern(ZIG_ZAG_PATTERN);
      break;
    case LAP:
      setPattern(LAP_PATTERN);
      break;
    case MANHATTAN_WALK:
      setPattern(MANHATTAN_WALK_PATTERN);
      break;
  }
  disconnectAllPins();
}



void setColor(int newColor) {
  if (newColor == RED || newColor == BLUE || newColor == GREEN) {
    color_to_use = constrain(newColor, RED, BLUE);
  }
}



void setSpeed(int newSpeed) {
  speed_to_use = newSpeed;
}

void setPattern(int (*user_defined_pattern)[pattern_length][num_within_pattern_variants]) {
  for (int i = 0; i < num_tentacles; i++) {
    for (int j = 0; j < pattern_length; j++) {
      int user_defined_row = user_defined_pattern[i][j][INDEX_OF_USER_DEFINED_CATHODE_PIN];
      int user_defined_instruction = user_defined_pattern[i][j][INDEX_OF_USER_DEFINED_INSTRUCTION];
      int cathode_given_row_tentacle = CATHODES_PER_ROW[i][user_defined_row];

      pattern[i][j][INDEX_OF_INSTRUCTION] = user_defined_instruction;
      //in the user defined pattern, I wanted to just write the "OFF" value for each dimension. This conditional
      //just prevents me from writing the value of OFF to the anode and cathode pin indices.
      //
      if (user_defined_instruction == OFF) {
        pattern[i][j][INDEX_OF_CATHODE_PIN] = curr_pattern_component[num_tentacles][INDEX_OF_CATHODE_PIN];
        pattern[i][j][INDEX_OF_ROW] = curr_pattern_component[num_tentacles][INDEX_OF_ROW];
      }
      else {
        pattern[i][j][INDEX_OF_CATHODE_PIN] = cathode_given_row_tentacle;
        pattern[i][j][INDEX_OF_ROW] = user_defined_row;

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




int c=1000;
void loop() {
  for (int i = 0; i < num_tentacles; i++) {
    updateTentaclePatternComponentIfNecessary(i);
  }

if(millis()%c==0){
  switchPattern(random(200)%NUM_PATTERNS);
}


  
}


bool timeToSwitchPatternComponent(unsigned long curr_time, int index_of_tentacle) {
  unsigned long start_time_of_pattern_component_of_this_tentacle_current_pattern_component = start_time_of_pattern_component[index_of_tentacle];
  return abs(curr_time - start_time_of_pattern_component_of_this_tentacle_current_pattern_component) >= speed_to_use;
}


int getIndexOfCathodePin(int index_of_tentacle) {
  int index_of_pin = curr_pattern_component[index_of_tentacle][INDEX_OF_CATHODE_PIN];
  return index_of_pin;
}

int getActualPinValue(int index_of_tentacle, int index_of_pin) {
  int actual_pin_value = pins[index_of_tentacle][index_of_pin];
  return actual_pin_value;
}

void disconnectCurrentAnode(int index_of_tentacle) {
  int index_of_anode = COLORS[index_of_tentacle][curr_pattern_component[index_of_tentacle][INDEX_OF_ROW]][color_to_use];
  int pin = getActualPinValue(index_of_tentacle, index_of_anode);
  disconnect(pin);
}


void disconnectCurrentCathode(int index_of_tentacle) {
  int index_of_cathode_pin = getIndexOfCathodePin(index_of_tentacle);
  int pin = getActualPinValue(index_of_tentacle, index_of_cathode_pin);
  disconnect(pin);
}



void setupNextPatternComponent(int index_of_tentacle) {
  setIndexOfNextPatternComponent(index_of_tentacle);
  setNextPatternComponent(index_of_tentacle);


  if (on(index_of_tentacle)) {
    groundCathodeOfCurrentPatternComponent(index_of_tentacle);
    lightupAnodeOfCurrentPatternCathode(index_of_tentacle);
  }

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
  int index_of_cathode_pin = getIndexOfCathodePin(index_of_tentacle);
  int pin = getActualPinValue(index_of_tentacle, index_of_cathode_pin);
  makeGround(pin);
}


void lightupAnodeOfCurrentPatternCathode(int index_of_tentacle) {
  int index_of_anode = COLORS[index_of_tentacle][curr_pattern_component[index_of_tentacle][INDEX_OF_ROW]][color_to_use];
  int pin = getActualPinValue(index_of_tentacle, index_of_anode);
  lightUp(pin);
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
