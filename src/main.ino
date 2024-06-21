#include <MFRC522.h> 
#include <SPI.h>    
#include <Wire.h>   

#define SS_PIN 10
#define RST_PIN 9

MFRC522 rfid(SS_PIN, RST_PIN);

const int pinoLedVerde = 3;    
const int pinoLedVermelho = 2;
const int DELAY_TIME = 3000;
const char* VALID_TAG = "TAG";
void setup() {

  // Inicializando biblioteca, barramento SPI e MFRC522
  Wire.begin();   
  SPI.begin();     
  rfid.PCD_Init();

  pinMode(pinoLedVerde, OUTPUT);   
  pinMode(pinoLedVermelho, OUTPUT);

  digitalWrite(pinoLedVerde, LOW);
  digitalWrite(pinoLedVermelho, LOW);
}

/**
* Função principal
*/
void loop() {
  leituraRfid();
}

char* formatID(byte *buffer, char *output) {
  for (byte i = 0; i < 4; i++) {
    sprintf(&output[i*3], "%02X:", buffer[i]);
  }
  output[11] = '\0';
  return output;
}

/*
* Essa função realiza a leitura do cartão RFID
*/
void leituraRfid() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()){
    return; 
  }
  
  // Gera a tag RFID lida
  char strID[12];
  formatID(rfid.uid.uidByte, strID);
  
if (strstr(strID, VALID_TAG) != NULL) {
    digitalWrite(pinoLedVerde, HIGH);
    delay(DELAY_TIME);
    digitalWrite(pinoLedVerde, LOW);
  } else {
    digitalWrite(pinoLedVermelho, HIGH);
    delay(DELAY_TIME);
    digitalWrite(pinoLedVermelho, LOW);
  }

  // Para leitura no cartão e criptografia no PCD
  rfid.PICC_HaltA();      
  rfid.PCD_StopCrypto1();
}
