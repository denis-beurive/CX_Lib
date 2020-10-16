#!/usr/bin/env bash

SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ] ; do SOURCE="$(readlink "$SOURCE")"; done
readonly __DIR__="$( cd -P "$( dirname "$SOURCE" )" && pwd )"

. "${__DIR__}/unit-tests-config.sh"


rm -f "${REPORT_PATH}"

for f in "${REPORTS_PATH}"/*
do
  mtrace "${f}" &> /dev/null
  if [ $? -ne 0 ]; then
    echo "${f}" >> "${REPORT_PATH}"
  fi
done

if [ -f "${REPORT_PATH}" ]; then
  echo -e "\e[101mMemory leak detected!\e[49m"
  echo
  cat "${REPORT_PATH}"
  exit 1
else
  echo -e "\e[42mNo memory leak detected!\e[49m"
fi
exit 0
