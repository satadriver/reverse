#include "StdAfx.h"
#include "QQPhoneBook.h"




void WriteQQPhoneBookIndexFile(DATEHEADER & stPackHeader,HttpProtocol * pHttpPro, char * Data,QQPIMDATA * QQpimdata)
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
			if (Ender ==0)
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
			Header = Ender + 2;
			
			SYSTEMTIME stNow;
			GetLocalTime(&stNow);
			CString tmppath = httpsession.m_dceprocotol->m_httpMissObj->GetTmpPath();
			ASSERT(!tmppath.IsEmpty());
			httpsession.m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_TXL_QQTXL.indexfile.txt", 
				tmppath, stNow.wYear, stNow.wMonth, stNow.wDay, stNow.wHour,stNow.wMinute, stNow.wSecond, httpsession.m_sip, httpsession.m_dip, rand());
			Txt.Format("user=%s\r\nimei=%s\r\nimsi=%s\r\nnick=%s\r\ncontact=%s\r\n",QQpimdata->QQ,QQpimdata->Imei,QQpimdata->Imsi,Name,Phone);
			httpsession.WriteIndexFile("TXL_QQTXL","TXL_QQTXL",Txt,httpsession.m_dceprocotol->m_httpMissObj);	
		}
	}
	
	return;
}










// int KeepQQPhoneBookBigUpPacket(HttpSession * pHttpSession, char * Data)
// {
// 	if( (pHttpSession->m_Requestline.m_Host.Find("3g.qq.com") != -1) && (pHttpSession->m_Requestline.m_useragent.Find("qqppim") != -1) &&
// 		(pHttpSession->m_Requestline.m_URI.Find("/?sig=") != -1) )
// 	{
// 	
// 		unsigned char	SrcIP[4];
// 		unsigned char	DstIP[4];
// 		unsigned short	SrcPort[4];
// 		unsigned short	DstPort[4];
// 		int Rst = GetIPPort(pHttpSession->m_sip, SrcIP, &SrcPort);
// 		int Rst = GetIPPort(pHttpSession->m_dip, DstIP, &DstPort);
// 
// 		char SocketKey[QQPIMDATA_ITEM_LEN];
// 
// 		sprintf(SocketKey,"%08x%08x%08x%08x",*(unsigned int*)SrcIP, *(unsigned int*)DstIP, SrcPort, DstPort);
// 		HANDLE hFile = CreateFile(SocketKey, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
// 		if (hFile == INVALID_HANDLE_VALUE)
// 		{
// 			return FALSE;
// 		}
// 
// 		long FilePtrHigh = 0;
// 		DWORD FilePtrLow = 0;
// 		FilePtrLow = SetFilePointer(hFile,0, &FilePtrHigh,FILE_END);
// 	// 	if (FilePtrLow == INVALID_SET_FILE_POINTER)
// 	// 	{
// 	// 		return FALSE;
// 	// 	}
// 
// 		DWORD Cnt = 0;
// 		DWORD Result = 0;
// 		Result = WriteFile(hFile, Data, pHttpSession->m_datalen, &Cnt, 0);
// 		if (Result != Cnt)
// 		{
// 			CloseHandle(hFile);
// 			return FALSE;
// 		}
// 
// 		CloseHandle(hFile);
// 		return TRUE;
// 	}
// 	return FALSE;
// }





// int GetQQPhoneBookBigUpPacket(DATEHEADER & packh, char * Data, int * DataSize)
// {
// 	char SocketKey[QQPIMDATA_ITEM_LEN * 2];
// 	sprintf(SocketKey,"%s_%s_qqphonebook.dat",packh.m_dip, packh.m_sip);
// 	HANDLE hFile = CreateFile(SocketKey, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
// 	if (hFile == INVALID_HANDLE_VALUE)
// 	{
// 		return FALSE;
// 	}
// 	
// 	
// 	DWORD FileSizeHigh = 0;
// 	DWORD FileSizeLow = GetFileSize(hFile, &FileSizeHigh);
// 	if( (FileSizeLow == 0) || (FileSizeLow >= * DataSize) || (FileSizeLow == INVALID_FILE_SIZE))
// 	{
// 		CloseHandle(hFile);
// 		DeleteFile(SocketKey);
// 		return FALSE;
// 	}
// 
// 	DWORD Cnt = 0;
// 	DWORD Result = ReadFile(hFile, Data, FileSizeLow, &Cnt, 0);
// 	if (Result != Cnt)
// 	{
// 		CloseHandle(hFile);
// 		DeleteFile(SocketKey);
// 		return FALSE;
// 	}
// 	
// 	CloseHandle(hFile);
// 	DeleteFile(SocketKey);
// 	return TRUE;
// }






