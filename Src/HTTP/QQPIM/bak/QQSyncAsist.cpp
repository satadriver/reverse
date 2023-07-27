#include "stdafx.h"
#include "QQSyncAsist.h"


void InitQQPimData(LPQQPIMDATA QQPimData)
{
	memset(QQPimData->QQ,0,QQPIMDATA_ITEM_LENTH);
	QQPimData->QQLen		= 0;
	memset(QQPimData->Imei,0,QQPIMDATA_ITEM_LENTH);
	QQPimData->ImeiLen		= 0;
	memset(QQPimData->Imsi,0,QQPIMDATA_ITEM_LENTH);
	QQPimData->ImsiLen		= 0;
	memset(QQPimData->QQPimKey,0,QQPIMDATA_ITEM_LENTH);	
	QQPimData->QQPimKeyLen	= 0;
}


QQSyncAsist::QQSyncAsist()
{
	InitQQPimData(&(this->QQPimData));
	memset(this->DataBuf,0,QQSYNCASIST_DATABUF_LENTH);
	this->DataLen		= 0;
	this->ContentLen	= 0;
	this->Time			= time(0);
}


QQSyncAsist::~QQSyncAsist()
{

}





CMap< CString,LPCSTR,QQPIMDATA*,QQPIMDATA*& >cmapQQPimData;
int cmapQQPimDataCnt = 0;
CMap< CString,LPCSTR,QQSyncAsist*,QQSyncAsist*& >cmapQQSyncAsist;
int cmapQQSyncAsistCnt = 0;

int ProcQQPhoneBookVobject(char * Buf, int BufLen, char * DestBuf, int * DestLen, char * HeaderFlag, char * EnderFlag, int Offset)
{
	if( (BufLen < 32) || (*DestLen < BufLen) )
	{
		return FALSE;
	}
	
	int DataLen = 4;		//��4���ֽڵ�����Ŀ���� //4�ֽ��ǵ�һ��ĳ���?
	char * Header = Buf;
	char * Ender = Buf;
	int HeaderFlagLen = strlen(HeaderFlag);
	int EnderFlagLen = strlen(EnderFlag);

	char * TmpEnder;
	char * TmpHeader;
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
		
		Ender = FindMemory(EnderFlag,EnderFlagLen,Header,Offset);
		if (Ender == 0)
		{
			break;
		}
		
		if( (Header - Buf) >= BufLen || (Ender - Buf) > BufLen  || (Ender - Header) > Offset )
		{
			break;
		}
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


		TmpHeader = strstr(Tmp,"CELL:");
		if (TmpHeader == 0)
		{
			continue;
		}

		TmpHeader += strlen("CELL:");
		TmpEnder = strstr(TmpHeader,"\r\n");
		if (TmpEnder == 0)
		{
			continue;
		}

		TmpEnder += strlen("\r\n");
		if (TmpEnder - TmpHeader >= Offset)
		{
			continue;
		}
		
		char Phone[256] = {0};
		int PhoneLen = TmpEnder - TmpHeader;
		memmove(Phone, TmpHeader, TmpEnder - TmpHeader);

		TmpHeader = strstr(TmpEnder,"FN:");
		if (TmpHeader == 0)
		{
			continue;
		}
		
		TmpHeader += strlen("FN:");
		TmpEnder = strstr(TmpHeader,"\r\n");
		if (TmpEnder == 0)
		{
			continue;
		}

		if (TmpEnder - TmpHeader >= Offset)
		{
			continue;
		}
		
		memmove(DestBuf + DataLen, TmpHeader, TmpEnder - TmpHeader);
		DataLen += (TmpEnder - TmpHeader);	
		TmpEnder += strlen("\r\n");


		*(DestBuf + DataLen) = '-';
		DataLen ++;

		memmove(DestBuf + DataLen, Phone, PhoneLen);
		DataLen += (PhoneLen);	


		Count ++;
	}

	*(unsigned int*)DestBuf = Count;
	
	if (DataLen <= 4)
	{
		return FALSE;
	}

	if (memcmp(DestBuf,"BEGIN:VMESSAGE",strlen("BEGIN:VMESSAGE")) == 0)
	{
		Count = 0;
		int MessageBufLen = 8092;
		char MessageBuf[8192] = {0};		//������󳤶�70�ֽ�

		Header = DestBuf;
		Ender = Header;
		while (Header - DestBuf < DataLen)
		{
			Header = strstr(Ender,"ENCODING=BASE64:");
			if(Header == 0)
			{
				break;
			}

			Header += strlen("ENCODING=BASE64:");
			Ender = strstr(Header,"\r\n\r\n");
			if (Ender == 0)
			{
				break;
			}

			MessageBufLen = Ender - Header;
			memmove(MessageBuf, Header, MessageBufLen);
			memset(Header,0x20,MessageBufLen);
			
			MessageBufLen = DecodeBase64(MessageBuf, MessageBufLen);
			if (MessageBufLen )
			{
				MessageBufLen = Utf8ToGB2312(MessageBuf,MessageBufLen);
				memmove(Header,MessageBuf, MessageBufLen );
			}
			Count ++;
		}
	}

	if (DataLen > 4 )
	{
		* DestLen = DataLen;
		*(unsigned int*)(DestBuf + DataLen) = 0;
		return TRUE;
	}
	
	return FALSE;
}


