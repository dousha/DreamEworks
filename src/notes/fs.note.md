QDFS - Quick & Dirty File System
================================

**WARNING: This filesystem is FOR TEST ONLY!**

If you could kindly help me developing a better filesystem, or can help me to
understand those standard filesystems, it would be appreciated. Thanks in 
advance.

You can found the hard drive image editor binary in `dist/hosttools/edhd`.
The binary is bulit on openSUSE 42.1 x86_64. If you want to build this tool,
the corresponding source code can be found in `src/hosttools/edhd.c`. *Build
it with HOST gcc unless you know what you are doing.*

-------------------------------------------------------------------------------

Generals
-------------------------------------------------------------------------------
```
+--------------------+
| Master Boot Record |
+--------------------+
|   Record Section   | -> ignore partition 
+--------------------+
|    Data Section    |
\____________________/

Record Structure
0               7     15   23         27        31 (Byte)
+---------------+------+----+----------+---------+
|name           |offset|size|attributes|timestamp|
+---------------+------+----+----------+---------+
|<---- 08 ----->|<-08->|<08>|<-- 04 -->|<-- 4 -->| (Byte)

Attributes Structure
0          7    11   14       15
+----------+-----+----+--------+
|Permission|owner|type|reserved|
+----------+-----+-------------+
|<-- 08 -->|<-4->|<---- 4 ---->| (bit)

Permission
0          7
+----------+
|rwx|rwx|rw| Owner, Group, Other
+----------+
```

```
struct record{
	char name[16];
	uint64_t offset;
	uint64_t size;
	uint32_t attribute;
	uint32_t timestamp;
};
```

The record section size is not fixed. It can be changed. It is recommended
to leave at least 1024 records (dummy record and expander are included). Or
leave 5% of disk size.

Avoid expanding record section because it is a low performance operation.
If a disk cleaning or fragment cleaning utility is implemented, section
expanding operation can be performed in the cleaning process.

For a temporary workaround, the last record can be used as a expander.
Program can create a temporary file as a part of the record section.
The permission of the expander must be set to `11111111` (aka `0xFF`) with
the type `3` (aka `(11)_2`).

The physical file address is calculated by `record section size + offset *
4Bytes`. For example, a record shows file `foo` has `offset = 0x10`, and the 
size of the record section is `0x1000`, the physical file address is 
`0x1000 + 0x10 * 4`. That means every file is 4Byte aligned.

The logical file size is simply shown in the `size`. Though the real physical 
size may be lesser than the `size` indicated.

The first record must be a dummy record, indicating the size of the record
section. The name of the dummy record must be `#REC_TBL`, the offset must be
set to `0`, the size is the record section size, the permission must be set 
to `11011011` (aka `0xDB`), and the timestamp must be `0`.

For folders, the attribute `type` is set to `1`. The `offset` is set to a 4KB
file which contains corresponing offset values of files in the folder.

For links, the attribute `type` is set to `2` (aka `(10)_2`). The `offset` is
set to the target file and the `size` is set to `0`.

For special devices file, the attribute `type` is set to `3` (aka `(11)_2`).
The `offset` and the `size` are used as data values.

Records must NOT be overlapped with each other. If the overlap is detected,
current program must abort further disk operation. The disk watchdog must set
the permission of the #0 record to `10010010` (aka `0x92`) to prevent
applications from operating the disk.

When updating an existing record, if the file size is increased and may overlap
with the next data entry, search forward to find a fit space (defined below) 
then move the data to corresponding place. This operation may result a terrible
performance.

The `fit space >= new space * 1.1`.

Bugs
-------------------------------------------------------------------------------

Since this filesystem has no backup table or whatever to prevent data 
corruption, it is very dirty and fragile. It is recommended to use it with
careness. Or just run it in a virtual machine. `qemu` is a good start.

