# Purpose

This is a WIMP front-end for [`draw2svg`](https://github.com/simpsonst/draw2svg), enabling conversion from RISC OS drawfiles to [SVG](http://www.w3.org/SVG/).

Contact [Steven Simpson](https://github.com/simpsonst) for
discussion of this program.
For further updates, see [the website](https://www.lancaster.ac.uk/~simpsons/software/pkg-draw2svg).


# Building from source

The code is meant to be cross-compiled from Linux to RISC OS.
You need:

* [Binodeps](https://github.com/simpsonst/binodeps)

* [GCCSDK](https://gccsdk.riscos.info/)

* [CSWIs](https://github.com/simpsonst/cswis)

* [Yacitros](https://github.com/simpsonst/yacitros)

The makefile attempts to read from `config.mk` adjacent to it, and then from `draw2svg-fe-env.mk`.
Use either of these to define local configuration.
For example, if you have GCCSDK installed in `~/.local/opt/gccsdk`, and the libraries in `~/.local/opt/riscos-devel`, you could place this in `config.mk`:

    CPPFLAGS += -pedantic -Wall -W -Wno-unused-parameter
    CPPFLAGS += -g
    
    CFLAGS += -O3
    CFLAGS += -std=gnu11
    
    CXXFLAGS += -O2
    CXXFLAGS += -std=gnu++11
    RISCOS_PREFIX=$(HOME)/.local/opt/riscos-devel
    GCCSDK=$(HOME)/.local/opt/gccsdk
    
    CPPFLAGS += -static
    APPTYPE=ff8
    APPTYPE=e1f
    
    APPCP=$(GCCSDK)/cross/arm-unknown-riscos/bin/elf2aif
    ELF2AIF=$(APPCP)
    CMHG=$(GCCSDK)/cross/bin/cmunge
    CC=$(GCCSDK)/cross/bin/arm-unknown-riscos-gcc
    CXX=$(GCCSDK)/cross/bin/arm-unknown-riscos-g++
    AR=$(GCCSDK)/cross/bin/arm-unknown-riscos-ar
    AS=$(GCCSDK)/cross/bin/arm-unknown-riscos-as
    RANLIB=$(GCCSDK)/cross/bin/arm-unknown-riscos-ranlib
    
    ARCHIVE_DIR=$(RISCOS_PREFIX)/export
    RISCOS_ZIP=$(GCCSDK)/env/bin/zip -,
    UNZIP=$(GCCSDK)/env/bin/unzip
    
    PREFIX=$(RISCOS_PREFIX)
    LDFLAGS += -L$(RISCOS_PREFIX)/lib
    CPPFLAGS += -I$(RISCOS_PREFIX)/include

Then you can run `make` to build `out/draw2svg-fe-riscos.zip`.
`make install` will install `!Draw2SVG` directly in `$(PREFIX)/apps/`, so an emulator or a physical machine mounted on that directory can use it without having to unpack a zip file.
