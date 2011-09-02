#!perl
use strict;

# AMScope - analyse and colorize AMSDOS header of a file
# Copyright 2011 PulkoMandy - Shinra!

# ANSI color constants
my $z="\x1B[0m";

my $A="\x1B[41m";
my $B="\x1B[42m";
my $C="\x1B[43m";
my $D="\x1B[44m";
my $E="\x1B[45m";
my $F="\x1B[46m";

die "AMSDOScope file\nShow header of file and check checksum\n" unless $#ARGV == 0;

# Tell what we are going to do
print "\x1B[1mAnalyzing header of $ARGV[0]$z\n";

# Read the file
my $data;
my @data=`xxd -l 128 $ARGV[0]`;

# Remove the newlines
foreach(@data) {
	chomp $_;
}

# add the colors
# Note we have to start from the END of each line as we insert chars in them.
# Do not offset what we are going to touch just after...
substr($data[0],62,0,$z);
substr($data[0],59,0,$C);  # Ext
substr($data[0],51,0,$B);  # Name
substr($data[0],50,0,$A);  # User

substr($data[0],38,0,$z);
substr($data[0],31,0,$C); # File ext
substr($data[0],11,0,$B); # Filename
substr($data[0],9,0,$A);  # User


substr($data[1],62,0,$z);
substr($data[1],60,0,$A);
substr($data[1],58,0,$E);
substr($data[1],57,0,$z);
substr($data[1],55,0,$F);
substr($data[1],53,0,$E);
substr($data[1],52,0,$D);

substr($data[1],38,0,$z);
substr($data[1],34,0,$A);  # Entry
substr($data[1],33,0,$z);
substr($data[1],29,0,$E);  # Size
substr($data[1],26,0,$z);
substr($data[1],21,0,$F);  # Load
substr($data[1],16,0,$E);  # Size
substr($data[1],14,0,$D);  # Filetype

substr($data[4],55,0,$z);
substr($data[4],53,0,$C);
substr($data[4],50,0,$E);

substr($data[4],21,0,$z);
substr($data[4],16,0,$C); # CkSum
substr($data[4],9,0,$E);  # Size


# Now print ur colorized header and a legend
print "$data[0] $A User $B Name  $C Ext  $z\n";
print "$data[1] $D Type $E Size  $F Load $A Entry $z\n";
print "$data[2]\n";
print "$data[3]\n";
print "$data[4] $E Size $C CkSum $z\n";
print "$data[5]\n";
print "$data[6]\n";
print "$data[7]\n";

##########################
# Check the checksum

my $bytes;
my $sum;

# Open the file for reading
open IN, "<", $ARGV[0];
binmode IN;

# Read the data
read(IN,$bytes,67);

# Compute the sum
foreach (split(//, $bytes)) {
    $sum += ord($_);
}

# Read the file-sum
my $ifsum;
my $csum;
read(IN,$ifsum,2);

# Make it an useful comparable thing
my @s = split(//, $ifsum);
$csum = ord($s[1]) * 256 + ord($s[0]);

close IN;

# Compare !
if ($sum != $csum) {
	printf "$A Checksum error : expected %04x got %04x$z\n",$sum,$csum;
	exit -1;
} else {
	print "$B Checksum valid$z\n";
	exit 0;
}
