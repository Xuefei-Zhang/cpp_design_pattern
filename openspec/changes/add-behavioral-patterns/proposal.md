# Change: Add Behavioral Pattern Implementations (Observer, Strategy, State)

## Why
The repository already covers many creational and structural patterns, but core behavioral examples were missing in the same implementation style. Adding Observer, Strategy, and State closes that gap with practical C++ examples and test coverage aligned to the existing pattern demos.

## What Changes
- Add `observer_patterns.h` and `test_observer_pattern.cpp` with push/pull notifications and thread-safe observer management.
- Add `strategy_patterns.h` and `test_strategy_pattern.cpp` with runtime-switchable strategy families.
- Add `state_patterns.h` and `test_state_pattern.cpp` with explicit workflow transitions and state-specific behavior.
- Add repository READMEs for the three new behavioral patterns following existing pattern README conventions.
- Update CI workflow to run a curated canonical set of stable pattern tests, including the new behavioral tests.

## Impact
- Affected specs:
  - `openspec/changes/add-behavioral-patterns/specs/observer-pattern/spec.md`
  - `openspec/changes/add-behavioral-patterns/specs/strategy-pattern/spec.md`
  - `openspec/changes/add-behavioral-patterns/specs/state-pattern/spec.md`
- Affected code:
  - `observer_patterns.h`, `strategy_patterns.h`, `state_patterns.h`
  - `test_observer_pattern.cpp`, `test_strategy_pattern.cpp`, `test_state_pattern.cpp`
  - `.github/workflows/pattern-tests.yml`
  - `OBSERVER_PATTERN_README.md`, `STRATEGY_PATTERN_README.md`, `STATE_PATTERN_README.md`
