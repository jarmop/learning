rm -rf rootfs
rm rootfs.img

mkdir -p rootfs

cd rootfs

# mkdir -p bin dev etc proc sys tmp usr/bin usr/lib var/lib/pkg
mkdir -p bin proc etc usr lib

cp /bin/busybox bin
cp ../init bin

# add apk and its dependencies
cp ../apkdeps/apk bin
cp -r ../apkdeps/usr .
cp -r ../apkdeps/lib .

cp ../fstab etc

cd ..

dd if=/dev/zero of=rootfs.img bs=1M count=64
mkfs.ext4 rootfs.img

sudo mount rootfs.img /mnt
sudo cp -r rootfs/* /mnt
sudo umount /mnt