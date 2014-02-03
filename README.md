SNL (Simple Network Layer)
==========================

    Version: 2.0.0 (2014-02-03)
    Purpose: Network Programming Library
    License: GPL


Brief Description
-----------------

Async network library, with built in message framing and blowfish encryption.


Detailed Description
--------------------

SNL is a very simple library that hides the more complicated details about
network programming behind a tiny C API. It is designed in a multithreaded
way to allow asyncron message handling and automatically frames messages up
to 4 GB. It supports transparent blowfish encryption by pre distributed keys
as well as UDP broadcasting and unframed TCP streams. Its main purpose is
inter-process-communication in a LAN environment or networking between
embedded devices.


Main Features
-------------

* Async IO via threads and callbacks
* Automatic message framing
* Transparent blowfish encryption
* Support for UDP broadcasts


Installation Guide
------------------

    untar the source archive            ... tar xfvz snl-<VERSION>.tar.gz
    change to the source directory      ... cd SNL
    edit the Makefile to fit your needs ... vi Makefile
    compile the library                 ... make
    create the API documentation        ... make doc
    compile the demo programms          ... make demo
    install the library and headers     ... make install
    remove binary and object files      ... make clean

    uninstall library and docs          ... make uninstall


Feedback
--------

Please report bugs and feature requests to <clemens@1541.org> or use the
issue tracker on github.
