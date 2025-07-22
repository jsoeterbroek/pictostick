.. _building:

========================
Building and Uploading
========================

Once your :ref:`setup` is complete, you can compile the firmware and upload it to the PictoStick device.

You can perform these actions using the PlatformIO toolbar in the VS Code status bar (at the bottom of the window) or by running the commands in the VS Code terminal.

Using the VS Code Terminal
==========================

The project is configured to use a local Python virtual environment (`penv`) for the PlatformIO CLI to ensure version consistency.

*   **On Linux/macOS**: ``penv/bin/pio``
*   **On Windows**: ``penv\\Scripts\\pio``

All commands below will use the Linux/macOS syntax.

Compiling the Firmware
----------------------

To build the project and compile the firmware, run the following command:

.. code-block:: bash

   penv/bin/pio run

This command compiles all source code and links the firmware. If successful, it will create a ``firmware.bin`` file in the project's ``.pio/build/<environment>/`` directory.

Uploading the Firmware
----------------------

To compile the firmware and upload it to a connected PictoStick device:

1.  Connect the PictoStick to your computer using the USB-C cable.
2.  Run the following command:

.. code-block:: bash

   penv/bin/pio run --target upload

PlatformIO will automatically detect the port the device is connected to and flash the new firmware.

Uploading the Filesystem Image
------------------------------

The PictoStick stores assets like pictos, web interface files (HTML, CSS), and configuration files in a separate SPIFFS (SPI Flash File System) partition on the device.

The contents of this filesystem are located in the ``data/`` directory of the project.

If you make any changes to the files in the ``data/`` directory (e.g., adding a new picto), you must rebuild and upload the filesystem image.

.. code-block:: bash

   penv/bin/pio run --target uploadfs

This will create a new filesystem image from the contents of the ``data/`` directory and flash it to the device.

All-in-One Command
==================

To clean the build, compile the firmware, and upload both the firmware and the filesystem in one go, you can run:

.. code-block:: bash

   penv/bin/pio run --target clean --target upload --target uploadfs

This is often the most convenient command to use when you want to deploy a full update to the device.
