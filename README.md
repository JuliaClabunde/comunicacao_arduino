Teste de descrição do projeto - Comunicação ARDUINO - PC
// CONFIGURAÇÃO DO ARDUINO

#include <LiquidCrystal.h>

/* Pinagem do LCD:
 * RS no pino 7
 * Enable no pino 6
 * D4 pin no pino 5
 * D5 pin no pino 4
 * D6 pin no pino 3
 * D7 pin no pino 2
 * R/W no GND
 * Ground no GND
 * Power no 5V
 */

int rs = 7;
int en = 6;
int d4 = 5;
int d5 = 4;
int d6 = 3;
int d7 = 2;
int escolha = LOW; // Variável para armazenar a escolha
char letras[] = {'a', 'e', 'i', 'o', 'u'}; // Letras disponíveis para seleção
char message_ino[6] = "     "; // Mensagem a ser enviada, inicializada com espaços
String mensagem_serial = " "; // Certifica-se de que a string termina corretamente
int cont = 0;




int posicao = 0; // Índice da letra a ser selecionada
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);

  // Define o pino do botão 1 como entrada
  pinMode(8, INPUT_PULLUP); // Alterado para INPUT_PULLUP
  // Define o pino do botão 2 como entrada
  pinMode(9, INPUT_PULLUP);

  chamaEscolha();
  delay(200); 
}

void loop() {
  // Mantém o loop vazio, já que a função chamaEscolha() é chamada no setup()
  // Limpa a tela
  chamaEscolha();
}

void chamaEscolha() {
  // Aguarda mais 1 segundo para exibir a segunda linha
  delay(1000);
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("ESCOLHA");
  lcd.setCursor(2, 1);
  lcd.print("PC(X) - INO( )");

  while (digitalRead(9) == HIGH) {
    pc_ino();
  }
}

void pc_ino() {
  if ((digitalRead(8) == LOW) && (escolha == LOW)) {
    lcd.setCursor(2, 1);
    lcd.print("PC( ) - INO(X)");
    escolha = HIGH;
    while (digitalRead(8) == LOW) {
    }
  }

 delay(50);
  if ((digitalRead(8) == LOW) && (escolha == HIGH)) {
    lcd.setCursor(2, 1);
    lcd.print("PC(X) - INO( )");
    escolha = LOW;
    while (digitalRead(8) == LOW) {
    }
  }
  delay(50);

  if ((digitalRead(9) == LOW) && (escolha == LOW)) {
    // Se PC estiver selecionado
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Aguarde...");
    delay(2000); // Aguarda por 2 segundos
    lcd.clear();
    lcd.setCursor(0, 0);
    receiveMessageFromPC();
    delay(200);
    chamaEscolha(); // Retorna ao início após exibir a mensagem
  }

  if ((digitalRead(9) == LOW) && (escolha == HIGH)) {
    // Se INO estiver selecionado
    escolha = LOW;
    selecionaLetra();
  }
  delay(200);
}
void receiveMessageFromPC() {
  String message = ""; // Inicializa a string para armazenar a mensagem
  while ((Serial.available() == 0)&&(digitalRead(9) == HIGH)){
      lcd.setCursor(0, 0);
      lcd.print("Mensagem:");
  }
  if (Serial.available() > 0) {
    message = Serial.readStringUntil('\n'); // Lê a mensagem enviada pelo PC até encontrar um '\n'
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Mensagem:");
  lcd.setCursor(0, 1);
  lcd.print(message); // Exibe a mensagem no LCD
  delay(1000);
}

void selecionaLetra() {
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Escolha: ");
  lcd.setCursor(1, 1);
  lcd.print(letras[posicao]);
  delay(200);

  for (int i = 1; i <= 5; i++) {
    while (digitalRead(9) == LOW) {}
    posicao = 0;
    lcd.setCursor(i, 1);
    lcd.print(letras[posicao]);

    while (true) {
      if (digitalRead(8) == LOW) {
        while (digitalRead(8) == LOW) {}
        posicao++;
        if (posicao >= 5) {
          posicao = 0; // Volta para a primeira letra se passar da última
        }
        lcd.setCursor(i, 1);
        lcd.print(letras[posicao]);
        delay(200);
      }
      if (digitalRead(9) == LOW) {
        while (digitalRead(9) == LOW) {}
        message_ino[cont] = letras[posicao]; // Adiciona a letra selecionada à mensagem
        lcd.setCursor(i, 1);
        lcd.print(letras[posicao]); // Mostra a letra selecionada no LCD
        mensagem_serial = String(message_ino); 
        cont++;
        break;
      }

    }
  }
  Serial.println(mensagem_serial); 
  //Serial.println(message_ino); // Envia a mensagem para o PC
  lcd.setCursor(3, 0);
  lcd.print("ENVIANDO...");
  delay(1000); // Aguarda 2 segundos antes de retornar ao menu de escolha
  cont = 0;
 
  chamaEscolha(); // Retorna ao menu de escolha
}
