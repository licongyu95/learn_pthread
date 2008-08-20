#ifdef WIN32

#define _WIN32_WINNT 0x0501

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>

#ifndef off_t
#define off_t unsigned
#endif

#else
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <string.h>
#include <fcntl.h>
#endif//

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif //min

unsigned int	g_flush_flag = 0;


void print_current_time()
{
	SYSTEMTIME	stTime;
	GetLocalTime(&stTime);
	printf("当前时间为：%4d年%2d月%2d日 %2d:%2d:%2d\n", stTime.wYear, stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond);
}

void pause_exit(int e)
{
	printf("press any key to exit(%d)",e);
	getch();
	exit(e);
}

//unsigned long g_show_times = 0;
void show_msg(unsigned long lba)
{
//	g_show_times++;
//	if( (g_show_times%2) == 0  )
	{
		printf("\rLBA:%lx",lba);
	}
	return ;
}

int getfilehandle(char *name)
{
	int		fd;
#ifdef WIN32	
	fd = (int)CreateFile(name,GENERIC_ALL,FILE_SHARE_WRITE|FILE_SHARE_READ,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL|FILE_FLAG_NO_BUFFERING|FILE_FLAG_WRITE_THROUGH,
				NULL);
	if( fd == (int)INVALID_HANDLE_VALUE )
	{
		printf("open failed with error code:0x%x.\n",GetLastError() );
		pause_exit(1);
	}
#else
//	fd = open("./direct_io.data", O_WRONLY | O_CREAT, 0755);
//	fd = open( name , O_RDWR | O_DIRECT , 0755 );
	fd = open( name , O_RDWR | O_DIRECT );
//	fd = open( name , O_RDWR );
	if (fd < 0)
	{
		perror("open failed");
		pause_exit(1);
	}
#endif//
	return fd;
}

#ifdef WIN32
int posix_memalign(void **memptr, size_t alignment, size_t size)
{
	*memptr = (void *)VirtualAlloc(NULL,size,MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE );
	if( *memptr == NULL )
	{
		printf("VirtualAlloc failed..");
		return -1;
	}else
	{
		return 0;
	}
}
int x_random()
{
	int		r = GetTickCount() * rand();
	if( 0 == r )
	{
		r = 6;
	}
	return r;
}
#endif//

#ifdef WIN32
//
#ifndef U64	
#define U64 ULONGLONG
#endif//

#else

//linux
#ifndef U64	
#define U64 unsigned long
#endif//

#endif//

#define U32	unsigned int

U64 api_getsize(int fd)
{
#ifdef WIN32
	LARGE_INTEGER	llfilesize;
	llfilesize.QuadPart = 0;
	if( !GetFileSizeEx((HANDLE)fd,&llfilesize) )
	{
		DWORD	byRet = 0;
		DeviceIoControl((HANDLE)fd,FSCTL_ALLOW_EXTENDED_DASD_IO,NULL,0,NULL,0,&byRet,NULL);
		
		if( !DeviceIoControl(
			(HANDLE)fd,
			IOCTL_DISK_GET_LENGTH_INFO,
			NULL,
			0,
			&llfilesize,
			sizeof(llfilesize),
			&byRet,
			NULL) )
		{
			printf("GetFileSizeEx error:%d\n" , GetLastError() );
			return 0;
		}
		return llfilesize.QuadPart;
	}
	return llfilesize.QuadPart;
#else
	U64		filesize;
	filesize = api_lseek(fd,0,SEEK_END);
	api_lseek(fd,0,SEEK_SET);
	return filesize;
#endif//	
}


U64 api_lseek(int fildes, U64 offset, int whence)
{
#ifdef WIN32
	/*
   0        SEEK_SET
       1        SEEK_CUR
       2        SEEK_END

FILE_BEGIN
0
The starting point is zero or the beginning of the file. If this flag is specified, then the liDistanceToMove parameter is interpreted as an unsigned value.
FILE_CURRENT
1
The start point is the current value of the file pointer.
FILE_END*/
	LARGE_INTEGER	newpost;
	LARGE_INTEGER	oldpost;

	oldpost.QuadPart = offset;

	if( !SetFilePointerEx(
			(HANDLE)fildes,//_In_  HANDLE hFile,
			oldpost,//_In_       LARGE_INTEGER liDistanceToMove,
			&newpost,//_Out_opt_  PLARGE_INTEGER lpNewFilePointer,
			whence // same as linux
			) )
	{
		printf("SetFilePointerEx error:0x%x.\n",GetLastError() );
		pause_exit(1);
	}
	return newpost.QuadPart;
#else
	 return lseek(fildes,offset,whence);
#endif
}

