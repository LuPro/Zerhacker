
/*
  overflo 7.2016


  zerhacker is free software, use as you please.
  if you find it useful consider buying something from my store.

  https://hackerspaceshop.com

*/

#include <Stepper.h>

#define STEPS_FOR_PART 6  //With 6 steps, the strip moves ~3,3mm. Because of this the strip would drift, there has to be some logic for it to know how much steps do do.
#define MM_PER_STEP 0.549778714

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
// for your motor

// initialize the stepper library on pins 8 through 11:
Stepper stripstepper(stepsPerRevolution, 2, 3, 4, 5);
Stepper zerhacker(stepsPerRevolution, 6, 7, 8, 9);

void setup() {
  // set the speed at 60 rpm:
  stripstepper.setSpeed(30);
  zerhacker.setSpeed(30);
  // initialize the serial port:
  Serial.begin(9600);
}

void cut () {
  zerhacker.step(10);
  delay(100);
  zerhacker.step(-10);
}

//Gets number of parts to be cut, and calculates how many steps should be done
void pullStrip (unsigned int nr_parts) {
  unsigned int mm_to_pull = nr_parts * 3;
  float mm_delta_1 = 0, mm_delta_2 = 0;
  unsigned int nr_steps;      //
  unsigned int calculated_nr_steps = 0;
  static float old_delta = 0;

  nr_steps = mm_to_pull / MM_PER_STEP;
  mm_delta_1 = mm_to_pull - (nr_steps * MM_PER_STEP) + old_delta;
  mm_delta_2 = mm_to_pull - ((nr_steps + 1) * MM_PER_STEP) + old_delta;

  if (mm_delta_1 > mm_delta_2 * (-1)) {
    calculated_nr_steps = nr_steps + 1;
    old_delta = mm_delta_2;
    Serial.println("Delta 2");
  } else {
    calculated_nr_steps = nr_steps;
    old_delta = mm_delta_1;
    Serial.println("Delta 1");
  }

  /*
  Serial.print("D wo old 1/2: "); Serial.print(mm_to_pull - (nr_steps * MM_PER_STEP)); Serial.print(" / "); Serial.println(mm_to_pull - ((nr_steps + 1) * MM_PER_STEP));
  Serial.print("mm_delta 1/2: "); Serial.print(mm_delta_1); Serial.print(" / "); Serial.println(mm_delta_2);
  Serial.print("nr_parts: "); Serial.println(nr_parts);
  Serial.print("calculated nr: "); Serial.println(calculated_nr_steps);
  Serial.print("Old delta: "); Serial.println(old_delta);
  Serial.println();
  */

  stripstepper.step(calculated_nr_steps);
}

void loop() {
  //TODO 10x 6 vor dann 1x5 vor dann wieder 10x 6 ..
  pullStrip (10);
  delay(100);
  cut();
}
