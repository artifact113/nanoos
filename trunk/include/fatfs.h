//////////////////////////////////////////////////////////
// This file is a part of Nanos Copyright (C) 2008, 2009//
// ashok.s.das@gmail.com                                //
//////////////////////////////////////////////////////////
// FATFS16: Prototypes for fat16                        //
//  todo: put FAT32                                     //
//////////////////////////////////////////////////////////
#ifndef __FATFS_H__
#define __FATFS_H__
//#include "drive.h"

//begining of boot sector
typedef struct
{
	unsigned char jmp[3];    	//jump instruction to the boot strap code in boot sector
	unsigned char OEM_name[8];      //OEM name MSDOS IBMDOS etc...
} fat_jmp_oem __attribute__ ((packed));

//The following structure defines some value which is compatible to BIOS functions
typedef struct
{
	unsigned short  BytesPerSector;      /* Bytes per sector may be 512,1024,2048 or 4096*/
	unsigned char   SectorsPerCluster;   /* Must be greater than 0 and power of 2 that is 1,2,4,8,16,32,64 and 128*/
	unsigned short  ReservedSectors;     /* Must not 0. For FAT12 and FAT16 it may be 1 and for FAT32 it may be 32 */
	unsigned char   NumberFATs;          /* 1 or 2. */
	unsigned short  RootDirEntries;      /* Number of root dir entries for FAT32 it is 0*/
	unsigned short  TotalSectors16;      /* Total Number of Sectors */
	unsigned char   MediaDescriptor;     /* F0 - For removable media. F8,F9,FA,FB,FC,FD,FF are valid*/
	unsigned short  FATSize16;           /* Sectors per FAT(Size of FAT). 0 for FAT32 */
	unsigned short  SectorsPerTrack;     /* Sectors per track used by INT 13*/
	unsigned short  NumberOfHeads;       /* Number of heads used by INT 13*/
	unsigned int    HiddenSectors;       /* For partitioned media it is setted */
	unsigned int    TotalSectors32;	     /* Number of sectors if TotalSectors16 == 0 */
} BPB __attribute__ ((packed));
/*  FAT 12 or FAT 16 file system */
/*  Size of this struture is 26 */
typedef struct 
{
	unsigned char    DriveNumber;       /* 80h = harddrive else for floppy 00h */
	unsigned char    Reserved;          /* set to 0 */
	unsigned char    BootSignature;     /* 29h */
	unsigned int  	 VolumeID;          /* Volume serial number */
	unsigned char    VolumeLabel[11];   /* Volume label */
	unsigned char    FileSystem[8];     /* File system type - FAT12 , FAT16  */
} FAT12_16 __attribute__ ((packed));
/* FAT 32 file system */
/*  Size of this struture is 52 */
typedef struct
{
	unsigned int FATSize32;            /* Sectors per FAT(Size of FAT) */
	unsigned short ExtFlag;              /* bits 0-3 - contains the number of active FAT only if Mirroring is disabled. 4-6 - Reserved. 7-if 0 Mirrored to all FATs else Mirror disabled. 8-15 - reserved*/
	unsigned short FileSystemVersion;    /* Low byte minor version. High byte major version my version is 0:0 check */
	unsigned int RootCluster;	     /* Root cluster starting number */
	unsigned short FileSystemInfo;	     /* 1 */
	unsigned short BackUpBootSector;     /* 6 Indicates the Sector where back up copy of the Boot Record is stored. */
	unsigned char   Reserved[12];         /* Set to 0 */
	unsigned char   DriveNumber;	     /* Same as FAT12_16 */
	unsigned char   Reserved1;
	unsigned char   BootSignature;        /* 29h */
	unsigned int VolumeID;             /* Volume serial number */
	unsigned char   VolumeLabel[11];      /* Volume label */
	unsigned char   FileSystem[8];        /* File system FAT32 */
} FAT32 __attribute__ ((packed));

//the whole boot sector for fat12 and fat16
typedef struct
{
	fat_jmp_oem	jmp_oem;	//jump e9 or eb and jump loc and OEM name MSDOS...
	BPB		bpb;
	FAT12_16	fat_param;
	unsigned char 	boot_code[448];	//boot code in boot sector
	unsigned short  boot_sig;	//0x550xaa
}fat_12_16_bs __attribute__ ((packed));

typedef struct
{
	fat_jmp_oem	jmp_oem;
	BPB		bpb;
	FAT32		fat_param;
}fat_32_bs __attribute__ ((packed));
/* Total of this structure is 32 byte */
typedef struct 
{
	unsigned char  FileName[8];                /* File name if Name[0]=0xE5 then no file in this entry if Name[0]=0 then no files in this entry and there is no more after this one*/
	unsigned char   Extension[3];               /* Extension */
	unsigned char   Attribute;                  /* Bit coded */
	unsigned char   Reserved;                   /* For FAT12_16=0 Reseved for NT */
	unsigned char   CreationTimeTenth;	   /* For FAT12_16=0 Millisecond Time for Tenth 0-199 */
	unsigned short CreationTime;               /* For FAT12_16=0 Encoded   */
	unsigned short CreationDate;               /* For FAT12_16=0 Encoded   */
	unsigned short LastAccessDate;             /* For FAT12_16=0 */
	unsigned short FirstClusterHigh;           /* For FAT12_16=0 high word of first cluster number */
	unsigned short AccessTime;		   /* Last Accessed Time - This field is used by Dos*/
	unsigned short AccessDate;                 /* Last Accessed Time - This field is used by Dos*/
	unsigned short FirstClusterLow;            /* Low word of first cluster number */
	unsigned int FileSize;                   /* Size of file must be zero for Directory */
} direntry __attribute__ ((packed));	

class fat16
{
	private:
		fat_12_16_bs boot_sect;
		
		unsigned int FirstFATbegin;
		unsigned int SecondFATbegin;
		unsigned int RootDirbegin;
		unsigned int DataAreaBegin;
		unsigned int TotalDataSectors;
		unsigned int TotalClusters;
	public:
		void init_fat16(void *buf);
		fat16(){};
		~fat16(){};
		void fat_read(); //just a holder
		void fat_write();//just a holder
		void fat_read_dir_entry();
		void fat_write_dir_entry();
		void dump_fat_info();
		bool valid_fat_fs();

};
#endif //__FATFS_H__
