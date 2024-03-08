#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "HTTPSRedirect.h"
#include <Firebase_ESP_Client.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>

AsyncWebServer server(80);


#define ONE_WIRE_BUS D3
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"
OneWire oneWire(ONE_WIRE_BUS);
int relay = D4;
DallasTemperature sensors(&oneWire);
float suhu = 0.0;
float last_state = 0.0;

#define WIFI_SSID "xxx"      //di isi SSID Wifi
#define WIFI_PASSWORD "xxx"  //di isi password wifi
// Enter network credentials:
const char* ssid = "xxx";
const char* password = "xxx";

const char* _SSID = "xxx";
const char* _PASS = "xxx.";

#define API_KEY "xxx-xxx-xxx-xxx" //diisi web api key firebase
// Enter Google Script Deployment ID:
const char* GScriptId = "xxx-xxx-xxx-xxx";
// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "xxx-xxx"
// Enter command (insert_row or append_row) and your Google Sheets sheet name (default is Sheet1):
String payload_base = "{\"command\": \"insert_row\", \"sheet_name\": \"Sheet1\", \"values\": ";
String payload = "";

// Google Sheets setup (do not edit)
const char* host = "script.google.com";
const int httpsPort = 443;
const char* fingerprint = "";
String url = String("/macros/s/") + GScriptId + "/exec";
HTTPSRedirect* client = nullptr;

// Declare variables that will be published to Google Sheets
int temperature = 0;
//int ipaddr = 0;
//int value1 = 0;
//int value2 = 0;

int hitung = 0;
//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;
bool signupOK = false;
void (*resetF)(void) = 0;

void setup() {
  sensors.begin();
  pinMode(relay, OUTPUT);
  pinMode(led, OUTPUT);
  Serial.begin(115200);
  delay(10);
  Serial.println('\n');
  WiFi.mode(WIFI_STA);
  //WiFi.setPhyMode(WIFI_PHY_MODE_11G);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.print(ssid);
  Serial.println(" ...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println('\n');
  Serial.println("Connection established!");
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("ok");
    signupOK = true;
  } else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback;  //see addons/TokenHelper.h

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  // Use HTTPSRedirect class to create a new TLS connection
  client = new HTTPSRedirect(httpsPort);
  client->setInsecure();
  client->setPrintResponseBody(true);
  client->setContentTypeHeader("application/json");

  Serial.print("Connecting to ");
  Serial.println(host);
  bool flag = false;
  for (int i = 0; i < 5; i++) {
    int retval = client->connect(host, httpsPort);
    if (retval == 1) {
      flag = true;
      Serial.println("Connected");
      break;
    } else
      Serial.println("Connection failed. Retrying...");
  }
  if (!flag) {
    Serial.print("Could not connect to server: ");
    Serial.println(host);
    return;
  }

  delete client;     // delete HTTPSRedirect object
  client = nullptr;  // delete HTTPSRedirect object

  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(200, "text/plain", "Hi! This is a sample response.");
  });

  AsyncElegantOTA.begin(&server);  // Start AsyncElegantOTA
  server.begin();
  Serial.println("HTTP server started");
}


void loop() {
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);
  suhu = tempC + 3.0;
  hitung++;
  static bool flag = false;
  if (!flag) {
    client = new HTTPSRedirect(httpsPort);
    client->setInsecure();
    flag = true;
    client->setPrintResponseBody(true);
    client->setContentTypeHeader("application/json");
  }
  if (client != nullptr) {
    if (!client->connected()) {
      client->connect(host, httpsPort);
    }
  } else {
    Serial.println("Error creating client object!");
  }

  if (hitung == 60) {
    temperature = suhu;
    Firebase.RTDB.setInt(&fbdo, "Boiler/FW-Temp1", suhu);
    Serial.println(suhu);
    // Create json object string to send to Google Sheets
    //payload = payload_base + "\"" + value0 + "," + value1 + "," + value2 + "\"}";
    String ipaddr = WiFi.localIP().toString();
    payload = payload_base + "\"" + temperature + "," + ipaddr + "\"}";

    // Publish data to Google Sheets
    Serial.println("Publishing data...");
    Serial.println(payload);
    if (client->POST(url, host, payload)) {
      // do stuff here if publish was successful
    } else {
      // do stuff here if publish was not successful
      Serial.println("Error while connecting");
    }
    hitung = 0;
  }
  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(led, LOW);
    //Serial.println("Not Connected");
    if (suhu > 0 && suhu < 81) {
      digitalWrite(relay, HIGH);
    }
    if (suhu >= 85) {
      digitalWrite(relay, LOW);
    }
  }
  if (WiFi.status() != WL_CONNECTED) {
    digitalWrite(led, HIGH);
    //Serial.println("Connected");
    if (suhu > 0 && suhu < 81) {
      digitalWrite(relay, HIGH);
    }
    if (suhu >= 85) {
      digitalWrite(relay, LOW);
    }
  }
  Serial.println(hitung);
  delay(1000);
}