//���ܺ������ؽ��ܳ��ȣ����ں���4�ֽڣ���ͷ���ֽ��ǵ�һ���ֳ���
//getRecycle				rollbackVersion					//1d 00 00 73 0a 0a 00 01 16
//WUPSYNC.GetRecycleReq		WUPSYNC.RollBackVersionReq		//1d 00 00/01 73/99 0a 0a 00 01 16
int GetQQPimClientData(const char * DecryptBuf, int DecryptBufLen, LPQQPIMDATA QQPimData)
{
	int ContentSize = 0;
	char * pData = 0;		//���ֽڳ��� + 10 02 2c 3c 40 01/15 56
	int pDataLen = 0;

	while (ContentSize < DecryptBufLen)
	{
		pData = (char*)DecryptBuf + 11 ;		//���ֽ�ͷ������ 10 02 2c 3c 40 01 56 �˴����ȹ̶�
		pDataLen = * pData;
		pData = pData + 1;
		if ( (memcmp(pData ,"wupsync",strlen("wupsync")) == 0) || ( memcmp(pData ,"syncm",strlen("syncm")) == 0 ) ||
			 (memcmp(pData ,"report",strlen("report")) == 0 ) )
		{
			pData = pData + pDataLen;
			pData = pData + 1;						//һ���ֽڵ�66������HTTP������

			pDataLen = *pData;
			pData = pData + 1;
			if(memcmp(pData,"GetUserIdentity",strlen("GetUserIdentity")) == 0) 		//��ȡ�ͻ�����Կ	
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
				if (memcmp(pData ,"WUPSYNC.GetUserIdentityReq",strlen("WUPSYNC.GetUserIdentityReq")) == 0) 
				{
					pData = pData + pDataLen;
					pData = pData + 10;				//1d 00 01 00 a3/a5 0a 0a 00 01 16

					pDataLen = * pData;
					pData = pData + 1;
					memmove(QQPimData->QQ, pData, pDataLen);
					QQPimData->QQLen = pDataLen;
					pData = pData + pDataLen + 1;		//&

					//�м���һ���ַ�����һ��MD5���� 0b 1a 06 14
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

					if (*pData == '&')
					{
						pData = pData + 1;			//&
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
						pData ++;
					}
					
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

	return FALSE;		//����ֵ����
}






//�ͻ�����Կ�ͷ���������Կ��һ�԰�
//���û���ҵ���Ӷ���ָ�뵽CMAPͬʱ��ȡ�ͻ�����Կ
int ProcQQPimHttpUpPacket(HttpSession * pHttpSession,DATEHEADER & packh,const char * pPacket)
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

			char * TmpData = strstr(pPacket,"\r\n\r\n");
			if (TmpData == 0)
			{
				return TRUE;
			}
			TmpData += 4;
			int TmpDataLen = packh.m_datalen - (TmpData - pPacket);
			char DecryptBuf[QQPIM_DECRYPTBUF_LENTH];
			int DecryptBufLen = QQPIM_DECRYPTBUF_LENTH;
			DecryptBufLen = sub_183C((char*)TmpData,TmpDataLen,"DFG#$%^#%$RGHR(&*M<><",21,DecryptBuf,DecryptBufLen,0);//�������ݲ���Ҫ��ѹ��ֱ�ӽ���
			if (DecryptBufLen > 0)
			{
				QQPIMDATA QQPimData;
				InitQQPimData(&QQPimData);
				int Result = GetQQPimClientData(DecryptBuf, DecryptBufLen, &QQPimData);		//����ֵ���������ʺ������İ�ȫ������

				if (Result)
				{
					char QQPimSockKey[QQPIMDATA_ITEM_LENTH];
					sprintf(QQPimSockKey,"%08x%08x%04x%04x",*(unsigned int*)packh.m_sip,*(unsigned int*)packh.m_dip,packh.m_sport,packh.m_dport);
					QQSyncAsist * QQsyncAsist = 0;
					Result = cmapQQSyncAsist.Lookup(QQPimSockKey,QQsyncAsist);
					if (Result == 0)
					{
						QQsyncAsist = new QQSyncAsist;
						memmove((char*)&QQsyncAsist->QQPimData, (char*)&QQPimData, sizeof(QQPIMDATA));
						cmapQQSyncAsist.SetAt(QQPimSockKey,QQsyncAsist);
						cmapQQSyncAsistCnt ++;
					}
					
					return TRUE;
				}
				
			}
		}
		
		return FALSE;

	}
	catch (...)
	{
		WriteLog(HTTPPRO, "ProcessQQPIM Error!!");				
	}
	return TRUE;
}