int ParseQQPhoneBookUpPack(char * Data, int DataLen, char * DestData, int * DestDataLen)
{
	if (DataLen <= 256)
	{
		return FALSE;
	}
	
	if (memcmp(Data + 16, "SyncML/", strlen("SyncML/")) )
	{
		return FALSE;
	}
	
	int pDataLen = DataLen ;
	char * pData = Data;
	
	pData = FindMemory("text/x-vcard",strlen("text/x-vcard"),pData, DataLen);
	if (pData == 0)
	{
		return FALSE;
	}
	
	pDataLen = DataLen - (pData - Data);
	int Result = ProcVObject(pData,pDataLen,DestData,DestDataLen,"BEGIN:VCARD","END:VCARD",VCARD_MAX_SIZE);
	if (Result == 0)
	{
		Result = ProcQQPhoneBookVobject(pData,pDataLen,DestData,DestDataLen,"BEGIN:VCARD","END:VCARD",VCARD_MAX_SIZE);
		if (Result == 0)
		{
			return FALSE;
		}	
	}
	
	return TRUE;
}





int ProcQQPhoneBookChat(DATEHEADER & packh ,const char * pPacket, int PacketSize, HttpProtocol * HttpPro)
{

	char DecryptBuf[QQPIM_DECRYPTBUF_LEN];
	int DecryptBufLen = QQPIM_DECRYPTBUF_LEN;
	char SocketKey[QQPIMDATA_ITEM_LEN];
	sprintf(SocketKey, "%08x%08x%04x%04x",0,*(unsigned int*)packh.m_dip,0,0);
	QQPIMDATA * QQpimdata = 0;

	int Result = cmapQQPimData.Lookup(SocketKey, QQpimdata);
	if (Result)
	{
		DecryptBufLen = sub_183C((char*)pPacket + 16, PacketSize - 16, (char*)QQpimdata->QQPimKey, QQpimdata->QQPimKeyLen, 
			DecryptBuf, DecryptBufLen,0);
		if (DecryptBufLen > 0)
		{
			int a  = 1;
		}
		return TRUE;
	}
	else
	{
		sprintf(SocketKey, "%08x%08x%04x%04x",*(unsigned int*)packh.m_dip,0,0,0);
		QQpimdata = 0;
		Result = cmapQQPimData.Lookup(SocketKey, QQpimdata);
		if (Result)
		{
			DecryptBufLen = sub_183C((char*)pPacket, PacketSize, (char*)QQpimdata->QQPimKey, QQpimdata->QQPimKeyLen, DecryptBuf, DecryptBufLen,0);
			if (DecryptBufLen > 0)
			{
				int tmp  = TRUE;
			}
			return TRUE;
		}
	}
	return TRUE;
}





//要放在POST多包处
int ProcQQPhoneBookUpPack(DATEHEADER & packh,char * Data, int DataSize)
{

	char SocketKey[QQPIMDATA_ITEM_LEN];
	sprintf(SocketKey,"%08x%08x%04x%04x",*(unsigned int*)packh.m_sip, *(unsigned int*)packh.m_dip,packh.m_sport,packh.m_dport);
	QQSyncAsist * QQsyncasist = 0;
	int Result = cmapQQSyncAsist.Lookup(SocketKey,QQsyncasist);		//如果已经处在相同的删除
	if (Result)
	{
		cmapQQSyncAsist.RemoveKey(SocketKey);
		cmapQQSyncAsistCnt --;
		delete QQsyncasist;
	}

	if (DataSize >= QQPIM_DECRYPTBUF_LEN)		//特别大的包删除
	{
		return FALSE;
	}
	QQsyncasist = new QQSyncAsist(QQPHONEBOOK_DATA);
	memmove(QQsyncasist->UpDataBuf + QQsyncasist->UpDataLen, Data, DataSize );
	QQsyncasist->UpDataLen += DataSize;
	cmapQQSyncAsist.SetAt(SocketKey, QQsyncasist);
	cmapQQSyncAsistCnt ++;

	return TRUE;
}
			



