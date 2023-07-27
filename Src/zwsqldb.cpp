// zwsqldb.cpp: implementation of the zwsqldb class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include <sys/stat.h>
#include "zwsqldb.h"
#include "zwsqlInc/zwsql.h"
#pragma comment(lib, "zwsqllib/libzwsql.lib")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CFyyTz	v_fyyTzUnit;
#define ZWDBUSER		"jncfwlkjyxgongs"

//////////////////////////////////////////////////////////////////////////
CString SQL_STR_CRYPT(CString str)
{
	CString stra;
	stra.Format( "AES_ENCRYPT('%s','inetsifter20021001')", str );

	return stra;
}

CString SQL_DCRYPT(CString str)// "AES_DECRYPT("str",\'inetsifter20021001\')"
{
	CString stra;
	stra.Format( "AES_DECRYPT(%s,'inetsifter20021001')", str );
	return stra;
}

//////////////////////////////////////////////////////////////////////////
zwsqldb::zwsqldb(int dbport, const char* dbname)
	:m_dbport(dbport),m_dbname(dbname)
{
	m_bInit = FALSE;
	m_result = NULL;
	zwsql_init (&m_zwsql);
	if(zwsql_real_connect(&m_zwsql,"127.0.0.1",ZWDBUSER,NULL,dbname,dbport,NULL,0))
		m_bInit = TRUE;

	return ;
}

zwsqldb::~zwsqldb()
{
	if(m_result)
	{
		zwsql_free_result(m_result);
	}
	if(m_bInit)
	{
		zwsql_close(&m_zwsql);
	}
}

BOOL zwsqldb::OpenConnect( int dbport, const char* dbname )
{
	if(m_bInit)
		zwsql_close(&m_zwsql);

	m_bInit = FALSE;
	zwsql_init (&m_zwsql);
	if(zwsql_real_connect(&m_zwsql,"127.0.0.1",ZWDBUSER,NULL,dbname,dbport,NULL,0))
		m_bInit = TRUE;

	return m_bInit;
}

DWORD zwsqldb::BeginQuery(CString strSQL)
{
	if(m_result )
	{
		zwsql_free_result(m_result);
		m_result = NULL;
	}

	BOOL bOpOK = m_bInit;
	if(FALSE == bOpOK)
		bOpOK = OpenConnect(m_dbport, m_dbname);
	if(FALSE == bOpOK)
		return 0;
	
	DWORD have = 0;
	bOpOK = (0 == zwsql_real_query(&m_zwsql,
		(char*)(LPCTSTR)strSQL,(UINT)strSQL.GetLength()));
	if(bOpOK)
	{
		if(!(m_result=zwsql_store_result(&m_zwsql)))
		{ 
			bOpOK =  FALSE;
		}
		else
		{
			have = zwsql_num_rows(m_result);
		}
	}
	if(FALSE == bOpOK)
	{
#ifdef _DEBUG
		int sqlerrno = zwsql_errno(&m_zwsql);
		const char *psqlerr = zwsql_error(&m_zwsql);
#endif
		if(m_bInit)
		{
			WriteLog("dce", "zwsql fail opera, close");
			zwsql_close(&m_zwsql);
			m_bInit = FALSE;
		}
	}
	return have;
}

DWORD zwsqldb::QueryOne(CStringArray &arrV)
{
	if(m_result == NULL)
		return 0;

	arrV.RemoveAll();
	BOOL bOpOK = FALSE;
	ZWSQL_ROW row;
	if(row=zwsql_fetch_row(m_result))
	{
		int numfield = zwsql_num_fields( m_result ) ;
		for (int ii = 0; ii < numfield ; ii++)
		{
			arrV.Add(row[ii]);
		}
		bOpOK = TRUE;
	}
	return bOpOK;
}

DWORD zwsqldb::QueryOne( CString strSQL, CStringArray &arrV )
{
	arrV.RemoveAll();
	BOOL bOpOK = m_bInit;
	if(FALSE == bOpOK)
		bOpOK = OpenConnect(m_dbport, m_dbname);
	if(FALSE == bOpOK)
		return 0;
	
	DWORD have = 0;
	bOpOK = (0 == zwsql_real_query(&m_zwsql,
		(char*)(LPCTSTR)strSQL,(UINT)strSQL.GetLength()));
	if(bOpOK)
	{
		ZWSQL_RES *result = NULL;
		if(!(result=zwsql_store_result(&m_zwsql)))
		{
			bOpOK = FALSE;
		}

		if(bOpOK)
		{
			int numfield = zwsql_num_fields( result ) ;
			have = zwsql_num_rows(result);
			ZWSQL_ROW row;
			if(row=zwsql_fetch_row(result))
			{
				for (int ii = 0; ii < numfield ; ii++)
				{
					arrV.Add(row[ii]);
				}
			}
		}
		zwsql_free_result(result);
	}
	return have;
}
void zwsqldb::FinishQuery()
{
	if(m_result )
	{
		zwsql_free_result(m_result);
		m_result = NULL;
	}

	return ;
}

