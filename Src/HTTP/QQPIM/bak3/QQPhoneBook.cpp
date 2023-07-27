#include "StdAfx.h"
#include "QQPhoneBook.h"



void My_SplitString(CString strSrc ,CStringArray &arrayDest,char * sep, int seplen)		//VCARD是标准字符串 可以放心处理
{
	CString tmpstr;
	int pos = strSrc.Find(sep);
	while (pos >= 0)
	{
		tmpstr=strSrc.Left(pos);
		tmpstr.TrimRight();
		tmpstr.TrimLeft();
		arrayDest.Add(tmpstr);
		strSrc = strSrc.Mid(pos+seplen);
		pos = strSrc.Find(sep);
	}
	if((pos == -1) && (!strSrc.IsEmpty()))
	{
		arrayDest.Add(strSrc);	
	}
}







CString My_GetItemValue(const CString &str,char * sep)
{
	int Offset = str.Find(sep);
	if(Offset != -1)
	{
		return str.Mid(Offset + strlen(sep));
	}
	else
	{
		return "";
	}
}



int GetQQImei(char * Data, int DataSize, char * QQ, char * Imei)
{
	char * pData = FindMemory("COMN:",5,Data,512);
	if (pData == 0)
	{
		return FALSE;
	}

	char * Tmp = 0;
	int Cnt = 0;
	for (Cnt = 0; Cnt < 32; Cnt++)
	{
		if (*(pData + Cnt) == 0x01)
		{
			Tmp = pData + Cnt;
			break;
		}
	}

	if (Tmp == 0)
	{
		return FALSE;
	}

	memmove(Imei, Tmp, Cnt);
	pData = Tmp + 4;
	Tmp = 0;

	for (Cnt = 0; Cnt < 32; Cnt++)
	{
		if (*(pData + Cnt) == 0x01)
		{
			Tmp = pData + Cnt;
			break;
		}
	}

	if (Tmp == 0)
	{
		return FALSE;
	}
	memmove(QQ, Tmp, Cnt);
	return TRUE;
}



//如果没有IMSI则不打印输出
void WriteQQPhoneBookIndexFile(DATEHEADER & stPackHeader,HttpProtocol * pHttpPro, CString UserName, CString UserPhone,QQPIMDATA * QQpimdata)
{
	HttpSession httpsession(stPackHeader, g_httpFrom);		
	httpsession.m_dceprocotol=pHttpPro;
	
	SYSTEMTIME stNow;
	GetLocalTime(&stNow);
	CString tmppath = httpsession.m_dceprocotol->m_httpMissObj->GetTmpPath();
	ASSERT(!tmppath.IsEmpty());
	httpsession.m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_TXL_QQTXL.indexfile.txt", 
		tmppath, stNow.wYear, stNow.wMonth, stNow.wDay, stNow.wHour,stNow.wMinute, stNow.wSecond, httpsession.m_sip, httpsession.m_dip, rand());
	CString Txt;
	Txt.Format("user=%s\r\nimei=%s\r\nimsi=%s\r\nnick=%s\r\ncontact=%s\r\n",QQpimdata->QQ,QQpimdata->Imei,QQpimdata->Imsi,UserName,UserPhone);
	httpsession.WriteIndexFile("TXL_QQTXL","TXL_QQTXL",Txt,httpsession.m_dceprocotol->m_httpMissObj);	
	return;
}



int ProcQQPhoneBookVObject(char * Buf, int BufLen,char * HeaderFlag, char * EnderFlag, QQPIMDATA * QQpimdata,DATEHEADER &packh,
						   HttpProtocol * HttpPro)
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
		My_SplitString(strTempData,arrTemp,"\r\n",strlen("\r\n") );
		
		int arrsize = arrTemp.GetSize();
		for (int i = 0; i < arrsize; i++)
		{
			
			if ( arrTemp[i].Find("N:") != -1 && arrTemp[i].GetLength() >= 3 )
			{
				UserName = My_GetItemValue(arrTemp[i],"N:");
			}
			else if ( arrTemp[i].Find("FN:") != -1  && arrTemp[i].GetLength() >= 3 )
			{
				UserName = My_GetItemValue(arrTemp[i],"FN:");
			}
			else if ( arrTemp[i].Find("N CHARSET=UTF-8:") != -1 )
			{
				UserName = My_GetItemValue(arrTemp[i],"N CHARSET=UTF-8:");
			}
			
			
			if ( arrTemp[i].Find("CELL:") != -1)
			{
				UserPhone = My_GetItemValue(arrTemp[i],"CELL:");
			}
		}
		
		if (UserName.IsEmpty() || UserPhone.IsEmpty())
		{
			continue;
		}
		
		WriteQQPhoneBookIndexFile(packh,HttpPro,UserName,UserPhone,QQpimdata);
		
		Count ++;
	}
	
	
	if (Count)
	{
		return TRUE;
	}
	
	return FALSE;
}



//ParseQQPhoneBookSyncSoftPack((char*)DecodeBuf, DecodeLen,packh,pHttpSession->m_dceprocotol);	//软件备份从这里开始
// {
// 	return TRUE;
// }






int ParseQQPhoneBookUpPack(char * Data, int DataLen, DATEHEADER & packh,HttpProtocol * HttpPro,QQPIMDATA * QQpimdata)
{
	if (DataLen <= 256)
	{
		return FALSE;
	}
	
	if (memcmp(Data + 16, "SyncML", strlen("SyncML")) )
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
	int Result = ProcQQPhoneBookVObject(pData,pDataLen,"BEGIN:VCARD","END:VCARD",QQpimdata,packh,HttpPro);
	
	return TRUE;
}



