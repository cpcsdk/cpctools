echo "Configure the scripts"

libtoolize
aclocal
autoconf
autoheader
automake -c -a
