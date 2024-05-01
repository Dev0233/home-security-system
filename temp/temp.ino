const int buttonPin = D0;    // Define the pin for the push button
const int ledPin = LED_BUILTIN; // Define the onboard LED pin
int buttonState = 0;         // Variable for storing the button state

void setup() {
  pinMode(buttonPin, INPUT);   // Set the button pin as input
  pinMode(ledPin, OUTPUT);     // Set the LED pin as output
  Serial.begin(9600);          // Initialize serial communication
}

void loop() {
  buttonState = digitalRead(buttonPin); // Read the state of the button

  if (buttonState == HIGH) {
    Serial.println("Button not pressed!");
    digitalWrite(ledPin, LOW); // Turn off the LED
  }
  else {
    Serial.println("Button pressed!");
    digitalWrite(ledPin, HIGH); // Turn on the LED
  }
  delay(500); // Add a small delay to debounce the button (optional)
}
