.. _architecture:

=====================
Codebase Architecture
=====================

This document provides a high-level overview of the PictoStick source code structure.

Directory Structure
===================

*   ``src/``: Contains all the C++ source code for the firmware.
    *   ``main.cpp``: The main entry point of the application. Handles initialization and the main loop.
    *   ``ui*.cpp``: Source files related to the user interface, built with the LVGL library.
    *   ``system.cpp``: Manages system-level functions like time, battery, and WiFi.
    *   ``fs_helpers.cpp``: Helper functions for interacting with the SPIFFS filesystem.
    *   ``PSpref.cpp``: Manages persistent storage of settings in NVS (Non-Volatile Storage).
*   ``include/``: Contains all the header files (`.h`) corresponding to the source files in `src/`.
*   ``data/``: Contains all files that will be uploaded to the device's filesystem (SPIFFS).
    *   ``picto/``: The library of all available PNG picto images.
    *   ``index.html``, ``style.css``: The web interface for managing activities.
    *   ``data_*.json``: The JSON files where the weekly activity schedules are stored.
*   ``lib/``: This directory is managed by PlatformIO and contains all the external libraries the project depends on.
*   ``platformio.ini``: The main configuration file for PlatformIO. It defines the board, framework, libraries, and build flags.

Core Components
===============

Main Loop
---------
The ``main.cpp`` file contains the ``setup()`` and ``loop()`` functions, which are standard in the Arduino framework.

*   ``setup()``: Initializes hardware (display, buttons), filesystems, WiFi, and the user interface. It determines which mode the device should boot into (Normal, WiFi Config, Web Admin).
*   ``loop()``: The main application loop. It primarily calls the LVGL task handler (``lv_timer_handler()``) which manages all UI rendering and interaction. It also handles button presses and power management.

User Interface (LVGL)
---------------------
The user interface is built using the **LVGL (Light and Versatile Graphics Library)**.

*   ``uimain.cpp`` is responsible for creating and managing the main screen, including the picto display, labels, and progress dots.
*   ``uisetup.cpp`` handles the on-device setup menu.
*   ``uiconfig.cpp`` manages the WiFi and Web Admin mode screens.

Data Flow
---------
The activity schedules are stored as JSON files in the ``data/`` directory.

*   When in Web Administration mode, the web server (ESPAsyncWebServer) serves the ``index.html`` and ``style.css`` files.
*   The web interface fetches the ``data_*.json`` files to display the current schedules.
*   When the caretaker saves the configuration, the web interface sends the new schedule data back to the device, which overwrites the corresponding ``data_*.json`` files on the filesystem.
*   In normal mode, the device reads the appropriate ``data_*.json`` file for the current day to load and display the activities.

Power Management
----------------
To conserve battery, the device makes use of several strategies:

*   **Light Sleep**: The ESP32 enters a light sleep mode when the screen is off.
*   **WiFi Off**: WiFi is only enabled when actively needed (initial time sync, web admin mode) and is turned off otherwise.
*   **Screen Timeout**: The screen backlight is turned off after a short period of inactivity.
