int[] delays_between_hr_events = {
  100, 200, 500, 800, 2000, 1000, 800, 300, 100, 80, 3000, 1500, 1000, 80, 200, 100, 30, 3000, 500, 800, 500, 2000
};

int DISPLAY_WIDTH=1000;
int DISPLAY_HEIGHT=800;

int Y_BASELINE=(int)(DISPLAY_HEIGHT/1.2);
int JUMP_DISTANCE=DISPLAY_HEIGHT/6;
int MAX_JTTER_DISTANCE=JUMP_DISTANCE/4;
int MIN_JITTERED_Y=(int)(Y_BASELINE-MAX_JTTER_DISTANCE);
int MAX_JITTERED_Y=(int)(Y_BASELINE+MAX_JTTER_DISTANCE);
int p_x;
int p_y;
int x;
int y=DISPLAY_HEIGHT/2;


int event_tracker=0;
int start_time_of_last_event;


void setup() {
  size(1000, 800);
  background(0);
  stroke(255, 0, 0);
  fill(255, 0, 0);
}




void draw() {
  p_x=x;
  p_y=y;

  x++;

  if (timeToSpike(millis())) {
    spike();
    updateHRTracker();
    recordStartTimeOfNewHREvent();
  } 
  else if(spikedOnLastFrame()){
   returnToBaseline(); 
  }
  else {
    jitterRandomlyBelowEventThreshold();
  }


  if (reachedEdgeOfScreen()) {
    refreshScreen();
  }


  renderPoint();
}

boolean spikedOnLastFrame(){
  return y==BASELINE+JUMP_DISTANCE;
}

void returnToBaseline(){
 y=BASELINE; 
}

void updateHRTracker() {
  event_tracker=(event_tracker+1)%delays_between_hr_events.length;
}

boolean aSecondElapsed() {
  return millis()%1000==0;
}

void recordStartTimeOfNewHREvent() {
  start_time_of_last_event=millis();
}

void refreshScreen() {
  background(0);
  x=0;
  p_x=0;
}

void renderPoint() {
  line(p_x, p_y, x, y);
}


boolean timeToSpike(int curr_millis) {
  return abs(curr_millis-start_time_of_last_event)>delays_between_hr_events[event_tracker];
}

boolean reachedEdgeOfScreen() {
  return x>width;
}

void spike() {
  y=Y_BASELINE-JUMP_DISTANCE;

}

void jitterRandomlyBelowEventThreshold() {
  y=constrain(y+(int)random(-JUMP_DISTANCE/4, JUMP_DISTANCE/4), MIN_JITTERED_Y, MAX_JITTERED_Y);
}