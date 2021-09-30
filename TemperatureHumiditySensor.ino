//ANALISE DA TEMPERATURA E HUMIDADE
//obs.: primeiro eh necessario baixar e instalar a biblioteca DHT
#include <dht.h> // biblioteca DHT (sera utilizada para o sensor analisar os parametros)

dht SensorTemp; // definindo um objeto do tipo dht para o sensor de temperatura e humidade

#define DHT11Pin 7 // definindo o pino digital onde o sensor DHT11 se encontra

void setup(){
	Serial.begin(9600); //  a taxa padrao de transmissao do Arduino eh de 9600 bits por segundo

}

void loop(){
	int leitura = SensorTemp.read(DHTPin); // le o valor da temperatura e humidade
	Serial.print("Temperatura = ");
	Serial.print(SensorTemp.temperature + "C\n"); // imprime a temperatura
	Serial.print("Humidade = ");
	Serial.print(SensorTemp.temperature + "%\n"); // imprime a humidade
	delay(1000); // a leitura dos parametros eh realizada a cada 1 segundo 

}