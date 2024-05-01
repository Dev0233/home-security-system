#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET    -1 // Reset pin (not used)
#define OLED_SDA 4 // OLED SDA pin
#define OLED_SCL 5 // OLED SCL pin
#define SCREEN_ADDRESS 0x3C // OLED display address

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET, OLED_SDA, OLED_SCL);

const char *correctPassword = "1234"; // Change this to your desired password
String enteredPassword = "";

const int buzzerPin = 15; // Buzzer pin

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {32, 33, 25, 26}; // Connect row pins to these Arduino pins.
byte colPins[COLS] = {27, 14, 12, 13}; // Connect column pins to these Arduino pins.

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup() {
  Serial.begin(115200);

  Wire.begin();

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
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    if (key == '#') {
      checkPassword();
      enteredPassword = ""; // Reset entered password
    } else {
      enteredPassword += key;
    }
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println(F("Enter Password:"));
    display.println(enteredPassword);
    display.display();
  }
}

void checkPassword() {
  if (enteredPassword == correctPassword) {
    // Correct password
    digitalWrite(buzzerPin, HIGH);
    delay(500);
    digitalWrite(buzzerPin, LOW);
  } else {
    // Incorrect password
    // Do something here (e.g., display message on OLED)
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println(F("Incorrect Password!"));
    display.display();
    delay(2000);
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println(F("Enter Password:"));
    display.display();
  }
}
