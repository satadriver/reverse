// StreamRTSP.h: interface for the CStreamRTSP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STREAMRTSP_H__ECF0D80A_4ABC_450A_AFAA_1873A6B925D9__INCLUDED_)
#define AFX_STREAMRTSP_H__ECF0D80A_4ABC_450A_AFAA_1873A6B925D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class RtspSession : public MediaSession
{
public:
	RtspSession(DATEHEADER& packh, int aT);
	virtual ~RtspSession() { }
	virtual int InsertPacket(DATEHEADER& dataheader,const char *pPacket);
	virtual BOOL CloseSession(time_t nowtm) {return TRUE;}
	virtual int ListToFile(){return 0;}
public:
	CString	m_realname;
	CString m_codeType;		// 文件名编码规则
	CString m_language;
	DWORD	m_opHappen;		// 发生的动作, 1,DESCRIBE 2,Setup, 4,RTSP 200OK
};

// 判断是否为RTSP会话包
inline int IsRTSPPacket(DATEHEADER& packh, const char* pPacket)
{
	if (packh.m_type == TCPPacket)
	{ 
		if( (memcmp(pPacket, "DESCRIBE", 8) == 0) ||
			(memcmp(pPacket, "SETUP", 5) == 0)  )
		{
			return 1;
		}
		else if (memcmp(pPacket, "RTSP/1.", 7) == 0)
		{
			return 2;
		}
	}
	return 0;
}
#endif // !defined(AFX_STREAMRTSP_H__ECF0D80A_4ABC_450A_AFAA_1873A6B925D9__INCLUDED_)
