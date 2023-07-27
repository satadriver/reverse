#include "stdafx.h"
#include "http.h"
#include "HttpSession.h"
#include "YiXinHttpSession.h"
#include "VoipUrl.h"
#include "..\imsg\imsg.h"
#include "..\\Include\\openssl\\rc4.h"



#define  _DWORD	DWORD
#define  _BYTE	BYTE


int YiXinHttpSession::BuildDecryptKey(DATEHEADER& stPackHeader, const char * lpcstrPacket)
{
	const int strYiXinDecryptKey[YIXIN_KEY_LEN] = 
	{
			0x00,0x00,0x34,0x6a,
			0x81,0xD4,0xEA,0x47,
			0xBC,0xC1,0x24,0xCA,
			0x7B,0x19,0x70,0x2F,
			0x1A,0x2A,0x68,0x62,
			0xB7,0x4E,0xFB,0x53,
			0x50,0x04,0x3A,0xD3,
			0x48,0xDD,0x71,0x4D,
			0x32,0x77,0x3F,0xEB,
			0x55,0x02,0x14,0xB6,
			0x93,0xA1,0xAB,0x03,
			0x11,0x8F,0xF1,0x57,
			0xB1,0xEE,0x52,0xB2,
			0xAC,0x7A,0xAE,0x06,
			0x86,0xF3,0x82,0xAA,
			0xBD,0x85,0xF7,0xD9,
			0x22,0xB0,0x8E,0x8A,
			0x79,0xF4,0x6C,0xE7,
			0xA4,0x9F,0xE3,0x96,
			0xFC,0x7C,0xBA,0x2C,
			0x4C,0xA3,0x80,0x98,
			0x65,0x5E,0xE6,0xAF,
			0xCB,0x58,0x67,0x1C,
			0x08,0x4F,0x3C,0x44,
			0xC9,0x6B,0xE4,0x7E,
			0x1E,0x3D,0xE8,0xA6,
			0xBB,0x5F,0x6F,0x59,
			0x97,0x9E,0xB4,0x30,			
			0x2D,0x2B,0xF2,0x6E,
			0x0A,0xA7,0xE9,0x5D,
			0x29,0x5C,0x33,0xEF,
			0x91,0xE5,0x39,0x36,
			0x17,0xDF,0x31,0x45,
			0x8D,0x5B,0x05,0xF0,
			0xC8,0x09,0xA5,0xF9,
			0x84,0xC7,0x1B,0xB5,
			0xC6,0x9B,0xE2,0xC5,
			0x89,0xE0,0x9A,0x27,
			0xC4,0x40,0x0D,0x20,	
			0x66,0x7F,0xA9,0xC3,
			0x07,0xB3,0x2E,0x49,
			0xFA,0xD5,0xAD,0xE1,
			0x83,0x6D,0x0E,0x43,
			0x28,0x78,0x42,0x64,
			0x8B,0x16,0x74,0x88,
			0xD8,0x1F,0xA0,0x3E,
			0x63,0x60,0xFD,0xA8,
			0xD7,0x76,0x72,0x18,
			0xDC,0x56,0xFE,0xD0,
			0x90,0xD1,0x9D,0xDB,
			0x94,0x8C,0xDA,0xDE,
			0xBE,0xCD,0xCF,0xC2,
			0x1D,0x51,0xCE,0xBF,
			0x73,0x35,0x00,0x92,
			0x7D,0xC0,0x12,0x69,
			0x26,0x38,0x4B,0xFF,
			0xED,0x9C,0x13,0xF5,
			0x15,0xD2,0xEC,0xF8,
			0xB9,0x46,0xA2,0x41,
			0xF6,0x99,0x75,0x95,
			0x87,0x23,0x10,0x61,
			0x25,0xB8,0x37,0xCC,
			0x0C,0xD6,0x01,0x0F,
			0x21,0x4A,0x54,0x3B,
			0x5A,0x0B,0x00,0x00
	};
	memmove(m_strYiXinDecryptKey,strYiXinDecryptKey,YIXIN_KEY_LEN * sizeof(int));	
	m_closetm			= stPackHeader.m_tm + 120;
	m_endfiletm			= m_closetm;
	m_lastpacktm		= stPackHeader.m_tm;
	m_begfiletm			= stPackHeader.m_tm;
	return TRUE;
}


