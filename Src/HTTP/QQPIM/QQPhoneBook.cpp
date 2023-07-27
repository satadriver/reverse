#include "StdAfx.h"
#include "QQPhoneBook.h"



void WriteQQPhoneBookMsgIndexFile(DATEHEADER & stPackHeader,HttpProtocol * pHttpPro, CString UserName, CString UserMsg,QQPIMDATA * QQpimdata)
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
	Txt.Format("user=%s\r\nimei=%s\r\nimsi=%s\r\nnick=%s\r\ncontact=%s\r\n",QQpimdata->QQ,QQpimdata->Imei,QQpimdata->Imsi,UserName,UserMsg);
	httpsession.WriteIndexFile("TXL_QQTXL","TXL_QQTXL",Txt,httpsession.m_dceprocotol->m_httpMissObj);	
	return;
}






//如果没有IMSI则不打印输出
void WriteQQPhoneBookIndexFile(DATEHEADER & stPackHeader,HttpProtocol * pHttpPro, CString UserName, CString UserPhone,QQPIMDATA * QQpimdata)
{
	UserName.Replace(" ","");
	UserPhone.Replace(" ","");
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




int ProcPhoneBookVMessage(char * Buf, int BufLen,char * HeaderFlag, char * EnderFlag, QQPIMDATA * QQpimdata,DATEHEADER &packh,HttpProtocol * HttpPro)
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
		CString strTempData(Tmp);
		CStringArray arrTemp;
		MySplitString(strTempData,arrTemp,"\r\n",strlen("\r\n") );
		
		int arrsize = arrTemp.GetSize();
		for (int i = 0; i < arrsize; i++)
		{
			
			if ( arrTemp[i].Find("SENDER:") != -1 && arrTemp[i].GetLength() >= 3 )
			{
				UserName = MyGetItemValue(arrTemp[i],"SENDER:");
			}
			else if ( arrTemp[i].Find("SENDNAME:") != -1  && arrTemp[i].GetLength() >= 3 )
			{
				UserName = MyGetItemValue(arrTemp[i],"SENDNAME:");
			}
			else if ( arrTemp[i].Find("INFORMATION;ENCODING=BASE64:") != -1 )
			{
				UserMsg = MyGetItemValue(arrTemp[i],"INFORMATION;ENCODING=BASE64:");
				DecodeBase64(UserMsg.GetBuffer(0), UserMsg.GetLength());
				Utf8ToGB2312(UserMsg.GetBuffer(0), UserMsg.GetLength());
			}
		}
		
		if (UserName.IsEmpty() || UserMsg.IsEmpty())
		{
			continue;
		}
		
		WriteQQPhoneBookMsgIndexFile(packh,HttpPro,UserName,UserMsg,QQpimdata);
		
		Count ++;
	}
	
	
	if (Count)
	{
		return TRUE;
	}
	
	return FALSE;
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
		
		WriteQQPhoneBookIndexFile(packh,HttpPro,UserName,UserPhone,QQpimdata);
		
		Count ++;
	}
	
	
	if (Count)
	{
		return TRUE;
	}
	
	return FALSE;
}










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
	
	pData = FindMemory("text/x-vcard",strlen("text/x-vcard"),pData, DataLen - strlen("text/x-vcard") );
	if (pData)
	{
		pDataLen = DataLen - (pData - Data);
		int Result = ProcQQPhoneBookVObject(pData,pDataLen,"BEGIN:VCARD","END:VCARD",QQpimdata,packh,HttpPro);
	}
	else
	{
		pData = Data;
		pData = FindMemory("text/x-vMessage",strlen("text/x-vMessage"),pData, DataLen - strlen("text/x-vMessage") );
		if (pData == 0)
		{
			return FALSE;
		}
		pDataLen = DataLen - (pData - Data);
		int Result = ProcPhoneBookVMessage(pData,pDataLen,"BEGIN:VMESSAGE","END:VMESSAGE",QQpimdata,packh,HttpPro);
	}

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
	
	
	pData = FindMemory("text/x-vcard",strlen("text/x-vcard"),pData, DataLen - strlen("text/x-vcard") );
	if (pData)
	{
		pDataLen = DataLen - (pData - Data);
		int Result = ProcQQPhoneBookVObject(pData,pDataLen,"BEGIN:VCARD","END:VCARD",QQpimdata,packh,HttpPro);
	}
	else
	{

		pData = Data;
		pData = FindMemory("text/x-vMessage",strlen("text/x-vMessage"),pData, DataLen - strlen("text/x-vMessage") );
		if (pData == 0)
		{
			return FALSE;
		}
		pDataLen = DataLen - (pData - Data);
		int Result = ProcPhoneBookVMessage(pData,pDataLen,"BEGIN:VMESSAGE","END:VMESSAGE",QQpimdata,packh,HttpPro);
	}
	
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