int api_read(int fd,char *buf,off_t size)
{
#ifdef WIN32	
	if( !ReadFile(
			(HANDLE)fd,//_In_         HANDLE hFile,
			buf,//_Out_        LPVOID lpBuffer,
			size,//_In_         DWORD nNumberOfBytesToRead,
			&size,//_Out_opt_    LPDWORD lpNumberOfBytesRead,
			NULL//_Inout_opt_  LPOVERLAPPED lpOverlapped
		) )
	{
		printf("ReadFile error:0x%x.\n",GetLastError() );
		return -1;
	}
	return size;
#else
	return read(fd, buf, size );
#endif//	
}

int api_write(int fd,char *buf,off_t size)
{
#ifdef WIN32	
	if( !WriteFile(
			(HANDLE)fd,//_In_         HANDLE hFile,
			buf,//_Out_        LPVOID lpBuffer,
			size,//_In_         DWORD nNumberOfBytesToRead,
			&size,//_Out_opt_    LPDWORD lpNumberOfBytesRead,
			NULL//_Inout_opt_  LPOVERLAPPED lpOverlapped
		) )
	{
		return -1;
	}
	return size;
#else
	return write(fd, buf, size );
#endif//	
}

#define COUNT_64BIT_PER_SECTOR	(0X200/sizeof(unsigned long))

int format_disk(int fd,unsigned long	total_sec)
{
	unsigned 		i;
	int				ret;
	unsigned char *	buf;
	unsigned 		sec_per_io;
	unsigned 		tmp_sec_per_io;
	unsigned long	lba;

	printf("start format disk.\n");

	sec_per_io = 0x1000;
	ret = posix_memalign((void **)&buf, 0x1000, sec_per_io*0x200 );
	if (ret) 
	{
		perror("posix_memalign failed");
		pause_exit(1);
	}

	api_lseek(fd, 0x200, SEEK_SET);
	memset(buf,0x66,sec_per_io*0x200);
	for( lba = 1 ;  lba < total_sec ; lba+= tmp_sec_per_io )
	{
		show_msg(lba);
		tmp_sec_per_io = min(total_sec-lba, sec_per_io);
		for( i = 0 ; i < tmp_sec_per_io ; i++ )
		{
			*(unsigned long *)(buf + i*0x200 ) = lba+i;
		}
		ret = api_write(fd, buf, tmp_sec_per_io*0x200 );
		if (ret < 0) 
		{
			perror("error:");
			printf("write LBA:0x%lx error",lba);
			pause_exit(1);
		}
	}
	api_lseek(fd, 0, SEEK_SET);
	*(unsigned long *)(buf) = 0;
	ret = api_write(fd, buf, 0x200 );
	if (ret < 0) 
	{
		perror("error:");
		printf("write LBA:0x%lx error",0);
		pause_exit(1);
	}
	return 0;
}

int check_disk(int fd,unsigned long	total_sec)
{
	unsigned 		i,n;
	int				ret;
	unsigned char *	buf;
	unsigned long  * cmpbuf;
	unsigned 		sec_per_io;
	unsigned 		tmp_sec_per_io;
	unsigned long	lba;

	printf("\nstart check data.\n");


	sec_per_io = 0x1000;

	ret = posix_memalign((void **)&buf, 0x1000, sec_per_io*0x200 );
	if (ret) 
	{
		perror("posix_memalign failed");
		pause_exit(1);
	}
	cmpbuf = (unsigned long *)buf;
	api_lseek(fd, 0, SEEK_SET);
	for( lba = 0;  lba < total_sec ; lba+= tmp_sec_per_io )
	{
		show_msg(lba);
		tmp_sec_per_io = min(total_sec-lba, sec_per_io);
		ret = api_read(fd, buf, tmp_sec_per_io*0x200 );
		if (ret < 0) 
		{
			perror("error:");
			printf("read LBA:0x%lx error",lba);
			pause_exit(1);
		}
		for( n = 0 ; n < tmp_sec_per_io ; n++ )
		{
			if( lba + n != 
				 *(unsigned long *)(  n * 0x200 + ((unsigned char *)cmpbuf) )  
			  )
			{
				printf("data error: LBA: 0x%lx \n",lba + n);				
				pause_exit(1);
			}
			for( i = 2 ; i < COUNT_64BIT_PER_SECTOR ; i++ )
			{
				if( cmpbuf[1] != cmpbuf[i] )
				{
					printf("data error: LBA: 0x%lx \n",lba + n);
					pause_exit(1);
				}
			}
		}
	}
	return 0;
}

