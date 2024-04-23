/*
This file is created for simulate the LDR situation ON/OFF in the room. 

It is control by manually type the "on" and "off" in the serial monitor terminal to run the simulation
*/

#ifndef STASSID
#define STASSID "Verizon_B7V7NG"            // set your SSID: tuiot
#define STAPSK  "serial6-jaw-arc"           // set your wifi password: bruc3l0w3
#endif

/*** Configuration of NTP ***/
#define MY_NTP_SERVER "north-america.pool.ntp.org"

/* Necessary Includes */
#include <WiFi.h>                           // WiFi access
#include <time.h>                           // for time() ctime()
#include <stdbool.h>

/*** Pin Definitions ***/
#define LDR_PIN 2           // the pin where the LDR is connected
#define LIGHT_THRESHOLD 800 // threshold for light detection

/*** Globals ***/
time_t now;                                 // seconds since Epoch (1970) - UTC
tm tm;                                      // structure tm holds time information
bool simulatedLightOn = false;              // simulate LDR sensor status

void setup() {
  Serial.begin(115200);
  pinMode(LDR_PIN, INPUT);                  // initialize LDR pin as an input
  Serial.println("\nNTP TZ DST - bare minimum");

  // set time zone to EST and start NTP
  configTime(-5 * 3600, 3600, MY_NTP_SERVER);           // GMT Offset for EST is -5 hours, DST offset is 1 hour

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

/*** Function for get the actual datetime from the NTP server ***/
String get_date() {
  time(&now);                                           // time update
  localtime_r(&now, &tm);                               // convert the epoch time to calendar time

  char currentDate[11];                                 // buffer for formatted date
  sprintf(currentDate, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

  return String(currentDate);                           // return the date as a string
}

/*** Function for check if the LDR sensor is on/off ***/
bool get_light_status() {
  int sensorValue = analogRead(LDR_PIN);

  return sensorValue > LIGHT_THRESHOLD;
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

/*** Function for simulate the LDR sensor status ON/OFF ***/
void readSerialCommands() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    if (command == "on") {
      simulatedLightOn = true;
      Serial.println("Simulated LDR Sensor: ON");
    } else if (command == "off") {
      simulatedLightOn = false;
      Serial.println("Simulated LDR Sensor: OFF");
    }
  }
}

/*** Function for  ***/
void loop() {
  readSerialCommands();
  get_min();

  if (simulatedLightOn != get_light_status()) {
    if (simulatedLightOn) {
      Serial.println("Light has turned ON");              // light turned on
      // sql incrementation
    } else {
      Serial.println("Light is OFF");
      Serial.println("Date when light turned off: " + get_date());
      Serial.print("Time when light turned off: ");
      Serial.print(get_hour());
      Serial.print(":");
      Serial.println(get_min());
    }
  }
  
  showTime();
}

void update() {
  year = tm.tm_year + 1900;
  month = tm.tm_mon + 1;
  day = tm.tm_day;
  hour = tm.tm_hour;
  light_on = get_light_status();

  if (light_on == 1) {
    // sql incrementation
  } 
}

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
