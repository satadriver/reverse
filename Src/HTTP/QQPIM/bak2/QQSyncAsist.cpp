#include "stdafx.h"
#include "QQSyncAsist.h"
#include "QQPhoneBook.h"







//解密函数返回解密长度（等于后面4字节），头四字节是第一部分长度
//getRecycle				rollbackVersion					//1d 00 00 73 0a 0a 00 01 16
//WUPSYNC.GetRecycleReq		WUPSYNC.RollBackVersionReq		//1d 00 00/01 73/99 0a 0a 00 01 16
int GetQQPimClientData(const char * DecryptBuf, int DecryptBufLen, LPQQPIMDATA QQPimData)
{
	int ContentSize = 0;
	char * pData = 0;		//四字节长度 + 10 02 2c 3c 40 01/15 56
	int pDataLen = 0;

	while (ContentSize < DecryptBufLen)
	{
		pData = (char*)DecryptBuf + 11 ;		//四字节头部长度 10 02 2c 3c 40 01 56 此处长度固定
		pDataLen = * pData;
		pData = pData + 1;
		if ( (memcmp(pData ,"wupsync",strlen("wupsync")) == 0) || ( memcmp(pData ,"syncm",strlen("syncm")) == 0 ) ||
			 (memcmp(pData ,"report",strlen("report")) == 0 ) )
		{
			pData = pData + pDataLen;
			pData = pData + 1;						//一个字节的66，所有HTTP包都有

			pDataLen = *pData;
			pData = pData + 1;
			if( memcmp(pData,"GetUserIdentity",strlen("GetUserIdentity")) == 0 ||  (memcmp(pData ,"init",strlen("init")) == 0 ) )  		//获取客户端密钥	
			{		
				pData = pData + pDataLen;
				pData = pData + 1;					//}
				pData = pData + 8;					//00 01 00 99/c5/cf/d0 08 00 01 06

				pDataLen = * pData;					//req resp
				pData = pData + 1;
				pData = pData + pDataLen;

				pData = pData + 4;					//18 00 01 06
				pDataLen = * pData;
				pData = pData + 1;
				if ( memcmp(pData ,"WUPSYNC.GetUserIdentityReq",strlen("WUPSYNC.GetUserIdentityReq")) == 0 ||  
					(memcmp(pData ,"WUPSYNC.InitReq",strlen("WUPSYNC.InitReq")) == 0 ) ) 
				{
					pData = pData + pDataLen;
					pData = pData + 10;				//1d 00 01 00 a3/a5 0a 0a 00 01 16

					pDataLen = * pData;
					pData = pData + 1;
					memmove(QQPimData->QQ, pData, pDataLen);
					QQPimData->QQLen = pDataLen;
					pData = pData + pDataLen + 1;		//&

					//中间是一段字符串和一段MD5加上 0b 1a 06 14
					pData = strstr(pData,"COMN:");
					if (pData == 0)
					{
						ContentSize = ContentSize + ntohl(*(unsigned int*)DecryptBuf);
						DecryptBuf = DecryptBuf + ntohl(*(unsigned int*)DecryptBuf);
						continue;
					}
					pDataLen = *(pData -1);
					pData = pData + strlen("COMN:");
					pDataLen = pDataLen - strlen("COMN:");
					memmove(QQPimData->Imei, pData, pDataLen);
					QQPimData->ImeiLen = pDataLen;
					pData = pData + pDataLen;

					pData = pData + 1;			//16
					pDataLen = *pData;
					pData = pData + 1;
					memmove(QQPimData->Imsi,pData,pDataLen);
					QQPimData->ImsiLen = pDataLen;
					pData = pData + pDataLen;

					pData ++;		//& 
					pDataLen = * pData;
					pData = pData + pDataLen + 1;
					pData ++;		//36
					pDataLen = * pData;
					pData = pData + pDataLen + 1;
									
//长度 手机品牌 36
//长度 手机型号
//46 02 31 35 6c 7c 8c 9c
//0b 22 54 bf 84 5c 31 7d		//可变化的
//00 41 01 f4 5c 6c 7a 01 02 80 11 02 80 2c 0b 89 00 01 0a 00 01 共计21字节相同数据
//10 25 2c 3c 4c 0b 9c a6		//可变化的
//长度 服务器密钥
//
					int pTmpDataLen = ntohl(*(unsigned int*)DecryptBuf) - (pData - DecryptBuf) - 3;
					char * pTmpData = FindMemory("\x0b\x9c\xa6",3,pData,pTmpDataLen);
					if (pTmpData)
					{
						//pData = pTmpData + 8;
						pDataLen = * pTmpData;
						pData = pTmpData + 1;
					}
					else
					{
						pData = strchr(pData,'&');
						if (pData == 0)
						{
							ContentSize = ContentSize + ntohl(*(unsigned int*)DecryptBuf);
							DecryptBuf = DecryptBuf + ntohl(*(unsigned int*)DecryptBuf);
							continue;
						}
						pData = pData + 1;
						pData = strchr(pData,'&');
						if (pData == 0)
						{
							ContentSize = ContentSize + ntohl(*(unsigned int*)DecryptBuf);
							DecryptBuf = DecryptBuf + ntohl(*(unsigned int*)DecryptBuf);
							continue;
						}
						pData = pData + 1;
						pDataLen = * pData;
						pData = pData + 1;
					}

					if (pDataLen >= QQPIMDATA_ITEM_LEN)
					{
						return FALSE;
					}
					memmove(QQPimData->QQPimKey,pData,pDataLen);
					QQPimData->QQPimKeyLen = pDataLen;
					return TRUE;
				}
			}
			else if (memcmp(pData,"getRecycle",pDataLen) == 0)
			{
				pData += pDataLen;
				pData ++;

				pData += 8;			//00 01 00 99 08 00 01 06 
				pDataLen = *pData;
				pData ++;
				pData += pDataLen;

				pData += 4;			//18 00 01 06
				pDataLen = *pData;
				pData ++;
				if (memcmp(pData,"WUPSYNC.GetRecycleReq",pDataLen) == 0)
				{
					pData += pDataLen;
					pData += 9;			//1d 00 00 73 0a 0a 00 01 16;
					pDataLen = * pData;
					pData ++;
					memmove(QQPimData->QQ, pData, pDataLen);
					pData += pDataLen;

					pData = strstr(pData,"COMN:");	//08 04 0b 16
					if (pData == 0)
					{
						ContentSize = ContentSize + ntohl(*(unsigned int*)DecryptBuf);
						DecryptBuf = DecryptBuf + ntohl(*(unsigned int*)DecryptBuf);
						continue;
					}
					pDataLen = *(pData -1);
					pData += 5;
					pDataLen -= 5;
					memmove(QQPimData->Imei,pData,pDataLen);
				}
			}
			else if (memcmp(pData,"rollbackVersion",pDataLen) == 0)
			{
				pData += pDataLen;
				pData ++;
				
				pData += 8;			//00 01 00 c5 08 00 01 06 
				pDataLen = *pData;
				pData ++;
				pData += pDataLen;
				
				pData += 4;			//18 00 01 06
				pDataLen = *pData;
				pData ++;
				if (memcmp(pData,"WUPSYNC.RollBackVersionReq",pDataLen) == 0)
				{
					pData += pDataLen;
					pData += 10;			//1d 00 01 00 99 0a 0a 00 01 16;
					pDataLen = * pData;
					pData ++;
					memmove(QQPimData->QQ, pData, pDataLen);
					pData += pDataLen;
					
					pData = strstr(pData,"COMN:");	//08 04 0b 16
					if (pData == 0)
					{
						ContentSize = ContentSize + ntohl(*(unsigned int*)DecryptBuf);
						DecryptBuf = DecryptBuf + ntohl(*(unsigned int*)DecryptBuf);
						continue;
					}
					pDataLen = *(pData -1);
					pData += 5;
					pDataLen -= 5;
					memmove(QQPimData->Imei,pData,pDataLen);
				}
			}
			else if (memcmp(pData,"GetCloudData",pDataLen) == 0)
			{
				pData += pDataLen;
				pData ++;
				
				pData += 8;			//00 01 01 4d 08 00 01 06 
				pDataLen = *pData;
				pData ++;
				pData += pDataLen;
				
				pData += 4;			//18 00 01 06
				pDataLen = *pData;
				pData ++;
				if (memcmp(pData,"WUPSYNC.GetCloudDataReq",pDataLen) == 0)
				{
					pData += pDataLen;
					pData += 10;			//1d 00 01 01 24 0a 0a 00 01 16;
					pDataLen = * pData;
					pData ++;
					memmove(QQPimData->QQ, pData, pDataLen);
					pData += pDataLen;
					
					pData = strstr(pData,"COMN:");	//08 04 0b 16
					if (pData == 0)
					{
						ContentSize = ContentSize + ntohl(*(unsigned int*)DecryptBuf);
						DecryptBuf = DecryptBuf + ntohl(*(unsigned int*)DecryptBuf);
						continue;
					}
					pDataLen = *(pData -1);
					pData += 5;
					pDataLen -= 5;
					memmove(QQPimData->Imei,pData,pDataLen);
				}
			}
		}

		ContentSize = ContentSize + ntohl(*(unsigned int*)DecryptBuf);
		DecryptBuf = DecryptBuf + ntohl(*(unsigned int*)DecryptBuf);
	}

	return FALSE;		//返回值无用
}





