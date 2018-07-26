#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <EEPROM.h>

const char *ssid = "FueraDeMiPantano>:v";
const char *password = "";

char mySSID[50];
char myPASS[50];


ESP8266WebServer server(80);
WiFiClient espClient;


//---------------------------------------------HTML------

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

//--------------------------------------------VARIABLES-----

int inpin = 0;
int outpin = 2;


void setup() {

  pinMode(outpin, OUTPUT);
  pinMode(inpin, INPUT);
  
	delay(1000);
	Serial.begin(115200);
  EEPROM.begin(4096);

	Serial.println();

  setup_WiFi();

  read(0).toCharArray(mySSID, 50);
  read(50).toCharArray(myPASS, 50);
	
}

void loop() {
	server.handleClient();
  if(digitalRead(inpin) == 1){
    config_mode();
  } 
  else {
    Serial.print(".");
  }
    digitalWrite(outpin, HIGH);
    delay(1000);
    digitalWrite(outpin, LOW);
    delay(1000);
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

  server.on("/",[](){server.send(200,"text/plain","Hola prro >:v");});
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

