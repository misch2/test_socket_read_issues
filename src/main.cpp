#include <Arduino.h>
#include <ArduinoHttpClient.h>
#include <WiFi.h>

#include "secrets.h"

WiFiClient wifiClient;
HttpClient httpClient = HttpClient(wifiClient, WEBSERVER_HOST, WEBSERVER_PORT);

#define BUFSIZE 100
unsigned char buffer[BUFSIZE];

void initWiFi() {
  Serial.println("connecting to wifi");
  WiFi.config(NETWORK_IP_ADDRESS, NETWORK_IP_GATEWAY, NETWORK_IP_SUBNET,
              NETWORK_IP_DNS);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }
  Serial.println("connected to wifi");
}

void test1() {
  httpClient.beginRequest();
  httpClient.get("/");
  httpClient.endRequest();

  int statusCode = httpClient.responseStatusCode();
  Serial.println("HTTP status code: " + String(statusCode));

  httpClient.skipResponseHeaders();

  memset(buffer, 0, BUFSIZE);
  int bytes = httpClient.read(buffer, BUFSIZE - 1);
  Serial.println("read " + String(bytes) + " bytes");

  String str_buf = String((char*)buffer);
  Serial.println("Response: [" + str_buf + "]");

  httpClient.stop();
}

void test2() {
  httpClient.beginRequest();
  httpClient.get("/");
  httpClient.endRequest();

  int statusCode = httpClient.responseStatusCode();
  Serial.println("HTTP status code: " + String(statusCode));

  httpClient.skipResponseHeaders();

  Serial.println("Reading 10 bytes sequentially");
  int c;
  for (int i = 0; i <= 10; i++) {
    c = httpClient.read();
    Serial.println("[" + String(i) + "] read: " + String(c));
  };

  httpClient.stop();
}

void setup() {
  delay(1000);  // wait for serial monitor to connect

  Serial.begin(115200);
  Serial.println("Starting");

  initWiFi();

  Serial.println("Testing GET request");
  test2();

  Serial.println("Done");
}

void loop() {}
