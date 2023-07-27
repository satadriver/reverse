// zwsqldb.h: interface for the zwsqldb class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZWSQLDB_H__CAD03DF4_B05A_47FD_93B5_07A8B37E3C49__INCLUDED_)
#define AFX_ZWSQLDB_H__CAD03DF4_B05A_47FD_93B5_07A8B37E3C49__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma warning( disable : 4786 )

#include <string>
#include <map>
#include <vector>
using namespace std;
#include "zwsqlInc/zwsql.h"

// ʵ���˿�10102
// ����client
// ������client.dbf(�ͻ���)
// ���ܴ洢
// ��ṹ:
// recordid		int					��¼id(ϵͳ�Զ�����)
// cardtype		char(20)			֤������(GA/Z 02-2005 D2008)
// cardnumber	char(40)			֤������(GA/Z 02-2005 D1000)
// name			char(80)			�ͻ�����
// country		char(4)				����(GA/Z 02-2005 D1016)
// roomid		char(40)			����� (����λ��)
// state		char(4)				״̬("��Ч"��"�"��"ע��")
// begtime		char(20)			��Чʱ��(YYYY-MM-DD hh:mm:ss)
// endtime		char(20)			ע��ʱ��(YYYY-MM-DD hh:mm:ss)
// clientip		char(16)			�ͻ���ip
// clientmac	char(20)			�ͻ���mac

// ʵ���˿�10102
// ����client
// ������terminal.dbf(�ͻ���)
// ���ܴ洢
// ��ṹ:
// recordid		int					��¼id(ϵͳ�Զ�����)
// roomid		char(40)			����� (����λ��)(����Ϊ��)
// state		char(4)				״̬("����"��"�"�� "��ֹ")
// clientip		char(16)			�ͻ���ip
// clientmac	char(20)			�ͻ���mac
// name			char(40)			ʹ����(����Ϊ��)

class zwsqldb  
{
public:
	zwsqldb(int dbport, const char* dbname);
	virtual ~zwsqldb();
public:
	BOOL	OpenConnect(int dbport, const char* dbname);
	DWORD	BeginQuery( CString strSQL);
	DWORD	QueryOne(CStringArray &arrV);
	void	FinishQuery();	
	DWORD	QueryOne( CString strSQL, CStringArray &arrV);
public:
	BOOL	m_bInit;
	int		m_dbport;
	CString m_dbname;
	ZWSQL	m_zwsql;
	ZWSQL_RES *m_result;
};

// �����ļ������ӵ���֤��Ϣ������
// cardtype(֤������)
// cardno(֤������)
// username(�ͻ�����)
// country(���� ������ۡ��ġ�̨����д)
// �ӿͻ���(client.dbf)��ȡֵ

typedef struct _TZUNIT_
{
public:
	string	m_roomid;
	string	m_cardtype;
	string	m_cardno;
	string	m_username;
	string	m_country;
	string	m_account;
	string	m_umac;
	string	m_curip;
}sTzUnit;

class CFyyTz
{
public:
	CFyyTz();
	~CFyyTz();

public:
	sTzUnit* CheckTzUnit(CString smac, CString dmac, int &bsrc);

	long					m_bufid;
	long					m_readflg[2];
	map<CString, sTzUnit*>	m_mapUnit[2];		/// �� IP(.�ֲ���)-mac(:��Сд)Ϊkey
};

/// �����߳�, EnterMainLoop���̵߳���
DWORD WINAPI UpdateTzUnit(LPVOID n);

/// 
extern CFyyTz	v_fyyTzUnit;

#endif // !defined(AFX_ZWSQLDB_H__CAD03DF4_B05A_47FD_93B5_07A8B37E3C49__INCLUDED_)
