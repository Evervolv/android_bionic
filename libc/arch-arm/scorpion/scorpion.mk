$(call libc-add-cpu-variant-src,MEMCPY,arch-arm/scorpion/bionic/memcpy.S)
$(call libc-add-cpu-variant-src,MEMSET,arch-arm/scorpion/bionic/memset.S)
$(call libc-add-cpu-variant-src,STRCMP,arch-arm/krait/bionic/strcmp.S)
$(call libc-add-cpu-variant-src,MEMMOVE,arch-arm/krait/bionic/memmove.S)
# Use cortex-a15 versions of strcat/strcpy/strlen.
$(call libc-add-cpu-variant-src,STRCAT,arch-arm/cortex-a15/bionic/strcat.S)
$(call libc-add-cpu-variant-src,STRCPY,arch-arm/cortex-a15/bionic/strcpy.S)
$(call libc-add-cpu-variant-src,STRLEN,arch-arm/cortex-a15/bionic/strlen.S)

#include bionic/libc/arch-arm/generic/generic.mk