//特定密钥 866309010254710h^J9o`(IMEI +	h^J9o`)		  %#!*^().,ljhd
//QQ通讯录密钥@#%^SEF#$Sddhfvh    		
//通用密钥DFG#$%^#%$RGHR(&*M<><	
//客户端密钥和服务器的密钥是一对包
//如果没有找到添加对象指针到CMAP同时获取客户端密钥
int ProcQQPimHttpUpPacket(HttpSession * pHttpSession,DATEHEADER & packh,const char * pPacket, int Flag)
{
	try
	{
		if( (pHttpSession->m_Requestline.m_Host.Find("3g.qq.com") != -1) && (pHttpSession->m_Requestline.m_useragent.Find("qqppim") != -1) )
		{
			if (cmapQQSyncAsistCnt == 0)
			{
				cmapQQSyncAsist.InitHashTable(0x10000);
			}
			if (cmapQQPimDataCnt == 0)
			{
				cmapQQPimData.InitHashTable(0x10000);
			}

			char * TmpData = 0;
			int TmpDataLen = 0;
			int Result = 0;

			if (Flag == FALSE)		//多包入口
			{
				unsigned char	SrcIP[4];
				unsigned char	DstIP[4];
				unsigned short	SrcPort;
				unsigned short	DstPort;
				int Result = GetIPPort((unsigned char *)pHttpSession->m_sip.GetBuffer(0), SrcIP, &SrcPort);
				if (Result == 0)
				{
					return TRUE;
				}
				Result = GetIPPort((unsigned char *)pHttpSession->m_dip.GetBuffer(0), DstIP, &DstPort);
				if (Result == 0)
				{
					return TRUE;
				}
				memmove(packh.m_sip, SrcIP,4);
				memmove(packh.m_dip, DstIP,4);
				packh.m_sport = SrcPort;
				packh.m_dport = DstPort;
				packh.m_datalen = pHttpSession->m_datalen;
				TmpData = (char*)pPacket;
				TmpDataLen = packh.m_datalen;
			}
			else
			{
				TmpData = strstr((char*)pPacket,"\r\n\r\n");
				if (TmpData == 0)
				{
					return TRUE;
				}
				TmpData += 4;
				TmpDataLen = packh.m_datalen - (TmpData - pPacket);
			}

			
			char Key[QQPIMDATA_ITEM_LEN] = {0};
			int Flag = 0;
			if ( (pHttpSession->m_Requestline.m_URI.Find("/xgetresult") != -1)  || 
				(pHttpSession->m_Requestline.m_URI.Find("/xdsync") != -1)		||
				(pHttpSession->m_Requestline.m_URI.Find("/xpimlogin") != -1) )
			{
				memmove(Key, "@#%^SEF#$Sddhfvh", strlen("@#%^SEF#$Sddhfvh"));
				Flag = QQPHONEBOOK;
			}
			else if (pHttpSession->m_Requestline.m_URI.Find("/?sig=") != -1)	
			{
				Flag = QQPHONEBOOK_DATA;
				int Result = ProcQQPhoneBookUpPack(packh, TmpData, TmpDataLen);
				return TRUE;
			}
			else
			{
				Flag = QQPIM;
				memmove(Key, "DFG#$%^#%$RGHR(&*M<><",strlen("DFG#$%^#%$RGHR(&*M<><"));
			}


			char DecryptBuf[QQPIM_DECRYPTBUF_LEN];
			int DecryptBufLen = QQPIM_DECRYPTBUF_LEN;
			DecryptBufLen = sub_183C((char*)TmpData,TmpDataLen,Key,strlen(Key),DecryptBuf,DecryptBufLen,0);//上行数据不需要解压缩直接解密
			if (DecryptBufLen > 0)
			{
				QQPIMDATA QQPimData;
				InitQQPimData(&QQPimData);
				Result = GetQQPimClientData(DecryptBuf, DecryptBufLen, &QQPimData);		//返回值无用碰到适合条件的包全部放行
			//	if (Result )
			//	{
					char QQPimSockKey[QQPIMDATA_ITEM_LEN];
					sprintf(QQPimSockKey,"%08x%08x%04x%04x",*(unsigned int*)packh.m_sip,*(unsigned int*)packh.m_dip,packh.m_sport,packh.m_dport);
					QQSyncAsist * QQsyncAsist = 0;
					Result = cmapQQSyncAsist.Lookup(QQPimSockKey,QQsyncAsist);
					if (Result == 0)			//找到了怎么办
					{
						QQsyncAsist = new QQSyncAsist(Flag);
						memmove((char*)&QQsyncAsist->QQPimData, (char*)&QQPimData, sizeof(QQPIMDATA));
						cmapQQSyncAsist.SetAt(QQPimSockKey,QQsyncAsist);
						cmapQQSyncAsistCnt ++;
					}
			//	}
				
				return TRUE;
			}
		}
		
		return FALSE;
	}
	catch (...)
	{
		WriteLog(HTTPPRO, "ProcessQQPIM post  Error!!");	
		return FALSE;
	}
}








