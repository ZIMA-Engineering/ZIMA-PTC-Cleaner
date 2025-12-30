# Repository Guidelines

## Project Structure & Module Organization
- `src/`: Qt 4.x C++ sources and UI definitions (`*.cpp`, `*.h`, `*.ui`, `*.rc`). Entry points live in `main.cpp` and `zima-ptc-cleaner.cpp`; UI logic is centered in `ptcclean.*`.
- `gfx/` and `zima-ptc-cleaner.qrc`: Icons and other resources bundled into the Qt resource system.
- `locale/`: Translation bundles (`*.qm`) loaded at runtime; keep locale files alongside the built binary or in `locale/`.
- `bin/`: Staging spot for built artifacts if you copy them here; not populated automatically by the default build.
- Root scripts: `install.sh` and `uninstall.sh` (manual/legacy), plus `shell.nix` for a reproducible Qt toolchain.

## Build, Test, and Development Commands
- Build (Qt/qmake):  
  ```bash
  qmake -recursive zima-ptc-cleaner.pro
  make -j$(nproc)
  ```
  Generates a GUI executable `ZIMA-PTC-Cleaner` in the project root.
- Nix shell (optional): `nix develop` or `nix-shell` to enter a dev environment with Qt tooling.
- Run locally: `./ZIMA-PTC-Cleaner` (ensure translation files are discoverable per the paths above).

## Coding Style & Naming Conventions
- Language: C++ with Qt 4 widgets; tabs are used for indentation in existing files—follow the prevailing style.
- Naming: Classes in `CamelCase` (e.g., `Ptcclean`), methods in `camelCase`, and Qt slots/signals follow Qt naming (e.g., `on_action_triggered`).
- UI: `.ui` forms edited via Qt Designer; keep signal/slot hookups consistent with generated headers.
- Formatting: Match brace placement seen in `main.cpp` and `ptcclean.cpp`; prefer Qt types (`QString`, `QList`) over STL unless justified.

## Testing Guidelines
- No automated test suite is present. Perform manual validation: start the app, exercise directory selection, cleaning actions, and preference dialogs on the target platform.
- Always verify changes with a full build (`qmake -recursive zima-ptc-cleaner.pro` + `make -j$(nproc)`) before considering the work done, ensuring UI forms and resources still compile and translations load for at least one locale.

## Commit & Pull Request Guidelines
- Commits: Use concise, imperative subjects (e.g., “Add dir change validation”). Group related UI and logic updates together; avoid mixing refactors with behavior changes when possible.
- Pull requests: Describe the user-facing impact, key implementation notes, and manual test steps. Link related issues or tickets if available, and include platform specifics for any UI change.

## Localization & Assets
- When adding new UI text, update corresponding translation sources and regenerate `*.qm` files with Qt Linguist tools, keeping them in `locale/`.
- Add new icons or assets through `zima-ptc-cleaner.qrc` so they are embedded consistently across platforms.
