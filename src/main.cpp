#define BLYNK_TEMPLATE_ID "TMPLC5-862P2"
#define BLYNK_DEVICE_NAME "water wheel control"
#define BLYNK_AUTH_TOKEN "7wwAzXbbfea962wpa5a0yW_J7ex9xzTA"

#define BLYNK_PRINT Serial

#include "Arduino.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "time.h"

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "WaterWheel";
char pass[] = "Water1234";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 25200;
const int   daylightOffset_sec = 0;

boolean isTimerSet = false;
String startTimeInSecs;
String stopTimeInSecs;


WidgetTerminal terminal(V3);
BlynkTimer timer;
void printLocalTime();

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

BLYNK_WRITE(V6)
{
  int value = param.asInt();
  if (value == 1)
  {
    isTimerSet = true;
    terminal.println("Turning on timer");
    terminal.print("start at ");
    int buffer = startTimeInSecs.toInt();
    terminal.print(buffer/3600);
    terminal.print(":");
    buffer = buffer%3600;
    terminal.println(buffer/60);
    terminal.print("stop at ");
    buffer = stopTimeInSecs.toInt();
    terminal.print(buffer/3600);
    terminal.print(":");
    buffer = buffer%3600;
    terminal.println(buffer/60);
    terminal.flush();
    Blynk.virtualWrite(V8, "เปิด");
  }
  else
  {
    isTimerSet = false;
    terminal.println("Turning off timer");
    terminal.flush();
    Blynk.virtualWrite(V8, "ปิด");
  }
  
}

BLYNK_WRITE(V5)
{   
  TimeInputParam t(param);
  startTimeInSecs = param[0].asStr();
  stopTimeInSecs = param[1].asStr();
}


void setup()
{
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);

  pinMode(5, OUTPUT);

  Blynk.virtualWrite(V4,ssid);

  timer.setInterval(60000L, printLocalTime);

  terminal.clear();
  terminal.println(F("Blynk v" BLYNK_VERSION ": Device started"));
  terminal.println(F("-------------"));
  terminal.println("Conneced to: " + String(ssid));
  terminal.flush();
   //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();
}

void loop()
{
  Blynk.run();
  timer.run();
}

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  char buffer[40];
  // Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  sprintf(buffer, "%02d:%02d น.", timeinfo.tm_hour, timeinfo.tm_min);
  Blynk.virtualWrite(V7, buffer);
  if(isTimerSet){
    if((timeinfo.tm_hour*3600 + timeinfo.tm_min*60) == startTimeInSecs.toInt()){
      terminal.println("Timer turning on motor");
      terminal.flush();
      digitalWrite(5, HIGH);
      Blynk.virtualWrite(V0, 1);
    }
    if((timeinfo.tm_hour*3600 + timeinfo.tm_min*60) == stopTimeInSecs.toInt()){
      terminal.println("Timer turning off motor");
      terminal.flush();
      digitalWrite(5, LOW);
      Blynk.virtualWrite(V0, 0);
    }
  }
}