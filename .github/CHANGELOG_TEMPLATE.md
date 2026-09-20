# Changelog Template

## [Version X.Y.Z] - [Date]

### 🔴 Critical Fixes

- **Issue #1:** Day index out of bounds crash
  - Added `validateDayIndex()` function
  - Fixed day index validation in all functions
  - Closes #1

- **Issue #2:** Missing activity file crash
  - Added file existence check before opening
  - Implemented fallback to default activity
  - Closes #2

- **Issue #3:** Invalid activity count crash
  - Added activity count validation
  - Fixed index validation in `getActivityFromDoc()`
  - Closes #3

- **Issue #4:** Time validation crash
  - Added time validation functions
  - Fixed time validation in all functions
  - Closes #4

### 🟡 Stability Improvements

- Activity index validation
- JSON parsing error handling
- File operation error handling

### 🟢 New Features

- [Feature description]

### 📝 Documentation

- [Documentation changes]

### 🧪 Testing

- [Testing improvements]

---

## [Version X.Y] - [Date]

### Previous changes

### [Version X.Y-1] - [Date]

### Previous changes

---

## Changelog

Please update this file for each release:

1. **Identify the fix** - Check existing issues
2. **Describe the fix** - What was changed
3. **Reference the issue** - Add "Closes #X"
4. **Test the fix** - Verify it works
5. **Update the changelog** - Document the changes

### Fix Format

```markdown
### 🔴 Critical Fixes

- **Issue #X:** [Issue title]
  - [What was changed]
  - [How it was changed]
  - Closes #X
```

### Stability Improvements Format

```markdown
### 🟡 Stability Improvements

- [Improvement 1]
- [Improvement 2]
```

### New Features Format

```markdown
### 🟢 New Features

- [Feature description]
```

### 📝 Documentation Format

```markdown
### 📝 Documentation

- [Documentation changes]
```

---

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
