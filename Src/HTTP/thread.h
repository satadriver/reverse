// thread.h: interface for the thread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THREAD_H__A53D30EC_2354_4C44_BA2B_3AD30ECBBB86__INCLUDED_)
#define AFX_THREAD_H__A53D30EC_2354_4C44_BA2B_3AD30ECBBB86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct
{
	CString mac;
	CString ip;
	CString wifissid;
	CString pcname;
}THREADSTRUCT;

CString thunder_GetItemValue(CString str);
CString thunder_GetSystemName(CString str);

BOOL thunder_DealThunderLogin(HttpSession& s,LPCTSTR pPacket);
BOOL thunder_DealThunderSend(HttpSession& s);
BOOL thunder_ClientSplit(CString posturl,THREADSTRUCT& threads);
BOOL thunder_GetDevInfo(HttpSession& s,LPCTSTR pPacket);
BOOL thunder_SplitUrl(CString urlstr,CStringArray &urlArray);
BOOL WriteThreadMacIndexFile(CString pcname,CString pcmac,CString pcip,CString pcostype,CString pcaccount);
std::string thunder_UrlDecode(const std::string& src);

inline BOOL isThreadPacket(HttpSession& s)
{
		if (s.m_Requestline.m_URL.Find("/download/create?") !=-1 && s.m_Requestline.m_Action == HTTP_POST)
		{
			return TRUE;
		}
		return FALSE;
}

inline BOOL isThreadLoginPacket(HttpSession& s)
{

		if (s.m_Requestline.m_Host.Find("login3.reg2t.sandai.net") !=-1 && s.m_Requestline.m_Action == HTTP_POST)
		{
			return TRUE;
		}
		return FALSE;
}

// 可以获取内容：机器MAC地址/操作系统版本/操作系统位数
inline BOOL isthunderpacket(HttpSession *s)
{
	if(s->m_Requestline.m_URL.Find("/?u=pusherv2")!=-1 && 
		s->m_Requestline.m_Host.Find("kkpgv3.xunlei.com")!=-1 &&
		(s->m_Requestline.m_URL.Find("u6=32")!=-1 || s->m_Requestline.m_URL.Find("u6=64")!=-1))
	{
		return TRUE;
	}
	return FALSE;
}

#endif // !defined(AFX_THREAD_H__A53D30EC_2354_4C44_BA2B_3AD30ECBBB86__INCLUDED_)
