# Design Notes: Category-first repository layout

## Context
The current layout spreads pattern assets across `include/`, `tests/`, `docs/`, and `examples/`, and the root is cluttered with compiled binaries. We need a navigable structure that groups everything for a pattern in one place while keeping CI/build simple.

## Goals / Non-Goals
- Goals: category-first directories (`creational/`, `structural/`, `behavioral/`, `utilities/`); per-pattern folders containing code/tests/docs/examples; reproducible build/test commands; clean root with ignores for outputs.
- Non-Goals: changing pattern implementations or adding new patterns.

## Decisions
- Use per-pattern folders with subfolders (`include/`, `tests/`, `docs/`, `examples/`) so all artifacts live together.
- Keep category names as the first-level directories to preserve the GoF grouping already used in docs and workflows.
- Use simple g++ build commands (mirroring current flags) driven by an updated script/CI workflow instead of introducing heavier tooling.

## Risks / Trade-offs
- Large file moves can break includes if paths are missed → mitigate with scripted build/test run after moves and path updates.
- CI/workflow drift if commands are not aligned with the new layout → keep a single authoritative script shared by local and CI usage.

## Migration Plan
1) Delete generated binaries and tighten `.gitignore`.
2) Move pattern assets into the new layout, updating includes and docs.
3) Update build/test automation to reference new paths.
4) Run the full test suite and fix any path issues.

## Open Questions
- None identified; patterns are self-contained and do not share code beyond utilities.
