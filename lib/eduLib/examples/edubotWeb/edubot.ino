
void colisao(){
    if(distF<7){
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
    
        // ...e volta para o loop normal ou até que haja colisao
    }else
    if(digitalRead(FCFD)==LOW || digitalRead(FCFE)==LOW){
        edu_para(); // caso haja colisao, só desiste (para)
    }  
}

void medeDistancias(){
    distR = sonarRight.medeDistancia();
    distL = sonarLeft.medeDistancia();
    distF = sonarFront.medeDistancia();
}

void mandaDistanciasSerial(){
    Serial.print("Distancia Direita: ");Serial.println(distR);
    Serial.print("Distancia Esquerda: ");Serial.println(distL);
    Serial.print("Distancia Frente: ");Serial.println(distF);
}
