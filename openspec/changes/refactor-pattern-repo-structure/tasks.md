# Tasks: Refactor repository into category-first pattern bundles

## Implementation Tasks

- [x] 1. Cleanup workspace: remove generated binaries/artifacts in repo root and expand `.gitignore` to prevent rebuild clutter.
- [x] 2. Define and create category-first layout with per-pattern folders; move headers/tests/docs/examples for each pattern into its folder.
- [x] 3. Update pattern source/tests to reference new paths (includes, assets) and ensure shared utilities remain reachable.
- [x] 4. Refresh build tooling and CI workflow to compile/tests from the new layout; keep commands reproducible locally.
- [x] 5. Update README/docs to describe the new structure and how to run tests.
- [x] 6. Run the full test matrix to confirm the refactor did not break behavior.
