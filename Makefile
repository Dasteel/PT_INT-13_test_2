obj-m := secret_storage.o

SRC_DIR := src

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all: build

build:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

secret_storage-objs := $(SRC_DIR)/secret_storage.o
