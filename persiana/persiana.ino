/*
   Control de la persiana de una habitacion mediante 3 pulsadores
   Author: Javier Gallego Carracedo
   11 Febrero 2019
*/

#include <ESP8266WiFi.h>

// Credenciales WiFi
const char* ssid = "MIWIFI_2G_7TVY";
const char* password = "Tw7ddkMk";

// Definicion de variables y pines asociados
int botUp = 4;    //D2
int botDown = 16; //D0
int botStop = 5;  //D1
int relUp = 13;   //D7
int relDown = 12; //D6

WiFiServer server(80);

void setup() {
  // put your setup code here, to run once:
  pinMode(botUp, INPUT);
  pinMode(botDown, INPUT);
  pinMode(botStop, INPUT);
  pinMode(relUp, OUTPUT);
  pinMode(relDown, OUTPUT);
  digitalWrite(relUp, HIGH);
  digitalWrite(relDown, HIGH);

  Serial.begin(115200);
  delay(1500);

  Serial.print("Connecting to: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println(WiFi.localIP());

  server.begin();                                // Iniciamos el servidor
  Serial.println("Server started");
  Serial.println(WiFi.localIP());      // Imprimimos la IP
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

  // servidor web

  WiFiClient client = server.available();
  if (!client) return;

  Serial.println("new client");
  while(!client.available())
        delay(1);

   String req = client.readStringUntil('\r');
   Serial.println(req);
   client.flush();

   int val;
   String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n";
   if ( req.indexOf("/gpio/0") != -1)
   {
     digitalWrite(relUp, HIGH);
     digitalWrite(relDown, HIGH);
     s+="Parada";
   }
   else if (req.indexOf("/gpio/1") != -1)
   {
     digitalWrite(relDown, HIGH);
     digitalWrite(relUp, LOW);
     s+="Subiendo";
   }
   else if (req.indexOf("/gpio/-1") != -1)
   {
     digitalWrite(relUp, HIGH);
     digitalWrite(relDown, LOW);
     s+="Bajando";
   }
   else
   { Serial.println("invalid request");
     client.stop();
     return;
   }
   s += "</html>\n";
   client.print(s);                    // Enviar el resultado de val al cliente
   delay(1);
   Serial.println("Client disonnected"); 
}
