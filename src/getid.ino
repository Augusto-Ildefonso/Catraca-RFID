#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 rfid(SS_PIN, RST_PIN);

/**
* Essa função inicializa a serial, o SPI e a MFRC522
*/
void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init(); 
}

char* formatar_id(byte *buffer, char *output) {
  for (byte i = 0; i < 4; i++) {
    sprintf(&output[i*3], "%02X:", buffer[i]);
  }
  output[11] = '\0';
  return output;
}

/**
* Essa é a função principal
*/
void loop() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    return;

  char strID[12];
  formatar_id(rfid.uid.uidByte, strID);

  Serial.print("Identificador (UID) da tag: ");
  Serial.println(strID);

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}
