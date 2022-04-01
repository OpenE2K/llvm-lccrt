#!/bin/bash

LCCRT_DEB="lccrt_0.0-vd6u189_amd64.deb"
LCCOPT_DEB="liblccopt_0.0-vd6u189_amd64.deb"
LLVM_DEB="llvm_9.0.1-vd6u68_amd64.deb"
CLANG_DEB="clang_9.0.1-vd6u68_amd64.deb"

if [ "$1" == "" ]; then
	echo "./unpack.sh <path-to-osl-boot-disk-mountpath>"
	exit 1
fi

REPO="$1/images/debs"

if [ ! -d $REPO ]; then\
	echo "$REPO is not a directory"
	exit 2
fi

die()
{
	echo "$1"
	exit 4
}

unpack_deb()
{
	ar x $1 || die "ar x $1 failed"
	tar -xvf data.tar.xz || die "Can't unpack data tarball from $1"
	rm control.tar.gz data.tar.xz debian-binary
}

for i in $LCCRT_DEB $LCCOPT_DEB $LLVM_DEB $CLANG_DEB; do
	cp $REPO/$i . || die "Can't find $i"

	unpack_deb $i
done


echo > list-files.txt

for i in `find usr/`; do
	if [ -h $i ]; then
		echo "symlink $i -> $(readlink $i)" >> list-files.txt
	else
		sha256sum $i >> list-files.txt
	fi
done
