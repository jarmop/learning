./configure --prefix=/usr                      \
            --build=$(sh support/config.guess) \
            --host=$LFS_TGT                    \
            --without-bash-malloc

time make -j22

make DESTDIR=$LFS install

ln -sv bash $LFS/bin/sh