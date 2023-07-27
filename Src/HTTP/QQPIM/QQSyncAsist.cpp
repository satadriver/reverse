#include "stdafx.h"
#include "QQSyncAsist.h"
#include "QQPhoneBook.h"
#include "WechatContact.h"







//解密函数返回解密长度（等于后面4字节） 
//头四字节是第一部分长度
int GetQQPimClientData(const char * DecryptBuf, int DecryptBufLen, LPQQPIMDATA QQPimData)
{
	int ContentSize = 0;
	char * pData = 0;							
	int pDataLen = 0;
	int Action = 0;

	while (ContentSize < DecryptBufLen)		//4a 2c 3c 40 01 56	
	{
		pData = FindMemory("\x2c\x3c\x40",3,(char*)DecryptBuf,64);	////////////////////////////////////
		if (pData == 0)
		{
			ContentSize = ContentSize + ntohl(*(unsigned int*)DecryptBuf);
			DecryptBuf = DecryptBuf + ntohl(*(unsigned int*)DecryptBuf);
			continue;
		}
		pData += 2;

		//pData = (char*)DecryptBuf + 11 ;		//四字节头部长度 10 02 2c 3c 40 01 56 此处长度固定
		pDataLen = * pData;
		pData = pData + 1;
		if (memcmp(pData ,"wupsync",strlen("wupsync")) == 0) 
		{
			pData = pData + pDataLen;
			pData = pData + 1;						//一个字节的66，所有HTTP包都有

			pDataLen = *pData;
			pData = pData + 1;
			if( memcmp(pData,"GetUserIdentity",strlen("GetUserIdentity")) == 0 || (memcmp(pData ,"init",strlen("init")) == 0) )//获取客户端密钥	
			{		
				pData = pData + pDataLen;
				pData = pData + 1;					//}
				pData = pData + 8;					//00 01 00 99/c5/cf/d0/e6 08 00 01 06

				pDataLen = * pData;					//req resp
				pData = pData + 1;
				pData = pData + pDataLen;

				pData = pData + 4;					//18 00 01 06
				pDataLen = * pData;
				pData = pData + 1;
				if ( memcmp(pData ,"WUPSYNC.GetUserIdentityReq",strlen("WUPSYNC.GetUserIdentityReq")) == 0 ||  
					memcmp(pData ,"WUPSYNC.InitReq",strlen("WUPSYNC.InitReq")) == 0 ) 
				{
					pData = pData + pDataLen;
					pData = pData + 10;				//1d 00 01 00 a3/a5 0a 0a 00 01 16

					pDataLen = * pData;
					pData = pData + 1;
					memmove(QQPimData->QQ, pData, pDataLen);
					QQPimData->QQLen = pDataLen;
					pData = pData + pDataLen + 1;		//&

					char * pTmpData = pData;
					pTmpData = FindMemory("\x0b\x1a\06",3,pTmpData,256);	//0b 1a 06 14/0f ///////////////////////////////////////////////////
					if (pTmpData == 0)
					{
						pTmpData = strstr(pData,"COMN:");
						if (pTmpData == 0)
						{
							//中间是一段字符串和一段MD5加上 0b 1a 06 
							ContentSize = ContentSize + ntohl(*(unsigned int*)DecryptBuf);
							DecryptBuf = DecryptBuf + ntohl(*(unsigned int*)DecryptBuf);
							continue;
						}
						pDataLen = *(pTmpData - 1);
						pTmpData += 5;
						pDataLen -= 5;
					}
					else
					{
						pDataLen = * pTmpData;
						pTmpData ++;
					}

					pData = pTmpData;
					if (memcmp(pData,"COMN:",5) == 0)
					{
						pData += 5;
						pDataLen -= 5;
					}

					memmove(QQPimData->Imei, pData, pDataLen);
					QQPimData->ImeiLen = pDataLen;
					pData = pData + pDataLen;

					pData = pData + 1;			//16
					pDataLen = *pData;
					pData = pData + 1;
					memmove(QQPimData->Imsi,pData,pDataLen);
					QQPimData->ImsiLen = pDataLen;
					pData = pData + pDataLen;

					pData ++;					//& 
					pDataLen = * pData;
					pData = pData + pDataLen + 1;
					pData ++;					//36
					pDataLen = * pData;
					pData = pData + pDataLen + 1;
									

					//长度 手机品牌 36
					//长度 手机型号
					//46 02 31 35 6c 7c 8c 9c
					//0b 22 54 bf 84 5c 31 7d		//可变化的
					//00 41 01 f4 5c 6c 7a 01 02 80 11 02 80 2c 0b 89 00 01 0a 00 01 共计21字节相同数据
					//10 25 2c 3c 4c 0b 9c a6		//可变化的
					//长度 服务器密钥

					pDataLen = ntohl(*(unsigned int*)DecryptBuf) - (pData - DecryptBuf);	//从最后面查找 最后面是\x0b\x8c\x98\x0c\xa8\x0c
					pData = FindMemory("\x0b\x8c\x98\x0c\xa8\x0c",6,pData,pDataLen);	/////////////////////////////////////
					if (pData == 0)
					{
						ContentSize = ContentSize + ntohl(*(unsigned int*)DecryptBuf);
						DecryptBuf = DecryptBuf + ntohl(*(unsigned int*)DecryptBuf);
						continue;
					}

					pData = pData - 6;
					pData --;
					pTmpData = pData;
					for (int Cnt = 0; Cnt < QQPIMDATA_ITEM_LEN; Cnt ++)
					{
						char TmpChr = *(pData - Cnt);
						if (TmpChr == Cnt)
						{
							pDataLen = Cnt;
							pData = pData - Cnt + 1;
							memmove(QQPimData->QQPimKey,pData,pDataLen);
							QQPimData->QQPimKeyLen = pDataLen;	
							Action = ACTION_IDENTITYREQ;
							break;
						}
					}
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

					pData = strstr(pData,"COMN:");	//前面固定5字节08 04 0b 16 14
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
					Action = ACTION_RECYCLEREQ;
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
					
					pData = strstr(pData,"COMN:");	//08 04 0b 16 14
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
					Action = ACTION_ROLLBACKREQ;
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
					Action = ACTION_CLOUDDATAREQ;
				}
			}
			else if (memcmp(pData,"GetSMSSummary",pDataLen) == 0)
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
				if (memcmp(pData,"WUPSYNC.GetSMSSummaryReq",pDataLen) == 0)
				{
					pData += pDataLen;
					pData += 9;			//1d 00 01 01 24 0a 0a 00 01 16;
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
					Action = ACTION_MESSAGEREQ;
				}
			}
			else if (memcmp(pData,"securedownload",pDataLen) == 0)
			{
				pData += pDataLen;
				pData ++;
				
				pDataLen = *pData;
				pData ++;
				if (memcmp(pData,"DownloadCmdRequest",pDataLen) == 0)
				{
					pData += pDataLen;
					pData += 10;			//7d 00 01 17 12 08 00 01 01 06;
				}
			}
		}
		else
		{
			//"syncm"
			//"report"
			int tmp = 0;		//执行到这里说明数据包无用或者判断偏移错误
		}

		ContentSize = ContentSize + ntohl(*(unsigned int*)DecryptBuf);
		DecryptBuf = DecryptBuf + ntohl(*(unsigned int*)DecryptBuf);
	}

	return Action;		
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
		if( (pHttpSession->m_Requestline.m_Host.Find("3g.qq.com") != -1) &&			//sync.3g.qq.com 
			(pHttpSession->m_Requestline.m_useragent.Find("qqppim") != -1 || 
			pHttpSession->m_Requestline.m_useragent.Find("qqphonebook") != -1 ||
			pHttpSession->m_Requestline.m_useragent.Find("mQQPim") != -1 ) )		//微信同步
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
				Result = GetIPPort((unsigned char *)pHttpSession->m_sip.GetBuffer(0), SrcIP, &SrcPort);
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


			char Key[QQPIMDATA_ITEM_LEN*2] = {0};
			int FlagType = 0;			//这里的FLAGTYPE和后面解析密钥的时候结合 可以区别各种软件的分类
			if ( (pHttpSession->m_Requestline.m_URI.Find("/xgetresult") != -1) || 
				(pHttpSession->m_Requestline.m_URI.Find("/xpimlogin") != -1) )		//微信或者QQ通讯录 微信电话本都有这个数据包
			{
				memmove(Key, "@#%^SEF#$Sddhfvh", strlen("@#%^SEF#$Sddhfvh"));
				FlagType = QQPHONEBOOK;
			}
			else if (pHttpSession->m_Requestline.m_URI.Find("/?sig=") != -1 ) 
			{
				// /xdsync解密失败
				FlagType = QQPHONEBOOK_DATA;
				Result = ProcQQPhoneBookUpPack((char*)TmpData, TmpDataLen,packh);
				return TRUE;
			}
			else if (pHttpSession->m_Requestline.m_URI.Find("/xdsync") != -1 ) 
			{
				FlagType = WECHAT_CONTACT;
				Result = ProcWechatContactUpPack((char*)TmpData, TmpDataLen,packh);
				return TRUE;
			}
			else
			{
				FlagType = QQPIM;
				memmove(Key, "DFG#$%^#%$RGHR(&*M<><",strlen("DFG#$%^#%$RGHR(&*M<><"));
			}


			char DecryptBuf[QQPIM_DECRYPTBUF_LEN];
			int DecryptBufLen = QQPIM_DECRYPTBUF_LEN;
