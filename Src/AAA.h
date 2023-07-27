// AAA协议还原插件

#ifdef AAA_EXPORTS
#define PROCOTOL_EXTEND __declspec(dllexport)
#else
#define PROCOTOL_EXTEND __declspec(dllimport)
#endif


typedef struct STRUCT_AAABUFFER
{
public:
	FILE*		m_authfile;
	CString		m_DstDir;
	DWORD		m_aaanum;
}AAABUFFER;

class AAAProtocol : public DceProcotol
{
public:
	AAAProtocol();
	virtual	~AAAProtocol();

public:
//	AAABUFFER*			m_pAAABuf;
	LPCTSTR				m_SpyAddr;

public:
	virtual BOOL InitObject(DceMission* pMiss);
	virtual void ProcessBuffer(DATABUFFER& packetbuf, time_t& packtm);
//	virtual void RenameTmpDir(LPCTSTR strtm);	
	virtual BOOL WriteObjectState(time_t now, CString& strtxt);
	BOOL WriteThreadMacIndexFile(CString pcname,CString pcmac,CString pcip,CString pcostype,CString pcaccount);

private:
	void ProcessAAAPacket(NEWAAAPACKET* pAAAData);
	void WriteAuthFile(LPIpDataStruct pAAABuf, NEWAAAPACKET* pAAA, LPCTSTR pDDEPOS);
};