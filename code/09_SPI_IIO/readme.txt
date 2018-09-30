//光越強 , 值越大

root@raspberrypi:/home/pi3/09_SPI_IIO# insmod ./spi-master/spi-bcm2835.ko
[  200.707854] spi-master: bcm2835_spi_probe,787:
root@raspberrypi:/home/pi3/09_SPI_IIO# [  200.722564] industrialio: exports duplicate symbol iio_buffer_init (owned by kernel)

root@raspberrypi:/home/pi3/09_SPI_IIO# insmod ./MCP3008/mcp320x.ko
[  205.727708] mcp3008_my: mcp320x_probe,319:the name = mcp3008
[  205.734930] mcp3008_my: mcp320x_probe,324:the index = 3
[  205.742148] mcp3008_my: mcp320x_probe,352:success
root@raspberrypi:/home/pi3/09_SPI_IIO#
root@raspberrypi:/home/pi3/09_SPI_IIO# source read_adc0.sh
613
581
593
616
589
574
617
601
571
613
608
575
601
614
584
585
616
593
572
615
602
571
611
611
582
587
616
590
574
617
599
570
613
607
574
604
613
582
^C584

root@raspberrypi:/home/pi3/09_SPI_IIO# rmmod mcp320x
root@raspberrypi:/home/pi3/09_SPI_IIO# lsmod
Module                  Size  Used by
spi_bcm2835             6742  0
bnep                   10340  2
hci_uart               17943  1
btbcm                   5929  1 hci_uart
bluetooth             326067  22 bnep,btbcm,hci_uart
brcmfmac              186286  0
brcmutil                5661  1 brcmfmac
cfg80211              427817  1 brcmfmac
snd_bcm2835            20511  0
rfkill                 16018  4 cfg80211,bluetooth
snd_pcm                75890  1 snd_bcm2835
snd_timer              19160  1 snd_pcm
snd                    51908  3 snd_bcm2835,snd_timer,snd_pcm
bcm2835_gpiomem         3040  0
bcm2835_wdt             3225  0
uio_pdrv_genirq         3164  0
uio                     8000  1 uio_pdrv_genirq
ipv6                  347473  32
root@raspberrypi:/home/pi3/09_SPI_IIO# rmmod spi_bcm2835
root@raspberrypi:/home/pi3/09_SPI_IIO#
root@raspberrypi:/home/pi3/09_SPI_IIO#


[檔案修改]
原本rmmod mcp320x會panic , 因為少了spi_set_drvdata(spi,indio_dev);