//			unsigned char DecodeBuf[QQPIM_DECRYPTBUF_LEN];
			unsigned long  DecodeLen = QQPIM_DECRYPTBUF_LEN;

			QQPIMDATA QQPimData;
			InitQQPimData(&QQPimData);
			DecryptBufLen = sub_183C((char*)TmpData,TmpDataLen,Key,strlen(Key),DecryptBuf,DecryptBufLen,0);//上行数据不需要解压缩直接解密
			if (DecryptBufLen > 0)
			{
// 				if (pHttpSession->m_Requestline.m_URI.Find("/xdsync") != -1 || pHttpSession->m_Requestline.m_URI.Find("/?mod=") != -1 ||
// 					pHttpSession->m_Requestline.m_URI.Find("/dsync") != -1 )		//软件备份从这里开始此处部分基本无用
// 				{
// 					Result = zdecompress((unsigned char*)DecryptBuf,DecryptBufLen, DecodeBuf, &DecodeLen);
// 					if (Result == 0)
// 					{
// 						DecodeLen = Utf8ToGB2312((char*)DecodeBuf,DecodeLen);
// 						//ParseQQPhoneBookSyncSoftPack((char*)DecodeBuf, DecodeLen,packh,pHttpSession->m_dceprocotol);	
// 						return TRUE;
// 					}
// 				}
				
				Result = GetQQPimClientData(DecryptBuf, DecryptBufLen, &QQPimData);		//返回值无用碰到适合条件的包全部放行
				if (Result == ACTION_MESSAGEREQ || Result == ACTION_IDENTITYREQ || Result == ACTION_CLOUDDATAREQ || Result == ACTION_RECYCLEREQ ||
					Result == ACTION_ROLLBACKREQ || Result == ACTION_CALLREQ)
				{
				
					char QQPimSockKey[QQPIMDATA_ITEM_LEN*2];
					sprintf(QQPimSockKey,"%08x%08x%04x%04x",*(unsigned int*)packh.m_sip,*(unsigned int*)packh.m_dip,packh.m_sport,packh.m_dport);
					QQSyncAsist * QQsyncAsist = 0;
					Result = cmapQQSyncAsist.Lookup(QQPimSockKey,QQsyncAsist);
					if (Result )			//找到了删除旧的SOCKET
					{
						cmapQQSyncAsist.RemoveKey(QQPimSockKey);
						delete QQsyncAsist;
						cmapQQSyncAsistCnt --;
					}
					QQsyncAsist = new QQSyncAsist(FlagType);
					memmove((char*)&QQsyncAsist->QQPimData, (char*)&QQPimData, sizeof(QQPIMDATA));
					cmapQQSyncAsist.SetAt(QQPimSockKey,QQsyncAsist);
					cmapQQSyncAsistCnt ++;
					return TRUE;
				}
			}
			else
			{
				int Tmp = FALSE;		//解密失败的包走这里
			}
		}
		
		return FALSE;
	}
	catch (...)
	{
		WriteLog(HTTPPRO, "ProcessQQPIM Post packet Error!!");	
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

	if( (*pData != 0x0a) || (DecryptBufLen <= 128) )	
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
	if (memcmp(pData,"COMN:",5) == 0)
	{
		pData = pData + 5;
		pDataLen = pDataLen - 5;
		
	}
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
int ProcQQPimSyncPacket(DATEHEADER & packh ,const char * pPacket, int PacketSize, HttpProtocol * HttpPro)	//密钥写在CMAP中不需要获取参数
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

		if ( (pData - DecryptBuf >= DecryptBufLen ) || (pDataLen >= DecryptBufLen ) )	
		{
			return FALSE;
		}

		QQPIMDATA * QQpimdata = 0;
		char QQPimDataKey[DECRYPTKEY_BUFSIZE];
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

		memset(DecodeBuf + DecodeBufLen + 4, 0, QQPIM_DECRYPTBUF_LEN - DecodeBufLen - 4);
		Result = ProcQQPimVObject((char*)DecodeBuf, DecodeBufLen,"BEGIN:VCARD","END:VCARD", QQpimdata,packh,HttpPro);
		if (Result == 0)
		{
			Result = ProcQQPimVMessage((char*)DecodeBuf, DecodeBufLen,"BEGIN:VMESSAGE","END:VMESSAGE", QQpimdata,packh,HttpPro);
		}
	}

	return FALSE;
}





