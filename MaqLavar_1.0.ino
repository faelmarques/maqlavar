/* -- INCLUDES -- */
#include <avr/wdt.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
/* ----------------------------- */

/* ---------- DEFINIÇÕES ------------- */
LiquidCrystal_I2C lcd(0x27, 16, 2);  //Configura o display LCD

int tempoMolho = 1;          //CONFIGURAÇÃO: TEMPO DE MOLHO (MINUTOS)
int tempoCentrifugacao = 1;  //CONFIGURAÇÃO: TEMPO DE CENTRIFUGAÇÃO (MINUTOS)

int menu = 1;       //Define o menu principal ao iniciar
int eEnchendo = 0;  //Define o estado parado da máquina
int podeEtapa = 0;  //Define que a etapa pode ser pulada (NÃO ALTERAR NUNCA)
/* -----------------------------------------------------------------------------*/

void setup() {

  wdt_disable();  // DESABILITA O CÃO DE GUARDA (Include de tempo)

  /* ---------- INPUTS ---------- */
  pinMode(2, INPUT);  //Botao Funcao
  pinMode(3, INPUT);  //Botao Start
  pinMode(4, INPUT);  //Nivel Agua
                      /* ------------------------------- */


  /* ---------- OUTPUTS ---------- */
  pinMode(5, OUTPUT);   //Motor H
  pinMode(6, OUTPUT);   //Motor AH
  pinMode(7, OUTPUT);   //Freio
  pinMode(8, OUTPUT);   //Dreno
  pinMode(9, OUTPUT);   //Solenoide Agua
  pinMode(10, OUTPUT);  //Buzzer
  /* ------------------------------- */

  /* -------- INICIALIZAÇÃO ---------- */
  lcd.init();              // Inicia a biblioteca do display LCD
  lcd.setBacklight(HIGH);  // Define a luz do display LCD acesa

  lcd.setCursor(0, 0);
  lcd.print("Maquina de Lavar");
  lcd.setCursor(0, 1);
  lcd.print("By: Rafa Marques");
  delay(1500);
  lcd.setBacklight(LOW);

  tone(10, 10000, 500);

  delay(1000);
  lcd.setBacklight(HIGH);
  lcd.clear();
  menu1();
}
/* --------------------------------------------------------------------- */

/* -------- MENUS DE FUNÇÕES -------- */
void menu1() {

  tone(10, 10000, 100);  //Toca buzzer ao pressionar tecla

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Ciclo 1:");
  lcd.setCursor(0, 1);
  lcd.print("Molho");
}

void menu2() {

  tone(10, 10000, 100);  //Toca buzzer ao pressionar tecla

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Ciclo 2:");
  lcd.setCursor(0, 1);
  lcd.print("Enxague");
}

void menu3() {

  tone(10, 10000, 100);  //Toca buzzer ao pressionar tecla

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Ciclo 3:");
  lcd.setCursor(0, 1);
  lcd.print("Centrifugacao");
}
/* ------------------------------------------------------------------ */

/* --- LÓGICA DOS MENUS DE CICLOS --- */
void leitura() {

  if (menu == 1) {

    menu1();
    delay(500);
  }
  if (menu == 2) {
    menu2();
    delay(500);
  }
  if (menu == 3) {
    menu3();
    delay(500);
  }
}
/* --------------------------------- */

/* ---- FUNÇÃO DE GIRO (BATER ROUPAS) ---- */
void baterMotor() {
  digitalWrite(7, LOW);  //Desliga o freio
  delay(1000);

  digitalWrite(5, HIGH);
  delay(600);
  digitalWrite(5, LOW);

  delay(500);

  digitalWrite(6, HIGH);
  delay(600);
  digitalWrite(6, LOW);
}
/* --------------------------------------- */

/* ---- FUNÇÃO DE MOLHO (BATER ROUPAS 1) ---- */
void molho() {

  while (digitalRead(4) == LOW) {
    lcd.setCursor(0, 0);
    lcd.print("Enchendo maquina!");
    lcd.setCursor(0, 1);
    lcd.print("................");
    eEnchendo = 1;
    digitalWrite(9, HIGH);
  }

  lcd.clear();
  tone(10, 10000, 600);
  lcd.print("Maquina cheia!!");
  lcd.setBacklight(LOW);
  delay(500);
  lcd.setBacklight(HIGH);
  delay(500);
  lcd.setBacklight(LOW);
  delay(500);
  lcd.setBacklight(HIGH);
  eEnchendo = 0;
  digitalWrite(9, LOW);
  delay(1500);
  if (eEnchendo == 0) {

    long inicio = millis();                                   
    while (((millis() - inicio) / 1000) < 60 * tempoMolho) {
      lcd.setCursor(0, 0);
      lcd.print("Batendo roupa!");

      int minutos = (millis() - inicio) / 1000 / 60 - tempoMolho;
      int segundos = 59 + (minutos * 60 - ((millis() - inicio) / 1000 - 60 * tempoMolho));

      lcd.setCursor(0, 1);
      lcd.print(minutos + 1);
      lcd.print(" mins ");
      lcd.print(segundos);
      lcd.print(" segs  ");

      baterMotor();
    }
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Molho concluido!");
    tone(10, 10000, 500);
    delay(500);
    lcd.setBacklight(LOW);
    delay(500);
    lcd.setBacklight(HIGH);
    delay(500);
    lcd.setBacklight(LOW);
    delay(500);
    lcd.setBacklight(HIGH);
    delay(3000);
    enxague();
  }
}
/* --------------------------------------- */

