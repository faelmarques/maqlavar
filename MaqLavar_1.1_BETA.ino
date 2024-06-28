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

/* ---- DEFINIÇÕES PORTAS ---- */
int bFuncao = 2;
int bStart = 3;
int nivelAgua = 4;

int motorH = 5;
int motorAH = 6;
int freio = 7;
int dreno = 8;
int solenoide = 9;
int buzzer = 10;
/* ---------------------------- */

void setup() {

  wdt_disable();  // DESABILITA O CÃO DE GUARDA (Include de tempo)

  /* ---------- INPUTS ---------- */
  pinMode(bFuncao, INPUT);
  pinMode(bStart, INPUT);
  pinMode(nivelAgua, INPUT);
  /* ------------------------------- */

  /* ---------- OUTPUTS ---------- */
  pinMode(motorH, OUTPUT);
  pinMode(motorAH, OUTPUT);
  pinMode(freio, OUTPUT);
  pinMode(dreno, OUTPUT);
  pinMode(solenoide, OUTPUT);
  pinMode(buzzer, OUTPUT);
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

  tone(buzzer, 10000, 500);

  delay(1000);
  lcd.setBacklight(HIGH);
  lcd.clear();
  menu1();
}
/* --------------------------------------------------------------------- */

/* -------- MENUS DE FUNÇÕES -------- */
void menu1() {

  tone(buzzer, 10000, 100);  //Toca buzzer ao pressionar tecla

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Ciclo 1:");
  lcd.setCursor(0, 1);
  lcd.print("Molho");
}

void menu2() {

  tone(buzzer, 10000, 100);  //Toca buzzer ao pressionar tecla

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Ciclo 2:");
  lcd.setCursor(0, 1);
  lcd.print("Enxague");
}

void menu3() {

  tone(buzzer, 10000, 100);  //Toca buzzer ao pressionar tecla

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
  digitalWrite(freio, LOW);
  delay(1000);

  digitalWrite(motorH, HIGH);
  delay(600);
  digitalWrite(motorH, LOW);

  delay(500);

  digitalWrite(motorAH, HIGH);
  delay(600);
  digitalWrite(motorAH, LOW);
}
/* --------------------------------------- */

/* ---- FUNÇÃO DE MOLHO (BATER ROUPAS 1) ---- */
void molho() {

  while (digitalRead(nivelAgua) == LOW) {
    lcd.setCursor(0, 0);
    lcd.print("Enchendo maquina!");
    lcd.setCursor(0, 1);
    lcd.print("................");
    eEnchendo = 1;
    digitalWrite(solenoide, HIGH);
  }

  digitalWrite(solenoide, LOW);
  lcd.clear();
  tone(buzzer, 10000, 600);
  lcd.print("Maquina cheia!!");
  lcd.setBacklight(LOW);
  delay(500);
  lcd.setBacklight(HIGH);
  delay(500);
  lcd.setBacklight(LOW);
  delay(500);
  lcd.setBacklight(HIGH);
  eEnchendo = 0;
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
    digitalWrite(motorH, LOW);
    digitalWrite(motorAH, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Molho concluido!");
    tone(buzzer, 10000, 500);
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

  while (digitalRead(nivelAgua) == HIGH) {
    digitalWrite(dreno, HIGH);
  }
  tone(buzzer, 10000, 100);

  digitalWrite(dreno, HIGH);
  delay(3000);            //mudar para 180000
  digitalWrite(freio, HIGH);
  delay(3000);
  digitalWrite(motorH, HIGH);
  delay(5000);            //mudar para 90000
  digitalWrite(motorH, LOW);
  delay(5000);            //mudar para 15000
  digitalWrite(freio, LOW);
  delay(5000);            //mudar para 60000
  digitalWrite(dreno, LOW);
  tone(buzzer, 10000, 1000);
  delay(6000);

  lcd.clear();

  digitalWrite(motorH, LOW);
  digitalWrite(motorAH, LOW);
  digitalWrite(freio, LOW);
  digitalWrite(dreno, LOW);
  digitalWrite(solenoide, LOW);

  delay(2000);

    while (digitalRead(nivelAgua) == LOW) {
    lcd.setCursor(0, 0);
    lcd.print("Enchendo maquina!");
    lcd.setCursor(0, 1);
    lcd.print("................");
    eEnchendo = 1;
    digitalWrite(solenoide, HIGH);
    }

    digitalWrite(solenoide, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Maquina cheia!!");
    eEnchendo = 0;
    tone(buzzer, 10000, 500);
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
    digitalWrite(motorH, LOW);
    digitalWrite(motorAH, LOW);
    delay(5000);
    centrifugacao();
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

  while (digitalRead(nivelAgua) == HIGH) {
    digitalWrite(dreno, HIGH);
  }
  tone(buzzer, 10000, 200);
  digitalWrite(dreno, HIGH);
  delay(5000);            //passar para 180000
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Centrifugando!");
  lcd.setCursor(0, 1);
  lcd.print("..................");

  digitalWrite(freio, HIGH);
  delay(4000);
  digitalWrite(motorH, HIGH);
  delay(5000);            //passars para 120000
  digitalWrite(motorH, LOW); 
  delay(5000);            //passars para 50000
  digitalWrite(freio, LOW);
  delay(20000);
  digitalWrite(dreno, LOW);
  tone(buzzer, 10000, 500);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Tarefa concluida!");
  tone(buzzer, 10000, 1000);

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

  if (digitalRead(bFuncao) == HIGH) {
    tone(buzzer, 10000, 500);
    if (menu <= 3) {
      menu = menu + 1;
      leitura();
    }
    if (menu > 3) {
      menu = 0;
      leitura();
    }
  }

  if (digitalRead(bStart) == HIGH) {
    tone(buzzer, 10000, 500);
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
