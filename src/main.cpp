#define BLYNK_TEMPLATE_ID           "TMPLC5-862P2"
#define BLYNK_DEVICE_NAME           "Quickstart Device"
#define BLYNK_AUTH_TOKEN            "7wwAzXbbfea962wpa5a0yW_J7ex9xzTA"

#define BLYNK_PRINT Serial

#include "Arduino.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "Gm";
char pass[] = "gm969/13";

WidgetTerminal terminal(V3);

BLYNK_WRITE(V0)
{
  int value = param.asInt();
  if (value == 1)
  {
    terminal.println("Turning on motor");
    terminal.flush();
    digitalWrite(5, HIGH);
    Blynk.virtualWrite(V1, "เปิด");
  }
  else
  {
    terminal.println("Turning off motor");
    terminal.flush();
    digitalWrite(5, LOW);
    Blynk.virtualWrite(V1, "ปิด");
  }
  
}

void setup()
{
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);

  pinMode(5, OUTPUT);

  Blynk.virtualWrite(V4,ssid);
  terminal.clear();
  terminal.println(F("Blynk v" BLYNK_VERSION ": Device started"));
  terminal.println(F("-------------"));
  terminal.println("Conneced to: " + String(ssid));
  terminal.flush();
}

void loop()
{
  Blynk.run();
}