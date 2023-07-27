// IpQueryLib.cpp: implementation of the IpQueryLib class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
// #include "..\\include\\d4all.hpp"
#include "IpQueryLib.h"
#include <sys/stat.h>
#include "SyncTable.h"
#include "iphlpapi.h"
#include <winsock2.h>
#pragma comment(lib,"Iphlpapi.lib") 

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define		TXTBUFSIZE		4096
#define		BUFBLOCK		8
#define		IPARRAYBLOCK	32
                                                                                                                                                     
//FIELD4INFO Ip_fields[] =			// IP地址表的结构，临时
//{
//	{"REC_TIME",	r4num, 10, 0},	// 记录同步标识(同步时使用)(time_t数据的串)
//	{"BEGINIP",		r4str, 15, 0},	// 开始IP地址
//	{"ENDIP",		r4str, 15, 0},	// 结束IP地址
//	{"IPAREA",		r4str, 100, 0},	// IP所属地区
//	{ 0,0,0,0 }
//};


const char*	pEmpty = _T("");

// TAG4INFO AllIp_tag[]=				// 全球IP地址表表索引
// {
// 	{"BEGINIP",		"BEGINIP", 0, 0, r4descending},	//用开始IP地址作为索引(倒序)
// 	{0, 0, 0, 0, 0}
// };
// 
// TAG4INFO LocalIp_tag[]=				// 本地IP地址表表索引
// {
// 	{"REC_TIME",		"REC_TIME", 0, 0, r4descending},	//用时间作为索引(倒序)
// 	{0, 0, 0, 0, 0}
// };
// 

int strtrimcpy(char*pdtr, char *pstr)
{
	static char space[265] = 
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0,	1, 1, 1, 1, 1, 0, 0,	// 0
		0, 0, 0, 0, 0, 0, 0, 0, 0,	0, 0, 0, 0, 0, 0, 0,	// 1
		1, 0, 0, 0, 0, 0, 0, 0, 0,	0, 0, 0, 0, 0, 0, 0,	// 2
		0, 0, 0, 0, 0, 0, 0, 0, 0,	0, 0, 0, 0, 0, 0, 0,	// 3
		0, 0, 0, 0, 0, 0, 0, 0, 0,	0, 0, 0, 0, 0, 0, 0,	// 4
		0, 0, 0, 0, 0, 0, 0, 0, 0,	0, 0, 0, 0, 0, 0, 0,	// 5
		0, 0, 0, 0, 0, 0, 0, 0, 0,	0, 0, 0, 0, 0, 0, 0,	// 6
		0, 0, 0, 0, 0, 0, 0, 0, 0,	0, 0, 0, 0, 0, 0, 0,	// 7
		0, 0, 0, 0, 0, 0, 0, 0, 0,	0, 0, 0, 0, 0, 0, 0,	// 8
		0, 0, 0, 0, 0, 0, 0, 0, 0,	0, 0, 0, 0, 0, 0, 0,	// 9
		0, 0, 0, 0, 0, 0, 0, 0, 0,	0, 0, 0, 0, 0, 0, 0,	// 10
		0, 0, 0, 0, 0, 0, 0, 0, 0,	0, 0, 0, 0, 0, 0, 0,	// 11
		0, 0, 0, 0, 0, 0, 0, 0, 0,	0, 0, 0, 0, 0, 0, 0,	// 12
		0, 0, 0, 0, 0, 0, 0, 0, 0,	0, 0, 0, 0, 0, 0, 0,	// 13
		0, 0, 0, 0, 0, 0, 0, 0, 0,	0, 0, 0, 0, 0, 0, 0,	// 14
		0, 0, 0, 0, 0, 0, 0, 0, 0,	0, 0, 0, 0, 0, 0, 0,	// 15
	};
	
	ASSERT(pstr != NULL);
	char *tp = pstr;
	
	while (space[(BYTE)*tp]) tp++;
	int len = strlen(tp);
	if (len <= 0)
	{
		if (pdtr != 0)
			*pdtr = 0;
		else
			*pstr = 0;
		return 0;
	}
	int bpos = tp-pstr;
	char *tep = tp+len-1;
	while (space[(BYTE)*tep]) tep--;
	if ((pdtr != 0) && (pdtr != pstr))
	{
		int l = tep-tp+1;
		memcpy(pdtr, pstr+bpos, l);
		pdtr[l] = 0;
		return l;
	}
	else
	{
		int l = tep-tp+1;
		memmove(pstr, pstr+bpos, l);
		pstr[l] = 0;
		return l;
	}
	
}

LPCTSTR _IPAREATXT_::AppendIPAreaText(LPCTSTR ptxt, int len/* = -1*/)
{
	if (len < 0)
	{
		char tbuf[200];
		len = strtrimcpy(tbuf, (char*)ptxt);
		len=len+2;
		sprintf(tbuf,"%s ",tbuf);//为了处理ip区域中有乱码情况做的处理
		return AppendText(tbuf, len);
	}
	else
	{
		return AppendText(ptxt, len);
	}
}

LPCTSTR _IPAREATXT_::AppendText(LPCTSTR ptxt, int len)
{
	if (len <= 0)
		return pEmpty;
	if (txtpos == 0)
	{
		// 增加一个新的缓冲区
		if (bufpos == txtbufcount)
		{
			// 开辟新的缓冲区
			if (0 == (txtbufcount & (BUFBLOCK-1)))
			{
				char **tp = new char*[txtbufcount+BUFBLOCK];
				memcpy(tp, ptxtbuf, txtbufcount*4);
				delete[] ptxtbuf;
				ptxtbuf = tp;
			}
			ptxtbuf[txtbufcount++] = new char[TXTBUFSIZE];
		}
	}

	int slen = (len+4) & ~3;
	if (slen > TXTBUFSIZE)
		slen = TXTBUFSIZE;

	if (slen+txtpos > TXTBUFSIZE)
	{
		txtpos=0;
		bufpos++;
		return AppendText(ptxt, len);
	}
	char* tpbuf = ptxtbuf[bufpos]+txtpos;
	memset(tpbuf+slen-4, 0, 4);
	memcpy(tpbuf, ptxt, len);
	txtpos += slen;
	return tpbuf;
}

BOOL QueryIPFromArray(DWORD uip, LPIPUNIT piparray, int arrsize, IPUNIT& ipunit)
{
	if ((piparray == 0) || (arrsize <= 0))
		return FALSE;
	// 折半法查找IP段(IP数组为begip的倒序列)
	int beg = 0, end = arrsize-1, pos;
	LPIPUNIT tp;
	while (beg <= end)
	{
		pos = (beg + end) / 2;
		tp = piparray + pos;
		if (uip < tp->begip)
		{
			beg = pos + 1;
		}
		else if (uip > tp->begip)
		{
			end = pos - 1;
		}
		else
		{
			if (ipunit.rectm < tp->rectm)
				ipunit = *tp;
			return TRUE;
// 			else
// 				return FALSE;
		}
	}
	if (end < pos)
	{
		ASSERT(tp->begip < uip);
		if (tp->endip >= uip)
		{
			if (ipunit.rectm < tp->rectm)
				ipunit = *tp;
			return TRUE;
		}
		else
			return FALSE;
	}
	if (beg > pos)
	{
		if (beg >= arrsize)	// 搜索指针定位到结尾且末尾的记录不不合法
			return FALSE;
		tp++;	// n.begip > n+1.begip
		ASSERT(tp->begip < uip);
		if (tp->endip >= uip)
		{
			if (ipunit.rectm < tp->rectm)
			{
				ipunit = *tp;
				return TRUE;
			}
			else
				return FALSE;
		}
		else
			return FALSE;
	}
	return FALSE;
}

BOOL _IPUNITARRAY_::QueryIP(DWORD uip, IPUNIT& ipunit)
{
	if (unitcount > 0)
	{
		ASSERT(piparray != NULL);
		LPIPARRAY tp = this;
		while (tp != NULL)
		{
			if (QueryIPFromArray(uip, tp->piparray, tp->unitcount, ipunit))
			{
//				if (ipunit.rectm == this->newtime)
				return TRUE;
			}
			tp = tp->pnext;
		}
	}
	return FALSE;
}


