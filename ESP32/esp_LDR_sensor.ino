#ifndef STASSID
#define STASSID "Verizon_B7V7NG"            // set your SSID
#define STAPSK  "serial6-jaw-arc"           // set your wifi password
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
unsigned long lightOnStartMillis = 0; // store the time when the light turns on
unsigned long lightOnDuration = 0;    // store how long the light has been on

bool lightWasOn = false; // flag for tracking the previous state of the ligh

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

void check_light() {
  int val = analogRead(LDR_PIN); // read the LDR sensor
  bool isLightOn = val > LIGHT_THRESHOLD;

  if (isLightOn && !lightWasOn) {
    // light on
    lightOnStartMillis = millis();
    Serial.println("Light has turned ON");
  } else if (!isLightOn && lightWasOn) {
    // light off
    lightOnDuration = millis() - lightOnStartMillis;
    Serial.print("Light was ON for ");
    Serial.print(lightOnDuration / 1000); // print the duration in seconds
    Serial.println(" seconds.");
    Serial.print("Light is OFF");
  }
  lightWasOn = isLightOn; // update the flag with the current light state
}

void loop() {
  showTime();
  check_light();
  delay(1000); 
}
