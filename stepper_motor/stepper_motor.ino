#include <Stepper.h>
const int one_circle_counter_clockwise = 2048;
const int one_circle_clockwise = -one_circle_counter_clockwise;

Stepper myStepper = Stepper(one_circle_counter_clockwise, 8, 10, 9, 11);

void setup() {
  myStepper.setSpeed(5);
  Serial.begin(9600);
}
int do_only_one_time = 0;
void loop() {
  if (do_only_one_time == 0){
    myStepper.step(one_circle_clockwise);
    do_only_one_time = 1;
  //delay(500);
  }
  
  //myStepper.step(one_circle_clockwise);
}
