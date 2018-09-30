make[1]: Leaving directory '/home/LinuxSource/linux'
root@raspberrypi:/home/pi3/07_ASYNC_NOTIFICATION# echo 8 > /proc/sys/kernel/printk
root@raspberrypi:/home/pi3/07_ASYNC_NOTIFICATION# insmod ./gpio.ko
[  889.164157] <1> Hello World, device mydevice INIT
[  889.170286] 'mknod /dev/mydevice c 200 0'.
[  889.175605] malloc allocator address: 0xb8910200
[  889.181491] ==== BUTTON GPIO Init
[  889.185476] ==== INTERRUPT INIT !
[  889.189495] Mapped int 189
[  889.192890] ==== INT HANDLER : schedule mytasklet
[  889.199001] r_irq_handler
roo[  889.202479] ==== TASKLET HANDLER :
t@raspberrypi:/home/pi3/07_ASYNC_NOTIFICATION# mknod /dev/mydevice c 200 0  //注意(沒此行,/dev/mydevice不會出現)
root@raspberrypi:/home/pi3/07_ASYNC_NOTIFICATION#
root@raspberrypi:/home/pi3/07_ASYNC_NOTIFICATION#
root@raspberrypi:/home/pi3/07_ASYNC_NOTIFICATION# ./test
process with pid[  920.835587] ==== FILE OPERATION: OPEN
 1867 START!
[  920.840813]
[  920.840813] *****Some body is opening me at major 200  minor 0*****
[  920.850399] FILE OPERATION: FASYNC
process open device /dev/mydevice with fd: 3
Nothing to do ... Sleeping ... ZZZzzz....
Nothing to do ... Sleeping ... ZZZzzz....
Nothing to do ... Sleeping ... ZZZzzz....
Nothing to do ... Sleeping ... ZZZzzz....
Nothing to do ... Sleeping ... ZZZzzz....
[  925.478267] ==== INT HANDLER : schedule mytasklet
[  925.484238] r_irq_handler
[  925.487558] ==== INT HANDLER : schedule mytasklet
[  925.493591] ==== TASKLET HANDLER :
accept signal w[  925.497838] ==== FILE OPERATION: READ
hich mean[  925.502963] copy_to_user: HELLO
s the dat[  925.506965] FILE OPERATION: FASYNC
a ready for rea[  925.511246] ==== FILE OPERATION: CLOSE
d:
So, I read [  925.516336] *****Some body is closing me at major 200*****
it !
msg[  925.523232] FILE OPERATION: FASYNC
 with len : 0
I accept : HELLO
I accepted something...and work done!
Now the process would exit....
root@raspberrypi:/home/pi3/07_ASYNC_NOTIFICATION# 
root@raspberrypi:/home/pi3/07_ASYNC_NOTIFICATION#
root@raspberrypi:/home/pi3/07_ASYNC_NOTIFICATION#
root@raspberrypi:/home/pi3/07_ASYNC_NOTIFICATION# rmmod gpio
[ 1354.857467] ==== INT RELEASE :
root@raspberrypi:/home/pi3/07_ASYNC_NOTIFICATION#


[檔案修改]
原本driver無法送SIGIO至AP (tasklet) , 改成workqueue 

