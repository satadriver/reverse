#include "stdafx.h"
#include "QQSyncAsist.h"
#include "QQPhoneBook.h"







//���ܺ������ؽ��ܳ��ȣ����ں���4�ֽڣ� 
//ͷ���ֽ��ǵ�һ���ֳ���
int GetQQPimClientData(const char * DecryptBuf, int DecryptBufLen, LPQQPIMDATA QQPimData)
{
	int ContentSize = 0;
	char * pData = 0;							
	int pDataLen = 0;
	int Action = 0;

	while (ContentSize < DecryptBufLen)		//4a 2c 3c 40 01 56
	{
		pData = FindMemory("\x2c\x3c\x40",3,(char*)DecryptBuf,64);	//�������ݰ�������
		if (pData == 0)
		{
			ContentSize = ContentSize + ntohl(*(unsigned int*)DecryptBuf);
			DecryptBuf = DecryptBuf + ntohl(*(unsigned int*)DecryptBuf);
			continue;
		}
		pData += 2;

		//pData = (char*)DecryptBuf + 11 ;		//���ֽ�ͷ������ 10 02 2c 3c 40 01 56 �˴����ȹ̶�
		pDataLen = * pData;
		pData = pData + 1;
		if (memcmp(pData ,"wupsync",strlen("wupsync")) == 0) 
		{
			pData = pData + pDataLen;
			pData = pData + 1;						//һ���ֽڵ�66������HTTP������

			pDataLen = *pData;
			pData = pData + 1;
			if( memcmp(pData,"GetUserIdentity",strlen("GetUserIdentity")) == 0 || (memcmp(pData ,"init",strlen("init")) == 0) )//��ȡ�ͻ�����Կ	
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
					pTmpData = FindMemory("\x0b\x1a\06",3,pTmpData,256);	//0b 1a 06 14/0f
					if (pTmpData == 0)
					{
						pTmpData = strstr(pData,"COMN:");
						if (pTmpData == 0)
						{
							//�м���һ���ַ�����һ��MD5���� 0b 1a 06 
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
									

					//���� �ֻ�Ʒ�� 36
					//���� �ֻ��ͺ�
					//46 02 31 35 6c 7c 8c 9c
					//0b 22 54 bf 84 5c 31 7d		//�ɱ仯��
					//00 41 01 f4 5c 6c 7a 01 02 80 11 02 80 2c 0b 89 00 01 0a 00 01 ����21�ֽ���ͬ����
					//10 25 2c 3c 4c 0b 9c a6		//�ɱ仯��
					//���� ��������Կ

					pDataLen = ntohl(*(unsigned int*)DecryptBuf) - (pData - DecryptBuf);	//���������� �������\x0b\x8c\x98\x0c\xa8\x0c
					pData = FindMemory("\x0b\x8c\x98\x0c\xa8\x0c",6,pData,pDataLen);
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

					pData = strstr(pData,"COMN:");	//ǰ��̶�5�ֽ�08 04 0b 16 14
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
			int tmp = 0;		//ִ�е�����˵�����ݰ����û����ж�ƫ�ƴ���
		}

		ContentSize = ContentSize + ntohl(*(unsigned int*)DecryptBuf);
		DecryptBuf = DecryptBuf + ntohl(*(unsigned int*)DecryptBuf);
	}

	return Action;		
}





//�ض���Կ 866309010254710h^J9o`(IMEI +	h^J9o`)		  %#!*^().,ljhd
//QQͨѶ¼��Կ@#%^SEF#$Sddhfvh    		
//ͨ����ԿDFG#$%^#%$RGHR(&*M<><	
//�ͻ�����Կ�ͷ���������Կ��һ�԰�
//���û���ҵ���Ӷ���ָ�뵽CMAPͬʱ��ȡ�ͻ�����Կ
int ProcQQPimHttpUpPacket(HttpSession * pHttpSession,DATEHEADER & packh,const char * pPacket, int Flag)
{
	try
	{
		if( (pHttpSession->m_Requestline.m_Host.Find("3g.qq.com") != -1) && 
			(pHttpSession->m_Requestline.m_useragent.Find("qqppim") != -1 || pHttpSession->m_Requestline.m_useragent.Find("qqphonebook") != -1) )
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

			if (Flag == FALSE)		//������
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

			char DecryptBuf[QQPIM_DECRYPTBUF_LEN];
			int DecryptBufLen = QQPIM_DECRYPTBUF_LEN;
			unsigned char DecodeBuf[QQPIM_DECRYPTBUF_LEN];
			unsigned long  DecodeLen = QQPIM_DECRYPTBUF_LEN;
			char Key[QQPIMDATA_ITEM_LEN*2] = {0};
			int FlagType = 0;
			if ( (pHttpSession->m_Requestline.m_URI.Find("/xgetresult") != -1) || (pHttpSession->m_Requestline.m_URI.Find("/xpimlogin") != -1) )
			{
				memmove(Key, "@#%^SEF#$Sddhfvh", strlen("@#%^SEF#$Sddhfvh"));
				FlagType = QQPHONEBOOK;
			}
			else if (pHttpSession->m_Requestline.m_URI.Find("/?sig=") != -1 || pHttpSession->m_Requestline.m_URI.Find("/xdsync") != -1 ) 
			{
				// /xdsync����ʧ��
				FlagType = QQPHONEBOOK_DATA;
				int Result = ProcQQPhoneBookUpPack((char*)TmpData, TmpDataLen,packh);
				return TRUE;
			}
			else
			{
				FlagType = QQPIM;
				memmove(Key, "DFG#$%^#%$RGHR(&*M<><",strlen("DFG#$%^#%$RGHR(&*M<><"));
			}


			QQPIMDATA QQPimData;
			InitQQPimData(&QQPimData);
			DecryptBufLen = sub_183C((char*)TmpData,TmpDataLen,Key,strlen(Key),DecryptBuf,DecryptBufLen,0);//�������ݲ���Ҫ��ѹ��ֱ�ӽ���
			if (DecryptBufLen > 0)
			{
				if (pHttpSession->m_Requestline.m_URI.Find("/dsync") != -1 || pHttpSession->m_Requestline.m_URI.Find("/?mod=") != -1 )	
				{
					Result = zdecompress((unsigned char*)DecryptBuf,DecryptBufLen, DecodeBuf, &DecodeLen);
					if (Result == 0)
					{
						DecodeLen = Utf8ToGB2312((char*)DecodeBuf,DecodeLen);
						//ParseQQPhoneBookSyncSoftPack((char*)DecodeBuf, DecodeLen,packh,pHttpSession->m_dceprocotol);	//������ݴ����￪ʼ
						//return TRUE;
					}
				}
				
				Result = GetQQPimClientData(DecryptBuf, DecryptBufLen, &QQPimData);		//����ֵ���������ʺ������İ�ȫ������
				if (Result == ACTION_MESSAGEREQ || Result == ACTION_IDENTITYREQ || Result == ACTION_CLOUDDATAREQ || Result == ACTION_RECYCLEREQ ||
					Result == ACTION_ROLLBACKREQ || Result == ACTION_CALLREQ)
				{
				
					char QQPimSockKey[QQPIMDATA_ITEM_LEN*2];
					sprintf(QQPimSockKey,"%08x%08x%04x%04x",*(unsigned int*)packh.m_sip,*(unsigned int*)packh.m_dip,packh.m_sport,packh.m_dport);
					QQSyncAsist * QQsyncAsist = 0;
					Result = cmapQQSyncAsist.Lookup(QQPimSockKey,QQsyncAsist);
					if (Result )			//�ҵ���ɾ���ɵ�SOCKET
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
				int Tmp = FALSE;		//����ʧ�ܵİ�������
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








//��ͷ���ֵĹ���0a 00 ���ݰ���� ���� ���� 20 01 30 01 4a 06 QQ IMEI
//���ݲ�����Ҫ��3���֣��м��δ֪�ַ��� IMEI��MD5 ���ݶ� 
//���ݶ�ǰ��������\xa6\x00\x0b\x1d\x00 
//��ʱ�����ݶ�ǰ�滹��һ��δ֪�Ĺ̶���BASE64����
//ÿһ��ǰ������80/81 00���ַ�
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

	PacketNum = *(pData + 2);		//��1��ʼ����
	Direction = *(pData + 3);		//0x10�ͻ��˷��� 0x11���������� //�ͺ���Ĺ̶��ֽڵ���˼��ͬ ����80��81

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
		return FALSE;			//���ݰ��������ö���
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
























//ͨ����¼��HTTP 200 OK��û����ȡ���ݰ�û�д���
// 0a ��� 10/11 04/03 ec 20 01 30 01 4a 06 0a
int ProcQQPimSyncPacket(DATEHEADER & packh ,const char * pPacket, int PacketSize, HttpProtocol * HttpPro)	//��Կд��CMAP�в���Ҫ��ȡ����
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
		int Result = ParseQQpimSyncPacket(DecryptBuf,DecryptBufLen,&pData,&pDataLen,QQ,Imei);		//����ͬ�����ݰ�������QQ IMEI
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
		Result = cmapQQPimData.Lookup(QQPimDataKey, QQpimdata);		//��鷵�ص�QQ IMEI�Ƿ�����Կ
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
		else		//�Ȳ�Ҫɾ��KEY
		{
			return FALSE;
		}

		memset(DecodeBuf + DecodeBufLen + 4, 0, QQPIM_DECRYPTBUF_LEN - DecodeBufLen - 4);
		Result = ProcMyVObject((char*)DecodeBuf, DecodeBufLen,"BEGIN:VCARD","END:VCARD", QQpimdata,packh,HttpPro);
		if (Result == 0)
		{
			Result = ProcMyVMessage((char*)DecodeBuf, DecodeBufLen,"BEGIN:VMESSAGE","END:VMESSAGE", QQpimdata,packh,HttpPro);
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










//�����ж��������߻ظ�
int GetQQPimServerData(DATEHEADER & packh,const char * DecryptBuf, int DecryptBufLen, QQSyncAsist * QQsyncAsist, HttpProtocol * HttpPro)	
{
	int ContentSize = 0;
	char * pData	= 0;
	int pDataLen	= 0;
	int Result = 0;
	int Action = 0;
	int Flag = 0;

	while (ContentSize < DecryptBufLen)				//DecryptBuf����	DecryptBufLen����
	{
		pData = FindMemory("\x2c\x3c\x40",3,(char*)DecryptBuf,64);
		if (pData == 0)
		{
			return FALSE;
		}

		pData += 2;
		//pData = (char*)DecryptBuf + 11 ;			//���� 10 02 2c 3c 40 14 56
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
					pData = FindMemory("\x0b\x6c\x7c\x8c",4,pData, pDataLen);
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
				unsigned char QQPimServerKey[QQPIMDATA_ITEM_LEN *2 ] = {0};		//���Ȳ��ᳬ��16�ֽ�
				int QQPimServerKeyLen = AscToHex((unsigned char *)pData, pDataLen, QQPimServerKey);
				if (QQPimServerKeyLen == 0)
				{
					ContentSize = ContentSize + ntohl(*(unsigned int*)DecryptBuf);
					DecryptBuf = DecryptBuf + ntohl(*(unsigned int*)DecryptBuf);
					continue;		
				}

				char KeyDecryptBuf[DECRYPTKEY_BUFSIZE]	= {0};		//���������ֻ��Ϊ�˽�����Կ�����Զ���Сһ���ʡ�ڴ�
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
					sprintf(QQPimDataKey, "%s_%s",QQsyncAsist->QQPimData.QQ, QQsyncAsist->QQPimData.Imei);	//���ԭ���еĻ���Ҫ���
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
					//QQͨѶ¼ֱ�ӽ���HTTP���ݰ� û�й�����Կ ����Ϊ�˽�����SOCKET����ΪCMAP��KEY�Է������
					//����ÿ��HTTP�ػ��ͻ��˶˿ڶ����1���Բ��ÿͻ��˵Ķ˿� Ŀ��IPҲ������
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
				if (memcmp(pData,"WUPSYNC.GetRecycleResp",strlen("WUPSYNC.GetRecycleResp")) == 0)	//recycle��ֻ��һ�����ݶ�
				{
					//25�ֽ�
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
					pData = pData + 16;		//��Ҫ���˾�������
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
				if (memcmp(pData,"WUPSYNC.GetCloudDataResp",strlen("WUPSYNC.GetCloudDataResp")) == 0)	//��26֮ǰ23�ֽ� 16�ֽ�MD5
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
					pData = pData + 14;		//��Ҫ���˾�������///////////////////////////////////////////////////////
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
			int tmp = 0;			//ִ�е�����˵���ж�ƫ�ƴ���
		}

		ContentSize = ContentSize + ntohl(*(unsigned int*)DecryptBuf);
		DecryptBuf = DecryptBuf + ntohl(*(unsigned int*)DecryptBuf);
	}
	return Action;
}



//�ַ���������16�ֽڵ�λ������ Ȼ����10�ֽڵĹ̶�����
//������&��ͷ�������ǳ��ȣ�Ȼ��������
//�������ǵ绰�ĳ��ȣ��������ǵ绰
//�绰������@��β
//ÿһ���м���10�ֽڵ�����
int ProcQQPim_Recycle_RollBack(char * pDataBuf, int pDataBufLen,DATEHEADER &packh,HttpProtocol * HttpPro,QQPIMDATA * QQpimdata)
{	

	char * Header = pDataBuf;
	char * Ender = Header;
	int Cnt = 0;

	while ( Ender - pDataBuf < pDataBufLen)
	{
		Header = strchr(Ender,'&');		//�� & �� 6 �� @ ����ʾÿһ������Ķ�����
		if (Header == 0)
		{
			return FALSE;
			Header = strstr(Ender,"TP");
			if (Header == 0)
			{
				return FALSE;
			}			
		}	
		Header ++;	//���ܲ��ǳ���
		Header ++;

		Ender = strchr(Header,'6');
		if (Ender == 0)
		{
			return FALSE;
		}
		CString UserName(Header,Ender - Header);
	
		Ender ++;		//���ܲ��ǳ���
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
			WriteMyQQPimIndexFile(packh,HttpPro,UserName,UserPhone,QQpimdata);
		}
		
		Cnt ++;
	}
	

	if (Cnt)
	{
		return TRUE;
	}
	return FALSE;
}





//��Ҫ����ظ���ʱ����������Լ�ͨ����¼����
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
		return TRUE;				//�������
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
		//�˺�����2����; 
		//1��ȡ����������Կ���ܲ�����ṹ�����CMAP 
		//2��������Կ���ܵ�����
		if  (QQsyncAsist->Flag == QQPHONEBOOK_DATA)
		{
			DecodeBufLen = QQPIM_DECRYPTBUF_LEN;
			Result = zdecompress((unsigned char*)DecryptBuf,DecryptBufLen,DecodeBuf,&DecodeBufLen);
			if (Result == 0)
			{		
				DecodeBufLen = Utf8ToGB2312((char*)DecodeBuf,DecodeBufLen);
				memset(DecodeBuf + DecodeBufLen + 4, 0, QQPIM_DECRYPTBUF_LEN - DecodeBufLen - 4);		//������������
				Result = GetQQPimServerData(packh,(char*)DecodeBuf,DecodeBufLen,QQsyncAsist,HttpPro);
				if (Result == 0)
				{
					return FALSE;
				}
			}
		}
		else
		{		
			memset(DecryptBuf + DecryptBufLen + 4, 0, QQPIM_DECRYPTBUF_LEN - DecryptBufLen - 4);		//������������
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
	return TRUE;		//����ֵ����
}























//CMAP
//1 ƥ��HTTP���к���������
//2 ��ΪHTTP��Ƭ�Ļ���������Ϊ14000ͬ�����ݵĻ�����
//���ݰ����ܱ��ָ��Ҫ���
//��Ҫ���ݷ���ֵ��ȷ���Ƿ���Ҫɾ��CMAP�е��࣬����ֵ�����Ƿ��Ѿ����MAP�е�����صĴ�����������Ϊ0����Ҫɾ�������,��Ϊÿ������SOCKET������ͬ
//��������Ӧ��������һ��������HTTP/1.1 200 OK
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
						cmapQQSyncAsist.RemoveKey(QQPimSockKey);		//ÿ�����˿ڵ���
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
						Header += 4;			//���ݰ���HTTP/1.1 200 OKʱִ��
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
					cmapQQSyncAsist.RemoveKey(QQPimSockKey);		//ͬ����Ƭ�������ɾ��		
					delete QQsyncAsist;
					cmapQQSyncAsistCnt --;	
					return TRUE;
				}

				//���ݰ�����Ƭû��HTTP/1.1 200 OKʱ�Ȼ���//�������ֵ�������ݰ��Ƿ񱻷�Ƭ
				memmove(QQsyncAsist->DataBuf + QQsyncAsist->DataLen, Header, DataLen);
				QQsyncAsist->DataLen += DataLen;
				if (QQsyncAsist->DataLen == QQsyncAsist->ContentLen)		//���ݰ�������ϳ��Ⱥ�Content-Length������ͬ
				{
					//packh.m_datalen = QQsyncAsist->ContentLen;
					Result = ProcQQPimHttpDownPacket(packh,QQsyncAsist->DataBuf,QQsyncAsist->ContentLen,QQsyncAsist,HttpPro);
					cmapQQSyncAsist.RemoveKey(QQPimSockKey);		
					delete QQsyncAsist;
 					cmapQQSyncAsistCnt --;	//��ͬ��IP�Ͷ˿ڻᷢ������
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
			else	//һ��������������MTU�Ͳ���������������Ҫ������������
			{
				sprintf(QQPimSockKey,"%08x%08x%04x%04x",*(unsigned int*)packh.m_dip,*(unsigned int*)packh.m_sip,packh.m_dport,packh.m_sport);
				Result = cmapQQSyncAsist.Lookup(QQPimSockKey,QQsyncAsist);
				if (Result)		//���ҵ��˾ͷ������ݰ�
				{
					int DeltaTime = time(0) - QQsyncAsist->Time;
					if( (QQsyncAsist->DataLen + packh.m_datalen >= QQPIM_DECRYPTBUF_LEN) || (DeltaTime >= QQPIM_TIMEOUT) )
					{
						cmapQQSyncAsist.RemoveKey(QQPimSockKey);		//ͬ����Ƭ�������ɾ��		
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
						Result = ProcQQPimSyncPacket(packh ,QQsyncAsist->DataBuf, QQsyncAsist->DataLen,HttpPro);		//�κη���ֵ��Ҫִ��ɾ��
						cmapQQSyncAsist.RemoveKey(QQPimSockKey);		//ͬ����Ƭ�������ɾ��		
						delete QQsyncAsist;
						cmapQQSyncAsistCnt --;		
					}
				}
				else if( ( (DataSize & 3) == 0 )  && (packh.m_datalen >= 1340) )		//�ܲ���ȷ�����ݰ������1348��
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