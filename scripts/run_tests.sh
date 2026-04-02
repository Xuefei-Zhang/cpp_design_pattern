#!/usr/bin/env bash

set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="${ROOT}/build/tests"

rm -rf "${BUILD_DIR}"
mkdir -p "${BUILD_DIR}"

TEST_NAMES=(
  test_abstract_factory.cpp
  test_factory_method.cpp
  test_builder_pattern.cpp
  test_prototype_pattern.cpp
  test_singleton_pattern.cpp
  test_adapter_pattern.cpp
  test_bridge_pattern.cpp
  test_composite_pattern.cpp
  test_decorator_pattern.cpp
  test_facade_pattern.cpp
  test_proxy_pattern_corrected.cpp
  test_chain_of_responsibility_final.cpp
  test_command_pattern_final.cpp
  test_flyweight_pattern_final.cpp
  test_interpreter_pattern.cpp
  test_iterator_pattern.cpp
  test_mediator_pattern.cpp
  test_memento_pattern.cpp
  test_observer_pattern.cpp
  test_state_pattern.cpp
  test_strategy_pattern.cpp
  test_template_method_pattern.cpp
  test_visitor_pattern.cpp
  test_friend_classes.cpp
  test_scene_builder.cpp
  test_thread_pool.cpp
  test_enhanced_thread_pool.cpp
  test_memory_pool.cpp
  test_enhanced_memory_pool.cpp
)

for testfile in "${TEST_NAMES[@]}"; do
  test_path="$(find "${ROOT}" -path "*/tests/${testfile}" -print -quit)"

  if [[ -z "${test_path}" ]]; then
    echo "Missing expected test: ${testfile}"
    exit 1
  fi

  rel="${test_path#${ROOT}/}"
  category="$(cut -d/ -f1 <<< "${rel}")"
  pattern="$(cut -d/ -f2 <<< "${rel}")"
  test_name="${testfile%.cpp}"

  pattern_dir="$(cd "$(dirname "${test_path}")/.." && pwd)"
  include_dir="${pattern_dir}/include"

  out_dir="${BUILD_DIR}/${category}/${pattern}"
  bin="${out_dir}/${test_name}"

  mkdir -p "${out_dir}"

  echo "Compiling ${rel} -> ${bin#${ROOT}/}"
  g++ -std=c++14 -pthread -Wall -Wextra -pedantic -I"${include_dir}" "${test_path}" -o "${bin}"

  echo "Running ${bin#${ROOT}/}"
  "${bin}"
done

echo "All pattern tests passed."
