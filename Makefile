obj-m := secret_storage.o

SRC_DIR := src

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all: prebuild build

prebuild:
	cd /usr/src/linux-headers-$(shell uname -r) && \
	sudo cp /usr/src/linux-source-6.1/include/generated/autoconf.h include/generated/ && \
	sudo cp /usr/src/linux-source-6.1/include/config/auto.conf include/config/

build:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

secret_storage-objs := $(SRC_DIR)/secret_storage.o

