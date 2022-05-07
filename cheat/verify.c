/*******************************
 函数：
	1.mac:162387C5B7F7（以太、本地、局域）16:23:87:C5:B7:F7#34:23:87:C5:B7:F7#E0:DB:55:B5:9C:16
	2.harddisk硬盘序列号：3W0PKVLE
	3.harddisk和 mac合并为machine_code：3W0PKVLE16:23:87:C5:B7:F7
	4.machine_code去冒号：machine_code：3W0PKVLE162387C5B7F7
	5.获取machine_code奇数位：oddnumber：30KL328CBF
	6.奇数位加密: Key:996837194
	7.注册程序
	8.登录程序
  主函数：main()
********************************/
#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <winsock2.h>
#include <Iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>
#pragma comment(lib, "Iphlpapi.lib")
#include <locale.h>
#include <tchar.h>
#include <windows.h>


// 1.1 MAC函数（以太网、本地、局域）
void byte2Hex(unsigned char bData, unsigned char hex[])
{
	int high = bData / 16, low = bData % 16;
	hex[0] = (high < 10) ? ('0' + high) : ('A' + high - 10);
	hex[1] = (low < 10) ? ('0' + low) : ('A' + low - 10);
}
// 1.2 获取本机MAC地址 
int getLocalMac(unsigned char* mac)
{
	ULONG ulSize = 0;
	PIP_ADAPTER_INFO pInfo = NULL;
	int temp = 0;
	temp = GetAdaptersInfo(pInfo, &ulSize);//第一次调用，获取缓冲区大小
	pInfo = (PIP_ADAPTER_INFO)malloc(ulSize);
	temp = GetAdaptersInfo(pInfo, &ulSize);

	int iCount = 0;
	while (pInfo)//遍历每一张网卡
	{
		//  pInfo->Address 是MAC地址
		for (int i = 0; i < (int)pInfo->AddressLength; i++)
		{
			byte2Hex(pInfo->Address[i], &mac[iCount]);
			iCount += 2;
			if (i < (int)pInfo->AddressLength - 1)
			{
				mac[iCount++] = ':';
			}
			else
			{
				mac[iCount++] = '#';
			}
		}
		pInfo = pInfo->Next;
	}
	if (iCount > 0)
	{
		mac[--iCount] = '\0';
		return iCount;
	}
	else return -1;
}