int ParseQQPhoneBookDownPack(char * Data, int DataLen, char * DestData, int * DestDataLen)
{
	if (DataLen <= 256)
	{
		return FALSE;
	}

	if (memcmp(Data + 6, "//SYNCML//DTD SyncML ", strlen("//SYNCML//DTD SyncML ")) )
	{
		return FALSE;
	}

// 	int pDataLen = ntohs(*(unsigned short*)Data);
// 	char * pData = Data + pDataLen;

	int pDataLen = DataLen ;
	char * pData = Data;

	pData = FindMemory("text/x-vcard",strlen("text/x-vcard"),pData, pDataLen);
	if (pData == 0)
	{
		return FALSE;
	}

	pDataLen = DataLen - (pData - Data);
	int Result = ProcVObject(pData,pDataLen,DestData,DestDataLen,"BEGIN:VCARD","END:VCARD",VCARD_MAX_SIZE);
	if (Result == 0)
	{
		Result = ProcQQPhoneBookVobject(pData,pDataLen,DestData,DestDataLen,"BEGIN:VCARD","END:VCARD",VCARD_MAX_SIZE);
		if (Result == 0)
		{
			return FALSE;
		}	
	}

	return TRUE;
}




//QQ通讯录同步数据包没有公共密钥 数据包在解密前不能获得QQ号码和IMEI 所以只能用SOCKET来标示 否则数据包找不到密钥解密失败
int ProcQQPhoneBookDownPack(DATEHEADER & packh, char * Data, int DataSize, QQSyncAsist * QQsyncasist,HttpProtocol * HttpPro)
{
	try
	{	
		char DecryptBuf[QQPIM_DECRYPTBUF_LEN];
		int DecryptBufLen = QQPIM_DECRYPTBUF_LEN;
		unsigned char DecodeBuf[QQPIM_DECRYPTBUF_LEN];
		unsigned long DecodeLen = QQPIM_DECRYPTBUF_LEN;
		
		char SocketKey[QQPIMDATA_ITEM_LEN];
		sprintf(SocketKey,"%08x%08x%04x%04x",*(unsigned int*)packh.m_sip &0x00ffffff, *(unsigned int*)packh.m_dip ,packh.m_sport,0);
		QQPIMDATA * QQpimdata = 0;
		int Result = cmapQQPimData.Lookup(SocketKey, QQpimdata);
		if (Result)
		{
			if (memcmp(Data,"\x1f\x8b\x08\x00\x00\x00\x00\x00",8) == 0)
			{
				Result = Mygzdecompress((unsigned char *)Data + 10,DataSize - 10, DecodeBuf,&DecodeLen);
				if (Result == 0)
				{
					DecryptBufLen = sub_183C((char*)DecodeBuf, DecodeLen, (char*)QQpimdata->QQPimKey, QQpimdata->QQPimKeyLen, 
						DecryptBuf, DecryptBufLen,0);
				}
				else
				{
					return FALSE;
				}
			}
			else
			{
				DecryptBufLen = sub_183C(Data, DataSize, (char*)QQpimdata->QQPimKey, QQpimdata->QQPimKeyLen, DecryptBuf, DecryptBufLen,0);
			}

			if (DecryptBufLen > 0)
			{
				DecodeLen = QQPIM_DECRYPTBUF_LEN;
				Result = zdecompress((unsigned char *)DecryptBuf,DecryptBufLen, DecodeBuf,&DecodeLen);
				if (Result == 0)
				{
					DecodeLen = Utf8ToGB2312((char*)DecodeBuf,DecodeLen);
					DecryptBufLen = QQPIM_DECRYPTBUF_LEN;
					memset(DecodeBuf + DecodeLen, 0, QQPIM_DECRYPTBUF_LEN - DecodeLen);
					Result = ParseQQPhoneBookDownPack((char*)DecodeBuf, DecodeLen, DecryptBuf, &DecryptBufLen);
					if (Result)
					{
						WriteQQPhoneBookIndexFile(packh, HttpPro, (char*)DecryptBuf, QQpimdata);
					}
				}
				else
				{
					return FALSE;				
				}
			}
			else
			{
				return FALSE;
			}


			DecryptBufLen = QQPIM_DECRYPTBUF_LEN;
			DecryptBufLen = sub_183C(QQsyncasist->UpDataBuf, QQsyncasist->UpDataLen, (char*)QQpimdata->QQPimKey, QQpimdata->QQPimKeyLen, 
				DecryptBuf, DecryptBufLen,0);
			if (DecryptBufLen > 0)
			{
				DecodeLen = QQPIM_DECRYPTBUF_LEN;
				Result = zdecompress((unsigned char *)DecryptBuf,DecryptBufLen, DecodeBuf,&DecodeLen);
				if (Result == 0)
				{
					DecodeLen = Utf8ToGB2312((char*)DecodeBuf,DecodeLen);
					DecryptBufLen = QQPIM_DECRYPTBUF_LEN;
					memset(DecodeBuf + DecodeLen, 0, QQPIM_DECRYPTBUF_LEN - DecodeLen);
					Result = ParseQQPhoneBookUpPack((char*)DecodeBuf, DecodeLen, DecryptBuf, &DecryptBufLen);
					if (Result)
					{
						WriteQQPhoneBookIndexFile(packh, HttpPro, (char*)DecryptBuf, QQpimdata);
					}
				}
			}
 			
// 			char Dest[QQPIM_DECRYPTBUF_LEN];
// 			int DestBufLen = QQPIM_DECRYPTBUF_LEN;
// 			char * DestBuf = Dest;
// 			Result = GetQQPhoneBookBigUpPacket(packh, (char*)DestBuf, &DestBufLen);
// 			if (Result)
// 			{
// 				char * pData = (char*)DestBuf;
// 				int pDataSize = *(unsigned int*)pData;
// 
// 				while ( pData - DestBuf < DestBufLen)
// 				{
// 					pDataSize = *(unsigned int*)pData;
// 					pData += 4;
// 
// 					DecryptBufLen = QQPIM_DECRYPTBUF_LEN;
// 					DecryptBufLen = sub_183C(pData, pDataSize, (char*)QQpimdata->QQPimKey, QQpimdata->QQPimKeyLen, DecryptBuf, DecryptBufLen,0);
// 					if (DecryptBufLen > 0)
// 					{
// 						DecodeLen = QQPIM_DECRYPTBUF_LEN;
// 						Result = zdecompress((unsigned char *)DecryptBuf,DecryptBufLen, DecodeBuf,&DecodeLen);
// 						if (Result == 0)
// 						{
// 							DecryptBufLen = QQPIM_DECRYPTBUF_LEN;
// 							Result = ParseQQPhoneBookUpPack((char*)DecodeBuf, DecodeLen, DecryptBuf, &DecryptBufLen);
// 							if (Result)
// 							{
// 								WriteQQPhoneBookIndexFile(packh, HttpPro, (char*)DecodeBuf, QQpimdata);
// 							}
// 						}
// 					}
// 
// 
// 					DestBufLen -= *(unsigned int*)DestBuf;
// 					DestBuf = DestBuf + *(unsigned int*)DestBuf;
// 					pData = DestBuf;
// 				}
// 			}
		}
	}
	catch(...)
	{
		WriteLog(HTTPPRO, "ProcessQQPhoneBook Error!!");	
		return FALSE;
	}
	
	return TRUE;
}