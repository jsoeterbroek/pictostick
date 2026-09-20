# TODO - PictoStick Development

## Session: Working on Issue #53 - Implementation Roadmap

### Phase 1: Critical Bug Fixes (IMMEDIATE)

**Goal**: Make application work reliably without crashes

- [ ] 1. PNG callback type mismatch - verify `pngDraw` function signature matches PNGdec library expectations
- [ ] 2. Fix PNG path handling in PNG SPIFFS support
- [ ] 3. Mode transition crash - ensure `mode` and `activeMode` are properly initialized
- [ ] 4. Uninitialized `activities` array - ensure JSON config is loaded before accessing
- [ ] 5. Day index out of bounds (#22) - verify array bounds checking
- [ ] 6. Missing activity file (#23) - add error handling for missing files
- [ ] 7. Invalid activity count (#24) - validate activity count from JSON

### Phase 2: Foundation Improvements (SHORT TERM)

**Goal**: Create solid foundation for future development

- [ ] Reduce global variables
- [ ] Refactor Preferences system
- [ ] Add settings definition system
- [ ] Create `include/app_status.h` - Status flags and management
- [ ] Create `include/config.h` - Configuration constants
- [ ] Create `include/settings.h` - Settings definition
- [ ] Create unit tests in `test/unit/test_*.cpp`

### Phase 3: Core Classes (MEDIUM TERM)

**Goal**: Refactor to use proper OOP structure

- [ ] Add Application class
- [ ] Add Display class
- [ ] Add comprehensive unit tests

### Phase 4: Module Refactoring (LONG TERM)

**Goal**: Refactor individual display modes

- [ ] Mode 0 (Clock/Home)
- [ ] Other modes

---

## Notes

- Issue #53 reference: https://github.com/jsoeterbroek/pictostick/issues/53
- Focus on Phase 1 items first
- Test on hardware after each fix