//包头部分的构成0a 00 数据包序号 方向 命令 20 01 30 01 4a 06 QQ IMEI
//数据部分主要有3部分，中间的未知字符串 IMEI的MD5 数据段 
//数据段前面总是有\xa6\x00\x0b\x1d\x00 
//有时候数据段前面还有一段未知的固定的BASE64编码
//每一段前面总有80/81 00等字符
int ParseQQpimSyncPacket(char * DecryptBuf,int DecryptBufLen, char ** pDataBuf, unsigned int * pDataBufLen, char * QQ, char * Imei)
{
	char * pData = DecryptBuf;
	unsigned int pDataLen = 0;
	int PacketNum = 0;
	int Direction = 0;

	if (*pData != 0x0a)		
	{
		return FALSE;
	}

	PacketNum = *(pData + 2);		//从1开始递增
	Direction = *(pData + 3);		//0x10客户端方向 0x11服务器方向 //和后面的固定字节的意思相同 例如80和81

	pData = FindMemory("\x20\x01\x30\x01\x4a\x06",6,(char*)pData,64);
	if (pData == 0)
	{
		return FALSE;
	}

	unsigned char Flag = *(unsigned char*)(pData - 6 - 1);
	if (Flag == 0xf5)
	{
		return FALSE;
	}

	pDataLen = * pData;
	pData = pData + 1;
	memmove(QQ, pData,pDataLen);
	pData = pData + pDataLen;
	
	pData = pData + 4;		//10 01 0b 56
	pDataLen = * pData;
	pData ++;
	if (memcmp(pData,"COMN:",5))
	{
		return FALSE;
	}
	pData = pData + 5;
	pDataLen = pDataLen - 5;
	memmove(Imei, pData, pDataLen);
	pData = pData + pDataLen;

	if (*(DecryptBuf + 1) == 0x0c) 
	{
		int LeastLenth = DecryptBufLen - (pData - DecryptBuf) - 5;
		pData = FindMemory("\xa6\x00\x0b\x1d\x00",5,pData, LeastLenth);
		if (pData == 0)
		{
			return FALSE;
		}
		
		pDataLen = ntohs(*(unsigned short*)pData);
		pData += 2;

// 		char Key[256] = {0};
// 		memmove(Key, Imei, strlen(Imei));
// 		memmove(Key + strlen(Imei), "h^J9o`", 6);
		char * DecodeBuf = new char [pDataLen];
		int DecodeBufLen = pDataLen;
		DecodeBufLen = sub_183C(pData, pDataLen, "DFG#$%^#%$RGHR(&*M<><", 21, DecodeBuf, DecodeBufLen,0);
		if (DecodeBufLen > 0)
		{
			DecodeBufLen = Utf8ToGB2312(DecodeBuf,DecodeBufLen);
			
		}
		delete []DecodeBuf;
		return FALSE;			//数据包内容无用丢弃
	}
	else if ( Flag == 0x04)
	{
		int LeastLenth = DecryptBufLen - (pData - DecryptBuf) - 6;
		pData = FindMemory("\xa6\x00\x0b\x1d\x00\x01",6,pData, LeastLenth);
		if (pData == 0)
		{
			pData = FindMemory("\xa6\x00\x0b\x1d\x00",5,DecryptBuf, DecryptBufLen - 5);
			if (pData == 0)
			{
				return FALSE;
			}	
		}

		pDataLen = ntohs(*(unsigned short*)pData);
		pData += 2;
	}
	else if(  Flag == 0x05 )
	{
		int LeastLenth = DecryptBufLen - (pData - DecryptBuf) - 6;
		pData = FindMemory("\xa6\x00\x0b\x1d\x00\x01",6,pData, LeastLenth);
		if (pData == 0)
		{
			pData = FindMemory("\xa6\x00\x0b\x1d\x00",5,DecryptBuf, DecryptBufLen - 5);
			if (pData == 0)
			{
				return FALSE;
			}	
		}
		
		pDataLen = ntohs(*(unsigned short*)pData);
		pData += 2;	
	}
	else if(  Flag == 0x06 )
	{
		int LeastLenth = DecryptBufLen - (pData - DecryptBuf) - 6;
		pData = FindMemory("\xa6\x00\x0b\x1d\x00\x01",6,pData, LeastLenth);
		if (pData == 0)
		{
			pData = FindMemory("\xa6\x00\x0b\x1d\x00",5,DecryptBuf, DecryptBufLen - 5);
			if (pData == 0)
			{
				return FALSE;
			}	
		}
		
		pDataLen = ntohs(*(unsigned short*)pData);
		pData += 2;	
	}
	else if (Flag == 0x0c )
	{
		return FALSE;
	}
	else if(  Flag == 0x0d )
	{
		int LeastLenth = DecryptBufLen - (pData - DecryptBuf) - 5;
		pData = FindMemory("\xa6\x00\x0b\x1d\x00",5,pData, LeastLenth);
		if (pData == 0)
		{
			return FALSE;
		}
		
		pDataLen = ntohs(*(unsigned short*)pData);
		pData += 2;	
	}
	else if(  Flag == 0x10 )
	{
		int LeastLenth = DecryptBufLen - (pData - DecryptBuf) - 5;
		pData = FindMemory("\xa6\x00\x0b\x1d\x00",5,pData, LeastLenth);
		if (pData == 0)
		{
			return FALSE;
		}
		
		pDataLen = ntohs(*(unsigned short*)pData);
		pData += 2;	
	}
	else if(  Flag == 0x12 )
	{
		int LeastLenth = DecryptBufLen - (pData - DecryptBuf) - 5;
		pData = FindMemory("\xa6\x00\x0b\x1d\x00",5,pData, LeastLenth);
		if (pData == 0)
		{
			return FALSE;
		}

		pDataLen = ntohs(*(unsigned short*)pData);
		pData += 2;	
	}
	else if ( Flag == 0xec)
	{
		int LeastLenth = DecryptBufLen - ((char*)pData - DecryptBuf) - 5;
		pData = FindMemory("\xa6\x00\x0b\x1d\x00",5,pData, LeastLenth);
		if (pData == 0)
		{
			return FALSE;
		}
		
		pDataLen = ntohs(*(unsigned short*)pData);
		pData += 2;
	}
	else if ( Flag == 0xed)
	{
		int LeastLenth = DecryptBufLen - (pData - DecryptBuf) - 5;
		pData = FindMemory("\xa6\x00\x0b\x1d\x00",5,pData, LeastLenth);
		if (pData == 0)
		{
			return FALSE;
		}

		pDataLen = ntohs(*(unsigned short*)pData);
		pData += 2;	
	}
	else if ( Flag == 0xee)
	{
		int LeastLenth = DecryptBufLen - (pData - DecryptBuf) - 6;
		pData = FindMemory("\xa6\x00\x0b\x1d\x00\x01",6,pData, LeastLenth);
		if (pData == 0)
		{
			pData = FindMemory("\xa6\x00\x0b\x1d\x00",5,DecryptBuf, DecryptBufLen - 5);
			if (pData == 0)
			{
				return FALSE;
			}	
		}
		
		pDataLen = ntohs(*(unsigned short*)pData);
		pData = pData + 2;	
	}
	else if ( Flag == 0xf4 || Flag== 0xf5 || Flag== 0xf6)
	{
		int LeastLenth = DecryptBufLen - (pData - DecryptBuf) - 5;
		pData = FindMemory("\xa6\x00\x0b\x1d\x00",5,pData, LeastLenth);
		if (pData == 0)
		{
			return FALSE;
		}	

		pDataLen = ntohs(*(unsigned short*)pData);
		pData += 2;
	}
	else if ( Flag == 0xf9)
	{
		int LeastLenth = DecryptBufLen - (pData - DecryptBuf) - 5;
		pData = FindMemory("\xa6\x00\x0b\x1d\x00",5,pData, LeastLenth);
		if (pData == 0)
		{
			return FALSE;
		}	
		pDataLen = ntohs(*(unsigned short*)pData);
		pData += 2;	
	}
	else if ( Flag == 0xfa)
	{
		int LeastLenth = DecryptBufLen - ((char*)pData - DecryptBuf) - 6;
		pData = FindMemory("\xa6\x00\x0b\x1d\x00\x01",6,pData, LeastLenth);
		if (pData == 0)
		{
			return FALSE;
		}

		pDataLen = ntohs(*(unsigned short*)pData);
		pData += 2;
	}
	else
	{
		int LeastLenth = DecryptBufLen - (pData - DecryptBuf) - 6;
		pData = FindMemory("\xa6\x00\x0b\x1d\x00\x01",6,pData, LeastLenth);
		if (pData == 0)
		{
			pData = FindMemory("\xa6\x00\x0b\x1d\x00",5,DecryptBuf, DecryptBufLen - 5);
			if (pData == 0)
			{
				return FALSE;
			}	
		}
		
		pDataLen = ntohs(*(unsigned short*)pData);
		pData = pData + 2;	
	}

	* pDataBuf = pData;
	* pDataBufLen = pDataLen;
	return TRUE;
}
























