#include <MFRC522.h> //INCLUSÃO DE BIBLIOTECA
#include <SPI.h>     //INCLUSÃO DE BIBLIOTECA
#include <Wire.h>    //INCLUSÃO DA BIBLIOTECA NECESSÁRIA

#define SS_PIN 10 // PINO SDA
#define RST_PIN 9 // PINO DE RESET

MFRC522 rfid(SS_PIN, RST_PIN); // PASSAGEM DE PARÂMETROS REFERENTE AOS PINOS

const int pinoLedVerde = 3;    // PINO DIGITAL REFERENTE AO LED VERDE
const int pinoLedVermelho = 2; // PINO DIGITAL REFERENTE AO LED VERMELHO
const int DELAY_TIME = 3000;
const char* VALID_TAG = "TAG";
void setup() {

  Wire.begin();    // INICIALIZA A BIBLIOTECA WIRE
  SPI.begin();     // INICIALIZA O BARRAMENTO SPI
  rfid.PCD_Init(); // INICIALIZA MFRC522

  pinMode(pinoLedVerde, OUTPUT);    // DEFINE O PINO COMO SAÍDA
  pinMode(pinoLedVermelho, OUTPUT); // DEFINE O PINO COMO SAÍDA

  digitalWrite(pinoLedVerde, LOW);    // LED INICIA DESLIGADO
  digitalWrite(pinoLedVermelho, LOW); // LED INICIA DESLIGADO
}

void loop() {
  leituraRfid(); // CHAMA A FUNÇÃO RESPONSÁVEL PELA VALIDAÇÃO DA TAG RFID
}

char* formatID(byte *buffer, char *output) {
  for (byte i = 0; i < 4; i++) {
    sprintf(&output[i*3], "%02X:", buffer[i]);
  }
  output[11] = '\0'; // null terminate the string
  return output;
}

// FUNÇÃO DE VALIDAÇÃO DA TAG RFID
void leituraRfid() {
  if (!rfid.PICC_IsNewCardPresent() ||
      !rfid.PICC_ReadCardSerial()) // VERIFICA SE O CARTÃO PRESENTE NO LEITOR É
                                   // DIFERENTE DO ÚLTIMO CARTÃO LIDO. CASO NÃO
                                   // SEJA, FAZ
    return; // RETORNA PARA LER NOVAMENTE

  /***INICIO BLOCO DE CÓDIGO RESPONSÁVEL POR GERAR A TAG RFID LIDA***/
  char strID[12];
  formatID(rfid.uid.uidByte, strID);
  /***FIM DO BLOCO DE CÓDIGO RESPONSÁVEL POR GERAR A TAG RFID LIDA***/

if (strstr(strID, VALID_TAG) != NULL) {
    digitalWrite(pinoLedVerde, HIGH);
    delay(DELAY_TIME);
    digitalWrite(pinoLedVerde, LOW);
  } else {
    digitalWrite(pinoLedVermelho, HIGH);
    delay(DELAY_TIME);
    digitalWrite(pinoLedVermelho, LOW);
  }
  rfid.PICC_HaltA();      // PARADA DA LEITURA DO CARTÃO
  rfid.PCD_StopCrypto1(); // PARADA DA CRIPTOGRAFIA NO PCD
}
