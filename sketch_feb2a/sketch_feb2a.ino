/*
  Programador: Hugo Fernandes
  Data: 28/12/2025
  ESP32 + L298N
  Máquina de Estados
*/

#include <ArduinoBLE.h>

// ===== Pinos =====
int IN1 = 8;
int IN2 = 9;
int IN3 = 10;
int IN4 = 11;
int ENA = 5;
int ENB = 6;

int redLED = 12;
int blueLED = 13;
int buzzer = 7;

// ===== Comando =====
char command;

// ===== Estados =====
enum Estado {
  LIGADO,
  AGUARDAR_COMANDO,
  AVANCAR,
  TRAS,
  DIREITA,
  ESQUERDA,
  PARAR
};

Estado estadoAtual = LIGADO;

// ===== Setup =====
void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(redLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(buzzer, OUTPUT);

  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);

  Serial.begin(9600);

  estadoAtual = AGUARDAR_COMANDO;
}

// ===== Loop =====
void loop() {

  // ---- Leitura de comandos ----
  if (Serial.available() > 0) {
    command = Serial.read();

    switch (command) {
      case 'F': estadoAtual = AVANCAR; break;
      case 'B': estadoAtual = TRAS; break;
      case 'L': estadoAtual = ESQUERDA; break;
      case 'R': estadoAtual = DIREITA; break;
      case 'S': estadoAtual = PARAR; break;

      case 'X': digitalWrite(redLED, HIGH); break;
      case 'Y': digitalWrite(blueLED, HIGH); break;
      case 'Z': digitalWrite(buzzer, HIGH); break;
    }
  }

  // ---- Máquina de estados ----
  switch (estadoAtual) {

    case AGUARDAR_COMANDO:
      stopAll();
      break;

    case AVANCAR:
      moveForward();
      break;

    case TRAS:
      moveBackward();
      break;

    case ESQUERDA:
      turnLeft();
      break;

    case DIREITA:
      turnRight();
      break;

    case PARAR:
      stopAll();
      estadoAtual = AGUARDAR_COMANDO;
      break;
  }
}

// ===== Movimentos =====
void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moveBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

// ===== Stop geral =====
void stopAll() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  digitalWrite(redLED, LOW);
  digitalWrite(blueLED, LOW);
  digitalWrite(buzzer, LOW);
}
