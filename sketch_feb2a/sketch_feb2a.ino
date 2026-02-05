/*
  Programador: Hugo Fernandes
  Arduino UNO R4 WiFi
  Carro Bluetooth + Máquina de Estados
  Sensor Ultrassónico (global)
  Matriz LED "NO!" quando encontra obstáculo + PWM nos motores
*/

#include <ArduinoBLE.h>
#include <Arduino_LED_Matrix.h>
#include "animation.h"  // sua animação personalizada "NO!"

ArduinoLEDMatrix matrix;

// ===== Motores =====
int IN1 = 8, 
    IN2 = 9,
    IN3 = 10,
    IN4 = 11;
int ENA = 5,   // PWM
    ENB = 6;   // PWM

int velocidade = 200; // 0 a 255, ajusta suavidade

// ===== Sensor =====
const int trigPin = 2;
const int echoPin = 3;
long duration;
float distance;

// ===== Bluetooth =====
BLEService robotService("19B10000-E8F2-537E-4F6C-D104768A1214");
BLEByteCharacteristic commandChar(
  "19B10001-E8F2-537E-4F6C-D104768A1214",
  BLERead | BLEWrite
);

// ===== Estados =====
char command = 'S';

#define AVANCAR   1
#define TRAS      2
#define DIREITA   3
#define ESQUERDA  4
#define PARAR     5

int estadoAtual = PARAR;

void setup() {
  Serial.begin(9600);

  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  matrix.begin();
  matrix.clear();

  BLE.begin();
  BLE.setLocalName("Arduino UNO R4");
  BLE.setAdvertisedService(robotService);
  robotService.addCharacteristic(commandChar);
  BLE.addService(robotService);
  BLE.advertise();
}

void loop() {
  BLEDevice central = BLE.central();

  if (central) {
    while (central.connected()) {

      if (commandChar.written()) {
        command = commandChar.value();
        atualizarEstado();
      }

      // SENSOR GLOBAL
      distance = lerDistancia();
      if (distance <= 20) {
        pararComStop();  // chama sua animação "NO!"
        continue;        // ignora movimentos
      }

      switch (estadoAtual) {
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
        default:
          stopAll();
          break;
      }

      delay(30);
    }

    stopAll();
    matrix.clear();
  }
}

// ===== FUNÇÕES =====

void atualizarEstado() {
  if (command == 'F') estadoAtual = AVANCAR;
  else if (command == 'B') estadoAtual = TRAS;
  else if (command == 'L') estadoAtual = ESQUERDA;
  else if (command == 'R') estadoAtual = DIREITA;
  else estadoAtual = PARAR;
}

float lerDistancia() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH, 30000);
  if (duration == 0) return 999;
  return duration * 0.034 / 2;
}

// ===== PARAR COM ANIMAÇÃO PERSONALIZADA =====
void pararComStop() {
  stopAll();                // para os motores
  showObstacleAnimation();  // chama sua animação "NO!"
  estadoAtual = PARAR;      // mantém estado PARAR
}

// ===== MOVIMENTOS COM PWM =====
void moveForward() {
  analogWrite(ENA, velocidade);
  analogWrite(ENB, velocidade);
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void moveBackward() {
  analogWrite(ENA, velocidade);
  analogWrite(ENB, velocidade);
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
}

void turnLeft() {
  analogWrite(ENA, velocidade);
  analogWrite(ENB, velocidade);
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void turnRight() {
  analogWrite(ENA, velocidade);
  analogWrite(ENB, velocidade);
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
}

void stopAll() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);  // para suavemente
  analogWrite(ENB, 0);
}
