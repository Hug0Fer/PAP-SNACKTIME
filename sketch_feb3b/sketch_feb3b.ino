/*
  Programador: Hugo Fernandes
  Data: 28/12/2025
  ESP32 + L298N
  Máquina de Estados com HC-SR04
*/

#include <ArduinoBLE.h>

// ===== Pinos Motores =====
int IN1 = 8;
int IN2 = 9;
int IN3 = 10;
int IN4 = 11;
int ENA = 5;
int ENB = 6;

// ===== LEDs e Buzzer =====
int redLED = 12;
int blueLED = 13;
int buzzer = 7;

// ===== Sensor Ultrassónico =====
#define TRIG_PIN 2
#define ECHO_PIN 4
#define DISTANCIA_MIN 20  // cm

long duration;
float distance;

// ===== Comando =====
char command;

// ===== Estados =====
enum Estado {
  AGUARDAR_COMANDO,
  AVANCAR,
  TRAS,
  DIREITA,
  ESQUERDA,
  PARAR
};

Estado estadoAtual = AGUARDAR_COMANDO;

// ===== Setup =====
void setup() {
  // Motores
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  // LEDs e buzzer
  pinMode(redLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(buzzer, OUTPUT);

  // Sensor
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);

  Serial.begin(9600);
  Serial.println("Sistema iniciado - Aguardar comandos");
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
    }
  }

  // ---- Máquina de Estados ----
  switch (estadoAtual) {

    case AGUARDAR_COMANDO:
      stopAll();
      break;

    case AVANCAR:
      moveForward();
      verificarObstaculo();
      break;

    case TRAS:
      moveBackward();
      verificarObstaculo();
      break;

    case ESQUERDA:
      turnLeft();
      verificarObstaculo();
      break;

    case DIREITA:
      turnRight();
      verificarObstaculo();
      break;

    case PARAR:
      stopAll();
      Serial.println("Parado - Aguardar comando");
      delay(300);
      estadoAtual = AGUARDAR_COMANDO;
      break;
  }
}

// ===== Função Ultrassónica =====
float medirDistancia() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH, 30000); // 30 ms timeout

  if (duration == 0) return -1;

  return duration * 0.034 / 2;
}

// ===== Verificar Obstáculo =====
void verificarObstaculo() {
  distance = medirDistancia();

  if (distance > 0) {
    Serial.print("Distancia: ");
    Serial.print(distance);
    Serial.println(" cm");
  }

  if (distance > 0 && distance <= DISTANCIA_MIN) {
    Serial.println(" Obstaculo a menos de 20 cm!");
    digitalWrite(buzzer, HIGH);
    estadoAtual = PARAR;
  } else {
    digitalWrite(buzzer, LOW);
  }

  delay(100);
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
  digitalWrite(buzzer, LOW);
}
