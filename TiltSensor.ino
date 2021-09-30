//ANALISE DA INCLINACAO

#define SensorInclinacao 2 // definindo o pino digital onde o sensor DHT11 se encontra

void setup(){
  Serial.begin(9600); //  a taxa padrao de transmissao do Arduino eh de 9600 bits por segundo
  pinMode(SensorInclinacao, INPUT); // definindo o pino da entrada de dados

}

void loop(){
  int SensorInclinacao_Condicao = digitalRead(SensorInclinacao); // le o valor condicao do sensor de inclinacao

  // avaliando se ha inclinacao
  if(SensorInclinacao_Condicao == LOW){
    Serial.println("Inclinado");
  }
	else{
    Serial.println("Normal");
  }

  delay(1000);// a leitura dos parametros eh realizada a cada 1 segundo
}