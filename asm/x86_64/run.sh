#!/bin/bash
yasm -f bin boot.asm -o boot.bin
dd if=boot.bin of=boot.img
VBoxManage startvm boot-test
