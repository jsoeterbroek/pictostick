.. _setup:

=================
Developer Setup
=================

.. note::
   **Intended Audience**: This document is for developers who want to modify, build, and flash the PictoStick firmware.

The PictoStick project is built using PlatformIO, a professional ecosystem for embedded development.

Github Repository
=================

The primary source code repository is hosted on GitHub:

https://github.com/jsoeterbroek/pictostick

Prerequisites
=============

You will need the following tools installed on your development machine:

1.  **Visual Studio Code**: The recommended IDE for this project.
    *   Download at: https://code.visualstudio.com/
2.  **PlatformIO IDE Extension**: The extension for VS Code that provides the PlatformIO toolchain.
    *   Install from the Visual Studio Code Marketplace: https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide
3.  **Git**: For cloning the repository and managing source control.
    *   Download at: https://git-scm.com/

Cloning the Repository
======================

First, clone the repository from GitHub to your local machine. Open a terminal and run:

.. code-block:: bash

   git clone https://github.com/jsoeterbroek/pictostick.git
   cd pictostick/pictodevice

Project Initialization
======================

The project includes a Python virtual environment (`penv`) which contains the specific version of the PlatformIO command-line tool used for this project. While using the VS Code extension is recommended for most tasks, you can also perform all actions from the command line.

No special initialization is required. When you open the `pictodevice` folder in VS Code with the PlatformIO extension installed, it should automatically recognize it as a PlatformIO project. It will then download the required toolchains and library dependencies as defined in the ``platformio.ini`` file.

This includes:
*   The ESP32 board support package.
*   All necessary libraries, such as WiFiManager, LVGL, and others.

You are now ready to move on to :ref:`building` the firmware.