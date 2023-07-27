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

// 实例端口10102
// 库名client
// 表名称client.dbf(客户表)
// 加密存储
// 表结构:
// recordid		int					记录id(系统自动生成)
// cardtype		char(20)			证件类型(GA/Z 02-2005 D2008)
// cardnumber	char(40)			证件号码(GA/Z 02-2005 D1000)
// name			char(80)			客户姓名
// country		char(4)				国籍(GA/Z 02-2005 D1016)
// roomid		char(40)			房间号 (物理位置)
// state		char(4)				状态("起效"、"活动"、"注销")
// begtime		char(20)			起效时间(YYYY-MM-DD hh:mm:ss)
// endtime		char(20)			注销时间(YYYY-MM-DD hh:mm:ss)
// clientip		char(16)			客户端ip
// clientmac	char(20)			客户端mac

// 实例端口10102
// 库名client
// 表名称terminal.dbf(客户表)
// 加密存储
// 表结构:
// recordid		int					记录id(系统自动生成)
// roomid		char(40)			房间号 (物理位置)(可以为空)
// state		char(4)				状态("允许"、"活动"、 "禁止")
// clientip		char(16)			客户端ip
// clientmac	char(20)			客户端mac
// name			char(40)			使用者(可以为空)

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

// 对照文件中增加的认证信息包括：
// cardtype(证件类型)
// cardno(证件名称)
// username(客户姓名)
// country(国籍 外国、港、澳、台需填写)
// 从客户表(client.dbf)中取值

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
	map<CString, sTzUnit*>	m_mapUnit[2];		/// 以 IP(.分补齐)-mac(:分小写)为key
};

/// 更新线程, EnterMainLoop开线程调用
DWORD WINAPI UpdateTzUnit(LPVOID n);

/// 
extern CFyyTz	v_fyyTzUnit;

#endif // !defined(AFX_ZWSQLDB_H__CAD03DF4_B05A_47FD_93B5_07A8B37E3C49__INCLUDED_)
