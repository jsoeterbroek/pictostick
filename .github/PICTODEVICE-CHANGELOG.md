# PictoDevice Changelog

## [Unreleased]

### 🔴 Critical Fixes

- **Issue #22:** Day index out of bounds crash
  - Added `validateDayIndex()` function
  - Fixed day index validation in all functions
  - Closes #22

- **Issue #23:** Missing activity file crash
  - Added file existence check before opening
  - Implemented fallback to default activity
  - Closes #23

- **Issue #24:** Invalid activity count crash
  - Added activity count validation
  - Fixed index validation in `getActivityFromDoc()`
  - Prevented out-of-bounds access
  - Closes #24

- **Issue #25:** Time validation
  - Added time validation functions
  - Fixed time validation in all functions
  - Closes #25

### 📝 Documentation

- Created PictoDevice safety issue template
- Created PictoDevice fixes pull request template
- Updated CHANGELOG_TEMPLATE.md

### 🧪 Testing

- Day index validation tests
- File not found handling tests
- Activity count validation tests
- Time validation tests

---

## [Version 1.0.0] - [Date]

### 🟢 Initial Release

- Basic activity display functionality
- Day/Time navigation
- Touch button support

---

## [Version 0.9.0] - [Date]

### 🟡 Stability Improvements

- Improved error handling
- Better file operation validation

---

## Previous Versions

### Version 0.8.0

### Version 0.7.0

---

## How to Update This Changelog

1. Update version number
2. Add date
3. Add release notes using the template
4. Use conventional commit messages

## Release Notes Template

### Version X.Y.Z (Date)

**Fixes:**
- [List critical fixes]
- [List stability improvements]

**Changes:**
- [List breaking changes]
- [List improvements]

**Testing:**
- [List testing requirements]

**Requirements:**
- [List any new requirements]

---

**Release Date:** [Date]
**Version:** X.Y.Z

## Changelog Format Guidelines

- Use emoji indicators for issue types
  - 🔴 Critical Fixes (urgent, crash-preventing)
  - 🟡 Stability Improvements (should fix)
  - 🟢 New Features (nice to have)
- Always reference issue numbers
- Keep entries concise
- Group related fixes together
- Update version numbers using semantic versioning

## Version Numbers

- **Breaking Changes:** MAJOR version increment
- **Critical Fixes:** MAJOR version increment (crash prevention)
- **Stability Improvements:** MINOR version increment
- **New Features:** MINOR version increment
- **Documentation Updates:** PATCH version increment
- **Bug Fixes (non-critical):** PATCH version increment

---

**Maintained by:** PictoDevice Team
**Last Updated:** [Date]
