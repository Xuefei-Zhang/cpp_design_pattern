## ADDED Requirements

### Requirement: Category-first pattern layout
The repository SHALL organize GoF patterns under category directories (`creational/`, `structural/`, `behavioral/`, `utilities/`), each containing per-pattern folders with colocated `include/`, `tests/`, `docs/`, and `examples/` assets.

#### Scenario: Pattern assets are colocated
- **WHEN** a developer opens a pattern folder (e.g., `creational/abstract_factory/`)
- **THEN** they find the pattern’s headers under `include/`, tests under `tests/`, docs under `docs/`, and examples under `examples/` within that folder.

### Requirement: Build and CI respect new layout
Build scripts and CI workflows MUST compile and run pattern tests from the category-first layout using the same include paths as the colocated `include/` folders.

#### Scenario: CI runs tests from new structure
- **WHEN** the CI workflow executes
- **THEN** it compiles each pattern test from its `tests/` subfolder with includes from the colocated `include/` folder and the run passes without missing-path errors.
