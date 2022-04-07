void checar_protocolo(char valorSerial, char primeiroValor, int checando, int s0){
  
  int maximo = 128;
  char comando[3]={'R','F','V'};
  if (Serial.available()>0){
    valorSerial=Serial.read();
    if (valorSerial=='b')
      analogWrite(s0, 0);
    else if (valorSerial=='a')
      analogWrite(s0, maximo);
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