//通话记录在HTTP 200 OK中没法获取数据包没有处理
// 0a 序号 10/11 04/03 ec 20 01 30 01 4a 06 0a
int ProcQQPimSyncPacket(DATEHEADER & packh ,const char * pPacket, int PacketSize, HttpProtocol * HttpPro)	//密钥写在文件中不需要获取参数
{
	char * pData = (char*)(pPacket + 4);
	unsigned int pDataLen = PacketSize - 4;

	char DecryptBuf[QQPIM_DECRYPTBUF_LEN];
	int DecryptBufLen = QQPIM_DECRYPTBUF_LEN;
	unsigned char DecodeBuf[QQPIM_DECRYPTBUF_LEN];
	unsigned long DecodeBufLen = QQPIM_DECRYPTBUF_LEN;

	DecryptBufLen = sub_183C((char*)pData,pDataLen,"DFG#$%^#%$RGHR(&*M<><",21,DecryptBuf,DecryptBufLen,0);
	if (DecryptBufLen > 0)
	{
		char QQ[QQPIMDATA_ITEM_LEN] = {0};
		char Imei[QQPIMDATA_ITEM_LEN] = {0};

		pDataLen = 0;
		pData = 0;
		int Result = ParseQQpimSyncPacket(DecryptBuf,DecryptBufLen,&pData,&pDataLen,QQ,Imei);		//处理同步数据包并返回QQ IMEI
		if(Result == 0)
		{
			return FALSE;
		}

		if ( (pData - DecryptBuf >= DecryptBufLen ) || (pDataLen >= DecryptBufLen ) )	//////////////////////////////////////////////////////////////////////////
		{
			return FALSE;
		}

		QQPIMDATA * QQpimdata = 0;
		char QQPimDataKey[256];
		sprintf(QQPimDataKey, "%s_%s",QQ, Imei);
		Result = cmapQQPimData.Lookup(QQPimDataKey, QQpimdata);		//检查返回的QQ IMEI是否有密钥
		if (Result == 0)
		{
			return FALSE;
		}
		
		DecryptBufLen = QQPIM_DECRYPTBUF_LEN;
		DecryptBufLen = sub_183C((char*)pData,pDataLen,(char*)QQpimdata->QQPimKey,QQpimdata->QQPimKeyLen,DecryptBuf,DecryptBufLen,0);
		if (DecryptBufLen > 0)
		{
			DecodeBufLen = QQPIM_DECRYPTBUF_LEN;
			Result = zdecompress((unsigned char *)DecryptBuf,DecryptBufLen, DecodeBuf,&DecodeBufLen);
			if (Result == 0)
			{
				DecodeBufLen = Utf8ToGB2312((char*)DecodeBuf,DecodeBufLen);

			}
			else
			{
				return FALSE;
			}
		}
		else		//先不要删除KEY
		{
			return FALSE;
		}

		

		char  DataBuf[QQPIM_DECRYPTBUF_LEN];
		int DataBufLen = QQPIM_DECRYPTBUF_LEN;

		memset(DecodeBuf + DecodeBufLen + 4, 0, QQPIM_DECRYPTBUF_LEN - DecodeBufLen - 4);
//Gf	Result = ProcVObject((char*)DecodeBuf,DecodeBufLen,DataBuf,&DataBufLen,"BEGIN:VCARD","END:VCARD",VCARD_MAX_SIZE);

		CString strData = PreVCARD((char*)DecodeBuf,DecryptBufLen);
//		if (Result)
		if (!strData.IsEmpty())
		{
			WriteQQPimIndexFile(packh, HttpPro, strData.GetBuffer(0),QQpimdata);
			return TRUE;
		}	
		else
		{
			Result = ProcQQPhoneBookVobject((char*)DecodeBuf,DecodeBufLen,DataBuf,&DataBufLen,"BEGIN:VCARD","END:VCARD",VMESSAGE_MAX_SIZE);
			if (Result)
			{
				WriteQQPimIndexFile(packh, HttpPro, DataBuf,QQpimdata);
				return TRUE;
			}
			else
			{
				return FALSE;
			}

		}
	}

	return FALSE;
}














