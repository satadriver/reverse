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
	//��ȡpcanywhere���ʿͻ�������
	CString clientname;
	clientname = CString(pPacket+56);
	
	//д�����ļ�
	CString txt;
	txt.Format("state=��½\r\nuser=%s\r\n",clientname);
	
	HttpSession tmpsession(packh, g_httpFrom);
	tmpsession.m_dceprocotol=httpprotocl;
	
	tmpsession.WritePcanyWhereFile(txt);

	return TRUE;
}