// 2.获取harddisk
// 2.1 harddisk函数
char* flipAndCodeBytes(const char* str, int pos, int flip, char* buf)
{
	int i;
	int j = 0;
	int k = 0;

	buf[0] = '\0';
	if (pos <= 0)
		return buf;

	if (!j)
	{
		char p = 0;
		// First try to gather all characters representing hex digits only.
		j = 1;
		k = 0;
		buf[k] = 0;
		for (i = pos; j && str[i] != '\0'; ++i)
		{
			char c = tolower(str[i]);

			if (isspace(c))
				c = '0';
			++p;
			buf[k] <<= 4;

			if (c >= '0' && c <= '9')
				buf[k] |= (unsigned char)(c - '0');
			else if (c >= 'a' && c <= 'f')
				buf[k] |= (unsigned char)(c - 'a' + 10);
			else
			{
				j = 0;
				break;
			}
			if (p == 2)
			{
				if (buf[k] != '\0' && !isprint(buf[k]))
				{
					j = 0;
					break;
				}
				++k;
				p = 0;
				buf[k] = 0;
			}
		}
	}

	if (!j)
	{
		// There are non-digit characters, gather them as is.
		j = 1;
		k = 0;
		for (i = pos; j && str[i] != '\0'; ++i)
		{
			char c = str[i];

			if (!isprint(c))
			{
				j = 0;
				break;
			}
			buf[k++] = c;
		}
	}

	if (!j)
	{
		// The characters are not there or are not printable.
		k = 0;
	}

	buf[k] = '\0';

	if (flip)
		// Flip adjacent characters
		for (j = 0; j < k; j += 2)
		{
			char t = buf[j];
			buf[j] = buf[j + 1];
			buf[j + 1] = t;
		}

	// Trim any beginning and end space
	i = j = -1;
	for (k = 0; buf[k] != '\0'; ++k)
	{
		if (!isspace(buf[k]))
		{
			if (i < 0)
				i = k;
			j = k;
		}
	}

	if ((i >= 0) && (j >= 0))
	{
		for (k = i; (k <= j) && (buf[k] != '\0'); ++k)
			buf[k - i] = buf[k];
		buf[k - i] = '\0';
	}
	return buf;
}
// 2.2 获取harddisk
ULONG GetHDSerial(PCHAR pszIDBuff, int nBuffLen, int nDriveID)
{
	HANDLE hPhysicalDrive = INVALID_HANDLE_VALUE;
	ULONG ulSerialLen = 0;
	__try
	{
		//  Try to get a handle to PhysicalDrive IOCTL, report failure
		//  and exit if can't.
		TCHAR szDriveName[32];
		wsprintf(szDriveName, TEXT("\\\\.\\PhysicalDrive%d"), nDriveID);

		//  Windows NT, Windows 2000, Windows XP - admin rights not required
		hPhysicalDrive = CreateFile(szDriveName, 0,
			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
			OPEN_EXISTING, 0, NULL);
		if (hPhysicalDrive == INVALID_HANDLE_VALUE)
		{
			__leave;
		}
		STORAGE_PROPERTY_QUERY query;
		DWORD cbBytesReturned = 0;
		static char local_buffer[10000];

		memset((void*)&query, 0, sizeof(query));
		query.PropertyId = StorageDeviceProperty;
		query.QueryType = PropertyStandardQuery;

		memset(local_buffer, 0, sizeof(local_buffer));

		if (DeviceIoControl(hPhysicalDrive, IOCTL_STORAGE_QUERY_PROPERTY,
			&query,
			sizeof(query),
			&local_buffer[0],
			sizeof(local_buffer),
			&cbBytesReturned, NULL))
		{
			STORAGE_DEVICE_DESCRIPTOR* descrip = (STORAGE_DEVICE_DESCRIPTOR*)&local_buffer;
			char serialNumber[1000];

			flipAndCodeBytes(local_buffer,
				descrip->SerialNumberOffset,
				1, serialNumber);

			if (isalnum(serialNumber[0]))
			{
				ULONG ulSerialLenTemp = strnlen(serialNumber, nBuffLen - 1);
				memcpy(pszIDBuff, serialNumber, ulSerialLenTemp);
				pszIDBuff[ulSerialLenTemp] = NULL;
				ulSerialLen = ulSerialLenTemp;
				__leave;
			}

		}
	}
	__finally
	{
		if (hPhysicalDrive != INVALID_HANDLE_VALUE)
		{
			CloseHandle(hPhysicalDrive);
		}
		return ulSerialLen;
	}
}

char  machine_code[100];  // 机器码 串接的硬盘序列号和mac

// 3. 获取mac和harddisk 并串接
void GetAllHDSerial(void)
{

	// harddisk序列号输出
	const int MAX_IDE_DRIVES = 16;
	static char szBuff[0x100];
	for (int nDriveNum = 0; nDriveNum < MAX_IDE_DRIVES; nDriveNum++)
	{
		ULONG ulLen = GetHDSerial(szBuff, sizeof(szBuff), nDriveNum);

		/****************
		if (ulLen > 0)
		{
			_tprintf(TEXT("第%d块硬盘的序列号为：%hs\n"), nDriveNum + 1, szBuff);
		}
		*****************/


	}

	// mac输出
	unsigned char address[1024];
	/*****************
	if (getLocalMac(address)>0)
	{
		printf("MAC：%s\n", address);
	}
	******************/

	if (getLocalMac(address) <= 0)
	{
		printf("invoke getMAC error!\n");
	}


	// 串接字符串
	static char szBuff1[64];
	strncat(szBuff1, szBuff, 8);    // 取szBuff前8字串接到szBuff1末尾
	//printf("%s\n", szBuff1);

	strncat(szBuff1, (const char*)address, 17);  // 取mac前17位串接到szBuff1末尾
	//printf(szBuff);
	strcpy(machine_code, szBuff1);  // szBuff1赋值到machine_code机器码
	//printf(machine_code);

}

// 4. 去掉冒号
void del_chr(char* s, char ch)
{
	char* t = s; //目标指针先指向原串头
	while (*s != '\0') //遍历字符串s
	{
		if (*s != ch) //如果当前字符不是要删除的，则保存到目标串中
			*t++ = *s;
		s++; //检查下一个字符
	}
	*t = '\0'; //置目标串结束符。
}

