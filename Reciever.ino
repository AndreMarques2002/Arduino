//RECIEVER (CONTROLE VIA RADIO)

#include <SPI.h> // biblioteca SPI (para comunicacao entre os dispositivos conectados)
#include <nRF24L01.h> // biblioteca do modulo NRF24L01
#include <RF24.h> // biblioteca RF24
#include <Servo.h> // biblioteca Servo

int ch_width_1 = 0;
int ch_width_2 = 0;
int ch_width_3 = 0;
int ch_width_4 = 0;

Servo ch1;
Servo ch2;
Servo ch3;
Servo ch4;

struct Signal{
  byte throttle;      
  byte pitch;
  byte roll;
  byte yaw;
};

Signal data;

const uint64_t pipeIn = 0xE9E8F0F0E1LL; // endereco do modulo
RF24 radio(13, 8); 

void ResetData(){
  // definindo o valor inicial
  // obs.: a posicao central dos potenciometros eh 127 (254/2=127) 
  data.throttle = 127; // para o motor
  data.pitch = 127; // posicionando no centro
  data.roll = 127; // posicionando no centro
  data.yaw = 127; // posicionando no centro
}

void setup(){
  
  // ajustando os pinos ao sinal PWM
  ch1.attach(9); // attaching o brushless
  ch2.attach(6); // attaching o servo do elevator
  ch3.attach(11); // attaching o servo do leme
  ch4.attach(10); // attaching o servo do aileron

  // configurando o modulo NRF24L01
  ResetData();
  radio.begin();
  radio.openReadingPipe(1,pipeIn);
  
  radio.startListening(); // inicia a comunicacao via radio atraves do receiver
}

unsigned long lastRecvTime = 0;

void recvData(){
  while(radio.available()){
    radio.read(&data, sizeof(Signal));
    lastRecvTime = millis(); // recebe os dados
  }
}

void loop(){
  recvData();
  unsigned long now = millis();
  if(now - lastRecvTime>1000){
    ResetData(); // perda de sinal (reiniciar o sistema)
  }
  ch_width_1 = map(data.throttle, 0, 255, 1000, 2000); // pino D9 (sinal PWM) - acelerador
  ch_width_2 = map(data.pitch, 0, 255, 1000, 2000); // pino D6 (sinal PWM) - angulo de inclinacao (eixo y)
  ch_width_3 = map(data.roll, 0, 255, 1000, 2000); // pino D11 (sinal PWM) - angulo de rolagem (eixo x)
  ch_width_4 = map(data.yaw, 0, 255, 1000, 2000); // pino D10 (sinal PWM) - angulo de guinada (eixo z)
  
  // escrevendo o sinal PWM
  ch1.writeMicroseconds(ch_width_1);
  ch2.writeMicroseconds(ch_width_2);
  ch3.writeMicroseconds(ch_width_3);
  ch4.writeMicroseconds(ch_width_4);
}