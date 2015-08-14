#
# Copyright (C) 2012 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

ifneq ($(BUILD_TINY_ANDROID),true)

LOCAL_PATH := $(call my-dir)

# -----------------------------------------------------------------------------
# Unit tests.
# -----------------------------------------------------------------------------

ifeq ($(HOST_OS)-$(HOST_ARCH),$(filter $(HOST_OS)-$(HOST_ARCH),linux-x86 linux-x86_64))
build_host := true
else
build_host := false
endif

common_additional_dependencies := $(LOCAL_PATH)/Android.mk $(LOCAL_PATH)/Android.build.mk

# -----------------------------------------------------------------------------
# All standard tests.
# -----------------------------------------------------------------------------
test_cflags = \
    -fstack-protector-all \
    -g \
    -Wall -Wextra -Wunused \
    -Werror \
    -fno-builtin \

test_cflags += -D__STDC_LIMIT_MACROS  # For glibc.

ifeq ($(MALLOC_IMPL),dlmalloc)
test_cflags += -DUSE_DLMALLOC
else
test_cflags += -DUSE_JEMALLOC
endif

test_cppflags = \
    -std=gnu++11 \

libBionicStandardTests_src_files := \
    arpa_inet_test.cpp \
    buffer_tests.cpp \
    complex_test.cpp \
    ctype_test.cpp \
    dirent_test.cpp \
    eventfd_test.cpp \
    fcntl_test.cpp \
    fenv_test.cpp \
    ftw_test.cpp \
    getauxval_test.cpp \
    getcwd_test.cpp \
    inttypes_test.cpp \
    libc_logging_test.cpp \
    libgen_test.cpp \
    locale_test.cpp \
    malloc_test.cpp \
    math_cos_test.cpp \
    math_cosf_test.cpp \
    math_exp_test.cpp \
    math_expf_test.cpp \
    math_log_test.cpp \
    math_logf_test.cpp \
    math_pow_test.cpp \
    math_powf_test.cpp \
    math_sin_test.cpp \
    math_sinf_test.cpp \
    math_sincos_test.cpp \
    math_sincosf_test.cpp \
    math_tan_test.cpp \
    math_tanf_test.cpp \
    math_test.cpp \
    mntent_test.cpp \
    netdb_test.cpp \
    pthread_test.cpp \
    pty_test.cpp \
    regex_test.cpp \
    sched_test.cpp \
    search_test.cpp \
    signal_test.cpp \
    stack_protector_test.cpp \
    stack_unwinding_test.cpp \
    stdatomic_test.cpp \
    stdint_test.cpp \
    stdio_test.cpp \
    stdlib_test.cpp \
    string_test.cpp \
    strings_test.cpp \
    stubs_test.cpp \
    sstream_test.cpp \
    sys_epoll_test.cpp \
    sys_mman_test.cpp \
    sys_resource_test.cpp \
    sys_select_test.cpp \
    sys_sendfile_test.cpp \
    sys_socket_test.cpp \
    sys_stat_test.cpp \
    sys_statvfs_test.cpp \
    sys_syscall_test.cpp \
    sys_time_test.cpp \
    sys_types_test.cpp \
    sys_vfs_test.cpp \
    system_properties_test.cpp \
    time_test.cpp \
    uchar_test.cpp \
    uniqueptr_test.cpp \
    unistd_test.cpp \
    utmp_test.cpp \
    wchar_test.cpp \

libBionicStandardTests_cflags := \
    $(test_cflags) \

ifeq ($(MALLOC_IMPL),dlmalloc)
  libBionicStandardTests_cflags += -DUSE_DLMALLOC
else
  libBionicStandardTests_cflags += -DUSE_JEMALLOC
endif

libBionicStandardTests_cppflags := \
    $(test_cppflags) \

libBionicStandardTests_c_includes := \
    bionic/libc \

libBionicStandardTests_ldlibs_host := \
    -lrt \

module := libBionicStandardTests
module_tag := optional
build_type := target
build_target := STATIC_TEST_LIBRARY
include $(LOCAL_PATH)/Android.build.mk
build_type := host
include $(LOCAL_PATH)/Android.build.mk

# -----------------------------------------------------------------------------
# Fortify tests.
# -----------------------------------------------------------------------------
$(foreach compiler,gcc clang, \
  $(foreach test,1 2, \
    $(eval fortify$(test)-tests-$(compiler)_cflags := \
      $(test_cflags) \
      -Wno-error \
      -U_FORTIFY_SOURCE \
      -D_FORTIFY_SOURCE=$(test) \
      -DTEST_NAME=Fortify$(test)_$(compiler)); \
    $(eval fortify$(test)-tests-$(compiler)_src_files := \
      fortify_test.cpp); \
    $(eval fortify_libs += fortify$(test)-tests-$(compiler)); \
  ) \
)

