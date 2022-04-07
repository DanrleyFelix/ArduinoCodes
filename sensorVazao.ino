// Danrley Santos Felix
// Atividade - Sensor de vazão
// O sinal PWM irá simular o sensor YF-S401

#include "protocolo.h"

#define e0 2                        
#define s0 3                       

void conta_pulsos();
void obter_vazao();
void checar_serial();

double tempo;
double tempoAnterior;
float timeout;
float timeoutAnterior=0;
float valorVazao=0.0;
float frequencia=1.0;
float periodoTotal;
int contaPeriodos;
int contaPulsos;
int checando=0;
char valorSerial;
char primeiroValor='0';

void setup() {

  Serial.begin(115200);
  pinMode(s0, OUTPUT);
  pinMode(e0, INPUT);
  attachInterrupt(digitalPinToInterrupt(e0),conta_pulsos, RISING);
  analogWrite(s0,maximo);
  
}

void loop(){
  checar_protocolo(valorSerial, primeiroValor, checando, s0);
  if (contaPeriodos>=100)
    calcula_frequencia();
  timeout=millis();
}

void obter_vazao(void){
  if (timeout-timeoutAnterior>=50)
    valorVazao=0;
  else{
    calcula_frequencia();
    valorVazao=1000*frequencia/98;
  }
  Serial.print("Vazao (ml/min): ");
  Serial.println(valorVazao);
}

void conta_pulsos(void){
  contaPulsos++;
  timeoutAnterior=timeout;
  if (contaPulsos==2){
    tempo=micros();
    contaPulsos=0;
    periodoTotal=periodoTotal+(tempo-tempoAnterior);
    contaPeriodos++;
  }
  else
    tempoAnterior=micros();
}

void calcula_frequencia(void){
  frequencia=1000000/(periodoTotal/contaPeriodos);
  contaPeriodos=0;
  periodoTotal=0;
}