IpQueryLib::IpQueryLib()
{
	// All IP
	m_ppAllIpMap_64k[0] = new LPIPARRAY[65536];	// 64k
	memset(m_ppAllIpMap_64k[0], 0, 4*65536);
	m_ppAllIpMap_64k[1] = new LPIPARRAY[65536];	// 64k
	memset(m_ppAllIpMap_64k[1], 0, 4*65536);
	m_ppAllIpMap_256[0] = new LPIPARRAY[256];		// 256
	memset(m_ppAllIpMap_256[0], 0, 4*256);
	m_ppAllIpMap_256[1] = new LPIPARRAY[256];		// 256
	memset(m_ppAllIpMap_256[1], 0, 4*256);
	m_pAllIplastMap[0] = m_pAllIplastMap[1] = NULL;
	
	// Local IP
	m_ppIpMap_64k[0] = new LPIPARRAY[65536];		// 64k
	memset(m_ppIpMap_64k[0], 0, 4*65536);
	m_ppIpMap_64k[1] = new LPIPARRAY[65536];		// 64k
	memset(m_ppIpMap_64k[1], 0, 4*65536);
	m_ppIpMap_256[0] = new LPIPARRAY[256];			// 256
	memset(m_ppIpMap_256[0], 0, 4*256);
	m_ppIpMap_256[1] = new LPIPARRAY[256];			// 256
	memset(m_ppIpMap_256[1], 0, 4*256);
	m_pIplastMap[0] = m_pIplastMap[1] = NULL;

	// Local IP
	m_ppyysIpMap_64k[0] = new LPIPARRAY[65536];		// 64k
	memset(m_ppyysIpMap_64k[0], 0, 4*65536);
	m_ppyysIpMap_64k[1] = new LPIPARRAY[65536];		// 64k
	memset(m_ppyysIpMap_64k[1], 0, 4*65536);
	m_ppyysIpMap_256[0] = new LPIPARRAY[256];			// 256
	memset(m_ppyysIpMap_256[0], 0, 4*256);
	m_ppyysIpMap_256[1] = new LPIPARRAY[256];			// 256
	memset(m_ppyysIpMap_256[1], 0, 4*256);
	m_pyysIplastMap[0] = m_pyysIplastMap[1] = NULL;

	m_AllIpLibReadFlag = 0;
	m_LocIpLibReadFlag = 0;
	m_yysLibReadFlag = 0;
}

IpQueryLib::~IpQueryLib()
{
	for (int i = 0; i < 65536; i++)
	{
		if (m_ppAllIpMap_64k[0][i] != 0)
			delete m_ppAllIpMap_64k[0][i];
		if (m_ppAllIpMap_64k[1][i] != 0)
			delete m_ppAllIpMap_64k[1][i];
		if (m_ppIpMap_64k[0][i] != 0)
			delete m_ppIpMap_64k[0][i];
		if (m_ppIpMap_64k[1][i] != 0)
			delete m_ppIpMap_64k[1][i];

		if (m_ppyysIpMap_64k[0][i] != 0)
			delete m_ppyysIpMap_64k[0][i];
		if (m_ppyysIpMap_64k[1][i] != 0)
			delete m_ppyysIpMap_64k[1][i];
	}
	delete[] m_ppIpMap_64k[0];
	delete[] m_ppIpMap_64k[1];
	delete[] m_ppAllIpMap_64k[0];
	delete[] m_ppAllIpMap_64k[1];
	delete[] m_ppyysIpMap_64k[0];
	delete[] m_ppyysIpMap_64k[1];

	for (i = 0; i < 256; i++)
	{
		if (m_ppAllIpMap_256[0][i] != 0)
			delete m_ppAllIpMap_256[0][i];
		if (m_ppAllIpMap_256[1][i] != 0)
			delete m_ppAllIpMap_256[1][i];
		if (m_ppIpMap_256[0][i] != 0)
			delete m_ppIpMap_256[0][i];
		if (m_ppIpMap_256[1][i] != 0)
			delete m_ppIpMap_256[1][i];
		if (m_ppyysIpMap_256[0][i] != 0)
			delete m_ppyysIpMap_256[0][i];
		if (m_ppyysIpMap_256[1][i] != 0)
			delete m_ppyysIpMap_256[1][i];
	}
	delete[] m_ppAllIpMap_256[0];
	delete[] m_ppAllIpMap_256[1];
	delete[] m_ppIpMap_256[0];
	delete[] m_ppIpMap_256[1];
	delete[] m_ppyysIpMap_256[0];
	delete[] m_ppyysIpMap_256[1];

	if (m_pIplastMap[0]!=NULL)
		delete m_pIplastMap[0];
	if (m_pIplastMap[1]!=NULL)
		delete m_pIplastMap[1];
	if (m_pAllIplastMap[0] != NULL)
		delete m_pAllIplastMap[0];
	if (m_pAllIplastMap[1] != NULL)
		delete m_pAllIplastMap[1];
	if (m_pyysIplastMap[0] != NULL)
		delete m_pyysIplastMap[0];
	if (m_pyysIplastMap[1] != NULL)
		delete m_pyysIplastMap[1];
}

void IpQueryLib::ClearAllIpLib(long bufid)
{
	ASSERT(bufid == 0 || bufid == 1);
	for (int i = 0; i < 65536; i++)
	{
		if (m_ppAllIpMap_64k[bufid][i] != 0)
			m_ppAllIpMap_64k[bufid][i]->Reset();
	}
	
	for (i = 0; i < 256; i++)
	{
		if (m_ppAllIpMap_256[bufid][i] != 0)
			m_ppAllIpMap_256[bufid][i]->Reset();
	}

	if (m_pAllIplastMap[bufid] != NULL)
		m_pAllIplastMap[bufid]->Reset();

	m_pAllIptxtbuf[bufid].Reset();

}
void IpQueryLib::ClearyysIpLib(long bufid)
{
	ASSERT(bufid == 0 || bufid == 1);
	for (int i = 0; i < 65536; i++)
	{
		if (m_ppyysIpMap_64k[bufid][i] != 0)
			m_ppyysIpMap_64k[bufid][i]->Reset();
	}
	
	for (i = 0; i < 256; i++)
	{
		if (m_ppyysIpMap_256[bufid][i] != 0)
			m_ppyysIpMap_256[bufid][i]->Reset();
	}

	if (m_pyysIplastMap[bufid] != NULL)
		m_pyysIplastMap[bufid]->Reset();

	m_pyysIptxtbuf[bufid].Reset();

}

void IpQueryLib::ClearLocIpLib(long bufid)
{
	ASSERT(bufid == 0 || bufid == 1);
	for (int i = 0; i < 65536; i++)
	{
		if (m_ppIpMap_64k[bufid][i] != 0)
			m_ppIpMap_64k[bufid][i]->Reset();
	}
	
	for (i = 0; i < 256; i++)
	{
		if (m_ppIpMap_256[bufid][i] != 0)
			m_ppIpMap_256[bufid][i]->Reset();
	}

	if (m_pIplastMap[bufid] != NULL)
		m_pIplastMap[bufid]->Reset();

	m_pLocalIptxtbuf[bufid].Reset();
}

