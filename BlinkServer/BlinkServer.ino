#include <WiFi.h>
#include "aWOT.h"
#include "StaticFiles.h"
#include "ConnectionConfig.h"
#include "FastLED.h"

#define LED_BUILTIN 22
#define STRIP_1_LENGTH 6
#define STRIP_1_PIN 4
#define LED_TYPE WS2812
#define RGB_ORDER GRB
CRGB strip_1[STRIP_1_LENGTH];

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
  
  FastLED.addLeds<LED_TYPE, STRIP_1_PIN, RGB_ORDER>(strip_1, STRIP_1_LENGTH);
  FastLED.setBrightness(255);
  fill_solid(strip_1, STRIP_1_LENGTH, CRGB( 0, 0, 0));
  FastLED.show();

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

  if (ledOff) {
    fill_solid(strip_1, STRIP_1_LENGTH, CRGB( 255, 0, 0));
  } else {
    fill_solid(strip_1, STRIP_1_LENGTH, CRGB( 0, 0, 0));
  }
  // should call show() after state change
  FastLED.show();
}
