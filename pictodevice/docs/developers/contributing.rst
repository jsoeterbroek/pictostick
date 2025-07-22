.. _contributing:

==============
Contributing
==============

We welcome contributions to the PictoStick project! This document outlines the standards and procedures for contributing.

Coding Style
============

The project uses a consistent C++ coding style enforced by **ClangFormat**. The configuration for this is defined in the ``.clang-format`` file in the root of the repository.

Before submitting any code changes, please ensure you have formatted your code by running ClangFormat. Many IDEs, including VS Code, have extensions that can do this automatically on save.

Adding New Pictos
=================

Adding a new picto to the library is a straightforward process:

1.  **Prepare the Image**:
    *   The image must be in **PNG** format.
    *   It should have a transparent background.
    *   The recommended size is **100x100 pixels**.
    *   The filename should be lowercase, descriptive, and use underscores instead of spaces (e.g., ``swimming_pool.png``).

2.  **Add to Filesystem**:
    *   Place the new PNG file into the ``data/picto/`` directory.

3.  **Update Filesystem Image**:
    *   You must now :ref:`rebuild and upload the filesystem <building>` to the device for the new picto to be available in the web interface. Run the following PlatformIO command:

    .. code-block:: bash

       penv/bin/pio run --target uploadfs

The new picto will now appear in the "Available Pictos" library in the web administration interface.

Submitting Changes
==================

All changes should be submitted via **Pull Requests** to the `main` branch of the GitHub repository.

1.  **Fork the repository** on GitHub.
2.  **Create a new branch** for your feature or bug fix.
3.  **Commit your changes** to your branch.
4.  **Push your branch** to your fork.
5.  **Open a Pull Request** from your branch to the `main` branch of the original repository.

Please provide a clear description of the changes you have made in the Pull Request.
