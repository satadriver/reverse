/*************  数据类型定义  ************/

#ifndef DCE_PUBLIC_TYPE_H__20050506__INCLUDED
#define DCE_PUBLIC_TYPE_H__20050506__INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif
#include "winsock2.h"
#include <afxtempl.h>
#include <list>

typedef struct STRUCT_DCETESPREAD
{
	struct sockaddr		dce_addr;
	SOCKET				dceradius;
}DceTeSpread;

typedef struct STRUCT_DCEARRAY
{
public:
	int count;
	DceTeSpread dceTe[200];
	STRUCT_DCEARRAY(){
		memset(this, 0, sizeof(STRUCT_DCEARRAY));
	}
}DCETEARRAY;



#define		MAX_BLOCK_SIZE		52*1024




typedef struct IPTRACE					// 全网ip数据结构(仅用于Allnet协议)(该数据结构需和dde配合改动)
{
	unsigned char packettype;			// 05
	unsigned char protocol;
	unsigned long sip;
	unsigned short sport;
	unsigned long dip;
	unsigned short dport;
	unsigned long packetnum;
	unsigned long packetbyte;
	unsigned long stm;
	unsigned long etm;
}sIpTrace;

#define MaxTelNo            60
#define MaxUserName         40
#define MaxSessionId        40



// IMSG的HTTP代理处理
typedef struct _IMSGPROXY_
{
	char*	pPacketData;
	int		datalen;
	int		Direction;
	time_t	packettm;
	int		proxytype;
	DWORD	vlanid;
	LPCTSTR ImsgType;
	CString sip;
	CString dip;
	unsigned char	csip[4]		;	//net格式ip
	unsigned char	cdip[4]		;	//net格式ip
	CString smac;
	CString dmac;
	CString userid;
	CString linkman;
//	CString msgtext;
	BOOL	bReaord;
	DWORD	m_dataSrc;
	char*	pmsgbuf;
	int		msglen;
}IMSGPROXY, *LPIMSGPROXY;

// IMSG的HTTP代理的函数指针定义
//typedef DceProcotol* (WINAPI *CreateObjectFunction)(void);
//typedef BOOL (WINAPI *PROCESSIMSGPROXYDATA) (IMSGPROXY*);

//typedef struct IMSGPROXYFUNCTION
//{
//	CHECKIMSGPROXY		checkfun;
////	PROCESSIMSGPROXYDATA processfun;
//}IMSGPROXYFUNCTION;

extern HANDLE MYHEAP;

// struct CPlex     // warning variable length structure
// {
// 	CPlex* pNext;
// #if (_AFX_PACKING >= 8)
// 	DWORD dwReserved[1];    // align on 8 byte boundary
// #endif
// 	// BYTE data[maxNum*elementSize];
// 	
// 	void* data() { return this+1; }
// 	
// 	static CPlex* Create(CPlex*& pHead, unsigned int nMax, unsigned int cbElement)
// 	{
// 		ASSERT(nMax > 0 && cbElement > 0);
// 		CPlex* p;
// 		if (MYHEAP==NULL)
// 			p = (CPlex*) new unsigned char[sizeof(CPlex) + nMax * cbElement];
// 		else
// 			p = (CPlex*)HeapAlloc(MYHEAP, 0, sizeof(CPlex) + nMax * cbElement);
// 		// may throw exception
// 		p->pNext = pHead;
// 		pHead = p;  // change head (adds in reverse order for simplicity)
// 		return p;
// 	}
// 	
// 	void CPlex::FreeDataChain()     // free this one and links
// 	{
// 		CPlex* p = this;
// // 		if (p!=NULL)
// // 			p = p->pNext;
// 		while (p != NULL)
// 		{
// 			BYTE* bytes = (BYTE*) p;
// 			CPlex* pNext = p->pNext;
// 			if (MYHEAP==NULL)
// 				delete[] bytes;
// 			else
// 				HeapFree(MYHEAP, 0, bytes);
// 			p = pNext;
// 		}
// 	}
// };

// 程序调试用的结构体
//#if defined(DCEPUB_EXPORTS) || defined(ADD_RUNSTATE_INFO)
#ifdef	ADD_RUNSTATE_INFO
	#define		DebugBufSize	400
	#define		WriteState		0xAA
	typedef struct RUNDEBUG
	{
	public:
		long		writeflg;
		WORD		pos;
		WORD		count;
		DWORD		lasttm;
		long		buf[DebugBufSize];		// 
		DWORD		mstm[DebugBufSize];		// 
	public:
		RUNDEBUG()
		{
			memset(this, 0, sizeof(RUNDEBUG));
			writeflg = WriteState;
		}
		void AddState(long ste, BOOL forcewrite = FALSE)
		{
			DWORD now = GetTickCount();
			if ((writeflg == WriteState) &&
				((now != lasttm) || (forcewrite)))
			{
				lasttm = now;
				if (pos >= DebugBufSize)
					pos = 0;
				buf[pos] = ste;
				mstm[pos] = now;
				pos++;
				if (count < DebugBufSize)
					count++;
			}
		}
		void ClearState()
		{
			if (writeflg == WriteState)
			{
				lasttm = 0;
				pos = count = 0;
				AddState(0, TRUE);
			}
			
		}
		CString DumpState()
		{
			RUNDEBUG tmp;
			InterlockedExchange(&writeflg, 0);
			::Sleep(1);
			memcpy(&tmp, this, sizeof(tmp));
			memset(this, 0, sizeof(tmp));
			InterlockedExchange(&writeflg, WriteState);
			CString str;
			int tmpc;
			WORD pos;
			if (tmp.count < DebugBufSize)
			{
				tmpc = tmp.count;
				pos = 0;
			}
			else
			{
				tmpc = DebugBufSize;
				pos = tmp.pos;
			}
			char tmpstr[2048];
			int len = 0;
			while (tmpc-->0)
			{
				if (pos >= DebugBufSize)
					pos = 0;
				len += sprintf(tmpstr+len, "%-15u%d\r\n", tmp.mstm[pos], tmp.buf[pos]);
				pos++;
				if (len > 2000)
				{
					len = 0;
					str.Format("%s%s", (CString)str, tmpstr);
				}
			}
			if (len > 0)
			{
				str.Format("%s%s", (CString)str, tmpstr);
			}
			return str;
		}
	}RUNDEBUG;

	typedef	struct AutoDestory
	{
	public:
		RUNDEBUG* pRunDebug;
		long		EndStatID;
	public:
		AutoDestory(RUNDEBUG* ptmpRunDebug, long state, long endstat)
		{
			ASSERT(ptmpRunDebug != NULL);
			pRunDebug = ptmpRunDebug;
			EndStatID = endstat;
			pRunDebug->AddState(state, TRUE);
		}
		~AutoDestory()
		{
			ASSERT(pRunDebug != NULL);
			if (pRunDebug != NULL)
				pRunDebug->AddState(EndStatID, TRUE);
		}
	}AutoDestory;
