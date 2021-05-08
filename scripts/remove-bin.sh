#!/bin/sh

for i in `find usr/`; do
	# TODO: How do I detect if file is binary or not?
	if file $i | grep "text" > /dev/null; then
		echo "Preserving $i"
	elif [ ! -d $i ]; then
		echo "Deleting $i"
		rm $i
	fi
done
