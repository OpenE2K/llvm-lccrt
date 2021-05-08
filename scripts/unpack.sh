#!/bin/bash

LCCRT_DEB="lccrt_0.0-vd6u165_amd64.deb"
LCCOPT_DEB="liblccopt_0.0-vd6u165_amd64.deb"
LLVM_DEB="llvm_9.0.1-vd6u51_amd64.deb"

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

cp $REPO/$LCCRT_DEB . || die "Can't find $LCCRT_DEB"
cp $REPO/$LCCOPT_DEB . || die "Can't find $LCCOPT_DEB"
cp $REPO/$LLVM_DEB  . || die "Can't find $LLVM_DEB"

unpack_deb $LCCRT_DEB
unpack_deb $LCCOPT_DEB
unpack_deb $LLVM_DEB

echo > list-files.txt

for i in `find usr/`; do
	if [ -h $i ]; then
		echo "symlink $i -> $(readlink $i)" >> list-files.txt
	else
		sha256sum $i >> list-files.txt
	fi
done
