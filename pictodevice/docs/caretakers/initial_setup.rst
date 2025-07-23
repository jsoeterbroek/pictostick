.. _initial_setup:

==============
Initial Setup
==============

.. note::
   This guide is for the one-time setup of a new PictoStick device.

The first time the PictoStick is powered on, it must be connected to a local WiFi network. This allows it to sync the correct time and date from the internet, which is essential for displaying the correct daily schedules.

This process uses a feature called **WiFiManager**. It creates a temporary WiFi access point that you can connect to with a phone or computer to enter your network credentials.

Step-by-Step Guide
====================

1.  **Power on the PictoStick.**
    Press and hold the side power button (Button A) for about 6 seconds. The device will boot up and, since it has no saved WiFi credentials, it will automatically enter WiFi configuration mode. The screen will display a message indicating it has started a WiFi Access Point.

    .. image:: /_static/images/firststart_wifi_1.jpg
       :width: 250
       :alt: PictoStick in WiFi configuration mode

2.  **Connect to the "pictodevice" Network.**
    On a smartphone, tablet, or computer, open your WiFi settings and look for a new network named **PictoStick**. Connect to it.

3.  **Open the Configuration Portal.**
    Once connected, a captive portal page should open automatically in your web browser. If it doesn't, open a browser and navigate to ``http://192.168.4.1``.

4.  **Configure WiFi.**
    In the portal, you will see a few options.
    *   Click on **"Configure WiFi"**.
    *   A list of nearby WiFi networks will be displayed. Select your local network (e.g., "MyHomeWiFi").
    *   Enter the password for your WiFi network in the password field.
    *   Select the correct **Timezone** from the dropdown list. This is crucial for ensuring the time is correct for your location.
    *   Click **"Save"**.

    .. image:: /_static/images/firststart_wifi_2.jpg
       :width: 250
       :alt: WiFiManager configuration screen

5.  **Setup Complete.**
    The PictoStick will now save your credentials, disconnect the temporary access point, and connect to your selected WiFi network. It will sync the time, and then reboot into its normal operating mode.

The device will not use WiFi during normal operation to conserve battery. It will only briefly reconnect when it needs to sync the time or when you enter the web administration mode to manage activities.
