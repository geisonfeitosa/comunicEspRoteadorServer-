#include <WiFi.h>

#define PINLED01 18 //Cliente 01
#define MAXSC 1

const char* ssid      = "LIVE TIM_41F9_2G";
const char* password  = "2pxtrvv3hd";

String message;
WiFiServer server(80);
WiFiClient clientArray[MAXSC];
IPAddress ip(192,168,25,240);
IPAddress gateway(192,168,25,1);
IPAddress subnet(255,255,255,0);

void setup() {
  Serial.begin(115200);
  
  pinMode(PINLED01, OUTPUT);

  WiFi.config(ip, gateway,subnet);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  AvailableClient();
  AvailableMessage();
}

void AvailableClient() {
  if(server.hasClient()) {
    Serial.println("Tem conectado");
    for(uint8_t i=0; i < MAXSC; i++) {
      if(!clientArray[i] || !clientArray[i].connected()) {
        if(clientArray[i]) {
          clientArray[i].stop();
        }
        if(clientArray[i] = server.available()) {
          Serial.println("New client: " + String(i+1));
        }
        continue;
      }
    }
    WiFiClient cliente = server.available();
    cliente.stop();
  }
}

void AvailableMessage() {
  for(uint8_t i=0; i < MAXSC; i++) {
    if(clientArray[i] && clientArray[i].connected() && clientArray[i].available()) {
      while(clientArray[i].available()) {
        message = clientArray[i].readStringUntil('\r');
        clientArray[i].flush();
        Serial.println("Client " + String(i+1) + " - " + message);
        ClientNumber();
      }
    }
  }
}

void ClientNumber() {
  if(message == "<Cliente 01-1>") {
    digitalWrite(PINLED01, HIGH);
  } else if(message == "<Cliente 01-0>") {
    digitalWrite(PINLED01, LOW);
  }
}
