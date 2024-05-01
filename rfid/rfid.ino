#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10  // Arduino pin connected to MFRC522's SDA/SS pin
#define RST_PIN 9  // Arduino pin connected to MFRC522's RST pin

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

void setup() {
  Serial.begin(9600);  // Initialize serial communication
  
  SPI.begin();  // Initialize SPI bus
  mfrc522.PCD_Init();  // Initialize MFRC522
  
  Serial.println("RFID Reader Initialized");
}

void loop() {
  // Look for new cards
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // Show UID on serial monitor
    Serial.print("UID tag :");
    String content= "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
      content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
      content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    Serial.println();
    Serial.print("Message : ");
    content.toUpperCase();
    if (content.substring(1) == "YOUR_RFID_TAG") {
      Serial.println("Authorized access");
      // Add your authorized access logic here
    } else {
      Serial.println("Unauthorized access");
      // Add your unauthorized access logic here
    }
    delay(1000);  // Delay before reading the next card
  }
}