//可能有多个请求或者回复
int GetQQPimServerData(DATEHEADER & packh,const char * DecryptBuf, int DecryptBufLen, QQSyncAsist * QQsyncAsist, HttpProtocol * HttpPro)	
{
	int ContentSize = 0;
	char * pData	= 0;
	int pDataLen	= 0;
	int Result = 0;
	int Action = 0;
	int Flag = 0;

	while (ContentSize < DecryptBufLen)				//DecryptBuf递增	DecryptBufLen不变
	{
		pData = FindMemory("\x2c\x3c\x40",3,(char*)DecryptBuf,64);
		if (pData == 0)
		{
			return FALSE;
		}

		pData += 2;
		//pData = (char*)DecryptBuf + 11 ;			//长度 10 02 2c 3c 40 14 56
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
					Flag = QQPIM;
				}					
				else if (memcmp(pData,"WUPSYNC.InitResp",strlen("WUPSYNC.InitResp")) == 0 )
				{
					pData = pData + pDataLen;
					pData = pData + 10;			//1d 00 01 00 3f/c9 0a 0c 1c 2c 36

					pDataLen = ntohl(*(unsigned int*)DecryptBuf) - (pData - DecryptBuf ) - 4;
					pData = FindMemory("\x7c\x8c",2,pData, pDataLen);		//7c 8c 9c a6 28	//	"\x0b\x6c\x7c\x8c"
					if (pData == 0)
					{
						ContentSize = ContentSize + ntohl(*(unsigned int*)DecryptBuf);
						DecryptBuf = DecryptBuf + ntohl(*(unsigned int*)DecryptBuf);
						continue;	
					}

					pDataLen = ntohl(*(unsigned int*)DecryptBuf) - (pData - DecryptBuf ) - 2;
					pData = FindMemory("\xa6\x28",2,pData, pDataLen);
					if (pData == 0)
					{
						ContentSize = ContentSize + ntohl(*(unsigned int*)DecryptBuf);
						DecryptBuf = DecryptBuf + ntohl(*(unsigned int*)DecryptBuf);
						continue;	
					}
				
// 					for (int i = 0; i< 0x28; i ++)
// 					{
// 						if (isxdigit(pData + i) == 0)
// 						{
// 							pDataLen = 0;
// 							pData = 0;
// 							break;
// 						}
// 					}
					pDataLen = * (pData - 1);
					Flag = QQPHONEBOOK;
				}
				else
				{
					ContentSize = ContentSize + ntohl(*(unsigned int*)DecryptBuf);
					DecryptBuf = DecryptBuf + ntohl(*(unsigned int*)DecryptBuf);
					continue;	
				}
					
				//1c 2c 3c 4c 0b 0a 00 09 1c 2c 3c 4c 0b 6c 7c 8c 9c a6
				if ( pDataLen >= (QQPIMDATA_ITEM_LEN * 2) || pDataLen <= 0)
				{
					ContentSize = ContentSize + ntohl(*(unsigned int*)DecryptBuf);
					DecryptBuf = DecryptBuf + ntohl(*(unsigned int*)DecryptBuf);
					continue;	
				}
				unsigned char QQPimServerKey[QQPIMDATA_ITEM_LEN *2 ] = {0};		//长度不会超过16字节
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

				QQPIMDATA * QQpimdata = 0;
				char QQPimDataKey[QQPIMDATA_ITEM_LEN];
				if (Flag == QQPIM)
				{
					sprintf(QQPimDataKey, "%s_%s",QQsyncAsist->QQPimData.QQ, QQsyncAsist->QQPimData.Imei);	//如果原来有的话还要清空
					Result = cmapQQPimData.Lookup(QQPimDataKey, QQpimdata);
					if (Result)
					{
						cmapQQPimData.RemoveKey(QQPimDataKey);
						cmapQQPimDataCnt --;
						delete QQpimdata;
					}

					QQpimdata = new QQPIMDATA;
					InitQQPimData(QQpimdata);
 					memmove(QQpimdata, &QQsyncAsist->QQPimData, sizeof(QQPIMDATA));					
					cmapQQPimData.SetAt(QQPimDataKey, QQpimdata);
					cmapQQPimDataCnt ++;
				}
				else if (Flag == QQPHONEBOOK)
				{
					//QQ通讯录直接解密HTTP数据包 没有公共密钥 所以为了解密用SOCKET来作为CMAP的KEY以方便查找
					//另外每个HTTP回话客户端端口都会加1所以不用客户端的端口 目的IP也会增加
					sprintf(QQPimDataKey, "%08x%08x%04x%04x",*(unsigned int*)packh.m_sip &0x00ffffff,*(unsigned int*)packh.m_dip,
						packh.m_sport,0);
					Result = cmapQQPimData.Lookup(QQPimDataKey, QQpimdata);
					if (Result)
					{
						cmapQQPimData.RemoveKey(QQPimDataKey);
						cmapQQPimDataCnt --;
						delete QQpimdata;
					}
					
					QQpimdata = new QQPIMDATA;
					InitQQPimData(QQpimdata);
 					memmove(QQpimdata, &QQsyncAsist->QQPimData, sizeof(QQPIMDATA));	
					cmapQQPimData.SetAt(QQPimDataKey, QQpimdata);
					cmapQQPimDataCnt ++;
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
					//1d 
					//00 01 03 69  
					//0a 0c 10 19  
					//22 54 bc 5e  
					//e8 39 00 19 
					//0a 00 10 12 
					//54 b6 1b 88
					pData = pData + pDataLen;
					pData = pData + 1;		//1d 00 01 04 b3 0a 0c 10
					pData += 4;
					pData = pData + 16;		//不要改了就是这样
					pDataLen = DecryptBufLen - (pData - DecryptBuf);		
					
					Result = ProcQQPim_Recycle_RollBack(pData, pDataLen,packh,HttpPro,&QQsyncAsist->QQPimData);
 					Action = ACTION_RECYCLEREQ;
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
					pData ++;
					pData = pData + 4;		
					//1d 
					//00 01 03  67 
					//0a 0c 12  54 
					//	b6 1a bd  29 
					//	00 19 0a  11 
					//	19 0a 00  19 
					//	12 54 9a a7 
					//	2c 
					pData = pData + 14;		//不要改了就是这样///////////////////////////////////////////////////////
					pDataLen = DecryptBufLen - (pData - DecryptBuf);	
					
					Result = ProcQQPim_Recycle_RollBack(pData, pDataLen,packh,HttpPro,&QQsyncAsist->QQPimData);
 					Action = ACTION_ROLLBACKREQ;

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
					pData = pData + 12;		//1d 00 01 01 47 0a 0c 19 00 0f 0a 06
					pDataLen = DecryptBufLen - (pData - DecryptBuf);
					Result = ProcQQPim_Recycle_RollBack(pData, pDataLen,packh,HttpPro,&QQsyncAsist->QQPimData);
 					Action = ACTION_MESSAGEREQ;
				}
			}
		}
		else
		{
			int tmp = 0;			//执行到这里说明判断偏移错误
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
int ProcQQPim_Recycle_RollBack(char * pDataBuf, int pDataBufLen,DATEHEADER &packh,HttpProtocol * HttpPro,QQPIMDATA * QQpimdata)
{	

	char * Header = pDataBuf;
	char * Ender = Header;
	int Cnt = 0;

	while ( Ender - pDataBuf < pDataBufLen)
	{
		Header = strchr(Ender,'&');		//用 & 和 6 和 @ 来表示每一项，其他的都不对
		if (Header == 0)
		{
			return FALSE;
			Header = strstr(Ender,"TP");
			if (Header == 0)
			{
				return FALSE;
			}			
		}	
		Header ++;	//可能不是长度
		Header ++;

		Ender = strchr(Header,'6');
		if (Ender == 0)
		{
			return FALSE;
		}
		CString UserName(Header,Ender - Header);
	
		Ender ++;		//可能不是长度
		Ender ++;

		Header = strchr(Ender,'@');
		if (Header == 0)
		{
			return FALSE;
		}
		CString UserPhone(Ender, Header - Ender) ;

		Ender = Header + 10 + 1;


		if (UserPhone && UserName)
		{
			WriteQQPimIndexFile(packh,HttpPro,UserName,UserPhone,QQpimdata);
		}
		
		Cnt ++;
	}
	

	if (Cnt)
	{
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
	int Result = 0;


	char Key[QQPIMDATA_ITEM_LEN*2] = {0};
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
	else if (QQsyncAsist->Flag == WECHAT_CONTACT)
	{
		int Result = ProcWechatContactDownPack(packh, (char*)pPacket, PacketSize, QQsyncAsist,HttpPro);
		return TRUE;				
	}
	else
	{
		return FALSE;
	}

	if (memcmp(Data,"\x1f\x8b\x08\x00\x00\x00\x00\x00",8) == 0)			//GzipHdr += 15;GzipLen -= 23;
	{
		DecodeBufLen = QQPIM_DECRYPTBUF_LEN;
		Result = Mygzdecompress((unsigned char*)Data + 10 ,DataLen - 10, DecodeBuf ,&DecodeBufLen);
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
		//此函数有2个用途 
		//1获取服务器端密钥解密并放入结构体插入CMAP 
		//2处理公共密钥加密的数据
		if  (QQsyncAsist->Flag == QQPHONEBOOK_DATA)			//此处基本无用
		{
// 			DecodeBufLen = QQPIM_DECRYPTBUF_LEN;
// 			Result = zdecompress((unsigned char*)DecryptBuf,DecryptBufLen,DecodeBuf,&DecodeBufLen);
// 			if (Result == 0)
// 			{		
// 				DecodeBufLen = Utf8ToGB2312((char*)DecodeBuf,DecodeBufLen);
// 				memset(DecodeBuf + DecodeBufLen + 4, 0, QQPIM_DECRYPTBUF_LEN - DecodeBufLen - 4);		//无用数据清零
// 				Result = GetQQPimServerData(packh,(char*)DecodeBuf,DecodeBufLen,QQsyncAsist,HttpPro);
// 				if (Result == 0)
// 				{
// 					return FALSE;
// 				}
// 			}
		}
		else
		{		
			memset(DecryptBuf + DecryptBufLen + 4, 0, QQPIM_DECRYPTBUF_LEN - DecryptBufLen - 4);		//无用数据清零
			DecryptBufLen = Utf8ToGB2312((char*)DecryptBuf,DecryptBufLen);
			int Result = GetQQPimServerData(packh,DecryptBuf,DecryptBufLen,QQsyncAsist,HttpPro);
			if (Result == 0)
			{
				return FALSE;
			}
		}
	}
	else
	{
		return FALSE;
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
		char QQPimSockKey[QQPIMDATA_ITEM_LEN*2];
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
 					cmapQQSyncAsistCnt --;	//相同的IP和端口会发生错误
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
		WriteLog(HTTPPRO, "ProcessQQPIMDownPacket Error!!");			
	}
	return TRUE;

}














//如果没有IMSI则不打印输出
void WriteQQPimIndexFile(DATEHEADER & stPackHeader,HttpProtocol * pHttpPro, CString UserName, CString UserPhone,QQPIMDATA * QQpimdata)
{
	UserName.Replace(" ","");
	UserPhone.Replace(" ","");
	HttpSession httpsession(stPackHeader, g_httpFrom);		
	httpsession.m_dceprocotol=pHttpPro;
	
	SYSTEMTIME stNow;
	GetLocalTime(&stNow);
	CString tmppath = httpsession.m_dceprocotol->m_httpMissObj->GetTmpPath();
	ASSERT(!tmppath.IsEmpty());
	httpsession.m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_TXL_QQTBZS.indexfile.txt", 
		tmppath, stNow.wYear, stNow.wMonth, stNow.wDay, stNow.wHour,stNow.wMinute, stNow.wSecond, httpsession.m_sip, httpsession.m_dip, rand());
	CString Txt;
	Txt.Format("user=%s\r\nimei=%s\r\nimsi=%s\r\nnick=%s\r\ncontact=%s\r\n",QQpimdata->QQ,QQpimdata->Imei,QQpimdata->Imsi,UserName,UserPhone);
	httpsession.WriteIndexFile("TXL_QQTBZS","TXL_QQTBZS",Txt,httpsession.m_dceprocotol->m_httpMissObj);	
	return;
}




void WriteQQPimMsgIndexFile(DATEHEADER & stPackHeader,HttpProtocol * pHttpPro, CString UserPhone,CString UserName, CString UserMsg,
							  QQPIMDATA * QQpimdata)
{
	HttpSession httpsession(stPackHeader, g_httpFrom);		
	httpsession.m_dceprocotol=pHttpPro;
	
	SYSTEMTIME stNow;
	GetLocalTime(&stNow);
	CString tmppath = httpsession.m_dceprocotol->m_httpMissObj->GetTmpPath();
	ASSERT(!tmppath.IsEmpty());
	httpsession.m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_TXL_QQTBZS.indexfile.txt", 
		tmppath, stNow.wYear, stNow.wMonth, stNow.wDay, stNow.wHour,stNow.wMinute, stNow.wSecond, httpsession.m_sip, httpsession.m_dip, rand());
	CString Txt;
	Txt.Format("user=%s\r\nimei=%s\r\nimsi=%s\r\nnick=%s\r\nphone=%s\r\ncontact=%s\r\n",
		QQpimdata->QQ,QQpimdata->Imei,QQpimdata->Imsi,UserName,UserPhone,UserMsg);
	httpsession.WriteIndexFile("TXL_QQTBZS","TXL_QQTBZS",Txt,httpsession.m_dceprocotol->m_httpMissObj);	
	return;
}










