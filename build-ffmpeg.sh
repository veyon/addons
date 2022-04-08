#!/bin/sh

sudo apt install g++-mingw-w64 nasm

for target in i686 x86_64; do
	mkdir build-$target
	cd build-$target
	../ffmpeg/configure --prefix=/usr/$target-w64-mingw32 --cross-prefix=$target-w64-mingw32- --target-os=mingw32 --arch=$target --enable-shared --disable-static --disable-everything --disable-gpl --enable-decoder=h264,rawvideo --enable-encoder=h264_mf --enable-indev=lavfi --enable-filter=nullsrc --enable-parser=h264,mpeg4video --enable-protocol=file --enable-demuxer=h264,mpegvideo,mov --enable-muxer=h264,mp4,mov
	make
	cd ..
done
