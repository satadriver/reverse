#include "StdAfx.h"
#include "WechatContact.h"



//如果没有IMSI则不打印输出
void WriteWechatContctIndexFile(DATEHEADER & stPackHeader,HttpProtocol * pHttpPro, CString UserName, CString UserPhone,QQPIMDATA * QQpimdata)
{
	UserName.Replace(" ","");
	UserPhone.Replace(" ","");
	HttpSession httpsession(stPackHeader, g_httpFrom);		
	httpsession.m_dceprocotol=pHttpPro;
	
	SYSTEMTIME stNow;
	GetLocalTime(&stNow);
	CString tmppath = httpsession.m_dceprocotol->m_httpMissObj->GetTmpPath();
	ASSERT(!tmppath.IsEmpty());
	httpsession.m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_TXL_WEIXIN.indexfile.txt", 
		tmppath, stNow.wYear, stNow.wMonth, stNow.wDay, stNow.wHour,stNow.wMinute, stNow.wSecond, httpsession.m_sip, httpsession.m_dip, rand());
	CString Txt;
	Txt.Format("user=%s\r\nimei=%s\r\nimsi=%s\r\nnick=%s\r\ncontact=%s\r\n",QQpimdata->QQ,QQpimdata->Imei,QQpimdata->Imsi,UserName,UserPhone);
	httpsession.WriteIndexFile("TXL_WEIXIN","TXL_WEIXIN",Txt,httpsession.m_dceprocotol->m_httpMissObj);	
	return;
}










int ProcWechatContactVObject(char * Buf, int BufLen,char * HeaderFlag, char * EnderFlag, QQPIMDATA * QQpimdata,DATEHEADER &packh,
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
		
		WriteWechatContctIndexFile(packh,HttpPro,UserName,UserPhone,QQpimdata);
		
		Count ++;
	}
	
	
	if (Count)
	{
		return TRUE;
	}
	
	return FALSE;
}






int GetQQInfo(char * Data, int DataSize, char * QQ, char * Imei)
{
	try
	{
		char * Tmp = FindMemory("COMN:",strlen("COMN:"),Data,DataSize);
		if (Tmp == 0)
		{
			return FALSE;
		}

		int i = 0;
		for (i = 0; i < 16; i++)
		{
			if (isdigit(Tmp[i]) )
			{
				Imei[i] = Tmp[i];
			}
			else
			{
				break;
			}
		}
		Imei[i + 1] = 0;


		Tmp += strlen(Imei);
		Tmp += 4;			//01 56 a8 ba


		
		for (i = 0; i < 16; i++)
		{
			if (isdigit(Tmp[i]) )
			{
				QQ[i] = Tmp[i];
			}
			else
			{
				break;
			}
		}
		QQ[i + 1] = 0;
		if (strlen(QQ) > 0)
		{
			return TRUE;
		}

		return FALSE;	
	}
	catch (...)
	{
		return FALSE;
	}
}






int ParseWechatContactUpPack(char * Data, int DataLen, DATEHEADER & packh,HttpProtocol * HttpPro,QQPIMDATA * QQpimdata)
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


	int Result = GetQQInfo(pData, DataLen, QQpimdata->QQ, QQpimdata->Imei);
	if (Result == 0)
	{
		return FALSE;
	}
	
	pData = FindMemory("text/x-vcard",strlen("text/x-vcard"),pData, DataLen);
	if (pData)
	{

		pDataLen = DataLen - (pData - Data);
		int Result = ProcWechatContactVObject(pData,pDataLen,"BEGIN:VCARD","END:VCARD",QQpimdata,packh,HttpPro);
	}

	return TRUE;
}






int ParseWechatContactDownPack(char * Data, int DataLen, DATEHEADER & packh,HttpProtocol * HttpPro,QQPIMDATA * QQpimdata)
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
	
	int Result = GetQQInfo(pData, DataLen, QQpimdata->QQ, QQpimdata->Imei);
	if (Result == 0)
	{
		return FALSE;
	}

	
	pData = FindMemory("text/x-vcard",strlen("text/x-vcard"),pData, DataLen);
	if (pData)
	{
		pDataLen = DataLen - (pData - Data);
		int Result = ProcWechatContactVObject(pData,pDataLen,"BEGIN:VCARD","END:VCARD",QQpimdata,packh,HttpPro);
	}

	
	return TRUE;
}







//要放在POST多包处
int ProcWechatContactUpPack(char * Data, int DataSize,DATEHEADER & packh)
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
	QQsyncasist = new QQSyncAsist(WECHAT_CONTACT);
	memmove(QQsyncasist->UpDataBuf + QQsyncasist->UpDataLen, Data, DataSize );
	QQsyncasist->UpDataLen += DataSize;
	cmapQQSyncAsist.SetAt(SocketKey, QQsyncasist);
	cmapQQSyncAsistCnt ++;
	return TRUE;
}
			








//QQ通讯录同步数据包没有公共密钥 数据包在解密前不能获得QQ号码和IMEI 所以只能用SOCKET来标示 否则数据包找不到密钥解密失败
int ProcWechatContactDownPack(DATEHEADER & packh, char * Data, int DataSize, QQSyncAsist * QQsyncasist,HttpProtocol * HttpPro)
{
	try
	{	
		char DecryptBuf[QQPIM_DECRYPTBUF_LEN];
		int DecryptBufLen = QQPIM_DECRYPTBUF_LEN;
		unsigned char DecodeBuf[QQPIM_DECRYPTBUF_LEN];
		unsigned long DecodeLen = QQPIM_DECRYPTBUF_LEN;
		int Result = 0;
		

		if (memcmp(Data,"\x1f\x8b\x08\x00\x00\x00\x00\x00",8) == 0)
		{
			Result = Mygzdecompress((unsigned char *)Data + 10,DataSize - 10, DecodeBuf,&DecodeLen);
			if (Result == 0)
			{
				DecryptBufLen = sub_183C((char*)DecodeBuf, DecodeLen, "@#%^SEF#$Sddhfvh", strlen("@#%^SEF#$Sddhfvh"), 
					DecryptBuf, DecryptBufLen,0);
			}	
		}
		else
		{
			DecryptBufLen = sub_183C(Data, DataSize, "@#%^SEF#$Sddhfvh", strlen("@#%^SEF#$Sddhfvh"), DecryptBuf, DecryptBufLen,0);
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
				Result = ParseWechatContactDownPack((char*)DecodeBuf, DecodeLen, packh,HttpPro,&QQsyncasist->QQPimData);
			}
		}
		else
		{
			int TMp = 0;
		}

		DecryptBufLen = QQPIM_DECRYPTBUF_LEN;
		DecryptBufLen = sub_183C(QQsyncasist->UpDataBuf, QQsyncasist->UpDataLen, "@#%^SEF#$Sddhfvh", strlen("@#%^SEF#$Sddhfvh"), 
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
				Result = ParseWechatContactUpPack((char*)DecodeBuf, DecodeLen,packh,HttpPro,&QQsyncasist->QQPimData);
			}
			else
			{
				return FALSE;
			}
		}			

	}
	catch(...)
	{
		WriteLog(HTTPPRO, "ProcessWechat Error!!");	
		return FALSE;
	}
	
	return TRUE;
}
