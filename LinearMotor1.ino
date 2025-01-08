/* Firgelli Automations
*  Limited or no support: we do not have the resources for Arduino code support
*/

void setup() {
    pinMode(26, OUTPUT); // Configure pin 27 as an Output
    pinMode(27, OUTPUT); // Configure pin 26 as an Output

    digitalWrite(26, HIGH); // Initialize pin 27 as Low
    digitalWrite(27, HIGH); // Initialize pin 27 as Low

}

void loop() {
    // Extend Linear Actuator
    // digitalWrite(27, LOW);
    // digitalWrite(26, HIGH);

    // delay(2000); // 2 seconds

    // // Stops Actuator
    // digitalWrite(27, HIGH);
    // digitalWrite(26, HIGH);

    // delay(2000); // 2 seconds

    // Retracts Linear Actuator
    digitalWrite(26, HIGH);
    digitalWrite(27, LOW);

    delay(2000); // 2 seconds

    // Stop Actuator
    digitalWrite(26, HIGH);
    digitalWrite(27, HIGH);

    delay(2000); // 2 seconds
}