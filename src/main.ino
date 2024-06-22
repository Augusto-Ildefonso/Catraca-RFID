#include <MFRC522.h> 
#include <SPI.h>    
#include <Wire.h>   

#define SS_PIN 10
#define RST_PIN 9

MFRC522 rfid(SS_PIN, RST_PIN);

const int led_verde = 3;    
const int led_vermelho = 2;
const int tempo_delay = 3000;
const char* tag_valida = "TAG";
void setup() {

  // Inicializando biblioteca, barramento SPI e MFRC522
  Wire.begin();   
  SPI.begin();     
  rfid.PCD_Init();

  pinMode(led_verde, OUTPUT);   
  pinMode(led_vermelho, OUTPUT);

  digitalWrite(led_verde, LOW);
  digitalWrite(led_vermelho, LOW);
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
  char id[12];
  formatID(rfid.uid.uidByte, id);
  
if (strstr(strID, tag_valida) != NULL) {
    digitalWrite(led_vermelho, HIGH);
    delay(DELAY_TIME);
    digitalWrite(led_verde, LOW);
  } else {
    digitalWrite(led_verde, HIGH);
    delay(tempo_delay);
    digitalWrite(led_vermelho, LOW);
  }

  // Para leitura no cartão e criptografia no PCD
  rfid.PICC_HaltA();      
  rfid.PCD_StopCrypto1();
}
