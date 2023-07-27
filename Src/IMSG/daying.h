// QQTranFile.h: interface for the QQTranFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Daying_H__453996A7_AB78_498D_B9A7_6840ADF0444D__INCLUDED_)
#define AFX_Daying_H__453996A7_AB78_498D_B9A7_6840ADF0444D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
 
#include "..\\SocketSession.h"

class DayingSession : public SocketSession
{
public:
	DayingSession(DATEHEADER& packh, int aT);
	virtual ~DayingSession();

	//*************************************************
	//	��������:		BfSession::InsertPacket
	//	����:			����һ���Ự��һ����
	//	�������:		packh �ǰ�ͷ�ṹ
	//					pPacket �ǰ�����
	//	�������:		
	//	����ֵ:			
	//	����:			
	//**************************************************
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);

protected:
	void WriteDayingIndexFile();
protected:

	CString m_tranfilename;
	unsigned char	m_firstsip[4];
	BOOL m_iswrite;
};



inline int IsDayingPacket(DATEHEADER& packh, const char* pPacket)
{
	if (packh.m_type ==TCPPacket)
	{
		if(packh.m_datalen==48
			&& memcmp(pPacket,"\x00\x01\x00\x01\x00\x00\x00\x30",8)==0 
			&&  memcmp(pPacket+0x1a,"\x00\x00",2)==0)
		{
			return 1;
		}
	}

	return 0;
}


#endif // !defined(AFX_QQTRANFILE_H__453996A7_AB78_498D_B9A7_6840ADF0444D__INCLUDED_)
