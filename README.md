# PictoStick

![Pictostick 2](https://github.com/jsoeterbroek/pictostick/blob/main/assets/pictostick_logo.png)

**Note:** this is a prototype, not all of the functionality is working yet.

**The PictoStick is a compact device that can be worn as a keychain or on the wrist for people on 
the autism spectrum. It displays a series of pictograms for daily activities.  Activities 
already completed can be checked off on the PictoStick. Parents or caregivers can configure the 
pictograms via a web browser.**

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

## Software
The code for the pictostick is Free/Open Source software made available under the
Creative Commons Zero v1.0 Universal License at
https://github.com/jsoeterbroek/pictostick Github repository.

## Picto Icons

### Google Material Design Icons
The picto icons used are Google Fonts Material Design icons, made available under the
Apache License Version 2.0 license, reside on the device fylesystem allready to be used.
I have made a rough selection of about 150 suitable picto icons, but more can be added or
requested to be added. Some are in need of a rework and some are currently missing alltogether.

The full list is in the [pictodevice/data/](pictodevice/data/) dir of this repository.

(https://fonts.google.com/icons)

### ChatGTP-generated Icons
Some missing icons where generated using ChatGTP (see assets/prompt.txt) following Material
Design Guidelines:

* backpack.png
* breakfast.png
* dinner.png
* femalehygiene.png
* garden.png
* hospital.png
* lunch.png
* petcat.png
* petdog.png
* toothbrush.png

### Missing pictos
* cuddling/snoozleing
* ~~female hygiene products (tampons, etc.)~~

## Attribution
* Special thanks to VolosR (https://github.com/VolosR) for many, many code examples and ideas,
  some of which are used in this project. I could not have finished this project without his
  youtube videos and code examples.
* The Rugged Case for the M5Stick by Dissident [https://makerworld.com/en/models/573271-m5stack-m5stickc-plus-2-rugged-case#profileId-493964]](https://makerworld.com/en/models/573271-m5stack-m5stickc-plus-2-rugged-case#profileId-493964).
* Part of this code is generated with Google Gemini AI.
