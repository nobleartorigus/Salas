#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server;
int inpin = 0;
//String url = "http://xdk-test-206419.appspot.com";
//String url = "https://webhook.site/2f4e232e-b751-45fb-b045-aa8cc35e3f44";
String url = "/notifications/sensorEndpoint/"; 

char* ssid = "Centraal-Comunidad";
char* password = "hoyemprendo";

char* host = "35.161.143.162";


const char index_html[] PROGMEM = {"<!DOCTYPE html>\n<html>\n<body>\n\n<h1>Obligame Prro >:v</h1>\n<p>Me obligo el prro >':v.</p>\n\n<form>\n\n<h1> Configuration Parameters </h1>\n\nRed: <br> \n<input type = \"text\" value = \"Centraal-Comunidad\">\n<br><br>\nPassword:<br> \n<input type = \"password\" value = \"hoyemprendo\">\n<br><br>\nURL: <br>\n<input type=\"url\">\n<br><br>\nHost: <br>\n<input type=\"text\">\n<br><br>\n<input type = \"submit\" value = \"Submit\">\n<input type = \"reset\" value = \"Reset\">\n\n</form>\n\n</body>\n</html>"};

//Function declaration
//void post(char status);
void get();
void config();

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
        server.on("/config", config);

        server.begin();
        

}

void loop() {
    server.handleClient();
    if(digitalRead(inpin) == 1){
    post(1);
    delay(3000);
    //get();
    delay(1000);
    }
    else {
      Serial.println("No hay presencia\r\n");
      post(0);
    }

    delay(2000);
    
}

void post(int status) {
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
                 "Content-Length: 61\r\n" +
                 "Content-Type: application/json\r\n\r\n" +
                 "{\"occupied\":" + status + ", \"room_name\":\"Procope\", \"sensor_name\":\"prueba\"}\r\n");;

    client.print("POST " + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Content-Length: 61\r\n" +
                 "Content-Type: application/json\r\n\r\n" +
                 "{\"occupied\":" + status + ", \"room_name\":\"Procope\", \"sensor_name\":\"prueba\"}");
    
    

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

void get() {
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
    Serial.print("GET " + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 //"Content-Length: 68\r\n" +
                 "Content-Type: application/json\r\n\r\n");

    client.print("GET " + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 //"Content-Length: 68\r\n" +
                 "Content-Type: application/json\r\n\r\n");
                 
    
    

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

void config() {

  server.send_P(200, "text/html", index_html);
}