/* ---- FUNÇÃO DE ENXAGUAR ROUPAS ---- */
void enxague() {

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Drenando Agua!");
  lcd.setCursor(0, 1);
  lcd.print("................");

  while (digitalRead(4) == HIGH) {
    digitalWrite(8, HIGH);  //Liga o dreno
  }
  tone(10, 10000, 100);

  digitalWrite(8, HIGH);
  delay(3000);            //mudar para 180000
  digitalWrite(7, HIGH);
  delay(3000);
  digitalWrite(5, HIGH);
  delay(5000);            //mudar para 90000
  digitalWrite(5, LOW);
  delay(5000);            //mudar para 15000
  digitalWrite(8, LOW);
  digitalWrite(7, LOW);
  tone(10, 10000, 1000);
  delay(6000);

  lcd.clear();

  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);

  delay(2000);

    while (digitalRead(4) == LOW) {
    lcd.setCursor(0, 0);
    lcd.print("Enchendo maquina!");
    lcd.setCursor(0, 1);
    lcd.print("................");
    eEnchendo = 1;
    digitalWrite(9, HIGH);
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Maquina cheia!!");
    eEnchendo = 0;
    tone(10, 10000, 500);
    delay(5000);
    lcd.clear();
    delay(3000);

  if (eEnchendo == 0) {

    long inicio = millis();                                 
    while (((millis() - inicio) / 1000) < 60 * tempoMolho) {
      lcd.setCursor(0, 0);
      lcd.print("Enxaguando");

      int minutos = (millis() - inicio) / 1000 / 60 - tempoMolho;
      int segundos = 59 + (minutos * 60 - ((millis() - inicio) / 1000 - 60 * tempoMolho));

      lcd.setCursor(0, 1);
      lcd.print(minutos + 1);
      lcd.print(" mins ");
      lcd.print(segundos);
      lcd.print(" segs  ");

      baterMotor();
    }
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    delay(5000);
    centrifugacao();
  }
}
}
/* --------------------------------------- */

/* ---- FUNÇÃO DE CENTRIFUGAR ROUPAS ---- */
void centrifugacao() {

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Esvaziando Agua!");
  lcd.setCursor(0, 1);
  lcd.print(".................");

  while (digitalRead(4) == HIGH) {
    digitalWrite(8, HIGH);
  }
  tone(10, 10000, 200);
  digitalWrite(8, HIGH);
  delay(5000);            //passar para 180000
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Centrifugando!");
  lcd.setCursor(0, 1);
  lcd.print("..................");

  digitalWrite(7, HIGH);
  delay(4000);
  digitalWrite(5, HIGH);
  delay(5000);            //passars para 120000
  digitalWrite(5, LOW); 
  delay(5000);            //passars para 120000
  digitalWrite(7, LOW);
  tone(10, 10000, 500);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Tarefa concluida!");
  tone(10, 10000, 1000);

  delay(1000);
  lcd.setBacklight(LOW);
  delay(500);
  lcd.setBacklight(HIGH);
  delay(500);
  lcd.setBacklight(LOW);
  delay(500);
  lcd.setBacklight(HIGH);
  delay(500);
  lcd.setBacklight(LOW);
  delay(500);
  lcd.setBacklight(HIGH);
  delay(500);
  lcd.setBacklight(LOW);
  delay(500);
  lcd.setBacklight(HIGH);
  delay(500);
  lcd.setBacklight(LOW);
  delay(500);
  lcd.setBacklight(HIGH);
  delay(500);

  lcd.clear();
  leitura();
}
/* --------------------------------------- */

/* --- LÓGICAS DE MENU E INICIAR CICLO --- */
void loop() {

  if (digitalRead(2) == HIGH) {
    tone(10, 10000, 500);
    if (menu <= 3) {
      menu = menu + 1;
      leitura();
    }
    if (menu > 3) {
      menu = 0;
      leitura();
    }
  }

  if (digitalRead(3) == HIGH) {
    tone(10, 10000, 500);
    if (menu == 1) {
      molho();
    }
    if (menu == 2) {
      enxague();
    }
    if (menu == 3) {
      centrifugacao();
    }
  }
}
/* --------------------------------------- */