char * FindMemory(char Flag[], int FlagLen, char Address[], int Offset)
{
	for (int Cnt = 0; Cnt < Offset; Cnt ++)
	{
		if (memcmp( Flag,Address + Cnt, FlagLen ) == 0)
		{
			return Address + Cnt + FlagLen;
		}
	}
	return FALSE;
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

	if (*pData != 0x0a)		
	{
		return FALSE;
	}

	PacketNum = *(pData + 2);		//��1��ʼ����
	Direction = *(pData + 3);		//0x10�ͻ��˷��� 0x11���������� //�ͺ���Ĺ̶��ֽڵ���˼��ͬ ����80��81

	pData = FindMemory("\x20\x01\x30\x01\x4a\x06",6,(char*)pData,32);
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








//�����1�ֽ� 0a 0a 00 01 16 01 36 2c 36 00 4c 0b 1d 00 01 �鳤��2�ֽ� 0D 00 01 ��Ϣ����2�ֽ� ��Ϣ 
int ProcVMessage(char * Buf, int BufLen, char * DestBuf, int * DestLen)
{
	if( (BufLen <= 128) || (*DestLen < BufLen) )
	{
		return FALSE;
	}
	
	int DataLen = 0;
	char * pData = Buf + 4;		//00 01 19 00
	int pDataLen = 0;
	char * Header = 0;
	char * Ender = 0;
	char DecodeBuf[0x2000];
	int DecodeLen = 0;
	int Cnt = 0;

	while ( pData - Buf < BufLen)
	{
		pData = pData + 21;	
		pDataLen = ntohs(*(unsigned short*)pData);
		pData += 2;
		if (memcmp(pData,"BEGIN:VMESSAGE",strlen("BEGIN:VMESSAGE")) )
		{
			break;
		}

		Header = strstr(pData,"ENCODING=BASE64:");
		if (Header)
		{
			Header += strlen("ENCODING=BASE64:");
			Ender = strstr(Header,"\r\n\r\n");
			if (Ender)
			{
				DecodeLen = Ender - Header;
				memmove(DecodeBuf, Header, DecodeLen);
				DecodeLen = DecodeBase64(DecodeBuf, DecodeLen);
				DecodeLen = Utf8ToGB2312(DecodeBuf, DecodeLen);
				memmove(DestBuf + DataLen, pData, Header - pData);
				DataLen += (Header - pData);

				memmove(DestBuf + DataLen, DecodeBuf, DecodeLen);
				DataLen += DecodeLen;

				Ender  += strlen("\r\n\r\nEND:VMESSAGE\r\n");
				memmove(DestBuf + DataLen, "\r\n\r\nEND:VMESSAGE\r\n", strlen("\r\n\r\nEND:VMESSAGE\r\n") );
				DataLen += strlen("\r\n\r\nEND:VMESSAGE\r\n");

				pData = Ender;
				Cnt ++;
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	}


	if (DataLen )
	{
		* DestLen = DataLen;
		*(unsigned int*)(DestBuf + DataLen) = 0;
		return TRUE;
	}
	
	return FALSE;
}









int ProcVObject(char * Buf, int BufLen, char * DestBuf, int * DestLen, char * HeaderFlag, char * EnderFlag, int Offset)
{
	if( (BufLen < 32) || (*DestLen < BufLen) )
	{
		return FALSE;
	}
	
	int DataLen = 4;		//��4���ֽڵ�����Ŀ���� //4�ֽ��ǵ�һ��ĳ���?
	char * Header = Buf;
	char * Ender = Buf;
	int HeaderFlagLen = strlen(HeaderFlag);
	int EnderFlagLen = strlen(EnderFlag);

	char * TmpEnder;
	char * TmpHeader;
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
		
		Ender = FindMemory(EnderFlag,EnderFlagLen,Header,Offset);
		if (Ender == 0)
		{
			break;
		}
		
		if( (Header - Buf) > BufLen || (Ender - Buf) > BufLen  || (Ender - Header) > Offset )
		{
			break;
		}
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


		TmpHeader = strstr(Tmp,"CHARSET=UTF-8:");
		if (TmpHeader == 0)
		{
			continue;
		}

		TmpHeader += strlen("CHARSET=UTF-8:");
		TmpEnder = strstr(TmpHeader,"\r\n");
		if (TmpEnder == 0)
		{
			continue;
		}

		if (TmpEnder - TmpHeader >= Offset)
		{
			continue;
		}
		
		memmove(DestBuf + DataLen, TmpHeader, TmpEnder - TmpHeader);
		DataLen += (TmpEnder - TmpHeader);	

		*(DestBuf + DataLen) = '-';
		DataLen ++;

		TmpEnder += strlen("\r\n");
		TmpHeader = strstr(TmpEnder,"CELL:");
		if (TmpHeader == 0)
		{
			continue;
		}
		
		TmpHeader += strlen("CELL:");
		TmpEnder = strstr(TmpHeader,"\r\n");
		if (TmpEnder == 0)
		{
			continue;
		}

		TmpEnder += 2;
		if (TmpEnder - TmpHeader >= Offset)
		{
			continue;
		}
		
		memmove(DestBuf + DataLen, TmpHeader, TmpEnder - TmpHeader);
		DataLen += (TmpEnder - TmpHeader);	

		Count ++;
	}

	*(unsigned int*)DestBuf = Count;
	
	if (DataLen <= 4)
	{
		return FALSE;
	}

	if (memcmp(DestBuf,"BEGIN:VMESSAGE",strlen("BEGIN:VMESSAGE")) == 0)
	{
		Count = 0;
		int MessageBufLen = 8092;
		char MessageBuf[8192] = {0};		//������󳤶�70�ֽ�

		Header = DestBuf;
		Ender = Header;
		while (Header - DestBuf < DataLen)
		{
			Header = strstr(Ender,"ENCODING=BASE64:");
			if(Header == 0)
			{
				break;
			}

			Header += strlen("ENCODING=BASE64:");
			Ender = strstr(Header,"\r\n\r\n");
			if (Ender == 0)
			{
				break;
			}

			MessageBufLen = Ender - Header;
			memmove(MessageBuf, Header, MessageBufLen);
			memset(Header,0x20,MessageBufLen);
			
			MessageBufLen = DecodeBase64(MessageBuf, MessageBufLen);
			if (MessageBufLen )
			{
				MessageBufLen = Utf8ToGB2312(MessageBuf,MessageBufLen);
				memmove(Header,MessageBuf, MessageBufLen );
			}
			Count ++;
		}
	}

	if (DataLen > 4 )
	{
		* DestLen = DataLen;
		*(unsigned int*)(DestBuf + DataLen) = 0;
		return TRUE;
	}
	
	return FALSE;
}








#define VCARD_MAX_SIZE 256
#define VMESSAGE_MAX_SIZE 2048
#define VCALL_MAX_SIZE 128


//ͨ����¼��HTTP 200 OK��û����ȡ���ݰ�û�д���
// 0a ��� 10/11 04/03 ec 20 01 30 01 4a 06 0a
int ProcQQPimSyncPacket(DATEHEADER & packh ,const char * pPacket, int PacketSize, HttpProtocol * HttpPro)	//��Կд���ļ��в���Ҫ��ȡ����
{
	char * pData = (char*)(pPacket + 4);
	unsigned int pDataLen = PacketSize - 4;

	char DecryptBuf[QQPIM_DECRYPTBUF_LENTH];
	int DecryptBufLen = QQPIM_DECRYPTBUF_LENTH;
	unsigned char DecodeBuf[QQPIM_DECRYPTBUF_LENTH];
	unsigned long DecodeBufLen = QQPIM_DECRYPTBUF_LENTH;

	DecryptBufLen = sub_183C((char*)pData,pDataLen,"DFG#$%^#%$RGHR(&*M<><",21,DecryptBuf,DecryptBufLen,0);
	if (DecryptBufLen > 0)
	{
		char QQ[QQPIMDATA_ITEM_LENTH] = {0};
		char Imei[QQPIMDATA_ITEM_LENTH] = {0};

		pDataLen = 0;
		pData = 0;
		int Result = ParseQQpimSyncPacket(DecryptBuf,DecryptBufLen,&pData,&pDataLen,QQ,Imei);		//����ͬ�����ݰ�������QQ IMEI
		if(Result == 0)
		{
			return FALSE;
		}

		if ( (pData - DecryptBuf >= DecryptBufLen ) || (pDataLen >= QQPIM_DECRYPTBUF_LENTH ) )
		{
			return FALSE;
		}

		QQPIMDATA * QQpimdata = 0;
		char QQPimDataKey[256];
		sprintf(QQPimDataKey, "%s_%s",QQ, Imei);
		Result = cmapQQPimData.Lookup(QQPimDataKey, QQpimdata);		//��鷵�ص�QQ IMEI�Ƿ�����Կ
		if (Result == 0)
		{
			return FALSE;
		}
		
		DecryptBufLen = QQPIM_DECRYPTBUF_LENTH;
		DecryptBufLen = sub_183C((char*)pData,pDataLen,(char*)QQpimdata->QQPimKey,QQpimdata->QQPimKeyLen,DecryptBuf,DecryptBufLen,0);
		if (DecryptBufLen > 0)
		{
			DecodeBufLen = QQPIM_DECRYPTBUF_LENTH;
			zdecompress((unsigned char *)DecryptBuf,DecryptBufLen, DecodeBuf,&DecodeBufLen);
			if (DecodeBufLen > 0)
			{
				Utf8ToGB2312((char*)DecodeBuf,DecodeBufLen);
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

		char  DataBuf[QQPIM_DECRYPTBUF_LENTH];
		int DataBufLen = QQPIM_DECRYPTBUF_LENTH;

	
		Result = ProcVObject((char*)DecodeBuf,DecodeBufLen,DataBuf,&DataBufLen,"BEGIN:VCARD","END:VCARD",VCARD_MAX_SIZE);
		if (Result)
		{
			WriteQQPimIndexFile(packh, HttpPro, DataBuf,QQpimdata);
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










void WriteQQPimIndexFile(DATEHEADER & stPackHeader,HttpProtocol * pHttpPro, char * Data,QQPIMDATA * QQpimdata)
{
	if (*(unsigned int*)Data >0)
	{
	
		HttpSession httpsession(stPackHeader, g_httpFrom);		
		httpsession.m_dceprocotol=pHttpPro;
		

		char Name[256] = {0};
		char Phone[256] = {0};
		int Cnt = *(unsigned int*)Data;
		char * Header = Data + 4;
		char * Ender = Header;
		CString Txt;
		
		for (int Count = 0; Count < Cnt; Count ++)
		{
			Ender = strchr(Header,'-');
			if (Header ==0)
			{
				Header ++;
				Count --;
				continue;
			}

			memmove(Name, Header, Ender - Header);
			Name[Ender - Header] = 0;
			
			Header = Ender + 1;
			Ender = strstr(Header, "\r\n");
			if (Ender == 0)
			{
				Header ++;
				Count --;
				continue;
			}
			
			memmove(Phone, Header, Ender - Header);
			Phone[Ender - Header] = 0;
			CString strPhone(Phone);
			strPhone.Replace(" ","");
			Header = Ender + 2;

			SYSTEMTIME stNow;
			GetLocalTime(&stNow);
			CString tmppath = httpsession.m_dceprocotol->m_httpMissObj->GetTmpPath();
			ASSERT(!tmppath.IsEmpty());
			httpsession.m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_TXL_QQTBZS.indexfile.txt", 
			tmppath, stNow.wYear, stNow.wMonth, stNow.wDay, stNow.wHour,stNow.wMinute, stNow.wSecond, httpsession.m_sip, httpsession.m_dip, rand());
			Txt.Format("user=%s\r\nimei=%s\r\nimsi=%s\r\nnick=%s\r\ncontact=%s\r\n",QQpimdata->QQ,QQpimdata->Imei,QQpimdata->Imsi,Name,strPhone);
			httpsession.WriteIndexFile("TXL_QQTBZS","TXL_QQTBZS",Txt,httpsession.m_dceprocotol->m_httpMissObj);	
		}
	}
	
	return;
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

//�����ж��������߻ظ�
int GetQQPimServerData(DATEHEADER & packh,const char * DecryptBuf, int DecryptBufLen, QQSyncAsist * QQsyncAsist, HttpProtocol * HttpPro)	
{
	int ContentSize = 0;
	char * pData	= 0;
	int pDataLen	= 0;
	int Result = 0;
	int Action = 0;
	char DecodeBuf[QQPIM_DECRYPTBUF_LENTH];
	int DecodeBufLen = QQPIM_DECRYPTBUF_LENTH;

	while (ContentSize < DecryptBufLen)				//DecryptBuf����	DecryptBufLen����
	{
		pData = (char*)DecryptBuf + 11 ;			//���� 10 02 2c 3c 40 14 56
		pDataLen = * pData;
		pData = pData + 1;
		if ( memcmp(pData ,"wupsync",strlen("wupsync")) == 0 )
		{
			pData = pData + pDataLen;
			pData = pData + 1;
			
			pDataLen = *pData;
			pData = pData + 1;
			if( memcmp(pData,"GetUserIdentity",strlen("GetUserIdentity")) == 0)
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
					unsigned char QQPimServerKey[QQPIMDATA_ITEM_LENTH ] = {0};		//���Ȳ��ᳬ��16�ֽ�ô��
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
						memset(QQsyncAsist->QQPimData.QQPimKey, 0, QQPIMDATA_ITEM_LENTH);
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
					char QQPimDataKey[256];
					sprintf(QQPimDataKey, "%s_%s",QQpimdata->QQ, QQpimdata->Imei);
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
					//1d 00 01 03   
					//69 0a 0c 10   19 22 54 bc   5e e8 39 00   19 0a 00 10   12 54 b6 1b   88
					pData = pData + pDataLen;
					pData = pData + 4;		//1d 00 01 04 b3 0a 0c 10
					pData = pData + 16;
					pDataLen = DecryptBufLen - (pData - DecryptBuf);		
					
					DecodeBufLen = QQPIM_DECRYPTBUF_LENTH;
					Result = ProcQQPim_Recycle_RollBack(pData, pDataLen,DecodeBuf, &DecodeBufLen);
					if (Result)
					{
//						WriteQQPimIndexFile(packh, HttpPro, (char*)DecodeBuf, &QQsyncAsist->QQPimData);
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
				if (memcmp(pData,"WUPSYNC.GetCloudDataResp",strlen("WUPSYNC.GetCloudDataResp")) == 0)	//��26֮ǰ23�ֽ� 16�ֽ�MD5
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
					
					DecodeBufLen = QQPIM_DECRYPTBUF_LENTH;
					Result = ProcQQPim_Recycle_RollBack(pData, pDataLen,DecodeBuf, &DecodeBufLen);
					if (Result)
					{
//						WriteQQPimIndexFile(packh, HttpPro, (char*)DecodeBuf, &QQsyncAsist->QQPimData);
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
//						WriteQQPimIndexFile(packh, HttpPro, (char*)DecodeBuf, &QQsyncAsist->QQPimData);
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



//�ַ���������16�ֽڵ�λ������ Ȼ����10�ֽڵĹ̶�����
//������&��ͷ�������ǳ��ȣ�Ȼ��������
//�������ǵ绰�ĳ��ȣ��������ǵ绰
//�绰������@��β
//ÿһ���м���10�ֽڵ�����
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





//��Ҫ����ظ���ʱ����������Լ�ͨ����¼����
int	ProcQQPimHttpDownPacket(DATEHEADER & packh,const char * pPacket, int PacketSize,QQSyncAsist * QQsyncAsist, HttpProtocol * HttpPro)
{
	char * Data = (char*)pPacket;
	int DataLen = PacketSize;

	char DecryptBuf[QQPIM_DECRYPTBUF_LENTH];
	int DecryptBufLen = QQPIM_DECRYPTBUF_LENTH;
	unsigned char DecodeBuf[QQPIM_DECRYPTBUF_LENTH];
	unsigned long  DecodeBufLen = QQPIM_DECRYPTBUF_LENTH;

	if (memcmp(Data,"\x1f\x8b\x08\x00\x00\x00\x00\x00",8) == 0)			//GzipHdr += 15;GzipLen -= 23;
	{
		Mygzdecompress((unsigned char*)Data + 10 ,DataLen - 10, DecodeBuf ,&DecodeBufLen);
		if (DecodeBufLen > 0)
		{
			DecryptBufLen = sub_183C((char*)DecodeBuf,(int)DecodeBufLen,"DFG#$%^#%$RGHR(&*M<><",21,DecryptBuf,DecryptBufLen,0);
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		DecryptBufLen = sub_183C((char*)Data,DataLen,"DFG#$%^#%$RGHR(&*M<><",21,DecryptBuf,DecryptBufLen,0);
	}

	if (DecryptBufLen > 0)
	{
		//�˺�����2����; 1��ȡ����������Կ���ܲ�����ṹ�����CMAP 2��������Կ���ܵ�����
		int Result = GetQQPimServerData(packh,DecryptBuf,DecryptBufLen,QQsyncAsist,HttpPro);
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
		char QQPimSockKey[QQPIMDATA_ITEM_LENTH];
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
					char ContentLen[QQPIMDATA_ITEM_LENTH] = {0};
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
					if( (QQsyncAsist->ContentLen <= 0) || (QQsyncAsist->ContentLen >= QQPIM_DECRYPTBUF_LENTH) )
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
				if( (QQsyncAsist->DataLen + packh.m_datalen >= QQPIM_DECRYPTBUF_LENTH) || (DeltaTime >= 180) )
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
			else	//һ��������������MTU�Ͳ���������������Ҫ������������
			{
				sprintf(QQPimSockKey,"%08x%08x%04x%04x",*(unsigned int*)packh.m_dip,*(unsigned int*)packh.m_sip,packh.m_dport,packh.m_sport);
				Result = cmapQQSyncAsist.Lookup(QQPimSockKey,QQsyncAsist);
				if (Result)		//���ҵ��˾ͷ������ݰ�
				{
					int DeltaTime = time(0) - QQsyncAsist->Time;
					if( (QQsyncAsist->DataLen + packh.m_datalen >= QQPIM_DECRYPTBUF_LENTH) || (DeltaTime >= 180) )
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
		WriteLog(HTTPPRO, "ProcessQQPIM Error!!");			
	}
	return TRUE;

}