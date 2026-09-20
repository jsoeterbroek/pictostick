# TODO - PictoStick Development

## Session: Issue #53 Phase 1 - COMPLETED ✅

**Phase 1 Status**: All critical bugs fixed in commit cf4b5e67 (Sep 20, 2026)

### Completed Phase 1 Items

- [x] 1. PNG callback type mismatch - Fixed return type void → int
- [x] 2. Mode transition crash prevention - Added JSON validation
- [x] 3. Uninitialized activities array - Added null checks before access
- [x] 4. Activity index bounds checking - Added validation with auto-reset
- [x] 5. Invalid activity count handling - Added size validation

### Remaining Phase 1 Items (Require Hardware Testing)

- [ ] Test on hardware - Device not connected during development session
- [ ] Verify mode transitions work smoothly (modes 1-4)
- [ ] Confirm no crashes in normal operation

---

## Phase 2: Foundation Improvements (SHORT TERM)

**Goal**: Create solid foundation for future development

- [ ] Reduce global variables
- [ ] Refactor Preferences system
- [ ] Add settings definition system
- [ ] Create `include/app_status.h` - Status flags and management
- [ ] Create `include/config.h` - Configuration constants
- [ ] Create `include/settings.h` - Settings definition
- [ ] Create unit tests in `test/unit/test_*.cpp`
  - BLOCKED by issue #54: `test/unit/*.cpp` is never collected by `pio test`
    (default test dir is `test/`), and the root runner registers 0 tests.

---

## Blockers / Findings

- Issue #54: Unit test suite cannot run - no test files collected, 0 tests executed (OPEN)
- Issue #24: Describes code that does not exist in this repo. It references
  `getActivityFromDoc`, `initDeviceMode3`, `parseActivities`, `MAX_ACTIVITY_INDEX` and
  `ACTIVITY_NEW`. `parseActivities`, `MAX_ACTIVITY_INDEX` and `ACTIVITY_NEW` appear nowhere
  in the repository; `getActivityFromDoc` / `initDeviceMode3` / `validateDayIndex` /
  `initDefaultActivity` / `validateTime` were declared in `include/uisetup.h` with no
  implementation and no callers (now removed). The `.github/` changelog and Pull-Request
  templates claim these were implemented, but no implementation exists in `src/`. Needs a
  rewrite against the real code path (`uimain.cpp` -> `fs_helpers.cpp`), or closing.

---

## Phase 3: Core Classes (MEDIUM TERM)

**Goal**: Refactor to use proper OOP structure

- [ ] Add Application class
- [ ] Add Display class
- [ ] Add comprehensive unit tests

---

## Phase 4: Module Refactoring (LONG TERM)

**Goal**: Refactor individual display modes

- [ ] Mode 0 (Clock/Home)
- [ ] Other modes

---

## Notes

- Issue #53: CLOSED - Phase 1 completed
- Issue #39: CLOSED - PNG callback type mismatch fixed
- Issue #40: CLOSED - Activity bounds checking implemented
- Firmware upload pending: Device needs to be connected
- Next step: Hardware testing when device available
- Issue #54: test suite broken - `pio test` collects 0 test files (see Blockers above)
- Device: /dev/ttyACM0 (1a86:55d4 CH9102) is present, but esptool gets
  `[Errno 71] Protocol error` on upload - board does not answer the handshake.
