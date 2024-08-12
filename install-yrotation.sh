make
adb shell su -c killall com.plankpad.plankpad
adb shell su -c rm /data/local/tmp/hook-yrotation.so
adb push hook-yrotation.so /data/local/tmp/
adb shell su -c chcon u:object_r:apk_data_file:s0 /data/local/tmp/hook-yrotation.so
adb shell su -c setprop wrap.com.plankpad.plankpad LD_PRELOAD=/data/local/tmp/hook-yrotation.so
