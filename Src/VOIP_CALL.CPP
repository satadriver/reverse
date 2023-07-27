
#include "StdAfx.h"
#include "VOIP_CALL.H"
#include <map>

std::map<CString,SVOIP_CALL_INFO *>g_mapVoipCall;

CString GetVoipCallKey(char *pro,DATEHEADER& packh)
{
	CString strKey;
	strKey.Format("%s-%03d.%03d.%03d.%03d", pro,
		packh.m_sip[0], packh.m_sip[1], packh.m_sip[2], packh.m_sip[3]);
	return strKey;
}

BOOL SetVoipCallInfo(CString &strVoipCallKey, SVOIP_CALL_INFO *pVoipCallInfo)
{
	if (g_mapVoipCall.size() > 10000)
	{
		for (std::map<CString,SVOIP_CALL_INFO *>::iterator iter2 = g_mapVoipCall.begin(); iter2 != g_mapVoipCall.end(); ++iter2)
		{
			if (iter2->second != NULL)
			{
				delete iter2->second;
			}
		}
		g_mapVoipCall.clear();
	}
	g_mapVoipCall.insert(std::make_pair(strVoipCallKey,pVoipCallInfo));
	return FALSE;
}

SVOIP_CALL_INFO *GetVoipCallInfo(const CString &strVoipKey)
{
	std::map<CString ,SVOIP_CALL_INFO * >::iterator iter = g_mapVoipCall.find(strVoipKey);
	if (iter != g_mapVoipCall.end())
	{
		return (SVOIP_CALL_INFO *)iter->second;
	}
	return NULL;

}