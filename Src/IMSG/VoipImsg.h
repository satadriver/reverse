

#ifndef VOIP_IMSG_H_H_H
#define VOIP_IMSG_H_H_H

#define VOIP_LOGIN 1
#define VOIP_CALL 2
#define VOIP_PHONE_LOGIN 3
#define VOIP_PHONE_CALL 4
#define VOIP_MSG 5
#define VOIP_PHONE_MSG 6
#define VOIP_REG 7
#define VOIP_PHONE_REG 8
#define VOIP_FRIEND 9
#define VOIP_PHONE_FRIEND 9



class ETphoneNumSession : public SocketSession
{
public:
	ETphoneNumSession(DATEHEADER& packh, int aT);
	virtual ~ETphoneNumSession();
	static int IsETphoneNumPacket(DATEHEADER& packh, const char* pPacket);
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);	
protected:
	void WriteETphoneNumIndexFile();
protected:
	CString m_state;
	CString m_tranfilename;
	unsigned char	m_firstsip[4];
	BOOL m_iswrite;
};





class VoipFEIYINImsgSession : public SocketSession
{
public:
	VoipFEIYINImsgSession(DATEHEADER& packh, int aT);
	virtual ~VoipFEIYINImsgSession();
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);
	static BOOL IsVoipFEIYINImsg(DATEHEADER& packh, const char* pPacket)
	{
		if( (packh.m_type ==TCPPacket) && (packh.m_dport == 8080) )
		{	
			if(memcmp(pPacket,"GET /minisvr/service/balance.action?",strlen("GET /minisvr/service/balance.action?")) == 0) 
			{
				return VOIP_CALL;
			}
		}
		return FALSE;
	}
protected:
	void WriteFEIYINIndexFile();
protected:
	CString m_tranfilename;
	unsigned char	m_firstsip[4];
	BOOL m_iswrite;
};



class VoipSKYImsgSession : public SocketSession
{
public:
	VoipSKYImsgSession(DATEHEADER& packh, int aT);
	virtual ~VoipSKYImsgSession();
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);
	static int IsVoipSKYImsg(DATEHEADER& packh, const char* pPacket)
	{
		if( (packh.m_type ==TCPPacket) && (packh.m_dport == 8080) )
		{
			if(memcmp(pPacket,"GET /minisvr/service/balance.action?",strlen("GET /minisvr/service/balance.action?")) == 0 ) 
			{
				return VOIP_CALL;
			}
			else if(memcmp(pPacket,"POST /minisvr/service/miniclt.action",strlen("POST /minisvr/service/miniclt.action")) == 0 ) 
			{
				return VOIP_LOGIN;
			}
		}
		return 0;
	}
protected:
	void WriteSKYIndexFile();
protected:
	CString m_state;
	CString m_tranfilename;
	unsigned char	m_firstsip[4];
	BOOL m_iswrite;
};



class VoipTUXINImsgSession : public SocketSession
{
public:
	VoipTUXINImsgSession(DATEHEADER& packh, int aT);
	virtual ~VoipTUXINImsgSession();
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);
	static BOOL IsVoipTUXINImsg(DATEHEADER& packh, const char* pPacket)
	{
		if( (packh.m_type ==TCPPacket) && (packh.m_dport == 8080)  )
		{
			if(memcmp(pPacket,"GET /minisrv/service/balance.action?",strlen("GET /minisrv/service/balance.action?")) == 0 ) 
			{
				return VOIP_CALL;
			}
		}
		return FALSE;
	}
protected:
	void WriteTUXINIndexFile();
protected:
	CString m_tranfilename;
	unsigned char	m_firstsip[4];
	BOOL m_iswrite;
};




class VoipKCImsgSession : public SocketSession
{
public:
	VoipKCImsgSession(DATEHEADER& packh, int aT);
	virtual ~VoipKCImsgSession();
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);
	static int IsVoipKCImsg(DATEHEADER& packh, const char* pPacket)
	{
		if( (packh.m_type ==TCPPacket) && ( (packh.m_dport == 2001) || (packh.m_sport == 2001) ) )	//server port == 2001
		{
			if (memcmp(pPacket,"HTTP/1.1 200 OK",strlen("HTTP/1.1 200 OK") ) == 0 ) 
			{
				return VOIP_CALL;
			}
			else if (memcmp(pPacket,"GET /1.0/kc/config/update?",strlen("GET /1.0/kc/config/update?") ) == 0 ) 
			{
				return VOIP_PHONE_CALL;
			}
		}
		return 0;
	}
