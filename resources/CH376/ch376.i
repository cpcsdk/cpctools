; Command and structure definitions for CH376.

; Multi-byte quantities are always in little endian format (lowest byte is read first)

; I/O Registers
; =========
CMD     equ     0xFE81 ; Write: commands. Read: status byte
DAT     equ     0xFE80

; Commands
; ========

GET_IC_VER      equ     0x01
; Result: 1 byte in data register, version number | 0x40

SET_BAUDRATE    equ     0x02
; Serial port mode only?

ENTER_SLEEP     equ     0x03
; Put device into sleep mode. It will be woken up by the next command, which should not have
; parameters (use GET_IC_VER for example). Wakeup takes several milliseconds then generates
; USB_INT_WAKEUP interrupt.

RESET_ALL       equ     0x05
; Need to wait for 35ms before device is ready again? (1ms for parallel communication mode?)

CHECK_EXIST     equ     0x06
; Test that the interface exists and works.
; Input: one data byte
; Output: input ^ 0xFF

SET_SD0_INT     equ     0x0B
; SPI mode only?

GET_FILE_SIZE   equ     0x0C
; Input: 0x68 (why?)
; Output: file length on 4 bytes

;SET_FILE_SIZE   equ     ???
; Input: 0x68 + file length on 4 bytes
; Acts on open file, in CH376 RAM. Needs FILE_CLOSE to flush the changes to disk.

SET_USB_MODE    equ     0x15
; Switch between different USB modes. Mode 6 is the one to use at runtime when an USB device is
; plugged. Mode 5 may be used when waiting for a device to be plugged. Mode 7 as a transition between
; 5 and 6?
; Input:
;       00: invalid device mode (reset default)
;       01: usb device, "peripheral firmware"
;       02: usb device, "inner firmware" (bootloader update?)
;       03: SD host, manage SD cards
;       04: invalid usb host
;       05: usb host, don't generate SOF
;       06: usb host, generate SOF
;       07: usb host, bus reset
; Output:
;       0x51: success
;       0x5F: failure

GET_STATUS      equ     0x22
; Get interrupt status after an interrupt was triggered.
; Output: interrupt status (see below)

RD_USB_DATA0    equ     0x27
; Read data from interrupt port, or USB receive buffer.
; After an interrupt was triggered, get interrupt status using command 22. Then, get data (if any)
; using this command. First byte returned is the data length, then the data.
; Output: length + data

WR_USB_DATA     equ     0x2C
; Write data to transfer buffer (for raw USB mode)
; Input: length + data

WR_REQ_DATA     equ     0x2D
; Write requested data
; Used when writing to files
; Output (before input!): length of chunk to write
; Input: data to fill the requested length

WR_OFS_DATA     equ     0x2E
; Write data to buffer with offset
; Input: offset, length, data

SET_FILE_NAME   equ     0x2F
; Set file or directory name for filesystem operations
; Input: null-terminated string
; The command accepts at most 14 characters. File name must start with '/'.
; Special values:
; "": do not open anything
; "/": open root directory
; "/FOO.TXT": file in root directory
; "FOO.TXT": file in current directory

; Asynchronous commands
; =====================

; These commands have no direct output, instead they trigger an interrupt when done running.

DISK_CONNECT    equ     0x30
; Wait for USB mass storage to be connected
; Interrupt with USB_INT_SUCCESS if drive is ready.

DISK_MOUNT      equ     0x31
; Mount detected USB drive.
; Triggers USB_INT_SUCCESS and returns 36 byte drive identifier in interrupt buffer.

FILE_OPEN       equ     0x32
; Open a file or directory.
; First use SET_FILE_NAME.
; Sets interrupt status and returns file length in interrupt data.
; Opening a directory returns ERR_OPEN_DIR and set length to FFFFFFFF (does a chdir)
; Can also return ERR_MISS_FILE if the file is not found.

FILE_ENUM_GO    equ     0x33
; Enumerate next file
; Used for reading directory catalog, get next FAT32 entry
; Use CMD_SET_FILE_NAME with a pattern (eg. "/*" to list all files in root dir).
; Then use FILE_OPEN to get the first matching file.
; Interrupt status will be USB_INT_DISK_READ, data will be the FAT32 directory entry
; Then use this command to move on to the next matching file until the interrupt is ERR_MISS_FILE.

FILE_CREATE     equ     0x34
; Create a file (or truncate an existing file).
; The file must be open (you will get ERR_MISS_FILE) before creating.
; The default date is 2004/1/1 and length is 1 byte.
; USe DIR_INFO_READ and DIR_INFO_SAVE to edit the directory entry.

FILE_ERASE      equ     0x35
; Delete a file.
; Make sure the current file is closed first or it will also be deleted!
; Use SET_FILE_NAME (no wildcards allowed) then CMD_FILE_ERASE (no need to open first - unless deleting a directory?)

FILE_CLOSE      equ     0x36
; Close an open file.
; Input: 1 to update file length, 0 to leave it unchanged
; When using sector-based commands, the file size is rounded to sector size. Use CMD_SET_FILE_SIZE
; to set the exact size if needed, or modify directly with DIR_INFO_READ/DIR_INFO_SAVE

DIR_INFO_READ   equ     0x37
; Read directory info
; Input one byte which is the id of the file to get info from (in the current dir). Only the first
; 16 entries can be accessed this way!
; Otherwise, first open the file then query for entry 0xFF. The FAT entry for the currently open
; file will be returned.
; The data is returned in the interrupt stream.

