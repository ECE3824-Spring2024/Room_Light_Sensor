
# Room_Light_Sensor
https://ferrous-quest-421700.ue.r.appspot.com/

## Overview

We will have a MongoDB or SQL database hosted on the cloud, which an ESP-32 can connect to. This ESP-32 has a light-sensor attached, that determines when the lights on a room go on. This ESP-32 then updates our database with the length of time the lights are on. This is queryable through a front-end running off of flask, which allows us to enter in a date to see the amount of time that the lights in a given room were on. Minimum viable product will be the ability to query a date without specifying a room, and seeing the results of the device from previous days (not necessarily updating current day in real-time). We can then update these features at a later date.


### Flask Credentials
flask
flaskpass

### ESP32 Credentials
esp32
esp32pass
