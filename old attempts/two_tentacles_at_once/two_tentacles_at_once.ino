
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

//9, 10,
// create servo objects to control a servo
Servo servo9;   //will go through pin 9, attached to Servo 1
Servo servo10;  //will go through pin 10, attached to Servo 2

// variable to store the servo positions
//int servo9Pos = 0;   //pos of servo9 - servo1
//int servo10Pos = 0;  //pos of servo10 - servo2
int pos = 0;
int degreeOfMovement = 90;

//create motor shield object and dc motor object
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *DCMotor = AFMS.getMotor(1); // getMotor(port#). Port# is which port it is connected to. If you're using M1 its 1, M2 use 2, M3 use 3 and M4 use 4

// variable to store the dc speed
int DCSpeed = 30; //Set the speed of the motor using setSpeed(speed) where the speed ranges from 0 (stopped) to 255 (full speed). You can set the speed whenever you want.



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
  setupMotors();
  Serial.begin(9600);
}


void setupTentacles() {
  for (int i = 0; i < NUM_PINS; i++) {
    pinMode(i, INPUT);

  }

  initializeArmsLEDVariables();
}


void setupMotors() {
  servo9.attach(9);  // attaches the servo on pin 9 to the servo object
  servo10.attach(10); //attaches the servo on pin 10 to the servo object

  AFMS.begin(); //connectto contoller - call'begin()" on the Adafruit_MotorShield object
}


void loop() {

  handleArms();
  //  handleMotors();
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























/* motor stuff*/

long motorPrevMillis;
int MOTOR_DELAY_TIME = 15;
int direction_;
const int FORWARD_ = 0;
const int BACKWARD_ = 1;



bool motorSwitchTimeHasElapsed(int currMillis) {
  return abs(currMillis - motorPrevMillis) >= MOTOR_DELAY_TIME;

}



void handleMotors()
{

  DCMotor->setSpeed(DCSpeed); // assign speed to dc motor, we can dynamically change this later on if we want
  DCMotor->run(FORWARD); //FORWARD, BACKWARD or RELEASE

  long currMillis = millis();

  if (motorSwitchTimeHasElapsed(currMillis)) {
    Serial.println(pos);
    if (direction_ == FORWARD_) {
      servo9.write(pos); // tell servo to go to position in variable 'pos'
      servo10.write(pos);
      pos++;
      if (pos == degreeOfMovement) direction_ = BACKWARD_;

    }
    else {
      servo9.write(pos); // tell servo to go to position in variable 'pos'
      servo10.write(pos);
      pos--;
      if (pos == 0) direction_ = FORWARD_;

    }
    motorPrevMillis = millis();
  }

}


