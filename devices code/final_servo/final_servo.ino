/* Example sketch to control a stepper motor with A4988 stepper motor driver and Arduino without a library. */

// Define stepper motor connections and steps per revolution:
#define dirPin 13
#define stepPin 12
#define stepsPerRevolution 1000
#define en 9

void setup() {
  // Declare pins as output:
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(en, OUTPUT);
  digitalWrite(en, LOW); // Activate with LOW
}

void loop() {
  // Move to one side (clockwise):

  
  digitalWrite(dirPin, HIGH);
  for (int i = 0; i < stepsPerRevolution; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000);
  }

  delay(1000);

  // Move back (counterclockwise):
  digitalWrite(dirPin, LOW);
  for (int i = 0; i < stepsPerRevolution; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000);
  }

  delay(1000);
}