YiXinHttpSession::YiXinHttpSession(DATEHEADER& stPackHeader, const char * lpcstrPacket)
{
	m_srcip = *(unsigned int*)stPackHeader.m_sip;
	m_dstip = *(unsigned int*)stPackHeader.m_dip;
	m_sport = stPackHeader.m_sport;
	m_dport = stPackHeader.m_dport;

	BuildDecryptKey(stPackHeader,lpcstrPacket);
	m_closetm			= stPackHeader.m_tm + 120;
	m_begfiletm			= stPackHeader.m_tm;
	m_endfiletm			= m_closetm;
	m_lastpacktm		= stPackHeader.m_tm;
}


YiXinHttpSession::~YiXinHttpSession() { }



int YiXinHttpSession::IsYiXinStartPacket(DATEHEADER& stPackHeader, const char* lpcstrPacket)
{
	if( (stPackHeader.m_sport == 8080) || (stPackHeader.m_sport == 80) || (stPackHeader.m_sport == 443) ||
		(stPackHeader.m_dport == 8080) || (stPackHeader.m_dport == 80) || (stPackHeader.m_dport == 443) )
	{
		if( stPackHeader.m_type == TCPPacket)
		{
			//\x93\x02\x50\x0a\x00\x00\x0d\xc8是第一个包含加密信息的包 解密为固定密钥
			if (memcmp(lpcstrPacket,"\x98\x12\x07\x55\x58\x57\xa9\x41\xd3\x01\xf7\x38\xcc\xab\xc3\xb6",16) == 0)				
			{
				return YIXIN_PC_FIRSTSEND;
			}
			else if (memcmp(lpcstrPacket,"\x90\x18\x1b\x57\x58\x5f\xa9\x41",8) == 0)	
			{
				return YIXIN_PC_FIRSTRECV;	
			}
		}
	}
	return FALSE;
}




void YiXinHttpSession::WriteYiXinIndexFile(DATEHEADER & stPackHeader,HttpProtocol * pHttpPro)
{
	HttpSession clsHttpSession(stPackHeader, g_httpFrom);		
	clsHttpSession.m_dceprocotol=pHttpPro;

	CString cstrTxt;
	if(m_uiState == YIXIN_MSG )
	{
		cstrTxt.Format("state=%s\r\nfrom=%s\r\nto=%s\r\nmsg=%s\r\n",
		"发送消息",m_cstrSendID,m_cstrRecvID,m_cstrMessage);
	}
	else if(m_uiState == YIXIN_LOGIN)
	{
		m_cstrMessage = "id:" + m_cstrNetEasyID + " " + "phone:" + m_cstrUserPhone + " " + "nick:" + m_cstrUserNick;
		cstrTxt.Format("state=%s\r\nfrom=%s\r\nto=%s\r\nmsg=%s\r\n","发送消息","","",m_cstrMessage);
	}
	else if(m_uiState == YIXIN_BUDDYLIST)
	{
		m_cstrSendID = "id:" + m_cstrNetEasyID + " " + "phone:" + m_cstrUserPhone + " " + "nick:" + m_cstrUserNick;
		cstrTxt.Format("state=%s\r\nfrom=%s\r\nto=%s\r\nmsg=%s\r\n","发送消息",m_cstrSendID,"",m_cstrMessage);
	}
	else
	{
		return;
	}
	
	m_uiState = 0;
	SYSTEMTIME stNow;
	GetLocalTime(&stNow);
	CString tmppath = clsHttpSession.m_dceprocotol->m_httpMissObj->GetTmpPath();
	ASSERT(!tmppath.IsEmpty());
	clsHttpSession.m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_imsg_yixin.indexfile.txt", 
		tmppath, stNow.wYear, stNow.wMonth, stNow.wDay, stNow.wHour,
		stNow.wMinute, stNow.wSecond, clsHttpSession.m_sip, clsHttpSession.m_dip, rand());
	clsHttpSession.WriteIndexFile("imsg_yixin","imsg_yixin",cstrTxt,clsHttpSession.m_dceprocotol->m_httpMissObj);
	return;
}





char * YiXinHttpSession::YixinFindMemory(char Flag[], int FlagLen, char Address[], int Offset)
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








