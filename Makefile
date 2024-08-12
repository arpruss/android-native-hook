NDK=~/ndk
#STANDALONE_TOOLCHAIN=~/ndk/bin/
target_host=$(NDK)/bin/aarch64-linux-android29
INC=../../../ndk/sysroot/usr/include/android
AR=$(target_host)-ar
AS=$(target_host)-clang
CC=$(target_host)-clang
CXX=$(target_host)-clang++
LD=$(target_host)-ld
STRIP=$(target_host)-strip

# Tell configure what flags Android requires.
CFLAGS=-pie -fPIC -O3 -landroid -I$(INC)
LDFLAGS=-pie

%.so: %.c
	$(CC) --shared $(CFLAGS) $(LDFLAGS) $(OBJECTS) $< -o $@

all: hook-yrotation.so

