
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

const int NUM_TENTACLES = 8;
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


int leds[NUM_TENTACLES][NUM_PINS] = {
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
  },

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

  initializeCurrentLEDVariables();
}


void setupMotors() {
  servo9.attach(9);  // attaches the servo on pin 9 to the servo object
  servo10.attach(10); //attaches the servo on pin 10 to the servo object

  AFMS.begin(); //connectto contoller - call'begin()" on the Adafruit_MotorShield object
}


void loop() {

  handleTentacles();
//  handleMotors();


}




int curr_led = 0;
int curr_color = 0;
int*  curr_led_arr;
int ground;
int r, g, b;
const int COLOR_SWITCH_TIME = 1000;
long colorPrevMillis;

bool switchTimeHasElapsed(int currMillis, int prevMillis, int interval) {
  return abs(currMillis - prevMillis) >= interval;
}



void initializeCurrentLEDVariables() {
  curr_led_arr = leds[curr_led];
  ground = curr_led_arr[CATHODE];
  r = curr_led_arr[R];
  g = curr_led_arr[G];
  b = curr_led_arr[B];
  pinMode(ground, OUTPUT);
  digitalWrite(ground, LOW);
}



void handleTentacles() {

  long currentMillis = millis();

  if (switchTimeHasElapsed(currentMillis, colorPrevMillis, COLOR_SWITCH_TIME)) {
    switch (curr_color) {
      case 0:
        lightUp(r, g, b);
        break;
      case 1:
        lightUp(g, b, r);
        break;
      case 2:
        lightUp(b, g, r);
        closeCurrentLED();
        setNextLED();
        initializeCurrentLEDVariables();
        break;
    }

    colorPrevMillis = millis();
    curr_color = (curr_color < 3 ? curr_color + 1 : 0);
  }
}



void closeCurrentLED() {
  pinMode(ground, INPUT);

}

void setNextLED() {
  if (curr_led < NUM_TENTACLES) curr_led++;
  else curr_led = 0;
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


