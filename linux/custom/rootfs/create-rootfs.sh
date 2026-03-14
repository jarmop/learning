rm -rf rootfs
rm rootfs.img

mkdir -p rootfs

cd rootfs

# mkdir -p bin dev etc proc sys tmp usr/bin usr/lib var/lib/pkg
mkdir -p bin proc

cp /bin/busybox bin/

# cp ../init-rootfs bin/
cp ../init bin/

cd ..

dd if=/dev/zero of=rootfs.img bs=1M count=64
mkfs.ext4 rootfs.img

sudo mount rootfs.img /mnt
sudo cp -r rootfs/* /mnt
sudo umount /mnt