//**********************************************************
//	根据dbf文件(IpAddr.dbf)生成本地IP库
//**********************************************************
BOOL IpQueryLib::LoadLocalIPFromDbf(LPCTSTR dbfname)
{
int bcp_ipaddr_count=4;
int sip_pos,dip_pos,iptimepos,ipaddrpos;
	// 判断并解压RAR文件
	if (v_programtype==SYSTEM_SLEUTH)
	{
		sip_pos=0;
		dip_pos=1;
		iptimepos=3;
		ipaddrpos=2;
		if (stricmp(dbfname, "e:\\netspy\\para\\dce\\ipaddr.rar") == 0)
		{
			if (!UnRarDbf(dbfname, "e:\\netspy\\para\\dce\\rar", 180))
			{
				return FALSE;
			}
			dbfname = "e:\\netspy\\para\\dce\\rar\\ipaddr.bcp";
		}
	}
	else
	{
		bcp_ipaddr_count=7;
		sip_pos=0;
		dip_pos=2;
		ipaddrpos=4;
		iptimepos=5;
		dbfname = "e:\\netspy\\para\\dce\\localipmatch.bcp";
	}
	if (_access(dbfname,0) != 0)
	{
		WriteLog("dce","文件 %s 不存在!");
		return FALSE;
	}

	// 打开DBF文件
	CString cdxfname = dbfname;
	cdxfname.Replace(".dbf", ".cdx");

	// 获取可用的IP库缓冲区ID
	long bufid = (m_LocIpLibReadFlag==0)? 1 : 0;
	int ipcount=0;
	long tm = 0x7fffffff;

//	for (int i=0; i<2; i++, Sleep(500))	// 如果第一次读表失败就再试一次
	{
		try
		{
			// 清除缓冲区
			ClearLocIpLib(bufid);
			CSyncPara synPara;
			DWORD dealPos = 0;
			if(FALSE == synPara.LoadPara(dbfname, dealPos, TRUE))
			{
				return FALSE;
			}
			CStringArray arrField;
			BOOL bGetOne;
			CString addres;
			do{
				arrField.RemoveAll();
				bGetOne = synPara.GetOneRecord(dealPos, arrField);
				if(FALSE == bGetOne)
					break;
				if (arrField.GetSize()==7)//修改之前固定ip表
				{
					bcp_ipaddr_count=7;
					sip_pos=0;
					dip_pos=2;
					ipaddrpos=4;
					iptimepos=5;
					addres.Format("%s",arrField[ipaddrpos]);
				}
				else if (arrField.GetSize()==10)//增加轨迹时表结构更改 对照文件ipaddr =区县+场所
				{
					bcp_ipaddr_count=10;
					sip_pos=0;
					dip_pos=2;
					iptimepos=4;
//					ipaddrpos=5;
					addres.Format("%s%s",arrField[7],arrField[8]);
				}
				else
				{
					if (arrField.GetSize() > 4)
					{
						addres.Format("%s",arrField[ipaddrpos]);
					}
				}
				//ASSERT(arrField.GetSize() == bcp_ipaddr_count);
				if(arrField.GetSize() != bcp_ipaddr_count)
				{
//						bReadOK = FALSE;
					continue;
				}


				if (0 == (ipcount & 0xFFF))
				{
					CreateFlgFile();		// 每处理4096条记录检查一次心跳文件
				}

				long rectm = atoi(arrField[iptimepos]);

				if (rectm==0)
				{
					rectm=111111;
				}

			//	if (tm >= rectm)
				{
					
					InsertLocIPAddr(arrField[sip_pos], arrField[dip_pos],
						addres, rectm, bufid);
					ipcount++;
					tm = rectm;
					if (m_error != 0)
					{
						break;
					}
				}
// 				else
// 				{
// 					WriteLog("dce", "索引<REC_TIME>错误!");
// 					break;
// 				}

			}while(bGetOne);

			InterlockedExchange(&m_LocIpLibReadFlag, bufid);
			WriteLog("dce", "更新本地IP表成功， 共读入IP%d个", ipcount);
			return TRUE;
		}
		catch(...)
		{
			WriteLog("dce","LoadLocalIPFromDbf() Error!!!");
		}
	}
	WriteLog("dce", "更新本地IP表失败");
	return FALSE;
}
//**********************************************************
//	根据dbf文件(IpAddr.dbf)生成本地IP库
//**********************************************************
BOOL IpQueryLib::LoadyysIPFromDbf(LPCTSTR dbfname)
{
int bcp_ipaddr_count=4;
int sip_pos,dip_pos,iptimepos,ipaddrpos;
	// 判断并解压RAR文件
	if (v_programtype==SYSTEM_SLEUTH)
	{
		sip_pos=0;
		dip_pos=1;
		iptimepos=3;
		ipaddrpos=2;
		if (stricmp(dbfname, "e:\\netspy\\para\\dce\\ipaddr.rar") == 0)
		{
			if (!UnRarDbf(dbfname, "e:\\netspy\\para\\dce\\rar", 180))
			{
				return FALSE;
			}
			dbfname = "e:\\netspy\\para\\dce\\rar\\ipaddr.bcp";
		}
	}
	else
	{
		bcp_ipaddr_count=6;
		sip_pos=0;
		dip_pos=2;
		ipaddrpos=4;
		iptimepos=5;
		dbfname = "e:\\netspy\\para\\dce\\ipaddr.bcp";
	}
	if (_access(dbfname,0) != 0)
	{
		WriteLog("dce","文件 %s 不存在!");
		return FALSE;
	}

	// 打开DBF文件
	CString cdxfname = dbfname;
	cdxfname.Replace(".dbf", ".cdx");

	// 获取可用的IP库缓冲区ID
	long bufid = (m_yysLibReadFlag==0)? 1 : 0;
	int ipcount=0;
	long tm = 0x7fffffff;

//	for (int i=0; i<2; i++, Sleep(500))	// 如果第一次读表失败就再试一次
	{
		try
		{
			// 清除缓冲区
			ClearyysIpLib(bufid);
			CSyncPara synPara;
			DWORD dealPos = 0;
			if(FALSE == synPara.LoadPara(dbfname, dealPos, FALSE))
			{
				return FALSE;
			}
			CStringArray arrField;
			BOOL bGetOne;
			CString addres;
			do{
				arrField.RemoveAll();
				bGetOne = synPara.GetOneRecord(dealPos, arrField);
				if(FALSE == bGetOne)
					break;
				if (arrField.GetSize()==7)//修改之前固定ip表
				{
					bcp_ipaddr_count=7;
					sip_pos=0;
					dip_pos=2;
					ipaddrpos=4;
					iptimepos=5;
					addres.Format("%s",arrField[ipaddrpos]);
				}
				else if (arrField.GetSize()==10)//增加轨迹时表结构更改 对照文件ipaddr =区县+场所
				{
					bcp_ipaddr_count=10;
					sip_pos=0;
					dip_pos=2;
					iptimepos=4;
//					ipaddrpos=5;
					addres.Format("%s%s",arrField[7],arrField[8]);
				}
				else if(arrField.GetSize()==0)
				{
					continue;
				}
				else
				{
					addres.Format("%s",arrField[ipaddrpos]);
				}
				ASSERT(arrField.GetSize() == bcp_ipaddr_count);
				if(arrField.GetSize() != bcp_ipaddr_count)
				{
//						bReadOK = FALSE;
					continue;
				}


				if (0 == (ipcount & 0xFFF))
				{
					CreateFlgFile();		// 每处理4096条记录检查一次心跳文件
				}

				long rectm = atoi(arrField[iptimepos]);

				if (rectm==0)
				{
					rectm=111111;
				}

			//	if (tm >= rectm)
				{
					
					InsertyysIPAddr(arrField[sip_pos], arrField[dip_pos],
						addres, rectm, bufid);
					ipcount++;
					tm = rectm;
					if (m_error != 0)
					{
						break;
					}
				}
// 				else
// 				{
// 					WriteLog("dce", "索引<REC_TIME>错误!");
// 					break;
// 				}

			}while(bGetOne);

			InterlockedExchange(&m_yysLibReadFlag, bufid);
			WriteLog("dce", "更新本地IP表成功， 共读入IP%d个", ipcount);
			return TRUE;
		}
		catch(...)
		{
			WriteLog("dce","LoadLocalIPFromDbf() Error!!!");
		}
	}
	WriteLog("dce", "更新本地IP表失败");
	return FALSE;
}
//**********************************************************
//	根据dbf文件(AllIpAddr.dbf)生成全球IP库
//**********************************************************
BOOL IpQueryLib::LoadAllIPFromDbf(LPCTSTR dbfname)
{
int bcp_ipaddr_count=3;
int sip_pos,dip_pos,iptimepos,ipaddrpos;
	// 判断并解压RAR文件
	if (v_programtype==SYSTEM_SLEUTH)
	{
		sip_pos=0;
		dip_pos=1;
		iptimepos=3;
		ipaddrpos=2;
		if (stricmp(dbfname, "e:\\netspy\\para\\dce\\allipaddr.rar") == 0)
		{
			if (!UnRarDbf(dbfname, "e:\\netspy\\para\\dce\\rar", 180))
			{
				return FALSE;
			}
			dbfname = "e:\\netspy\\para\\dce\\rar\\allipaddr.bcp";
		}

	}
	else
	{
		bcp_ipaddr_count=3;
		sip_pos=0;
		dip_pos=1;
		ipaddrpos=2;
		//iptimepos=5;
		dbfname = "e:\\netspy\\para\\dce\\netipmatch.bcp";

	}

	if (_access(dbfname,0) != 0)
	{
		WriteLog("dce","文件 %s 不存在!");
		return FALSE;
	}

	// 打开DBF文件
	CString cdxfname = dbfname;
	cdxfname.Replace(".dbf", ".cdx");

	// 获取可用的IP库缓冲区ID
	long bufid = (m_AllIpLibReadFlag==0)? 1 : 0;
	int ipcount=0;
	long tm = 0x7fffffff;

//	for (int i=0; i<2; i++, Sleep(500))	// 如果第一次读表失败就再试一次
	{
		try
		{
			// 清除缓冲区
			ClearAllIpLib(bufid);
			CSyncPara synPara;
			DWORD dealPos = 0;
			if(FALSE == synPara.LoadPara(dbfname, dealPos, TRUE))
			{
				return FALSE;
			}
			CStringArray arrField;
			BOOL bGetOne;
			do{
				arrField.RemoveAll();
				bGetOne = synPara.GetAllIpOneRecord(dealPos, arrField);
				if(FALSE == bGetOne)
					break;
				ASSERT(arrField.GetSize() == bcp_ipaddr_count);
				if(arrField.GetSize() != bcp_ipaddr_count)
				{
//						bReadOK = FALSE;
					continue;
				}


				if (0 == (ipcount & 0xFFF))
				{
					CreateFlgFile();		// 每处理4096条记录检查一次心跳文件
				}

		//		long rectm = atoi(arrField[iptimepos]);
				long rectm = 0;
				if (rectm==0)
				{
					rectm=111111;
				}
				if (tm >= rectm)
				{
					
					InsertAllIPAddr(arrField[sip_pos], arrField[dip_pos],
						arrField[ipaddrpos], rectm, bufid);
					ipcount++;
					tm = rectm;
					if (m_error != 0)
					{
						break;
					}
				}
				else
				{
					tm = rectm;
					continue;
					WriteLog("dce", "索引<REC_TIME>错误!");
					break;
				}

			}while(bGetOne);

			InterlockedExchange(&m_AllIpLibReadFlag, bufid);
			WriteLog("dce", "更新NetIPMatch表成功， 共读入IP%d个", ipcount);
			return TRUE;
		}
		catch(...)
		{
			WriteLog("dce","LoadLocalIPFromDbf() Error!!!");
		}
	}
	WriteLog("dce", "更新本地IP表失败");
	return FALSE;
	// 判断并解压RAR文件
// 	if (stricmp(dbfname, "x:\\xxx\\para\\dce\\allipaddr.rar") == 0)
// 	{
// 		if (!UnRarDbf(dbfname, "x:\\xxx\\para\\dce\\rar", 180))
// 		{
// 			return FALSE;
// 		}
// 		dbfname = "x:\\xxx\\para\\dce\\rar\\allipaddr.dbf";
// 	}
// 
// 	// 打开DBF文件
// 	CString cdxfname = dbfname;
// 	cdxfname.Replace(".dbf", ".cdx");
// 	// 获取可用的IP库缓冲区ID
// 	long bufid = (m_AllIpLibReadFlag==0)? 1 : 0;
// 	for (int i=0; i<2; i++, Sleep(500))	// 如果第一次读表失败就再试一次
// 	{
// 		try
// 		{
// 			// 清缓冲区
// 			ClearAllIpLib(bufid);
// 
// 			Code4 code;
// 			CTableAccess ac_table;
// 			code.autoOpen = 0;
// 			code.lockAttempts = 7;
// 			Data4 data;
// 			ac_table.SetCode4(&code, &data);
// 			// 打开IP地址表
// 			if (!OpenDbfTable(code, data, dbfname))
// 				continue;
// 			
// 			// 打开索引
// 			if (!DeleteFile(cdxfname))
// 			{
// 				if (_access(cdxfname, 0) == 0)
// 				{
// 					WriteLog("dce", "删除索引文件%s失败!", cdxfname);
// 				}
// 			}
// 			Index4 index;
// 			CreateFlgFile();	// 创建心跳文件
// 			if (!OpenIndex(data, index, AllIp_tag, cdxfname))
// 				continue;
// 			ac_table.SetIndex4(&index);
// 			
// 			// 选择索引
// 			Tag4 ipTag(data, "BEGINIP");
// 			if (!ipTag.isValid())
// 			{
// 				WriteLog("dce", "索引<BEGINIP>无效!");
// 				continue;
// 			}
// 			data.select(ipTag);
// 			
// 			// 关联字段
// 			Field4	fields[3];
// 			static LPCTSTR FIELDSNAME[3] =
// 			{
// 				_T("BEGINIP"),	_T("ENDIP"),
// 				_T("IPAREA")
// 			};
// 			for (int j = 0; j < 3; j++)
// 			{
// 				if (r4success != fields[j].init(data, FIELDSNAME[j]))
// 				{
// 					WriteLog("dce", "关联全球IP表字段%s失败!", FIELDSNAME[j]);
// 					break;
// 				}
// 			}
// 			if (j < 3)
// 				continue;	// 关联字段失败
// 
// 
// 			// 获取所有的IP地址
// 			int reccount = data.recCount();
// 			int ipcount = 0;
// 			m_error = 0;
// 			CString bip = "zzzz";
// 
// 			for (int rc = data.top(); rc == r4success; rc = data.skip(), reccount--)
// 			{
// 				if (0 == (reccount & 0xFFF))
// 				{
// 					CreateFlgFile();		// 每处理4096条记录检查一次心跳文件
// 				}
// 				if (!data.deleted())
// 				{
// 					if (bip >= fields[0].str())
// 					{
// 						InsertAllIPAddr(fields[0].str(), fields[1].str(),
// 							fields[2].str(), bufid);
// 						ipcount++;
// 						bip = fields[0].str();
// 						if (m_error != 0)
// 							break;
// 					}
// 					else
// 					{
// 						WriteLog("dce", "索引<BEGINIP>错误!");
// 						break;
// 					}
// 				}
// 			}
// 			if (reccount == 0)
// 			{
// 				InterlockedExchange(&m_AllIpLibReadFlag, bufid);
// 				WriteLog("dce", "更新全球IP表成功， 共读入IP%d个", ipcount);
// 				return TRUE;
// 			}
// 			
// 		}
// 		catch(...)
// 		{
// 			WriteLog("dce","LoadAllIPFromDbf() Error!!!");
// 		}
// 	}
// 	WriteLog("dce", "更新全球IP表失败");
	return FALSE;
}

