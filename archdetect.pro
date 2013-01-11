QT += core
QT -= gui

TARGET = archdetect
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app

HEADERS += mach-o.h
SOURCES += main.cpp test/lib.c
RESOURCES += test/binaries.qrc

OTHER_FILES += \
    .gitignore \
    README.md \
    test/app.c \
    test/lib.c \
    test/mach-o/Makefile \
    test/mach-o/*.macho \
    test/mach-o/*.dylib

contains(CONFIG, ppc) {
    # Qt likes to add i386 when we're compiling for ppc, and on an Intel machine the i386
    # architecture will be run in preference to the ppc - obviously for debugging purposes
    # we only want to run the ppc version (through Rosetta or natively)
    # CONFIG -= x86 doesn't work - QTBUG-6150 related?
    QMAKE_POST_LINK += lipo -remove i386 $$OUT_PWD/$$TARGET -output $$OUT_PWD/$$TARGET
}