//////////////////////////////////////////////////////////////////////////
CFyyTz::CFyyTz()
{
	m_bufid = 0;
	m_readflg[0] = 0;
	m_readflg[1] = 0;	
};

CFyyTz::~CFyyTz()
{
	map<CString, sTzUnit*>::iterator It;
	for(It = m_mapUnit[0].begin(); It != m_mapUnit[0].end(); It++)
	{
		if(It->second != NULL)
			delete It->second;
	}
	m_mapUnit[0].clear();
	for(It = m_mapUnit[1].begin(); It != m_mapUnit[1].end(); It++)
	{
		if(It->second != NULL)
			delete It->second;
	}
	m_mapUnit[1].clear();
};

sTzUnit* CFyyTz::CheckTzUnit( CString smac, CString dmac, int &bsrc)
{
//	return NULL;
	if(smac.IsEmpty() || dmac.IsEmpty())
		return NULL;

	smac.Replace("_", ".");
	dmac.Replace("_", ".");
	smac.MakeLower();
	dmac.MakeLower();
	
	bsrc = 0;
	int bufid = m_bufid;
	sTzUnit *pResult = NULL;
	InterlockedIncrement(&m_readflg[bufid]);
	map<CString, sTzUnit*>::iterator It;
	It = m_mapUnit[bufid].find(smac);
	if(It != m_mapUnit[bufid].end())
	{
		bsrc = 1;
		pResult = It->second;
	}
	else
	{
		It = m_mapUnit[bufid].find(dmac);
		if(It != m_mapUnit[bufid].end())
		{
			bsrc = 2;
			pResult = It->second;
		}
	}

	InterlockedDecrement(&m_readflg[bufid]);
	return pResult;
}

