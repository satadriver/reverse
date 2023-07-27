

#ifndef VOIP_CALL_H_H_H
#define VOIP_CALL_H_H_H

typedef struct _SVOIP_CALL_INFO
{
	CString strUser;
	CString strPass;
	CString strFrom;
	CString strTo;
}SVOIP_CALL_INFO, *PSVOIP_CALL_INFO; 


extern std::map<CString,SVOIP_CALL_INFO *>g_mapVoipCall;

CString GetVoipCallKey(char *pro,DATEHEADER& packh);

BOOL SetVoipCallInfo(CString &strVoipCallKey, SVOIP_CALL_INFO *pVoipCallInfo);

SVOIP_CALL_INFO *GetVoipCallInfo(const CString &strVoipCallKey);

void CleadVoipCallInfo();

#endif