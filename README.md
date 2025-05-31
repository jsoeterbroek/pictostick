# PictoStick

![Pictostick 2](https://github.com/jsoeterbroek/pictostick/blob/main/assets/pictostick_logo.png)

Note: this is a prototype, not all of the functionality working yet.

![Pictostick 2](https://github.com/jsoeterbroek/pictostick/blob/main/assets/IMG_0917.jpg)

## Introduction
Care givers, parents and health care professionals of people with a mental disability and/or
an Autism spectrum disorder (ASD), or previously refered to as 'autism', often make use of
visual pictograms to give people a clearer understanding of the world around them.

One of the biggest advantages of using pictograms (or pictos) is that in one
easy to grasp visual form the activities and their sequence that are planned for the day
ahead are presented. This provides understanding and prevents stress and miscommunication.

![a typical picto board](https://github.com/jsoeterbroek/pictostick/blob/main/assets/1911376602_cropped.jpg)

Often the pictos are presented on a board on the wall (as in picture above) or simply on a wooden plank
(as in picture below) on which the pictos are added (or even removed during the day) showing the
activities and the order in which these take place. While this low tech way often works fine in practice
it is not portable and often only used in either the users' household or the (work) places during the day.

![a typical picto plank](https://github.com/jsoeterbroek/pictostick/blob/main/assets/1823811866_cropped.jpg)

## PictoStick
The PictoStick offers a method to present a scrollable sequence of pictos for the user in a portable way,
either as a small device that can be carried around or a watch-like option with a wrist band.
The device can be referred to by the user as to the daily activities and even to check off the
ones that have already been completed.

![Pictostick 3](https://github.com/jsoeterbroek/pictostick/blob/main/assets/IMG_0918.jpg)

## Documentation
The documentation is available at (https://pictostick.readthedocs.io/)

## Video
[![Youtube Demo Pictostick](https://github.com/jsoeterbroek/pictostick/blob/main/assets/IMG_0917.jpg)](http://www.youtube.com/watch?v=uw7wsZyZL4c "Demo Pictostick")


## Hardware

* M5 StickC Plus2, a low-cost, small form factor ESP32 Mini IoT Development Kit.
  It has a 1.14-inch TFT screen with a resolution of 135x240 pixels and a battery
  capacity of 200mAh.
  (https://shop.m5stack.com/products/m5stickc-plus2-esp32-mini-iot-development-kit)
* M5-Watch wristband for M5 StickC Plus2 (optional)

Future possibly bigger device/screen.

## Software
The code for the pictostick is Free/Open Source software made available under the
Creative Commons Zero v1.0 Universal License at
https://github.com/jsoeterbroek/pictostick Github repository.

## Picto Icons
The picto icons used are Google Fonts Material Design icons, made available under the
Apache License Version 2.0 license, reside on the device fylesystem allready to be used.
I have made a rough selection of about 150 suitable picto icons, but more can be added or
requested to be added. Some are in need of a rework and some are currently missing alltogether.

The full list is in the [pictodevice/data/](pictodevice/data/) dir of this repository.

(https://fonts.google.com/icons)

### Missing pictos

* cuddling/snoozleing
* ~~female hygiene products (tampons, etc.)~~

### Conceptual
A caregiver or parent of the pictostick user configures the picto's on the device in
a correct time-based order and updates the device via a Web Browser interface over
WiFi.

### Technical
The configuration of the PictoStick device is provided on a per device/user basis via a
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
device or in the software code. On first startup of the PictoStick device these
secure credentials configuration data can be programmed into the device via a Web
browser and a temporary WiFI network between he device and the user.

See the Documentation for specific details of this procedure.

Documentation Link: procedure_wifi.md  TBA

## TODO
* configuration screen [#2]
* configurable options for user (eg. color themes) [#3]
* ~~show battery charging status when charging~~
* ~~make user changes (eg. activites done) persistent~~ [#4]
* user changes (eg. activites done) to be cleared overnight to signify new day [#5]
* write documentation for parents, care givers and health care professionals [#6]
* write documentation for users [#7]
* battery display is too 'wobbly'; changes to frequently [#8]
* The caregivers web interface for deploying (new) configuration file is very, very
barebones atm. This needs to become a full-fledged web server service. [#9]

### battery saving stuff
* ~~sleep mode after x seconds inactivity~~ [#10]
* ~~turn of WiFi when not needed~~
* backlight dimmable by user (and make persistent) [#11]

### Attribution
Special thanks to VolosR (https://github.com/VolosR) for many, many code examples and ideas,
some of which are used in this project. I could not have finished this project without his
youtube videos and code examples. By him a coffee: https://ko-fi.com/volosprojects
