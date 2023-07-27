// DceMission.h: interface for the CDceMission class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCEMISSION_H__8063D36E_A2B9_413D_AEA3_0BAC6E40C5AE__INCLUDED_)
#define AFX_DCEMISSION_H__8063D36E_A2B9_413D_AEA3_0BAC6E40C5AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class DceProcotol;
// ��������ඨ��
class DceMission
{
public:
	DceMission();
	~DceMission(){	}
public:
	void RunForNoHash();										// ������ѭ��1(��������ϣ��)
	void RunForOneSubPro();										// ������ѭ��2(һ����Э��)
	void RenameTmpProDir(LPCTSTR workpath, LPCTSTR strtime);
	void ClearSocketMap(time_t nowtm);	
//	CString QueryIpAddr(LPCTSTR strip);
//	CString QueryIpAddr(DWORD uip);
	CString GetTmpPath();
	SocketSession* LocateSession(DATEHEADER& packh, char* key);	
	static DWORD WINAPI MissRunProc(LPVOID p);					// �����߳����
public:
	int				m_ddebakflg;		// Э�鱸�ݱ�־
	DDEFILEINFO*	m_pDdeFileInfo;		// DDE���ݶ��е�Ԫ����
	LPCTSTR			m_ddefiletype;		// ����dde�ļ�����������
	SocketMapClass	m_SocketMap;		// �Ự��ϣ��
	DceProcotol*	m_pSubPro;			// Э���������
	HANDLE			m_MissHeap;			// �����̶߳Ѿ��

	// DEBUG 
	DWORD			m_packetnum;		// ��ǰ����İ�����
	DWORD			m_loopnum;			// ��ǰѭ������
	LPCTSTR			m_logfilename;		// ��־�ļ���
};

#endif // !defined(AFX_DCEMISSION_H__8063D36E_A2B9_413D_AEA3_0BAC6E40C5AE__INCLUDED_)
