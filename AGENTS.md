# Repository Guidelines

## Project Structure & Module Organization
- `src/main.cpp` hosts the HTTP API and state machine coordinating motors and recipes.
- `src/motor_control.cpp` and `include/motor_control.h` wrap stepper helpers; `src/recipe_handler.cpp` orchestrates JSON recipes loaded from SD.
- `include/` exposes shared config (`config.h` pin map, `state.h` enums, `web_page.h` static assets). Keep headers lean and prefer forward declarations.
- `lib/` is reserved for project-specific libraries; favor dependency management through `platformio.ini` when possible.
- `test/` (Unity/PlatformIO) holds unit or component tests; the stub README documents the harness expectations.
- `platformio.ini` defines the `esp32doit-devkit-v1` environment and locked dependency versions.

## Build, Test, and Development Commands
- `platformio run` — build firmware for the default ESP32 target.
- `platformio run --target upload` — flash the connected board; check the USB serial port first.
- `platformio device monitor --baud 115200` — stream serial logs for diagnostics.
- `platformio test -e esp32doit-devkit-v1` — execute Unity tests against the configured environment.
- `platformio run --target clean` — clear build artifacts after toolchain or configuration changes.

## Coding Style & Naming Conventions
Use two-space indentation and brace-on-same-line style that matches `src/`. Favor UpperCamelCase for functions (`handleStatus`), lowerCamelCase for locals, and `ALL_CAPS` for pin or state macros (see `include/config.h`). Keep user-facing strings in Spanish to align with existing UI copy. Avoid dynamic allocation in timing-sensitive paths and wrap repeated GPIO logic in helpers.

## Testing Guidelines
Place Unity test suites under `test/<feature>_test/` and mirror module names. Cover state transitions, recipe parsing, and safety interlocks using mocks while reserving hardware-in-the-loop checks for manual test notes. Run `platformio test` before pushing and capture serial evidence for motion-critical features.

## Commit & Pull Request Guidelines
Follow Conventional Commits (`feat: add homing timeout`). Group related changes together, call out dependency bumps, and document configuration updates. Pull requests should link tracking issues, summarize behavior changes, list executed commands (build, test, upload when applicable), and attach serial logs or photos validating hardware behavior. Highlight breaking changes prominently.

## Configuration & Security
Never hardcode credentials; rely on WiFiManager’s captive portal for deployment. Validate `include/config.h` pin mappings prior to flashing new boards. Recipes sourced from `/recipes.json` on the SD card should be sanity-checked for dispenser indices and volumes before being shared.