//**********************************************************
//	查询IP的所属位置
//**********************************************************
CString IpQueryLib::QueryIpAddr(LPCTSTR strip)
{
	int num;
	int ipc1, ipc2, ipc3, ipc4; 
	
	// 构造IP单元对象
	num = sscanf(strip,"%d.%d.%d.%d",&ipc1,&ipc2,&ipc3,&ipc4);
	if ((num != 4) || (ipc1 < 0) || (ipc1 > 255) ||
		(ipc2 < 0) || (ipc2 > 255) || (ipc3 < 0) ||
		(ipc3 > 255) || (ipc4 < 0) || (ipc4 > 255))
	{
		num = sscanf(strip,"%d_%d_%d_%d",&ipc1,&ipc2,&ipc3,&ipc4);
		if ((num != 4) || (ipc1 < 0) || (ipc1 > 255) ||
			(ipc2 < 0) || (ipc2 > 255) || (ipc3 < 0) ||
			(ipc3 > 255) || (ipc4 < 0) || (ipc4 > 255))
		return "";
	}
	DWORD uip = (ipc1 << 24) + (ipc2 << 16) + (ipc3 << 8) + ipc4;
	return QueryIpAddr(uip);
}

/*!\fn ConvertIP
 * \brief 把一个字符串IP转为整型
*/
DWORD ConvertIP(CString strip)
{
	int num;
	int ipc1, ipc2, ipc3, ipc4; 
	
	// 构造IP单元对象
	num = sscanf(strip,"%d.%d.%d.%d",&ipc1,&ipc2,&ipc3,&ipc4);
	if ((num != 4) || (ipc1 < 0) || (ipc1 > 255) ||
		(ipc2 < 0) || (ipc2 > 255) || (ipc3 < 0) ||
		(ipc3 > 255) || (ipc4 < 0) || (ipc4 > 255))
	{
		num = sscanf(strip,"%d_%d_%d_%d",&ipc1,&ipc2,&ipc3,&ipc4);
		if ((num != 4) || (ipc1 < 0) || (ipc1 > 255) ||
			(ipc2 < 0) || (ipc2 > 255) || (ipc3 < 0) ||
			(ipc3 > 255) || (ipc4 < 0) || (ipc4 > 255))
		return 0;
	}
	DWORD uip = (ipc1 << 24) + (ipc2 << 16) + (ipc3 << 8) + ipc4;
	return uip;
}

class IPUnit
{
public:
	CStringArray m_ip;
	CStringArray m_netmask;
	CString desc;
};

CString  GetAdpterGUID(CString  cardname)
{
	CString res;
	HKEY hLocKey;
	HKEY hLocKey1;
	
	char  lpName[500];
	memset(lpName,0,500);
	char  lpSubKeyName[500];
	int i = 0;
	char  lpValue[500];
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
		"System\\CurrentControlSet\\Control\\NetWork\\{4D36E972-E325-11CE-BFC1-08002BE10318}\\", 
		0, KEY_READ, &hLocKey)==ERROR_SUCCESS)
	{
		long lCode = ERROR_SUCCESS;
		BOOL bExit = FALSE;	
		while(lCode == ERROR_SUCCESS && !bExit)
		{
			lCode = RegEnumKey(hLocKey, i++, lpName, 300);
			if(lCode ==ERROR_SUCCESS)
			{
				sprintf(lpSubKeyName, 
					"System\\CurrentControlSet\\Control\\NetWork\\{4D36E972-E325-11CE-BFC1-08002BE10318}\\%s\\Connection\\", 
					lpName);
				if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
					lpSubKeyName, 0, KEY_READ, &hLocKey1)==ERROR_SUCCESS)
				{
					//printf("Now is RegQueryValueEx.\n");
					unsigned long num=300;
					DWORD ValueType;
					if(RegQueryValueEx(	hLocKey1, "Name", NULL, &ValueType,
						(unsigned char*)lpValue, &num)==ERROR_SUCCESS)
						//AfxMessageBox(lpValue);
					{
						if(cardname.CompareNoCase(lpValue)==0)
						{
							res.Format("%s",lpName);
							RegCloseKey(hLocKey1);
							break; 
						}
					 
					}
					RegCloseKey(hLocKey1);
				}
			}
			else if(lCode == ERROR_NO_MORE_ITEMS)
			{
				bExit = TRUE;
			}
		}
		RegCloseKey(hLocKey);
	}
	else
	{
		TRACE( "Error to Open Key System\\CurrentControlSet\\Control\\NetWork\\{4D36E972-E325-11CE-BFC1-08002BE10318}.");
		 
	}
	return res;
}

