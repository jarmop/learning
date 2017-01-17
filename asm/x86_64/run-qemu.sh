#!/bin/bash
qemu-system-x86_64 --enable-kvm -drive format=raw,file=$1
