// VPNNotify.h: interface for the CVPNNotify class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VPNNOTIFY_H__20BDBB77_E1EC_4EE4_8276_27018F787F8F__INCLUDED_)
#define AFX_VPNNOTIFY_H__20BDBB77_E1EC_4EE4_8276_27018F787F8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CVPNNotify  
{
public:
	CVPNNotify();
	virtual ~CVPNNotify();

public:
	BOOL	Init(LPCTSTR svrIP);		// 初始化

	BOOL	Notify(LPCTSTR clientIP);	// 申报

	//openvpn拨号 第一个参数是vpn链接的大网ip，第二个是上级中心的ip,第三个是拨入指定的ip
	BOOL	RasConnect(CString vpnserverip,CString vpnroute,CString SubNetIp); 

	BOOL	CheckRasConnect(); //检查openvpn是否已经拨号

	BOOL	CloseOpenVpn();//关闭openvpn

	BOOL	WriteOpenvpnServerFile(CString route);

public:
	CString m_TapCard;
	CString m_SubNetIp;
private:
	// 形成一个申报包
	BOOL	GetPacket(DWORD stage, char *buffer, int &buflen);
	// 检查一个申报回应包是否符合要求
	BOOL	CheckPacket(DWORD stage, const char *buffer, int buflen);
	int GetAdapterAndSetip();

private:
	BOOL  m_bInit;
	BOOL  m_bInitSocket;
	BOOL  m_bIsAPPNotify;

	DWORD	m_SessionID;
	DWORD	m_dwSvrIP;
	DWORD	m_dwClientIP;

	SOCKET	m_vpnSocket;
	struct sockaddr_in m_vpnServer;
};

#endif // !defined(AFX_VPNNOTIFY_H__20BDBB77_E1EC_4EE4_8276_27018F787F8F__INCLUDED_)
