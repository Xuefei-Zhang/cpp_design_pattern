#!/bin/bash

# Run script for all design pattern tests
# Usage: ./run_all_tests.sh [category] [pattern]

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Base directories
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$SCRIPT_DIR/build"

# Counters
TOTAL=0
PASSED=0
FAILED=0

# Function to run a test
run_test() {
    local test_executable="$1"
    local test_name=$(basename "$test_executable")

    TOTAL=$((TOTAL + 1))

    echo -e "\n${BLUE}╔════════════════════════════════════════════════════════╗${NC}"
    echo -e "${BLUE}║ Running: $test_name${NC}"
    echo -e "${BLUE}╚════════════════════════════════════════════════════════╝${NC}\n"

    if [ ! -f "$test_executable" ]; then
        echo -e "${RED}✗ Test executable not found: $test_name${NC}"
        echo -e "${YELLOW}  Run build_all_tests.sh first to compile tests${NC}\n"
        FAILED=$((FAILED + 1))
        return 1
    fi

    if [ ! -x "$test_executable" ]; then
        chmod +x "$test_executable"
    fi

    # Run the test and capture output
    if "$test_executable"; then
        echo -e "\n${GREEN}✓ $test_name PASSED${NC}"
        PASSED=$((PASSED + 1))
        return 0
    else
        echo -e "\n${RED}✗ $test_name FAILED${NC}"
        FAILED=$((FAILED + 1))
        return 1
    fi
}

# Function to run all tests in a pattern directory
run_pattern_tests() {
    local pattern_name="$1"

    # Find all test executables for this pattern
    while IFS= read -r test_exe; do
        if [ -f "$test_exe" ]; then
            run_test "$test_exe"
        fi
    done < <(find "$BUILD_DIR" -maxdepth 1 -name "${pattern_name}_test_*" -type f -executable)
}

# Function to run all tests in a category
run_category_tests() {
    local category="$1"

    echo -e "\n${YELLOW}═══════════════════════════════════════${NC}"
    echo -e "${YELLOW}Running $category pattern tests${NC}"
    echo -e "${YELLOW}═══════════════════════════════════════${NC}"

    # Find all pattern test executables for this category
    while IFS= read -r test_exe; do
        if [ -f "$test_exe" ] && [ -x "$test_exe" ]; then
            run_test "$test_exe"
        fi
    done < <(find "$BUILD_DIR" -maxdepth 1 -name "*_test_*" -type f | sort)
}

# Function to run all tests
run_all_tests() {
    if [ ! -d "$BUILD_DIR" ]; then
        echo -e "${RED}Build directory not found: $BUILD_DIR${NC}"
        echo -e "${YELLOW}Run build_all_tests.sh first to compile tests${NC}"
        exit 1
    fi

    echo -e "${YELLOW}═══════════════════════════════════════${NC}"
    echo -e "${YELLOW}Running all tests${NC}"
    echo -e "${YELLOW}═══════════════════════════════════════${NC}"

    # Find all test executables
    while IFS= read -r test_exe; do
        if [ -f "$test_exe" ] && [ -x "$test_exe" ]; then
            run_test "$test_exe"
        fi
    done < <(find "$BUILD_DIR" -maxdepth 1 -type f -executable | sort)
}

# Main script
main() {
    echo -e "${YELLOW}╔════════════════════════════════════════════╗${NC}"
    echo -e "${YELLOW}║  Design Patterns Test Runner              ║${NC}"
    echo -e "${YELLOW}╚════════════════════════════════════════════╝${NC}"

    local target="${1:-all}"

    if [ ! -d "$BUILD_DIR" ] || [ -z "$(ls -A "$BUILD_DIR" 2>/dev/null)" ]; then
        echo -e "\n${YELLOW}No compiled tests found.${NC}"
        echo -e "${YELLOW}Building tests first...${NC}\n"
        "$SCRIPT_DIR/build_all_tests.sh" "$target"
        echo ""
    fi

    case "$target" in
        all)
            run_all_tests
            ;;
        creational|structural|behavioral)
            run_category_tests "$target"
            ;;
        */*)
            # Specific pattern (e.g., creational/singleton)
            local pattern=$(basename "$target")
            run_pattern_tests "$pattern"
            ;;
        *)
            # Try as pattern name
            run_pattern_tests "$target"
            ;;
    esac

    # Summary
    echo -e "\n${YELLOW}╔════════════════════════════════════════════╗${NC}"
    echo -e "${YELLOW}║           Test Results Summary             ║${NC}"
    echo -e "${YELLOW}╚════════════════════════════════════════════╝${NC}"
    echo -e "Total tests run: $TOTAL"
    echo -e "${GREEN}Passed:         $PASSED${NC}"
    echo -e "${RED}Failed:         $FAILED${NC}"

    if [ $TOTAL -eq 0 ]; then
        echo -e "\n${YELLOW}No tests were run.${NC}"
        echo -e "Usage: $0 [all|creational|structural|behavioral|pattern_name]"
        exit 1
    fi

    if [ $FAILED -eq 0 ]; then
        echo -e "\n${GREEN}╔════════════════════════════════════════════╗${NC}"
        echo -e "${GREEN}║     ALL TESTS PASSED! ✓                    ║${NC}"
        echo -e "${GREEN}╚════════════════════════════════════════════╝${NC}"
        exit 0
    else
        echo -e "\n${RED}╔════════════════════════════════════════════╗${NC}"
        echo -e "${RED}║     SOME TESTS FAILED ✗                    ║${NC}"
        echo -e "${RED}╚════════════════════════════════════════════╝${NC}"
        exit 1
    fi
}

main "$@"
