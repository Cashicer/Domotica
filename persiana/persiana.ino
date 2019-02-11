/*
   Control de la persiana de una habitacion mediante 3 pulsadores
   Author: Javier Gallego Carracedo
   11 Febrero 2019
*/

// Definicion de variables y pines asociados
int botUp;
int botDown;
int botStop;
int relUp;
int relDown;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(botUp) != 1){
    digitalWrite(relDown, 0);
    digitalWrite(relUp, 1);
  }
  else if (digitalRead(botDown) != 1){
    digitalWrite(relUp, 0);
    digitalWrite(relDown, 1);
  }
  else if (digitalRead(botStop) != 1){
    digitalWrite(relUp, 0);
    digitalWrite(relDown, 0);
  }
}
