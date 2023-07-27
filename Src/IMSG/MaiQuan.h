// MaiQuan.h: interface for the CMaiQuan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAIQUAN_H__ACDB89CB_E007_4A90_9EBA_7AF1E71494D2__INCLUDED_)
#define AFX_MAIQUAN_H__ACDB89CB_E007_4A90_9EBA_7AF1E71494D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMaiQuan  : public SocketSession
{
public:
	CMaiQuan(DATEHEADER& packh, int aT);
	virtual ~CMaiQuan();

	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);
};

inline BOOL IsMQPacket(DATEHEADER& packh, const char* pPacket)
{
	if(packh.m_datalen < 100)
		return FALSE;
	if(memcmp(pPacket, "\x80\x27", 2) != 0)
		return FALSE;
	if(memcmp(pPacket+14, "\x0\x4", 2) != 0)
		return FALSE;
	if(memcmp(pPacket+20, "\x0\x21", 2) != 0)
		return FALSE;

	return TRUE;
}

#endif // !defined(AFX_MAIQUAN_H__ACDB89CB_E007_4A90_9EBA_7AF1E71494D2__INCLUDED_)