int YiXinHttpSession::ProcessYiXinPacket(DATEHEADER& stPackHeader, const char *lpcstrPacket,HttpProtocol * pHttpPro)
{
	if (m_srcip != *(unsigned int*)stPackHeader.m_sip || m_dstip != *(unsigned int*)stPackHeader.m_dip ||
		m_sport != stPackHeader.m_sport || m_dport != stPackHeader.m_dport)
	{
		return FALSE;
	}

	if (memcmp(m_cmpbuf, lpcstrPacket, 8) == 0)
	{
		return FALSE;
	}
	memmove(m_cmpbuf, lpcstrPacket,8);

	if( (memcmp(lpcstrPacket,"\x98\x12\x07\x55\x58\x57\xa9\x41\xd3\x01\xf7\x38\xcc\xab\xc3\xb6",16) == 0)  )	
	{
		BuildDecryptKey(stPackHeader,lpcstrPacket);	
		m_uiMode = YIXIN_PC_FIRSTSEND;
	}
	else if( (memcmp(lpcstrPacket,"\x90\x18\x1b\x57\x58\x5f\xa9\x41",8) == 0) )
	{
		BuildDecryptKey(stPackHeader,lpcstrPacket);	
		m_uiMode = YIXIN_PC_FIRSTRECV;
	}



#ifdef DEBUG
	if (m_uiMode == YIXIN_PC_FIRSTSEND)
	{
		int flag = 0;
	}
#endif


	memmove(m_strEncryptBuffer,lpcstrPacket,stPackHeader.m_datalen);
	sub_67FEF0((int)m_strYiXinDecryptKey,stPackHeader.m_datalen,(int)m_strEncryptBuffer,(int)m_strDecryptBuffer);


	char * result = YiXinHttpSession::YixinFindMemory("\x0d\xc8",2,m_strDecryptBuffer,16);
	if (result == 0)
	{
		return FALSE;
	}

	if (memcmp(m_strDecryptBuffer + 1,"\x01\x62\x6a",3) == 0)	//buddylist	
	{
		return FALSE;

		char strBuddyList[YIXIN_BUDDYLIST_SIZE];
		int iListPointer = 0;
		int iItemPointer = 0x34;
		int iItemLen;
		for (int iCnt = *(m_strDecryptBuffer + 39); iCnt; iCnt --)
		{
			if (iListPointer >= YIXIN_BUDDYLIST_SIZE)
			{
				return FALSE;
			}
			memmove(strBuddyList + iListPointer, "网易号:",strlen("网易号:"));
			iListPointer += strlen("网易号:");
			iItemLen = *(unsigned short*)(m_strDecryptBuffer + iItemPointer - 2);
			memmove(strBuddyList + iListPointer, m_strDecryptBuffer + iItemPointer,iItemLen);
			iListPointer += iItemLen;

			memmove(strBuddyList + iListPointer, "账号:",strlen("账号:"));
			iListPointer += strlen("账号:");

			iItemPointer += 20;
			iItemLen = *(unsigned short*)(m_strDecryptBuffer + iItemPointer - 2);
			memmove(strBuddyList + iListPointer, m_strDecryptBuffer + iItemPointer,iItemLen);
			iListPointer += iItemLen;
			memmove(strBuddyList + iListPointer,"\r\n" ,2);
			iListPointer += 2;

			iItemPointer += 27;
		}

		*(strBuddyList + iListPointer ) = 0;
		m_uiState = YIXIN_BUDDYLIST;
		m_cstrMessage = strBuddyList;
		WriteYiXinIndexFile(stPackHeader,pHttpPro);	
	}
	else if( memcmp(m_strDecryptBuffer + 1,"\x5b\0x0f",2) == 0 )		//login
	{
		if (m_uiMode == YIXIN_PC_FIRSTRECV)			//下行数据，从发送方不会得到该数据
		{
			int iCnt = *(unsigned short*)(m_strDecryptBuffer + 0x19);
			m_cstrNetEasyID = m_strDecryptBuffer + 0x1b;
			*(m_strDecryptBuffer + 0x1b + iCnt) = 0;
			m_cstrNetEasyID = m_cstrNetEasyID.Left(iCnt);		//m_cstrNetEasyID.Left(iCnt)并不会重新赋值，需要再一次赋值

			iCnt = *(unsigned short*)(m_strDecryptBuffer + 0x27);
			m_cstrUserPhone = m_strDecryptBuffer + 0x29;
			*(m_strDecryptBuffer + 0x29 + iCnt ) = 0;
			m_cstrUserPhone = m_cstrUserPhone.Left(iCnt);

			iCnt = *(unsigned short*)(m_strDecryptBuffer + 0x38);
			m_cstrUserNick = m_strDecryptBuffer + 0x3a;
			*(m_strDecryptBuffer + 0x3a + iCnt ) = 0;
			m_cstrUserNick = m_cstrUserNick.Left(iCnt);
			m_uiState = *(unsigned short*)(m_strDecryptBuffer + 1);
		}
		else if (m_uiMode == YIXIN_PC_FIRSTSEND)
		{
			int iCnt = *(unsigned short*)(m_strDecryptBuffer + 0x10);
			m_cstrUserPhone = m_strDecryptBuffer + 0x12;
			*(m_strDecryptBuffer + 0x12 + iCnt) = 0;
			m_cstrUserPhone = m_cstrUserPhone.Left(iCnt);				
		}
		m_uiState = YIXIN_LOGIN;
		WriteYiXinIndexFile(stPackHeader,pHttpPro);	
	}
	else if( memcmp(m_strDecryptBuffer + 1,"\x5b\x65",2) == 0 )		//login
	{
		if (m_uiMode == YIXIN_PC_FIRSTRECV)			//下行数据，从发送方不会得到该数据
		{
			int iCnt = *(unsigned short*)(m_strDecryptBuffer + 0x18);
			m_cstrNetEasyID = m_strDecryptBuffer + 0x1a;
			*(m_strDecryptBuffer + 0x1a + iCnt) = 0;
			m_cstrNetEasyID = m_cstrNetEasyID.Left(iCnt);		//m_cstrNetEasyID.Left(iCnt)并不会重新赋值，需要再一次赋值
			
			iCnt = *(unsigned short*)(m_strDecryptBuffer + 38);
			m_cstrUserPhone = m_strDecryptBuffer + 40;
			*(m_strDecryptBuffer + 40 + iCnt ) = 0;
			m_cstrUserPhone = m_cstrUserPhone.Left(iCnt);
			
			iCnt = *(unsigned short*)(m_strDecryptBuffer + 55);
			m_cstrUserNick = m_strDecryptBuffer + 57;
			*(m_strDecryptBuffer + 57 + iCnt ) = 0;
			m_cstrUserNick = m_cstrUserNick.Left(iCnt);
			m_uiState = *(unsigned short*)(m_strDecryptBuffer + 1);
		}
		else if (m_uiMode == YIXIN_PC_FIRSTSEND)
		{
			int iCnt = *(unsigned short*)(m_strDecryptBuffer + 0x10);
			m_cstrUserPhone = m_strDecryptBuffer + 0x12;
			*(m_strDecryptBuffer + 0x12 + iCnt) = 0;
			m_cstrUserPhone = m_cstrUserPhone.Left(iCnt);				
		}
		m_uiState = YIXIN_LOGIN;
		WriteYiXinIndexFile(stPackHeader,pHttpPro);	
	}
	else if( memcmp(m_strDecryptBuffer + 1,"\x01\x5c",2) == 0) 
	{
		int iCnt = *(unsigned short*)(m_strDecryptBuffer + 0x2d);
		m_cstrSendID = m_strDecryptBuffer + 0x2f;
		*(m_strDecryptBuffer + 0x2f + iCnt ) = 0;
		m_cstrSendID =m_cstrSendID.Left(iCnt);
		Utf8ToGB2312(m_cstrSendID.GetBuffer(0));

		iCnt = *(unsigned short*)(m_strDecryptBuffer + 0x3b);
		m_cstrRecvID = m_strDecryptBuffer + 0x3d;
		*(m_strDecryptBuffer + 0x3d + iCnt ) = 0;
		m_cstrRecvID = m_cstrRecvID.Left(iCnt);
		Utf8ToGB2312(m_cstrRecvID.GetBuffer(0));

		char * strMessage = m_strDecryptBuffer + 0x4b;
		*(strMessage + *(unsigned short*)(m_strDecryptBuffer + 0x49) ) = 0;
		m_cstrMessage = Utf8ToGB2312(strMessage);
		m_uiState = YIXIN_MSG;
		WriteYiXinIndexFile(stPackHeader,pHttpPro);	
	}
	else if (memcmp(m_strDecryptBuffer + 1,"\x01\x60\x0A",3) == 0)	//message3
	{
		int iCnt = *(unsigned short*)(m_strDecryptBuffer + 21);
		m_cstrRecvID = m_strDecryptBuffer + 23;
		*(m_strDecryptBuffer + 23 + iCnt) = 0;
		m_cstrRecvID = m_cstrRecvID.Left(iCnt);
		Utf8ToGB2312(m_cstrRecvID.GetBuffer(0));

		iCnt = *(unsigned short*)(m_strDecryptBuffer + 42);
		m_cstrRecvID = m_strDecryptBuffer + 44;
		*(m_strDecryptBuffer + 44 + iCnt) = 0;
		m_cstrRecvID = m_cstrRecvID.Left(iCnt);
		Utf8ToGB2312(m_cstrRecvID.GetBuffer(0));

		iCnt = *(unsigned short*)(m_strDecryptBuffer + 59);
		m_cstrSendID = m_strDecryptBuffer + 61;
		*(m_strDecryptBuffer + 61 + iCnt) = 0;
		m_cstrSendID = m_cstrSendID.Left(iCnt);
		Utf8ToGB2312(m_cstrSendID.GetBuffer(0));
		
		iCnt = *(unsigned short*)(m_strDecryptBuffer + 73);
		char * strMessage = m_strDecryptBuffer + 75;
		*(strMessage + iCnt) = 0;
		m_cstrMessage = Utf8ToGB2312(strMessage);
		m_uiState = YIXIN_MSG;
		WriteYiXinIndexFile(stPackHeader,pHttpPro);	
	}
	else if (memcmp(m_strDecryptBuffer + 1,"\x01\x60",2) == 0)		//message
	{
		int iCnt = *(unsigned short*)(m_strDecryptBuffer + 0x19);
		m_cstrSendID = m_strDecryptBuffer + 0x1b;
		*(m_strDecryptBuffer + 0x1b + iCnt) = 0;
		m_cstrSendID = m_cstrSendID.Left(iCnt);
		Utf8ToGB2312(m_cstrSendID.GetBuffer(0));
		
		iCnt = *(unsigned short*)(m_strDecryptBuffer + 0x27);
		m_cstrRecvID = m_strDecryptBuffer + 0x29;
		*(m_strDecryptBuffer + 0x29 + iCnt ) = 0;
		m_cstrRecvID = m_cstrRecvID.Left(iCnt);
		Utf8ToGB2312(m_cstrRecvID.GetBuffer(0));
		
		iCnt = *(unsigned short*)(m_strDecryptBuffer + 0x35);
		char * strMessage = m_strDecryptBuffer + 0x37;
		*(strMessage + iCnt) = 0;
		m_cstrMessage = Utf8ToGB2312(strMessage);
		m_uiState = YIXIN_MSG;
		WriteYiXinIndexFile(stPackHeader,pHttpPro);	
	}
	else if( (memcmp(m_strDecryptBuffer + 1,"\x02\x60\x01",3) == 0)	||(memcmp(m_strDecryptBuffer + 1,"\x03\x60\x01",3) == 0) )	//message2
	{
		int iCnt = *(unsigned short*)(m_strDecryptBuffer + 0x19);
		m_cstrSendID = m_strDecryptBuffer + 0x1b;
		m_cstrSendID = m_cstrSendID.Left(iCnt);
		
		iCnt = *(unsigned short*)(m_strDecryptBuffer + 0x27);
		m_cstrRecvID = m_strDecryptBuffer + 0x29;
		m_cstrRecvID = m_cstrRecvID.Left(iCnt);
		
		char * strMessage = m_strDecryptBuffer + 0x37;
		iCnt = *(unsigned short*)(m_strDecryptBuffer + 0x35);

		if (memcmp(strMessage,"14",2) == 0 && iCnt == 10 && *(unsigned char*)(strMessage + 16) == 0x31)
		{
			strMessage = YiXinHttpSession::YixinFindMemory("http://nos.netease.com",strlen("http://nos.netease.com"), 
				m_strDecryptBuffer, stPackHeader.m_datalen);
			if (strMessage == 0)
			{
				return FALSE;
			}
			strMessage = strMessage - strlen("http://nos.netease.com");
			iCnt = *(unsigned short*)(strMessage -2);
		}

		*(strMessage + iCnt) = 0;
		m_cstrMessage = Utf8ToGB2312(strMessage);
		m_uiState = YIXIN_MSG;
		WriteYiXinIndexFile(stPackHeader,pHttpPro);	
	}
	else if (memcmp(m_strDecryptBuffer + 1,"\x07\x62",2) == 0)
	{
		return FALSE;
	}
	else if (memcmp(m_strDecryptBuffer + 1,"\x5c\x01",2) == 0)
	{
		return FALSE;
	}
	else if (memcmp(m_strDecryptBuffer + 1,"\x5d\x01",2) == 0)
	{
		return FALSE;
	}
	else if (memcmp(m_strDecryptBuffer + 1,"\x60\x01",2) == 0)
	{
		return FALSE;
	}

	m_lastpacktm = stPackHeader.m_tm;
	return TRUE;	
}








