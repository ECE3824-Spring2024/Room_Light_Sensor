#ifndef STASSID
#define STASSID "tuiot"               // set your SSID: tuiot     
#define STAPSK  "bruc3l0w3"           // set your wifi password: bruc3l0w3      
#endif

/*** Configuration of NTP ***/
#define MY_NTP_SERVER "north-america.pool.ntp.org"

/*** Necessary Includes ***/
#include <WiFi.h>                           // WiFi access
#include <time.h>                           // for time() ctime()

/*** Pin Definitions ***/
#define LDR_PIN 4           // the pin where the LDR is connected

/*** Globals ***/
time_t now;                                 // seconds since Epoch (1970) - UTC
tm tm;                                      // structure tm holds time information
int sensorValue = 0;                        

void setup() {
  Serial.begin(115200);
  pinMode(LDR_PIN, INPUT);                  // initialize LDR pin as an input
  Serial.println("\nNTP TZ DST - bare minimum");

  // network config
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  // set time zone to EST and start NTP
  configTime(-5 * 3600, 3600, MY_NTP_SERVER);           // GMT Offset for EST is -5 hours, DST offset is 1 hour
  
  // loop for NTP server datetime until it update to current datetime
  while(tm.tm_year + 1900 < 2000){
    time(&now);                      
    localtime_r(&now, &tm); 
  }
}

/*** Function for get the actual min from the NTP server ***/
int get_min() {
    update_time();

    int currentMinutes = tm.tm_min;                       // extract the minutes from the current time

    return currentMinutes;                                // return the actual current minutes from NTP
}

/*** Function for get the actual hour from the NTP server ***/
int get_hour() {
    update_time();

    int currentHour = tm.tm_hour;                         // extract the hour from the current time

    return currentHour;                                   // return the actual current hour from NTP
}

/*** Function for get the actual datetime from the NTP server ***/
String get_date() {
  update_time();

  char currentDate[11];                                   // buffer for formatted date
  sprintf(currentDate, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
  
  return String(currentDate);                             // return the date as a string
}

/*** Function for check if the LDR sensor is on/off ***/
void get_light_status() {
  sensorValue = digitalRead(LDR_PIN);
}

/*** Main Loop function***/
void loop() {
  
  static unsigned long lastTimeUpdateMillis = 0;
  unsigned long currentMillis = millis();
  get_light_status();

  // call the functions every minutes
  if (currentMillis - lastTimeUpdateMillis >= 60000) {  // 60 seconds
    lastTimeUpdateMillis = currentMillis;
    update_time();

    // check the light status change on every loop iteration
    if (sensorValue==0) {
      Serial.println("Light is ON");    

      String currdatetime = get_date();         // get the yy-mm-dd from NTP
      int currhour    = get_hour();             // get the current hour from NTP
      int currmin     = get_min();              // get the current minute from NTP

      Serial.println(currdatetime);             // print yy-mm-dd in terminal
      Serial.println(currhour);                 // print current hour in terminal
      Serial.println(currmin);                  // print current minute in terminal
      
      sql_query(currdatetime, currhour,currmin);
    } else {
      Serial.println("Light is OFF"); 
    }
  }
}

/*** Function for sql database query ***/
void sql_query(String datetime, int hour, int min) {
  ;
}

/*** Time update function ***/
void update_time() {
  time(&now);                       // read the current time
  localtime_r(&now, &tm); 
}