module := fortify1-tests-gcc
module_tag := optional
build_type := target
build_target := STATIC_TEST_LIBRARY
include $(LOCAL_PATH)/Android.build.mk
build_type := host
include $(LOCAL_PATH)/Android.build.mk

module := fortify2-tests-gcc
module_tag := optional
build_type := target
build_target := STATIC_TEST_LIBRARY
include $(LOCAL_PATH)/Android.build.mk
build_type := host
include $(LOCAL_PATH)/Android.build.mk

fortify1-tests-clang_clang_target := true
fortify1-tests-clang_cflags_host := -D__clang__

module := fortify1-tests-clang
module_tag := optional
build_type := target
build_target := STATIC_TEST_LIBRARY
include $(LOCAL_PATH)/Android.build.mk
build_type := host
include $(LOCAL_PATH)/Android.build.mk

fortify2-tests-clang_clang_target := true

fortify2-tests-clang_cflags_host := -D__clang__

module := fortify2-tests-clang
module_tag := optional
build_type := target
build_target := STATIC_TEST_LIBRARY
include $(LOCAL_PATH)/Android.build.mk
build_type := host
include $(LOCAL_PATH)/Android.build.mk

# -----------------------------------------------------------------------------
# Library of all tests (excluding the dynamic linker tests).
# -----------------------------------------------------------------------------
libBionicTests_whole_static_libraries := \
    libBionicStandardTests \
    $(fortify_libs) \

module := libBionicTests
module_tag := optional
build_type := target
build_target := STATIC_TEST_LIBRARY
include $(LOCAL_PATH)/Android.build.mk
build_type := host
include $(LOCAL_PATH)/Android.build.mk

# -----------------------------------------------------------------------------
# Tests for the device using bionic's .so. Run with:
#   adb shell /data/nativetest/bionic-unit-tests/bionic-unit-tests32
#   adb shell /data/nativetest/bionic-unit-tests/bionic-unit-tests64
# -----------------------------------------------------------------------------
bionic-unit-tests_whole_static_libraries := \
    libBionicTests \

bionic-unit-tests_src_files := \
    atexit_test.cpp \
    dlext_test.cpp \
    dlfcn_test.cpp \

bionic-unit-tests_cflags := $(test_cflags)

bionic-unit-tests_conlyflags := \
    -fexceptions \
    -fnon-call-exceptions \

bionic-unit-tests_cppflags := $(test_cppflags)

bionic-unit-tests_ldflags := \
    -Wl,--export-dynamic \
    -Wl,-u,DlSymTestFunction \

bionic-unit-tests_c_includes := \
    bionic/libc \
    $(call include-path-for, libpagemap) \

bionic-unit-tests_shared_libraries_target := \
    libdl \
    libpagemap \

module := bionic-unit-tests
module_tag := optional
build_type := target
build_target := NATIVE_TEST
include $(LOCAL_PATH)/Android.build.mk

# -----------------------------------------------------------------------------
# Tests for the device linked against bionic's static library. Run with:
#   adb shell /data/nativetest/bionic-unit-tests-static/bionic-unit-tests-static32
#   adb shell /data/nativetest/bionic-unit-tests-static/bionic-unit-tests-static64
# -----------------------------------------------------------------------------
bionic-unit-tests-static_whole_static_libraries := \
    libBionicTests \

bionic-unit-tests-static_static_libraries := \
    libstlport_static \
    libm \
    libc \
    libstdc++ \

bionic-unit-tests-static_force_static_executable := true

module := bionic-unit-tests-static
module_tag := optional
build_type := target
build_target := NATIVE_TEST
include $(LOCAL_PATH)/Android.build.mk

# -----------------------------------------------------------------------------
# Tests to run on the host and linked against glibc. Run with:
#   cd bionic/tests; mm bionic-unit-tests-glibc-run
# -----------------------------------------------------------------------------

ifeq ($(HOST_OS)-$(HOST_ARCH),$(filter $(HOST_OS)-$(HOST_ARCH),linux-x86 linux-x86_64))

bionic-unit-tests-glibc_src_files := \
    atexit_test.cpp \
    dlfcn_test.cpp \

bionic-unit-tests-glibc_whole_static_libraries := \
    libBionicStandardTests \