protected:
	void WriteKCIndexFile();
protected:
	CString m_tranfilename;
	unsigned char	m_firstsip[4];
	BOOL m_iswrite;
};







class VoipYLTImsgSession : public SocketSession
{
public:
	VoipYLTImsgSession(DATEHEADER& packh, int aT);
	virtual ~VoipYLTImsgSession();
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);
	static int IsVoipYLTImsg(DATEHEADER& packh, const char* pPacket)
	{
		if( (packh.m_type ==TCPPacket) && (packh.m_dport == 8080)  )
		{
			if(memcmp(pPacket + 4,"/minisvr/service/miniclt.action",strlen("/minisvr/service/miniclt.action") ) == 0)
			{
				return VOIP_CALL;
			}
		}
		return 0;
	}
	
protected:
	void WriteYLTIndexFile();
protected:
	CString m_tranfilename;
	unsigned char	m_firstsip[4];
	BOOL m_iswrite;
};




class VoipWEIWEIImsgSession : public SocketSession
{
public:
	VoipWEIWEIImsgSession(DATEHEADER& packh, int aT);
	virtual ~VoipWEIWEIImsgSession();
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);
	static int IsVoipWEIWEIImsg(DATEHEADER& packh, const char* pPacket)
	{
		if( (packh.m_type ==TCPPacket) && (packh.m_dport == 5222)  )
		{
			if( (memcmp(pPacket,"<iq from=",strlen("<iq from=")) == 0) || 
				(memcmp(pPacket,"<iq to=",strlen("<iq to=")) == 0) )
			{
				return VOIP_CALL;
			}
		}
		else if( (packh.m_type ==TCPPacket) && ( packh.m_sport == 5222 ) )
		{
			if( (memcmp(pPacket,"From: <sip:",strlen("From: <sip:")) == 0) || 
				(memcmp(pPacket,"To: <sip:",strlen("To: <sip:")) == 0) )
			{
				return VOIP_CALL;
			}
		}
// 		else if(memcmp(pPacket,"GET /cloud/pc/getaccount.aspx?",strlen("GET /cloud/pc/getaccount.aspx?") ) == 0)
// 		{
// 			return VOIP_CALL;
// 		}
		return FALSE;
	}
protected:
	void WriteWEIWEIIndexFile();
protected:
	CString m_tranfilename;
	unsigned char	m_firstsip[4];
	BOOL m_iswrite;
};




class VoipUxinImsgSession : public SocketSession
{
public:
	VoipUxinImsgSession(DATEHEADER& packh, int aT);
	virtual ~VoipUxinImsgSession();
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);
	static BOOL IsVoipUxinImsg(DATEHEADER& packh, const char* pPacket)
	{
		if( (packh.m_type ==TCPPacket) && (packh.m_dport == 8887) )
		{	
			if( (memcmp(pPacket,"GET /login?account=",strlen("GET /login?account=")) == 0) &&
				(strstr(pPacket,"uxin.keicei.com") )  )
			{
				return VOIP_LOGIN;
			}
			
			if( (memcmp(pPacket,"GET /emodel_report?",strlen("GET /emodel_report?")) == 0) &&
				(strstr(pPacket,"uxin.keicei.com")) )
			{
				return VOIP_CALL;
			}
			if( (memcmp(pPacket,"GET /v2/emodel_report?",strlen("GET /v2/emodel_report?")) == 0) &&
				(strstr(pPacket,"conn.uxin.com")) )
			{
				return VOIP_PHONE_CALL;
			}			
		}
		return FALSE;
	}
protected:
	void WriteUxinIndexFile();
protected:
	CString m_tranfilename;
	unsigned char	m_firstsip[4];
	BOOL m_iswrite;
};





class VoipAdlImsgSession : public SocketSession
{
public:
	VoipAdlImsgSession(DATEHEADER& packh, int aT);
	virtual ~VoipAdlImsgSession();
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);
	static BOOL IsVoipAdlImsg(DATEHEADER& packh, const char* pPacket)
	{
		if( (packh.m_type ==TCPPacket) && (packh.m_dport == 780) )
		{	
			if( (memcmp(pPacket,"GET /httpservice?",strlen("GET /httpservice?")) == 0) &&
				(strstr(pPacket,"Host: pes.idingling.com:780\r\n") )  )
			{
				return VOIP_CALL;
			}		
		}
		return FALSE;
	}
protected:
	void WriteAdlIndexFile();
protected:
	CString m_tranfilename;
	unsigned char	m_firstsip[4];
	BOOL m_iswrite;
};





#endif
