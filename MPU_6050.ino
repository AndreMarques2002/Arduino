//ANALISE DA INCLINACAO DO VANT ATRAVES DO ACELEROMETRO E GIROSCOPIO MPU-6050

#include <Wire.h> // biblioteca Wire

// variaveis associadas ao giroscopio
int giroscopio_x, giroscopio_y, giroscopio_z;
long giroscopio_x_medidido, giroscopio_y_medidido, giroscopio_z_medidido;
boolean set_gyro_angles;

// variaveis associadas ao acelerometro
long acelerometro_x, acelerometro_y, acelerometro_z, acelerometro_total_vector;
float angulo_rolagem_acelerometro, angulo_inclinacao_acelerometro;

float angulo_inclinacao, angulo_rolagem;
int angulo_inclinacao_buffer, angulo_rolagem_buffer;
float angulo_inclinacao_output, angulo_rolagem_output;

long loop_contador;
int temp;

void setup(){
    Wire.begin();
    setup_mpu_6050_registers(); // configura os registros do MPU-6050 
    for(int cal_int=0; cal_int<1000; cal_int++){ // le os dados preliminares do giroscopio e acelerometro (1000 vezes)
        read_mpu_6050_data();                                             
        giroscopio_x_medidido += giroscopio_x; // soma a variacao do giroscopio (no que tange ao eixo x)
        giroscopio_y_medidido += giroscopio_y; // soma a variacao do giroscopio (no que tange ao eixo y)
        giroscopio_z_medidido += giroscopio_z; // soma a variacao do giroscopio (no que tange ao eixo z)
        delay(3); // a leitura dos parametros eh realizada a cada 0.003s para que um loop tenha 250Hz
    }
    
    // dividindo por 1000 para obter a variacao media
    giroscopio_x_medidido /= 1000;                                                 
    giroscopio_y_medidido /= 1000;                                                 
    giroscopio_z_medidido /= 1000;                                                 
    Serial.begin(115200);
    loop_contador = micros(); // reinicia o loop
}

void loop(){
    read_mpu_6050_data(); // le os dados coletados do MPU-6050
    
    // subtraindo as variacoes dos valores preliminares obtidos com o giroscopio
    giroscopio_x -= giroscopio_x_medidido;
    giroscopio_y -= giroscopio_y_medidido;
    giroscopio_z -= giroscopio_z_medidido;
    
    // calculando os angulos obtidos atraves do giroscopio
    // obs.: 0.0000611 = 1/(250Hz x 65.5)
    angulo_inclinacao += giroscopio_x * 0.0000611;  // soma o angulo de inclinacao
    angulo_rolagem += giroscopio_y * 0.0000611; // soma o angulo de rolagem
    
    // obs.: 0.000001066 = 0.0000611*(3.142/180deg)
    angulo_inclinacao += angulo_rolagem * sin(giroscopio_z * 0.000001066); // se houve variacao em relacao ao angulo de guinada (eixo z), transfere o angulo de rolagem ao angulo de inclinacao
    angulo_rolagem -= angulo_inclinacao * sin(giroscopio_z * 0.000001066); // se houve variacao em relacao ao angulo de guinada (eixo z), transfere o angulo de inclinacao ao angulo de rolagem
    
    // calculando os angulos obtidos atraves do acelerometro
    acelerometro_total_vector = sqrt((acelerometro_x*acelerometro_x)+(acelerometro_y*acelerometro_y)+(acelerometro_z*acelerometro_z)); // armazena os angulos em um vetor
    
    // calculando os angulos
    // obs.: 1 rad = 57.296 deg
    angulo_inclinacao_acelerometro = asin((float)acelerometro_y/acelerometro_total_vector)* 57.296; // calcula o angulo de inclinacao
    angulo_rolagem_acelerometro = asin((float)acelerometro_x/acelerometro_total_vector)* -57.296; // calcula o angulo de rolagem
    angulo_inclinacao_acelerometro -= 0.0; //calibrando o acelerometro para o angulo de inclinacao
    angulo_rolagem_acelerometro -= 0.0; // calibrando o acelerometro para o angulo de rolagem
    if(set_gyro_angles){
        angulo_inclinacao = angulo_inclinacao * 0.9996 + angulo_inclinacao_acelerometro * 0.0004; // corrige o angulo de inclinacao do giroscopio com o do acelerometro
        angulo_rolagem = angulo_rolagem * 0.9996 + angulo_rolagem_acelerometro * 0.0004; // corrige o angulo de rolagem do giroscopio com o do acelerometro
    }
    else{
        angulo_inclinacao = angulo_inclinacao_acelerometro; // iguala o angulo de inclinacao do girscopio ao obtido pelo acelerometro 
        angulo_rolagem = angulo_rolagem_acelerometro; // iguala o angulo de rolagem do girscopio ao obtido pelo acelerometro 
        set_gyro_angles = true;
    }
    
    // reduzindo os errros de medicao dos angulos
    angulo_inclinacao_output = angulo_inclinacao_output*0.9 + angulo_inclinacao*0.1; // soma 90% do angulo de inclinacao da saida a 10% do preliminar
    angulo_rolagem_output = angulo_rolagem_output*0.9 + angulo_rolagem*0.1; // soma 90% do angulo de rolagem da saida a 10% do preliminar
    Serial.print("Inclinacao  = %.2f", angulo_inclinacao_output);
    
    while(micros()-loop_contador<4000){ // eh executado ate o contador loop_contador atingir 4000us (250Hz)
        loop_contador = micros(); // reinicia o loop
    }
}

void setup_mpu_6050_registers(){

    // ativando o modulo MPU-6050
    Wire.beginTransmission(0x68); // inicia a comunicacao com o modulo MPU-6050
    Wire.write(0x6B); // envia o registro inicial
    Wire.write(0x00); // define o registro inicial
    Wire.endTransmission();
    
    // configurando o acelerometro
    Wire.beginTransmission(0x68); // inicia a comunicacao com o modulo MPU-6050
    Wire.write(0x1C); // envia o registro inicial
    Wire.write(0x10); // define o registro inicial
    Wire.endTransmission();
    
    // configurando o giroscopio
    Wire.beginTransmission(0x68); // inicia a comunicacao com o modulo MPU-6050
    Wire.write(0x1B); // envia o registro inicial
    Wire.write(0x08); // define o registro inicial
    Wire.endTransmission();                                             
}

void read_mpu_6050_data(){ // le os dados preliminares do giroscopio e acelerometro
    Wire.beginTransmission(0x68); // inicia a comunicacao com o modulo MPU-6050
    Wire.write(0x3B); // envia o registro inicial
    Wire.endTransmission(); // termina a trasmissao
    Wire.requestFrom(0x68,14); // solicita 14 bytes do modulo MPU-6050
    while(Wire.available()<14){ // espera todos os bytes serem recebidos
        acelerometro_x = Wire.read()<<8|Wire.read();                                  
        acelerometro_y = Wire.read()<<8|Wire.read();                                  
        acelerometro_z = Wire.read()<<8|Wire.read();                                  
        temp = Wire.read()<<8|Wire.read();                                   
        giroscopio_x = Wire.read()<<8|Wire.read();                                 
        giroscopio_y = Wire.read()<<8|Wire.read();                                 
        giroscopio_z = Wire.read()<<8|Wire.read();
    }
}