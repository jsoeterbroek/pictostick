# Code Quality and Structure Improvements

Here is a list of suggestions to improve the overall code quality and structure of the PictoDevice project. We can work through these items and mark them as complete.

## 1. Refactor `main.cpp` for Clarity

The `main.cpp` file, especially the `loop()` and `setup()` functions, can be made much cleaner and easier to understand.

- [ ] **Break down `loop()`:** The main `loop()` function is a large dispatcher. Break down the large `draw...()` functions (like `drawDeviceMode3()` and `drawMain()`) into smaller, more focused functions (e.g., `drawSetupUI()`, `handleSetupInput()`, `drawActivityUI()`, `handleMainInput()`).
- [ ] **Break down `setup()`:** The `setup()` function has a lot of responsibilities. Create smaller initialization functions (e.g., `init_display()`, `init_preferences()`, `init_filesystem()`) and have `setup()` call them in sequence.

## 2. Reduce Global Variables and Scope

The `common.h` file is a central hub for many global variables, which can make the code harder to maintain.

- [ ] **Group Global Status Flags:** Group status flags (`STATUS_WIFI_OK`, `STATUS_NTP_OK`, etc.) into a single `struct` or class (e.g., `AppStatus`) to make state management cleaner and dependencies more explicit.
- [ ] **Separate Configuration:** Move static configuration values (like WiFi credentials, NTP servers, screen dimensions) from `common.h` into a dedicated `config.h` file.

## 3. Improve the Preferences Module (`PSpref.cpp`)

The preferences module contains repetitive code that can be simplified.

- [ ] **Remove Repetitive `switch` Statements:** In `PSpref.cpp`, replace the large `switch` statements in `set_pspref_activity_done`, `set_pspref_activity_undone`, and `get_pspref_activity_done` with functions that use dynamically generated keys.
- [ ] **Use Arrays for Stepped Values:** Replace the `switch` statements for incrementing/decrementing brightness and timeout with arrays of predefined steps. This will make the levels easier to configure.

## 4. Architectural Suggestion: Create a Central "Application" Class

This is a larger, more structural change that would significantly professionalize the codebase.

- [ ] **Implement a `PictoDevice` Class:** Create a central `PictoDevice` class to own and manage all core components (like `tft`, `sprite`, `wm`), application state (`mode`, `status`), and logic. The `main.cpp` file would then become a very simple entry point that instantiates and runs this class.
