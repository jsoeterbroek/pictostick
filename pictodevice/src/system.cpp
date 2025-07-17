#include "system.h"
#include <Arduino.h>
#include <M5StickCPlus2.h>
#include <WiFi.h>
#include <time.h>
#include "status.h"
#include <WiFiManager.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include "_locale.h"
#include "devicemode.h"
#include "common.h"
#include <AsyncJson.h>

// extern declarations for global variables defined in main.cpp
extern AppStatus app_status;
extern const char *ntpServer;
extern struct tm timeinfo;
extern WiFiManager wm;
extern AsyncWebServer server;
extern String processor(const String &var);


void setTimezone(String timezone) {
  Serial.printf("  Setting Timezone to %s\n", timezone.c_str());
  setenv("TZ", timezone.c_str(),
         1);  //  Now adjust the TZ.  Clock settings are adjusted to show the
              //  new local time
  tzset();
}

void initTime(String timezone) {
  Serial.println(" ");
  Serial.print("connecting to time server ");
  Serial.println(" ");
  // Init and get the time
  configTime(0, 0, ntpServer);
  delay(500);
  setTimezone(timezone);
  delay(100);
  if (!getLocalTime(&timeinfo)) {
    Serial.println("ERROR: failed to obtain time");
    delay(10000);
  } else {
    Serial.println("OK: obtained time");
    Serial.println(&timeinfo, " %A, %B %d %Y %H:%M:%S");
    app_status.ntp_ok = true;
  }
}

void configModeCallback(WiFiManager *myWiFiManager) {
  StickCP2.Display.drawString("******************", 4, 28);
  StickCP2.Display.drawString(TXT_WM_WEBPORTAL_STARTED, 4, 40);
  StickCP2.Display.drawString(WiFi.softAPIP().toString(), 4, 52);
  StickCP2.Display.drawString(TXT_WM_CONNECT_SSID, 4, 64);
  StickCP2.Display.drawString(myWiFiManager->getConfigPortalSSID(), 4, 76);
  StickCP2.Display.drawString(TXT_WM_CONFIG_DEVICE, 4, 88);
  StickCP2.Display.drawString("******************", 4, 100);
  app_status.wifi_mgr_config_mode_ok = true;
}

bool initWiFi() {
  WiFi.mode(WIFI_STA);

  Serial.print("Connecting to WiFi network... ");
  Serial.println(wm.getWiFiSSID());
  WiFi.begin(wm.getWiFiSSID(), wm.getWiFiPass());

  unsigned long currentMillis = millis();
  while (WiFi.status() != WL_CONNECTED && (millis() - currentMillis) <= 5000) {
    Serial.print(".");
    delay(100);
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Not connected to the WiFi network");
    Serial.println("restarting in device mode 1");
    set_devicemode(1);
    ESP.restart();
  } else {
    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local IP: ");
    Serial.println(WiFi.localIP());
    String localip = WiFi.localIP().toString();
    StickCP2.Display.drawString(localip, 120, 16);
  }

  return true;
}

