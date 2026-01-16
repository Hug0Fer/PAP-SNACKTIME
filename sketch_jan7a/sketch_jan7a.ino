/*
/*
      Programador.............: (C) Hugo Fernandes
      Data....................: 28/12/2025
      Observações.............: Código feito e estruturado por mim mas com ajuda de pesquisas também.
      Projeto: Mesa Telecomandada
      Descrição: Controlo por Bluetooth com máquina de estados (switch case)
 */

// -------- PINOS L298N --------
#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11
#define ENA 5
#define ENB 6

// -------- VARIÁVEIS --------
char comando = ' ';
bool bluetoothLigado = false;
bool erroDetetado = false;
int velocidade = 150; // 0 a 255

// máquina de estados
enum EstadoSistema {
  INICIALIZACAO,
  AGUARDAR_BLUETOOTH,
  AGUARDAR_COMANDO,
  MOVIMENTO,
  PARAGEM,
  ERRO
};

EstadoSistema estadoAtual = INICIALIZACAO;

// Função dos motores
void pararMotores() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void moverFrente() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, velocidade);
  analogWrite(ENB, velocidade);
}

void moverTras() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, velocidade);
  analogWrite(ENB, velocidade);
}

void moverEsquerda() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, velocidade);
  analogWrite(ENB, velocidade);
}

void moverDireita() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, velocidade);
  analogWrite(ENB, velocidade);
}

// void setup
void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  Serial.begin(9600); // HC-05
  pararMotores();
}

// Loop
void loop() {

  switch (estadoAtual) {

    case INICIALIZACAO:
      pararMotores();
      estadoAtual = AGUARDAR_BLUETOOTH;
      break;

    case AGUARDAR_BLUETOOTH:
      if (Serial.available()) {
        bluetoothLigado = true;
        estadoAtual = AGUARDAR_COMANDO;
      }
      break;

    case AGUARDAR_COMANDO:
      if (Serial.available()) {
        comando = Serial.read();
        estadoAtual = MOVIMENTO;
      }
      break;

    case MOVIMENTO:
      switch (comando) {
        case 'F': // Frente
          moverFrente();
          break;

        case 'T': // Trás
          moverTras();
          break;

        case 'E': // Esquerda
          moverEsquerda();
          break;

        case 'D': // Direita
          moverDireita();
          break;

        case 'P': // Parar
          estadoAtual = PARAGEM;
          break;

        default:
          estadoAtual = PARAGEM;
          break;
      }
      break;

    case PARAGEM:
      pararMotores();
      estadoAtual = AGUARDAR_COMANDO;
      break;

    case ERRO:
      pararMotores();
      // Sistema em modo de segurança
      break;
  }

  // Verificação de erro (exemplo simples)
  if (erroDetetado) {
    estadoAtual = ERRO;
  }
}
