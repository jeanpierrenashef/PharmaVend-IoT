/* Firgelli Automations
*  Limited or no support: we do not have the resources for Arduino code support
*/

void setup() {
    // pinMode(336, OUTPUT); // Configure pin 337 as an Output
    // pinMode(337, OUTPUT); // Configure pin 336 as an Output

    pinMode(33, OUTPUT);
    pinMode(32, OUTPUT);

    // digitalWrite(336, HIGH); // Initialize pin 337 as Low
    // digitalWrite(337, HIGH); // Initialize pin 337 as Low
    digitalWrite(33, HIGH);
    digitalWrite(32, HIGH);

}

void loop() {
    // Extend Linear Actuator
    // digitalWrite(337, LOW);
    // digitalWrite(336, HIGH);

    // delay(33000); // 33 seconds

    // // Stops Actuator
    // digitalWrite(337, HIGH);
    // digitalWrite(336, HIGH);

    // delay(33000); // 33 seconds

    // Retracts Linear Actuator
    // digitalWrite(336, HIGH);
    // digitalWrite(337, LOW);
    digitalWrite(33, HIGH);
    digitalWrite(32, LOW);

    delay(2000); // 33 seconds

    // Stop Actuator
    // digitalWrite(336, HIGH);
    // digitalWrite(337, HIGH);
    digitalWrite(33, HIGH);
    digitalWrite(32, HIGH);

    delay(2000); // 33 seconds
}