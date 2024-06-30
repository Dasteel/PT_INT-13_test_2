# Linux Secret Storage Module

This repository contains Linux kernel module for storing secrets using procfs. The module provides an interface for users to write, read, and delete secrets.

## Building the Module

To build the module, run:

```sh
sudo make
```

## Loading the Module
To load the module, run:

```sh
sudo insmod secret_storage.ko
```

## Unloading the Module
To unload the module, run:
```sh
sudo rmmod secret_storage
```

## Userspace Test Program
```sh
gcc -o test_program test_program.c
./test_program
```
