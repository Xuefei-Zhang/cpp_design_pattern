# Change: Refactor repository layout into category-first pattern bundles

## Why
- Root directory is cluttered with compiled binaries and scattered assets, making navigation and cleanup difficult.
- Pattern artifacts (headers, tests, docs, examples) are split across `include/`, `tests/`, `docs/`, and `examples/`, which slows down learning and maintenance.
- Build and CI scripts need to reflect a stable, predictable layout so future additions stay organized.

## What Changes
- Delete existing generated binaries and harden ignore rules to keep the workspace clean.
- Reorganize the codebase to use category-first directories (`creational/`, `structural/`, `behavioral/`, `utilities/`) with per-pattern subfolders that colocate headers, tests, docs, and examples.
- Update build tooling and CI workflows to compile/run tests from the new layout without regressions.
- Refresh developer docs/README to explain the new structure and commands.

## Impact
- Affects all pattern source/tests/docs location paths.
- Requires updates to CI workflow, README, and any scripts that reference old paths.
- No behavior changes to pattern implementations expected; structural and documentation impact only.
