/***************  基类定义  **************/

#ifndef DCE_PUBLIC_CLASS_H__20050506__INCLUDED
#define DCE_PUBLIC_CLASS_H__20050506__INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif
#include "io.h"
//#include "dcepub.h"
#include <afxtempl.h>
#include "rbtempl.h"
#include <list>
using namespace std;

class AuthInfo
{
public:
	CString		m_UserAccount1;
	CString		m_Phone1;
	CString     m_userImei1;
	CString     m_userCelllac1;
	CString     m_userCellCi1;
	CString		m_UserAccount2;
	CString		m_Phone2;
	CString     m_userImei2;
	CString     m_userCelllac2;
	CString     m_userCellCi2;
	CString		m_vlanid;

	// 认证数据的类型(1:Radius, 2:CDMA, 3:VLan)
	UINT		m_AuthType;
	AuthInfo();
	~AuthInfo(){};

	// 获取认证信息
	int  GetAuthInfo(DATEHEADER& packh,		// 数据包的包头
					BOOL bCDMA = FALSE		// 该数据是否是CDMA
					);

	// 交换认证数据
	void SwapAuthInfo();

};

class DceMission;
//类型转义
//	IDX_UFORMAT		=	19,KEYTYPE	//类型：ID、password、
//	IDX_PFORMAT		=	20,DESCRIP	//描述，类型的说明


enum IdxHisRecord
{
	IDX_BEGTM		=	0,
	IDX_ENDTM		=	1,
	IDX_FORWTYPE	=	2,
	IDX_IP1			=	3,
	IDX_IP2			=	4,
	IDX_PORT1		=	5,
	IDX_PORT2		=	6,
	IDX_ADDR1		=	7,
	IDX_ADDR2		=	8,
	IDX_ACCOUNT1	=	9,
	IDX_PHONE1		=	10,
	IDX_ACCOUNT2	=	11,
	IDX_PHONE2		=	12,
	IDX_ID			=	13,
	IDX_CONTACT		=	14,
	IDX_PRO			=	15,
	IDX_SUBPRO		=	16,
	IDX_USER		=	17,
	IDX_PASS		=	18,
	IDX_UFORMAT		=	19,
	IDX_PFORMAT		=	20,
	IDX_UTYPE		=	21,
	IDX_PTYPE		=	22,
	IDX_URL			=	23,	
	IDX_DOMAIN		=	24,
	IDX_STATE		=	25,
	IDX_DATASRC		=	26,
	IDX_IFAGENT		=	27,
	IDX_CRCID		=	28,
	IDX_COUNT		=	29,
	IDX_SPYADDR		=   30,
	IDX_MAC1		=	31,
	IDX_MAC2		=	32,
	IDX_AccountType1=	33,
	IDX_AccountType2=	34,
	IDX_CLIENTMAC	=	35,

	IDX_MARK		=	36
};

//*************************************************
//	类名称:			网络活动历史数据临时存放和组合取出串处理类
//	类继承:			
//	描述:			
//	主要函数:		SetRecord
//					GetHisRecord
//	使用事项:		
//*************************************************/

typedef struct _WiFiRouterInfo
{
	CString Name;
	CString Lng;
	CString Lat;
}WiFiRouterInfo;

#include <map>
class  HistoryRecord
{
public:
	static map<CString ,WiFiRouterInfo * >s_mapWiFiRouter;
	static void LoadWiFiRouter();
	static WiFiRouterInfo *GetWiFiRouterInfo(const CString &WiFiRouterMac);
	HistoryRecord();
	static bool m_bWiFiRouter;
public:
	void SetRecord(int idx, const char *szValue);
	BOOL GetHisRecord(CString &tempstr);
	BOOL GetWebfigRecord(CString &tempstr);
	BOOL GetWebidRecord(CString &tempstr);
	BOOL GetAccount( CString &tempstr );

	CString GetField(int idx) { 
		if(idx < m_arrField.GetSize())
			return m_arrField[idx];
		
		return "";
	};

private:
	CStringArray m_arrField;
};





