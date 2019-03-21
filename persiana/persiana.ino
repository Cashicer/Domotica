/*
   Control de la persiana de una habitacion mediante 3 pulsadores
   v1.0 Implementado servidor web
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

int estado = 0;

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

  if (digitalRead(botUp) == LOW) {
    while (digitalRead(botUp) == LOW) {}
    estado = 2;
  }
  else if (digitalRead(botDown) == LOW) {
    while (digitalRead(botDown) == LOW) {}
    estado = 3;
  }
  else if (digitalRead(botStop) == LOW) {
    while (digitalRead(botStop) == LOW) {}
    estado = 1;
  }

  // servidor web

  WiFiClient client = server.available();
  if (client) {

    Serial.println("new client");
    while (!client.available())
      delay(1);

    String req = client.readStringUntil('\r');
    Serial.println(req);
    client.flush();

    int val;
    String s = "HTTP/1.1 200 OK\r\nContent-Type: text/json\r\n\r\n{\"status\":\"";
    if ( req.indexOf("/gpio/stop") != -1)
    {
      estado = 1;
      s += "Parada\"";
    }
    else if (req.indexOf("/gpio/up") != -1)
    {
      estado = 2;
      s += "Subiendo\"";
    }
    else if (req.indexOf("/gpio/down") != -1)
    {
      estado = 3;
      s += "Bajando\"";
    }
    else
    { Serial.println("invalid request");
      estado = 1;
      client.stop();
      return;
    }
    s += "}";
    client.print(s);                    // Enviar el resultado de val al cliente
    delay(1);
    Serial.println("Client disonnected");
  }

  if (estado == 2) {
    digitalWrite(relDown, HIGH);
    digitalWrite(relUp, LOW);
  }
  else if (estado == 3) {
    digitalWrite(relUp, HIGH);
    digitalWrite(relDown, LOW);
  }
  else if (estado == 1) {
    digitalWrite(relUp, HIGH);
    digitalWrite(relDown, HIGH);
  }
}