#ifdef WIN32
ULONGLONG vtp_X86RDTSC_u64()
{
	ULONG u32rax,u32rdx;
	ULONGLONG v;
	_asm rdtsc;
	_asm mov u32rax,eax;
	_asm mov u32rdx,edx;
	v = u32rdx;
	v <<= 32;
	return v + u32rax;
}
#else
unsigned long vtp_X86RDTSC_u64()
{
	unsigned long reg_rdx,reg_rax;
	asm volatile("rdtsc \n\t" : "=d" (reg_rdx),"=a" (reg_rax) );
	return reg_rax | (reg_rdx<<32);
}
#endif

U32 new_random()
{
	U32	 ret = 1;
	U32	 n = (U32)(vtp_X86RDTSC_u64()/100);
	n &= 0x3f;
	for( n+=3; n ; n-- )
	{
		ret = rand();
	}
	return ret*vtp_X86RDTSC_u64();
}
char *g_buf_rw = NULL;

#define G_BUF_RW_SIZE	(0x10000)

int disk_read_write(int fd,int isread,U32	u32LBA,U32 u32Cnt,char *buf)
{
	int		ret;
	unsigned int	bmemcpy = 0;
	unsigned int	mempost = 0;
	U64		fileoffset = u32LBA;
	fileoffset*=0x200;
	u32Cnt*=0x200;

	if( NULL == g_buf_rw )
	{
		ret = posix_memalign((void **)&g_buf_rw, 0x1000, 2*G_BUF_RW_SIZE );
		if (ret) 
		{
			perror("posix_memalign failed");
			pause_exit(1);
		}
	}

	api_lseek(fd, fileoffset, SEEK_SET);
	if( u32Cnt <= (G_BUF_RW_SIZE) )
	{
		mempost = u32LBA/8;
		if( 0 == (mempost%3) )
		{//memcpy
			bmemcpy = 1;
			mempost %= G_BUF_RW_SIZE;
			mempost &= (~3);	//dword对其.
		}
	}

	if( isread )
	{
		if( bmemcpy )
		{
			ret = api_read(fd, g_buf_rw+mempost, u32Cnt );
			if( ret < 0 )
			{
				printf("read LBA:0x%lx error",u32LBA);
				pause_exit(1);
			}
			memcpy(buf, g_buf_rw+mempost , u32Cnt  );
		}else
		{
			ret = api_read(fd, buf, u32Cnt );	
			if (ret < 0) 
			{
				printf("read LBA:0x%lx error",u32LBA);
				pause_exit(1);
			}
		}
	}else
	{
		if ( bmemcpy )
		{
			memcpy(g_buf_rw+mempost ,buf,u32Cnt  );
			buf = g_buf_rw+mempost ;
		}
		ret = api_write(fd, buf, u32Cnt );	
		if (ret < 0) 
		{
			printf("write LBA:0x%lx error",u32LBA);
			pause_exit(1);
		}
	}
	return 0;
}

#define IOCTL_DISK_FLUSH_DISK          CTL_CODE(IOCTL_DISK_BASE, 0x0818, METHOD_BUFFERED, FILE_ANY_ACCESS)

void flush_disk_by_ioctl(HANDLE	fd)
{
	DWORD		dwRet;

	DeviceIoControl(fd,IOCTL_DISK_FLUSH_DISK,
		NULL,0,
		NULL,0,
		&dwRet,NULL);

}

int flush_disk(int fd)
{
	if ( g_flush_flag )
	{
		FlushFileBuffers((HANDLE)fd);
		flush_disk_by_ioctl((HANDLE)fd);
	}
	return 0;
}

//U32		g_used_bitmap_start;
//U32		g_used_bitmap_end;

U32	 init_cluster_mgr(unsigned char * pbitmap,U32	totalclusterCnt)
{
	memset(pbitmap,0, (totalclusterCnt+7)/8 );
	pbitmap[0] |= 1;
	//g_used_bitmap_start = 0;
	//g_used_bitmap_end = 0;
	return 0;
}