bionic-unit-tests-glibc_ldlibs := \
    -lrt -ldl -lutil \

bionic-unit-tests-glibc_c_includes := \
    bionic/libc \

bionic-unit-tests-glibc_cflags := $(test_cflags)
bionic-unit-tests-glibc_cppflags := $(test_cppflags)
bionic-unit-tests-glibc_ldflags := -Wl,--export-dynamic

module := bionic-unit-tests-glibc
module_tag := optional
build_type := host
build_target := NATIVE_TEST
include $(LOCAL_PATH)/Android.build.mk

ifneq ($(TARGET_ARCH),$(filter $(TARGET_ARCH),arm mips x86))
LINKER = linker64
NATIVE_TEST_SUFFIX=64
else
LINKER = linker
NATIVE_TEST_SUFFIX=32
endif

# gtest needs ANDROID_DATA/local/tmp for death test output.
# Make sure to create ANDROID_DATA/local/tmp if doesn't exist.
# Use the current target out directory as ANDROID_DATA.
# BIONIC_TEST_FLAGS is either empty or it comes from the user.
bionic-unit-tests-glibc-run: bionic-unit-tests-glibc
	mkdir -p $(TARGET_OUT_DATA)/local/tmp
	ANDROID_DATA=$(TARGET_OUT_DATA) \
	ANDROID_ROOT=$(TARGET_OUT) \
		$(HOST_OUT_EXECUTABLES)/bionic-unit-tests-glibc$(NATIVE_TEST_SUFFIX) $(BIONIC_TEST_FLAGS)

# -----------------------------------------------------------------------------
# Run the unit tests built against x86 bionic on an x86 host.
# -----------------------------------------------------------------------------

ifeq ($(TARGET_ARCH),$(filter $(TARGET_ARCH),x86 x86_64))
# gtest needs ANDROID_DATA/local/tmp for death test output.
# Make sure to create ANDROID_DATA/local/tmp if doesn't exist.
# bionic itself should always work relative to ANDROID_DATA or ANDROID_ROOT.
# BIONIC_TEST_FLAGS is either empty or it comes from the user.
bionic-unit-tests-run-on-host: bionic-unit-tests $(TARGET_OUT_EXECUTABLES)/$(LINKER) $(TARGET_OUT_EXECUTABLES)/sh
	if [ ! -d /system -o ! -d /system/bin ]; then \
	  echo "Attempting to create /system/bin"; \
	  sudo mkdir -p -m 0777 /system/bin; \
	fi
	mkdir -p $(TARGET_OUT_DATA)/local/tmp
	cp $(TARGET_OUT_EXECUTABLES)/$(LINKER) /system/bin
	cp $(TARGET_OUT_EXECUTABLES)/sh /system/bin
	ANDROID_DATA=$(TARGET_OUT_DATA) \
	ANDROID_ROOT=$(TARGET_OUT) \
	LD_LIBRARY_PATH=$(TARGET_OUT_SHARED_LIBRARIES) \
		$(TARGET_OUT_DATA_NATIVE_TESTS)/bionic-unit-tests/bionic-unit-tests$(NATIVE_TEST_SUFFIX) $(BIONIC_TEST_FLAGS)
endif

ifeq ($(TARGET_ARCH),$(filter $(TARGET_ARCH),x86_64))
# add target to run lp32 tests
bionic-unit-tests-run-on-host32: bionic-unit-tests_32 $(TARGET_OUT_EXECUTABLES)/$(LINKER) $(TARGET_OUT_EXECUTABLES)/sh
	if [ ! -d /system -o ! -d /system/bin ]; then \
	  echo "Attempting to create /system/bin"; \
	  sudo mkdir -p -m 0777 /system/bin; \
	fi
	mkdir -p $(TARGET_OUT_DATA)/local/tmp
	cp $(TARGET_OUT_EXECUTABLES)/linker /system/bin
	cp $(TARGET_OUT_EXECUTABLES)/sh /system/bin
	ANDROID_DATA=$(TARGET_OUT_DATA) \
	ANDROID_ROOT=$(TARGET_OUT) \
	LD_LIBRARY_PATH=$(2ND_TARGET_OUT_SHARED_LIBRARIES) \
		$(2ND_TARGET_OUT_DATA_NATIVE_TESTS)/bionic-unit-tests/bionic-unit-tests32 $(BIONIC_TEST_FLAGS)
endif

endif # linux-x86

include $(call first-makefiles-under,$(LOCAL_PATH))
endif # !BUILD_TINY_ANDROID
