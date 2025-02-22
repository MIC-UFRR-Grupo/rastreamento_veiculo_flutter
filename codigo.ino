#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_PN532.h>

// Definindo os pinos SDA e SCL para o PN532
#define PN532_SDA 21
#define PN532_SCL 22

#define OLED_RESET -1
Adafruit_SSD1306 display(OLED_RESET);
Adafruit_PN532 nfc(PN532_SDA, PN532_SCL);

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  // Inicializa o display OLED
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("OLED Iniciado");
  display.display();
  
  // Inicializa o sensor PN532
  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.println("Não foi possível encontrar o PN532");
    while (1);
  }
  nfc.SAMConfig();
  Serial.println("PN532 Iniciado");

  // Configura o pino do buzzer
  pinMode(18, OUTPUT);
  digitalWrite(18, LOW);
}

void loop() {
  // Código para ler dados do AD8232 e exibir no OLED
  int sensorValue = analogRead(34);
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("AD8232: ");
  display.print(sensorValue);
  
  // Leitura do PN532
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 }; // Buffer para armazenar o ID da tag
  uint8_t uidLength;                      // Comprimento do buffer de ID
  
  // Verifica se há uma tag disponível
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  if (success) {
    display.setCursor(0,10);
    display.print("Tag ID: ");
    for (uint8_t i=0; i < uidLength; i++) {
      display.print(uid[i], HEX);
    }
    display.display();
    
    // Aciona o buzzer se a tag for lida com sucesso
    digitalWrite(18, HIGH);
    delay(500); // Mantém o buzzer ligado por 500ms
    digitalWrite(18, LOW);
  } else {
    display.display();
  }

  delay(500);
}
