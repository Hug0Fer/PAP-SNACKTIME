/*
      Programador.............: (C) Hugo Fernandes
      Data....................: 28/12/2025
      Observações.............: Código feito e estruturado por mim mas com ajuda de pesquisas também.
*/



// Pinos dos motores
int IN1 = 8;
int IN2 = 9;
int IN3 = 10;
int IN4 = 11;
int ENA = 5;
int ENB = 6;

// LEDs e Buzzer 
int redLED = 12;
int blueLED = 13;
int buzzer = 7;

// Variáveis
char command;
int speedVal = 150;   // velocidade inicial (0–255)

// Setup 
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

  // Bluetooth
  Serial.begin(9600);
}

// Loop 
void loop() {
  if (Serial.available() > 0) {
    command = Serial.read();

    stopAll(); // reseta antes de executar novo comando

    switch (command) {
      case 'F': moveForward(); break;
      case 'B': moveBackward(); break;
      case 'L': turnLeft(); break;
      case 'R': turnRight(); break;
      case '+': increaseSpeed(); break;
      case '-': decreaseSpeed(); break;
      case 'X': digitalWrite(redLED, HIGH); break;
      case 'Y': digitalWrite(blueLED, HIGH); break;
      case 'Z': digitalWrite(buzzer, HIGH); break;
      case 'S': stopAll(); break;
    }
  }
}

// Movimentos
void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, speedVal);
  analogWrite(ENB, speedVal);
}

void moveBackward() {
7778798081828384858687888990919293949596979899100101102103
/*
      Programador.............: (C) Hugo Fernandes
      Data....................: 28/12/2025
      Observações.............: Código feito e estruturado por mim mas com ajuda de pesquisas também.
*/



// Pinos dos motores
int IN1 = 8;


  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, speedVal);
  analogWrite(ENB, speedVal);
}

void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, speedVal);
  analogWrite(ENB, speedVal);
}

void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, speedVal);
  analogWrite(ENB, speedVal);
}

//Stop geral do carro
void stopAll() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(redLED, LOW);
  digitalWrite(blueLED, LOW);
  digitalWrite(buzzer, LOW);
}

//Controlo da Velocidade
void increaseSpeed() {
  speedVal += 25;
  if (speedVal > 255) speedVal = 255;
  Serial.print("Speed: ");
  Serial.println(speedVal);
}

void decreaseSpeed() {
  speedVal -= 25;
  if (speedVal < 0) speedVal = 0;
  Serial.print("Speed: ");
  Serial.println(speedVal);
}