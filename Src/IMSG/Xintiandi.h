// Xintiandi.h: interface for the CXintiandi class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XINTIANDI_H__9B3B69A8_B348_48EA_AC70_D121AA2E51B5__INCLUDED_)
#define AFX_XINTIANDI_H__9B3B69A8_B348_48EA_AC70_D121AA2E51B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXintiandi : public SocketSession 
{
public:
	CXintiandi(DATEHEADER& packh, int aT);
	virtual ~CXintiandi();

	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);

private:
	int DecodeLogin(char buf[0x50]);
};

inline BOOL IsxtdPacket(DATEHEADER& packh, const char* pPacket)
{
	if(packh.m_datalen != 80)
		return FALSE;
	if(packh.m_dport != 6207)
		return FALSE;
// 	if(memcmp(pPacket+14, "\x0\x4", 2) != 0)
// 		return FALSE;
// 	if(memcmp(pPacket+20, "\x0\x21", 2) != 0)
// 		return FALSE;
	
	return TRUE;
}
#endif // !defined(AFX_XINTIANDI_H__9B3B69A8_B348_48EA_AC70_D121AA2E51B5__INCLUDED_)
