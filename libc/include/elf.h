/*
 * Copyright (C) 2008 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#ifndef _ELF_H
#define _ELF_H

#include <linux/auxvec.h>
#include <linux/elf.h>
#include <linux/elf-em.h>

#include <machine/elf_machdep.h>

typedef struct {
  __u32 a_type;
  union {
    __u32 a_val;
  } a_un;
} Elf32_auxv_t;

typedef struct {
  __u64 a_type;
  union {
    __u64 a_val;
  } a_un;
} Elf64_auxv_t;

#define DF_ORIGIN     0x00000001
#define DF_SYMBOLIC   0x00000002
#define DF_TEXTREL    0x00000004
#define DF_BIND_NOW   0x00000008
#define DF_STATIC_TLS 0x00000010

#define DF_1_NOW        0x00000001 // Perform complete relocation processing.
#define DF_1_GLOBAL     0x00000002 // implies RTLD_GLOBAL
#define DF_1_GROUP      0x00000004
#define DF_1_NODELETE   0x00000008 // implies RTLD_NODELETE
#define DF_1_LOADFLTR   0x00000010
#define DF_1_INITFIRST  0x00000020
#define DF_1_NOOPEN     0x00000040 // Object can not be used with dlopen(3)
#define DF_1_ORIGIN     0x00000080
#define DF_1_DIRECT     0x00000100
#define DF_1_TRANS      0x00000200
#define DF_1_INTERPOSE  0x00000400
#define DF_1_NODEFLIB   0x00000800
#define DF_1_NODUMP     0x00001000 // Object cannot be dumped with dldump(3)
#define DF_1_CONFALT    0x00002000
#define DF_1_ENDFILTEE  0x00004000
#define DF_1_DISPRELDNE 0x00008000
#define DF_1_DISPRELPND 0x00010000
#define DF_1_NODIRECT   0x00020000
#define DF_1_IGNMULDEF  0x00040000 // Internal use
#define DF_1_NOKSYMS    0x00080000 // Internal use
#define DF_1_NOHDR      0x00100000 // Internal use
#define DF_1_EDITED     0x00200000
#define DF_1_NORELOC    0x00400000 // Internal use
#define DF_1_SYMINTPOSE 0x00800000
#define DF_1_GLOBAUDIT  0x01000000
#define DF_1_SINGLETON  0x02000000
#define DF_1_STUB       0x04000000
#define DF_1_PIE        0x08000000

#define DT_BIND_NOW 24
#define DT_INIT_ARRAY 25
#define DT_FINI_ARRAY 26
#define DT_INIT_ARRAYSZ 27
#define DT_FINI_ARRAYSZ 28
#define DT_RUNPATH 29
#define DT_FLAGS 30
/* glibc and BSD disagree for DT_ENCODING; glibc looks wrong. */
#define DT_PREINIT_ARRAY 32
#define DT_PREINIT_ARRAYSZ 33

#define ELFOSABI_SYSV 0 /* Synonym for ELFOSABI_NONE used by valgrind. */

#define PT_GNU_RELRO 0x6474e552

#define STB_LOOS      10
#define STB_HIOS      12
#define STB_LOPROC    13
#define STB_HIPROC    15

#define STT_GNU_IFUNC 10
#define STT_LOOS      10
#define STT_HIOS      12
#define STT_LOPROC    13
#define STT_HIPROC    15

#endif /* _ELF_H */
