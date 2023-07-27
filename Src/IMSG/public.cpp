// public.cpp: implementation of the publicc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "public.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CString Utf82GB2312(LPCTSTR strMessage)
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

void FormatData(BYTE* pData, int datalen, CString& strData, CString& strASC)
{
	char* pDst = new char[3*datalen+500];
	char* pAsc = new char[2*datalen+500];
	memset(pDst, 0, 3*datalen+500);
	memset(pAsc, 0, 2*datalen+500);
	
	char* p = pDst;
	char* p1 = pAsc;
	int len=0;
	while(len<datalen)
	{
		sprintf(p, "%02X ", pData[len]);
		*p1 = pData[len]==0?0x20:pData[len];
		len++;
		p+=3;
		p1++;
		
		if((len%16)==0)
		{
			memcpy(p, "\r\n", 2);
			p+=2;
			
			memcpy(p1, "\r\n", 2);
			p1+=2;
		}
	}
	strData = pDst;
	strASC = pAsc;
	strASC = Utf82GB2312(strASC);
	delete[] pDst;
	delete[] pAsc;
	return;
}



BOOL GetPacketInfo(PACKETINFO& packinfo, const BYTE* pPacket, int len)
{
	if(len<42)
	{
/*		ASSERT(FALSE);*/
		return FALSE;
	}
	memset(packinfo.tagarr, 0, sizeof(packinfo.tagarr));
	packinfo.frametagnum = 0;
	memcpy(packinfo.mac, pPacket, 12);
	packinfo.datalen=len;
	// 判断帧类型
	for (int i=0; i<8; i++)
	{
		if (memcmp(pPacket+12, "\x81\x00", 2) == 0)
		{
			// VLan
			packinfo.frametagnum++;
			memcpy(&packinfo.tagarr[i].tagdata, pPacket+14, 2);		// vlanid
			packinfo.tagarr[i].tagtype = FRAMETYPE_VLAN;
			pPacket+=4;
			packinfo.datalen -= 4;
			len -= 4;
			if (len < 42)
			{
				return FALSE;
			}
		}
		else
		{
			break;
		}
	}
	if ((memcmp(pPacket+12, "\x88\x64", 2) == 0)
		&& (packinfo.frametagnum<8))
	{
		// PPPoE
		if (memcmp(pPacket+20, "\x00\x21", 2) != 0)		// pppoe所携带的不是IP包
			return FALSE;		// 目前不需要处理

		packinfo.tagarr[packinfo.frametagnum].tagtype = FRAMETYPE_PPPOE;
		memcpy(&packinfo.tagarr[packinfo.frametagnum].tagdata, pPacket+16, 2);		// PPPoE SessionID
		memcpy((char*)pPacket+20, "\x08\x00", 2);
		packinfo.frametagnum++;
		pPacket += 8;
		packinfo.datalen -= 8;
		len -= 8;
	}
	if (len <= 14)
		return FALSE;

	if (memcmp(pPacket+12, "\x08\x00", 2) == 0)
	{
		if (len < 42)
			return FALSE;

		pPacket+=14;
		len -= 14;
		
IPHEADER:
		if ((0x40==(*pPacket&0xF0)) &&					// ipv4
			(pPacket[9]==6 || pPacket[9]==17))
		{
			packinfo.pro = pPacket[9];
			packinfo.datalen = ntohs(*((WORD*)(pPacket+2)));
			if (len < packinfo.datalen)
				return FALSE;
			packinfo.iphlen = (*pPacket&0x0F)*4;
			memcpy(packinfo.sip, pPacket+12, 4);
			memcpy(packinfo.dip, pPacket+16, 4);
			len-=packinfo.iphlen;
			if (len < 4)
			{
				return FALSE;
			}
			pPacket+=packinfo.iphlen;
			memcpy(&packinfo.sport, pPacket, 2);
			memcpy(&packinfo.dport, pPacket+2, 2);
			if (packinfo.pro == 6)
			{ // tcp
				if (len < 14)
				{
					return FALSE;
				}
				packinfo.seq = ntohl(*((DWORD*)(pPacket+4)));
				packinfo.ack = ntohl(*((DWORD*)(pPacket+8)));
				packinfo.tcphlen = (pPacket[12]>>4)*4;
				packinfo.datalen -= packinfo.tcphlen+packinfo.iphlen;
				packinfo.flag = pPacket[13];
				packinfo.winsize = ntohs(*((WORD*)(pPacket+14)));
				if (len < packinfo.tcphlen)
				{
					return FALSE;
				}
				if (packinfo.tcphlen>20)
					memcpy(packinfo.tcpoperation, pPacket+20, packinfo.tcphlen-20);
				memcpy(packinfo.pData,(BYTE*)pPacket+packinfo.tcphlen, packinfo.datalen);
				return TRUE;
			}
			else // packinfo.pro == 17
			{ // UDP
				if(packinfo.sport==0xA506 && packinfo.dport==0xA506)	//l2tp协议
				{
					if(len<50)
						return FALSE;
					pPacket	+=	16;
					len		-=	16;
					if(memcmp(pPacket, "\x00\x21", 2) == 0)	//IP
					{
						pPacket+=2;
						len-=2;
						goto IPHEADER;
					}
					else
						return FALSE;
				}

				packinfo.tcphlen = 8;
				packinfo.datalen -= packinfo.tcphlen+packinfo.iphlen;
				memcpy(packinfo.pData, (BYTE*)pPacket+packinfo.tcphlen, packinfo.datalen);
				return TRUE;
			}
		}
	}
	return FALSE;
}

//查找字符串出现位置
int mystrstr(char srcstr[],char deststr[],int len)
{

	char* sp;
	sp = srcstr;

	for(int i=0; i<len-strlen(deststr); i++)
	{
 		if(memcmp(sp+i,deststr,strlen(deststr))==0)
			return i;
	}

	return -1;
}