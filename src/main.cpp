// PROJETO INTEGRADOR UNIVESP - 7º E 8º  BIMESTRES
// Controle e Monitoramento do estoque de medicamentos  em farmácias 
// de postos de saúde por meio da tecnologia RFID
//
// Programa : Leitura de Tags RFID
// 

// Inclusão de bibliotecas
#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Definicoes pino modulo RC522 
#define SS_PIN 02
#define RST_PIN 20 

// Inicializa o módulo RC522
MFRC522 mfrc522(SS_PIN, RST_PIN);

// Inicializa o display no endereço 0x3F
LiquidCrystal_I2C lcd(0x3F,16,2);

// Declaração de variávies
char st[20];
String registro[] = {"85 0B 11 AB", "C3 FE 87 AB"}; // Tags registradas
int len;
int cont1 = 0;
int cont2 = 0;

void setup() {

    // Inicia a serial
    Serial.begin(9600);
    // Inicia  SPI bus
    SPI.begin();
    // Inicia MFRC522
    mfrc522.PCD_Init(); 

    // Mensagem inicial no LCD
    lcd.begin(16,2);
    lcd.setBacklight(HIGH);
    lcd.print("Inicializando...");



    // Mensagens iniciais no serial monitor
    Serial.println();
    for (int i = 0; i < 2; i++)
    {
        Serial.println(registro[i]);
        delay(500);
    }
    len = sizeof(registro)/sizeof(registro[0]); // Calcula a qunatidade de tags registradas
    Serial.print("Quantidade de tags registradas: ");
    Serial.println(len);
    Serial.println("Aproxime o seu cartao do leitor...");
    Serial.println();

}

void loop() {
    
    // Aguarda a aproximacao da tag
    if ( ! mfrc522.PICC_IsNewCardPresent()) 
    {
        delay(50);
        return;
    }
    // Seleciona a tag
    if ( ! mfrc522.PICC_ReadCardSerial()) 
    {
        delay(50);
        return;
    }
    // Mostra UID na serial
    Serial.print("UID da tag :");
    String conteudo= "";
    // Faz a leitura da tag
    for (byte i = 0; i < mfrc522.uid.size; i++) 
    {
        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(mfrc522.uid.uidByte[i], HEX);
        conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
        conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    Serial.println();
    Serial.print("Mensagem : ");
    conteudo.toUpperCase();
    
    // Testa se a tag foi lida
    for (int i = 0; i < len; i++)
    {
        if (conteudo.substring(1) == registro[i])
        {
            // Faz a contagem das tags
            if (conteudo.substring(1) == registro[0])
            {
                cont1++;
                Serial.print("Quantidade Chaveiro: ");
                Serial.println(cont1);
                delay(500);
            }
            if (conteudo.substring(1) == registro[1])
            {
                cont2++;
                Serial.print("Quantidade Cartão: ");
                Serial.println(cont2);
                delay(500);
            }
        }
    }

}