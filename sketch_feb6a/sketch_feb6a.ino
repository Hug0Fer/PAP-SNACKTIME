/*
  Programador........................: (C) Hugo Fernandes
  Data...............................: 29/12/2025
  Observações........................: Arduino UNO R4 WiFi, Carro Bluetooth, switch case da Máquina de Estados
  HC-SR04 Frente e na parte de Trás.
  Matriz LED a dizer "NO!" quando encontra obstáculo na parte da frente ou na parte de trás.
*/

#include <ArduinoBLE.h>
#include <Arduino_LED_Matrix.h>
#include "animation.h"

ArduinoLEDMatrix matrix;

// Pinos do Motor L298N
const byte IN1 = 8;
const byte IN2 = 9;
const byte IN3 = 10;
const byte IN4 = 11;
const byte ENA = 5;
const byte ENB = 6;

byte velocidade = 250;

// Sensor da Frente e Trás
const int trigPinFront = 2;
const int echoPinFront = 3;
const int trigPinBack = 4;
const int echoPinBack = 7;

long duration;
float distFrente;
float distTras;

// Bloqueios
bool bloqueioFrente = false;
bool bloqueioTras   = false;

// Para não repetir animação sem parar
bool jaMostrouFrente = false;

// Bluetooth
BLEService robotService("18B10001-E8F2-537E-4F6C-D104768A1215");
BLEByteCharacteristic commandChar(
  "18B10002-E8F2-537E-4F6C-D104768A1215",
  BLERead | BLEWrite
);

// Estados da maquina
char command = 'S';

#define LIGADO    0
#define AVANCAR   1
#define TRAS      2
#define DIREITA   3
#define ESQUERDA  4
#define PARAR     5

int estadoAtual = PARAR;

// Setup
void setup() {
  Serial.begin(9600);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(trigPinFront, OUTPUT);
  pinMode(echoPinFront, INPUT);

  pinMode(trigPinBack, OUTPUT);
  pinMode(echoPinBack, INPUT);

  matrix.begin();
  matrix.clear();

  BLE.begin();
  BLE.setLocalName("Arduino do Hugão");
  BLE.setAdvertisedService(robotService);
  robotService.addCharacteristic(commandChar);
  BLE.addService(robotService);
  BLE.advertise();
}

// Loop
void loop() {
  BLEDevice central = BLE.central();

  if (central) {
    while (central.connected()) {

      // Ler sensores
      distFrente = lerDistancia(trigPinFront, echoPinFront);
      distTras   = lerDistancia(trigPinBack, echoPinBack);

      // Atualiza bloqueios
      bloqueioFrente = (distFrente <= 20);
      bloqueioTras   = (distTras   <= 20);

      // Se recebeu comando
      if (commandChar.written()) {
        command = commandChar.value();
        atualizarEstado(); // aqui já respeita bloqueios
      }

      // Intercepta movimento perigoso
      // Frente bloqueada e está a tentar avançar -> pára e mostra animação (1x por evento)
      if (estadoAtual == AVANCAR && bloqueioFrente) {
        estadoAtual = PARAR;

        if (!jaMostrouFrente) {
          showObstacleAnimation();
          jaMostrouFrente = true;
        }
      } else {
        // se já não há obstáculo à frente, volta a permitir mostrar numa próxima vez
        if (!bloqueioFrente) jaMostrouFrente = false;

        // Trás bloqueada e está a tentar recuar -> pára (sem animação)
        if (estadoAtual == TRAS && bloqueioTras) {
          estadoAtual = PARAR;
        } else {
          // Máquina de estados 
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
        }
      }

      delay(30);
    }

    stopAll();
    matrix.clear();
  }
}

// Funções e Estado

void atualizarEstado() {
  // Bloqueia só o comando perigoso
  if (command == 'F') {
    if (!bloqueioFrente) estadoAtual = AVANCAR;
    else estadoAtual = PARAR; // ignora F (fica parado)
  }
  else if (command == 'B') {
    if (!bloqueioTras) estadoAtual = TRAS;
    else estadoAtual = PARAR; // ignora B (fica parado)
  }
  else if (command == 'L') estadoAtual = ESQUERDA;
  else if (command == 'R') estadoAtual = DIREITA;
  else estadoAtual = PARAR;
}

float lerDistancia(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH, 30000);
  if (duration == 0) return 999;
  return duration * 0.034 / 2;
}

// Movimentos
void moveForward() {
  analogWrite(ENA, velocidade);
  analogWrite(ENB, velocidade);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void moveBackward() {
  analogWrite(ENA, velocidade);
  analogWrite(ENB, velocidade);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnLeft() {
  analogWrite(ENA, velocidade);
  analogWrite(ENB, velocidade);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnRight() {
  analogWrite(ENA, velocidade);
  analogWrite(ENB, velocidade);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void stopAll() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
