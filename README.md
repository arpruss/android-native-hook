This is a demo of how to hook a native function to an app (com.plankpad.plankpad
in the example). 

If you install the hook, you should always run the following in a root shell on boot 
on your device: 

	chcon u:object_r:apk_data_file:s0 /data/hook-yrotation.so
    setprop wrap.com.plankpad.plankpad LD_PRELOAD=/data/hook-yrotation.so
