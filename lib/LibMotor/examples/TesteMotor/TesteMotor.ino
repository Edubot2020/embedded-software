#include "LibMotor.h"
#include <Encoder.h>

#define CHAD 2
#define CHBD 12
#define CHAE 3
#define CHBE 13
Encoder knobLeft(CHAE, CHBE);
Encoder knobRight(CHAD, CHBD);

#define IN1 5 
#define IN2 6
#define IN3 10
#define IN4 9

#define maxMvolt 6
#define maxBvolt 6

Mdrive mDireita(IN1,IN2);
Mdrive mEsquerda(IN3,IN4);

long positionLeft  = -999;
long positionRight = -999;

void setup() {
  Serial.begin(9600);
  mDireita.init(maxMvolt,maxBvolt);
  mEsquerda.init(maxMvolt,maxBvolt);  
}

void loop() {
  mEsquerda.setVoltage(6);
  mDireita.setVoltage(6);
  delay(1000);
  long newLeft, newRight;
  newLeft = knobLeft.read();
  newRight = knobRight.read();
  if (newLeft != positionLeft || newRight != positionRight) {
    Serial.print("Left = ");
    Serial.print(newLeft);
    Serial.print(", Right = ");
    Serial.print(newRight);
    Serial.println();
    positionLeft = newLeft;
    positionRight = newRight;
  }
  
}
