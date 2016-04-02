
#include <Servo.h> //servo library
#include <Wire.h> //library for dc motor
#include <Adafruit_MotorShield.h> //library for dc motor




const int BROWN = 5;
const int RED = 6;
const int GREEN = 7;
const int BLUE = 8;

const int ORANGE = 4;
const int PURPLE = 3;
const int WHITE = 2;
const int YELLOW = 1;


const int CATHODE = 0;
const int R = 1;
const int G = 3;
const int B = 2;


const int NUM_ARMS = 2;
const int NUM_TENTACLES = 4;
const int NUM_PINS = 8;



int pins[NUM_PINS] = {
  1, 2, 3, 4, 5, 6, 7, 8
};

const int NUM_LED_DATA_FIELDS = 7;
const int CURR_LED_INDEX = 0;
const int CURR_COLOR_INDEX = 1;
const int R_INDEX = 2;
const int G_INDEX = 3;
const int B_INDEX = 4;
const int GROUND_INDEX = 5;
const int PREV_MILLIS_INDEX = 6;

int arm_led_data[NUM_ARMS][NUM_LED_DATA_FIELDS] = {
  {0, 0, 0, 0, 0, 0, 0}, //curr_led, curr_led_color, r, g, b, ground, revMillis
  {0, 0, 0, 0, 0, 0, 0},
};

int leds[NUM_ARMS][NUM_TENTACLES][NUM_PINS] = {

  {
    {
      BROWN, GREEN, BLUE, RED
    }
    ,
    {
      RED, BLUE, BROWN, GREEN
    }
    ,
    {
      BLUE, BROWN, GREEN, RED
    }
    ,
    {
      GREEN, RED, BLUE, BROWN
    }
  },
  { //arm 2
    {
      WHITE, YELLOW, PURPLE, ORANGE
    }
    ,
    {
      ORANGE, WHITE, YELLOW, PURPLE
    }
    ,
    {
      PURPLE, ORANGE, WHITE, YELLOW
    }
    ,
    {
      YELLOW, BROWN, ORANGE, WHITE
    }
  }
};

void setup() {
  setupTentacles();
 
  Serial.begin(9600);
}


void setupTentacles() {
  for (int i = 0; i < NUM_PINS; i++) {
    pinMode(i, INPUT);

  }

  initializeArmsLEDVariables();
}




void loop() {

  handleArms();

}
const int COLOR_SWITCH_TIME = 500;


bool switchTimeHasElapsed(int currMillis, int prevMillis, int interval) {
  return abs(currMillis - prevMillis) >= interval;
}


void initializeArmsLEDVariables() {

  for (int i = 0; i < NUM_ARMS; i++) {
    int* data = arm_led_data[i];
    initializeCurrentLEDVariables(data, leds[i][data[CURR_LED_INDEX]]);
  }
}

void initializeCurrentLEDVariables(int* data, int* curr_led_arr ) {


  data[R_INDEX] = curr_led_arr[R];
  data[G_INDEX] = curr_led_arr[G];
  data[B_INDEX] = curr_led_arr[B];
  data[GROUND_INDEX] = curr_led_arr[CATHODE];
  pinMode(data[GROUND_INDEX], OUTPUT);
  digitalWrite( data[GROUND_INDEX], LOW);


}


void handleArm(int* arm_data, int *curr_led_arr) {
  long currentMillis = millis();
  int curr_led = arm_data[CURR_LED_INDEX];
  int curr_color = arm_data[CURR_COLOR_INDEX];
  int r = arm_data[R_INDEX];
  int g = arm_data[G_INDEX];
  int b = arm_data[B_INDEX];
  int ground = arm_data[GROUND_INDEX];
  int prev_millis = arm_data[PREV_MILLIS_INDEX];

  if (switchTimeHasElapsed(currentMillis, prev_millis, COLOR_SWITCH_TIME)) {
    switch (curr_color) {
      case 0:
        lightUp(r, g, b);
        break;
      case 1:
        lightUp(g, b, r);
        break;
      case 2:
        lightUp(b, g, r);
        closeCurrentLED(ground);
        setNextLED(arm_data);
        initializeCurrentLEDVariables(arm_data,curr_led_arr);
        break;
    }

    arm_data[PREV_MILLIS_INDEX] = millis();
    arm_data[CURR_COLOR_INDEX] = (curr_color < 3 ? curr_color + 1 : 0);
  }

}


void handleArms() {
  for (int i = 0; i < NUM_ARMS; i++) {
    handleArm(arm_led_data[i], leds[i][arm_led_data[i][CURR_LED_INDEX]]);
  }





}



void closeCurrentLED(int ground_pin) {
  pinMode(ground_pin, INPUT);

}

void setNextLED(int* arm_data) {
  int curr_led = arm_data[CURR_LED_INDEX];
  if (curr_led < NUM_TENTACLES) curr_led++;
  else curr_led = 0;
  arm_data[CURR_LED_INDEX] = curr_led;
}


void lightUp(int hi, int disconnectA, int disconnectB) {
  pinMode(hi, OUTPUT);
  pinMode(disconnectA, INPUT);
  pinMode(disconnectB, INPUT);
  digitalWrite(hi, HIGH);
}




















