/*
Reference link: https://werner.rothschopf.net/202011_arduino_esp8266_ntp_en.htm

*/

#ifndef STASSID
#define STASSID ""            // set your SSID
#define STAPSK  ""           // set your wifi password
#endif

/* Configuration of NTP */
#define MY_NTP_SERVER "north-america.pool.ntp.org"

/* Necessary Includes */
#include <WiFi.h>    // WiFi access
#include <time.h>    // for time() ctime()

/* Pin Definitions */
#define LDR_PIN 2           // the pin where the LDR is connected
#define LIGHT_THRESHOLD 800 // threshold for light detection

/* Globals */
time_t now;           // seconds since Epoch (1970) - UTC
tm tm;                // structure tm holds time information

void setup() {
  Serial.begin(115200);
  pinMode(LDR_PIN, INPUT);  // initialize LDR pin as an input
  Serial.println("\nNTP TZ DST - bare minimum");

  // set time zone to EST and start NTP
  configTime(-5 * 3600, 3600, MY_NTP_SERVER); // GMT Offset for EST is -5 hours, DST offset is 1 hour

  // network config
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
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

void checkLight() {
  int val = analogRead(LDR_PIN); // read the LDR sensor
  if (val > LIGHT_THRESHOLD) {
    Serial.println("Light is ON");
  } else {
    Serial.println("Light is OFF");
  }
}

void loop() {
  showTime();
  checkLight();
  delay(1000); 
}
