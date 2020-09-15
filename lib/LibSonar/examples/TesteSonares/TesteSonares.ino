/*
    Código de exemplo para a biblioteca LibSonar
    
    A medição de distância com o sensor ultrassônico é simples.
    Basta criar um objeto da classe sonar com um dos dois construtores disponíveis, nos 
    quais se define os pinos em que o sonar está conectado e, opcionalmente, um tempo de 
    time out para o sensor diferente do padrão (equivalente a 5m).
    
    A função medeDistancia retorna a distância entre o sensor e algum obstáculo, ou a 
    distância equivalente ao tempo de time out. Esta função retorna um valor do tipo
    long, portanto, caso deseje atribuir o resultado desta função a uma variável, atente
    para o tipo dela.
    
    Neste código os objetos da classe sonar são criado utilizando o tempo de time out
    padrão. Caso seja necessário utilizar outro valor para este tempo, deve-se criar
    os objetos com o seguinte construtor:
        Sonar::Sonar(int _TRIG, int _ECHO, long _TOUT)
    Ou seja, adcionar uma variável do tipo long especificando o tempo de time out desejado.
    
*/

#include <LibSonar.h>

  //pinos dos sonares
#define ECHOF 7   //Echo Sonar da Frente
#define TRIGF 8   //Trig Sonar da Frente
#define ECHOR 12  //Echo Sonar da Direita
#define TRIGR 13  //Trig Sonar da Direita
#define ECHOL A5  //Echo Sonar da Esquerda
#define TRIGL A4  //Trig Sonar da Esquerda

  //criando objetos da classe sonar
Sonar sonarLeft(TRIGL,ECHOL);
Sonar sonarRight(TRIGR,ECHOR);
Sonar sonarFront(TRIGF,ECHOF);
  // o tempo de time out default é de 3000us (equivalente a 5m)
  

void setup() {
  // Configurando a comunicação serial, usada para verificar as distâncias
  Serial.begin(9600);
}

void loop() {
  
    //Distância medida pelo sensor da esquerda
    Serial.print(" Esquerda: ");
    Serial.print(sonarLeft.medeDistancia()); 
    Serial.print(" cm " );
    delay(100);
    //Distância medida pelo sensor da frente
    Serial.print(" Frente: ");
    Serial.print(sonarFront.medeDistancia()); 
    Serial.print(" cm " );
    delay(100);
    //Distância medida pelo sensor da direita
    Serial.print(" Direita: ");
    Serial.print(sonarRight.medeDistancia()); 
    Serial.println(" cm " );
    delay(100);
  

}