#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>
#include "webpage.h"
#include "EasyPCF8574.h"

// PCF8574 object
EasyPCF8574 pcf(0x27, 0);  // address, optional

const char* ssid = "Lakshmi Luxury pg 3rd sub";
const char* password = "9705560260@03";

// Web server
WebServer server(80);

// Preferences storage
Preferences prefs;

// Keys and mapping to PCF8574 pins
const char* keys[] = {
  "wifi","bt","light","audio","dark","lock","cloud","power"
};

struct ControlMap {
  const char* key;
  uint8_t pin;
};

ControlMap mapTable[] = {
  {"wifi",  0},
  {"bt",    1},
  {"light", 2},
  {"audio", 3},
  {"dark",  4},
  {"lock",  5},
  {"cloud", 6},
  {"power", 7},
};

// Apply saved preferences to PCF8574 pins
/*void applySavedState() {
  for (auto &m : mapTable) {
    bool val = prefs.getBool(m.key, false);
    int state= !val.toInt();
    pcf.WriteBit(state, m.pin);
  }
}*/

// Root handler
void handleRoot() {
  server.send_P(200, "text/html", WEBPAGE_HTML);
}

// /set handler
void handleSet() {
  if (server.hasArg("key") && server.hasArg("value")) {
    String key = server.arg("key");
    bool value = server.arg("value").toInt();

    // Save in preferences
    prefs.putBool(key.c_str(), value);

    // Update PCF8574 immediately
    for (auto &m : mapTable) {
      if (key == m.key) {
        int state= !value;
        pcf.WriteBit(state, m.pin);
        break;
      }
    }
  }
  server.send(200, "text/plain", "OK");
}

// /state handler
void handleState() {
  String json = "{";
  for (int i = 0; i < 8; i++) {
    json += "\"" + String(keys[i]) + "\":" + String(prefs.getBool(keys[i], false));
    if (i < 7) json += ",";
  }
  json += "}";
  server.send(200, "application/json", json);
}

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected! IP address: ");
  Serial.println(WiFi.localIP());

  // Initialize PCF8574  /change the pins for espc3 core board as 8 sda, 9 scl
  //if (!pcf.startI2C(21, 22)) {
  if (!pcf.startI2C(8, 9)) {
    Serial.println("PCF8574 not started. Check address and pins!");
    while (true);
  }

  // Start preferences
  prefs.begin("smarthome", false);

  // Set all pins HIGH (released)
  pcf.setFullValue(0xFF);
  Serial.print("PCF8574 initial value: ");
  Serial.println(pcf.getPCFValue(), BIN);

  // Apply saved states from Preferences
  //applySavedState();

  // Define web routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/set", HTTP_GET, handleSet);
  server.on("/state", HTTP_GET, handleState);

  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient(); // Required for synchronous WebServer
}
