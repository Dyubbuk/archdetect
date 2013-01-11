/*
 * Copyright (c) 2013 Petroules Corporation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <QtCore>
#include "mach-o.h"

QString cputypeToString(qint32 cputype)
{
    switch (cputype)
    {
    case CPU_TYPE_I386:
        return "i386";
    case CPU_TYPE_X86_64:
        return "x86_64";
    case CPU_TYPE_ARM:
        return "arm";
    case CPU_TYPE_POWERPC:
        return "ppc";
    case CPU_TYPE_POWERPC64:
        return "ppc64";
    default:
        return "unknown";
    }
}

QStringList archListFromDevice(QIODevice *device)
{
    QStringList architectureNames;
    if (!device || !device->isReadable())
        return architectureNames;

    QDataStream ds(device);
    ds.setByteOrder(QDataStream::BigEndian);

    quint32 magic;
    qint32 cpuType;
    ds >> magic;

    // fat_header and fat_arch fields are always big endian
    if (magic == FAT_MAGIC)
    {
        quint32 architectureCount;
        ds >> architectureCount;

        for (quint32 i = 0; i < architectureCount; i++)
        {
            ds >> cpuType;
            architectureNames << cputypeToString(cpuType);
            ds.skipRawData(16);
        }
    }
    else
    {
        device->seek(0);
        QDataStream ds2(device);
        ds2 >> magic;
        ds2 >> cpuType;

        switch (magic)
        {
        case MH_MAGIC:
        case MH_MAGIC_64:
            architectureNames << cputypeToString(cpuType);
            break;
        case MH_CIGAM:
        case MH_CIGAM_64:
            architectureNames << cputypeToString(qbswap(cpuType));
            break;
        }
    }

    return architectureNames;
}

QStringList archListFromFile(const QString &file)
{
    QFile dev(file);
    if (dev.open(QIODevice::ReadOnly))
    {
        return archListFromDevice(&dev);
    }

    return QStringList();
}

extern "C" const char* currentArchName();

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTextStream out(stdout);
#if Q_BYTE_ORDER == Q_BIG_ENDIAN
    QString endianString = "big";
#elif Q_BYTE_ORDER == Q_LITTLE_ENDIAN
    QString endianString = "little";
#endif
    out << "=== " << currentArchName() << ", " << endianString << " endian ===" << endl;

    QMap<QString, QStringList> fileArchMap;

    fileArchMap.insert(a.applicationFilePath(), QStringList());

    fileArchMap.insert(":/mach-o/libi386.dylib", QStringList() << "i386");
    fileArchMap.insert(":/mach-o/libx86_64.dylib", QStringList() << "x86_64");
    fileArchMap.insert(":/mach-o/libintel.dylib", QStringList() << "i386" << "x86_64");

    fileArchMap.insert(":/mach-o/libppc7400.dylib", QStringList() << "ppc");
    fileArchMap.insert(":/mach-o/libppc64.dylib", QStringList() << "ppc64");
    fileArchMap.insert(":/mach-o/libppc.dylib", QStringList() << "ppc" << "ppc64");

    fileArchMap.insert(":/mach-o/libuniversal.dylib", QStringList() << "i386" << "ppc");
    fileArchMap.insert(":/mach-o/libfulluniversal.dylib", QStringList() << "i386" << "x86_64" << "ppc" << "ppc64");

    fileArchMap.insert(":/mach-o/arm.macho", QStringList() << "arm");

    fileArchMap.insert(":/mach-o/i386.macho", QStringList() << "i386");
    fileArchMap.insert(":/mach-o/x86_64.macho", QStringList() << "x86_64");
    fileArchMap.insert(":/mach-o/intel.macho", QStringList() << "i386" << "x86_64");

    fileArchMap.insert(":/mach-o/ppc7400.macho", QStringList() << "ppc");
    fileArchMap.insert(":/mach-o/ppc64.macho", QStringList() << "ppc64");
    fileArchMap.insert(":/mach-o/ppc.macho", QStringList() << "ppc" << "ppc64");

    fileArchMap.insert(":/mach-o/universal.macho", QStringList() << "i386" << "ppc");
    fileArchMap.insert(":/mach-o/fulluniversal.macho", QStringList() << "i386" << "x86_64" << "ppc" << "ppc64");

    QMapIterator<QString, QStringList> i(fileArchMap);
    while (i.hasNext())
    {
        i.next();
        QFileInfo fi(i.key());

        // Get the list of architectured we expect to find in this file, and the list of
        // architectures that are *actually* in it, through analysis of the file
        QStringList expectedArchitectures = i.value();
        QStringList actualArchitectures = archListFromFile(fi.absoluteFilePath());

        // Sort the lists alphabetically so we can check whether the file contained
        // the same list of architectures we expected
        expectedArchitectures.sort();
        actualArchitectures.sort();

        // Empty expected architecture list is considered a successful test pass
        // since we don't know in advance what this file will contain
        if (expectedArchitectures == actualArchitectures || expectedArchitectures.isEmpty())
            out << QString("%1: PASS: %2").arg(fi.fileName()).arg(actualArchitectures.join(", ")) << endl;
        else
            out << QString("%1: FAIL: expected = %2, actual = %3").arg(fi.fileName()).arg(expectedArchitectures.join(", ")).arg(actualArchitectures.join(", ")) << endl;
    }

    return 0;
}
