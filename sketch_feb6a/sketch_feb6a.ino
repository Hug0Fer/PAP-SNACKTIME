/*
  Programador........................: (C) Hugo Fernandes
  Data...............................: 29/12/2025
  Arduino UNO R4 WiFi
  Carro Bluetooth + Máquina de Estados
  HC-SR04 Frente e Trás
  Matriz LED "NO!" quando encontra obstáculo
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

byte VELOCIDADE = 200;

// Sensores
const byte TRIG_FRONT = 2;
const byte ECHO_FRONT = 3;
const byte TRIG_BACK  = 4;
const byte ECHO_BACK  = 7;

// Bluetooth
BLEService robotService("19B10000-E8F2-537E-4F6C-D104768A1214");
BLEByteCharacteristic commandChar(
  "19B10001-E8F2-537E-4F6C-D104768A1214",
  BLERead | BLEWrite
);

// Estados
#define AVANCAR   1
#define TRAS      2
#define DIREITA   3
#define ESQUERDA  4
#define PARAR     5

int estadoAtual = PARAR;
char command = 'S';

void setup() {

  Serial.begin(9600);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(TRIG_FRONT, OUTPUT);
  pinMode(ECHO_FRONT, INPUT);
  pinMode(TRIG_BACK, OUTPUT);
  pinMode(ECHO_BACK, INPUT);

  matrix.begin();
  matrix.clear();

  BLE.begin();
  BLE.setLocalName("Arduino do Hugão");
  BLE.setAdvertisedService(robotService);
  robotService.addCharacteristic(commandChar);
  BLE.addService(robotService);
  BLE.advertise();
}

//Switch Case da Maquina de Estados

void loop() {

  BLEDevice central = BLE.central();

  if (central) {

    while (central.connected()) {

      if (commandChar.written()) {
        command = commandChar.value();
        atualizarEstado();
      }

      float distFront = lerDistancia(TRIG_FRONT, ECHO_FRONT);
      float distBack  = lerDistancia(TRIG_BACK, ECHO_BACK);

      bool OBSTACULOFRENTE = distFront <= 20;
      bool OBSTACULOTRAS   = distBack  <= 20;

      if (OBSTACULOFRENTE || OBSTACULOTRAS) {
        showObstacleAnimation();
      } else {
        matrix.clear();
      }

      if (estadoAtual == AVANCAR && OBSTACULOFRENTE) {
        stopAll();
        estadoAtual = PARAR;
      }

      if (estadoAtual == TRAS && OBSTACULOTRAS) {
        stopAll();
        estadoAtual = PARAR;
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

// Estados

void atualizarEstado() {

  if (command == 'F') estadoAtual = AVANCAR;
  else if (command == 'B') estadoAtual = TRAS;
  else if (command == 'L') estadoAtual = ESQUERDA;
  else if (command == 'R') estadoAtual = DIREITA;
  else estadoAtual = PARAR;
}

float lerDistancia(byte TrigPin, byte EchoPin) {

  digitalWrite(TrigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);

  long dur = pulseIn(EchoPin, HIGH, 30000);

  if (dur == 0) return 404;

  return dur * 0.034 / 2;
}

void moveForward() {

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, VELOCIDADE);
  analogWrite(ENB, VELOCIDADE);
}

void moveBackward() {

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, VELOCIDADE);
  analogWrite(ENB, VELOCIDADE);
}

void turnLeft() {

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, VELOCIDADE);
  analogWrite(ENB, VELOCIDADE);
}

void turnRight() {

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, VELOCIDADE);
  analogWrite(ENB, VELOCIDADE);

}

void stopAll() {

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
