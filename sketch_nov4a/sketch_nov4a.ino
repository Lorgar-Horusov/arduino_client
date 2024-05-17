#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define RST_PIN         9   // Пин сброса для модуля MFRC522, настраиваемый
#define SS_PIN          10  // Пин выбора устройства для модуля MFRC522, настраиваемый

MFRC522 rfid(SS_PIN, RST_PIN);   // Создаем экземпляр модуля RFID MFRC522
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Создаем экземпляр модуля LCD через I2C

struct Key {
  String uid;  // Уникальный идентификатор карты
  String name; // Имя пользователя
};

Key allowedKeys[] = {
  {"1b512f5f", "USER 1"},  // Разрешенные карты с их уникальными идентификаторами и именами пользователей
  {"64d747b", "USER 2"}
};

void loading() {
  lcd.init();
  lcd.backlight();
  delay(1000);
  lcd.setCursor(0, 0);
  lcd.print("Slava Ukraini!");   // Начальные сообщения на LCD
  lcd.setCursor(0, 1);
  lcd.print("Heroiam slava!");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Slava Nacii");
  lcd.setCursor(0, 1);
  lcd.print("Smert` vrogam");
  delay(2000);
  lcd.clear();
}

void setup() {
  loading();
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();   // Инициализация модуля RFID
  delay(4);
  rfid.PCD_DumpVersionToSerial();
  Serial.println(F("Сканируйте карту для просмотра UID, SAK, типа и блоков данных..."));
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent()) {
    return;   // Если новая карта не обнаружена, выходим из цикла
  }

  if (!rfid.PICC_ReadCardSerial()) {
    return;   // Если чтение карты не удалось, выходим из цикла
  }

  lcd.setCursor(0, 0);

  String uid = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    uid += String(rfid.uid.uidByte[i], HEX);   // Преобразуем байты UID в шестнадцатеричный формат и добавляем к строке uid
  }

  lcd.setCursor(0, 1);
  lcd.print(uid);

  // Проверка доступа
  int keyIndex = isAccessAllowed(uid);
  if (keyIndex != -1) {
    lcd.setCursor(0, 0);
    lcd.print("Access allowed");
    lcd.setCursor(0, 1);
    lcd.print("User: " + allowedKeys[keyIndex].name);
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Access denied!");
    lcd.setCursor(0, 1);
    lcd.print("Unknown user");
  }

  delay(3000);
  lcd.clear();
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}

// Функция для поиска индекса ключа в массиве allowedKeys
int isAccessAllowed(String uid) {
  for (int i = 0; i < sizeof(allowedKeys) / sizeof(allowedKeys[0]); i++) {
    if (uid.equals(allowedKeys[i].uid)) {
      return i; // Возвращаем индекс ключа, если найден
    }
  }
  return -1;    // Возвращаем -1, если ключ не найден
}