int ProcQQPimMessage(char * DecryptBuf, int DecryptBufLen,char * DestBuf, int * DestBufLen)
{
	return FALSE;

	int ContentSize = 0;
	unsigned char * pData = (unsigned char*)DecryptBuf + 12;
	unsigned int pDataLen = DecryptBufLen - 12 ;
	*(unsigned int*)DestBuf = ContentSize;



	memmove(DestBuf + 4, pData, pDataLen);
	* DestBufLen = pDataLen;
	*(unsigned int*)(DestBuf + pDataLen) = 0;
	Utf8ToGB2312(DestBuf,pDataLen);

	return TRUE;
}








#define DECRYPTKEY_BUFSIZE	256

//可能有多个请求或者回复
int GetQQPimServerData(DATEHEADER & packh,const char * DecryptBuf, int DecryptBufLen, QQSyncAsist * QQsyncAsist, HttpProtocol * HttpPro)	
{
	int ContentSize = 0;
	char * pData	= 0;
	int pDataLen	= 0;
	int Result = 0;
	int Action = 0;
	char DecodeBuf[QQPIM_DECRYPTBUF_LEN];
	int DecodeBufLen = QQPIM_DECRYPTBUF_LEN;

	while (ContentSize < DecryptBufLen)				//DecryptBuf递增	DecryptBufLen不变
	{
		pData = (char*)DecryptBuf + 11 ;			//长度 10 02 2c 3c 40 14 56
		pDataLen = * pData;
		pData = pData + 1;
		if ( memcmp(pData ,"wupsync",strlen("wupsync")) == 0 )
		{
			pData = pData + pDataLen;
			pData = pData + 1;
			
			pDataLen = *pData;
			pData = pData + 1;
			if( memcmp(pData,"GetUserIdentity",strlen("GetUserIdentity")) == 0 || memcmp(pData,"init",strlen("init")) == 0)
			{		
				pData = pData + pDataLen;
				pData = pData + 1;
				pData = pData + 13;				//00 01 01 08 08 00 02 06 00 18 00 01 06
				
				pDataLen = * pData;				//int32
				pData = pData + 1;
				pData = pData + pDataLen;
				
				pData = pData + 6;				//1d 00 00 01 0c 06
				pDataLen = * pData;
				pData = pData + 1;
				pData = pData + pDataLen;		//resp

				pData = pData + 4;				//18 00 01 06
				pDataLen = * pData;
				pData = pData + 1;
				if (memcmp(pData ,"WUPSYNC.GetUserIdentityResp",strlen("WUPSYNC.GetUserIdentityResp")) == 0)
				{	
					pData = pData + pDataLen;
					pData = pData + 10;			//1d 00 01 00 3f/c9 0a 0c 1c 2c 36

					pDataLen = * pData;
					pData = pData + 1;

					QQsyncAsist->Flag = QQPIM;

				}					
				else if (memcmp(pData,"WUPSYNC.InitResp",strlen("WUPSYNC.InitResp")) == 0 )
				{
					pData = pData + pDataLen;
					pData = pData + 10;			//1d 00 01 00 3f/c9 0a 0c 1c 2c 36

					pDataLen = ntohl(*(unsigned int*)DecryptBuf) - (pData - DecryptBuf ) - 6;
					pData = FindMemory("\x0b\x6c\x7c\x8c\x9c\xa6",6,pData, pDataLen);
					if (pData == 0)
					{
						return FALSE;
					}


					pDataLen = * pData;
					pData = pData + 1;	
					QQsyncAsist->Flag = QQPHONEBOOK;
				}
				else
				{
					return FALSE;
				}
					
				//1c 2c 3c 4c 0b 0a 00 09 1c 2c 3c 4c 0b 6c 7c 8c 9c a6
				if ( pDataLen >= (QQPIMDATA_ITEM_LEN * 2) || pDataLen <= 0)
				{
					return FALSE;
				}
				unsigned char QQPimServerKey[QQPIMDATA_ITEM_LEN  ] = {0};		//长度不会超过16字节	/////////////////////////////////////////
				int QQPimServerKeyLen = AscToHex((unsigned char *)pData, pDataLen, QQPimServerKey);
				if (QQPimServerKeyLen == 0)
				{
					ContentSize = ContentSize + ntohl(*(unsigned int*)DecryptBuf);
					DecryptBuf = DecryptBuf + ntohl(*(unsigned int*)DecryptBuf);
					continue;		
				}

				char KeyDecryptBuf[DECRYPTKEY_BUFSIZE]	= {0};		//这个缓冲区只是为了解密密钥，可以定的小一点节省内存
				int KeyDecryptBufLen = DECRYPTKEY_BUFSIZE;					
				KeyDecryptBufLen = sub_183C((char*)QQPimServerKey, QQPimServerKeyLen, 
					(char*)(QQsyncAsist->QQPimData.QQPimKey), QQsyncAsist->QQPimData.QQPimKeyLen, KeyDecryptBuf,KeyDecryptBufLen,0);
				if (KeyDecryptBufLen > 0)
				{
					memset(QQsyncAsist->QQPimData.QQPimKey, 0, QQPIMDATA_ITEM_LEN);
					memmove(QQsyncAsist->QQPimData.QQPimKey, KeyDecryptBuf,KeyDecryptBufLen);
					QQsyncAsist->QQPimData.QQPimKeyLen = KeyDecryptBufLen;
				}
				else
				{
					ContentSize = ContentSize + ntohl(*(unsigned int*)DecryptBuf);
					DecryptBuf = DecryptBuf + ntohl(*(unsigned int*)DecryptBuf);
					continue;			
				}

				QQPIMDATA * QQpimdata = new QQPIMDATA;
				InitQQPimData(QQpimdata);
 				memmove(QQpimdata, &QQsyncAsist->QQPimData, sizeof(QQPIMDATA));
				char QQPimDataKey[QQPIMDATA_ITEM_LEN];
				if (QQsyncAsist->Flag == QQPIM)
				{
					
					sprintf(QQPimDataKey, "%s_%s",QQpimdata->QQ, QQpimdata->Imei);
					cmapQQPimData.SetAt(QQPimDataKey, QQpimdata);
					cmapQQPimDataCnt ++;
				}
				else if (QQsyncAsist->Flag == QQPHONEBOOK)
				{
					//QQ通讯录直接解密HTTP数据包 没有公共密钥 所以为了解密用SOCKET来作为CMAP的KEY以方便查找
					//另外每个HTTP回话客户端端口都会加1所以不用客户端的端口 目的IP也会增加
					sprintf(QQPimDataKey, "%08x%08x%04x%04x",*(unsigned int*)packh.m_sip &0x00ffffff,*(unsigned int*)packh.m_dip,
						packh.m_sport,0);
					cmapQQPimData.SetAt(QQPimDataKey, QQpimdata);
					cmapQQPimDataCnt ++;


					//////////////////////////////////////////////////////////////////////////
					//QQ通讯录消息用AES加密
// 					sprintf(QQPimDataKey, "%08x%08x%04x%04x",0,*(unsigned int*)packh.m_dip,0,0);
// 					cmapQQPimData.SetAt(QQPimDataKey, QQpimdata);
// 					cmapQQPimDataCnt ++;
				}
			}
			else if (memcmp(pData,"getRecycle",strlen("getRecycle")) == 0)
			{
				pData = pData + pDataLen;
				pData = pData + 1;				//}
				pData = pData + 12;				//00 01 03 a3 08 00 02 06 00 18 00 01

				pDataLen = * pData;
				pData = pData + 1;
				pData = pData + pDataLen;

				pData = pData + 6;				//1d 00 00 01 0c 06
				pDataLen = * pData;
				pData = pData + 1;
				pData = pData + pDataLen;

				pData = pData + 4;				//18 00 01 06
				pDataLen = * pData;
				pData = pData + 1;
				if (memcmp(pData,"WUPSYNC.GetRecycleResp",strlen("WUPSYNC.GetRecycleResp")) == 0)	//recycle等只有一个数据段
				{
					//25字节
					//1d 00 01 03   
					//69 0a 0c 10   19 22 54 bc   5e e8 39 00   19 0a 00 10   12 54 b6 1b   88
					pData = pData + pDataLen;
					pData = pData + 4;		//1d 00 01 04 b3 0a 0c 10
					pData = pData + 16;
					pDataLen = DecryptBufLen - (pData - DecryptBuf);		
					
					DecodeBufLen = QQPIM_DECRYPTBUF_LEN;
					Result = ProcQQPim_Recycle_RollBack(pData, pDataLen,DecodeBuf, &DecodeBufLen);
					if (Result)
					{
						WriteQQPimIndexFile(packh, HttpPro, (char*)DecodeBuf, &QQsyncAsist->QQPimData);
						Action = ACTION_RECYCLE;
					}
				}
			}
			else if (memcmp(pData,"GetCloudData",strlen("GetCloudData")) == 0)
			{
				pData = pData + pDataLen;
				pData = pData + 1;				//}
				pData = pData + 13;				//00 01 03 a3 08 00 02 06 00 18 00 01 06
				
				pDataLen = * pData;
				pData = pData + 1;
				pData = pData + pDataLen;
				
				pData = pData + 6;				//1d 00 00 01 0c 06
				pDataLen = * pData;
				pData = pData + 1;
				pData = pData + pDataLen;
				
				pData = pData + 4;				//18 00 01 06
				pDataLen = * pData;
				pData = pData + 1;
				if (memcmp(pData,"WUPSYNC.GetCloudDataResp",strlen("WUPSYNC.GetCloudDataResp")) == 0)	//到26之前23字节 16字节MD5
				{
					pData = pData + pDataLen;
					pData = pData + 4;		
					//1d 00 01 03  
					//67 0a 0c 12  
					//54 b6 1a bd  
					//29 00 19 0a  
					//11 19 0a 00  
					//19 12 54 9a 
					//a7 2c 
					pData = pData + 16;
					pDataLen = DecryptBufLen - (pData - DecryptBuf);	
					
					DecodeBufLen = QQPIM_DECRYPTBUF_LEN;
					Result = ProcQQPim_Recycle_RollBack(pData, pDataLen,DecodeBuf, &DecodeBufLen);
					if (Result)
					{
						WriteQQPimIndexFile(packh, HttpPro, (char*)DecodeBuf, &QQsyncAsist->QQPimData);
						Action = ACTION_ROLLBACK;
					}
				}
			}
			else if (memcmp(pData,"GetSMSSummary",strlen("GetSMSSummary")) == 0)
			{
				pData = pData + pDataLen;
				pData = pData + 1;				//}
				pData = pData + 13;				//00 01 27 24 08 00 02 06 00 18 00 01 06
				
				pDataLen = * pData;
				pData = pData + 1;
				pData = pData + pDataLen;		//int32
				
				pData = pData + 6;				//1d 00 00 01 0c 06
				pDataLen = * pData;
				pData = pData + 1;				//resp
				pData = pData + pDataLen;
				
				pData = pData + 4;				//18 00 01 06
				pDataLen = * pData;
				pData = pData + 1;
				if (memcmp(pData,"WUPSYNC.GetSMSSummaryResp",strlen("WUPSYNC.GetSMSSummaryResp")) == 0)
				{
					pData = pData + pDataLen;
					//pData = pData + 12;		//1d 00 01 01 47 0a 0c 19 00 0f 0a 06
					pDataLen = DecryptBufLen - (pData - DecryptBuf);		
					Result = ProcQQPimMessage(pData, pDataLen,DecodeBuf, &DecodeBufLen);
					if (Result)
					{
						WriteQQPimIndexFile(packh, HttpPro, (char*)DecodeBuf, &QQsyncAsist->QQPimData);
						Action = ACTION_MESSAGE;
					}
				}
			}
		}

		ContentSize = ContentSize + ntohl(*(unsigned int*)DecryptBuf);
		DecryptBuf = DecryptBuf + ntohl(*(unsigned int*)DecryptBuf);
	}
	return Action;
}



