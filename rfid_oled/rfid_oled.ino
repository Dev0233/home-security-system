#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET    -1 // Reset pin (not used)
#define SCREEN_ADDRESS 0x3C // OLED display address

#define ROWS 4
#define COLS 4

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {32, 33, 25, 26}; // Connect row pins to these Arduino pins.
byte colPins[COLS] = {27, 14, 12, 13}; // Connect column pins to these Arduino pins.

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char *correctPassword = "1234"; // Change this to your desired password
String enteredPassword = "";

const int buzzerPin = 15; // Buzzer pin

void setup() {
  Serial.begin(115200);

  Wire.begin(4, 5); // SDA connected to D4, SCL connected to D5

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;) {}
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("Enter Password:"));
  display.display();

  pinMode(buzzerPin, OUTPUT);

  // Initialize keypad
  for (int row = 0; row < ROWS; row++) {
    pinMode(rowPins[row], OUTPUT);
    digitalWrite(rowPins[row], HIGH);
  }

  for (int col = 0; col < COLS; col++) {
    pinMode(colPins[col], INPUT_PULLUP);
  }
}

void loop() {
  char key = getKey();

  if (key != '\0') {
    if (key ==
