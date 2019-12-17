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

// Outras defnicoes
#define buttom 16

// Inicializa o módulo RC522
MFRC522 mfrc522(SS_PIN, RST_PIN);

// Inicializa o display no endereço 0x27
// Pinagem para o LCD D1 => SCL; D2 => SDA 
LiquidCrystal_I2C lcd(0x27,16,2);

// Declaração de variávies
char st[20];
String registro[] = {"85 0B 11 AB", "C3 FE 87 AB"}; // Tags registradas
String mensagem = "Aguardando...";
int len;
int cont1 = 0;
int cont2 = 0;
int botao;
bool troca = false;


void setup() {

    // Inicia a serial
    Serial.begin(9600);
    // Inicia as portas
    pinMode(buttom, INPUT);
    // Inicia a SPI bus
    SPI.begin();
    // Inicia o leitor de RFID MFRC522
    mfrc522.PCD_Init(); 
    // Inicia o LCD
    lcd.init();

    lcd.begin(16,2);
    lcd.backlight();

    // Mensagem inicial no LCD
    
    lcd.setBacklight(HIGH);
    lcd.print("Inicializando...");



    // Mensagens iniciais no serial monitor
    Serial.println();
    for (int i = 0; i < 2; i++)
    {
        Serial.println(registro[i]);
        delay(1000);
    }
    len = sizeof(registro)/sizeof(registro[0]); // Calcula a quantidade de tags registradas
    Serial.print("Quantidade de tags registradas: ");
    Serial.println(len);
    Serial.println("Aproxime o seu cartao do leitor...");
    Serial.println();

    lcd.setCursor(0,0);
    lcd.print("Aguardando...");
}

void loop() {
    
    botao = digitalRead(buttom);
    delay(200);
    if (botao)
    {
        troca = !troca;
        if (troca)
        {
            mensagem = "Gravando...";
            lcd.clear();                // Limpa o visor do LCD
            lcd.setCursor(0,0);         // Ajusta o cursor para o inicio
            lcd.print(mensagem);
        }
        else
        {
            mensagem = "Aguardando...";
            lcd.clear();                // Limpa o visor do LCD
            lcd.setCursor(0,0);         // Ajusta o cursor para o inicio
            lcd.print(mensagem);
        }
        
    }

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
            lcd.setCursor(0,0);
            // Faz a contagem das tags
            if (conteudo.substring(1) == registro[0])
            {
                if (troca)
                {
                    cont1++; 
                }
                else
                {
                    cont1--;
                    if (cont1 < 0)
                    {
                        cont1 = 0;
                    }
                    
                }
                
                Serial.print("Quantidade Chaveiro: ");
                Serial.println(cont1);
                // Mensagem impressa no LCD
                lcd.print("Dipirona 40mg");
                lcd.setCursor(0,1);
                lcd.print(cont1);
                lcd.print(" UN");

            }
            if (conteudo.substring(1) == registro[1])
            {
                if (troca)
                {
                    cont2++;
                }
                else
                {
                    cont2--;
                    if (cont2 < 0)
                    {
                        cont2 = 0;
                    }
                    
                }
                Serial.print("Quantidade Cartão: ");
                Serial.println(cont2);
                // Mensagem impressa no LCD
                lcd.print("Paracetamol 50mg");
                lcd.setCursor(0,1);
                lcd.print(cont2);
                lcd.print(" UN");

            }
        }
    }
    delay(2000);
    lcd.clear();                // Limpa o visor do LCD
    lcd.setCursor(0,0);         // Ajusta o cursor para o inicio
    lcd.print(mensagem);        // Aguarda nova leitura
}