class TcpPacket
{
public:
	unsigned int	Ssequence;	//
	unsigned int	Sack;		//建立会话的sequence ack len
	unsigned int	Nextsequence;	//
	unsigned int	Slen;		//	
	time_t			tm;	
	char*			buf;
	TcpPacket() {memset(this, 0, sizeof(TcpPacket));}
	TcpPacket(DATEHEADER &datahead1,const char *databuf, DWORD seqoffset, DWORD ackoffset)
	{
		ASSERT(databuf != NULL);
		Ssequence = datahead1.m_sequence - seqoffset;
		Sack	  = datahead1.m_ack - ackoffset;
		Slen = datahead1.m_datalen;
		Nextsequence = Ssequence + Slen;
		tm = datahead1.m_tm;
		buf = new char[Slen+1];
		memcpy(buf, databuf, Slen);
		buf[Slen] = 0;
	}
	~TcpPacket() { delete[] buf; buf=NULL;}
};

inline BOOL AFXAPI operator<(const TcpPacket& s1, const TcpPacket& s2)
{
	DWORD dseq = (s1.Ssequence - s2.Ssequence);
	return dseq > 0x80000000;		// (s1.Ssequence < s2.Ssequence)
//	return (s1.Ssequence < s2.Ssequence);
}

// inline BOOL AFXAPI operator==(const TcpPacket& s1, const TcpPacket& s2)
// { return ((s1.Ssequence == s2.Ssequence) && (s1.Nextsequence == s2.Nextsequence)); }
// 
// inline BOOL AFXAPI operator==(const TcpPacket& s1, DATEHEADER &s2)
// { return ((s1.Ssequence == s2.sequence) && (s1.Nextsequence == s2.sequence + s2.datalen)); }
// 
// inline BOOL AFXAPI operator==(DATEHEADER & s1, const TcpPacket& s2)
// { return ((s2.Ssequence == s1.sequence) && (s2.Nextsequence == s1.sequence + s1.datalen)); }
// 
// inline BOOL AFXAPI operator!=(const TcpPacket& s1, const TcpPacket& s2)
// { return !(s1 == s2); }
// 
// inline BOOL AFXAPI operator!=(const TcpPacket& s1, DATEHEADER & s2)
// { return !(s1 == s2); }
// 
// inline BOOL AFXAPI operator!=(DATEHEADER & s1, const TcpPacket& s2)
// { return !(s1 == s2); }


// inline BOOL AFXAPI operator<(const TcpPacket& s1, DATEHEADER & s2)
// { return (s1.Ssequence < s2.sequence); }
// 
// inline BOOL AFXAPI operator<(DATEHEADER & s1, const TcpPacket& s2)
// { return (s1.sequence < s2.Ssequence); }
// 
// inline BOOL AFXAPI operator>(const TcpPacket& s1, const TcpPacket& s2)
// { return (s1.Ssequence > s2.Ssequence); }
// 
// inline BOOL AFXAPI operator>(const TcpPacket& s1, DATEHEADER & s2)
// { return (s1.Ssequence > s2.sequence); }
// 
// inline BOOL AFXAPI operator>(DATEHEADER & s1, const TcpPacket& s2)
// { return (s1.sequence > s2.Ssequence); }
// 
// inline BOOL AFXAPI operator<=(const TcpPacket& s1, const TcpPacket& s2)
// { return (s1.Ssequence <= s2.Ssequence); }
// 
// inline BOOL AFXAPI operator<=(const TcpPacket& s1, DATEHEADER & s2)
// { return (s1.Ssequence <= s2.sequence); }
// 
// inline BOOL AFXAPI operator<=(DATEHEADER & s1, const TcpPacket& s2)
// { return (s1.sequence <= s2.Ssequence); }
// 
// inline BOOL AFXAPI operator>=(const TcpPacket& s1, const TcpPacket& s2)
// { return (s1.Ssequence >= s2.Ssequence); }
// 
// inline BOOL AFXAPI operator>=(const TcpPacket& s1, DATEHEADER & s2)
// { return (s1.Ssequence >= s2.sequence); }
// 
// inline BOOL AFXAPI operator>=(DATEHEADER & s1, const TcpPacket& s2)
// { return (s1.sequence >= s2.Ssequence); }