int ProcQQPimVObject(char * Buf, int BufLen,char * HeaderFlag, char * EnderFlag, QQPIMDATA * QQpimdata,DATEHEADER &packh,HttpProtocol * HttpPro)
{
	if( BufLen < 32 )
	{
		return FALSE;
	}
	

	int Offset = BufLen;
	char * Header = Buf;
	char * Ender = Buf;
	int HeaderFlagLen = strlen(HeaderFlag);
	int EnderFlagLen = strlen(EnderFlag);

	char Tmp[4096] = {0};
	int TmpLen = 0;
	int Count = 0;

	while ( Header - Buf < BufLen)
	{
		Header = FindMemory(HeaderFlag,HeaderFlagLen,Ender,Offset);
		if (Header == 0)
		{
			break;
		}
		
		Offset = BufLen - (Header - Buf);
		Ender = FindMemory(EnderFlag,EnderFlagLen,Header,Offset);
		if (Ender == 0)
		{
			break;
		}
		Offset = BufLen - (Ender - Buf);		
		Header -= HeaderFlagLen;

		TmpLen = Ender - Header;
		memmove(Tmp, Header, TmpLen);
		Tmp[TmpLen] = 0;

		for (int Cnt = 0; Cnt < TmpLen; Cnt ++)
		{
			if (Tmp[Cnt] == ';')
			{
				Tmp[Cnt] = ' ';
			}
		}

		CString UserName;
		CString UserPhone;
		CString strTempData(Tmp);
		CStringArray arrTemp;
		MySplitString(strTempData,arrTemp,"\r\n",strlen("\r\n") );
		
		int arrsize = arrTemp.GetSize();
		for (int i = 0; i < arrsize; i++)
		{

			if ( arrTemp[i].Find("N:") != -1 && arrTemp[i].GetLength() >= 3 )
			{
				UserName = MyGetItemValue(arrTemp[i],"N:");
			}
			else if ( arrTemp[i].Find("FN:") != -1  && arrTemp[i].GetLength() >= 3 )
			{
				UserName = MyGetItemValue(arrTemp[i],"FN:");
			}
			else if ( arrTemp[i].Find("N CHARSET=UTF-8:") != -1 )
			{
				UserName = MyGetItemValue(arrTemp[i],"N CHARSET=UTF-8:");
			}


			if ( arrTemp[i].Find("CELL:") != -1)
			{
				UserPhone = MyGetItemValue(arrTemp[i],"CELL:");
			}
			else if ( arrTemp[i].Find("CELL PREF:") != -1)
			{
				UserPhone = MyGetItemValue(arrTemp[i],"CELL PREF:");
			}
			else if ( arrTemp[i].Find("TEL X-CUSTOM=") != -1 )
			{
				UserPhone = MyGetItemValue(arrTemp[i],"TEL X-CUSTOM=");
				CString TmpUserPhone= UserPhone;
				DecodeBase64(TmpUserPhone.GetBuffer(0), TmpUserPhone.GetLength());
				UserPhone = TmpUserPhone;
				int Len = UserPhone.GetLength();
				Utf8ToGB2312(UserPhone.GetBuffer(0),Len);
			}
			else
			{
				int tmp = 0;
			}
		}
		
		if (UserName.IsEmpty() || UserPhone.IsEmpty())
		{
			continue;
		}

		WriteQQPimIndexFile(packh,HttpPro,UserName,UserPhone,QQpimdata);

		Count ++;
	}


	if (Count)
	{
		return TRUE;
	}
	
	return FALSE;
}










