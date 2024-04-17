#ifndef STASSID
#define STASSID ""        // set your SSID
#define STAPSK  ""        // set your wifi password
#endif

/* Configuration of NTP */
#define MY_NTP_SERVER "north-america.pool.ntp.org"  

/* Necessary Includes */
#include <WiFi.h>    // we need wifi to get internet access
#include <time.h>    // for time() ctime()

/* Globals */
time_t now;            // this are the seconds since Epoch (1970) - UTC
tm tm;                 // the structure tm holds time information in a more convenient way

void setup() {
  Serial.begin(115200);
  Serial.println("\nNTP TZ DST - bare minimum");

  // set time zone to Eastern Standard Time and start NTP
  configTime(-5 * 3600, 3600, MY_NTP_SERVER); // GMT Offset for EST is -5 hours, DST offset is 1 hour

  // network config
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  // NTP will be started after the WiFi connection is established
}

void showTime() {
  time(&now);                       // read the current time
  localtime_r(&now, &tm);           // update the structure tm with the current time
  Serial.print("year:");
  Serial.print(tm.tm_year + 1900);  // years since 1900
  Serial.print("\tmonth:");
  Serial.print(tm.tm_mon + 1);      // January = 0 (!)
  Serial.print("\tday:");
  Serial.print(tm.tm_mday);         // day of month
  Serial.print("\thour:");
  Serial.print(tm.tm_hour);         // hours since midnight  0-23
  Serial.print("\tmin:");
  Serial.print(tm.tm_min);          // minutes after the hour  0-59
  Serial.print("\tsec:");
  Serial.print(tm.tm_sec);          // seconds after the minute  0-61*
  Serial.print("\twday");
  Serial.print(tm.tm_wday);         // days since Sunday 0-6
  if (tm.tm_isdst == 1)             // Daylight Saving Time flag
    Serial.print("\tDST");
  else
    Serial.print("\tstandard");
  Serial.println();
}

void loop() {
  showTime();
  delay(1000); // delay for 1 second
}
