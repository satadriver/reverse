// pcanywhere.cpp: implementation of the pcanywhere class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "pcanywhere.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL Pcanywhere_DealPacket(const char* pPacket, DATEHEADER& packh,HttpProtocol* httpprotocl)
{
	//获取pcanywhere访问客户端名称
	CString clientname;
	clientname = CString(pPacket+56);
	
	//写对照文件
	CString txt;
	txt.Format("state=登陆\r\nuser=%s\r\n",clientname);
	
	HttpSession tmpsession(packh, g_httpFrom);
	tmpsession.m_dceprocotol=httpprotocl;
	
	tmpsession.WritePcanyWhereFile(txt);

	return TRUE;
}
