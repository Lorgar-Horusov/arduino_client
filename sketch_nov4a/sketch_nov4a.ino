#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <deprecated.h>
#include <require_cpp11.h>


#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above

#define SCK_PIN 5
#define SO_PIN 4
#define SI_PIN 3
#define CS_PIN 2
#define RST_ETHRNT_PIN 6  // Используйте доступный порт для RST


MFRC522 rfid(SS_PIN, RST_PIN);  // Create rfid instance
LiquidCrystal_I2C lcd(0x27,16,2);


void loading(){
  lcd.init();
  lcd.backlight();
  delay(1000);
  lcd.setCursor(0,0);
  lcd.print("Slava Ukraini!");
  lcd.setCursor(0,1);
  lcd.print("Heroiam slava!");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Slava Nacii");
  lcd.setCursor(0,1);
  lcd.print("Smert` vrogam");
  delay(2000);
  lcd.clear();
}

void setup() {
 loading();
 Serial.begin(9600);		// Initialize serial communications with the PC
	SPI.begin();			// Init SPI bus
	rfid.PCD_Init();		// Init rfid
	delay(4);				// Optional delay. Some board do need more time after init to be ready, see Readme
	rfid.PCD_DumpVersionToSerial();	// Show details of PCD - rfid Card Reader details
	Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent()) {
    return;
  }

  if (!rfid.PICC_ReadCardSerial()) {
    return;
  }

  lcd.setCursor(0, 0);
  lcd.print("Card UID:");

  String uid = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    uid += String(rfid.uid.uidByte[i], HEX);
  }

  lcd.setCursor(0, 1);
  lcd.print(uid);

  delay(3000);

  lcd.clear();
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}
