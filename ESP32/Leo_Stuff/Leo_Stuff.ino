/*
This file is created for simulate the LDR situation on and off in the room. 

It is control by manually type the "on" and "off" in the serial monitor terminal to run the simulation
*/


#ifndef STASSID
#define STASSID "Verizon_B7V7NG"            // set your SSID: tuiot
#define STAPSK  "serial6-jaw-arc"           // set your wifi password: bruc3l0w3
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
time_t now;                               // seconds since Epoch (1970) - UTC
tm tm;                                    // structure tm holds time information
unsigned long lightOnStartMillis = 0;     // store the time when the light turns on
unsigned long lightOnDurationMillis = 0;  // store how long the light has been on within the current minute
unsigned long lightOnMinutes = 0;         // minutes light has been on during the current check
unsigned long totalLightOnMinutes = 0;    // cumulative total minutes the light has been on
unsigned long totalLightOnHours = 0;      // cumulative total hours the light has been on
bool isCurrentlyLightOn = false;          // flag for tracking if the light is currently on
bool simulatedLightOn = false;            // simulate LDR sensor status

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


String get_date() {
  time(&now);                                           // time update
  localtime_r(&now, &tm);                               // convert the epoch time to calendar time

  char currentDate[11];                                 // buffer for formatted date
  sprintf(currentDate, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

  return String(currentDate);                           // return the date as a string
}

/* Leo stuff */

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

int get_light_status() {
  /* check if pin is on
   *  if so  return 1 if not return 0
   */
}

void updateHourlyTime() {
  configTime(-5 * 3600, 3600, MY_NTP_SERVER);
}

void updateMinutelyTime() {
  time(&now);                       // read the current time
  localtime_r(&now, &tm); 
}



void loop() {
  // every minute call get_min
  // and call updateTime
}


/* End Leo Stuff */


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

void readSerialCommands() {
    if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n');
        command.trim();             // remove any extraneous whitespace or newline
        
        if (command == "on") {
            simulatedLightOn = true;
            Serial.println("Simulated LDR Sensor: ON");
        } else if (command == "off") {
            simulatedLightOn = false;
            Serial.println("Simulated LDR Sensor: OFF");
        }
    }
}

int get_light_status() {
/*
    int val = analogRead(LDR_PIN);                        // read the LDR sensor
    bool isLightOn = val > LIGHT_THRESHOLD;
*/
    if (simulatedLightOn && !isCurrentlyLightOn) {
        lightOnStartMillis = millis();
        isCurrentlyLightOn = true;
        Serial.println("Light has turned ON");
    } else if (!simulatedLightOn && isCurrentlyLightOn) {
      lightOnDurationMillis = millis() - lightOnStartMillis;
      lightOnMinutes += (lightOnDurationMillis / 60000);// convert milliseconds to minutes
      lightOnDurationMillis = 0;                        // reset the duration
      isCurrentlyLightOn = false;
      Serial.println("Light is OFF");
      get_min();
      String currentDate = date_capture();
      Serial.print("Date when light turned off: ");     // handle minute calculation
      Serial.println(currentDate);
    }
}

int get_min_old() {
    totalLightOnMinutes += lightOnMinutes;              // add recent minutes to total
    if (totalLightOnMinutes >= 60) {
        totalLightOnHours += totalLightOnMinutes / 60;  // increment hours
        totalLightOnMinutes %= 60;                      // keep remainder minutes
    }
    lightOnMinutes = 0;                                 // reset recent minute count after adding to total

    Serial.print("Total light on-time: ");
    Serial.print(totalLightOnMinutes);
    Serial.println(" minutes");

    return totalLightOnMinutes;                         // return current minute count
}

int get_hour() {
    Serial.print("Total light on-time: ");
    Serial.print(totalLightOnHours);
    Serial.println(" hours");

    return totalLightOnHours;                           // return the total hours
}

void loop_jalen() {
  static unsigned long lastMillis = 0;

  readSerialCommands();                                 // read and process any serial commands
  
  // update the light status every minute
  if (millis() - lastMillis >= 60000) {
    lastMillis += 60000;                                // increment by one minute to keep the check consistent
    check_light();
    showTime();
  }
}
