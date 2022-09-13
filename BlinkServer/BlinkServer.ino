#include <WiFi.h>
#include "aWOT.h"
#include "StaticFiles.h"
#include "ConnectionConfig.h"

#define LED_BUILTIN 22

#define INITIAL true

bool ledOff = INITIAL;

void readLed(Request &req, Response &res) {
  res.print(!ledOff);
}

void updateLed(Request &req, Response &res) {
  ledOff = (req.read() != '0');
  digitalWrite(LED_BUILTIN, !ledOff);
  return readLed(req, res);
} 

WiFiServer server(80);
Application app;

void setup() {
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, INITIAL);

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(".");
  }
  Serial.println();
  Serial.println(WiFi.localIP());

  app.get("/led", &readLed);
  app.put("/led", &updateLed);

  app.use(staticFiles());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();

  if (client.connected()) {
    app.process(&client);
  }
}
