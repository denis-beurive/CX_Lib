#!/usr/bin/env bash

SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ] ; do SOURCE="$(readlink "$SOURCE")"; done
readonly __DIR__="$( cd -P "$( dirname "$SOURCE" )" && pwd )"

readonly HEADER_TARGET_DIR="/usr/local/include"

if [ ! -d "${HEADER_TARGET_DIR}/CX_Lib" ]; then mkdir "${HEADER_TARGET_DIR}/CX_Lib"; fi
cp "${__DIR__}/src/"*.h "${HEADER_TARGET_DIR}/CX_Lib"
chmod a+r "${HEADER_TARGET_DIR}/CX_Lib/"*.h
cp "${__DIR__}/lib/"* "/usr/lib"
chmod a+r "/usr/lib"/libCX_Lib.a
