#!/usr/bin/env bash

# The path to the directory used to store the reports generated during
# the executions of the unit tests.
readonly REPORTS_PATH="${__DIR__}/../tests/src/report"

# The path to the file used to store the report generated by Mtrace.
readonly REPORT_PATH="${__DIR__}/../memory-leaks.txt"
