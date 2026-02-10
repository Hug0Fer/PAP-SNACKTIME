# PAP SNACKTIME - HUGÃO

Este projeto foi desenvolvido no âmbito da **Prova de Aptidão Profissional (PAP)** e consiste numa **mesinha telecomandada** controlada por uma aplicação que só dá para android, equipado com **sensor para detetar obstaculos** e **matriz LED que está no Arduino UNO R4 Wifi**, capaz de evitar obstáculos de forma autónoma parando mesmo a tempo.

Apesar de não ter sido uma ideia genial, para mim foi uma ideia **fora da caixa**, complicada de implementar, **nunca imaginaria o quanto este projeto pode ser melhorado no seu futuro**, acabou por ser um projeto inesperado, desafiante e muito divertido de desenvolver. Se tivesse mais tempo conserteza que estaria **mil vezes melhor** pois ai eu já teria uma visão para a sua fase final. No inicio do ano letivo do 12º tinha descoberto esta ideia, mas sem saber como implementar nem oque usar, mas por outro lado foi algo muito divertido de desenvolver e achei uma ideia que se por mãos á obra este projeto pode virar algo para ir ao **mercado!!**



## Componentes Utilizados😎

- Arduino UNO R4 WiFi com ESP32-S3 
- Motor L298N  
- 4 Motores DC  
- Sensor ultrassónico HC-SR04  
- Matriz LED integrada do Arduino UNO R4  
- Alimentação externa (bateria)
- LEDs externas compradas
- Cabos
- Chave PH1

---

## Funcionalidades🧐

- Controlo da mesinha telecomandada através de **Bluetooth**
- Movimento:  
  - Avançar  
  - Recuar  
  - Virar à esquerda  
  - Virar à direita  
- Sistema de **detetar obstáculos**:
  - Se um objeto estiver a **20 cm ou menos**, a mesa pára automaticamente e espera o proximo comando
- **Aviso visual**:
  - Quando o carro pára devido a um obstáculo, a matriz LED apresenta uma animação de aviso
- Máquina de estados para controlo lógico do funcionamento

---

## Lógica de Funcionamento😫

O sistema baseia-se numa **máquina de estados**, onde cada comando recebido por Bluetooth altera o estado atual da mesa.  
O sensor ultrassónico é lido continuamente, independentemente do movimento do carro.

Sempre que é detetado um obstáculo:
- O carro pára imediatamente
- A matriz LED entra em modo de aviso
- O carro só volta a mover-se quando o é dado um comando para o lado contrario do obstaculo deixa de estar presente

---

## Estrutura do Projeto🤩

- `sketch_feb2a` / `sketch_feb6a` – Código Arduino  
- `Datasheets` – Documentação técnica dos componentes  
- `Máquina de Estados.png` – Diagrama da máquina de estados  
- `Links e Coisas da PAP.rtf` – Apontamentos e recursos  
- `Imagens e vídeos` – Registos do desenvolvimento e testes

---

## Conclusão😂
Ao inicio nao sabia como realizar este projeto, nem tinha ideia da estrutura, foi complicado gerir bem o tempo, mas de resto correu tudo bem, agora daqui para frente quero melhorar e fazer oque gostava de ter tudo certinho na apresentação da PAP, mas somos humanos e sempre cometemos erros. Quando a microsoft lançou a primeira versão do Windows nao ficou perfeito e tava cheia de erros mas mesmo assim melhoraram, e é isso que irei fazer melhorar e tentar conseguir fazer algo pro futuro da humanidade. :D!!!

Este projeto permitiu aplicar conhecimentos de:
- Programação em Arduino muitos mesmo pois nao sou um génio... XD
- Eletrónica
- Sensores
- Aprendizagem na soldagem
- Comunicação Bluetooth
- Estruturas de controlo (máquina de estados)

Foi um trabalho desafiante que contribuiu significativamente para o desenvolvimento de competências técnicas e práticas.

---

## Autor🤗

**Hugo Luís Fernandes Robalo**  
Prova de Aptidão Profissional – Curso Profissional de Técnico de Informática
