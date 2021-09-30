//CONTROLE DO BRUSHLESS ATRAVEs DO ESC
#include <Servo.h> // biblioteca Servo (sera utilizada para o brushless e o ESC)
#include <cmath> // biblioteca matematica

#define PotenciometroPin A0 // definindo um pino analogico para o potenciometro
#define MotorPin 3 // definindo um pino digital  que envia o sinal para o ESC

Servo Brushless; // definindo um objeto do tipo servo para o motor brushless
int velocidade = 0;
int SensorValor= 0;

void setup(){
	Brushless.attach(MotorPin); // attaching o brushless ao pino digital para o ESC controla-lo
	Serial.begin(9600); //  a taxa padrao de transmissao do Arduino eh de 9600 bits por segundo

}

void loop(){
	SensorValor = analogRead(PotenciometroPin); // le o valor do potenciometro (entre 0 e 1023)
	Velocidade = map(SensorValor, 0, 1023, 0, 180);

	// considerando uma bateria de 11.1V como exemplo
	int tensao = Velocidade/255*11.1; // calculando a tensao no brushless
	float rotacao = tensao*1450/11.1;

	// considerando um propeller 13x4
	float VelocidadeMotor = ((0.3302/2)*M_PI*2*rotacao)/60; // convertendo para m/s

	Serial.println("Velocidade: " + VelocidadeMotor + " m/s"); // imprime a velocidade
	if(VelocidadeMotor>30){
		Serial.println.("Desacelere");
	}

}