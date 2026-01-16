#define in1 8
#define in2 9
#define in3 10
#define in4 11
#define enA 5   // PWM motor A
#define enB 6   // PWM motor B

int command;
int Speed = 204; 
int Speedsec = 0; 
int brakeTime = 45;
int brkonoff = 1;

void setup() {
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    command = Serial.read();

    // Ignorar caracteres extras
    if (command == '\n' || command == '\r') return;

    Serial.print("Recebi: "); Serial.println((char)command);

    Stop(); // Para antes de qualquer movimento

    switch (command) {
      case 'F': forward(); break;
      case 'B': back(); break;
      case 'L': left(); break;
      case 'R': right(); break;
      case 'G': forwardleft(); break;
      case 'I': forwardright(); break;
      case 'H': backleft(); break;
      case 'J': backright(); break;

      case '0': Speed = 100; break;
      case '1': Speed = 140; break;
      case '2': Speed = 153; break;
      case '3': Speed = 165; break;
      case '4': Speed = 178; break;
      case '5': Speed = 191; break;
      case '6': Speed = 204; break;
      case '7': Speed = 216; break;
      case '8': Speed = 229; break;
      case '9': Speed = 242; break;
      case 'q': Speed = 255; break;

      case 'S': // Freio
        if (brkonoff == 1) {
          analogWrite(enA, 255);
          analogWrite(enB, 255);
          digitalWrite(in1, HIGH);
          digitalWrite(in2, HIGH);
          digitalWrite(in3, HIGH);
          digitalWrite(in4, HIGH);
          delay(brakeTime);
          Stop();
        }
        break;
    }
  }
}

// Funções de movimento
void forward() { digitalWrite(in1, HIGH); digitalWrite(in2, LOW); digitalWrite(in3, HIGH); digitalWrite(in4, LOW); analogWrite(enA, Speed); analogWrite(enB, Speed);}
void back() { digitalWrite(in1, LOW); digitalWrite(in2, HIGH); digitalWrite(in3, LOW); digitalWrite(in4, HIGH); analogWrite(enA, Speed); analogWrite(enB, Speed);}
void left() { digitalWrite(in1, LOW); digitalWrite(in2, HIGH); digitalWrite(in3, HIGH); digitalWrite(in4, LOW); analogWrite(enA, Speed); analogWrite(enB, Speed);}
void right() { digitalWrite(in1, HIGH); digitalWrite(in2, LOW); digitalWrite(in3, LOW); digitalWrite(in4, HIGH); analogWrite(enA, Speed); analogWrite(enB, Speed);}
void forwardleft() { digitalWrite(in1, LOW); digitalWrite(in2, HIGH); digitalWrite(in3, HIGH); digitalWrite(in4, LOW); analogWrite(enA, Speed/2); analogWrite(enB, Speed);}
void forwardright() { digitalWrite(in1, HIGH); digitalWrite(in2, LOW); digitalWrite(in3, LOW); digitalWrite(in4, HIGH); analogWrite(enA, Speed); analogWrite(enB, Speed/2);}
void backleft() { digitalWrite(in1, LOW); digitalWrite(in2, HIGH); digitalWrite(in3, HIGH); digitalWrite(in4, LOW); analogWrite(enA, Speed/2); analogWrite(enB, Speed);}
void backright() { digitalWrite(in1, HIGH); digitalWrite(in2, LOW); digitalWrite(in3, LOW); digitalWrite(in4, HIGH); analogWrite(enA, Speed); analogWrite(enB, Speed/2);}
void Stop() { analogWrite(enA, 0); analogWrite(enB, 0); digitalWrite(in1, LOW); digitalWrite(in2, LOW); digitalWrite(in3, LOW); digitalWrite(in4, LOW);}

