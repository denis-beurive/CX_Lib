# Install

    chmod +x script/*.sh

# Compiling the library

    cmake . && make clean && make
    
# Run the unit tests

    PROJECT_ROOT=<path to the root directory of the project>
    MCHECK_REPORT=<path to the report file for MCHECK>
    DATA_DIR=<path to the directory that contains the data used for the tests>

    cmake . && make clean && make && make test

Example:

    export PROJECT_ROOT=/home/user/Projects/C/CX_Lib
    export MCHECK_REPORT=/home/user/Projects/C/CX_Lib/tests/report
    export DATA_DIR=/home/user/Projects/C/CX_Lib/tests/data/

Please note that, in order to run the unit tests, you need to install:
* [CUNIT](http://cunit.sourceforge.net/). You may need to install the package `libtool`.
  Create the `configure` script by executing `./bootstrap`. Then run `./configure && make`.
  And, finally, `sudo make install`.
* [mtrace](https://en.wikipedia.org/wiki/Mtrace).

# Annex

Installing `mtrace` on CentOS:

    sudo yum install glibc-utils.x86_64


