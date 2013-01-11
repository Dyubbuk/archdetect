#ifndef MACH_O_H
#define MACH_O_H

#ifndef _MACH_MACHINE_H_
#define CPU_ARCH_ABI64 0x01000000
#define CPU_TYPE_I386 7
#define	CPU_TYPE_X86_64 (CPU_TYPE_I386 | CPU_ARCH_ABI64)
#define CPU_TYPE_ARM 12
#define CPU_TYPE_POWERPC 18
#define CPU_TYPE_POWERPC64 (CPU_TYPE_POWERPC | CPU_ARCH_ABI64)
#endif

#ifndef _MACHO_LOADER_H_
#define	MH_MAGIC 0xfeedface
#define MH_CIGAM 0xcefaedfe
#define MH_MAGIC_64 0xfeedfacf
#define MH_CIGAM_64 0xcffaedfe
#endif

#ifndef _MACH_O_FAT_H_
#define FAT_MAGIC 0xcafebabe
#define FAT_CIGAM 0xbebafeca
#endif

#endif // MACH_O_H
