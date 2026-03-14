#!/bin/bash

make mrproper

make x86_64_defconfig

# build
time make -j"$(nproc)"