#define BLYNK_TEMPLATE_ID "TMPL3f9QfXbVy"
#define BLYNK_TEMPLATE_NAME "otp"
#include <WiFi.h> // Change to WiFi.h for ESP32
#include <BlynkSimpleEsp32.h> // Change to BlynkSimpleEsp32.h for ESP32
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>
#define RST_PIN     22         // Reset pin for RFID module
#define RELAY_PIN 2
#define SS_PIN      21         // Slave Select pin for RFID module
#define BUZZER_PIN  15         // Define the pin for the buzzer
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_ADDRESS 0x3C      // OLED display address
#define SDA_PIN 4              // SDA pin for OLED display
#define SCL_PIN 5              // SCL pin for OLED display
#define OLED_RESET    -1
#include <Arduino.h>
#include <Adafruit_Fingerprint.h>

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {32, 33, 25, 26}; // Connect to the row pinouts of the keypad
byte colPins[COLS] = {27, 14, 12, 13}; // Connect to the column pinouts of the keypad

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); // Create SSD1306 display instance
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
HardwareSerial serialPort(2); // use UART2
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&serialPort);

String pass = "";
int otpCode= 0;
bool passEntered = false;
int attempts = 0;
char auth[] = "5wKys8Z0hCEyyOa_Inw_YUiSNX5B0r3L"; // Blynk authentication token

const char* ssid = "Vivo Y20 T";
const char* password = "Amanbhai";

int generateOTP() {
  return random(1000, 9999); // Generate a random 4-digit OTP
}

enum SecurityState {
  PASSWORD,
  OTP,
  RFID,
  FINGERPRINT,
  UNLOCKED
};

// Initialize the current state to PASSWORD
SecurityState currentState = PASSWORD;

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN,HIGH);
  Serial.begin(115200);
  SPI.begin();          // Initialize SPI bus
  mfrc522.PCD_Init();   // Initialize MFRC522 RFID reader
  finger.begin(57600);

  // Initialize OLED display
  Wire.begin(SDA_PIN, SCL_PIN);
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  display.println(F("RFID & OLED"));
  display.println(F("Initializing..."));
  display.display();
  delay(2000);
  display.clearDisplay();

  Blynk.begin(auth, ssid, password);

  // Generate OTP once
  otpCode = generateOTP();
  Blynk.virtualWrite(V1, otpCode); // Send OTP to virtual pin V0 on the Blynk server for ESP32
  Serial.println(otpCode);

  // Wait for Blynk to connect
  while (Blynk.connect() == false) {
    // Wait until Blynk connection is established
  }
}

void loop() {
  Blynk.run();
  switch (currentState) {
    case PASSWORD:
      enterPassword();
      break;
    case OTP:
      sendOTP();
      break;
    case RFID:
      enterRFID();
      break;
    case FINGERPRINT:
      enterFingerprint();
      break;
    case UNLOCKED:
      // Nothing to do in this state, the solenoid is already unlocked
      break;
  }
}

void enterFingerprint() {  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Place your finger:");
  display.display();
  while (finger.getImage() != FINGERPRINT_OK) {
    delay(100); // Add a small delay to avoid overwhelming the sensor
  }
  int fingerprintID = getFingerprintIDez();
  if (fingerprintID == 2) {
    digitalWrite(RELAY_PIN, LOW); // Unlock if fingerprint ID is 2
    currentState = UNLOCKED;
  } else {
    currentState = PASSWORD; // Go back to password entry if fingerprint doesn't match
  }
}

int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)
    return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)
    return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)
    return -1;

  return finger.fingerID;
}

void enterPassword() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Enter Password:");
  display.println(pass);
  display.display();
  char key = keypad.getKey();
  if (key != NO_KEY) {
    tone(BUZZER_PIN, 1000, 100);
    
    if (key == 'C' && pass.length() > 0) {
      pass.remove(pass.length() - 1); // Remove the last character from the password
    } 
    else if (key != 'D') {
      pass += key;
    } 
    

    
    if (key == 'D') {
      passEntered = true;
      verifyPassword();
    }
  }
}

void verifyPassword() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Verifying...");
  display.display();
  delay(2000);
  
  if (pass == "1234") {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
    display.println("Password Verified");
    currentState = RFID;
    Serial.println(currentState);
    display.display();

    
  } else {
    attempts++;
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
    display.println("Password Incorrect");
    display.display();
    
    if (attempts >= 3) {
      tone(BUZZER_PIN, 1000, 1000);
      delay(2000); // Delay for alarm sound
      resetESP(); // Reset the ESP32
    }
  }
  pass = "";
  passEntered = false;
}

void enterRFID() {  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Scan RFID Card:");
  display.display();
  // Look for new cards
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // Show UID on serial monitor
    Serial.print(F("UID tag :"));
    String content = "";
    byte letter;
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
      content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
      content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    Serial.println();
    Serial.print(F("Message : "));
    content.toUpperCase();
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println(F("UID:"));
    display.println(content);
    display.display();
    delay(1000);
    
    // Verify RFID tag
    if (content.substring(1) == "B8 61 9C 12") { // Change to the desired RFID tag UID
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0,0);
      display.println("Authorized access");
      display.display();
      
      // Perform further actions or unlock mechanism here
      currentState = OTP;
    } else {
      Serial.println("Access denied");
    }
    delay(3000);
  }
}

void enterOTP() {
  char key;
  static int enteredOTP = 0; // Static variable to store the entered OTP
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Enter OTP:");
  display.display();

  while (true) {
    key = keypad.getKey();
    
    if (key != NO_KEY && key != 'D') {
      tone(BUZZER_PIN, 1000, 100);
      
      if (key == 'C' && enteredOTP > 0) {
        enteredOTP /= 10; // Remove the last digit from the entered OTP
      } 
      else if (isDigit(key)) {
        enteredOTP = enteredOTP * 10 + (key - '0'); // Append the entered digit to the OTP
      }
      
      display.setCursor(0, 10);
      display.print(enteredOTP);
      display.display();
    } else if (key == 'D') {
      tone(BUZZER_PIN, 1000, 100);
      verifyOTP(enteredOTP); // Verify the entered OTP
      enteredOTP = 0; // Reset entered OTP for next entry// Exit the loop after verifying OTP
    }
  }
}


void verifyOTP(int enteredOTP) {
  Serial.println(enteredOTP); 
  Serial.println(otpCode);  // Print entered OTP
  
  // Rest of the function remains unchanged
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Verifying OTP...");
  display.display();
  delay(2000);
  
  if (enteredOTP == otpCode) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
    display.println("OTP Verified");
    display.display();
    delay(2000);
    enterFingerprint();
    
  } else {
    attempts++;
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
    display.println("OTP Incorrect");
    display.display();
    
    if (attempts >= 3) {
      tone(BUZZER_PIN, 1000, 1000);
      delay(2000);
      resetESP();
    }
  }
}


void sendOTP() {  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Sending OTP..");
  display.display();
  delay(2000);
  display.println("Check Blynk App");
  display.display();
  delay(2000);
  enterOTP();
}



void resetESP() {
  ESP.restart(); // Reset the ESP32
}
