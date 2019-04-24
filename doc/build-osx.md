Mac OS X Build Instructions and Notes
====================================
This guide will show you how to build essd (headless client) for OSX.

Notes
-----

* Tested on OS X 10.7 through 10.10 on 64-bit Intel processors only.

* All of the commands should be executed in a Terminal application. The
built-in one is located in `/Applications/Utilities`.

Preparation
-----------

You need to install XCode with all the options checked so that the compiler
and everything is available in /usr not just /Developer. XCode should be
available on your OS X installation media, but if not, you can get the
current version from https://developer.apple.com/xcode/. If you install
Xcode 4.3 or later, you'll need to install its command line tools. This can
be done in `Xcode > Preferences > Downloads > Components` and generally must
be re-done or updated every time Xcode is updated.

There's also an assumption that you already have `git` installed. If
not, it's the path of least resistance to install [Github for Mac](https://mac.github.com/)
(OS X 10.7+) or
[Git for OS X](https://code.google.com/p/git-osx-installer/). It is also
available via Homebrew.

You will also need to install [Homebrew](http://brew.sh) in order to install library
dependencies.

The installation of the actual dependencies is covered in the Instructions
sections below.

Instructions: Homebrew
----------------------

#### Install dependencies using Homebrew

        brew install autoconf automake berkeley-db4 libtool boost miniupnpc openssl pkg-config protobuf qt5 libzmq

### Building `essd`

1. Clone the github tree to get the source code and go into the directory.

        git clone https://github.com/essentiaone/essx.git
        cd essx

2.  Build essd:

        ./autogen.sh
        ./configure 
        make

3.  It is also a good idea to build and run the unit tests:

        make check

4.  (Optional) You can also install essd to your path:

        make install

Creating a release build
------------------------
You can ignore this section if you are building `essd` for your own use.

If you are building `essd` for others, your build machine should be set up
as follows for maximum compatibility:

All dependencies should be compiled with these flags:

 -mmacosx-version-min=10.7
 -arch x86_64
 -isysroot $(xcode-select --print-path)/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.7.sdk

Running
-------

It's now available at `./essd`, provided that you are still in the `src`
directory. We have to first create the RPC configuration file, though.

Run `./essd` to get the filename where it should be put, or just try these
commands:

    echo -e "rpcuser=essrpc\nrpcpassword=$(xxd -l 16 -p /dev/urandom)" > "/Users/${USER}/Library/Application Support/ESS/ess.conf"
    chmod 600 "/Users/${USER}/Library/Application Support/ESS/ess.conf"

The next time you run it, it will start downloading the blockchain, but it won't
output anything while it's doing this. This process may take several hours;
you can monitor its process by looking at the debug.log file, like this:

    tail -f $HOME/Library/Application\ Support/ESS/debug.log

Other commands:
-------

    ./essd -daemon # to start the ess daemon.
    ./ess-cli --help  # for a list of command-line options.
    ./ess-cli help    # When the daemon is running, to get a list of RPC commands
