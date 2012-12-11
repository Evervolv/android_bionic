/*
 * Copyright (C) 2012 The Android Open Source Project
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
#ifndef LINKER_PHDR_H
#define LINKER_PHDR_H

/* Declarations related to the ELF program header table and segments.
 *
 * The design goal is to provide an API that is as close as possible
 * to the ELF spec, and does not depend on linker-specific data
 * structures (e.g. the exact layout of struct soinfo).
 */

#include "linker.h"

#ifdef __cplusplus
extern "C" {
#endif

/* See linker_phdr.c for all usage documentation */

int
phdr_table_load(int                fd,
                Elf32_Addr         phdr_offset,
                Elf32_Half         phdr_num,
                void**             phdr_mmap,
                Elf32_Addr*        phdr_size,
                const Elf32_Phdr** phdr_table);

void
phdr_table_unload(void* phdr_mmap, Elf32_Addr phdr_memsize);

Elf32_Addr
phdr_table_get_load_size(const Elf32_Phdr* phdr_table,
                         size_t phdr_count);

int
phdr_table_reserve_memory(const Elf32_Phdr* phdr_table,
                          size_t phdr_count,
                          Elf32_Addr required_base,
                          void** load_start,
                          Elf32_Addr* load_size,
                          Elf32_Addr* load_bias);

int
phdr_table_load_segments(const Elf32_Phdr* phdr_table,
                         int               phdr_count,
                         Elf32_Addr        load_bias,
                         int               fd);

int
phdr_table_protect_segments(const Elf32_Phdr* phdr_table,
                            int               phdr_count,
                            Elf32_Addr        load_bias);

int
phdr_table_unprotect_segments(const Elf32_Phdr* phdr_table,
                              int               phdr_count,
                              Elf32_Addr        load_bias);

int
phdr_table_protect_gnu_relro(const Elf32_Phdr* phdr_table,
                             int               phdr_count,
                             Elf32_Addr        load_bias);

const Elf32_Phdr*
phdr_table_get_loaded_phdr(const Elf32_Phdr*   phdr_table,
                           int                 phdr_count,
                           Elf32_Addr          load_bias);

#ifdef ANDROID_ARM_LINKER
int
phdr_table_get_arm_exidx(const Elf32_Phdr* phdr_table,
                         int               phdr_count,
                         Elf32_Addr        load_bias,
                         Elf32_Addr**      arm_exidx,
                         unsigned*         arm_exidix_count);
#endif

void
phdr_table_get_dynamic_section(const Elf32_Phdr* phdr_table,
                               int               phdr_count,
                               Elf32_Addr        load_bias,
                               Elf32_Addr**      dynamic,
                               size_t*           dynamic_count);

#ifdef __cplusplus
};
#endif

#endif /* LINKER_PHDR_H */
