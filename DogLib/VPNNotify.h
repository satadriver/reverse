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
	BOOL	Init(LPCTSTR svrIP);		// ��ʼ��

	BOOL	Notify(LPCTSTR clientIP);	// �걨

	//openvpn���� ��һ��������vpn���ӵĴ���ip���ڶ������ϼ����ĵ�ip,�������ǲ���ָ����ip
	BOOL	RasConnect(CString vpnserverip,CString vpnroute,CString SubNetIp); 

	BOOL	CheckRasConnect(); //���openvpn�Ƿ��Ѿ�����

	BOOL	CloseOpenVpn();//�ر�openvpn

	BOOL	WriteOpenvpnServerFile(CString route);

public:
	CString m_TapCard;
	CString m_SubNetIp;
private:
	// �γ�һ���걨��
	BOOL	GetPacket(DWORD stage, char *buffer, int &buflen);
	// ���һ���걨��Ӧ���Ƿ����Ҫ��
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
