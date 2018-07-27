#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <EEPROM.h>



//---------------------------------GLOBAL VARIABLES--------
const char *ssid = ">:v";
const char *password = "";

char mySSID[50];
char myPASS[50];

String url = "/notifications/sensorEndpoint/"; 
//String url = "http://webhook.site/629ff139-09de-4edd-8628-f8b598d3ffc9";

char* host = "35.161.143.162";
//char* host = "webhook.site";

int PIR = 0;
int APMode = 2;

unsigned long previousMillis;
unsigned long currentMillis;
int interval = 1000;
int minutes, secs;
int time_total_1_counted = 0;
int time_total_0_counted = 0;

ESP8266WebServer server(80);
WiFiClient espClient;

//---------------------------------------------HTML------------

String index_html = {"<!DOCTYPE html>\n"
"<html>\n"
"<body>\n"
"\n"
"<form>\n"
"\n"
"<h1> Configuration Parameters </h1>\n"
"\n"
"</body>\n"
"</html>\n"
"\n"
"\n"
"<form action= \"save_config\" method = \"get\">\n"
"\n"
"SSID: <br> \n"
"<input class = \"input\" name = \"mySSID\" type = \"text\" value = \"Centraal-Comunidad\">\n"
"<br><br>\n"
"Password:<br> \n"
"<input class = \"input\" name = \"myPASS\" type = \"password\" value = \"hoyemprendo\">\n"
"<br><br>\n"
"<input type = \"submit\" value = \"Save\">\n"
"<input type = \"reset\" value = \"Reset\"><br><br>\n"
"\n"
"</form>\n"
"URL: <br>\n"
"<input type=\"url\">\n"
"<br><br>\n"
"Host: <br>\n"
"<input type=\"text\">\n"
"<br><br>\n"
"\n"
"<a href = \"scan\"><button class = \"boton\"> Scan</button></a>\n"
"\n"
"\n"
"</body>"};
String msg = "";

//---------------------------------------------FUNCTIONS-----------------------------
void post();
void scan();
void read();
void config_page();
void config_mode();
void save_config();
void record();
void setup_WiFi();


//---------------------------------------------MAIN-------------------------------

void setup() {

  currentMillis = 0;
  previousMillis = 0;

  pinMode(PIR, INPUT);
  pinMode(APMode, INPUT);
  
	delay(1000);
	Serial.begin(115200);
  EEPROM.begin(4096);

	Serial.println();

  setup_WiFi();

  read(0).toCharArray(mySSID, 50);
  read(50).toCharArray(myPASS, 50);
	
}

void loop() {
  unsigned long currentMillis = millis();
	server.handleClient();
  if(digitalRead(APMode) == 1){
    config_mode();
  } 
  else { 
      if (currentMillis > millis()) {
        previousMillis = previousMillis - currentMillis + millis();
      }
      currentMillis = millis();

      if ((currentMillis - previousMillis) >= 1000) {
        previousMillis = currentMillis;
        secs++;
      }
      if (secs >= 60) {
        secs = 0;
        minutes++;
      }
      if (minutes >= 5) {
        if (digitalRead(PIR) == 1) {
          time_total_1_counted += 1;
          if (time_total_1_counted == 2) {
            post(1);
            delay(500);
            time_total_0_counted = 0;
            delay(500);
          }
        }
        else if(digitalRead(PIR) == 0) {
          time_total_0_counted += 1;
          Serial.println("No Presence\r\n");
          if (time_total_0_counted == 2) {
            post(0);
            delay(500);
            time_total_1_counted = 0;
            delay(500);
          }
        }
      minutes = 0;
        
      
      }
      
  }
  delay(1000);
  

  //---Just in case, millis() overload:----
  if ((currentMillis - previousMillis) >= interval) {
    previousMillis = currentMillis;
  }
    
}

//----------------------------------------POST METHOD---------------------------

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

//------------------------WEB PAGE-----------------------

void config_page() {

  server.send(200, "text/html", index_html + msg);
}

//----------------------Configuration Mode------------

void config_mode () {
  Serial.println("");
  Serial.print("Configuring access point...");
  
	WiFi.softAP(ssid, password);

	IPAddress myIP = WiFi.softAPIP();
	Serial.print("AP IP address: ");
	Serial.println(myIP);

  server.on("/",[](){server.send(200,"text/plain","BOSCH Presence Detection\r\nTo Enter Configuration Settings Please Enter \"config\" On Your Web Browser\r\n");});
  server.on("/config", config_page);

  server.on("/saveconfig", save_config);
  server.on("/scan", scan);

	server.begin();
	Serial.println("HTTP server started");

 

}


//----------------------------------------SAVE CONFIGURATION-----------

void save_config() {

  Serial.println(server.arg("mySSID")); //Values recieved from GET petition in the formular
  record(0, server.arg("mySSID"));
  Serial.println(server.arg("myPASS")); 
  record(50, server.arg("myPASS"));

  Serial.println("Saved configuration...");
  msg = "Saved configuration....";
  config_page();
}



//-----------------------Record in the EEPROM----------

void record(int addr, String a) {

  int size = a.length();
  char inchar[50];
  a.toCharArray(inchar, size+1);
  for(int i = 0; i < size; i++) {
    EEPROM.write(addr+i, inchar[i]);
  } 
  for(int i = addr; i < 50; i++){
    EEPROM.write(addr+i, 255);
  }
  EEPROM.commit();
  Serial.println("done prro >:v");
}


//-----------------------Read in the EEPROM--------------

String read(int addr) {
  byte read_l;
  String strread;
  for (int i = addr; i < addr+50; i++) {
    read_l = EEPROM.read(i);
    if (read_l != 255) {
      strread += (char)read_l;
    }
  }
  return strread;
}



//----------------------------SCAN------------------------

void scan() {
  int n = WiFi.scanNetworks(); //return no. of scanned networks
  Serial.println("Scan finished");
  if (n == 0) {
    Serial.println("No Networks Found");
    msg = "No Networks Found";
  }
  else {
    Serial.print(n);
    Serial.println("Networks Found"); 
    msg = "";
    for (int i = 0; i < n; i++) {
      //Add to String "msg" network found info
      msg = (msg) + "<p>" + String(i+1) + ": " + WiFi.SSID(i) + " (" + WiFi.RSSI(i) + ") Ch:" +
      WiFi.channel(i) + " EncryptionType: " + WiFi.encryptionType(i) + "</p\r\n";
      delay(10);
    }
    Serial.println(msg);
    config_page();
  }
}


//---------------------------------SETUP WIFI-----------
void setup_WiFi() {
  WiFi.mode(WIFI_STA); 
  WiFi.begin(mySSID, myPASS);
  while(WiFi.status()!=WL_CONNECTED){
        Serial.print("/");
        delay(500);
        }
        Serial.print("\r\n");
        Serial.print("IP Address\r\n");
        Serial.println(WiFi.localIP());
    

}
