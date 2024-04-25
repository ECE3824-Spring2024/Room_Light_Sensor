#ifndef STASSID
#define STASSID "Verizon_B7V7NG"            // set your SSID: tuiot     Verizon_B7V7NG
#define STAPSK  "serial6-jaw-arc"           // set your wifi password: bruc3l0w3      serial6-jaw-arc
#endif

/*** Configuration of NTP ***/
#define MY_NTP_SERVER "north-america.pool.ntp.org"

/*** Necessary Includes ***/
#include <WiFi.h>                           // WiFi access
#include <time.h>                           // for time() ctime()
#include <stdbool.h>

/*** Pin Definitions ***/
#define LDR_PIN 2           // the pin where the LDR is connected
#define LIGHT_THRESHOLD 800 // threshold for light detection

/*** Globals ***/
time_t now;                                 // seconds since Epoch (1970) - UTC
tm tm;                                      // structure tm holds time information

void setup() {
  Serial.begin(115200);
  pinMode(LDR_PIN, INPUT);                  // initialize LDR pin as an input
  //pinMode();
  Serial.println("\nNTP TZ DST - bare minimum");

  // set time zone to EST and start NTP
  configTime(-5 * 3600, 3600, MY_NTP_SERVER);           // GMT Offset for EST is -5 hours, DST offset is 1 hour

  // network config
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
}

/*** Function for check if the LDR sensor is on/off ***/
bool get_light_status() {
  int sensorValue = analogRead(LDR_PIN);

  // light on value around 170
  // light off value around 1960
  if (sensorValue < LIGHT_THRESHOLD) {
    return true;
  } else if (sensorValue > LIGHT_THRESHOLD) {
    return false;
  }
}

/*** Function for get the actual min from the NTP server ***/
int get_min() {
    time(&now);                                           // time update
    localtime_r(&now, &tm);                               // convert the epoch time to calendar time

    int currentMinutes = tm.tm_min;                       // extract the minutes from the current time

    // showing on serial monitor only, db might not need it
    Serial.print("Current minutes: ");
    Serial.println(currentMinutes);

    return currentMinutes;                                // return the actual current minutes from NTP
}

/*** Function for get the actual hour from the NTP server ***/
int get_hour() {
    time(&now);                                           // time update
    localtime_r(&now, &tm);                               // convert the epoch time to calendar time

    int currentHour = tm.tm_hour;                         // extract the hour from the current time

    // showing on serial monitor only, db might not need it
    Serial.print("Current hour: ");
    Serial.println(currentHour);

    return currentHour;                                   // return the actual current hour from NTP
}

/*** Function for get the actual datetime from the NTP server ***/
String get_date() {
  time(&now);                                             // time update
  localtime_r(&now, &tm);                                 // convert the epoch time to calendar time

  char currentDate[11];                                   // buffer for formatted date
  sprintf(currentDate, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
  
  return String(currentDate);                             // return the date as a string
}

/*** Loop Function***/
void loop() {
  
  static unsigned long lastTimeUpdateMillis = 0;
  unsigned long currentMillis = millis();
  //int sensorValue = analogRead(LDR_PIN);

  // call the functions every minutes
  if (currentMillis - lastTimeUpdateMillis >= 60000) {  // 60 seconds
    lastTimeUpdateMillis = currentMillis;
    
    showTime();                                 // serial terminal monitoring, this one can be comment
    bool currentLightStatus = get_light_status();      // light status check

    // check the light status change on every loop iteration
    if (sensorValue < 800) {
      Serial.println("Light has turned ON");    // light turned on
      Serial.println(get_date());               // printing yy-mm-dd in terminal
      get_date();                               // capture actual NTP yy-mm-dd for db
      get_hour();                               // capture actual NTP hour for db
      get_min();                                // capture actual NTP minute for db
    } else {
      Serial.println("Light is OFF"); 
    }
  }
}

/*** Please keep this function in here, I need for get the actual datetime in serial monitor ***/
// You can comment the whole function if needed
void showTime() {
  time(&now);                       // read the current time
  localtime_r(&now, &tm);           // update the structure tm with the current time
  Serial.print("Year:");
  Serial.print(tm.tm_year + 1900);  // years since 1900
  Serial.print("\tMonth:");
  Serial.print(tm.tm_mon + 1);      // January = 0 (!)
  Serial.print("\tDay:");
  Serial.print(tm.tm_mday);         // day of month
  Serial.print("\tHour:");
  Serial.print(tm.tm_hour);         // hours since midnight  0-23
  Serial.print("\tMin:");
  Serial.print(tm.tm_min);          // minutes after the hour  0-59
  if (tm.tm_isdst == 1)             // Daylight Saving Time flag
    Serial.print("\tDST");
  else
    Serial.print("\tStandard");
  Serial.println();
}