// Define a template class list of int
typedef list<TcpPacket, allocator<TcpPacket> > TcpPacketList ;
//Define an iterator for template class list of ints
typedef TcpPacketList::iterator TcpPacketListIt ;


// BOOL SearchCaseForSession(SocketSession* s, BOOL opencaseflag, time_t nowtm);


class DTcpPacket
{
public:
	unsigned int	Ssequence;	//
	unsigned int	Sack;		//建立会话的sequence ack len
	unsigned int	Nextsequence;	//
	unsigned int	Slen;		//	
	time_t			tm;	
	time_t			tm1;	
	char*			buf;
	unsigned char	sip[4];			// 源IP
	unsigned char	dip[4];			// 目的IP
	unsigned short	sport;			//
	unsigned short	dport;			// 
	unsigned char	control;
	WORD	serial;			// 包序列号udp/audio udp第3 4字节
	WORD	packettype;			// 类型(2 tcp 3 udp
	DTcpPacket() 
	{
		memset(this, 0, sizeof(DTcpPacket));
	}
	DTcpPacket(DATEHEADER &datahead1,const char *databuf)
	{
		ASSERT(databuf != NULL);
		packettype=datahead1.m_type;
		Slen = datahead1.m_datalen;

		tm = datahead1.m_tm;
		tm1 = datahead1.m_tm1;
		buf = new char[Slen+1];
		memcpy(buf, databuf, Slen);
		buf[Slen] = 0;
		memcpy(sip,datahead1.m_sip,4);
		memcpy(dip,datahead1.m_dip,4);
		sport=datahead1.m_sport;
		dport=datahead1.m_dport;
		control=datahead1.m_control;
		if(datahead1.m_type==UDPPacket || datahead1.m_type==ProxyUDP)
		{
			serial=datahead1.m_ipid;
		}
		else if(datahead1.m_type==TCPPacket || datahead1.m_type==ProxyTCP)
		{
			Ssequence = datahead1.m_sequence ;
			Sack	  = datahead1.m_ack;
			Nextsequence = Ssequence + Slen;
		}
	}
	~DTcpPacket()	
	{
		if (buf)
			delete[] buf;
	}
};

inline BOOL AFXAPI operator< (const DTcpPacket& s1, const DTcpPacket& s2)
{
	//tcp
	if(s1.packettype==UDPPacket || s1.packettype==ProxyUDP)
	{

			if (s1.tm==s2.tm)
			{
				return s1.tm1<s2.tm1;
			}
			else
				return s1.tm<s2.tm;

	}
	else //if(s1.packettype==TCPPacket || s1.packettype==ProxyTCP)
	{
		if(memcmp(s1.sip,s2.sip,4)==0){
			LONG dif=0;
			dif=(LONG)(s1.Ssequence -s2.Ssequence);
			//分不同的条件比较序列大小
			
			if(dif>0)
				return FALSE;
			else if(dif==0)
				return FALSE;
			else 
				return TRUE;
		}
		else{
			LONG dif=0;
			dif=(LONG)(s1.Sack -s2.Ssequence);
			
			if(dif>0)
				return FALSE;
			else if(dif==0)
				return TRUE;
			else 
				return TRUE;
		}
	}
}


typedef list<DTcpPacket, allocator<DTcpPacket> > DTcpPacketList;
typedef DTcpPacketList::iterator DTcpPacketListIt ;

#define		DUMPHTTPPACKET		// 是否允许Dump Http的包

#ifdef		DUMPHTTPPACKET

#define		DUMPBUFFERSIZE		2*1024*1024
#define		DUMPFILESIZE		30*1024*1024
#define		DUMPFilECOUNT		60

class  DumpPacket
{
public:
	BOOL		m_bCanDump;
	DWORD		m_FileSerial;
	char*		m_buf;
	DWORD		m_datalen;
	CFile		m_file;
	DumpPacket();
	~DumpPacket();
	void DumpOnePacket(DATEHEADER& packh, const char* pPacket);
	void SaveBuffer();
	void InitDumpBuffer();
};

extern  DumpPacket	v_HttpDumpHttpPacket;



#endif	//	DUMPHTTPPACKET

#endif










