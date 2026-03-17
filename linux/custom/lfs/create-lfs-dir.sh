#!/bin/bash

# LFS=/home/lfs/lfs

# sudo rm -rfv $LFS
# umask 022
# sudo mkdir -pv $LFS/{etc,var,lib64,tools} $LFS/usr/{bin,lib,sbin}

# for i in bin lib sbin; do
#   sudo ln -sv usr/$i $LFS/$i
# done

# sudo chown -v lfs $LFS/{usr/{bin,lib,sbin},var,etc,tools}
# sudo chown -v lfs $LFS/usr
# case $(uname -m) in
#   x86_64) sudo chown -v lfs $LFS/lib64 ;;
# esac