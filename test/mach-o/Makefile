all:
	$(CC) -mmacosx-version-min=10.4 -dynamiclib -arch i386 -o libi386.dylib lib.c
	$(CC) -mmacosx-version-min=10.4 -dynamiclib -arch x86_64 -o libx86_64.dylib lib.c
	$(CC) -mmacosx-version-min=10.4 -dynamiclib -arch i386 -arch x86_64 -o libintel.dylib lib.c

	$(CC) -mmacosx-version-min=10.4 -dynamiclib -arch ppc -o libppc7400.dylib lib.c
	$(CC) -mmacosx-version-min=10.4 -dynamiclib -arch ppc64 -o libppc64.dylib lib.c
	$(CC) -mmacosx-version-min=10.4 -dynamiclib -arch ppc -arch ppc64 -o libppc.dylib lib.c

	$(CC) -mmacosx-version-min=10.4 -dynamiclib -arch i386 -arch ppc -o libuniversal.dylib lib.c
	$(CC) -mmacosx-version-min=10.4 -dynamiclib -arch i386 -arch x86_64 -arch ppc -arch ppc64 -o libfulluniversal.dylib lib.c

	$(CC) -mmacosx-version-min=10.4 -L. -li386 -arch i386 -o i386.macho app.c
	$(CC) -mmacosx-version-min=10.4 -L. -lx86_64 -arch x86_64 -o x86_64.macho app.c
	$(CC) -mmacosx-version-min=10.4 -L. -lintel -arch i386 -arch x86_64 -o intel.macho app.c

	$(CC) -mmacosx-version-min=10.4 -L. -lppc7400 -arch ppc -o ppc7400.macho app.c
	$(CC) -mmacosx-version-min=10.4 -L. -lppc64 -arch ppc64 -o ppc64.macho app.c
	$(CC) -mmacosx-version-min=10.4 -L. -lppc -arch ppc -arch ppc64 -o ppc.macho app.c

	$(CC) -mmacosx-version-min=10.4 -L. -luniversal -arch i386 -arch ppc -o universal.macho app.c
	$(CC) -mmacosx-version-min=10.4 -L. -lfulluniversal -arch i386 -arch x86_64 -arch ppc -arch ppc64 -o fulluniversal.macho app.c

clean:
	rm -f *.macho
	rm -f *.dylib
