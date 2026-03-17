./configure --prefix=/usr --host=$LFS_TGT

time make -j22

make DESTDIR=$LFS install