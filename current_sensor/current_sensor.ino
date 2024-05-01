// Include the ESP32 library
#include <Arduino.h>

// Define the pin connected to the relay
const int relayPin = 21; // D21

// Define the pins connected to the ACS712 and voltage sensor
const int acsPin = 36; // Analog pin for ACS712
const int voltagePin = 34; // Analog pin for voltage sensor

void setup() {
  // Set the relay pin as an output
  pinMode(relayPin, OUTPUT);

  // Initialize serial communication for debugging purposes
  Serial.begin(115200);
}

void loop() {
  // Read the analog voltage from ACS712
  int sensorValueACS = analogRead(acsPin);
  
  // Convert the analog value to current (adjust sensitivity according to your ACS712 model)
  float voltageACS = sensorValueACS * (5.0 / 1023.0);
  float current = voltageACS / 0.185; // Sensitivity for ACS712 model (185mV/A)

  // Read the analog voltage from the voltage sensor
  int sensorValueVoltage = analogRead(voltagePin);
  
  // Convert the analog value to voltage (adjust based on your voltage sensor characteristics)
  float voltage = sensorValueVoltage * (5.0 / 1023.0) * 2; // Assuming a voltage divider circuit

  // Print current and voltage values to serial monitor in CSV format for each graph
  Serial.print(current); // Print current value
  Serial.print(","); // Separate values with a comma
  Serial.println(voltage); // Print voltage value

  // Turn on the relay (this may depend on the relay module, some may be active LOW)
  digitalWrite(relayPin, HIGH);
  delay(1000); // Wait for 1 second

  // Turn off the relay
  digitalWrite(relayPin, LOW);
  delay(1000); // Wait for 1 second
}
