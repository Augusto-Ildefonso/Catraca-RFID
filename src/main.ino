#include <MFRC522.h>
#include <SPI.h>
#include <Wire.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 rfid(SS_PIN, RST_PIN);
int cont = 1;
char id_org[12];
const int led_verde = 3;
const int led_vermelho = 2;
const int led_teste = 8;
const int tempo_delay = 1000;
const char *tag_valida = "A3:79:B1:0E";
void setup() {
  int cond = 1;
  // Inicializando biblioteca, barramento SPI e MFRC522
  Wire.begin();
  SPI.begin();
  rfid.PCD_Init();
  pinMode(led_verde, OUTPUT);
  pinMode(led_vermelho, OUTPUT);
  pinMode(led_teste, OUTPUT);
  digitalWrite(led_teste, LOW);
  digitalWrite(led_verde, LOW);
  digitalWrite(led_vermelho, LOW);
  // formatID(rfid.uid.uidByte, id_org);
}

/**
 * Função principal
 */
void loop() {
  digitalWrite(led_teste, HIGH);
  while (cont) {
    if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
      return;

    formatID(rfid.uid.uidByte, id_org);

    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
    cont = 0;
  }
  digitalWrite(led_teste, LOW);
  leituraRfid();
}

char *formatID(byte *buffer, char *output) {

  for (byte i = 0; i < 4; i++) {
    sprintf(&output[i * 3], "%02X:", buffer[i]);
  }
  output[11] = '\0';

  return output;
}

/*
 * Essa função realiza a leitura do cartão RFID
 */
void leituraRfid() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }

  // Gera a tag RFID lida
  char id[12];
  formatID(rfid.uid.uidByte, id);

  if (strstr(id, id_org) != NULL) {
    digitalWrite(led_verde, HIGH);
    delay(tempo_delay);
    digitalWrite(led_verde, LOW);
  } else {
    digitalWrite(led_vermelho, HIGH);
    delay(tempo_delay);
    digitalWrite(led_vermelho, LOW);
  }

  // Para leitura no cartão e criptografia no PCD
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}
