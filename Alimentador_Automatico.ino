//========================================================================================
//********** ALIMENTADOR AUTOMÁTICO ******************************************************
//========================================================================================


#include <Servo.h>
#include <TimerOne.h>


Servo myservo;

  const byte botao1 = 4;
  const byte botao2 = 5;
  const byte redLed = 7;
  const byte greenLed = 8;
  const byte buzzer = 9;
  const byte pinSom = 12;
  const byte led1 = A3;
  const byte led2 = A1;
  const byte led3 = A0;
  const byte led4 = A4;
  const byte led5 = A5;
    
int pos=0;
int doorclose = 177; //grau da porta totalmente fechada
int dooropen = 80; //grau da porta totalmente aberta

int led = 0;
int pisca = 0;
int contaBotao = 0;

volatile int contaPalmas = 0;
static unsigned long tempoEntrePalmas = 0;
static unsigned long tempoAnterior = 0;
int tempoMaxDeUmaPalma = 300;
int tempoMaxEntrePalmas = 1000;

int stateLedContaBotao[] = {0, 255};

void setup() {
  
  Serial.begin(9600);

  myservo.attach(11);
  
  pinMode(greenLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(botao1, INPUT_PULLUP);
  pinMode(botao2, INPUT_PULLUP);
  pinMode(pinSom, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);

  int led[] = {led1, led2, led3, led4, led5}; //Não precisa declarar o tamanho do vetor
  myservo.write(doorclose);delay(1000);

  porquinhoFeliz();

  for(int a=0; a<=4; a++){
    analogWrite(led[a], 255);
    delay(300);
    analogWrite(led[a], 0);
  }
  Serial.println("Arduino pronto para uso!");
  
}

void loop() {
  
  if(digitalRead(pinSom) == LOW){
    digitalWrite(redLed, HIGH);
    if(tempoAnterior == 0){
      contaPalmas++;
      tempoAnterior = tempoEntrePalmas = millis();
    } else if((millis()-tempoAnterior) >= tempoMaxDeUmaPalma){
      tempoAnterior = 0;
    }
  }
  
  if(contaPalmas!=0 &&((millis() - tempoEntrePalmas) >= tempoMaxEntrePalmas)){
    Serial.print("Foram ");Serial.print(contaPalmas);Serial.println(" palma(s)!");
    executarAcao();
    delay(1000);
    contaPalmas = 0;
    tempoEntrePalmas = millis();
  }
  
  if(digitalRead(botao1) == LOW){
    contarBotao();
    delay(250);
  }
  
  if(digitalRead(botao2) == LOW){
    executar();
  }
  ledsContarBotao();
}

//================================
//Decide o que o arduino vai fazer
//================================

void executarAcao(){
  
  digitalWrite(redLed, LOW);
  switch (contaPalmas){

    case 1:
        for(led=0; led<1; led+=1){
          digitalWrite(greenLed, HIGH);
          delay(500);
          digitalWrite(greenLed, LOW);
          delay(250);
        }
        break;
        
    case 2:
        for(led=0; led<2; led+=1){
          digitalWrite(greenLed, HIGH);
          delay(500);
          digitalWrite(greenLed, LOW);
          delay(250);
        }
    break;
    
    case 3:
        abrirFecharPorta();
    break;
  }
  
}

//==============================================
//Executa a movimentação da porta do alimentador
//==============================================

void abrirFecharPorta(){

    //==========================
    //Indica a abertura da Porta
    //==========================
   
    porquinhoFeliz();
    
    for(led=0; led<3; led+=1){
      digitalWrite(greenLed, HIGH);
      delay(500);
      digitalWrite(greenLed, LOW);
      delay(250);
    }

    //============
    //Abre a porta
    //============
    
    for(pos=doorclose; pos>dooropen; pos-=1){
      myservo.write(pos);
      delay(15);
    }

    //==========================
    //Indica que a porta fechará
    //==========================
    
    for(led=0; led<2; led+=1){
      digitalWrite(redLed, HIGH);
      delay(500);
      digitalWrite(redLed, LOW);
      delay(250);
    }

    //=============
    //Fecha a porta
    //=============
    
    for(pos=dooropen; pos<doorclose; pos+=1){
      myservo.write(pos);
      delay(25);
    }
  delay(1000);
}

//===========================================================
//Contabiliza o número de vezes que o botão 1 foi pressionado
//===========================================================

void contarBotao(){

  digitalWrite(greenLed, HIGH);
  tone(buzzer, 1500);
  contaBotao++;
  if(contaBotao == 14){
    contaBotao = 0;
  }
  Serial.println(contaBotao);
  delay(250);
  noTone(buzzer);
  digitalWrite(greenLed, LOW);
}

//===============================================================
//Executa o timer configurado após o botão 2 ter sido pressionado
//===============================================================

void executar(){

  for(int i = 0; i < contaBotao*60; i++){
    for(int j = 0; j < 12; j++){
      digitalWrite(redLed, HIGH);
      delay(500);
      digitalWrite(redLed, LOW);
      delay(4500);
    }
  }
  contaBotao = 0;
  digitalWrite(greenLed, HIGH);
  abrirFecharPorta();
  digitalWrite(greenLed, LOW);
}

//===============================================================
//Demonstra o timer em Horas=====================================
//===============================================================

void ledsContarBotao(){
  static long unsigned delayLed;
  switch (contaBotao){
    case 0:
      analogWrite(led1, 0);
      analogWrite(led2, 0);
      analogWrite(led3, 0);
      analogWrite(led4, 0);
      analogWrite(led5, 0);
    break;
    case 1:
      analogWrite(led1, 255);
    break;
    case 2:
      analogWrite(led2, 255);
    break;
    case 3:
      analogWrite(led3, 255);
    break;
    case 4:
      if((millis()-delayLed)<500){
        analogWrite(led1, 255);
      }else{
        analogWrite(led1, 0);
      }
      if((millis()-delayLed)>1000){
        delayLed = millis();
      }
      analogWrite(led4, 255);
      analogWrite(led2, 0);
      analogWrite(led3, 0);
    break;
    case 5:
      analogWrite(led1, 0);  
      analogWrite(led4, 255);
    break;
    case 6:
      analogWrite(led1, 255);
    break;
    case 7:
      analogWrite(led2, 255);
    break;
    case 8:
      analogWrite(led3, 255);
    break;
    case 9:
      analogWrite(led5, 255);
      if((millis()-delayLed)<500){
        analogWrite(led1, 255);
      }else{
        analogWrite(led1, 0);
      }
      if((millis()-delayLed)>1000){
        delayLed = millis();
      }
      analogWrite(led2, 0);
      analogWrite(led3, 0);
      analogWrite(led4, 0);
    break;
    case 10:
      analogWrite(led1, 0);
    break;
    case 11:
      analogWrite(led1, 255);
    break;
    case 12:
      analogWrite(led2, 255);
    break;
    case 13:
      analogWrite(led3, 255);
    break;
  }
}
//==============================================================
//Música e efeito pirotécnico da música "Sou um Porquinho Feliz"
//==============================================================

void porquinhoFeliz(){ //Música e efeito pirotécnico da música "Sou um Porquinho Feliz"             
  
  tone(buzzer, 528);
  digitalWrite(redLed, HIGH);
  digitalWrite(greenLed, LOW);
  delay(500);
  noTone(buzzer);
  tone(buzzer, 660);
  digitalWrite(redLed, LOW);
  digitalWrite(greenLed, HIGH);
  delay(500);
  noTone(buzzer);
  tone(buzzer, 794);
  digitalWrite(redLed, HIGH);
  digitalWrite(greenLed, LOW);
  delay(250);
  noTone(buzzer);
  tone(buzzer, 794);
  digitalWrite(redLed, LOW);
  digitalWrite(greenLed, HIGH);
  delay(250);
  noTone(buzzer);
  tone(buzzer, 660);
  digitalWrite(redLed, HIGH);
  digitalWrite(greenLed, LOW);
  delay(250);
  noTone(buzzer);
  tone(buzzer, 594);
  digitalWrite(redLed, LOW);
  digitalWrite(greenLed, HIGH);
  delay(250);
  noTone(buzzer);
  tone(buzzer, 660);
  digitalWrite(redLed, HIGH);
  digitalWrite(greenLed, LOW);
  delay(1000);
  noTone(buzzer);
  digitalWrite(redLed, LOW);
  
}
