echo "Configure the scripts"

aclocal
libtoolize
autoconf
autoheader
automake -c -a