U32	 alloc_one_cluster(unsigned char * pbitmap,U32	totalclusterCnt)
{
	U32		lcn;
	lcn = x_random();
	lcn %= totalclusterCnt;
//	if ( g_used_bitmap_start <= lcn && lcn < g_used_bitmap_end )
//	{
//		lcn = g_used_bitmap_end;
//	}
	while( 1 )
	{
		lcn %= totalclusterCnt;

		if( pbitmap[ lcn/8 ] & (1<<(lcn&7)) )
		{
			lcn++;
		}else
		{
			pbitmap[ lcn/8 ] |= (1<<(lcn&7));
			return lcn;
		}
	}
	return 0;
}


U32	 get_random_alloced_lcn(unsigned char * pbitmap,U32	totalclusterCnt)
{
	U32		lcn;
	lcn = x_random();
	while( 1 )
	{
		lcn %= totalclusterCnt;
		if( pbitmap[ lcn/8 ] & (1<<(lcn&7)) )
		{
			return lcn;
		}else
		{
			lcn++;
			continue;
		}
	}	
}


#define TEST_HFLAG_X_BASE	0x5634
U32	 TEST_FS_HFLAG_X = 0;

typedef struct ST_TESTFS_HEAD 
{
	U32		hflag;
	U32		rwflag1;
#define CNT_OF_LBALIST	((0x200-0x10)/4)
	U32		lbalist[CNT_OF_LBALIST];
	U32		rwflag2;
	U32		rwpostPerIo; //这个变量全部不样同
}TESTFS_HEAD;

#define SIZE_OF_SAME_DATA	(sizeof(TESTFS_HEAD)-4)

void print_buf_0x200(unsigned char *printbuf)
{
	int		n;
	printf("dump buf addr: %p:\n",printbuf);
	for ( n = 0; n < 0x200; n+=4 )
	{
		printf("%08x", *(unsigned int *)(printbuf + n) );
		if ( n%32 == 4 || n%32 == 12 || n%32 == 20 )
		{
			printf("-");
		}else
		{
			printf(" ");
		}
		if (  n%32 == 28 )
		{
			printf("\n");
		}
	}
	return ;
}

