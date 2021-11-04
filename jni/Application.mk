APP_PLATFORM := android-19
APP_ABI := arm64-v8a #armeabi-v7a 
APP_STL := c++_static
APP_CPPFLAGS += -std=c++11 -fvisibility=hidden -fno-exceptions -fPIC
APP_LDFLAGS := -Wl,--exclude-libs,ALL -fPIC
APP_CLANG := true
#
#MLNK_DEBUG := true
ifneq ($(MLNK_DEBUG), true)
  $(warning "RELEASE BUILD")
  APP_CPPFLAGS += -mllvm -sub -mllvm -fla -mllvm -split -mllvm -sobf -O3
  NDK_TOOLCHAIN_VERSION := ollvm
else
  $(warning "DEBUG BUILD")
  APP_CFLAGS += -DMLNK_NOISY=1 -O0 -g
endif
