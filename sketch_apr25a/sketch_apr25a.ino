#include <Keypad.h>

// Define the pins for the keypad rows and columns
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[COLS] = {D1, D2, D3, D4}; // Connect to the row pinouts of the keypad
byte colPins[ROWS] = {D5, D6, D7, D8}; // Connect to the column pinouts of the keypad

// Define the pin for the relay
const int relayPin = D0;

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(9600);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH); // Initialize relay to be off
}

void loop() {
  char key = keypad.getKey();
  
  if (key) {
    Serial.println(key);
    if (key == '#') {
      // Turn off the relay
      digitalWrite(relayPin, LOW);
    } else if (key == '*') {
      // Turn on the relay
      digitalWrite(relayPin, HIGH);
    }
  }
}
