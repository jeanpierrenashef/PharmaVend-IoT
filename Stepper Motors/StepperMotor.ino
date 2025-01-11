// Define the connection pins
const int DIR_PIN = 15; // Direction pin
const int STEP_PIN = 2; // Step pin
const int ENABLE_PIN = 13; // Enable pin
const int stepsPerRevolution = 400; // Number of steps per full revolution

void setup() {
  // Initialize the serial communication
  Serial.begin(115200);
  
  // Set the motor control pins as outputs
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT); // Set the enable pin as output

  // Initially disable the motor
  digitalWrite(ENABLE_PIN, HIGH); // Assuming LOW will disable the motor
}

void loop() {
  // Enable the motor
  digitalWrite(ENABLE_PIN, LOW); // Assuming HIGH enables the motor

  // Record start time to manage the 15-second run
  unsigned long startTime = millis();

  // Rotate clockwise
  digitalWrite(DIR_PIN, HIGH);
  Serial.println("Spinning Clockwise...");
  while(millis() - startTime < 15000) { // Run motor for 15 seconds
    for (int i = 0; i < stepsPerRevolution; i++) {
      if (millis() - startTime >= 15000) break; // Break loop if 15 seconds are up
      makeStep(2000); // 2000 microseconds step delay
    }
  }

  // Disable the motor after 15 seconds
  digitalWrite(ENABLE_PIN, HIGH);

  // Wait for a bit before the next loop iteration
  delay(5000); 
}

// Function to perform a single step
void makeStep(int stepDelay) {
  digitalWrite(STEP_PIN, HIGH); // Turn on the step pin
  delayMicroseconds(stepDelay); // Hold the step pin high for 'stepDelay' microseconds
  digitalWrite(STEP_PIN, LOW); // Turn off the step pin
  delayMicroseconds(stepDelay); // Hold the step pin low for 'stepDelay' microseconds
}
