
#ifndef AFX_SERVERSTATE_H__86455788_F25A_49A2_BAC0_4F3CA2ADCAD2__INCLUDED_
#define AFX_SERVERSTATE_H__86455788_F25A_49A2_BAC0_4F3CA2ADCAD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

typedef struct STRUCT_SERVERHOST
{
public:
	char		m_ip[20];
	int			m_power;
	DWORD		m_dbfstate;
	DWORD		m_webfigerstate;
	BYTE		m_workstate[4];
}SERVERHOST;

typedef struct DSEPOWER
{
	int			m_dsepower;
	int*		m_pdsepower;
}DSEPOWER;

class ServerArray
{
public:
	ServerArray();
	~ServerArray();

public:
	SERVERHOST*		m_pServer;
	int				m_count;
	int				m_totalpower;
	int*			m_AllDsePower;

	DSEPOWER*		m_pCurDse;
	DSEPOWER		m_CurDsePower[2];
public:
	void CheckServerJiyaState();
	LPCTSTR AllocateData(UINT hash, int* psvridx = NULL);
	void	AddDse(LPCTSTR ip, int power);
	
protected:
	void RefurDsePower(BOOL bAllDseJiya);
	int RenameTmpFile(LPCTSTR DseIP);
	void MoveJiYaFile(LPCTSTR DataType, int nSrcSvr, int nDstSvr);
};

extern ServerArray g_DseServer;		// dse服务器数组

#endif