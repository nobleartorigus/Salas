#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server;
int inpin = 0;
//String url = "http://xdk-test-206419.appspot.com";
String url = "http://webhook.site/c3f8d8f6-5beb-4a56-a1ca-44a0d15dcf3a";

char* ssid = "Centraal-Comunidad";
char* password = "hoyemprendo";

char* host = "webhook.site";

//Function declaration
void post();

void setup() {
    pinMode(inpin, INPUT);
    WiFi.begin(ssid,password);
    Serial.begin(115200);
    while(WiFi.status()!=WL_CONNECTED){
        Serial.print(".");
        delay(500);
        }
        Serial.print("\r\n");
        Serial.print("IP Address\r\n");
        Serial.print(WiFi.localIP());


        server.on("/",[](){server.send(200,"text/plain","Hola prro >:v");});
        
        server.begin();
        

}

void loop() {
    server.handleClient();
    if(digitalRead(inpin) == 1){
    post();
    delay(3000);
    }
    else {
      Serial.println("No hay presencia\r\n");
    }

    delay(2000);
    
}

void post() {
  Serial.print("Connecting to: ");
    Serial.print(host);

    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
    }

    Serial.println("Requesting URL: ");
    Serial.println(url);
    delay(1000);
    Serial.print("POST " + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Content-Length: 60\r\n" +
                 "Content-Type: application/json\r\n\r\n" +
                 "{\n\"PIR5482\":1 ,\r\n"+
                 "\"SALAID\":3 ,\r\n" +
                 "\"msg\":fuerademipantano >:v\r\n}\r\n"); 

    client.print("POST " + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Content-Length: 60\r\n" +
                 "Content-Type: application/json\r\n\r\n" +
                 "{\n\"PIR5482\":1 ,\r\n"+
                 "\"SALAID\":3 ,\r\n" +
                 "\"msg\":fuerademipantano >:v\r\n}\r\n"); 
    
    

    unsigned long timeout = millis();
    while(client.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println(">>> Client Timeout");
        client.stop();
        return;
      }
    }
    while(client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print("\r\n"+line);
    }

    Serial.println();
    Serial.println("closing connection");
    client.stop();
}