void init_ESPAsync_Ws() {
  app_status.config_data_ok = false;
  app_status.set_config_data_spiff_ok = false;

  // mount SPIFFS
  Serial.println("Mounting SPIFFS...");
  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }
  if (!SPIFFS.exists("/index.html")) {
    Serial.println("ERROR: index.html not found in SPIFFS");
    Serial.println("Please upload the web files to SPIFFS");
    return;
  }

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html", "text/html", false, processor);
  });

  // Route to get list of available pictos
  server.on("/get-pictos", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    JsonDocument doc;
    JsonArray pictos = doc.to<JsonArray>();

    File root = SPIFFS.open("/picto");
    if (!root || !root.isDirectory()) {
      response->print("[]");
      request->send(response);
      return;
    }

    File file = root.openNextFile();
    while (file) {
      if (!file.isDirectory()) {
        String filename = file.name();
        if (filename.endsWith(".png")) {
          // Remove path prefix if present
          int lastSlash = filename.lastIndexOf('/');
          if (lastSlash >= 0) {
            filename = filename.substring(lastSlash + 1);
          }
          // Remove .png extension for the name
          String pictoName = filename.substring(0, filename.length() - 4);

          JsonObject picto = pictos.add<JsonObject>();
          picto["filename"] = filename;
          picto["name"] = pictoName;
        }
      }
      file = root.openNextFile();
    }
    root.close();

    serializeJson(doc, *response);
    request->send(response);
  });

  // Route to handle JSON configuration save
  AsyncCallbackJsonWebHandler *handler = new AsyncCallbackJsonWebHandler("/save-config", [](AsyncWebServerRequest *request, JsonVariant &json) {
    Serial.println("Received JSON configuration");

    // Extract the day name from the JSON payload
    String dayName = "";
    if (json["weekdays"].is<JsonObject>()) {
      JsonObject weekdays = json["weekdays"].as<JsonObject>();
      for (JsonPair day : weekdays) {
        dayName = day.key().c_str();
        break; // Assuming only one day's data is sent at a time
      }
    }

    if (dayName.isEmpty()) {
      Serial.println("Day name not found in JSON payload");
      request->send(400, "text/plain", "Bad Request: Day name missing");
      return;
    }

    // Construct the filename based on the day name
    String filename = "/data_" + dayName + ".json";

    // Write the JSON for the specific day to its file
    File file = SPIFFS.open(filename, FILE_WRITE);
    if (!file) {
      Serial.printf("Failed to open %s for writing\n", filename.c_str());
      request->send(500, "text/plain", "Failed to save configuration 1");
      return;
    } else {
      Serial.printf("Success opened %s for writing\n", filename.c_str());
    }

    // Serialize only the activities for the current day
    if (serializeJson(json["weekdays"][dayName], file) == 0) {
      Serial.print(F("Failed write to file "));
      request->send(500, "text/plain", "Failed to save configuration 2");
      app_status.set_config_data_spiff_ok = false;
    } else {
      Serial.printf("Configuration saved to %s\n", filename.c_str());
      request->send(200, "text/plain", "Configuration saved successfully");
      app_status.config_data_ok = true;
      app_status.set_config_data_spiff_ok = true;
    }
    file.close();
  });
  server.addHandler(handler);

  server.serveStatic("/", SPIFFS, "/");

  // Route to get current configuration
  server.on("/get-config", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncResponseStream *response = request->beginResponseStream("application/json");

    String dayName = request->arg("day");
    if (dayName.isEmpty()) {
      Serial.println("Day parameter missing for /get-config");
      request->send(400, "text/plain", "Bad Request: Day parameter missing");
      return;
    }

    String filename = "/data_" + dayName + ".json";
    File file = SPIFFS.open(filename, FILE_READ);
    if (file) {
      String content = file.readString();
      file.close();
      response->print(content);
    } else {
      Serial.printf("File %s not found, sending empty JSON\n", filename.c_str());
      response->print("{}"); // Send empty JSON if file not found
    }

    request->send(response);
  });

  server.begin();
}


void reset_all_activities_undone() {
  String daysOfWeek[] = {"monday", "tuesday", "wednesday", "thursday", "friday", "saturday", "sunday"};

  for (int d = 0; d < 7; d++) {
    String dayName = daysOfWeek[d];
    String filename = "/data_" + dayName + ".json";

    File file = SPIFFS.open(filename, FILE_READ);
    if (!file) {
      Serial.printf("Failed to open %s for reading, skipping\n", filename.c_str());
      continue;
    }

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, file);
    file.close();

    if (error) {
      Serial.printf("deserializeJson() failed for %s: %s\n", filename.c_str(), error.c_str());
      continue;
    }

    JsonArray activities = doc["activities"].as<JsonArray>();
    for (JsonObject activity : activities) {
      activity["done"] = false;
    }

    file = SPIFFS.open(filename, FILE_WRITE);
    if (!file) {
      Serial.printf("Failed to open %s for writing\n", filename.c_str());
      return;
    }

    if (serializeJson(doc, file) == 0) {
      Serial.printf("Failed to write to file %s\n", filename.c_str());
    } else {
      Serial.printf("Activities in %s reset to undone.\n", filename.c_str());
    }
    file.close();
  }
}
