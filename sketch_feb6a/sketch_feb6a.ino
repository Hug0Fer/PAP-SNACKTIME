/* 
  Programador........................: (C) Hugo Fernandes
  Data...............................:     29/12/2025
  Observações........................: Em baixo hehe :D
    Arduino UNO R4 WiFi
    Carro Bluetooth + Máquina de Estados
    HC-SR04 na Frente e Trás
    Matriz LED "NO!" quando encontra obstáculo
*/

#include <ArduinoBLE.h>
#include <Arduino_LED_Matrix.h>
#include "animation.h"

ArduinoLEDMatrix matrix;

//  Motores defenidos
int IN1 = 8, 
IN2 = 9, 
IN3 = 10, 
IN4 = 11;
int ENA = 5, 
ENB = 6;
int velocidade = 200;

// Sensor Frente
const int trigFront = 2;
const int echoFront = 3;

// Sensor Trás 
const int trigBack = 4;
const int echoBack = 7;

// Bluetooth  que está no site do Arduino
BLEService robotService("19B10000-E8F2-537E-4F6C-D104768A1214");
BLEByteCharacteristic commandChar(
  "19B10001-E8F2-537E-4F6C-D104768A1214",
  BLERead | BLEWrite
);

// Estados
#define avancar   1
#define tras      2
#define direita   3
#define esquerda  4
#define parar     5

int estadoAtual = parar;
char command = 'S';

void setup() {
  Serial.begin(9600);

  pinMode(IN1, OUTPUT); 
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); 
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT); 
  pinMode(ENB, OUTPUT);

  pinMode(trigFront, OUTPUT);
  pinMode(echoFront, INPUT);
  pinMode(trigBack, OUTPUT);
  pinMode(echoBack, INPUT);

  matrix.begin();
  matrix.clear();

  BLE.begin();
  BLE.setLocalName("Arduino do Hugão");
  BLE.setAdvertisedService(robotService);
  robotService.addCharacteristic(commandChar);
  BLE.addService(robotService);
  BLE.advertise();
}

void loop() {
  BLEDevice central = BLE.central();

  if (central) {
    while (central.connected()) {

      // Comando
      if (commandChar.written()) {
        command = commandChar.value();
        atualizarEstado();
      }

      // Leitura dos Sensores
      float distFront = lerDistancia(trigFront, echoFront);
      float distBack  = lerDistancia(trigBack, echoBack);

      bool obstaculoFrente = distFront <= 20;
      bool obstaculoTras   = distBack  <= 20;

      // LED
      if (obstaculoFrente || obstaculoTras) {
        showObstacleAnimation();
      } else {
        matrix.clear();
      }

      // Bloquear só a direção perigosa
      if (estadoAtual == avancar && obstaculoFrente) {
        stopAll();
        estadoAtual = parar;
      }

      if (estadoAtual == tras && obstaculoTras) {
        stopAll();
        estadoAtual = parar;
      }

      // Movimentos/switch case da maquina de estados
      switch (estadoAtual) {
        case avancar:
          moveForward();
          break;

        case tras:
          moveBackward();
          break;

        case esquerda:
          turnLeft();
          break;

        case direita:
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

// ===== FUNÇÕES =====

void atualizarEstado() {
  if (command == 'F') estadoAtual = avancar;
  else if (command == 'B') estadoAtual = tras;
  else if (command == 'L') estadoAtual = esquerda;
  else if (command == 'R') estadoAtual = direita;
  else estadoAtual = parar;
}

float lerDistancia(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long dur = pulseIn(echoPin, HIGH, 30000);
  if (dur == 0) return 999;
  return dur * 0.034 / 2;
}

// void dos movimentos
void moveForward() {
  analogWrite(ENA, velocidade);
  analogWrite(ENB, velocidade);
  digitalWrite(IN1, HIGH); 
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); 
  digitalWrite(IN4, LOW);
}

void moveBackward() {
  analogWrite(ENA, velocidade);
  analogWrite(ENB, velocidade);
  digitalWrite(IN1, LOW); 
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); 
  digitalWrite(IN4, HIGH);
}

void turnLeft() {
  analogWrite(ENA, velocidade);
  analogWrite(ENB, velocidade);
  digitalWrite(IN1, LOW); 
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); 
  digitalWrite(IN4, LOW);
}

void turnRight() {
  analogWrite(ENA, velocidade);
  analogWrite(ENB, velocidade);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); 
  digitalWrite(IN4, HIGH);
}

void stopAll() {
  digitalWrite(IN1, LOW); 
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); 
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
