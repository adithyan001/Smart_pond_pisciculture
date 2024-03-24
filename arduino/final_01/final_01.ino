#include <WiFi.h>
#include "time.h"

const char* ssid       = "FTTH-D7F0";
const char* password   = "ava1234567890";
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 19800;
const int   daylightOffset_sec = 3600;




int get_time()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return 0;
  }
  int hrs=timeinfo.tm_hour;
  int min=timeinfo.tm_min;
  return ((hrs-1)*100+min);
}

void setup()
{
  Serial.begin(115200);
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  get_time();
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

void loop()
{

}
