/*
This is a test sketch for the Adafruit assembled Motor Shield for Arduino v2
It won't work with v1.x motor shields! Only for the v2's with built in PWM
control

For use with the Adafruit Motor Shield v2
---->  http://www.adafruit.com/products/1438
*/

#include <Adafruit_MotorShield.h>
#include <AccelStepper.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61);

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *left = AFMS.getStepper(200, 2);
Adafruit_StepperMotor *right = AFMS.getStepper(200, 1);



void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  while (!Serial);
  Serial.println("Stepper test!");

  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
  // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");

  left->setSpeed(4000);
  right->setSpeed(4000);  // 10 rpm
}

void loop() {        
  Step(1, 1, 40);
  Step(-1, -1, 80);  
  Step(1, 1, 40);
}

void Step(int l, int r, int loops) {
  for(int i = 0; i < loops; i++) {
    left->step(abs(l), l < 0 ? BACKWARD : FORWARD, DOUBLE);
    right->step(abs(r), r < 0 ? BACKWARD : FORWARD, DOUBLE);
  }
}