BOOL GetSPENetGateIp(IPUnit &ipunit,CString cardguid)
{
	ULONG len	=0;
	int datalen	=0;
	int ilen	=0;
	PIP_ADAPTER_INFO pinfo=NULL;
	PIP_ADAPTER_INFO b_pinfo=NULL;

	ULONG nError  = GetAdaptersInfo(pinfo,&len);  
//     if (nError==ERROR_NO_DATA || nError==ERROR_NOT_SUPPORTED)
//     {
// 		return FALSE;
//     }
//     
//     if (nError==ERROR_BUFFER_OVERFLOW)
//     {
//         pinfo	= (PIP_ADAPTER_INFO)malloc(len);
//         nError  = GetAdaptersInfo(pinfo,&len);
//     }
	if (nError!=ERROR_BUFFER_OVERFLOW)
    {
		return FALSE;
    }
    
//    pinfo	= (PIP_ADAPTER_INFO)malloc(len);
	pinfo =(PIP_ADAPTER_INFO) HeapAlloc(GetProcessHeap(), 0, len);
    nError  = GetAdaptersInfo(pinfo,&len);
	
	if (nError!=ERROR_SUCCESS)
	{
		HeapFree(GetProcessHeap(), 0, pinfo);
		return FALSE;
	}
	if(nError==0)
	{
		char * ipaddress=NULL;
		char * netmask=NULL;
		IP_ADDR_STRING *paddr=NULL;
		CString strV;
		b_pinfo=pinfo;
		while(pinfo!=NULL)
		{
			if(cardguid.CompareNoCase(pinfo->AdapterName)!=0)
			{
				pinfo = pinfo->Next;
				continue;
			}


			ipaddress=pinfo->IpAddressList.IpAddress.String;
			netmask=pinfo->IpAddressList.IpMask.String;
			if(ipaddress!=NULL)
			{
				if(strlen(ipaddress) > 0)
				{
					ipunit.m_ip.Add(ipaddress);
					ipunit.m_netmask.Add(netmask);
					strV.Format("ip %s, mask %s", ipaddress, netmask);
					ipunit.desc += strV;

					paddr=pinfo->IpAddressList.Next;
					while (paddr!=NULL)
					{
						ipaddress=paddr->IpAddress.String;
						netmask=paddr->IpMask.String;
						ipunit.m_ip.Add(ipaddress);
						ipunit.m_netmask.Add(netmask);
						strV.Format("ip %s, mask %s ", ipaddress, netmask);
						ipunit.desc += strV;

						paddr=paddr->Next;
					}
				}
			}
			HeapFree(GetProcessHeap(), 0, b_pinfo);

			return TRUE;
		}
	}
	HeapFree(GetProcessHeap(), 0, b_pinfo);
	return FALSE;
}

/*!\fn CheckFyyLan
 * \brief 检查uip是否fyy内网IP
 读 e:\\netspy\\para\\card.ini
[lan]
ip=192.168.0.149
netmask=255.255.255.0
scope=192.168.0.0
gateway=192.168.0.60

  uip & netmask == scope 返回TRUE, 否则返回FASLE
*/
BOOL CheckFyyLan(DWORD uip)
{
	static time_t tcheck = 0;
	static IPUnit staticipunit;
	if(staticipunit.m_ip.GetSize() == 0 && abs(time(0)-tcheck) > 60*60)
	{
		CString guid=GetAdpterGUID("lan");
		IPUnit tmpipunit;
		BOOL flg=GetSPENetGateIp(tmpipunit, guid);
		time(&tcheck);
		if(!flg)
		{
			WriteLog("dce", "GetLanIP lan口ip不能获取!!");
			return FALSE;
		}
		WriteLog("dce", "GetLanIP lan口ip!!");
		if(staticipunit.desc != tmpipunit.desc)
		{
			staticipunit.m_ip.RemoveAll();
			staticipunit.m_ip.Append(tmpipunit.m_ip);
			staticipunit.m_netmask.RemoveAll();
			staticipunit.m_netmask.Append(tmpipunit.m_netmask);
			staticipunit.desc = tmpipunit.desc;
			WriteLog("dce", "GetLanIP %s", staticipunit.desc);
		}

	}
	DWORD ttip, ttmask;		/// 临时变量
	for (int ii = 0; ii< staticipunit.m_ip.GetSize(); ii++)
	{
		ttip = ConvertIP(staticipunit.m_ip[ii]);
		if(ttip == 0)
			return FALSE;
		
		ttmask = ConvertIP(staticipunit.m_netmask[ii]);
		if(ttmask == 0)
			return FALSE;


		if(ttip > 0 && ttmask > 0)
		{
			if((ttip&ttmask) == (uip&ttmask))
				return TRUE;
		}
	}
	return FALSE;
}
//**********************************************************
//	查询IP的所属位置
//**********************************************************
BOOL IpQueryLib::GetLocalWebSite(LPCTSTR strip)
{
//	return FALSE;
	int num;
	int ipc1, ipc2, ipc3, ipc4; 
	
	// 构造IP单元对象
	num = sscanf(strip,"%d.%d.%d.%d",&ipc1,&ipc2,&ipc3,&ipc4);
	if ((num != 4) || (ipc1 < 0) || (ipc1 > 255) ||
		(ipc2 < 0) || (ipc2 > 255) || (ipc3 < 0) ||
		(ipc3 > 255) || (ipc4 < 0) || (ipc4 > 255))
	{
		num = sscanf(strip,"%d_%d_%d_%d",&ipc1,&ipc2,&ipc3,&ipc4);
		if ((num != 4) || (ipc1 < 0) || (ipc1 > 255) ||
			(ipc2 < 0) || (ipc2 > 255) || (ipc3 < 0) ||
			(ipc3 > 255) || (ipc4 < 0) || (ipc4 > 255))
		return FALSE;
	}
	DWORD uip = (ipc1 << 24) + (ipc2 << 16) + (ipc3 << 8) + ipc4;
	return GetLocalWebSite(uip);
}

BOOL IpQueryLib::GetLocalWebSite(DWORD uip)
{
	IPUNIT ipunit;
	ipunit.rectm = 0;
	ipunit.ptext = pEmpty;

	// Search Local IPLib
	DWORD arridx64k = uip >> 16;
	if (m_ppIpMap_64k[m_LocIpLibReadFlag][arridx64k] != NULL)
	{
		if(m_ppIpMap_64k[m_LocIpLibReadFlag][arridx64k]->QueryIP(uip, ipunit))
		{
			return TRUE;
		}
	}
	DWORD arridx256 = uip >> 24;
	if ((m_ppIpMap_256[m_LocIpLibReadFlag][arridx256] != NULL) &&
		(m_ppIpMap_256[m_LocIpLibReadFlag][arridx256]->newtime> ipunit.rectm))
	{
		if(m_ppIpMap_256[m_LocIpLibReadFlag][arridx256]->QueryIP(uip, ipunit))
		{
			return TRUE;
		}
	}
	if ((m_pIplastMap[m_LocIpLibReadFlag] != NULL) &&
		(m_pIplastMap[m_LocIpLibReadFlag]->newtime > ipunit.rectm))
	{
		if(m_pIplastMap[m_LocIpLibReadFlag]->QueryIP(uip, ipunit))
		{
			return TRUE;
		}
	}

		// Search All IPLib
	if (m_ppyysIpMap_64k[m_yysLibReadFlag][arridx64k] != NULL)
	{
		if (m_ppyysIpMap_64k[m_yysLibReadFlag][arridx64k]->QueryIP(uip, ipunit))
			return TRUE;
	}
	if (m_ppyysIpMap_256[m_yysLibReadFlag][arridx256] != NULL)
	{
		if (m_ppyysIpMap_256[m_yysLibReadFlag][arridx256]->QueryIP(uip, ipunit))
			return TRUE;
	}
	if (m_pyysIplastMap[m_yysLibReadFlag] != NULL)
	{
		if (m_pyysIplastMap[m_yysLibReadFlag]->QueryIP(uip, ipunit))
			return TRUE;
	}

	return FALSE;
}
CString IpQueryLib::QueryIpAddr(DWORD uip)
{
	IPUNIT ipunit;
	ipunit.rectm = 0;
	ipunit.ptext = pEmpty;

	// Search Local IPLib
	DWORD arridx64k = uip >> 16;
	if (m_ppIpMap_64k[m_LocIpLibReadFlag][arridx64k] != NULL)
	{
		m_ppIpMap_64k[m_LocIpLibReadFlag][arridx64k]->QueryIP(uip, ipunit);
	}
	DWORD arridx256 = uip >> 24;
	if ((m_ppIpMap_256[m_LocIpLibReadFlag][arridx256] != NULL) &&
		(m_ppIpMap_256[m_LocIpLibReadFlag][arridx256]->newtime> ipunit.rectm))
	{
		m_ppIpMap_256[m_LocIpLibReadFlag][arridx256]->QueryIP(uip, ipunit);
	}
	if ((m_pIplastMap[m_LocIpLibReadFlag] != NULL) &&
		(m_pIplastMap[m_LocIpLibReadFlag]->newtime > ipunit.rectm))
	{
		m_pIplastMap[m_LocIpLibReadFlag]->QueryIP(uip, ipunit);
	}

//	m_AllIpLibReadFlag=1;
	if (ipunit.rectm <= 0 && ipunit.ptext=="")
	{
		if(v_programtype == SYSTEM_FYY||
		v_programtype == SYSTEM_DEPARTMENT||
		v_programtype == SYSTEM_HOTEL)
		{
			if(CheckFyyLan(uip))
				return "局域网IP";
		}

		// Search All IPLib
		if (m_ppAllIpMap_64k[m_AllIpLibReadFlag][arridx64k] != NULL)
		{
			if (m_ppAllIpMap_64k[m_AllIpLibReadFlag][arridx64k]->QueryIP(uip, ipunit))
				return ipunit.ptext;
		}
		if (m_ppAllIpMap_256[m_AllIpLibReadFlag][arridx256] != NULL)
		{
			if (m_ppAllIpMap_256[m_AllIpLibReadFlag][arridx256]->QueryIP(uip, ipunit))
				return ipunit.ptext;
		}
		if (m_pAllIplastMap[m_AllIpLibReadFlag] != NULL)
		{
			if (m_pAllIplastMap[m_AllIpLibReadFlag]->QueryIP(uip, ipunit))
				return ipunit.ptext;
		}
	}

	return ipunit.ptext;
}

