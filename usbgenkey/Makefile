
# Makefile for Eagle32 DVR ioprocess app
#

include ../config

SOURCES=usbgenkey.c crypt.c

all: usbgenkey usbcheck

usbgenkey : usbgenkey.c crypt.c
	gcc $(CFLAGS_PC) -o $@ usbgenkey.c crypt.c
#	cp usbgenkey /home/jianpeng/eagle34/dvrtest/

usbcheck : usbcheck.c crypt.c
	$(CC) $(CFLAGS) -o $@  usbcheck.c crypt.c $(LDFLAGS)
#	cp usbcheck /home/jianpeng/eagle34/dvrtest/

clean :
	rm -f *.o
	rm -f usbgenkey
	rm -f usbcheck


