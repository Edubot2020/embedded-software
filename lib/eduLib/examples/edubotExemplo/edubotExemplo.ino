
/**
 * Código de demonstração do arquivo Edubot.h
 * 
 * Para controle do Edubot, foi implementado controle de velocidade PID em ambos os motores. 
 *    Cada motor é levemente diferente um do outro - para uma mesma tensão aplicada no motor,
 *    o motor pode adquirir maior velocidade ou demorar mais para acelerar.
 *    Por isso, para que o robô ande em linha reta, deve ser feito um controlador de velocidade.
 *    Além disso, para que o robô rotacione exatos 90 graus, por exemplo, é necessário um controle de posição.
 *    (Na UFRGS, aprendemos a teoria necessária para calcular os parâmetros na disciplina Controle I)
 *
 * As funções edu_moveReto(int velocidade), edu_para() e edu_rotaciona(int angulo)
 * fazem a movimentação do robô.
 * Perceba que embora haja controle de velocidade, ainda pode ocorer "drift" devido a erros numericos, entao use os
 * fins de curso para ajuste! 
 * 
 * A rotina de exemplo mostra usos comuns dessas três funções. Velocidades negativas e ângulos negativos são aceitos.
 * 
 * Não é necessário alterar o código fonte Edubot.h!!
 */
#include <Edubot.h>

// Arduino IDE won't link these dependencies from Edubot.h, so put them here...
#include <LibMotor.h>
#include <Encoder.h>
#include <LibSonar.h>
#include <Controller.h>
//     *** Variáveis globais: ***


// Sonares utilizados pelo robô. O método medeDistancia() adquire a distancia
// Exemplo: sonarRight.medeDistancia() retorna a distancia na direita
Sonar sonarRight(TRIGR, ECHOR);
Sonar sonarFront(TRIGF, ECHOF);
Sonar sonarLeft(TRIGL, ECHOL);

// Distancias em cada direção. 
// (as distâncias são armazenadas em variaveis para nao chamar
// a função medeDistancia() repetidamente)
long distR, distF,distL;


//     *** Protótipos de função:  ***

//Adquire as distancias de cada sonar nas variaveis distR, distF, distL
void medeDistancias();
// Envia os dados de distancia para a serial (usar se for desejado)
void mandaDistanciasSerial();

//     *** Rotinas principais: ***

void setup() {
  
  Serial.begin(9600);

  // Inicializa os motores, os encoders, e os controladores,
  // assim como o timer necessário.
  // Sempre chamar edu_setup() no setup!
  edu_setup();
  
}


void loop() {

  // Adquire as distâncias dos sonares
  medeDistancias();

  // edu_moveReto(velocidade), faz o robo andar "reto" com a velocidade desejada.
  // VMAX é definido na biblioteca.
  // Botar multiplos menores para diminuir a velocidade, e.x: 0.5*EDU_VMAX
  edu_moveReto(0.8*EDU_VMAX);

  // Robo anda reto até que a distância à frente seja menor que 7 cm...
  if(distF<7)
  {
    // Manda parar e espera um momento para garantir que o robô parou
    edu_para();
    delay(500);

    // Move com a velocidade máxima no sentido oposto (para tras)
    // por 500 ms...
    edu_moveReto(-EDU_VMAX);
    delay(500);

    // para...
    edu_para();
    delay(500);
    
    // rotaciona 180 graus positivos..
    edu_rotaciona(180);
    // rotaciona 90 graus negativos..
    edu_rotaciona(-90);

    // ...e volta para o loop normal
  }
  // ou até que haja colisao
  else if(digitalRead(FCFD)==LOW || digitalRead(FCFE)==LOW)
  {
    edu_para(); // caso haja colisao, só desiste (para)
  }  
}

// *** Definições das funções: ***
void medeDistancias()
{
  distR = sonarRight.medeDistancia();
  distL = sonarLeft.medeDistancia();
  distF = sonarFront.medeDistancia();
}

void mandaDistanciasSerial()
{
  Serial.print("Distancia Direita: ");Serial.println(distR);
  Serial.print("Distancia Esquerda: ");Serial.println(distL);
  Serial.print("Distancia Frente: ");Serial.println(distF);
}