void Verify_disk_data(int fd,U32 dwCntofCluster,U32 SECTOR_PER_CLUSTER,unsigned char *bitmap,TESTFS_HEAD	*testfshead,TESTFS_HEAD	*verify_buf,U32		u32TestCount)
{
	U32		ii,n;
	U32		u32LBA;
	U32		cmp1,cmp2,cmp3,cmp4,cmp5;

	if ( 1 == u32TestCount )
	{
		u32TestCount = x_random()&0xff;
	}

	for( ; u32TestCount ; u32TestCount-- )
	{
		u32LBA = SECTOR_PER_CLUSTER*get_random_alloced_lcn(bitmap,dwCntofCluster);
		for ( ii = 1 ; ii < CNT_OF_LBALIST ; ii++ )
		{
			if( u32LBA == testfshead->lbalist[ii] )
			{//还在缓存中，没有写入硬盘。
				break;
			}
		}
		if( ii < CNT_OF_LBALIST )
		{//还在缓存中，没有写入硬盘。
			continue;
		}
		//从硬盘读出来检查。
		memset(verify_buf,0x66,0x200*SECTOR_PER_CLUSTER);
		disk_read_write(fd,1,u32LBA,SECTOR_PER_CLUSTER,(char *)verify_buf);

		cmp1 = verify_buf->lbalist[0];
		cmp2 = verify_buf->hflag;
		cmp3 = verify_buf->rwflag1;
		cmp4 = verify_buf->rwflag2;
		cmp5 = verify_buf->rwpostPerIo;

		if (cmp1 != u32LBA ||
			cmp2 != testfshead->hflag  || 
			cmp3 != testfshead->rwflag1 || 
			cmp4 != testfshead->rwflag2 || 
			cmp5 != testfshead->rwpostPerIo 
			)
		{
			printf("verify BUG 001:\n");
			print_current_time();
			printf("最后正确写入LBA: 0x%x,lbalist[0]:0x%x hflag:0x%x rwflag1:0x%x rwflag2:0x%x post:0x%x\n", testfshead->lbalist[0],testfshead->lbalist[0], testfshead->hflag,testfshead->rwflag1,testfshead->rwflag2,testfshead->rwpostPerIo );
			printf("读出发现错误LBA: 0x%x,lbalist[0]:0x%x hflag:0x%x rwflag1:0x%x rwflag2:0x%x post:0x%x\n", u32LBA , cmp1, cmp2 ,cmp3 ,cmp4 ,cmp5 );
			printf("Dump all the verify_buf: \n");
			print_buf_0x200(verify_buf);
			printf("Dump all the testfshead buf: \n");
			print_buf_0x200(testfshead);
			pause_exit(1);
			break;
		}
		for ( ii = 1; ii < SECTOR_PER_CLUSTER ; ii++ )
		{
			if ( memcmp(verify_buf,&verify_buf[ii],SIZE_OF_SAME_DATA ) || ii!= verify_buf[ii].rwpostPerIo )
			{
				printf("verify BUG 002: LBA 0x%x (error) != LBA: 0x%x (GOOD) \n",u32LBA+ii, u32LBA );
				print_current_time();
				printf("最后正确写入LBA:0x%x,lbalist[0]:0x%x hflag:0x%x rwflag1:0x%x rwflag2:0x%x post:0x%x\n", testfshead->lbalist[0],testfshead->lbalist[0], testfshead->hflag,testfshead->rwflag1,testfshead->rwflag2 ,testfshead->rwpostPerIo);
				printf("正确写入    LBA:0x%x,lbalist[0]:0x%x hflag:0x%x rwflag1:0x%x rwflag2:0x%x post:0x%x\n", verify_buf->lbalist[0],verify_buf->lbalist[0], verify_buf->hflag,verify_buf->rwflag1,verify_buf->rwflag2 ,verify_buf->rwpostPerIo);
				printf("读出发现错误LBA:0x%x,lbalist[0]:0x%x hflag:0x%x rwflag1:0x%x rwflag2:0x%x post:0x%x\n", u32LBA,verify_buf[ii].lbalist[0], verify_buf[ii].hflag,verify_buf[ii].rwflag1,verify_buf[ii].rwflag2 ,verify_buf[ii].rwpostPerIo);
				printf("Dump all the verify_buf: \n");
				print_buf_0x200(verify_buf);
				printf("Dump all the verify_buf[ii]: \n");
				print_buf_0x200(&verify_buf[ii]);
			}
		}
		if( ii < SECTOR_PER_CLUSTER )
		{
			pause_exit(1);			
			break;
		}
	}	
	return ;
}