#endif

// #define		HISTORY_DATA		1
// #define		SEARCHKEY_DATA		2
// #define		INDEXKEY_DATA		3
// #define		WEBFIG_DATA			4
// #define		AGENT_DTAT			5
#define     QQPASS_DATA			 6
// #define     WARNALARM_DATA      7   
#define     TAOBAOPASS_DATA		 8
//#define     ERROR_DATA			9
#define		WEBID_DATA			10
#define		QQCRACK_DATA		11
#define		HTTPREGRIST_DATA	12
#define		UNIP_DATA			13
#define		ATTFILE_DATA		14

#define		CCIC_ALARM			15
#define		KEYOBJ_ALARM		16
#define		KEYOBJ_ONLINE		17
// 唯一值, 360,qq
#define		UNIMID_DATA		    18
#define		QQGCHAT_DATA		19

//
#define		REACTIONARY_ALARM	20


#define		HTTP_PROTOCOL		0
#define		POP3_PROTOCOL		1
#define		MEDIA_PROTOCOL		2
#define		IMSG_PROTOCOL		3
#define		SMTP_PROTOCOL		4
#define		FTP_PROTOCOL		5
#define		TELNET_PROTOCOL		5
#define		P2P_PROTOCOL		5
#define		UNIP_PROTOCOL		6
#define		AAA_PROTOCOL		7
#define		OTHER_PROTOCOL		8

#define		TOTALPRONUM			10

#define		PRONUM_MAX			4
#define		PRO_HTTPS			0
#define		PRO_SQL				1
#define		PRO_OICQ			2
#define		PRO_MSN				3

class STAT
{
public:
	long			m_writeflag;
// 	CStringArray	m_hisdataarr[2];		// 活动历史
//	CStringArray	m_keydataarr[2];		// 引擎关键字
//	CStringArray	m_prekeydataarr[2];		// 预设关键字
// 	CStringArray	m_webfigurearr[2];		// 虚拟身份
	CStringArray	m_webidarr[2];			// 虚拟身份
	CMapStringToString	m_qqcrackarr[2];	
	CMapStringToString	m_keyobjonlinearr[2];	

	CStringArray    m_httpregistarr[2];
	CStringArray    m_uniparr[2];
	CStringArray    m_attfilearr[2];
	CStringArray    m_keyobjarr[2];
	CStringArray    m_ccicarr[2];
	CStringArray    m_uniqmidarr[2];
	CStringArray    m_qqgchatarr[2];

//	CStringArray	m_agentdataarr[2];		// 代理
	CStringArray    m_qqpassdataarr[2];
//	CStringArray	m_warnalarmdataarr[2];
	CStringArray    m_taobaopassdataarr[2];
//	CStringArray	m_errorqqdataarr[2];
//	CStringArray	m_errordataarr[2];
	CStringArray    m_saReactionary[2];
public:
	STAT()
	{
		m_writeflag = 0;
		for (int i=0; i<2; i++)
		{
// 			m_hisdataarr[i].SetSize(0, 1024);
//			m_keydataarr[i].SetSize(0, 128);	
//			m_prekeydataarr[i].SetSize(0, 128);	
			m_webidarr[i].SetSize(0, 128);	
			m_qqcrackarr[i].InitHashTable(128);	
// 			m_webfigurearr[i].SetSize(0, 128);	
//			m_agentdataarr[i].SetSize(0, 128);
			m_qqpassdataarr[i].SetSize(0,128);
//			m_warnalarmdataarr[i].SetSize(0,128);
			m_taobaopassdataarr[i].SetSize(0,128);
//			m_errordataarr[i].SetSize(0,128);

		}
	}
};

typedef struct STRUCT_HISTABLE
{
	CString id;					//ID 
	CString pro;				//协议
	CString subpro;				//子协议
	CString cip;				//客户端ip
	CString sip;				//服务器端ip
	CString account;			//帐号
	CString phone;				//电话
}HISTABLE;


// typedef	CMap<CString, LPCTSTR, IpDataStruct, IpDataStruct&> DseIpInfoMap;

typedef struct SocketInfo
{
	CString			sip			;	//134.035.006.001:25
	CString			dip			;
	CString			smac		;
	CString			dmac		;
}SocketInfo;


#endif