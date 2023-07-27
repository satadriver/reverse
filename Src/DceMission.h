// DceMission.h: interface for the CDceMission class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCEMISSION_H__8063D36E_A2B9_413D_AEA3_0BAC6E40C5AE__INCLUDED_)
#define AFX_DCEMISSION_H__8063D36E_A2B9_413D_AEA3_0BAC6E40C5AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class DceProcotol;
// 任务对象类定义
class DceMission
{
public:
	DceMission();
	~DceMission(){	}
public:
	void RunForNoHash();										// 任务主循环1(不包含哈希表)
	void RunForOneSubPro();										// 任务主循环2(一个子协议)
	void RenameTmpProDir(LPCTSTR workpath, LPCTSTR strtime);
	void ClearSocketMap(time_t nowtm);	
//	CString QueryIpAddr(LPCTSTR strip);
//	CString QueryIpAddr(DWORD uip);
	CString GetTmpPath();
	SocketSession* LocateSession(DATEHEADER& packh, char* key);	
	static DWORD WINAPI MissRunProc(LPVOID p);					// 任务线程入口
public:
	int				m_ddebakflg;		// 协议备份标志
	DDEFILEINFO*	m_pDdeFileInfo;		// DDE数据队列单元数组
	LPCTSTR			m_ddefiletype;		// 处理dde文件的类型名称
	SocketMapClass	m_SocketMap;		// 会话哈希表
	DceProcotol*	m_pSubPro;			// 协议对象链表
	HANDLE			m_MissHeap;			// 任务线程堆句柄

	// DEBUG 
	DWORD			m_packetnum;		// 当前处理的包个数
	DWORD			m_loopnum;			// 当前循环次数
	LPCTSTR			m_logfilename;		// 日志文件名
};

#endif // !defined(AFX_DCEMISSION_H__8063D36E_A2B9_413D_AEA3_0BAC6E40C5AE__INCLUDED_)