DIR_INFO_SAVE   equ     0x38
; Update directory info
; After reading the info with DIR_INFO_READ, *without retrieving it by reading the interrupt stream*
; You can modify the directory entry using WR_OFS_DATA and then write it again using this command.

BYTE_LOCATE     equ     0x39
; Seek to position in file
; Input: 4 byte relative(?) file offset
; Returns USB_INT_SUCCESS with new (absolute) offset or FFFFFFFF if reached end of file.
; Moving to FFFFFFFF actually seeks to the end of the file (to write in append mode)

BYTE_READ       equ     0x3A
; Read from file
; Data is returned in chunks of 255 bytes max at a time as interrupt data, then BYTE_RD_GO must be
; used to get next chunk (as long as the interrupt status is USB_INT_DISK_READ).
; If the pointer becomes USB_INT_SUCCESS before the requested number of bytes has been read, it
; means the EOF was reached.
; Input: number of bytes to read (16 bit)

BYTE_READ_GO    equ     0x3B
; Get next chunk of data after BYTE_READ

BYTE_WRITE      equ     0x3C
; Write to file
; Triggers interupt USB_INT_DISK_WRITE. MCU should ask how much bytes to write using WR_REQ_DATA
; and send the bytes. Operation is finished when the interrupt is USB_INT_SUCCESS.
; This can automatically grow the file, size in FAT will be updated when closing the file.
BYTE_WR_GO      equ     0x3D
; Continue write operation, after a WR_REQ_DATA if the interrupt is not INT_SUCCESS yet.

DISK_CAPACITY   equ     0x3E
; Get the number of sectors on disk (interrupt return, 4 bytes).

DISK_QUERY      equ     0x3F
; Get the info about the FAT partition via interrupt data:
; 4 bytes: total number of sectors
; 4 bytes: number of free sectors
; 1 byte: partition type

DIR_CREATE      equ     0x40
; Create and open a directory (name must be set using SET_FILE_NAME).
; Open an already existing directory (does not truncate)
; Returns ERR_FOUND_NAME if the name exists but is a file
; Can only support a single level of directories?
; As with FILE_CREATE, the FAT entry can be edited (default values are the same except size is 0 and
; directory attribute is set)

SEC_LOCATE      equ     0x4A
; Seek by sector size
; Similar to byte locate, but the unit is sectors of 512 bytes, instead of single bytes.

SEC_READ        equ     0x4B
; Read by sectors
; Input: number of sectors to read (one byte)
; Output (via interrupt data):
;       4 bytes: number of available sectors
;       4 bytes: current offset in file
; Then use DISK_READ and DISK_RD_GO to read the data as usual

SEC_WRITE       equ     0x4C
; Write by sector (possibly faster?)

DISK_BOC_CMD    equ     0x50
; Send "bulk-only" raw mass storage commands to USB device.
; Can be used to send raw SCSI commands to the device (if you want to drive an USB cdrom for example)
; Use WR_HOST_DATA to write the command and read the reply from the interrupt data as usual.

DISK_READ       equ     0x54
; Read raw sector
; Input: first sector (4 bytes) + sector count (1 byte)
; Each sector is read in 8 chunks of 64 bytes (each chunk generates an interrupt), then RD_GO must
; be sent to move on to next sector. A final interrupt returns the status of the whole operation.
DISK_RD_GO      equ     0x55

DISK_WRITE      equ     0x56
; Write raw sector
; Input: first sector (4 bytes) + sector count (1 byte)
DISK_WR_GO      equ     0x57

; Interrupt status
; ================

; Bit 6 of the status port is 0 when an interrupt is pending.
; As read from command 0x22, status of interrupts (also clears the interrupt)

; 00 to 0F is for USB device mode (see CH372 docs)

USB_INT_SUCCESS equ     0x14    ; Operation sucessful, no further data
USB_INT_CONNECT equ     0x15    ; New USB device connected
USB_INT_DISCONNECT equ  0x16    ; USB device unplugged!
USB_INT_BUF_OVER   equ  0x17    ; Buffer overflow
USB_INT_USB_READY  equ  0x18    ; Device is ready

USB_INT_DISK_READ  equ  0x1D    ; Disk read operation
USB_INT_DISK_WRITE equ  0x1E    ; Disk write operation
USB_INT_DISK_ERR   equ  0x1F    ; USB storage device error

; 0x2*, 0x3*: usb device error
; bit 4: parity valid (if the bit is 0 data may be corrupt)
; Low nibble:
;       0xA: NAK
;       0xE: stalled transfer
;       xx00: timeout
;       other: PID of device

ERR_OPEN_DIR    equ     0x41    ; Tried to open a directory with FILE_OPEN ?
ERR_MISS_FILE   equ     0x42    ; File not found
ERR_FOUND_NAME  equ     0x43

ERR_DISK_DISCON equ     0x82    ; Disk disconnected
ERR_LARGE_SECTOR equ    0x84    ; Sector size is not 512 bytes
ERR_TYPE_ERROR  equ     0x92    ; Invalid partition type, reformat drive
ERR_BPB_ERROR   equ     0xA1    ; Partition not formatted
ERR_DISK_FULL   equ     0xB1
ERR_FDT_OVER    equ     0xB2    ; Directory full
ERR_FILE_CLOSE  equ     0xB4    ; Attempted operation on closed file

; 0x2*, 0x3*

