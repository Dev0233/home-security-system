const int relayPin = D0; // Digital pin connected to relay

void setup() {
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW); // Make sure relay is initially off
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    char input = Serial.read(); // Read the input from the serial monitor
    if (input == '1') {
      Serial.println("Turning relay ON");
      digitalWrite(relayPin, HIGH); // Turn the relay ON
    } else if (input == '0') {
      Serial.println("Turning relay OFF");
      digitalWrite(relayPin, LOW); // Turn the relay OFF
    } else {
      Serial.println("Invalid input. Enter '1' to turn the relay ON or '0' to turn it OFF.");
    }
  }delay
}
