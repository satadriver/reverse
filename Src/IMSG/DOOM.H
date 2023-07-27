
#ifndef DOOM_H_H_H
#define DOOM_H_H_H

#include "..\\SocketSession.h"

class TrojanSession : public SocketSession
{
public:
	TrojanSession(DATEHEADER& packh, int aT);
	virtual ~TrojanSession();
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);
protected:
	void WriteTrojanIndexFile();

	CString m_strPro;
	CString m_strTxt;

	CString m_strContent;
	CString m_strAction;

	CString m_strWarn;
};

//BOER
class TrojanBoerSession : public TrojanSession
{
public:
	static int  IsBoerPacket(DATEHEADER& packh, const char * pPacket);
	virtual ~TrojanBoerSession();
	TrojanBoerSession(DATEHEADER& packh, int aT);
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);	

};


class TrojanZxshellSession : public TrojanSession
{
public:
	static int IsZxshellPacket(DATEHEADER& packh, const char * pPacket);
	virtual ~TrojanZxshellSession();
	TrojanZxshellSession(DATEHEADER& packh, int aT);
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);	

};

class TrojanXengineSession : public TrojanSession
{
public:
	static int IsXenginePacket(DATEHEADER& packh, const char* pPacket);
	virtual ~TrojanXengineSession();
	TrojanXengineSession(DATEHEADER& packh,int aT);
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);	

};

class TrojanSunShadowSession : public TrojanSession
{
public:
	static int IsSunShadowPacket(DATEHEADER& packh, const char* pPacket);
	virtual ~TrojanSunShadowSession();
	TrojanSunShadowSession(DATEHEADER& packh,int aT);
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);	
};

//
class TrojanHavratSession : public TrojanSession
{
public:
	static int IsHavratPacket(DATEHEADER& packh, const char* pPacket);
	virtual ~TrojanHavratSession();
	TrojanHavratSession(DATEHEADER& packh,int aT);
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);	

};

//≤ ∫Á«≈
class TrojanRainbowBridgeSession : public TrojanSession
{
public:
	static int IsRainbowBridgePacket(DATEHEADER& packh, const char* pPacket);
	virtual ~TrojanRainbowBridgeSession();
	TrojanRainbowBridgeSession(DATEHEADER& packh,int aT);
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);	
};

class TrojanEvilLotusSession : public TrojanSession
{
public:
	static int IsEvilLotusPacket(DATEHEADER& packh, const char* pPacket);
	virtual ~TrojanEvilLotusSession();
	TrojanEvilLotusSession(DATEHEADER& packh,int aT);
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);	
};

class TrojanRenWoSession : public TrojanSession
{
public:
	static int IsRenWoPacket(DATEHEADER& packh, const char* pPacket);
	virtual ~TrojanRenWoSession();
	TrojanRenWoSession(DATEHEADER& packh,int aT);
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);	
};

class TrojanNSSession : public TrojanSession
{
public:
	static int IsNSPacket(DATEHEADER& packh, const char* pPacket);
	virtual ~TrojanNSSession();
	TrojanNSSession(DATEHEADER& packh,int aT);
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);	

};

class TrojanGhostSession : public TrojanSession
{
public:
	static int IsGhostPacket(DATEHEADER& packh, const char* pPacket);
	virtual ~TrojanGhostSession();
	TrojanGhostSession(DATEHEADER& packh,int aT);
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);	
	
};


class TrojanXDoorSession : public TrojanSession
{
public:
	static int IsXDoorPacket(DATEHEADER& packh, const char* pPacket);
	virtual ~TrojanXDoorSession();
	TrojanXDoorSession(DATEHEADER& packh,int aT);
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);	
	
};


class TrojanHuiGeZiSession : public TrojanSession
{
public:
	static int IsHuiGeZiPacket(DATEHEADER& packh, const char* pPacket);
	virtual ~TrojanHuiGeZiSession();
	TrojanHuiGeZiSession(DATEHEADER& packh,int aT);
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);	
	
};

class TrojanWangLing1Session : public TrojanSession
{
public:
	static int IsWangLing1Packet(DATEHEADER& packh, const char* pPacket);
	virtual ~TrojanWangLing1Session();
	TrojanWangLing1Session(DATEHEADER& packh,int aT);
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);	
	
};

class TrojanHeiKeFangXianSession : public TrojanSession
{
public:
	static int IsHeiKeFangXianPacket(DATEHEADER& packh, const char* pPacket);
	virtual ~TrojanHeiKeFangXianSession();
	TrojanHeiKeFangXianSession(DATEHEADER& packh,int aT);
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);	
	
};

class TrojanMoJianChuanShuoSession : public TrojanSession
{
public:
	static int IsMoJianChuanShuoPacket(DATEHEADER& packh, const char* pPacket);
	virtual ~TrojanMoJianChuanShuoSession();
	TrojanMoJianChuanShuoSession(DATEHEADER& packh,int aT);
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);	
	
};

class TrojanBlackHoleSession : public TrojanSession
{
public:
	static int IsBlackHole(DATEHEADER& packh, const char* pPacket);
	virtual ~TrojanBlackHoleSession();
	TrojanBlackHoleSession(DATEHEADER& packh,int aT);
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);	
	
};

class TrojanLanHuDieSession : public TrojanSession
{
public:
	static int IsLanHuDiePacket(DATEHEADER& packh, const char* pPacket);
	virtual ~TrojanLanHuDieSession();
	TrojanLanHuDieSession(DATEHEADER& packh,int aT);
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);	
	
};


#endif
