LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := mandibule
LOCAL_C_INCLUDES:= ../icrt ../code
LOCAL_SRC_FILES := ../mandibule.c
#LOCAL_CPPFLAGS := -fPIC
#LOCAL_CFLAGS := -fPIC
#LOCAL_WHOLE_STATIC_LIBRARIES := libc

ifeq ($(TARGET_ARCH_ABI),arm64-v8a)
LOCAL_LDFLAGS += -nostdlib
endif

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
LOCAL_LDFLAGS += -nostartfiles
endif
LOCAL_CFLAGS += -marm
LOCAL_CFLAGS += -DLJJ_NO_LEGACY_INLINE
LOCAL_CFLAGS += -D_GNU_SOURCE -std=gnu99 -nostdlib -ffreestanding -fno-common -fno-stack-protector -fomit-frame-pointer -fno-exceptions -fno-asynchronous-unwind-tables -fno-unwind-tables
# Note that by default ndk-build system doesn't support static executable,
# but option "-static" can instruct gcc to link with proper crt*o files to
# generate static executable.
LOCAL_LDFLAGS += -static

# Be aware that -Wl,--eh-frame-hdr is also needed for "-static" (at least for
# x86 and mips which use eh frames) since crt files are now in C, and the trick
# in the previous *S file to iterate all eh frames and record ranges is gone
#LOCAL_LDFLAGS += -Wl,--eh-frame-hdr
include $(BUILD_EXECUTABLE)
