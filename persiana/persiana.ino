/*
   Control de la persiana de una habitacion mediante 3 pulsadores
   Author: Javier Gallego Carracedo
   11 Febrero 2019
*/

// Definicion de variables y pines asociados
int botUp = 7;
int botDown = 5;
int botStop = 6;
int relUp = 11;
int relDown = 12;

void setup() {
  // put your setup code here, to run once:
  pinMode(botUp, INPUT);
  pinMode(botDown, INPUT);
  pinMode(botStop, INPUT);
  pinMode(relUp, OUTPUT);
  pinMode(relDown, OUTPUT);
  digitalWrite(relUp, HIGH);
  digitalWrite(relDown, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  if (digitalRead(botUp) == LOW){
    while (digitalRead(botUp) == LOW){}
    digitalWrite(relDown, HIGH);
    digitalWrite(relUp, LOW);
  }
  else if (digitalRead(botDown) == LOW){
    while (digitalRead(botDown) == LOW){}
    digitalWrite(relUp, HIGH);
    digitalWrite(relDown, LOW);
  }
  else if (digitalRead(botStop) == LOW){
    while (digitalRead(botStop) == LOW){}
    digitalWrite(relUp, HIGH);
    digitalWrite(relDown, HIGH);
  }
  /*
  digitalWrite(relDown, LOW);
  digitalWrite(relUp, HIGH);
  delay(1000);
  digitalWrite(relUp, LOW);
  digitalWrite(relDown, HIGH);
  delay(1000);
  */
}
