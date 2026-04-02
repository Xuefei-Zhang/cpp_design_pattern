#!/bin/bash

# Build script for all design pattern tests
# Usage: ./build_all_tests.sh [category] [pattern]
#   category: creational, structural, behavioral, or all (default)
#   pattern: specific pattern name (optional)

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Base directories
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
INCLUDE_DIR="$SCRIPT_DIR/../include"
BUILD_DIR="$SCRIPT_DIR/build"

# Create build directory
mkdir -p "$BUILD_DIR"

# Counters
TOTAL=0
SUCCESS=0
FAILED=0

# Compiler flags
CXX="g++"
CXXFLAGS="-std=c++14 -I$INCLUDE_DIR -Wall -Wextra"
THREAD_FLAGS="-pthread"

# Function to compile a test file
compile_test() {
    local test_file="$1"
    local output_name="$2"
    local needs_threading="$3"

    TOTAL=$((TOTAL + 1))

    echo -n "Compiling $(basename $test_file)... "

    local flags="$CXXFLAGS"
    if [ "$needs_threading" = "true" ]; then
        flags="$flags $THREAD_FLAGS"
    fi

    if $CXX $flags "$test_file" -o "$BUILD_DIR/$output_name" 2>/dev/null; then
        echo -e "${GREEN}✓${NC}"
        SUCCESS=$((SUCCESS + 1))
        return 0
    else
        echo -e "${RED}✗${NC}"
        FAILED=$((FAILED + 1))
        return 1
    fi
}

# Function to build tests in a pattern directory
build_pattern_tests() {
    local pattern_dir="$1"
    local pattern_name=$(basename "$pattern_dir")
    local category=$(basename "$(dirname "$pattern_dir")")

    if [ ! -d "$pattern_dir" ]; then
        return
    fi

    echo -e "\n${YELLOW}Building tests for: $category/$pattern_name${NC}"

    # Find all test cpp files in the pattern directory
    while IFS= read -r test_file; do
        if [ -f "$test_file" ]; then
            local test_base=$(basename "$test_file" .cpp)
            local output_name="${pattern_name}_${test_base}"

            # Check if this test needs threading (thread_safety tests)
            local needs_threading="false"
            if [[ "$test_file" == *"thread"* ]] || [[ "$test_file" == *"concurrent"* ]]; then
                needs_threading="true"
            fi

            compile_test "$test_file" "$output_name" "$needs_threading"
        fi
    done < <(find "$pattern_dir" -maxdepth 1 -name "test_*.cpp" -o -name "*_test.cpp")
}

# Function to build all tests in a category
build_category_tests() {
    local category="$1"
    local category_dir="$SCRIPT_DIR/$category"

    if [ ! -d "$category_dir" ]; then
        echo -e "${RED}Category directory not found: $category${NC}"
        return 1
    fi

    echo -e "\n${YELLOW}═══════════════════════════════════════${NC}"
    echo -e "${YELLOW}Building $category patterns${NC}"
    echo -e "${YELLOW}═══════════════════════════════════════${NC}"

    # Find all pattern directories in the category
    for pattern_dir in "$category_dir"/*; do
        if [ -d "$pattern_dir" ]; then
            build_pattern_tests "$pattern_dir"
        fi
    done
}

# Main script
main() {
    echo -e "${YELLOW}╔════════════════════════════════════════════╗${NC}"
    echo -e "${YELLOW}║  Design Patterns Test Build Script        ║${NC}"
    echo -e "${YELLOW}╚════════════════════════════════════════════╝${NC}"

    local target="${1:-all}"

    case "$target" in
        creational|structural|behavioral)
            build_category_tests "$target"
            ;;
        all)
            build_category_tests "creational"
            build_category_tests "structural"
            build_category_tests "behavioral"
            ;;
        */*)
            # Specific pattern (e.g., creational/singleton)
            local category=$(dirname "$target")
            local pattern=$(basename "$target")
            build_pattern_tests "$SCRIPT_DIR/$category/$pattern"
            ;;
        *)
            echo -e "${RED}Unknown target: $target${NC}"
            echo "Usage: $0 [all|creational|structural|behavioral|category/pattern]"
            exit 1
            ;;
    esac

    # Summary
    echo -e "\n${YELLOW}═══════════════════════════════════════${NC}"
    echo -e "${YELLOW}Build Summary${NC}"
    echo -e "${YELLOW}═══════════════════════════════════════${NC}"
    echo "Total:   $TOTAL"
    echo -e "Success: ${GREEN}$SUCCESS${NC}"
    echo -e "Failed:  ${RED}$FAILED${NC}"
    echo ""
    echo "Binaries located in: $BUILD_DIR"

    if [ $FAILED -eq 0 ]; then
        echo -e "${GREEN}All tests built successfully!${NC}"
        exit 0
    else
        echo -e "${RED}Some tests failed to build.${NC}"
        exit 1
    fi
}

main "$@"