//////////////////////////////////////////////////////////////////////////
DWORD WINAPI UpdateTzUnit( LPVOID n )
{
// +------------+------+------+-----+---------+----------------+
// | Field      | Type | Null | Key | Default | Extra          |
// +------------+------+------+-----+---------+----------------+
// | RECORDID   |      | NO   | UNI | NULL    | auto_increment |
// | ACCOUNT    |      | NO   | PRI |         |                |
// | PASS       |      | YES  |     |         |                |
// | CARDTYPE   |      | YES  | MUL |         |                |
// | CARDNUMBER |      | YES  | MUL |         |                |
// | NAME       |      | YES  | MUL |         |                |
// | COUNTRY    |      | YES  |     |         |                |
// | ROOMID     |      | YES  |     |         |                |
// | STATE      |      | YES  |     |         |                |
// | ONLINE     |      | NO   | PRI |         |                |
// | AUTHTYPE   |      | YES  | MUL |         |                |
// | BEGTIME    |      | YES  |     |         |                |
// | ENDTIME    |      | YES  |     |         |                |
// | CLIENTIP   |      | YES  | MUL |         |                |
// | CLIENTMAC  |      | YES  |     |         |                |
// | SESSIONID  |      | YES  |     |         |                |
// | GENDER     |      | YES  |     |         |                |
// | CERTUNIT   |      | YES  |     |         |                |
// | DESCRI     |      | YES  |     |         |                |
// +------------+------+------+-----+---------+----------------+
// 19 rows in set (0.00 sec)
// 	time_t tOld = 0;
// 	time_t tNew = 0;
	const char *clientdbf = "e:\\netspy\\database\\client\\client.dbf";
	const char *clientquery = "select aes_decrypt(cardtype,'inetsifter20021001'),"
				"aes_decrypt(cardnumber,'inetsifter20021001'), "
				"aes_decrypt(name, 'inetsifter20021001'), "
				"aes_decrypt(country, 'inetsifter20021001'), "
				"aes_decrypt(roomid, 'inetsifter20021001'), "
				"aes_decrypt(curmac, 'inetsifter20021001'), "
				"aes_decrypt(account, 'inetsifter20021001'), "
				"aes_decrypt(curip, 'inetsifter20021001') "
				"from client where online=aes_encrypt('1','inetsifter20021001')";

	const char *terminaldbf = "e:\\netspy\\database\\client\\terminal.dbf";
	const char *terminalquery = "select aes_decrypt(cardtype,'inetsifter20021001'),"
				"aes_decrypt(cardnumber,'inetsifter20021001'), "
				"aes_decrypt(name, 'inetsifter20021001'), "
				"aes_decrypt(country, 'inetsifter20021001'), "
				"aes_decrypt(roomid, 'inetsifter20021001'), "
				"aes_decrypt(curmac, 'inetsifter20021001'), "
				"aes_decrypt(account, 'inetsifter20021001'), "
				"aes_decrypt(curip, 'inetsifter20021001') "
				"from terminal where online=aes_encrypt('1','inetsifter20021001')";

// 	struct stat statbuf;
// 	CString strTNew, strTOld;
	zwsqldb commdb_client(10102, "client");
	CStringArray arrT;
	while(1)
	{
		Sleep(5000);
		if(_access(clientdbf, 0) != 0 && _access(terminaldbf, 0) != 0)
			continue;

		ASSERT(v_fyyTzUnit.m_bufid==0 || v_fyyTzUnit.m_bufid==1);
		long	bufid = (v_fyyTzUnit.m_bufid==0)? 1 : 0;
		if(v_fyyTzUnit.m_readflg[bufid] > 0)
			continue;

// 		if(commdb_client.QueryOne("SELECT update_time,TABLE_NAME FROM INFORMATION_SCHEMA.TABLES where table_name='client'", arrT))
// 		{
// 			strTOld = arrT[0];
// 		}
		/// 检查数据库中记录是否变化
// 		if(0 == stat(terminaldbf, &statbuf))
// 			tNew = statbuf.st_mtime;//time(0);//GetSqlLastModify();
// 		if(tNew != tOld || strTNew.Compare(strTOld) != 0)	/// 有变化
		{
			/// 清旧
			map<CString, sTzUnit*>::iterator It;
			for(It = v_fyyTzUnit.m_mapUnit[bufid].begin(); It != v_fyyTzUnit.m_mapUnit[bufid].end(); It++)
			{
				if(It->second != NULL)
					delete It->second;
			}
			v_fyyTzUnit.m_mapUnit[bufid].clear();

// cardtype(证件类型)
// cardno(证件名称)
// username(客户姓名)
// country(国籍 外国、港、澳、台需填写)
			/// 获得新参数入 v_fyyTzUnit.m_mapUnit[bufid]中
			CString key;
			CStringArray arrUnit;
			BOOL bDump = (_access("e:\\netspy\\flg\\dumpclient.flg", 0)== 0);
			for (int ii = 0; ii < 2 ; ii++)
			{
				if(ii == 0 && _access(clientdbf, 0) != 0)
					continue;
				if(ii == 1 && _access(terminaldbf, 0) != 0)
					continue;

				if(commdb_client.BeginQuery((ii==0)?clientquery:terminalquery))
				{
					while(commdb_client.QueryOne(arrUnit))
					{
						ASSERT(arrUnit.GetSize() == 8);
						if(arrUnit.GetSize() != 8)
							break;

						LPCTSTR tpmac = (LPCTSTR)arrUnit[5];
						char mac[20];
						memset(mac, 0, sizeof(mac));
						if (strlen(tpmac) == 12)
						{
							memset(mac, ':', 17);
							memcpy(mac, tpmac, 2);
							memcpy(mac+3, tpmac+2, 2);
							memcpy(mac+6, tpmac+4, 2);
							memcpy(mac+9, tpmac+6, 2);
							memcpy(mac+12, tpmac+8, 2);
							memcpy(mac+15, tpmac+10, 2);
						}
						else if (strlen(tpmac) == 17)
						{
							strcpy(mac, tpmac);
						}
						else
						{
							continue;
						}
						/// 转小写
						strlwr(mac);
						if(strlen(arrUnit[7]) != 15)
							continue;		/// ip不符合点分补齐格式

						if(bDump)
						{
							WriteLog("dce", "account:%s, user:%s online", arrUnit[6], arrUnit[2]);
						}
						key.Format("%s-%s", arrUnit[7], mac);
						It = v_fyyTzUnit.m_mapUnit[bufid].find(key);
						if(It == v_fyyTzUnit.m_mapUnit[bufid].end())
						{
							sTzUnit *pUnit = new sTzUnit;
							pUnit->m_cardtype = arrUnit[0];
							pUnit->m_cardno = arrUnit[1];
							pUnit->m_username = arrUnit[2];
							pUnit->m_country = arrUnit[3];
							pUnit->m_roomid = arrUnit[4];
							pUnit->m_umac = mac;
							pUnit->m_account = arrUnit[6];
							pUnit->m_curip = arrUnit[7];
							v_fyyTzUnit.m_mapUnit[bufid].insert(make_pair(key, pUnit));
						}
// 						else
// 						{
// 							It->second->m_cardtype = arrUnit[0];
// 							It->second->m_cardno = arrUnit[1];
// 							It->second->m_username = arrUnit[2];
// 							It->second->m_country = arrUnit[3];
// 							It->second->m_roomid = arrUnit[4];
// 							It->second->m_umac = mac;
// 							It->second->m_account = arrUnit[6];
// 							It->second->m_curip = arrUnit[7];
// 						}
					}
					
				}
				commdb_client.FinishQuery();
			}

			if(bDump)
				remove("e:\\netspy\\flg\\dumpclient.flg");

			/// 启用新
			if(v_fyyTzUnit.m_mapUnit[v_fyyTzUnit.m_bufid].size() != v_fyyTzUnit.m_mapUnit[bufid].size())
				WriteLog("dce", "更新活动客户信息 %d 项", v_fyyTzUnit.m_mapUnit[bufid].size());
			InterlockedExchange(&v_fyyTzUnit.m_bufid, bufid);
// 			tOld = tNew;
// 			strTNew = strTOld;
// 			WriteLog("dce", "更新时间 %s", strTNew);
		}
	}

	return 0;
}