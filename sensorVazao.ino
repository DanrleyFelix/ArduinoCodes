#define e0 2                       
#define s0 3                        
#define maximo 128

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
  pinMode(s0,OUTPUT);
  pinMode(e0,INPUT);
  attachInterrupt(digitalPinToInterrupt(e0),conta_pulsos,RISING);
  analogWrite(s0,maximo);
}

void loop(){
  checar_serial();
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

void checar_serial(void){
  char comando[3]={'R','F','V'};
  if (Serial.available()>0){
    valorSerial=Serial.read();
    if (valorSerial=='b')
      analogWrite(s0,0);
    else if (valorSerial=='a')
      analogWrite(s0,maximo);
    // Iniciar protocolo
    if (valorSerial=='T' and checando==0){
      primeiroValor=valorSerial;
      checando++;
    }
    else if(valorSerial=='T' and checando>0 and primeiroValor=='T')
      primeiroValor=='T';
    else if(checando>0 and primeiroValor=='T' and comando[checando-1]!=valorSerial){
      primeiroValor=='0';
      checando=0;
    }
    else if (comando[checando-1]==valorSerial and primeiroValor=='T'){
      checando++;
      if (checando==4){
        obter_vazao();
        checando=0;
        primeiroValor='0';
      }
    }
    else
      checando=0;
  }
}