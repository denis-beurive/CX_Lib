#!/usr/bin/env bash

SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ] ; do SOURCE="$(readlink "$SOURCE")"; done
readonly __DIR__="$( cd -P "$( dirname "$SOURCE" )" && pwd )"

. "${__DIR__}/unit-tests-config.sh"

rm -f "${REPORTS_PATH}/"*
rm -f "${REPORT_PATH}"