int ProcQQPimVMessage(char * Buf, int BufLen,char * HeaderFlag, char * EnderFlag, QQPIMDATA * QQpimdata,DATEHEADER &packh,HttpProtocol * HttpPro)
{
	if( BufLen < 32 )
	{
		return FALSE;
	}
	
	
	int Offset = BufLen;
	char * Header = Buf;
	char * Ender = Buf;
	int HeaderFlagLen = strlen(HeaderFlag);
	int EnderFlagLen = strlen(EnderFlag);
	
	char Tmp[4096] = {0};
	int TmpLen = 0;
	int Count = 0;
	
	while ( Header - Buf < BufLen)
	{
		Header = FindMemory(HeaderFlag,HeaderFlagLen,Ender,Offset);
		if (Header == 0)
		{
			break;
		}
		
		Offset = BufLen - (Header - Buf);
		Ender = FindMemory(EnderFlag,EnderFlagLen,Header,Offset);
		if (Ender == 0)
		{
			break;
		}
		Offset = BufLen - (Ender - Buf);		
		Header -= HeaderFlagLen;
		
		TmpLen = Ender - Header;
		memmove(Tmp, Header, TmpLen);
		Tmp[TmpLen] = 0;
		
		for (int Cnt = 0; Cnt < TmpLen; Cnt ++)
		{
			if (Tmp[Cnt] == ';')
			{
				Tmp[Cnt] = ' ';
			}
		}
		
		CString UserName;
		CString UserMsg;
		CString UserPhone;
		CString strTempData(Tmp);
		CStringArray arrTemp;
		MySplitString(strTempData,arrTemp,"\r\n",strlen("\r\n") );
		
		int arrsize = arrTemp.GetSize();
		for (int i = 0; i < arrsize; i++)
		{
			
			if ( arrTemp[i].Find("SENDER:") != -1 && arrTemp[i].GetLength() >= 3 )
			{
				UserPhone = MyGetItemValue(arrTemp[i],"SENDER:");
			}
			else if ( arrTemp[i].Find("SENDNAME:") != -1  && arrTemp[i].GetLength() >= 3 )
			{
				UserName = MyGetItemValue(arrTemp[i],"SENDNAME:");
			}
			else if ( arrTemp[i].Find("INFORMATION;ENCODING=BASE64:") != -1 )
			{
				UserMsg = MyGetItemValue(arrTemp[i],"INFORMATION;ENCODING=BASE64:");
				int Length = DecodeBase64(UserMsg.GetBuffer(0), UserMsg.GetLength());
				Length = Utf8ToGB2312(UserMsg.GetBuffer(0), Length);
			}
		}
		
		if (UserPhone.IsEmpty() || UserMsg.IsEmpty())
		{
			continue;
		}
		
		WriteQQPimMsgIndexFile(packh,HttpPro,UserPhone,UserName,UserMsg,QQpimdata);
		
		Count ++;
	}
	
	
	if (Count)
	{
		return TRUE;
	}
	
	return FALSE;
}