//字符串后面是16字节的位置数据 然后是10字节的固定数据
//名字以&开头，后面是长度，然后是姓名
//接下来是电话的长度，接下来是电话
//电话后面以@结尾
//每一项中间有10字节的乱码
int ProcQQPim_Recycle_RollBack(char * pDataBuf, int pDataBufLen, char * DestBuf, int * DestBufLen)
{	
	int ContentSize = 4;
	char * pData = pDataBuf;
	int pDataLen = 0;
	int Cnt = 0;
	
	while ( pData - pDataBuf < pDataBufLen)
	{
		pData = strchr(pData,'&');
		if (pData == 0)
		{
			break;
		}
		
		pData += 1;
		pDataLen = * pData;
		pData += 1;
		if ( *(pData + pDataLen)  != '6')
		{
			break;
		}
		memmove(DestBuf + ContentSize, pData,  pDataLen);
		ContentSize += pDataLen;
		
		*(DestBuf + ContentSize) = '-';
		ContentSize ++;

		pData = pData + pDataLen + 1;
		pDataLen = * pData;
		pData ++;
		if ( *(pData + pDataLen) != '@')
		{
			break;
		}
		memmove(DestBuf + ContentSize, pData,  pDataLen);
		ContentSize += pDataLen;

		pData = pData + pDataLen + 1;
		pData = pData + 10;
		
		memmove(DestBuf + ContentSize, "\r\n",  2);
		ContentSize += 2;	
		
		Cnt ++;
	}
	
	*(unsigned int*)DestBuf = Cnt;
	if (ContentSize > 4)
	{
		*(unsigned int*)(DestBuf + ContentSize) = 0;
		*DestBufLen = ContentSize;
		Utf8ToGB2312(DestBuf,ContentSize);
		return TRUE;
	}
	return FALSE;
}