//**********************************************************
//	插入一个IP段到IP数组中
//**********************************************************
void IpQueryLib::InsertAllIP(LPIPARRAY piparray, LPIPUNIT pipunit, int& error)
{
	ASSERT(piparray != NULL);
	if (piparray->unitcount == 0)
	{
		if (piparray->maxipnum == 0)
		{
			piparray->piparray = new IPUNIT[IPARRAYBLOCK];
			piparray->maxipnum = IPARRAYBLOCK;
		}
		*piparray->piparray = *pipunit;
		piparray->unitcount=1;
		return;
	}

	// 找到链表中最后一个不为空的块 --> piparray
	LPIPARRAY tmparray = piparray->pnext;
	while(tmparray != NULL)
	{
		if (tmparray->unitcount == 0)
			break;
		if (tmparray->pnext == NULL)
		{
			piparray = tmparray;
			break;
		}
		tmparray = tmparray->pnext;
	}

	int ins = -1;
	while(1)
	{
		int tmpins = CheckIPUnit(piparray, pipunit);
		if (tmpins >= 0)
		{
			ins = tmpins;
			if (piparray->pPrev != NULL)
			{
				piparray = piparray->pPrev;
			}
			else
			{
				// 插入到piparray单元，位置为tmpins
 				if (piparray->maxipnum == piparray->unitcount)
				{
 					// 数组已经满了
 					piparray->maxipnum += min(1024, max(IPARRAYBLOCK, piparray->unitcount / 8));
 					ASSERT(piparray->maxipnum > piparray->unitcount);
 					LPIPUNIT tp = new IPUNIT[piparray->maxipnum];
					if ((tmpins>=0) && (tmpins <= piparray->unitcount))
					{
						memcpy(tp, piparray->piparray, tmpins*sizeof(IPUNIT));
					}
					else
					{
						tmpins = 0;
					}
					memcpy(tp+tmpins, pipunit, sizeof(IPUNIT));
					memcpy(tp+tmpins+1, piparray->piparray+tmpins, (piparray->unitcount-tmpins)*sizeof(IPUNIT));
					delete[] piparray->piparray;
					piparray->piparray = tp;
				}
				else
				{
					if ((tmpins>=0) && (tmpins <= piparray->unitcount))
						memmove(piparray->piparray+tmpins+1, piparray->piparray+tmpins,
							(piparray->unitcount-tmpins)*sizeof(IPUNIT));
					else
						tmpins = 0;
					memcpy(piparray->piparray+tmpins, pipunit, sizeof(IPUNIT));
				}
				piparray->unitcount++;
				return;
			}
		}
		else if (tmpins == -1)
		{
			// 插入到piparray的下一个单元，位置为ins
			tmparray = piparray->pnext;
			if (tmparray == NULL)
			{
				tmparray = new IPARRAY;
				piparray->pnext = tmparray;
				tmparray->pPrev = piparray;
			}
 			if (tmparray->maxipnum == tmparray->unitcount)
			{
 				// 数组已经满了
 				tmparray->maxipnum += min(1024, max(IPARRAYBLOCK, tmparray->unitcount / 8));
 				ASSERT(tmparray->maxipnum > tmparray->unitcount);
 				LPIPUNIT tp = new IPUNIT[tmparray->maxipnum];
				if ((ins>=0) && (ins <= tmparray->unitcount))
				{
					memcpy(tp, tmparray->piparray, ins*sizeof(IPUNIT));
				}
				else
				{
					ins = 0;
				}
				memcpy(tp+ins, pipunit, sizeof(IPUNIT));
				memcpy(tp+ins+1, tmparray->piparray+ins, (tmparray->unitcount-ins)*sizeof(IPUNIT));
 				memset(tmparray->piparray, 0, tmparray->unitcount*sizeof(IPUNIT));
 				delete[] tmparray->piparray;
				tmparray->piparray = tp;
			}
			else
			{
				if ((ins>=0) && (ins <= tmparray->unitcount))
					memmove(tmparray->piparray+ins+1, tmparray->piparray+ins,
						(tmparray->unitcount-ins)*sizeof(IPUNIT));
				else
					ins = 0;
				memcpy(tmparray->piparray+ins, pipunit, sizeof(IPUNIT));
			}
			tmparray->unitcount++;
			return;
		}
		else
		{
			// IP段重复
			return;
		}
	}
}

int IpQueryLib::CheckIPUnit(LPIPARRAY piparray, LPIPUNIT pipunit)
{
	ASSERT(piparray->unitcount > 0);
	LPIPUNIT tpiparray = piparray->piparray;
	for (int ins = 0; ins < piparray->unitcount; ins++, tpiparray++)
	{
		if (pipunit->endip < tpiparray->begip)
			continue;
		else if (pipunit->begip > tpiparray->endip)
			return ins;		// 没有冲突 可以插入
		else if ((pipunit->begip == tpiparray->begip) &&
			(pipunit->endip == tpiparray->endip))
			return -2;	// IP段重复   丢弃
		else
			return -1;	// IP段有交叉 不能插入
	}
	return ins;
}


void IpQueryLib::InsertLocIP(LPIPARRAY piparray, LPIPUNIT pipunit, int& error)
{
	ASSERT(piparray != NULL);
	if (piparray->unitcount == 0)
	{
		if (piparray->maxipnum == 0)
		{
			piparray->piparray = new IPUNIT[IPARRAYBLOCK];
			piparray->maxipnum = IPARRAYBLOCK;
		}
		*piparray->piparray = *pipunit;
		piparray->unitcount=1;
		return;
	}

	// 找到链表中最后一个不为空的块 --> piparray
	LPIPARRAY tmparray = piparray->pnext;
	while(tmparray != NULL)
	{
		if (tmparray->unitcount == 0)
			break;
		if (tmparray->pnext == NULL)
		{
			piparray = tmparray;
			break;
		}
		tmparray = tmparray->pnext;
	}

	int ins = -1;
	while(1)
	{
		int tmpins = CheckIPUnit(piparray, pipunit);
		if (tmpins >= 0)
		{
			ins = tmpins;
			if (piparray->pPrev != NULL)
			{
				piparray = piparray->pPrev;
			}
			else
			{
				// 插入到piparray单元，位置为tmpins
 				if (piparray->maxipnum == piparray->unitcount)
				{
 					// 数组已经满了
 					piparray->maxipnum += min(1024, max(IPARRAYBLOCK, piparray->unitcount / 8));
 					ASSERT(piparray->maxipnum > piparray->unitcount);
 					LPIPUNIT tp = new IPUNIT[piparray->maxipnum];
					if ((tmpins>=0) && (tmpins <= piparray->unitcount))
					{
						memcpy(tp, piparray->piparray, tmpins*sizeof(IPUNIT));
					}
					else
					{
						tmpins = 0;
					}
					memcpy(tp+tmpins, pipunit, sizeof(IPUNIT));
					memcpy(tp+tmpins+1, piparray->piparray+tmpins, (piparray->unitcount-tmpins)*sizeof(IPUNIT));
					delete[] piparray->piparray;
					piparray->piparray = tp;
				}
				else
				{
					if ((tmpins>=0) && (tmpins <= piparray->unitcount))
						memmove(piparray->piparray+tmpins+1, piparray->piparray+tmpins,
							(piparray->unitcount-tmpins)*sizeof(IPUNIT));
					else
						tmpins = 0;
					memcpy(piparray->piparray+tmpins, pipunit, sizeof(IPUNIT));
				}
				piparray->unitcount++;
				return;
			}
		}
		else if (tmpins == -1)
		{
			// 插入到piparray的下一个单元，位置为ins
			tmparray = piparray->pnext;
			if (tmparray == NULL)
			{
				tmparray = new IPARRAY;
				piparray->pnext = tmparray;
				tmparray->pPrev = piparray;
			}
 			if (tmparray->maxipnum == tmparray->unitcount)
			{
 				// 数组已经满了
 				tmparray->maxipnum += min(1024, max(IPARRAYBLOCK, tmparray->unitcount / 8));
 				ASSERT(tmparray->maxipnum > tmparray->unitcount);
 				LPIPUNIT tp = new IPUNIT[tmparray->maxipnum];
				if ((ins>=0) && (ins <= tmparray->unitcount))
				{
					memcpy(tp, tmparray->piparray, ins*sizeof(IPUNIT));
				}
				else
				{
					ins = 0;
				}
				memcpy(tp+ins, pipunit, sizeof(IPUNIT));
				memcpy(tp+ins+1, tmparray->piparray+ins, (tmparray->unitcount-ins)*sizeof(IPUNIT));
 				memset(tmparray->piparray, 0, tmparray->unitcount*sizeof(IPUNIT));
 				delete[] tmparray->piparray;
				tmparray->piparray = tp;
			}
			else
			{
				if ((ins>=0) && (ins <= tmparray->unitcount))
					memmove(tmparray->piparray+ins+1, tmparray->piparray+ins,
						(tmparray->unitcount-ins)*sizeof(IPUNIT));
				else
					ins = 0;
				memcpy(tmparray->piparray+ins, pipunit, sizeof(IPUNIT));
			}
			tmparray->unitcount++;
			return;
		}
		else
		{
			// IP段重复
			return;
		}
	}
}

