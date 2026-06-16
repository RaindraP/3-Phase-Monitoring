#include <WiFi.h>
#include <HTTPClient.h>
#include "EmonLib.h"
#include "ZMPT101B.h"

// =============================
// WIFI CONFIG
// =============================
const char* ssid = "Jodio";
const char* password = "jojonojojo";

// =============================
// SUPABASE CONFIG
// =============================
String SUPABASE_URL = "";
String API_KEY = "";

// =============================
// SENSOR CONFIG
// =============================
#define sct1 32
#define sct2 33
#define sct3 34

#define v1 15
#define v2 2
#define v3 4

#define SENSITIVITY 735.5f

EnergyMonitor emon1;
EnergyMonitor emon2;
EnergyMonitor emon3;

ZMPT101B voltageSensor1(v1, 60.0);
ZMPT101B voltageSensor2(v2, 60.0);
ZMPT101B voltageSensor3(v3, 60.0);

// =============================
// SETUP
// =============================
void setup() {
  Serial.begin(9600);
  delay(500);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Initialize sensors
  emon1.current(sct1, 1.5);
  emon2.current(sct2, 0.75);
  emon3.current(sct3, 0.75);

  voltageSensor1.setSensitivity(SENSITIVITY);
  voltageSensor2.setSensitivity(SENSITIVITY);
  voltageSensor3.setSensitivity(SENSITIVITY);

  Serial.println("=== 3-PHASE POWER MONITOR STARTED ===");
}

// =============================
// LOOP
// =============================
void loop() {
  // Read currents
  double Irms1 = emon1.calcIrms(39461);
  double Irms2 = emon2.calcIrms(39461);
  double Irms3 = emon3.calcIrms(39461);

  // Read voltages
  float Vrms1 = voltageSensor1.getRmsVoltage();
  float Vrms2 = voltageSensor2.getRmsVoltage();
  float Vrms3 = voltageSensor3.getRmsVoltage();

  // Print to Serial
  Serial.printf("L1: %.2f V | %.3f A\n", Vrms1, Irms1);
  Serial.printf("L2: %.2f V | %.3f A\n", Vrms2, Irms2);
  Serial.printf("L3: %.2f V | %.3f A\n", Vrms3, Irms3);
  Serial.println("-----------------------------");

  // =============================
  // SEND DATA TO SUPABASE
  // =============================
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    http.begin(SUPABASE_URL);
    http.addHeader("apikey", API_KEY);
    http.addHeader("Authorization", "Bearer " + API_KEY);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Prefer", "return=minimal");

    // JSON payload (sesuai kolom di monitoring_raw)
    String payload = "{";
    payload += "\"v_l1\":" + String(Vrms1, 2) + ",";
    payload += "\"i_l1\":" + String(Irms1, 3) + ",";
    payload += "\"v_l2\":" + String(Vrms2, 2) + ",";
    payload += "\"i_l2\":" + String(Irms2, 3) + ",";
    payload += "\"v_l3\":" + String(Vrms3, 2) + ",";
    payload += "\"i_l3\":" + String(Irms3, 3);
    payload += "}";

    int httpResponseCode = http.POST(payload);

    Serial.print("Supabase Response: ");
    Serial.println(httpResponseCode);

    http.end();
  } else {
    Serial.println("WiFi lost, reconnecting...");
    WiFi.reconnect();
  }

  delay(1000); // send every 1 second
}