void start_test_hd_serial(int	fd,U32	SECTOR_PER_CLUSTER,U32	u32TotalSectorCnt,unsigned int verify_count)
{
	U32				n,i,ret;
	TESTFS_HEAD		*verify_buf;
	TESTFS_HEAD		*testfshead;
	TESTFS_HEAD		*cachetfs;
	unsigned char	*bitmap; 
	U32				dwCntofCluster,dwUsedCnt,dwLastID1,dwLastID2;
	BOOLEAN			errorfound;


	if( sizeof(TESTFS_HEAD) != 0x200 )
	{
		printf("error: if( sizeof(TESTFS_HEAD) != 0x200 )");
		pause_exit(1);
	}

	dwCntofCluster = u32TotalSectorCnt/SECTOR_PER_CLUSTER;

	if ( 4*dwCntofCluster < CNT_OF_LBALIST )
	{
		printf("disk size <= 0x200*0x200 ");
		pause_exit(1);		
	}

	bitmap = malloc( (dwCntofCluster+7)/8 );

	ret = posix_memalign((void **)&testfshead, 0x1000, SECTOR_PER_CLUSTER * 0x200 );
	if (ret) 
	{
		perror("posix_memalign failed\n");
		pause_exit(1);
	}
	ret = posix_memalign((void **)&verify_buf, 0x1000, SECTOR_PER_CLUSTER * 0x200 );
	if (ret) 
	{
		perror("posix_memalign failed\n");
		pause_exit(1);
	}
	ret = posix_memalign((void **)&cachetfs, 0x1000, SECTOR_PER_CLUSTER * 0x200 );
	if (ret) 
	{
		perror("posix_memalign failed\n");
		pause_exit(1);
	}

	disk_read_write(fd,1,0,SECTOR_PER_CLUSTER,(char *)testfshead);
	if ( TEST_FS_HFLAG_X == testfshead->hflag )
	{//check it:
		//mbr 不做check..
		printf("start check disk..\n");
		while(1)
		{
			memcpy(cachetfs,testfshead,SECTOR_PER_CLUSTER*sizeof(TESTFS_HEAD));
reread_disk:
			disk_read_write(fd,1,cachetfs->lbalist[1],SECTOR_PER_CLUSTER,(char *)testfshead);
			if ( TEST_FS_HFLAG_X != testfshead->hflag || 
				cachetfs->rwflag1 != testfshead->rwflag1 ||
				cachetfs->rwflag2 != testfshead->rwflag2 ||
				cachetfs->rwpostPerIo != testfshead->rwpostPerIo 
				)
			{
				printf("warning: LBA 0x%x error: if ( TEST_FS_HFLAG_X != testfshead->hflag)\n",cachetfs->lbalist[1]);
				break;
			}
			
			if( memcmp( &cachetfs->lbalist[1] , &testfshead->lbalist[0] , sizeof(U32) * (CNT_OF_LBALIST - 1) ) )
			{
				printf("BUG::: 一个扇区写入一半: LBA_A:0x%x 数据错误,不等于正确的 LBA_B:0x%x \n",cachetfs->lbalist[1],cachetfs->lbalist[0]);
				printf("数据内容:");
				for ( n = 0 ; n < (CNT_OF_LBALIST - 1) ; n++ )
				{
					printf("%08lx(Err:%08lx) ",cachetfs->lbalist[n+1],testfshead->lbalist[n]);
				}
				printf("\npress any key to retry:");
				getch();
				goto reread_disk;
				pause_exit(1);
			}
			errorfound = FALSE;
			for ( i = 1; i < SECTOR_PER_CLUSTER ; i++ )
			{
				if ( memcmp(testfshead,&testfshead[i],SIZE_OF_SAME_DATA ) || i!= testfshead[i].rwpostPerIo )
				{
					printf("warning: 一次写入多扇区，前后不一致: 正确:LBA 0x%x error: LBA: 0x%x \n",testfshead->lbalist[0],testfshead->lbalist[0]+i);
					errorfound = TRUE;
				}
			}
			if( errorfound )
			{
				break;
			}
		}
		//查看最后一块后面还有没有数据:
		errorfound = FALSE;
		for ( i = 2 ; i < CNT_OF_LBALIST ; i++ )
		{
			memset(verify_buf,0x99,0x200*SECTOR_PER_CLUSTER);
			disk_read_write(fd,1,cachetfs->lbalist[i],SECTOR_PER_CLUSTER,(char *)verify_buf);
			if ( TEST_FS_HFLAG_X != verify_buf->hflag || 
				cachetfs->rwflag1 != verify_buf->rwflag1 ||
				cachetfs->rwflag2 != verify_buf->rwflag2	)
			{
				continue;
			}else
			{//找到先后顺序不一致的情况：
				printf("BUG:::先写入的数据丢失：丢失之前LBA: 0x%x 丢失的LBA: 0x%x, 丢失之后写入的: LBA 0x%x \n ",cachetfs->lbalist[0],cachetfs->lbalist[1],cachetfs->lbalist[i]);
				printf("丢失之前LBA: 0x%x 的数据:lba0:0x%x hflag:0x%x rwflag1: 0x%x rwflag2: 0x%x \n", cachetfs->lbalist[0], cachetfs->lbalist[0], cachetfs->hflag,cachetfs->rwflag1,cachetfs->rwflag2 );
				printf("丢失LBA: 0x%x 的数据    :lba0:0x%x hflag:0x%x rwflag1: 0x%x rwflag2: 0x%x \n", cachetfs->lbalist[1], testfshead->lbalist[0], testfshead->hflag,testfshead->rwflag1,testfshead->rwflag2 );
				printf("之后成功写入LBA: 0x%x   :lba0:0x%x hflag:0x%x rwflag1: 0x%x rwflag2: 0x%x \n",cachetfs->lbalist[i],  verify_buf->lbalist[0],  verify_buf->hflag,verify_buf->rwflag1,verify_buf->rwflag2 );
				errorfound = TRUE;
				continue;
			}
		}
		if( errorfound )
		{
			pause_exit(1);
		}
		//check end:
	}
	//new rewrite:

	dwLastID1 = testfshead->rwflag1;
	dwLastID2 = testfshead->rwflag2;

rewrite:

	printf("start write disk");

	memset(testfshead,0,sizeof(TESTFS_HEAD));

	while( 1 )
	{
		testfshead->hflag = TEST_FS_HFLAG_X;
		testfshead->rwflag1 = x_random()*vtp_X86RDTSC_u64();
		testfshead->rwflag2 = new_random();
		testfshead->rwpostPerIo = 0;
		if ( dwLastID1 != testfshead->rwflag1 && dwLastID2 != testfshead->rwflag2 )
		{
			break;
		}
	}
	printf(".");
	init_cluster_mgr(bitmap,dwCntofCluster);
	dwUsedCnt = 1; //lba 0

	testfshead->lbalist[0] = 0; //lba 0;
	for ( i = 1 ; i < CNT_OF_LBALIST ; i++ )
	{
		testfshead->lbalist[i] = SECTOR_PER_CLUSTER*alloc_one_cluster(bitmap,dwCntofCluster);
		dwUsedCnt++;
	}
	for ( i = 1; i < SECTOR_PER_CLUSTER ; i++ )
	{
		memcpy(&testfshead[i],testfshead,sizeof(TESTFS_HEAD) );
		testfshead[i].rwpostPerIo = i;
	}
	printf(".\n");
	disk_read_write(fd,0,testfshead->lbalist[0],SECTOR_PER_CLUSTER,(char *)testfshead);
	flush_disk(fd);
	while( dwUsedCnt < dwCntofCluster )
	{
		memmove( &testfshead->lbalist[0], &testfshead->lbalist[1], (CNT_OF_LBALIST-1)*4 );
		testfshead->lbalist[CNT_OF_LBALIST-1] = SECTOR_PER_CLUSTER*alloc_one_cluster(bitmap,dwCntofCluster);
		dwUsedCnt++;
		for ( i = 1; i < SECTOR_PER_CLUSTER ; i++ )
		{
			memcpy(&testfshead[i],testfshead,sizeof(TESTFS_HEAD) );
			testfshead[i].rwpostPerIo = i;
		}
		disk_read_write(fd,0,testfshead->lbalist[0],SECTOR_PER_CLUSTER,(char *)testfshead);
		flush_disk(fd);

		if ( verify_count )
		{//要做校验数据
			if ( dwUsedCnt > 2*verify_count  )
			{//写入的数据量大于2倍要校验的数据时，开始校验数据。
				Verify_disk_data(fd,dwCntofCluster,SECTOR_PER_CLUSTER,bitmap,testfshead,(TESTFS_HEAD *)verify_buf,verify_count);
			}
		}
	}
	//all complete.. retry;
	dwLastID1 = testfshead->rwflag1;
	dwLastID2 = testfshead->rwflag2;
	goto rewrite;

	return ;
}

