/* ��BL0�׶Σ�Irom�ڹ̻��Ĵ����ȡnandflash��SD��ǰ16K�����ݣ�
 * ���ȶ�ǰ16�ֽ��е�У����Ƿ���ȷ����ȷ�������������ֹͣ��
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFSIZE                 (16*1024)
#define IMG_SIZE                (16*1024)
#define SPL_HEADER_SIZE         16
#define SPL_HEADER              "S5PC110 HEADER  "

int main (int argc, char *argv[])
{
	FILE		*fp;
	char		*Buf, *a;
	int		BufLen;
	int		nbytes, fileLen;
	unsigned int	checksum, count;
	int		i;
	
	// 1. 3������
	if (argc != 3)
	{
		printf("Usage: mkbl1 <source file> <destination file>\n");
		return -1;
	}

	// 2. ����16K��buffer
	BufLen = BUFSIZE;
	Buf = (char *)malloc(BufLen);
	if (!Buf)
	{
		printf("Alloc buffer failed!\n");
		return -1;
	}

	memset(Buf, 0x00, BufLen);

	// 3. ��Դbin��buffer
	// 3.1 ��Դbin
	fp = fopen(argv[1], "rb");
	if( fp == NULL)
	{
		printf("source file open error\n");
		free(Buf);
		return -1;
	}
	// 3.2 ��ȡԴbin����
	fseek(fp, 0L, SEEK_END);
	fileLen = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	// 3.3 Դbin���Ȳ��ó���16K-16byte
	count = (fileLen < (IMG_SIZE - SPL_HEADER_SIZE))
		? fileLen : (IMG_SIZE - SPL_HEADER_SIZE);
	// 3.4 buffer[0~15]���"S5PC110 HEADER  "
	memcpy(&Buf[0], SPL_HEADER, SPL_HEADER_SIZE);
	// 3.5 ��Դbin��buffer[16]
	nbytes = fread(Buf + SPL_HEADER_SIZE, 1, count, fp);
	if ( nbytes != count )
	{
		printf("source file read error\n");
		free(Buf);
		fclose(fp);
		return -1;
	}
	fclose(fp);

	// 4. ����У���
 	// 4.1 �ӵ�16byte��ʼͳ��buffer�й��м���1
	a = Buf + SPL_HEADER_SIZE;
	for(i = 0, checksum = 0; i < IMG_SIZE - SPL_HEADER_SIZE; i++)
		checksum += (0x000000FF) & *a++;
	// 4.2 ��У��ͱ�����buffer[8~15]
	a = Buf + 8;
	*( (unsigned int *)a ) = checksum;

	// 5. ����buffer�е����ݵ�Ŀ��bin
	// 5.1 ��Ŀ��bin
	fp = fopen(argv[2], "wb");
	if (fp == NULL)
	{
		printf("destination file open error\n");
		free(Buf);
		return -1;
	}
	// 5.2 ��16k��buffer������Ŀ��bin��
	a = Buf;
	nbytes	= fwrite( a, 1, BufLen, fp);
	if ( nbytes != BufLen )
	{
		printf("destination file write error\n");
		free(Buf);
		fclose(fp);
		return -1;
	}

	free(Buf);
	fclose(fp);

	return 0;
}
