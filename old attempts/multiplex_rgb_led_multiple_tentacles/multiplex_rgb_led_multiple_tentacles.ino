
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
const int NUM_PINS = NUM_TENTACLES;

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
  1,2,3,4,5,6,7,8
};


int leds[2][NUM_TENTACLES][NUM_PINS] = {
  { //first tentacle
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

  { //second tentacle

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
}


void setupTentacles() {
  for (int i = 0; i < NUM_PINS; i++) {
    pinMode(i, INPUT);

  }
}


void setupMotors() {
  servo9.attach(9);  // attaches the servo on pin 9 to the servo object
  servo10.attach(10); //attaches the servo on pin 10 to the servo object

  AFMS.begin(); //connectto contoller - call'begin()" on the Adafruit_MotorShield object
}


void loop() {

  handleTentacles();
  //handleMotors();


}




int curr_led = 0;
int colorSwitchTimer = 500;
const int COLOR_SWITCH_TIME = 500;
int color_counter = 0;

void handleTentacles() {
  int* curr_led_arr = leds[curr_led];
  int ground = curr_led_arr[CATHODE];
  pinMode(ground, OUTPUT);
  digitalWrite(ground, LOW);
  //cycle through all colours
  int r = curr_led_arr[R];
  int g = curr_led_arr[G];
  int b = curr_led_arr[B];


  pinMode(r, OUTPUT);
  pinMode(g, INPUT);
  pinMode(b, INPUT);
  digitalWrite(r, HIGH);
  delay(500);
  pinMode(r, INPUT);
  pinMode(g, OUTPUT);
  digitalWrite(g, HIGH);
  delay(500);
  pinMode(g, INPUT);
  pinMode(b, OUTPUT);
  digitalWrite(b, HIGH);
  delay(500);
  pinMode(b, OUTPUT);
  pinMode(ground, INPUT);

  if (curr_led < NUM_TENTACLES - 1) curr_led++;
  else curr_led = 0;

}


void lightUp(int hi, int disconnectA, int disconnectB) {
  pinMode(hi, OUTPUT);
  pinMode(disconnectA, INPUT);
  pinMode(disconnectB, INPUT);
  digitalWrite(hi, HIGH);
}



void handleMotors()
{

  DCMotor->setSpeed(DCSpeed); // assign speed to dc motor, we can dynamically change this later on if we want
  DCMotor->run(FORWARD); //FORWARD, BACKWARD or RELEASE

  for (pos = 0; pos < degreeOfMovement; pos += 1) // goes from 0 degrees to 180 degrees, in steps of 1 degree
  {
    //servo9Pos, servo10Pos = pos;
    servo9.write(pos); // tell servo to go to position in variable 'pos'
    servo10.write(pos);
    delay(15); // waits 15ms for the servo to reach the position
  }
  for (pos = degreeOfMovement; pos >= 1; pos -= 1) // goes from 180 degrees to 0 degrees
  {
    //servo9Pos, servo10Pos = pos;
    servo9.write(pos); // tell servo to go to position in variable 'pos'
    servo10.write(pos);
    delay(15); // waits 15ms for the servo to reach the position
  }
}