int main(int argc, char * argv[])
{
	int				fd;
	unsigned int	sec_per_io;
	unsigned int	verify_count;
	unsigned long	total_sec;
	char			*devname;

	U64				total_sec_cnt;

	print_current_time();

	if( argc != 5 )
	{
		printf("%s dev sec_per_io flush_flag read_verify\n\
dev:设备名\n\
sec_per_io:一次同步IO请求的扇区数，16进制.\n\
flush_flag:每次同步IO后是否调用硬盘的刷硬盘缓存指令,1表示要刷，0表未不刷.\n\
read_verify:写了数据后，随机读取数据校验的块数(16进制),0表示不读.1表示0-255间随机,其它表示块数\n\
		Ex\n%s /dev/sda 0x8 1 0\n",argv[0],argv[0]);
		return -1;
	}
	devname = argv[1];
	sscanf(argv[2],"%x",&sec_per_io);
	sscanf(argv[3],"%x",&g_flush_flag);	
	sscanf(argv[4],"%x",&verify_count);

	TEST_FS_HFLAG_X = TEST_HFLAG_X_BASE + 0x10000ul*sec_per_io;
	fd = getfilehandle(devname);

	total_sec_cnt = api_getsize(fd);
	total_sec_cnt /= 512;
	total_sec = (unsigned long)total_sec_cnt;

	printf("正在测试硬盘%s,  硬盘的总扇区: 0x%lx , 每次IO扇区数:0x%x %s, 读校验块数:0x%x\n",
		devname , total_sec , sec_per_io , 
		(g_flush_flag)?"同步IO并且刷硬盘缓存!":"同步IO并且不刷硬盘缓存!",
		verify_count
		);

	start_test_hd_serial(fd,sec_per_io,total_sec,verify_count);
	
	return 0;
}
