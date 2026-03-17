# Hello init

Experimenting by booting into nothing but a kernel and a simple init script that
will run busybox shell.

## Steps

Build Linux kernel:
```sh
wget https://www.kernel.org/pub/linux/kernel/v6.x/linux-6.18.10.tar.xz
tar -xf linux-6.18.10.tar.xz
cd linux-6.18.10

# Cleanup. Remove all build artifacts and config files.
make mrproper

# Create the default config file for the x86_64 architecture
make x86_64_defconfig

# Build with all cpu cores. Takes about 8 minutes.
time make -j$(nproc)

cd ..
```

Make sure sh symlink points to bash (Ubuntu default is dash):
```sh
sudo ln -sf /bin/bash /bin/sh
```

Create lfs user:
```sh
sudo groupadd lfs
sudo useradd -s /bin/bash -g lfs -m -k /dev/null lfs
sudo su - lfs

# Login takes you to the lfs home directory, so the commands are run there until exit
cat > ~/.bash_profile << "EOF"
exec env -i HOME=$HOME TERM=$TERM PS1='\u:\w\$ ' /bin/bash
EOF

cat > ~/.bashrc << "EOF"
set +h
umask 022
LFS=~/lfs
LC_ALL=POSIX
LFS_TGT=$(uname -m)-lfs-linux-gnu
PATH=/usr/bin
if [ ! -L /bin ]; then PATH=/bin:$PATH; fi
PATH=$LFS/tools/bin:$PATH
CONFIG_SITE=$LFS/usr/share/config.site
export LFS LC_ALL LFS_TGT PATH CONFIG_SITE
EOF

exit

# Disable Ubuntu bashrc for now
sudo mv -v /etc/bash.bashrc /etc/bash.bashrc.NOUSE
```

Download packages for LFS chapters 5 and 6:
```sh
sudo cp sources-wget-list /home/lfs
sudo chown lfs /home/lfs/sources-wget-list
sudo su - lfs
wget --input-file=sources-wget-list --continue --directory-prefix=$LFS/sources
```

Create the $LFS directory tree:
```sh
mkdir -pv $LFS/{etc,var,lib64,tools} $LFS/usr/{bin,lib,sbin}
for i in bin lib sbin; do
  ln -sv usr/$i $LFS/$i
done
```

Install packages to $LFS as the lfs user following the instructions in LFS chapters 5 and 6. Summaries of all the commands are in the scripts directory but the scripts are not tested.

After everything is installed, copy the $LFS directory here and turn it into a disk image: 
```sh
sudo cp /home/lfs/lfs .
./create-lfs-image.sh
```

Run with QEMU:
```sh
./run.sh
```

Exit busybox with `poweroff -f`, otherwise QEMU freezes in kernel panic:
```sh
poweroff -f
```
