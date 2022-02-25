#include <DHT.h>
#include <DHT_U.h>

#include <WiFi.h>
#include <WiFiUdp.h>

#define pinDHT 26
#define typDHT11 DHT11 
DHT mojeDHT(pinDHT, typDHT11);

// WiFi network name and password:
const char * networkName = "Vincent1";
const char * networkPswd = "Stepanek11";

//IP address to send UDP data to:
const char * udpAddress = "192.168.2.89";
const int udpPort = 3333;
boolean connected = false;
WiFiUDP udp;

void setup(){
  Serial.begin(115200);
  mojeDHT.begin();
  pinMode(pinDHT, INPUT_PULLUP);
  connectToWiFi(networkName, networkPswd);
}

void loop(){
  float tep = mojeDHT.readTemperature();
  String teplota = String(tep, 1);
  Serial.println(tep);
  if(connected){
    udp.beginPacket(udpAddress,udpPort);    
    udp.print(teplota);
    udp.endPacket();
  }
  
  //Wait for 1 second
  delay(1000);
}

void connectToWiFi(const char * ssid, const char * pwd){
  Serial.println("Connecting to WiFi network: " + String(ssid));
  // delete old config
  WiFi.disconnect(true);
  //register event handler
  WiFi.onEvent(WiFiEvent);  
  //Initiate connection
  WiFi.begin(ssid, pwd);
  Serial.println("Waiting for WIFI connection...");
}

void WiFiEvent(WiFiEvent_t event){
    switch(event) {
      case SYSTEM_EVENT_STA_GOT_IP:
          //When connected set 
          Serial.print("WiFi connected! IP address: ");
          Serial.println(WiFi.localIP());  
          //initializes the UDP state
          //This initializes the transfer buffer
          udp.begin(WiFi.localIP(),udpPort);
          connected = true;
          break;
      case SYSTEM_EVENT_STA_DISCONNECTED:
          Serial.println("WiFi lost connection");
          connected = false;
          break;
      default: break;
    }
}
