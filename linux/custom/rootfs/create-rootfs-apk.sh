rm -rf rootfs
rm rootfs.img

mkdir -p rootfs

cd rootfs

mkdir -p bin sbin dev proc sys tmp usr/bin usr/lib var/lib/pkg
# mkdir -p bin proc etc usr lib
# mkdir -p bin proc

cp /bin/busybox bin
cp ../init bin

# add apk and its dependencies
mkdir -p lib/apk/db
mkdir -p lib/apk/exec
cp ../apkdeps/apk sbin
cp -r ../apkdeps/usr .
cp -r ../apkdeps/lib .

mkdir -p etc
cp ../fstab etc

cd ..

# Create disk image
dd if=/dev/zero of=rootfs.img bs=1M count=64
mkfs.ext4 rootfs.img

# Copy rootfs directory contents into the disk image
sudo mount rootfs.img /mnt
sudo cp -r rootfs/* /mnt
sudo umount /mnt