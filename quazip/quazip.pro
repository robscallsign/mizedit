
INCLUDEPATH+=$$(ZLIB_INCLUDE)
LIBS += -L$$(ZLIB_LIB) -lzlib
#CONFIG += staticlib

TEMPLATE=subdirs
SUBDIRS=quazip
qztest.depends = quazip
