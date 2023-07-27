// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__FAB925B9_B967_4781_9E1F_80E698EAF1A3__INCLUDED_)
#define AFX_STDAFX_H__FAB925B9_B967_4781_9E1F_80E698EAF1A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma warning(disable:4786)

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#define   _WIN32_WINNT   0x0500

#include <afx.h>
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <direct.h>
#include "BaseStruct.h"
#include "doglib/imwdog.h"
#include "DogLib/Cryptoimw.h"
#include "dbgswitch.h"

// �����궨��
//#define DCEPUBLICAPI

#define		PRONUM				10
//#define		Maxfilelen			3000000
#define		Maxfilelen			10000000
#define		MaxSeq				0xA0000000
#define		SeqOffset			0x80000000
#define		BufPacketCount		200
#define		FILEBUFFERSIZE		2048*1024			// 2M
#define		MAXLEN				2000
#define		AuthHeadLen			(sizeof(NEWAAAPACKET))
#define		OLDHEADERLEN		(sizeof(OLDDATEHEADER))
#define		NEWHEADERLEN		(sizeof(DATEHEADER))

// ����Դ
#define		DATASRC_IDC			0x0			//_T("IDC")
#define		DATASRC_CDMA		0x1			//_T("CDMA")
#define		DATASRC_GPRS		0x2			//_T("GPRS")
#define		DATASRC_OTHER		0x3			//_T("����")

// IDCͳ�Ʒ���������(״̬)
#define		TYPESTATE_DECIDED		"��ȷ"
#define		TYPESTATE_POSSIBLE		"����"
#define		INTERACT_DYNAMIC		"�ɽ���"
#define		INTERACT_STATIC		    "���ɽ���"

// ϵͳ���Ͷ���
#define		SYSTEM_002		0x01
#define		SYSTEM_SZFJ		0x02
#define		SYSTEM_SLEUTH	0x04
#define		SYSTEM_FYY		0x08
#define		SYSTEM_ZONE		0x10
#define		SYSTEM_HOTEL		0x20
#define		SYSTEM_TELEWIRELESS	0x40
#define		SYSTEM_UNICOMWIRELESS 0x80
#define		SYSTEM_DEPARTMENT	0x100
#define		SYSTEM_ZK	0x200

// #define		SYSTEM_JC		0x08
// #define		SYSTEM_SJ		0x10
// #define		SYSTEM_YD		0x16
#define		SYSTEM_ALL		0xFFFF

// ������
#define		AAAPacket			1		// ��֤���ݰ���־
#define		TCPPacket			2		// TCP���ݰ���־
#define		UDPPacket			3		// UDP���ݰ���־
#define		AllIPPacket			4		
#define		IdcHttpPacket		7		// Http���س�������־
#define		ProxyTCP			8
#define		ProxyUDP			9
#define		UnipTCP				10
#define		UnipUDP				11
#define		CDMATCP				12
#define		CDMAUDP				13
#define		GPRSTCP				14
#define		GPRSUDP				15
#define		HTTP200OK			16
#define		CdmaHttpPacket		17
#define		GprsHttpPacket		18
#define		CdmaUnipTCP			19
#define		CdmaUnipUDP			20
#define		GprsUnipTCP			21
#define		GprsUnipUDP			22




#include "PubType.h"	// �������Ͷ���
#include "PubVar.h"		// ȫ�ֱ�������
#include "PubFunc.h"	// ȫ�ֺ�������
#include "PubClass.h"	// ���ඨ��
#include "ProMission.h"
#include "IdcIpLib.h"	// IDCIP��ַ��
#include "LimitPara.h"	// 
#include "IDCServer.h"	// IDC������ͳ����
#include "ServerState.h"

#include "compress\Lzw.h"



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__FAB925B9_B967_4781_9E1F_80E698EAF1A3__INCLUDED_)