//**********************************************************
//	组织一个IP段单元并将它插入到IP数组中
//**********************************************************
void IpQueryLib::InsertAllIPAddr(LPCTSTR begip, LPCTSTR endip, LPCTSTR txt, long rectm, long bufid)
//(LPCTSTR begip, LPCTSTR endip, LPCTSTR txt, long bufid)
{
	if (rectm <= 0)
		return;
	IPUNIT ipunit;
	int num;
	int ipc1, ipc2, ipc3, ipc4; 

	// 构造IP单元对象
	num = sscanf(begip,"%d.%d.%d.%d",&ipc1,&ipc2,&ipc3,&ipc4);
	if ((num != 4) || (ipc1 < 0) || (ipc1 > 255) ||
		(ipc2 < 0) || (ipc2 > 255) || (ipc3 < 0) ||
		(ipc3 > 255) || (ipc4 < 0) || (ipc4 > 255))
		return;
	ipunit.begip = (ipc1 << 24) + (ipc2 << 16) + (ipc3 << 8) + ipc4;

	num = sscanf(endip,"%d.%d.%d.%d",&ipc1,&ipc2,&ipc3,&ipc4);
	if ((num != 4) || (ipc1 < 0) || (ipc1 > 255) ||
		(ipc2 < 0) || (ipc2 > 255) || (ipc3 < 0) ||
		(ipc3 > 255) || (ipc4 < 0) || (ipc4 > 255))
		return;
	ipunit.endip = (ipc1 << 24) + (ipc2 << 16) + (ipc3 << 8) + ipc4;

	ipunit.rectm = rectm;
	
	if (ipunit.endip < ipunit.begip)
	{
		DWORD uip = ipunit.endip;
		ipunit.endip = ipunit.begip;
		ipunit.begip = uip;
	}

	if(strlen(txt) > 50)
	{
		return ;
	}
	ipunit.ptext = m_pAllIptxtbuf[bufid].AppendIPAreaText(txt);

	if (ipunit.ptext[0] == 0)
		return;
	// 插入IP单元到数组
	if ((ipunit.begip & 0xFFFF0000) == (ipunit.endip & 0xFFFF0000))
	{
		DWORD arayidx = ipunit.begip>>16;
		if (m_ppAllIpMap_64k[bufid][arayidx] == 0)
			m_ppAllIpMap_64k[bufid][arayidx] = new IPARRAY;
		InsertAllIP(m_ppAllIpMap_64k[bufid][arayidx], &ipunit, m_error);
	}
	else if ((ipunit.begip & 0xFF000000) == (ipunit.endip & 0xFF000000))
	{
		DWORD arayidx = ipunit.begip>>24;
		if (m_ppAllIpMap_256[bufid][arayidx] == 0)
			m_ppAllIpMap_256[bufid][arayidx] = new IPARRAY;
		InsertAllIP(m_ppAllIpMap_256[bufid][arayidx], &ipunit, m_error);
	}
	else
	{
		if (m_pAllIplastMap[bufid] == 0)
			m_pAllIplastMap[bufid] = new IPARRAY;
		InsertAllIP(m_pAllIplastMap[bufid], &ipunit, m_error);
	}
}