// 5. 获取字符串奇数位
char oddnumber[30];
void odd(char a[30])
{
	int i, j = 0;

	for (i = 0; i < strlen(a); i += 2)
		oddnumber[j++] = a[i];
	oddnumber[j] = '\0';
	//printf("oddnumber:%s\n", oddnumber);
}
// 6.1 奇数位加密
long StrToInt(char* s)  // 将一个字符串转化为整数
{
	long number = 0;
	while (*s != '\0')
	{
		number = number * 10 + *s - '0';
		s++;
	}
	return number;
}
// 6.2 奇数位加密
char Key[30];
void encrypt(char a[20])
{
	int num_Key = 0;
	num_Key = StrToInt(a);
	// 取正
	if (num_Key < 0)
	{
		num_Key = -num_Key;
	}

	_itoa(num_Key, Key, 10);  //把整数num_Key转成字符串str_Key
	printf("Key: %s\n", Key);

}

// 7.注册程序
void regist(void)
{
	// 提示输入注册码
	char input_Key[30];
	char incout[] = "请输入注册码input_Key：";
	printf("%s", incout);
	scanf("%s", input_Key);
	//printf("please input code:%s\n", code);

	// 获取机器码加密成注册码
	//char encode[] = "asd";
	//printf("got encode:%s\n", encode);

	// 比较两个字符串是否一致
	if (strcmp(Key, input_Key) == 0)  // 若一致，新建打开data.txt ，写入注册码，运行程序
	{
		FILE* fp = fopen("data.txt", "w");
		fprintf(fp, "%s ", input_Key);
		fclose(fp); //关闭文件

		char succe[] = "运行应用成功2！";
		printf("%s\n", succe);

	}
	else  //若不一致，提示不一致，并重新调用注册程序
	{
		char fail[] = "注册码不正确，请重新输入";
		printf("%s\n", fail);
		regist();
	}
}

// 8.登录程序
int checkAuthored(void)
{
	// 2.1 获取机器码加密成注册码
	//char encode[] = "asd";
	//printf("got encode:%s\n", encode);

	FILE* fp = fopen("data.txt", "r");
	char  filekey[30];
	// 2.2 若无注册文件，调用注册程序
	if ((fp = fopen("data.txt", "r")) == NULL)
	{
		printf("%s\n", "您未注册");
		char fail[] = "请发送下列验证码到邮箱 863110979@qq.com 进行注册！";
		//printf("%s\n ", fail);
		printf("验证码为：%s\n", oddnumber);
		regist();
		exit(1);
	}
	// 2.3 若文件为空，调用注册程序
	if (fgets(filekey, 30, fp) == NULL)
	{
		printf("%s", "file empty");
		char fail[] = "注册文件缺失！请重新输入注册码！";
		printf("%s\n", fail);
		printf("%s\n", oddnumber);
		regist();
		exit(1);
	}
	// 2.4 若文件非空，取出注册码与新获得的注册码比较：一致：success；不一致：调用注册程序
	else
	{
		filekey[strlen(filekey) - 1] = '\0';   // 去除取出注册码的末尾字符
	//	int n, m;
	//	n = strlen(filekey);
	//	m = strlen(Key);
	//	printf("%d,%d\n", n, m);
	//	printf("%s\n", filekey); //输出注册文件内容
		if (strcmp(Key, filekey) == 0)
		{
			char succe[] = "运行应用成功1！";
			printf("%s\n", succe);
			return 1;
		}
		else
		{
			printf("%s", "file exist");
			char fail[] = "注册文件内容与注册码不一致！请重新输入注册码!";
			printf("%s\n", fail);
			printf("%s\n ", oddnumber);
			regist();
		}
	}


	fclose(fp); //关闭文件
}


// 主函数
int verify()
{
	setlocale(LC_ALL, "chs");
	GetAllHDSerial();  // 获取mac与harddisk并拼接成机器码
	del_chr(machine_code, ':');  // 删除字符串内的冒号
	//printf("machine code:%s\n", machine_code); // 整理好的机器码
	odd(machine_code); // oddnumber  机器码奇数位
	//scanf("%s", oddnumber);
	//encrypt(oddnumber);// 加密后的注册码 key

	if (checkAuthored() == 1)// 登录程序
		return 1;
	else return -1;
}