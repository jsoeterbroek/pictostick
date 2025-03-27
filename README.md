# pictostick
The pictostick

## Hardware

* M5 StickC Plus2 (https://shop.m5stack.com/products/m5stickc-plus2-esp32-mini-iot-development-kit), 
  a low-cost, small form factor ESP32 Mini IoT Development Kit.
  It has a 1.14-inch TFT screen with a resolution of 135x240 pixels and a battery capacity of 200mAh.
* M5-Watch wristband for M5 StickC Plus2 (optional)

Future possibly bigger device/screen.

## Software
The code for the pictostick is Free/Open Source software made available under the 
Creative Commons Zero v1.0 Universal License at 
https://github.com/jsoeterbroek/pictostick Github repository.

## Configuration

### Conceptual
A caregiver or parent of the pictostick user configures the picto's on the device in 
a correct time-based order and updates the device via a Web Browser interface over
WiFi.

### Technical
The configuration of the Pictostick device is provided on a per device/user basis via a 
JSON configuration file which is located on the device file system /data directory 
called data.json.
An update mechanism for providing the device with a new
and/or updated configuration file via HTTP is in the repository picto_ws directory,
together with a sample data.json file to be used as a starting point.

## WiFi
The pictostick device is connected to the outside world via WiFi. This is needed for
periodical time synchronisation of the devices' internal clock with a public 
Internet NTP clock server. Secondly, the WiFi connection is needed to allow for 
caregivers and/or parents of the device user to remotely configure the device and
specifically the picto sequences stored on the device for the specific user.


### WiFi secure credentials 
The WiFi secure
credentials of the care-giver or parent of the user are not initially stored on the 
device or in the software code. On first startup of the Pictostick device these 
secure credentials configuration data can be programmed into the device via a Web
browser and a temporary WiFI network between he device and the user. 

See the Documentation for specific details of this procedure.

Documentation Link: procedure_wifi.md  TODO

## TODO
* screen for config and/or time/date, battery info bigger that the top bar on main screen
* configurable options for user (eg. color themes)
* code formatting

### battery saving stuff
* sleep mode after x seconds inactivity
* turn of WiFi when not needed
* backlight dimming by user

### Attribution
Special thanks to VolosR (https://github.com/VolosR) for many, many code examples and ideas
some of which are used in this project. I could not have finished this project without his
youtube videos and code examples. By him a coffee: https://ko-fi.com/volosprojects
