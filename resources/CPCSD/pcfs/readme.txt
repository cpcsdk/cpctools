Amélie Filesystem documentation

Disk structure :
The disk is cut in volumes of 32MB (65536 sectors) each. The volumes allow some
corruption safety: if the filesystem goes crazy, only one volume is lost.

Each volume starts with a block map of 16 sectors. Each bit maps to a sector in
the volume. Bit set means the sector is allocated. The block map starts with
FF FF to make space for itself.
The bits are allocated from LSB to MSB.

So, if we look at first byte:
1 1 1 1 1 1 1 1
| | | | | | | \_ Sector 0
| | | | | | \___ Sector 1
| | | | | \_____ Sector 2
| | | | \_______ Sector 3
| | | \_________ Sector 4
| | \___________ Sector 5
| \_____________ Sector 6
\_______________ Sector 7


Next to the block map, starting at sector 16, is the file catalog. It is made
of consecutive sectors and has no size limit. When an allocated file comes in a
sector next to the file catalog, the disk is full. To solve this, some files
must be removed, then the drive defragmented. There is no plan on implementing
this, so the way out is copying everything to a new volume.

The filemap entries are 16 bytes wide and can have different formats :

0 1 2 3 4 5 6 7 8 9 A B C D E F

00000000000000000000000000000000	EMPTY ENTRY
	An empty entry is just unused space. It can be allocated for a file.

Ent L|D I R N A M E L O N G|Next	DIRECTORY ENTRY
	A directory entry. The most important part is the 11-character name. The
	characters are ASCII (7 bits), the 8th bit is reserved for flags. The name
	is space-padded and case insensitive.
	The directory is a list of other file entries. The first of these is entry
	number Ent in the catalog, and it is followed by L more entries. If L is 0,
	it is followed by 255 entries, for a total of 256. If this is not enough
	for a single directory, then Nxt points to an extended entry for this
	directory, see below.
	
	The first char is ored with 0x80 to identify a directory.

Sec L|F I L E N A M E E X T|Next	FILE ENTRY
	A file entry is just like a directory entry, but it points to sectors in
	the volume instead of pointing to other entries. The dot between file name
	and extension is not stored. The format is 8.3. A file with just one entry
	can use up to 256 sectors, or 128Kbyte. But fragmentation might happen
	long before this limit is reached, in this case Nxt points to an extended
	entry like for directories.

Ent L|Ent L|Ent L|Ent L|000|Next	EXTENDED ENTRY
Sec L|Sec L|Sec L|Sec L|000|Next
	Extended entries are allowing more storage for files and directories. They
	hold up to four sector- or entries-extents, that work just like the one in
	the first entry.
	The entries are allocated from right to left, so that a non-extended entry
	can be handled like an extended entry with the 3 first extents already used.
	You'll see what I mean, when you look at the code for this.

EOC 0|EOC 0|EOC 0|EOC 0|EOC|000|	END OF CATALOG
	This entry is always at the end of the catalog. It is always the last one
	in a sector.

If we look at all these we can see the common scheme :
NNN L|X X X X X X X X X X X|Next

And we set the following rules :
 * A free entry can be detected because Nxt = 0. This never happens for other entries.
 * For file, dirs, and extended entries, Nxt points to either another entry, or
to the entry itself. The latter marks the end of a file or directory linked list.
 * The end of catalog has Nxt = 0, like free sectors. But it has NNN pointing to
itself (or non-zero, for that matter), so it can be identified.

The first entry in the catalog is a directory called "ROOT    DIR". It is, as
you guessed, the root directory of the volume. On a freshly formatted drive,
this entry is allocated but has Nxt = 0 (itself!). Since it is ALWAYS allocated
anyway, the entry allocation algorithm just skips it when looking for free
entries.

This structure gives us a maximum of 65536 files and directories on an
unfragmented volume. They would use 2048 sectors, so the limit would be about
63000 files and 200 dirs (as each file needs at least one sector).


Block allocation algorithm

The blocks are allocated to file starting from the end of the volume. This
allows to keep as much space as possible for the catalog at the start. When the
allocated file space meets the catalog, the disk is full.

However, the extent, once allocated, is used backwards. This means the last
sector allocated is the one that will be used for the first part of the file.
This sounds a bit strange, but it allows to :
 * Have the file stocked forward in memory and use the "read multiple sector"
easily.
 * Conveniently put the allocated extents in a stack, and POP them as we use
them.


TODO LIST

Program to check disk integrity :
 * Every file is in a dir
 * Allocated xentries are all linked from a file or dir
 * No files (storage and catalog) overlap
 * Sector map matches file catalog
 * Circular/merging xentries chains

There is no concept of a current path, nor parent dir. So it is not possible to
go "up" in the filesystem. This can be added if we store the entry numbers of
the parent dirs somewhere.

RSXs:
ùCD : with no args, go to root dir. ùCD "path","to","dir" to go in path/to/dir.
This allows easy use from other programs with not too much string management,
but is a bit painful for user, so accept / syntax as well maybe

ùERA : can remove EMPTY dirs. Removing a dir that has other dir/files inside is
too complicated, and the erase code is largely crazy enough.

ùSEARCH : find a file by name on whole disk. But we need to crawl the catalog
multiple times to get the path to the file !
