/*
    This sketch sends data via HTTP GET requests to data.sparkfun.com service.

    You need to get streamId and privateKey at data.sparkfun.com and paste them
    below. Or just customize this script to talk to other HTTP servers.

*/
#include <ArduinoJson.h>
#include <Preferences.h>
#include <WiFi.h>
#include <U8g2lib.h>
#include <Esp32WifiManager.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display




WifiManager manager;

void setup()
{
  Serial.begin(115200);
  delay(10);
  u8g2.begin();

  manager.setupScan();

  // We start by connecting to a WiFi network

  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
  u8g2.drawStr(0, 10, "Starting Up"); // write something to the internal memory
  //u8g2.drawStr(0, 20, ssid); // write something to the internal memory
  u8g2.sendBuffer();          // transfer internal memory to the display

  Serial.println();
  Serial.println();

  

  //WiFi.begin(ssid, password);

  //while (WiFi.status() != WL_CONNECTED) {
  //delay(500);
  //Serial.print(".");
  //}

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

int value = 0;

void loop()
{
  delay(1000);
  ++value;

  manager.loop();

  if (manager.getState() != Connected) {

    return;
  }

  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  // We now create a URI for the request
  String url = "/all/";
  //String url = "/countries/New%20Zealand";
  // url += streamId;
  // url += "?private_key=";
  // url += privateKey;
  // url += "&value=";
  // url += value;

  Serial.print("Requesting URL: ");
  Serial.println(url);

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  //while(client.available()) {
  //  String line = client.readStringUntil('\r');
  // Serial.print(line);
  //}

  //const size_t capacity = JSON_OBJECT_SIZE(3) + 30;
  const size_t capacity = JSON_OBJECT_SIZE(9) + 120;
  DynamicJsonDocument doc(capacity);

  //const char* json = "{\"cases\":245613,\"deaths\":10048,\"recovered\":88437}";

  // Parse JSON object
  while (client.available()) {

    // Check HTTP status
    char status[32] = {0};
    client.readBytesUntil('\r', status, sizeof(status));
    // It should be "HTTP/1.0 200 OK" or "HTTP/1.1 200 OK"
    if (strcmp(status + 9, "200 OK") != 0) {
      Serial.print(F("Unexpected response: "));
      Serial.println(status);
      return;
    }

    // Skip HTTP headers
    char endOfHeaders[] = "\r\n\r\n";
    if (!client.find(endOfHeaders)) {
      Serial.println(F("Invalid response"));
      return;
    }

    DeserializationError error = deserializeJson(doc, client);
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      return;
    }
  }



  long cases = doc["cases"]; // 245613
  int deaths = doc["deaths"]; // 10048
  long recovered = doc["recovered"]; // 88437

  Serial.println(cases);
  Serial.println(deaths);
  Serial.println(recovered);

  //char lineOne = "Cases: " + cases;

  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setCursor(0, 10);
  u8g2.print("Cases:");
  u8g2.setCursor(70, 10);
  u8g2.print(cases );  // write something to the internal memory

  u8g2.setCursor(0, 20);
  u8g2.print("Deaths:");
  u8g2.setCursor(70, 20);
  u8g2.print(deaths );  // write something to the internal memory

  u8g2.setCursor(0, 30);
  u8g2.print("Recovered:");
  u8g2.setCursor(70, 30);
  u8g2.print(recovered );  // write something to the internal memory

  u8g2.sendBuffer();          // transfer internal memory to the display

  Serial.println();
  Serial.println("closing connection");












  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections


  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  // We now create a URI for the request
  // url = "/all/";
  url = "/countries/New%20Zealand";
  // url += streamId;
  // url += "?private_key=";
  // url += privateKey;
  // url += "&value=";
  // url += value;

  Serial.print("Requesting URL: ");
  Serial.println(url);

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  //while(client.available()) {
  //  String line = client.readStringUntil('\r');
  // Serial.print(line);
  //}

  //const size_t capacity = JSON_OBJECT_SIZE(3) + 30;
  //const size_t capacity = JSON_OBJECT_SIZE(9) + 120;
  //DynamicJsonDocument doc(capacity);

  //const char* json = "{\"cases\":245613,\"deaths\":10048,\"recovered\":88437}";

  // Parse JSON object
  while (client.available()) {

    // Check HTTP status
    char status[32] = {0};
    client.readBytesUntil('\r', status, sizeof(status));
    // It should be "HTTP/1.0 200 OK" or "HTTP/1.1 200 OK"
    if (strcmp(status + 9, "200 OK") != 0) {
      Serial.print(F("Unexpected response: "));
      Serial.println(status);
      return;
    }

    // Skip HTTP headers
    char endOfHeaders[] = "\r\n\r\n";
    if (!client.find(endOfHeaders)) {
      Serial.println(F("Invalid response"));
      return;
    }

    DeserializationError error = deserializeJson(doc, client);
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      return;
    }
  }



  cases = doc["cases"]; // 245613
  deaths = doc["deaths"]; // 10048
  recovered = doc["recovered"]; // 88437

  Serial.println(cases);
  Serial.println(deaths);
  Serial.println(recovered);

  //char lineOne = "Cases: " + cases;

  //u8g2.clearBuffer();          // clear the internal memory
  u8g2.setCursor(0, 40);
  u8g2.print("Cases (NZ):");
  u8g2.setCursor(70, 40);
  u8g2.print(cases );  // write something to the internal memory

  u8g2.setCursor(0, 50);
  u8g2.print("Deaths:");
  u8g2.setCursor(70, 50);
  u8g2.print(deaths );  // write something to the internal memory

  u8g2.setCursor(0, 60);
  u8g2.print("Recovered:");
  u8g2.setCursor(70, 60);
  u8g2.print(recovered );  // write something to the internal memory

  u8g2.sendBuffer();          // transfer internal memory to the display

  Serial.println();
  Serial.println("closing connection");

  //"https://coronavirus-19-api.herokuapp.com/countries/New%20Zealand"

  delay(29000);
}