void IpQueryLib::InsertLocIPAddr(LPCTSTR begip, LPCTSTR endip, LPCTSTR txt, long rectm, long bufid)
{
// 	try
// 	{
		if (rectm <= 0)
			return;
		IPUNIT ipunit;
		int num;
		int ipc1, ipc2, ipc3, ipc4; 

		// 构造IP单元对象
		num = sscanf(begip,"%d.%d.%d.%d",&ipc1,&ipc2,&ipc3,&ipc4);
		if ((num != 4) || (ipc1 < 0) || (ipc1 > 255) ||
			(ipc2 < 0) || (ipc2 > 255) || (ipc3 < 0) ||
			(ipc3 > 255) || (ipc4 < 0) || (ipc4 > 255))
			return;
		ipunit.begip = (ipc1 << 24) + (ipc2 << 16) + (ipc3 << 8) + ipc4;

		num = sscanf(endip,"%d.%d.%d.%d",&ipc1,&ipc2,&ipc3,&ipc4);
		if ((num != 4) || (ipc1 < 0) || (ipc1 > 255) ||
			(ipc2 < 0) || (ipc2 > 255) || (ipc3 < 0) ||
			(ipc3 > 255) || (ipc4 < 0) || (ipc4 > 255))
			return;
		ipunit.endip = (ipc1 << 24) + (ipc2 << 16) + (ipc3 << 8) + ipc4;

		ipunit.rectm = rectm;
		
		if (ipunit.endip < ipunit.begip)
		{
			DWORD uip = ipunit.endip;
			ipunit.endip = ipunit.begip;
			ipunit.begip = uip;
		}

		if(strlen(txt) > 50)
		{
			return ;
		}
		ipunit.ptext = m_pLocalIptxtbuf[bufid].AppendIPAreaText(txt);

		if (ipunit.ptext[0] == 0)
			return;
		// 插入IP单元到数组
		if ((ipunit.begip & 0xFFFF0000) == (ipunit.endip & 0xFFFF0000))
		{
			DWORD arayidx = ipunit.begip>>16;
			if (m_ppIpMap_64k[bufid][arayidx] == 0)
				m_ppIpMap_64k[bufid][arayidx] = new IPARRAY;
			InsertLocIP(m_ppIpMap_64k[bufid][arayidx], &ipunit, m_error);
			if ((m_error == 0) && (m_ppIpMap_64k[bufid][arayidx]->newtime < rectm))
				m_ppIpMap_64k[bufid][arayidx]->newtime = rectm;
		}
		else if ((ipunit.begip & 0xFF000000) == (ipunit.endip & 0xFF000000))
		{
			DWORD arayidx = ipunit.begip>>24;
			if (m_ppIpMap_256[bufid][arayidx] == 0)
				m_ppIpMap_256[bufid][arayidx] = new IPARRAY;
			InsertLocIP(m_ppIpMap_256[bufid][arayidx], &ipunit, m_error);
			if ((m_error == 0) && (m_ppIpMap_256[bufid][arayidx]->newtime < rectm))
				m_ppIpMap_256[bufid][arayidx]->newtime = rectm;
		}
		else
		{
			if (m_pIplastMap[bufid] == 0)
				m_pIplastMap[bufid] = new IPARRAY;
			InsertLocIP(m_pIplastMap[bufid], &ipunit, m_error);
			if ((m_error == 0) && (m_pIplastMap[bufid]->newtime < rectm))
				m_pIplastMap[bufid]->newtime = rectm;
		}
// 	}
// 	catch (...)
// 	{
// 		WriteLog("dce", "error");
// 	}
}
void IpQueryLib::InsertyysIPAddr(LPCTSTR begip, LPCTSTR endip, LPCTSTR txt, long rectm, long bufid)
{
// 	try
// 	{
		if (rectm <= 0)
			return;
		IPUNIT ipunit;
		int num;
		int ipc1, ipc2, ipc3, ipc4; 

		// 构造IP单元对象
		num = sscanf(begip,"%d.%d.%d.%d",&ipc1,&ipc2,&ipc3,&ipc4);
		if ((num != 4) || (ipc1 < 0) || (ipc1 > 255) ||
			(ipc2 < 0) || (ipc2 > 255) || (ipc3 < 0) ||
			(ipc3 > 255) || (ipc4 < 0) || (ipc4 > 255))
			return;
		ipunit.begip = (ipc1 << 24) + (ipc2 << 16) + (ipc3 << 8) + ipc4;

		num = sscanf(endip,"%d.%d.%d.%d",&ipc1,&ipc2,&ipc3,&ipc4);
		if ((num != 4) || (ipc1 < 0) || (ipc1 > 255) ||
			(ipc2 < 0) || (ipc2 > 255) || (ipc3 < 0) ||
			(ipc3 > 255) || (ipc4 < 0) || (ipc4 > 255))
			return;
		ipunit.endip = (ipc1 << 24) + (ipc2 << 16) + (ipc3 << 8) + ipc4;

		ipunit.rectm = rectm;
		
		if (ipunit.endip < ipunit.begip)
		{
			DWORD uip = ipunit.endip;
			ipunit.endip = ipunit.begip;
			ipunit.begip = uip;
		}

		if(strlen(txt) > 50)
		{
			return ;
		}
		ipunit.ptext = m_pyysIptxtbuf[bufid].AppendIPAreaText(txt);

		if (ipunit.ptext[0] == 0)
			return;
		// 插入IP单元到数组
		if ((ipunit.begip & 0xFFFF0000) == (ipunit.endip & 0xFFFF0000))
		{
			DWORD arayidx = ipunit.begip>>16;
			if (m_ppyysIpMap_64k[bufid][arayidx] == 0)
				m_ppyysIpMap_64k[bufid][arayidx] = new IPARRAY;
			InsertLocIP(m_ppyysIpMap_64k[bufid][arayidx], &ipunit, m_error);
			if ((m_error == 0) && (m_ppyysIpMap_64k[bufid][arayidx]->newtime < rectm))
				m_ppyysIpMap_64k[bufid][arayidx]->newtime = rectm;
		}
		else if ((ipunit.begip & 0xFF000000) == (ipunit.endip & 0xFF000000))
		{
			DWORD arayidx = ipunit.begip>>24;
			if (m_ppyysIpMap_256[bufid][arayidx] == 0)
				m_ppyysIpMap_256[bufid][arayidx] = new IPARRAY;
			InsertLocIP(m_ppyysIpMap_256[bufid][arayidx], &ipunit, m_error);
			if ((m_error == 0) && (m_ppyysIpMap_256[bufid][arayidx]->newtime < rectm))
				m_ppyysIpMap_256[bufid][arayidx]->newtime = rectm;
		}
		else
		{
			if (m_pyysIplastMap[bufid] == 0)
				m_pyysIplastMap[bufid] = new IPARRAY;
			InsertLocIP(m_pyysIplastMap[bufid], &ipunit, m_error);
			if ((m_error == 0) && (m_pyysIplastMap[bufid]->newtime < rectm))
				m_pyysIplastMap[bufid]->newtime = rectm;
		}
// 	}
// 	catch (...)
// 	{
// 		WriteLog("dce", "error");
// 	}
}
// IpAreaBuffer::IpAreaBuffer()
// {
// 	m_readarith[0] = 0;
// 	m_readarith[1] = 0;
// 	m_readflg = -1;
// }
// 
// IpAreaBuffer::~IpAreaBuffer()
// {
// 
// }
// 
// void checkdbffile(LPCTSTR fname, CString& ftime)
// {
// 	struct _stat fst;
// 	if (0 == _stat(fname, &fst))
// 		ftime.Format("%s_%u", CTime(fst.st_mtime).Format("%Y%m%d%H%M%S"), fst.st_size);
// 	else
// 		ftime.Empty();
// }
// 
// void IpAreaBuffer::UpdateIpAddr()
// {
// 	static LPCTSTR allipaddrtable = _T("x:\\xxx\\Para\\dce\\allipaddr.dbf");
// 	static LPCTSTR localipaddrtable = _T("x:\\xxx\\para\\dce\\ipaddr.dbf");
// 	try
// 	{
// 		// 检查本地的文件
// 		CString ftime;
// 		checkdbffile(ipaddrtable, ftime);
// 		if (!ftime.IsEmpty() && m_tabletime != ftime)
// 		{
// 			WriteLog("dce", "更新IP地址表: %s != %s", m_tabletime, ftime);
// 			// 更新IP地址表
// 			int bufid = LockIpBuffer();
// 			ASSERT((bufid == 0) || (bufid == 1));
// 			m_iplib[bufid].Reset();
// 			if (m_iplib[bufid].LoadAllIPFromDbf(ipaddrtable))
// 			{
// 				m_tabletime = ftime;
// 				UnLockIpBuffer(bufid, TRUE);
// 				WriteLog("dce","更新IP地址表成功,共读入%d个IP段!!", m_iplib[bufid].m_ipcount);
// 			}
// 			else
// 			{
// 				if (m_iplib[bufid].m_error != 0)
// 				{
// 					// 删除索引
// 					WriteLog("dce","IP地址表索引损坏,重建索引并再次读取!");
// 					CString str(ipaddrtable);
// 					str.Replace(".dbf", ".cdx");
// 					DeleteFile(str);
// 					// 重新读取一遍
// 					m_iplib[bufid].Reset();
// 					if (m_iplib[bufid].LoadAllIPFromDbf(ipaddrtable))
// 					{
// 						m_tabletime = ftime;
// 						UnLockIpBuffer(bufid, TRUE);
// 						WriteLog("dce","更新IP地址表成功,共读入%d个IP段!!", m_iplib[bufid].m_ipcount);
// 						return;
// 					}
// 				}
// 				UnLockIpBuffer(bufid, FALSE);
// 				WriteLog("dce","更新IP地址表失败!");
// 			}
// 		}
// 	}
// 	catch(...)
// 	{
// 		WriteLog("dce","UpdateIpAddr() Error!!");
// 	}
// }
// 
// int	IpAreaBuffer::LockIpBuffer()
// {
// 	//**************************************************************************
// 	//	获得当前可写的缓冲区ID
// 	//	最多等待10秒，如果时间超时则强行获取
// 	//**************************************************************************
// 	int bufid;
// 	if (m_readflg == 1)
// 	{
// 		bufid = 0;
// 	}
// 	else	// (m_readflg == 1)
// 	{
// 		bufid = 1;
// 	}
// 	for (int wait = 0; wait < 5000; wait += 50)
// 	{
// 		if (m_readarith[bufid] == 0)
// 		{
// 			// 得到了数据的访问权限(正常途径)
// 			return bufid;
// 		}
// 		else
// 		{
// 			::Sleep(50);
// 		}
// 	}
// 	m_readarith[bufid] = 0;
// 	WriteLog("dce","强行获取IP地址缓冲区(%d)的访问权限", bufid);
// 	return bufid;
// }
// 
// CString IpAreaBuffer::QueryIpAddr(LPCTSTR strip)
// {
// 	CString str;
// 	int bufid = GetIpBuffer();
// 	if ((bufid == 0) || (bufid == 1))
// 	{
// 		str = m_iplib[bufid].QueryIP(strip);
// 		ReleaseIpBuffer(bufid);
// 	}
// 	return str;
// }
// 
// CString IpAreaBuffer::QueryIpAddr(DWORD uip)
// {
// 	CString str;
// 	int bufid = GetIpBuffer();
// 	if ((bufid == 0) || (bufid == 1))
// 	{
// 		str = m_iplib[bufid].QueryIP(uip);
// 		ReleaseIpBuffer(bufid);
// 	}
// 	return str;
// }
// BOOL OpenDbfTable(Code4 &code, Data4 &data, LPCTSTR dbfname)
// {
// 	if(_access(dbfname, 0) == 0)
// 	{
// 		for(int i = 0; i< 20; i++)
// 		{
// 			data.open(code, dbfname);
// 			if(!data.isValid())
// 				Sleep(500);
// 			else
// 				break;
// 		}
// 		if (data.isValid())
// 		{
// 			int iTry = 0;
// 			CString strCurOp;
// 			while(iTry++ < 50)
// 			{
// 				strCurOp.Format("尝试第 %d 锁定表", iTry);
// 				if(data.lockAll() == r4success)
// 					break;
// 				Sleep(20);
// 			}
// 			if(iTry >= 5)
// 			{
// 				WriteLog("dce","锁定表 %s 失败", dbfname);
// 				return FALSE;
// 			}
// 			return TRUE;
// 		}
// 		else
// 			WriteLog("dce","数据库文件%s打开失败!!", dbfname);
// 
// 	}
// 	else
// 		WriteLog("dce","数据库文件%s不存在!!", dbfname);
// 	return FALSE;
// }

// BOOL CloseTable(Code4 *code, Data4 *data, Index4 *index, CTableAccess *acTable)
// {
// 	ASSERT(code != NULL);
// 	ASSERT(data != NULL);
// 	ASSERT(acTable != NULL);
// 	
// 	if(code->readOnly == 0 && data->isValid()){
// 		data->flush();
// 		//		if(bNeedPack)
// 		//			data->pack();
// 		
// 	}
// 	
// 	
// 	if(index){
// 		if(index->isValid())
// 			index->close();
// 	}
// 	
// 	if(data->isValid())	{
// 		data->close();
// 	}
// 	
// 	
// 	code->closeAll();
// 	code->initUndo();
// 	
// 	acTable->SetCode4(NULL, NULL);
// 	acTable->SetIndex4(NULL);
// 	
// 	return TRUE;
// }
// BOOL OpenIndex(Data4 &data, Index4& index, TAG4INFO* ptaginfo, LPCTSTR cdxname)
// {
// 	if(_access(cdxname, 0) == 0)
// 	{
// 		index.open(data, cdxname);
// 		if (!index.isValid())
// 		{
// 			WriteLog("dce","打开索引文件%s失败!!", cdxname);
// 			return FALSE;
// 		}
// 		else
// 			return TRUE;
// 	}
// 	else
// 	{
// 		WriteLog("dce","索引文件%s不存在,自动创建!!", cdxname);
// 		index.create(data, cdxname, ptaginfo);
// 		if(index.isValid())
// 		{
// 			data.data->codeBase->errorCode = 0;
// 			if (index.reindex() != r4success)
// 			{
// 				WriteLog("dce","索引文件%s重建索引失败!!", cdxname);
// 				return FALSE;
// 			}
// 			else
// 				return TRUE;
// 		}
// 		else
// 			WriteLog("dce","索引文件%s创建失败!!", cdxname);
// 		return FALSE;
// 	}
// 		return FALSE;
// 	
// }
// CTableAccess::CTableAccess()
// {
// 	code	= NULL;
// 	data	= NULL;
// 	index	= NULL;
// }

// CTableAccess::~CTableAccess()
// {
// 	if(index){
// 		if(index->isValid())
// 			index->close();
// 	}
// 	if(data){
// 		if(data->isValid())
// 			data->close();
// 	}
// 	
// 	if(code){
// 		code->closeAll();
// 		code->initUndo();
// 	}
// }
// 
// BOOL CTableAccess::SetCode4(Code4 *Incode, Data4 *Indata)
// {
// 	code = Incode;
// 	data = Indata;
// 	return TRUE;
// }
// 
// BOOL CTableAccess::SetIndex4(Index4 *Inindex)
// {
// 	index = Inindex;
// 	return TRUE;
// }

IpQueryLib	k_IpQueryTable;	// IP区域表
