cpulimit
========

[![Build Status][0]][1]

CPU usage limiter for windows (cygwin)

This program allow limit CPU usage by any other program. This can be usefull 
for example for those users, who use distributed calculations clients and 
have problems with CPU overheating.

It's a clone of [***cpulimit for linux***][2].


Build
-----

* WINDOWS

    $ `git submodule init`

    $ `git submodule update`

    It's configure to build with "Microsoft Visual C++ 2008"


* CYGWIN
    * Requirements
        * `cmake`
    * Just type $ `make`
    * You can also create a package with `make package`

Installation
------------

You can use it into cygwin.

* CYGWIN
    * $ `make install`

Help
----

$ `cpulimit --help`


TODO
----

* Tests suite
* Man
* bash conpletion
* Fix `make package` (the src tarball is outdate when there are no commit changes)


Acknowledgments
---------------

The original cpulimit was written by [***Killer***][3]

[0]: https://travis-ci.org/alquerci/cpulimit.png?branch=master
[1]: https://travis-ci.org/alquerci/cpumilit
[2]: https://github.com/opsengine/cpulimit
[3]: http://www.killprog.com
