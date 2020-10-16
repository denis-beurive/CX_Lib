# Compiling the library

    cmake . && make clean && make
    
# Run the unit tests

    cmake . && make clean && make && make test

Please note that, in order to run the unit tests, you need to install:
* [CUNIT](http://cunit.sourceforge.net/). You may need to install the package `libtool`.
  Create the `configure` script by executing `./bootstrap`. Then run `./configure && make`.
  And, finally, `sudo make install`.
* [mtrace](https://en.wikipedia.org/wiki/Mtrace).

# Annex

Installing `mtrace` on CentOS:

    sudo yum install glibc-utils.x86_64


