/*
 * HTTP Client POST Request
 * Copyright (c) 2018, circuits4you.com
 * All rights reserved.
 * https://circuits4you.com 
 * Connects to WiFi HotSpot. */

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include "DHT.h"

const int DHTPIN = 4;     

//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT11 //--> Defines the type of DHT sensor used. Here used is the DHT11 sensor.
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

/* Set these to your desired credentials. */
const char *ssid = "iPhone";  //ENTER YOUR WIFI SETTINGS
const char *password = "Yvnalv99";

DHT dht(DHTPIN, DHTTYPE);

//Web/Server address to read/write from 
const char *host = "172.20.10.3";   //https://circuits4you.com website or IP address of server

//=======================================================================
//                    Power on setup
//=======================================================================

void setup() {
  delay(1000);
  Serial.begin(115200);
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");

  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
}

//=======================================================================
//                    Main Program Loop
//=======================================================================
void loop() {
  HTTPClient http;    //Declare object of class HTTPClient
  WiFiClient client;

  double kelembaban = dht.readHumidity();
  // Read temperature as Celsius (the default)
  double suhu = dht.readTemperature();

  String ADCData, station, postData;
  //int adcvalue=analogRead(A0);  //Read Analog value of LDR
  ADCData = String(suhu);   //String to interger conversion
  station = "A";

  //Post Data
  postData = "status=" + ADCData + "&station=" + station ;
  
  http.begin(client, "http://172.20.10.3:8080/webapp_sensor/postdemo.php");              //Specify request destination
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //Specify content-type header

  int httpCode = http.POST(postData);   //Send the request
  String payload = http.getString();    //Get the response payload

  Serial.print(F("Temperature:"));
  Serial.print(suhu);
  Serial.print(F("°C   Humidity:"));
  Serial.print(kelembaban);
  Serial.println(F("%"));

  Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payload

  http.end();  //Close connection
  
  delay(5000);  //Post Data at every 5 seconds
}
//=======================================================================
