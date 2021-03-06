/************************************************************************
 *     COPYRIGHT NOTICE
 *     Copyright (c)
 *     All rights reserved.
 *
 * @file	x86flash.c
 * @brief	
 *
 * 
 *
 * @version		
 * @author     	chenss
 * @date       	2011-5-13
 *
************************************************************************/

#if defined _X86_ || defined  __x86_64__

#include "../../system.h"
#include "../../flash.h"
#include <stdio.h>
#include <string.h>

const flash_t flash = {
		.startAddr 	= 0x00,
		.endAddr	= 0xffff,
		.pageSize  	= 0x0100,
		.eraseSize 	= EARSE_SIZE,
		.writeTime 	= 5,
		.speed		= 400,
};

static const char *x86flash = "flash.bin";

#define DEBUG 0
#if DEBUG
	#define debug(fmt, args...) printf(fmt, ##args)
#else
	#define debug(fmt, args...)
#endif

bool flash_test(void);

/**
 *
 *
 *
 * @param
 * @return
 * @note
 */
void flash_init(void)
{
	FILE *fp;
	uint8_t page[flash.pageSize];
	int pageCount = (flash.endAddr - flash.startAddr + 1) / flash.pageSize;
	int i;

	memset(page, 0xffffffff, sizeof(page));

	fp = fopen(x86flash, "r+b");
	if(fp == NULL)
	{
		debug("file not exist, creating new file: %s.\n", x86flash);

		fp = fopen(x86flash, "w+b");
		if(fp == NULL)
		{
			debug("Can not create file: %s!\n", x86flash);
			return;
		}

		for (i = 0; i < pageCount; ++i)
		{
			if(fwrite(page, 1, sizeof(page), fp) != sizeof(page))
			{
				debug("Init file error!\n");
				fclose(fp);
				return;
			}
		}
	}

	fclose(fp);

	debug("Flash information:\n");
	debug("totalSize  = %u bytes\n", flash.endAddr - flash.startAddr + 1);
	debug("pageSize   = %u bytes\n", flash.pageSize);
	debug("eraseSize  = %u bytes\n", flash.eraseSize);
	debug("writeTime  = %u Ms\n", flash.writeTime);
	debug("speed      = %u KHz\n", flash.speed);

	flash_test();
}
/**
 *
 *
 *
 * @param
 * @return
 * @note
 */
#if 1
bool flash_test(void)
{
	bool stat;
	uint16_t rval, wval, bval;

	stat = flash_read(0, &rval, sizeof(rval));
	bval = rval;
	wval = ~rval;

	stat &= flash_write(0, &wval, sizeof(rval));
	stat &= flash_read(0, &rval, sizeof(rval));
	stat &= flash_write(0, &bval, sizeof(rval));

	if(stat && (wval == rval))
	{
		debug("Flash is OK.\n");
		return TRUE;
	}
	else
	{
		debug("Flash is Fail.\n");
		return FALSE;
	}
}
#endif
/*****************************************************************
 * @berif	写一系锟斤拷锟斤拷锟�
 *
 *
 *
 * @param	addr 锟街节碉拷址
 * @param	buf		 锟斤拷莼锟斤拷锟�
 * @param	len		 锟斤拷写锟斤拷锟斤拷莸某锟斤拷锟�
 * @return	锟斤拷锟斤拷确锟斤拷锟斤拷TRUE锟斤拷锟斤拷锟津返伙拷FALSE
 ******************************************************************/
bool flash_write(uint32_t addr, const void *buf, uint32_t len)
{
	FILE *wf;

	wf = fopen(x86flash, "r+b");
	if(wf == NULL)
	{
		debug("Can not open file: %s!\n", x86flash);
		return FALSE;
	}

	if(fseek(wf, addr, SEEK_SET) != 0)
	{
		debug("Can not seek file: %s!\n", x86flash);
		fclose(wf);
		return FALSE;
	}

	if(addr % flash.eraseSize == 0)
	{
		uint8_t ebuf[flash.eraseSize];

		memset(&ebuf, 0xffffffff, sizeof(ebuf));

		if(fwrite(ebuf, 1, sizeof(ebuf), wf) != sizeof(ebuf))
		{
			debug("file write error!\n");
			fclose(wf);
			return FALSE;
		}

		if(fseek(wf, addr, SEEK_SET) != 0)
		{
			debug("Can not seek file: %s!\n", x86flash);
			fclose(wf);
			return FALSE;
		}

	}

	if(fwrite(buf, 1, len, wf) != len)
	{
		debug("file write error!\n");
		fclose(wf);
		return FALSE;
	}

	fclose(wf);
	return TRUE;
}
/*****************************************************************
 * @berif	锟斤拷取一系锟斤拷锟斤拷莸锟�
 *
 *
 *
 * @param	addr 锟街节碉拷址
 * @param	buf		 锟斤拷莼锟斤拷锟�
 * @param	len		 锟斤拷锟饺★拷锟捷的筹拷锟斤拷
 * @return	锟斤拷锟斤拷确锟斤拷锟斤拷TRUE锟斤拷锟斤拷锟津返伙拷FALSE
 ******************************************************************/
bool flash_read(uint32_t addr, void *buf, uint32_t len)
{
	FILE *rf;

	rf = fopen(x86flash, "rb");
	if(rf == NULL)
	{
		debug("Can not open file: %s!\n", x86flash);
		return FALSE;
	}

	if(fseek(rf, addr, SEEK_SET) != 0)
	{
		debug("Can not seek file: %s!\n", x86flash);
		fclose(rf);
		return FALSE;
	}

	if(fread(buf, 1, len, rf) != len)
	{
		debug("file read error!\n");
		fclose(rf);
		return FALSE;
	}

	fclose(rf);
	return TRUE;
}

#endif /* _X86_ */
