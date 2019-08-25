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

/*const char* ssid = "OpenWrt";
const char* password = "E64680C9E8220";*/

/*const char* ssid = "RT-N13E_B";
const char* password = "NidGewTcDLUMHjGNByjn";*/

// Definicion de variables y pines asociados
int botUp = 4;    //D2
int botDown = 16; //D0
int botStop = 5;  //D1
int relUp = 12;   //D6
int relDown = 13; //D7

int estado = 1;
int prev = 1;
int timeout;

WiFiServer server(80);

void setup() {
  // put your setup code here, to run once:
  /*pinMode(botUp, INPUT);
  pinMode(botDown, INPUT);
  pinMode(botStop, INPUT);*/
  pinMode(relUp, OUTPUT);
  pinMode(relDown, OUTPUT);
  digitalWrite(relUp, HIGH);
  digitalWrite(relDown, HIGH);

  Serial.begin(115200);
  delay(1500);

  connectWiFi();
}

void loop() {

  //leerBotones();
  //paradaEmergencia();
  leerPeticionWeb();
  ejecutarEstado();
  //if(WiFi.status() != WL_CONNECTED) connectWiFi();
  /*if(estado != prev){
    Serial.println(estado);
    prev = estado;
  }*/
}

void connectWiFi() {
  Serial.print("Connecting to: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  server.begin();                                // Iniciamos el servidor
  Serial.println("Server started");
  Serial.println(WiFi.localIP());      // Imprimimos la IP
}

void leerBotones() {
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
}

void paradaEmergencia(){
  if (digitalRead(botStop) == LOW) {
    while (digitalRead(botStop) == LOW) {}
    estado = 1;
  }
}

void leerPeticionWeb() {
  // servidor web

  WiFiClient client = server.available();
  if (!client) return;

  Serial.println("new client");
  while (!client.available()){
    delay(1);
  }   

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
    timeout = millis();
    s += "Subiendo\"";
  }
  else if (req.indexOf("/gpio/down") != -1)
  {
    estado = 3;
    timeout = millis();
    s += "Bajando\"";
  }
  else if (req.indexOf("/favicon.ico") != -1)
  {
    Serial.println("invalid request");
    client.stop();
    return;
  }
  else if (req.indexOf("/gpio/status") != -1)
  {
    switch(estado){
    case 1: 
       s += "Parada\"";
    break;
    case 2: 
       s += "Subiendo\"";
    break;
    case 3: 
       s += "Bajando\"";
    break;
    default: 
       s += "Invalid status\"";  
    break;
    }  
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

void ejecutarEstado(){
  if (estado == 2) {
    digitalWrite(relDown, HIGH);
    digitalWrite(relUp, LOW);
    if(millis() - timeout > 40000) estado = 1;
  }
  else if (estado == 3) {
    digitalWrite(relUp, HIGH);
    digitalWrite(relDown, LOW);
    if(millis() - timeout > 40000) estado = 1;
  }
  else if (estado == 1) {
    digitalWrite(relUp, HIGH);
    digitalWrite(relDown, HIGH);
  }
}
