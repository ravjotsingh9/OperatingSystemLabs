MODULE_NAME=leds
obj-m := ${MODULE_NAME}.o
all: control
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
control:
	gcc -o Controller controlleds.c

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