//需要处理回复和时光机的数据以及通话记录数据
int	ProcQQPimHttpDownPacket(DATEHEADER & packh,const char * pPacket, int PacketSize,QQSyncAsist * QQsyncAsist, HttpProtocol * HttpPro)
{
	char * Data = (char*)pPacket;
	int DataLen = PacketSize;

	char DecryptBuf[QQPIM_DECRYPTBUF_LEN] ;
	int DecryptBufLen = QQPIM_DECRYPTBUF_LEN;
	unsigned char DecodeBuf[QQPIM_DECRYPTBUF_LEN];
	unsigned long  DecodeBufLen = QQPIM_DECRYPTBUF_LEN;


	char Key[QQPIMDATA_ITEM_LEN] = {0};
	if ( QQsyncAsist->Flag == QQPHONEBOOK ) 
	{
		memmove(Key, "@#%^SEF#$Sddhfvh", strlen("@#%^SEF#$Sddhfvh"));
	}
	else if ( QQsyncAsist->Flag == QQPIM)
	{
		memmove(Key, "DFG#$%^#%$RGHR(&*M<><",strlen("DFG#$%^#%$RGHR(&*M<><"));
	}
	else if (QQsyncAsist->Flag == QQPHONEBOOK_DATA)
	{
		int Result = ProcQQPhoneBookDownPack(packh, (char*)pPacket, PacketSize, QQsyncAsist,HttpPro);
		return TRUE;				//处理完毕
	}
	else
	{
		return FALSE;
	}


	if (memcmp(Data,"\x1f\x8b\x08\x00\x00\x00\x00\x00",8) == 0)			//GzipHdr += 15;GzipLen -= 23;
	{
		int Result = Mygzdecompress((unsigned char*)Data + 10 ,DataLen - 10, DecodeBuf ,&DecodeBufLen);
		if (Result == 0)
		{
			DecryptBufLen = sub_183C((char*)DecodeBuf,(int)DecodeBufLen,Key,strlen(Key),DecryptBuf,DecryptBufLen,0);
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		DecryptBufLen = sub_183C((char*)Data,DataLen,Key,strlen(Key),DecryptBuf,DecryptBufLen,0);
	}

	if (DecryptBufLen > 0)
	{
		//此函数有2个用途 1获取服务器端密钥解密并放入结构体插入CMAP 2处理公共密钥加密的数据
		memset(DecryptBuf + DecryptBufLen + 4, 0, QQPIM_DECRYPTBUF_LEN - DecryptBufLen - 4);
		int Result = GetQQPimServerData(packh,DecryptBuf,DecryptBufLen,QQsyncAsist,HttpPro);
		if (Result == 0)
		{
			return FALSE;
		}
	}
	return TRUE;		//返回值无用
}























//CMAP
//1 匹配HTTP上行和下行数据
//2 作为HTTP分片的缓冲区和作为14000同步数据的缓冲区
//数据包可能被分割，需要组合
//需要根据返回值来确定是否需要删除CMAP中的类，返回值代表是否已经完成MAP中的类相关的处理操作，如果为0后面要删除类对象,因为每个流的SOCKET都不相同
//服务器回应数据流第一个包总有HTTP/1.1 200 OK

int ProcQQPimPacketStream(DATEHEADER & packh, const char * pPacket, HttpProtocol * HttpPro)
{
	try
	{
		if (packh.m_datalen <= 0 )
		{
			return TRUE;
		}

		QQSyncAsist  * QQsyncAsist = 0;
		char QQPimSockKey[QQPIMDATA_ITEM_LEN];
		int Result = 0;
		
		if( packh.m_sport == 80)
		{
			sprintf(QQPimSockKey,"%08x%08x%04x%04x",*(unsigned int*)packh.m_dip,*(unsigned int*)packh.m_sip,packh.m_dport,packh.m_sport);
			Result = cmapQQSyncAsist.Lookup(QQPimSockKey,QQsyncAsist);
			if (Result)
			{
				char * Header = (char*)pPacket;
				int DataLen = packh.m_datalen;
				char * Ender = 0;
				if (memcmp(pPacket,"HTTP/1.1 200 OK",strlen("HTTP/1.1 200 OK")) == 0)		//HTTP/1.1 200 OK Content-Length \r\n\r\n
				{
					char ContentLen[QQPIMDATA_ITEM_LEN] = {0};
					Header = strstr(pPacket,"\r\nContent-Length: ");
					if (Header ==0 )
					{
						cmapQQSyncAsist.RemoveKey(QQPimSockKey);		//每个流端口递增
						delete QQsyncAsist;
 						cmapQQSyncAsistCnt --;
						return TRUE;
					}
					Header += strlen("\r\nContent-Length: ");
					Ender = strstr(Header,"\r\n");
					if (Ender == 0)
					{
						cmapQQSyncAsist.RemoveKey(QQPimSockKey);		
						delete QQsyncAsist;
 						cmapQQSyncAsistCnt --;		
						return TRUE;
					}

					memmove(ContentLen,Header,Ender - Header);
					QQsyncAsist->ContentLen = atoi(ContentLen);
					if( (QQsyncAsist->ContentLen <= 0) || (QQsyncAsist->ContentLen >= QQPIM_DECRYPTBUF_LEN) )
					{
						cmapQQSyncAsist.RemoveKey(QQPimSockKey);		
						delete QQsyncAsist;
 						cmapQQSyncAsistCnt --;		
						return TRUE;
					}
					
					Header = strstr(Ender + 2,"\r\n\r\n");		
					if (Header)
					{
						Header += 4;			//数据包有HTTP/1.1 200 OK时执行
						DataLen = DataLen - (Header - pPacket) ;
					}
					else
					{
						cmapQQSyncAsist.RemoveKey(QQPimSockKey);		
						delete QQsyncAsist;
 						cmapQQSyncAsistCnt --;	
						return TRUE;
					}
				}


				int DeltaTime = time(0) - QQsyncAsist->Time;
				if( (QQsyncAsist->DataLen + packh.m_datalen >= QQPIM_DECRYPTBUF_LEN) || (DeltaTime >= QQPIM_TIMEOUT) )
				{
					cmapQQSyncAsist.RemoveKey(QQPimSockKey);		//同步分片整合完毕删除		
					delete QQsyncAsist;
					cmapQQSyncAsistCnt --;	
					return TRUE;
				}

				//数据包被分片没有HTTP/1.1 200 OK时先缓存//这个返回值代表数据包是否被分片
				memmove(QQsyncAsist->DataBuf + QQsyncAsist->DataLen, Header, DataLen);
				QQsyncAsist->DataLen += DataLen;
				if (QQsyncAsist->DataLen == QQsyncAsist->ContentLen)		//数据包缓存完毕长度和Content-Length长度相同
				{
					//packh.m_datalen = QQsyncAsist->ContentLen;
					Result = ProcQQPimHttpDownPacket(packh,QQsyncAsist->DataBuf,QQsyncAsist->ContentLen,QQsyncAsist,HttpPro);
					cmapQQSyncAsist.RemoveKey(QQPimSockKey);		
					delete QQsyncAsist;
 					cmapQQSyncAsistCnt --;	
				}
				return TRUE;
			}		
		}
		else if( (packh.m_sport == 14000) || (packh.m_dport == 14000) )
		{
			unsigned int DataSize = ntohl(*(unsigned int*)pPacket);
			if ( DataSize == packh.m_datalen - 4)
			{
				if( (DataSize & 3 ) == 0 )
				{
					ProcQQPimSyncPacket(packh,pPacket,packh.m_datalen,HttpPro);
				}
			}
// 			else if ( *(unsigned int*)pPacket == 0xfcffffff )			//////////////////////////////////////////////////////////////////////////
// 			{
// 				DataSize = ntohl(*(unsigned int*)(pPacket + 4));
// 				if( DataSize = packh.m_datalen - 4)
// 				{
// 					ProcQQPhoneBookChat(packh,pPacket,packh.m_datalen,HttpPro);
// 				}
// 			}
			else	//一个数据流超过了MTU就不符合上述条件需要整合起来解密
			{
				sprintf(QQPimSockKey,"%08x%08x%04x%04x",*(unsigned int*)packh.m_dip,*(unsigned int*)packh.m_sip,packh.m_dport,packh.m_sport);
				Result = cmapQQSyncAsist.Lookup(QQPimSockKey,QQsyncAsist);
				if (Result)		//查找到了就放入数据包
				{
					int DeltaTime = time(0) - QQsyncAsist->Time;
					if( (QQsyncAsist->DataLen + packh.m_datalen >= QQPIM_DECRYPTBUF_LEN) || (DeltaTime >= QQPIM_TIMEOUT) )
					{
						cmapQQSyncAsist.RemoveKey(QQPimSockKey);		//同步分片整合完毕删除		
						delete QQsyncAsist;
						cmapQQSyncAsistCnt --;	
						return TRUE;
					}

					memmove(QQsyncAsist->DataBuf + QQsyncAsist->DataLen, pPacket, packh.m_datalen);
					QQsyncAsist->DataLen += packh.m_datalen;
					DataSize = ntohl(*(unsigned int*)(QQsyncAsist->DataBuf));
					if( (DataSize == QQsyncAsist->DataLen - 4) && (DataSize & 3) == 0 )
					{
						//packh.m_datalen = QQsyncAsist->DataLen;
						Result = ProcQQPimSyncPacket(packh ,QQsyncAsist->DataBuf, QQsyncAsist->DataLen,HttpPro);		//任何返回值都要执行删除
						cmapQQSyncAsist.RemoveKey(QQPimSockKey);		//同步分片整合完毕删除		
						delete QQsyncAsist;
						cmapQQSyncAsistCnt --;		
					}
				}
				else if( ( (DataSize & 3) == 0 )  && (packh.m_datalen >= 1340) )		//能不能确定数据包最大是1348？
				{
					QQsyncAsist = new QQSyncAsist;
					memmove(QQsyncAsist->DataBuf + QQsyncAsist->DataLen, pPacket, packh.m_datalen);
					QQsyncAsist->DataLen += packh.m_datalen;
					cmapQQSyncAsist.SetAt(QQPimSockKey,QQsyncAsist);
					cmapQQSyncAsistCnt ++;	
				}
			}
			return TRUE;
		}	
		return FALSE;

	}
	catch (...)
	{
		WriteLog(HTTPPRO, "ProcessQQPIM http Error!!");			
	}
	return TRUE;

}