// defines pins numbers
const int stepPin = 2;
const int dirPin = 15;
const int enablePin = 13; // Choose an appropriate GPIO pin for enable

void setup() {
  // Sets the two pins as Outputs
  pinMode(stepPin, OUTPUT); 
  pinMode(dirPin, OUTPUT);
  pinMode(enablePin, OUTPUT); // Initialize the enable pin as output

  // Initialize the enable pin to HIGH to disable the stepper initially
  digitalWrite(enablePin, HIGH);
}

void loop() {
  digitalWrite(enablePin, LOW); // Enable the stepper motor
  digitalWrite(dirPin, HIGH); // Sets the motor to move in a particular direction

  // Start a timer
  unsigned long startTime = millis();

  // Run the motor for approximately 5 seconds
  while(millis() - startTime < 5000) { // 5000 ms = 5 seconds
    digitalWrite(stepPin, HIGH); 
    delayMicroseconds(500); 
    digitalWrite(stepPin, LOW); 
    delayMicroseconds(500); 
  }

  digitalWrite(enablePin, HIGH); // Disable the stepper motor after 5 seconds

  // Optional: add a long delay or stop further executions if this is a one-time operation
  while(true) {
    delay(1000); // Keep looping indefinitely, effectively stopping further executions
  }
}
