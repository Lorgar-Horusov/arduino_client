#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <deprecated.h>
#include <require_cpp11.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define RST_PIN         9          // Пин сброса для модуля MFRC522, настраиваемый
#define SS_PIN          10         // Пин выбора устройства для модуля MFRC522, настраиваемый

#define SCK_PIN 5                    // Пин SPI Clock для модуля Ethernet
#define SO_PIN 4                     // Пин SPI MISO (Master In Slave Out) для модуля Ethernet
#define SI_PIN 3                     // Пин SPI MOSI (Master Out Slave In) для модуля Ethernet
#define CS_PIN 2                     // Пин выбора устройства для модуля Ethernet
#define RST_ETHRNT_PIN 6             // Пин сброса для модуля Ethernet, используйте доступный порт для RST

MFRC522 rfid(SS_PIN, RST_PIN);      // Создаем экземпляр модуля RFID MFRC522
LiquidCrystal_I2C lcd(0x27,16,2);   // Создаем экземпляр модуля LCD через I2C

void loading(){
  lcd.init();                        // Инициализируем LCD
  lcd.backlight();                   // Включаем подсветку LCD
  delay(1000);
  lcd.setCursor(0,0);
  lcd.print("Slava Ukraini!");      // Выводим сообщение на LCD
  lcd.setCursor(0,1);
  lcd.print("Heroiam slava!");
  delay(2000);
  lcd.clear();                       // Очищаем экран LCD
  lcd.setCursor(0,0);
  lcd.print("Slava Nacii");
  lcd.setCursor(0,1);
  lcd.print("Smert` vrogam");
  delay(2000);
  lcd.clear();
}

void setup() {
  loading();                         // Вызываем функцию loading для отображения начальных сообщений на LCD
  Serial.begin(9600);                // Инициализируем последовательную связь с ПК
  SPI.begin();                       // Инициализируем шину SPI
  rfid.PCD_Init();                   // Инициализируем модуль RFID
  delay(4);                          // Дополнительная задержка после инициализации
  rfid.PCD_DumpVersionToSerial();    // Выводим детали модуля RFID в последовательный монитор
  Serial.println(F("Сканируйте карту для просмотра UID, SAK, типа и блоков данных..."));
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent()) {
    return;                          // Если новая карта не обнаружена, выходим из цикла
  }

  if (!rfid.PICC_ReadCardSerial()) {
    return;                          // Если чтение карты не удалось, выходим из цикла
  }

  lcd.setCursor(0, 0);
  lcd.print("Card UID:");

  String uid = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    uid += String(rfid.uid.uidByte[i], HEX);  // Преобразуем байты UID в шестнадцатеричный формат и добавляем к строке uid
  }

  lcd.setCursor(0, 1);
  lcd.print(uid);                    // Выводим UID на LCD

  delay(3000);

  lcd.clear();                       // Очищаем экран LCD
  rfid.PICC_HaltA();                 // Прекращаем взаимодействие с картой RFID
  rfid.PCD_StopCrypto1();            // Останавливаем шифрование на модуле RFID
}
