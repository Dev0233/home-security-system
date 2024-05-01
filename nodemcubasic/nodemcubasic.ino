#include <Keypad.h>

const byte ROWS = 4; // Number of rows in the keypad
const byte COLS = 4; // Number of columns in the keypad

// Define the symbols on the buttons of the keypad
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Connect keypad rows and columns to the corresponding pins
byte rowPins[ROWS] = {D0, D1, D2, D3};
byte colPins[COLS] = {D5, D6, D7, D8};

// Create the Keypad object
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(9600);
}

void loop() {
  // Get the key pressed on the keypad
  char key = keypad.getKey();
  
  // If a key is pressed, print it to the serial monitor
  if (key) {
    Serial.println(key);
  }
}