int YiXinHttpSession::sub_67FEF0(int a1, unsigned int a2, int a3, int a4)
{
  int result; // eax@1
  int v5; // ecx@1
  char v6; // bl@1
  int v7; // ebp@1
  int v8; // edi@1
  int v9; // esi@1
  int v10; // ecx@2
  int v11; // edx@2
  int v12; // esi@2
  int v13; // ebx@2
  int v14; // ecx@2
  int v15; // edx@2
  int v16; // esi@2
  int v17; // ebx@2
  int v18; // ecx@2
  int v19; // edx@2
  int v20; // esi@2
  int v21; // ebx@2
  int v22; // ecx@2
  int v23; // edx@2
  int v24; // esi@2
  int v25; // ebx@2
  int v26; // ecx@2
  int v27; // edx@2
  int v28; // esi@2
  int v29; // ebx@2
  int v30; // ecx@2
  int v31; // edx@2
  int v32; // esi@2
  int v33; // ebx@2
  int v34; // ecx@2
  int v35; // edx@2
  int v36; // esi@2
  int v37; // ebx@2
  int v38; // edx@2
  int v39; // ebx@2
  int v41; // edx@5
  int v42; // ebx@5
  char v43; // zf@5
  int v44; // edx@6
  int v45; // ebx@6
  char v46; // zf@6
  int v47; // edx@7
  int v48; // ebx@7
  char v49; // zf@7
  int v50; // edx@8
  int v51; // ebx@8
  char v52; // zf@8
  int v53; // edx@9
  int v54; // ebx@9
  char v55; // zf@9
  int v56; // edx@10
  int v57; // ebx@10
  char v58; // zf@10
  int v59; // edx@11
  int v60; // ebx@11
  char v61; // zf@11
  int v62; // edx@12
  int v63; // ebx@12
  char v64; // zf@12
  unsigned int v65; // [sp+14h] [bp+4h]@1
  int v66; // [sp+14h] [bp+4h]@4
  int v67; // [sp+14h] [bp+4h]@5
  int v68; // [sp+14h] [bp+4h]@6
  int v69; // [sp+14h] [bp+4h]@7
  int v70; // [sp+14h] [bp+4h]@8
  int v71; // [sp+14h] [bp+4h]@9
  int v72; // [sp+14h] [bp+4h]@10
  int v73; // [sp+14h] [bp+4h]@11

  result = a1;
  v5 = *(_DWORD *)a1;
  v6 = a2;
  v7 = a3;
  v9 = *(_DWORD *)(a1 + 4);
  v8 = a4;
  v65 = a2 >> 3;
  if ( a2 >> 3 )
  {
    do
    {
      v10 = (v5 + 1) & 0xFF;
      v11 = *(_DWORD *)(result + 4 * v10 + 8);
      v12 = (v11 + v9) & 0xFF;
      v13 = *(_DWORD *)(result + 4 * v12 + 8);
      *(_DWORD *)(result + 4 * v10 + 8) = v13;
      *(_DWORD *)(result + 4 * v12 + 8) = v11;
      *(_BYTE *)v8 = *(_BYTE *)v7 ^ *(_BYTE *)(result + 4 * ((v11 + v13) & 0xFF) + 8);
      v14 = (v10 + 1) & 0xFF;
      v15 = *(_DWORD *)(result + 4 * v14 + 8);
      v16 = (v15 + v12) & 0xFF;
      v17 = *(_DWORD *)(result + 4 * v16 + 8);
      *(_DWORD *)(result + 4 * v14 + 8) = v17;
      *(_DWORD *)(result + 4 * v16 + 8) = v15;
      *(_BYTE *)(v8 + 1) = *(_BYTE *)(v7 + 1) ^ *(_BYTE *)(result + 4 * ((v15 + v17) & 0xFF) + 8);
      v18 = (v14 + 1) & 0xFF;
      v19 = *(_DWORD *)(result + 4 * v18 + 8);
      v20 = (v19 + v16) & 0xFF;
      v21 = *(_DWORD *)(result + 4 * v20 + 8);
      *(_DWORD *)(result + 4 * v18 + 8) = v21;
      *(_DWORD *)(result + 4 * v20 + 8) = v19;
      *(_BYTE *)(v8 + 2) = *(_BYTE *)(v7 + 2) ^ *(_BYTE *)(result + 4 * ((v19 + v21) & 0xFF) + 8);
      v22 = (v18 + 1) & 0xFF;
      v23 = *(_DWORD *)(result + 4 * v22 + 8);
      v24 = (v23 + v20) & 0xFF;
      v25 = *(_DWORD *)(result + 4 * v24 + 8);
      *(_DWORD *)(result + 4 * v22 + 8) = v25;
      *(_DWORD *)(result + 4 * v24 + 8) = v23;
      *(_BYTE *)(v8 + 3) = *(_BYTE *)(v7 + 3) ^ *(_BYTE *)(result + 4 * ((v23 + v25) & 0xFF) + 8);
      v26 = (v22 + 1) & 0xFF;
      v27 = *(_DWORD *)(result + 4 * v26 + 8);
      v28 = (v27 + v24) & 0xFF;
      v29 = *(_DWORD *)(result + 4 * v28 + 8);
      *(_DWORD *)(result + 4 * v26 + 8) = v29;
      *(_DWORD *)(result + 4 * v28 + 8) = v27;
      *(_BYTE *)(v8 + 4) = *(_BYTE *)(v7 + 4) ^ *(_BYTE *)(result + 4 * ((v27 + v29) & 0xFF) + 8);
      v30 = (v26 + 1) & 0xFF;
      v31 = *(_DWORD *)(result + 4 * v30 + 8);
      v32 = (v31 + v28) & 0xFF;
      v33 = *(_DWORD *)(result + 4 * v32 + 8);
      *(_DWORD *)(result + 4 * v30 + 8) = v33;
      *(_DWORD *)(result + 4 * v32 + 8) = v31;
      v34 = (v30 + 1) & 0xFF;
      *(_BYTE *)(v8 + 5) = *(_BYTE *)(v7 + 5) ^ *(_BYTE *)(result + 4 * ((v31 + v33) & 0xFF) + 8);
      v35 = *(_DWORD *)(result + 4 * v34 + 8);
      v36 = (v35 + v32) & 0xFF;
      v37 = *(_DWORD *)(result + 4 * v36 + 8);
      *(_DWORD *)(result + 4 * v34 + 8) = v37;
      *(_DWORD *)(result + 4 * v36 + 8) = v35;
      *(_BYTE *)(v8 + 6) = *(_BYTE *)(v7 + 6) ^ *(_BYTE *)(result + 4 * ((v35 + v37) & 0xFF) + 8);
      v5 = (v34 + 1) & 0xFF;
      v38 = *(_DWORD *)(result + 4 * v5 + 8);
      v9 = (v38 + v36) & 0xFF;
      v39 = *(_DWORD *)(result + 4 * v9 + 8);
      *(_DWORD *)(result + 4 * v5 + 8) = v39;
      *(_DWORD *)(result + 4 * v9 + 8) = v38;
      v38 = *(_BYTE *)(v7 + 7) ^ *(_BYTE *)(result + 4 * ((v38 + v39) & 0xFF) + 8);
	  v38 &= 0xff;
      v7 += 8;
      *(_BYTE *)(v8 + 7) = v38;
      v8 += 8;
    }
    while ( v65-- != 1 );
    v6 = a2;
  }
  v66 = v6 & 7;
  if ( v6 & 7 )
  {
    v5 = (v5 + 1) & 0xFF;
    v41 = *(_DWORD *)(result + 4 * v5 + 8);
    v9 = (v41 + v9) & 0xFF;
    v42 = *(_DWORD *)(result + 4 * v9 + 8);
    *(_DWORD *)(result + 4 * v5 + 8) = v42;
    *(_DWORD *)(result + 4 * v9 + 8) = v41;
    v43 = v66 == 1;
    v67 = v66 - 1;
    *(_BYTE *)v8 = *(_BYTE *)v7 ^ *(_BYTE *)(result + 4 * ((v41 + v42) & 0xFF) + 8);
    if ( !v43 )
    {
      do
      {
        v5 = (v5 + 1) & 0xFF;
        v44 = *(_DWORD *)(result + 4 * v5 + 8);
        v9 = (v44 + v9) & 0xFF;
        v45 = *(_DWORD *)(result + 4 * v9 + 8);
        *(_DWORD *)(result + 4 * v5 + 8) = v45;
        *(_DWORD *)(result + 4 * v9 + 8) = v44;
        v46 = v67 == 1;
        v68 = v67 - 1;
        *(_BYTE *)(v8 + 1) = *(_BYTE *)(v7 + 1) ^ *(_BYTE *)(result + 4 * ((v44 + v45) & 0xFF) + 8);
        if ( v46 )
          break;
        v5 = (v5 + 1) & 0xFF;
        v47 = *(_DWORD *)(result + 4 * v5 + 8);
        v9 = (v47 + v9) & 0xFF;
        v48 = *(_DWORD *)(result + 4 * v9 + 8);
        *(_DWORD *)(result + 4 * v5 + 8) = v48;
        *(_DWORD *)(result + 4 * v9 + 8) = v47;
        v49 = v68 == 1;
        v69 = v68 - 1;
        *(_BYTE *)(v8 + 2) = *(_BYTE *)(v7 + 2) ^ *(_BYTE *)(result + 4 * ((v47 + v48) & 0xFF) + 8);
        if ( v49 )
          break;
        v5 = (v5 + 1) & 0xFF;
        v50 = *(_DWORD *)(result + 4 * v5 + 8);
        v9 = (v50 + v9) & 0xFF;
        v51 = *(_DWORD *)(result + 4 * v9 + 8);
        *(_DWORD *)(result + 4 * v5 + 8) = v51;
        *(_DWORD *)(result + 4 * v9 + 8) = v50;
        v52 = v69 == 1;
        v70 = v69 - 1;
        *(_BYTE *)(v8 + 3) = *(_BYTE *)(v7 + 3) ^ *(_BYTE *)(result + 4 * ((v50 + v51) & 0xFF) + 8);
        if ( v52 )
          break;
        v5 = (v5 + 1) & 0xFF;
        v53 = *(_DWORD *)(result + 4 * v5 + 8);
        v9 = (v53 + v9) & 0xFF;
        v54 = *(_DWORD *)(result + 4 * v9 + 8);
        *(_DWORD *)(result + 4 * v5 + 8) = v54;
        *(_DWORD *)(result + 4 * v9 + 8) = v53;
        v55 = v70 == 1;
        v71 = v70 - 1;
        *(_BYTE *)(v8 + 4) = *(_BYTE *)(v7 + 4) ^ *(_BYTE *)(result + 4 * ((v53 + v54) & 0xFF) + 8);
        if ( v55 )
          break;
        v5 = (v5 + 1) & 0xFF;
        v56 = *(_DWORD *)(result + 4 * v5 + 8);
        v9 = (v56 + v9) & 0xFF;
        v57 = *(_DWORD *)(result + 4 * v9 + 8);
        *(_DWORD *)(result + 4 * v5 + 8) = v57;
        *(_DWORD *)(result + 4 * v9 + 8) = v56;
        v58 = v71 == 1;
        v72 = v71 - 1;
        *(_BYTE *)(v8 + 5) = *(_BYTE *)(v7 + 5) ^ *(_BYTE *)(result + 4 * ((v56 + v57) & 0xFF) + 8);
        if ( v58 )
          break;
        v5 = (v5 + 1) & 0xFF;
        v59 = *(_DWORD *)(result + 4 * v5 + 8);
        v9 = (v59 + v9) & 0xFF;
        v60 = *(_DWORD *)(result + 4 * v9 + 8);
        *(_DWORD *)(result + 4 * v5 + 8) = v60;
        *(_DWORD *)(result + 4 * v9 + 8) = v59;
        v61 = v72 == 1;
        v73 = v72 - 1;
        *(_BYTE *)(v8 + 6) = *(_BYTE *)(v7 + 6) ^ *(_BYTE *)(result + 4 * ((v59 + v60) & 0xFF) + 8);
        if ( v61 )
          break;
        v5 = (v5 + 1) & 0xFF;
        v62 = *(_DWORD *)(result + 4 * v5 + 8);
        v9 = (v62 + v9) & 0xFF;
        v63 = *(_DWORD *)(result + 4 * v9 + 8);
        *(_DWORD *)(result + 4 * v5 + 8) = v63;
        *(_DWORD *)(result + 4 * v9 + 8) = v62;
        v64 = v73 == 1;
        v67 = v73 - 1;
        *(_BYTE *)v8 = *(_BYTE *)v7 ^ *(_BYTE *)(result + 4 * ((v62 + v63) & 0xFF) + 8);
      }
      while ( !v64 );
    }
  }
  *(_DWORD *)(result + 4) = v9;
  *(_DWORD *)result = v5;
  return result;
}