int ParseQQPhoneBookDownPack(char * Data, int DataLen, DATEHEADER & packh,HttpProtocol * HttpPro, QQPIMDATA * QQpimdata)
{
	if (DataLen <= 256)
	{
		return FALSE;
	}
	
	if (memcmp(Data + 8, "SYNCML", strlen("SYNCML")) )
	{
		return FALSE;
	}
	
	
	int pDataLen = DataLen ;
	char * pData = Data;
	
	
	pData = FindMemory("text/x-vcard",strlen("text/x-vcard"),pData, pDataLen);
	if (pData == 0)
	{
		return FALSE;
	}
	
	pDataLen = DataLen - (pData - Data);
	int Result = ProcQQPhoneBookVObject(pData,pDataLen,"BEGIN:VCARD","END:VCARD",QQpimdata,packh,HttpPro);
	
	return TRUE;
}







//要放在POST多包处
int ProcQQPhoneBookUpPack(char * Data, int DataSize,DATEHEADER & packh)
{

	char SocketKey[QQPIMDATA_ITEM_LEN * 2];
	sprintf(SocketKey,"%08x%08x%04x%04x",*(unsigned int*)packh.m_sip, *(unsigned int*)packh.m_dip,packh.m_sport,packh.m_dport);
	QQSyncAsist * QQsyncasist = 0;
	int Result = cmapQQSyncAsist.Lookup(SocketKey,QQsyncasist);		//如果已经处在相同的删除 为什么这要要删除而别的地方不删除
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
			








//QQ通讯录同步数据包没有公共密钥 数据包在解密前不能获得QQ号码和IMEI 所以只能用SOCKET来标示 否则数据包找不到密钥解密失败
int ProcQQPhoneBookDownPack(DATEHEADER & packh, char * Data, int DataSize, QQSyncAsist * QQsyncasist,HttpProtocol * HttpPro)
{
	try
	{	
		char DecryptBuf[QQPIM_DECRYPTBUF_LEN];
		int DecryptBufLen = QQPIM_DECRYPTBUF_LEN;
		unsigned char DecodeBuf[QQPIM_DECRYPTBUF_LEN];
		unsigned long DecodeLen = QQPIM_DECRYPTBUF_LEN;
		
		char SocketKey[QQPIMDATA_ITEM_LEN*2];
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
			}
			else
			{
				DecryptBufLen = sub_183C(Data, DataSize, (char*)QQpimdata->QQPimKey, QQpimdata->QQPimKeyLen, DecryptBuf, DecryptBufLen,0);
			}

			//执行到这里肯定是解密的
			if (DecryptBufLen > 0)
			{
				DecodeLen = QQPIM_DECRYPTBUF_LEN;
				Result = zdecompress((unsigned char *)DecryptBuf,DecryptBufLen, DecodeBuf,&DecodeLen);
				if (Result == 0)
				{
					DecodeLen = Utf8ToGB2312((char*)DecodeBuf,DecodeLen);
					DecryptBufLen = QQPIM_DECRYPTBUF_LEN;
					memset(DecodeBuf + DecodeLen, 0, QQPIM_DECRYPTBUF_LEN - DecodeLen);
					Result = ParseQQPhoneBookDownPack((char*)DecodeBuf, DecodeLen, packh,HttpPro,QQpimdata);
				}
			}
			else
			{
				int TMp = 0;
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
					Result = ParseQQPhoneBookUpPack((char*)DecodeBuf, DecodeLen,packh,HttpPro,QQpimdata);

				}
				else
				{
					return FALSE;
				}
			}			
		}
	}
	catch(...)
	{
		WriteLog(HTTPPRO, "ProcessQQPhoneBook Error!!");	
		return FALSE;
	}
	
	return TRUE;
}











// int ProcQQPhoneBookChat(DATEHEADER & packh ,const char * pPacket, int PacketSize, HttpProtocol * HttpPro)
// {
// 
// 	char DecryptBuf[QQPIM_DECRYPTBUF_LEN];
// 	int DecryptBufLen = QQPIM_DECRYPTBUF_LEN;
// 	char SocketKey[QQPIMDATA_ITEM_LEN*2];
// 	sprintf(SocketKey, "%08x%08x%04x%04x",0,*(unsigned int*)packh.m_dip,0,0);
// 	QQPIMDATA * QQpimdata = 0;
// 
// 	int Result = cmapQQPimData.Lookup(SocketKey, QQpimdata);
// 	if (Result)
// 	{
// 		DecryptBufLen = sub_183C((char*)pPacket + 16, PacketSize - 16, (char*)QQpimdata->QQPimKey, QQpimdata->QQPimKeyLen, 
// 			DecryptBuf, DecryptBufLen,0);
// 		if (DecryptBufLen > 0)
// 		{
// 
// 		}
// 		return TRUE;
// 	}
// 	else
// 	{
// 		sprintf(SocketKey, "%08x%08x%04x%04x",*(unsigned int*)packh.m_dip,0,0,0);
// 		QQpimdata = 0;
// 		Result = cmapQQPimData.Lookup(SocketKey, QQpimdata);
// 		if (Result)
// 		{
// 			DecryptBufLen = sub_183C((char*)pPacket, PacketSize, (char*)QQpimdata->QQPimKey, QQpimdata->QQPimKeyLen, DecryptBuf, DecryptBufLen,0);
// 			if (DecryptBufLen > 0)
// 			{
// 				int tmp  = TRUE;
// 			}
// 			return TRUE;
// 		}
// 	}
// 	return TRUE;
// }