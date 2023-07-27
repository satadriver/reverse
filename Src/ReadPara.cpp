// ReadPara.cpp: implementation of the CReadPara class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "dce.h"
#include "ReadPara.h"
#include "PubVar.h"
#include "PubFunc.h"
#include "SyncTable.h"
#include "IpQueryLib.h"
#include "Getmoniterweb.h"
//////////////////////////////////////////////////////////////////////////
BOOL GetSynTblFile();		// 获得更新

static DWORD v_printpara = 0;
static DWORD v_limitcount[2][25] = 
{
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};
#define		SQLPORT					11020
#define		LIMITTYPE_CASEIP		0
#define		LIMITTYPE_CASEMAC		1
#define		LIMITTYPE_CASEVLANID	2
#define		LIMITTYPE_CASEACCOUNT	3
#define		LIMITTYPE_CASEPHONE		4
#define		LIMITTYPE_CASEIPURL		5
#define		LIMITTYPE_CASEURL		6
#define		LIMITTYPE_CASEKEYWORD	7
#define		LIMITTYPE_CASEJZWORD	8
#define		LIMITTYPE_HTTPWORD		9
#define		LIMITTYPE_INDEXIPURL	10
#define		LIMITTYPE_INDEXPORT		11
#define		LIMITTYPE_RUBBISHIP		12
#define		LIMITTYPE_RUBBISHURL	13
#define		LIMITTYPE_RUBBISHAAA	14
#define		LIMITTYPE_RUBBISHMAIL	15
#define		OICQID					16
#define		OICQPASS				17
#define		HTTPUPSPLIT				18
#define		LIMITTYPE_IDCHTTPKEY	19
#define		LIMITTYPE_IDCSVRPORT	20
#define		TAOBAOID				21
#define		TAOBAOPASS				22
#define		KEYPEOPLE				23
#define		CCIC					24

BOOL UpdateSystemPara(LPCTSTR szBcp);
BOOL UpdateCLimitPara(LPCTSTR szBcp);
BOOL UpdateTaobaoPwd(LPCTSTR szBcp);
BOOL UpdateOicqPwd(LPCTSTR szBcp);
BOOL UpdateDictionary(AllDicMap* alldicmap,int bufid);
BOOL UpdateAllIpAddr(LPCTSTR filename);
BOOL UpdateLocalIpAddr(LPCTSTR filename);
BOOL UpdateIndexKeyWord(LPCTSTR filename);
BOOL UpdateFyyOneClimit(LPCTSTR szBcp, AllDicMap& alldicmap, int bufid);
BOOL UpdateHttpupSplit(LPCTSTR filename);
BOOL Updatehttpregist(LPCTSTR filename);
BOOL UpdateRubbish(LPCTSTR filename);
BOOL UpdateFullRule(LPCTSTR filename);
BOOL UpdateMoniterWeb(LPCTSTR filename);
BOOL UpdateIndexLimit(LPCTSTR filename);

BOOL UpdateWarnLimit(AllDicMap* alldicmap,long bufid);
BOOL UpdateCcicPara(LPCTSTR filename);
BOOL UpdateyysIpAddr(LPCTSTR filename);


extern RubbishPara		v_RubbishPara;

//////////////////////////////////////////////////////////////////////////
typedef BOOL (*LIMITFUNCTION)(LPCTSTR);	// 更新参数函数指针
struct ParaFileInfo
{
	LPCTSTR			fname;				// dbf文件名(不包含路径)
	int				updatetm;			// 更新标志
	BOOL			updateflg;			// 更新标志
	int				update;				// 强行更新时间间隔
	LIMITFUNCTION	pUpdateFunc;		// 更新参数函数指针
	UINT			systemtype;			// 适用系统
};

#define PARAMTABLECLIMIT	1
int			g_tablenum = 21;					// 表的总个数
static ParaFileInfo g_sleuthparainfo[32] = \
{
/*0*/	{_T("system"),			0,	TRUE,	72*60*60,	UpdateSystemPara,		SYSTEM_ALL},
		{_T("climit"),			0,	TRUE,	72*60*60,	UpdateCLimitPara,		SYSTEM_ALL},
		{_T("taobaopas"),		0,	TRUE,	72*60*60,	UpdateTaobaoPwd,		SYSTEM_ALL},
		{_T("qqpas"),			0,	TRUE,	72*60*60,	UpdateOicqPwd,			SYSTEM_ALL},
		{_T("Ipaddr.rar"),		0,  TRUE,	72*60*60,	UpdateLocalIpAddr,		SYSTEM_ALL},
		{_T("allIpaddr.rar"),	0,	TRUE,	72*60*60,	UpdateAllIpAddr,		SYSTEM_ALL},
		{_T("KeyWord"),			0,	TRUE,	72*60*60,	UpdateIndexKeyWord,		SYSTEM_ALL},
	//	{_T("localipmatch"),0,  TRUE,	72*60*60,		UpdateLocalIpAddr,		SYSTEM_ALL},
	//	{_T("netipmatch"),	0,	TRUE,	72*60*60,		UpdateAllIpAddr,		SYSTEM_ALL},

};
// static ParaFileInfo g_fyyparainfo[32] = \
// {
// /*0*/	{_T("system"),		0,	TRUE,	600,		UpdateSystemPara,		SYSTEM_ALL},
// 		{_T("climit"),		0,	TRUE,	600,		UpdatefyyCLimitPara,		SYSTEM_ALL},
// 		{_T("taobaopas"),	0,	TRUE,	48*60*60,	UpdateTaobaoPwd,		SYSTEM_ALL},
// 		{_T("qqpas"),		0,	TRUE,	48*60*60,	UpdateOicqPwd,			SYSTEM_ALL},
// 		{_T("Ipaddr.rar"),	0,  TRUE,	72*60*60,	UpdateLocalIpAddr,		SYSTEM_ALL},
// 		{_T("allIpaddr.rar"),0, TRUE,	72*60*60,	UpdateAllIpAddr,		SYSTEM_ALL},
// 		{_T("UpdateIndexKeyWord"),0, TRUE,	72*60*60,	UpdateIndexKeyWord,		SYSTEM_ALL},
// }
static ParaFileInfo g_parainfo[32] = \
{
/*0*/	{_T("system"),		0,	TRUE,	72*60*60,		UpdateSystemPara,		SYSTEM_ALL},
		{_T("climit"),		0,	TRUE,	72*60*60,		UpdateCLimitPara,		SYSTEM_ALL},
		{_T("taobaopas"),	0,	TRUE,	72*60*60,		UpdateTaobaoPwd,		SYSTEM_ALL},
		{_T("qqpas"),		0,	TRUE,	72*60*60,		UpdateOicqPwd,			SYSTEM_ALL},
		{_T("localipmatch"),0,  TRUE,	72*60*60,		UpdateLocalIpAddr,		SYSTEM_ALL},
		{_T("netipmatch"),	0,	TRUE,	72*60*60,		UpdateAllIpAddr,		SYSTEM_ALL},
		{_T("KeyWord"),		0,	TRUE,	72*60*60,		UpdateIndexKeyWord,		SYSTEM_ALL},
		{_T("httpupmatch"),	0,	TRUE,	72*60*60,		UpdateHttpupSplit,		SYSTEM_ALL},
 		{_T("shield"),		0,  TRUE,	72*60*60,		UpdateRubbish,			SYSTEM_ALL-SYSTEM_SLEUTH},
 		{_T("fullrule"),	0,  TRUE,	72*60*60,		UpdateFullRule,			SYSTEM_ALL-SYSTEM_SLEUTH},
 		{_T("ekeyword"),	0,  TRUE,	72*60*60,		UpdateMoniterWeb,		SYSTEM_ALL-SYSTEM_SLEUTH},
 		{_T("httpregist"),	0,  TRUE,	72*60*60,		Updatehttpregist,		SYSTEM_ALL-SYSTEM_SLEUTH},
		{_T("indexlimit"),	0,  TRUE,	72*60*60,		UpdateIndexLimit,		SYSTEM_ALL-SYSTEM_SLEUTH},
		{_T("keypeople"),	0,	TRUE,	72*60*60,		UpdateCLimitPara,		SYSTEM_ALL-SYSTEM_SLEUTH},
		{_T("kpkeyword"),	0,	TRUE,	72*60*60,		UpdateCLimitPara,		SYSTEM_ALL-SYSTEM_SLEUTH},
		{_T("kprule"),	    0,	TRUE,	72*60*60,		UpdateCLimitPara,		SYSTEM_ALL-SYSTEM_SLEUTH},
		{_T("netclimit"),	0,	TRUE,	72*60*60,		UpdateCLimitPara,		SYSTEM_ALL-SYSTEM_SLEUTH},
		{_T("ccic"),	    0,	TRUE,	72*60*60,		UpdateCcicPara,			SYSTEM_ALL-SYSTEM_SLEUTH},
		{_T("cardinfo"),    0,	TRUE,	72*60*60,		UpdateCLimitPara,		SYSTEM_ALL-SYSTEM_SLEUTH},
		{_T("ipaddr"),		0,	TRUE,	72*60*60,		UpdateyysIpAddr,		SYSTEM_ALL},

// 		{_T("climit.sql"),		0,  0,	TRUE,	600,		UpdateCaseLimit,		SYSTEM_ALL},
// 		{_T("indexlimit.sql"),	0,  0,	TRUE,	3600,		UpdateIndexLimit,		SYSTEM_ALL-SYSTEM_SLEUTH},
// 		{_T("keyword.sql"),		0,  0,	TRUE,	3600,		UpdateIndexKeyWord,		SYSTEM_ALL-SYSTEM_SLEUTH},
// 		{_T("exception.sql"),	0,  0,	TRUE,	3600,		UpdateRubbish,			SYSTEM_ALL-SYSTEM_SLEUTH},
// /*5*/	{_T("legalip.sql"),		0,  0,	TRUE,	24*60*60,	UpdateLegalIP,			SYSTEM_SZFJ},
// 		{_T("idc_ipinfo.sql"),	0,  0,	TRUE,	48*60*60,	UpdateIdcIpAddr,		SYSTEM_002},
// 		{_T("Ipaddr.sql"),		0,  0,	TRUE,	72*60*60,	UpdateLocalIpAddr,		SYSTEM_ALL-SYSTEM_SLEUTH},
// 		{_T("allIpaddr.sql"),	0,  0,	TRUE,	72*60*60,	UpdateAllIpAddr,		SYSTEM_ALL-SYSTEM_SLEUTH},
// 		{_T("httpupmatch.sql"),	0,  0,	TRUE,	7200,		UpdateHttpupSplit,		SYSTEM_ALL-SYSTEM_SLEUTH},
// /*10*/	{_T("idchttpkey.sql"),	0,  0,	TRUE,	3600,		UpdateIDCHttpKey,		SYSTEM_002},
// 		{_T("idcsvrport.sql"),	0,  0,	TRUE,	3600,		UpdateIDCServerPort,	SYSTEM_002},
// 		{_T("taobaopass.sql"),	0,  0,	TRUE,	48*60*60,	UpdateTaobaoPwd,		SYSTEM_ALL},
// 		{_T("qqpas.sql"),		0,	0,	TRUE,	48*60*60,	UpdateOicqPwd,			SYSTEM_ALL},
// 		{_T("ImportantMan.sql"),0,  0,	TRUE,	3600,		UpdateWarnLimit,		SYSTEM_ALL-SYSTEM_SLEUTH},
// 		{_T("warnlevel.sql"),	0,  0,	TRUE,	3600,		UpdateWarnLimit,		SYSTEM_ALL-SYSTEM_SLEUTH},
// 		{_T("warnrule.sql"),	0,  0,	TRUE,	3600,		UpdateWarnLimit,		SYSTEM_ALL-SYSTEM_SLEUTH},
// /*17*/	{_T(""),				0,  0,			FALSE,	0,			NULL,					0},
// 		{_T(""),				0,  0,			FALSE,	0,			NULL,					0},
// 		{_T(""),				0,  0,			FALSE,	0,			NULL,					0},
// 		{_T(""),				0,  0,			FALSE,	0,			NULL,					0},
// 		{_T(""),				0,  0,			FALSE,	0,			NULL,					0},
// 		{_T(""),				0,  0,			FALSE,	0,			NULL,					0},
// 		{_T(""),				0,  0,			FALSE,	0,			NULL,					0},
// 		{_T(""),				0,  0,			FALSE,	0,			NULL,					0},
// /*25*/	{_T(""),				0,  0,			FALSE,	0,			NULL,					0},
// 		{_T(""),				0,  0,			FALSE,	0,			NULL,					0},
// 		{_T(""),				0,  0,			FALSE,	0,			NULL,					0},
// 		{_T(""),				0,  0,			FALSE,	0,			NULL,					0},
// 		{_T(""),				0,  0,			FALSE,	0,			NULL,					0},
// 		{_T(""),				0,  0,			FALSE,	0,			NULL,					0},
// /*31*/	{_T(""),				0,  0,			FALSE,	0,			NULL,					0},
};
extern moniterweb g_moniterweb;

void ExitDCE(LPCTSTR filename)
{
	if (_access(filename, 0) == 0)
	{
		WriteLog("dce", "Error 文件%s删除失败!!", filename);
		InterlockedExchange((long*)&v_DceRunFlg, 0);
		FILE *pf = fopen(v_WorkPath+"\\flg\\deldcepara.flg", "wb");
		if (pf == NULL)
			WriteLog("dce", "创建标志文件deldcepara.flg失败!");
		else
			fclose(pf);
		::Sleep(2000);
		exit(-2);
	}
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
BOOL UpdatePara()
{ 
	//	UpdateCLimitPara("E:\\netspy\\para\\dce\\climit20081021193831_switch.bcp");
	int error=0;
	CString strBcp;

	try
	{
		CreateFlgFile(g_DelDceParaFlg);
error=1;
 		if (GetSynTblFile())
		{
			CSyncPara synTable;
			ParaFileInfo *pParaCheck = NULL;
error=2;

			if (v_programtype == SYSTEM_SLEUTH)
			{
				pParaCheck = g_sleuthparainfo;
			}
			else
			{
				pParaCheck = g_parainfo;
			}
error=3;

			for (int i=0; i<g_tablenum; i++)
			{
				if ((pParaCheck[i].updateflg && (pParaCheck[i].systemtype & v_programtype)))
				{
					if(strstr(pParaCheck[i].fname, ".rar") != NULL)
						strBcp.Format("%s\\%s", g_strRootPara, pParaCheck[i].fname);
					else
						strBcp.Format("%s\\%s.bcp", g_strRootPara, pParaCheck[i].fname);
error=4;

					if(_access(strBcp, 0) == 0)
					{
						CreateFlgFile();	// 创建心跳文件
error=5;

						if (!pParaCheck[i].pUpdateFunc(strBcp))
						{
							WriteLog("dce", "更新表%s失败!", pParaCheck[i].fname);
							if(FALSE == DeleteFile(strBcp))
								ExitDCE(strBcp);
						}
					}
error=6;

					pParaCheck[i].updateflg = FALSE;
				}
			}

			/// 内置443输出unip
			v_IndexLimitPara.m_port[0][443] = 0x55;
			v_IndexLimitPara.m_port[1][443] = 0x55;


			v_printpara++;
			if ((v_programtype==SYSTEM_SLEUTH && (1==(v_printpara%100))) ||
				(v_programtype!=SYSTEM_SLEUTH && (1==(v_printpara%60)))  ||
				memcmp(v_limitcount[0], v_limitcount[1], sizeof(v_limitcount)/2)!=0)
			{
				CString str;
				str.Format("Update Para: IP(%d), MAC(%d), VLANID(%d), ACCOUNT(%d), PHONE(%d)\r\n",
					v_limitcount[0][LIMITTYPE_CASEIP], v_limitcount[0][LIMITTYPE_CASEMAC],
					v_limitcount[0][LIMITTYPE_CASEVLANID], v_limitcount[0][LIMITTYPE_CASEACCOUNT],
					v_limitcount[0][LIMITTYPE_CASEPHONE]);
				str.Format("%sIP_URL(%d), URL(%d), WetChat(%d), Port(%d)\r\n",
					(CString)str, v_limitcount[0][LIMITTYPE_CASEIPURL], v_limitcount[0][LIMITTYPE_CASEURL],
					v_limitcount[0][LIMITTYPE_INDEXIPURL], v_limitcount[0][LIMITTYPE_INDEXPORT]);

				str.Format("%sKeyWord(%d), JZKEYWORD(%d), HttpWord(%d), IdcKeyWord(%d)\r\n",
					(CString)str, v_limitcount[0][LIMITTYPE_CASEKEYWORD], v_limitcount[0][LIMITTYPE_CASEJZWORD],
					v_limitcount[0][LIMITTYPE_HTTPWORD], v_limitcount[0][LIMITTYPE_IDCHTTPKEY]);

				str.Format("%sRubIP(%d), RubURL(%d), RubAAA(%d), RubMail(%d)\r\n",
					(CString)str, v_limitcount[0][LIMITTYPE_RUBBISHIP], v_limitcount[0][LIMITTYPE_RUBBISHURL],
					v_limitcount[0][LIMITTYPE_RUBBISHAAA], v_limitcount[0][LIMITTYPE_RUBBISHMAIL]);

				str.Format("%sQQID(%d), QQPASS(%d), TAOBAOID(%d), TAOBAOPASS(%d),HttpupSplit(%d),KeyPeople(%d),Ccic(%d)", (CString)str,
					v_limitcount[0][OICQID], v_limitcount[0][OICQPASS], v_limitcount[0][TAOBAOID], v_limitcount[0][TAOBAOPASS],
					v_limitcount[0][HTTPUPSPLIT],v_limitcount[0][KEYPEOPLE],v_limitcount[0][CCIC]);
				WriteLog("dce", str);
				memcpy(v_limitcount[1], v_limitcount[0], sizeof(v_limitcount)/2);
			}
		}

		return TRUE;
	}
	catch (...)
	{
		WriteLog("dce", "UpdatePara error =%d!strBcp=%s",error,strBcp);
	}
	return TRUE;
}

// 获得更新
BOOL GetSynTblFile()
{
	int error=0;
	try
	{
		// 检查本地的参数表
		CString LocalParaPath(g_strRootPara);
		if (_access(LocalParaPath, 0) != 0)
		{
			CreateDir(LocalParaPath);
		}
error=1;
		CStringArray arrCS;
		if(v_programtype == SYSTEM_SLEUTH)
			arrCS.Add("127.0.0.1");
		else
		{
			arrCS.Append(v_ParaServer);
		}
error=2;
		ParaFileInfo *pParaCheck = NULL;
		if (v_programtype == SYSTEM_SLEUTH)
		{
			pParaCheck = g_sleuthparainfo;
		}
		else
		{
			pParaCheck = g_parainfo;
		}

		BOOL bGetOne ;
		BOOL bChange = FALSE;
		CString strSQL;
		BOOL	bForce = FALSE;
error=3;
		DWORD begtm = GetTickCount();
		CSyncPara synTable;
		for (int i = 0; i < g_tablenum; i++)
		{
			if ((v_programtype & pParaCheck[i].systemtype)==0)
				continue;

			bForce = FALSE;
			time_t now = time(0);
			if(abs(now-pParaCheck[i].updatetm)>=pParaCheck[i].update)
			{
				// 强制更新
				bForce = TRUE;
			}
error=4;			
			bGetOne = synTable.UpdatePara(g_strRootPara, pParaCheck[i].fname, arrCS, CSyncPara::syn_bcp, bForce);
			if(bGetOne)
			{
				pParaCheck[i].updateflg = TRUE;
				pParaCheck[i].updatetm = now;
			}
			bChange |= bGetOne;

			DWORD nowtm = GetTickCount();
			if ((nowtm - begtm)>60000)
			{
				CreateFlgFile();
				nowtm = begtm;
			}
		}
error=5;
		return bChange;

	}
	catch(...)
	{
		WriteLog("dce", "GetSynTblFile error =%d!",error);

	}
	return true;
}

BOOL ReleaseQQMap(OicqPasswrdMap* oicqpassmap)
{
	POSITION pos=oicqpassmap->GetStartPosition();
	QQPWD* p;
	
	DWORD key;
	while (pos!=NULL)
	{
		oicqpassmap->GetNextAssoc(pos,key,p);
// 		delete p;
// 		p = NULL;
		v_OicqPassWord.ReleaseUnit(p);
	}
	oicqpassmap->RemoveAll();
	return TRUE;

}
BOOL UpdateOicqPwd(LPCTSTR szBcp)
{
int error=0;
// 	{"QQID",		r4str, 20,  0},	// qqid	
// 	{"REC_TIME",	r4num, 10, 0},	// 记录同步标识(同步时使用)(time_t数据的串)
// 	{"PASS",		r4str, 40, 0},	// 密码
// 	{"PASSTYPE",	r4str, 40, 0},	// md5
// 	{"SYSTYPE",		r4str, 20, 0},	// 系统类型
// 	{"SRCDEV",		r4str, 50, 0},	// 设备
// 	{"MARK",		r4str, 100, 0},	// 备注
// 	{"CAP_TIME",	r4str, 20, 0},	// 截获时间,格式: 2007-02-27 09:58:20
// 	{"LUSE_TIME",	r4str, 20, 0},	// 最后使用时间,格式: 2007-02-27 09:58:47
	try
	{
		int bcp_qqpass_count;
		int	bcp_qqpass_user;
		int	bcp_qqpass_pass;
		int	bcp_qqpass_type;
		if(v_programtype != SYSTEM_SLEUTH)
		{
			bcp_qqpass_count=4;
			bcp_qqpass_user=0;
			bcp_qqpass_pass=1;
			bcp_qqpass_type=3;
		}
		else
		{
			bcp_qqpass_count=9;
			bcp_qqpass_user	=0;
			bcp_qqpass_pass = 2;
			bcp_qqpass_type	=3;
		}
		ASSERT(v_OicqPassWord.m_readflg==0 || v_OicqPassWord.m_readflg==1);
		long	bufid = (v_OicqPassWord.m_readflg==0)? 1 : 0;
error=1;
		// 清空原缓冲区
	//	v_OicqPassWord.m_qqpwdmap[bufid].RemoveAll();
//		ReleaseQQMap(&v_OicqPassWord.m_qqpwdmap[bufid]);
error=2;
		memset(v_limitcount[0]+OICQID, 0, (HTTPUPSPLIT-OICQID)*4);
		int taobaocount = 0;
		int taobaomd5count = 0;
		CString passtype, qqpwdmd5;
		int qqcount = 0;
		int qqmd5count = 0;

		BOOL bReadOK = TRUE;
error=3;
		CSyncPara synPara;
		DWORD dealPos = 0;
		if(FALSE == synPara.LoadPara(szBcp, dealPos, TRUE))
		{
			return TRUE;
		}
error=4;		
	#ifdef _DEBUG
		BOOL bFirst = TRUE;
	#endif
		CStringArray arrField;
		BOOL bGetOne;
		do{
			arrField.RemoveAll();
			bGetOne = synPara.GetOneRecord(dealPos, arrField);
error=5;
			if(FALSE == bGetOne)
				break;		// 已经没有了
//			ASSERT(arrField.GetSize() == bcp_qqpass_count);
			if(arrField.GetSize() != bcp_qqpass_count)
			{
	//			bReadOK = FALSE;
				continue;
			}

			DWORD qqnum = atol(arrField[bcp_qqpass_user]);
			if (qqnum<10000)
			{
				continue;
			}
			CString passtype = arrField[bcp_qqpass_type];
			passtype.TrimRight();
error=6;
			QQPWD* pvalue=NULL;
			if (//passtype.CompareNoCase("0")==0 ||
				//passtype.CompareNoCase("4")==0 ||
				//passtype.IsEmpty()
				TRUE)	// 现在全明码, md5形式的没了, 忽略passtype字段
			{
				CString qqpwd = arrField[bcp_qqpass_pass];
#ifdef _DEBUG
				if(bFirst)
				{
					qqnum = 1770369298;	// testqq
					qqpwd = "1234woaini";
					bFirst = FALSE;		// Debug版记录首条被替换
				}
#endif

				qqpwd.TrimLeft();
				qqpwd.TrimRight();
				v_OicqPassWord.m_qqpwdmap[bufid].Lookup(qqnum, pvalue);
				if (qqpwd.GetLength()>0)
				{
error=7;
					QQPWD* tp = v_OicqPassWord.GetNewUnit();
					memset(tp, 0, sizeof(QQPWD)/*-sizeof(CString)*/);
					_snprintf(tp->longpwd,16,"%s",qqpwd);
					char md51[16+8];
					memset(md51, 0, 24);
					MD5Tran(md51, (void*)(LPCTSTR)qqpwd, qqpwd.GetLength());

// 郝振石 (hzs)   02/09/11  08:49:45 // qq2011beta版后
// 一遍md5+ 8字节qq号 的md5
// ?? ?? 9A 40 67 DB C8 64 15 5A 4C 09 10 AD 8E 16 00 00 00 00 04 93 64 18
					DWORD oo = Swap4Byte(qqnum);
					memcpy(md51+20, &oo, 4);
					MD5Tran(tp->pwdqqnomd5, md51, 24);

					MD5Tran(tp->pwd2md5, md51, 16);
					if (pvalue==NULL)
					{
						v_OicqPassWord.m_qqpwdmap[bufid].SetAt(qqnum, tp);
						qqcount++;
						qqmd5count++;
						continue;
					}
error=8;
					QQPWD* p = pvalue;
					while(p!=NULL)
					{
						if (0 == memcmp(p->pwd2md5, tp->pwd2md5, 16))
						{
// 							if (p->password[0]==0)
// 								strcpy(p->password, tp->password);
							break;
						}
						p = p->pNext;
					}
					if (p == NULL)
					{
						tp->pNext = pvalue->pNext;
						pvalue->pNext = tp;
						qqmd5count++;
					}
					else
					{
error=12;
						// 重复的密码MD5
						v_OicqPassWord.ReleaseUnit(tp);
						continue;
					}
				}
			}
			else
			{
error=9;
				CString qqpwdmd5;
				qqpwdmd5.Format("%s",arrField[bcp_qqpass_pass]);
				qqpwdmd5.TrimLeft();
				qqpwdmd5.TrimRight();
				if (qqpwdmd5.GetLength()==0)
					continue;
				if (qqpwdmd5.GetLength()!=32)
				{
					qqpwdmd5.Empty();
				}
error=10;
				qqpwdmd5.MakeUpper();
				DWORD hash[4];
				memset(hash, 0, sizeof(hash));
				if (4 == sscanf(qqpwdmd5, "%08X%08X%08X%08X",
					hash, hash+1, hash+2, hash+3))
				{
					hash[0] = Swap4Byte(hash[0]);
					hash[1] = Swap4Byte(hash[1]);
					hash[2] = Swap4Byte(hash[2]);
					hash[3] = Swap4Byte(hash[3]);
error=9;
					QQPWD* tp = v_OicqPassWord.GetNewUnit();
					memset(tp, 0, sizeof(QQPWD)/*-sizeof(CString)*/);
					memcpy(tp->pwd2md5, hash, 16);
					if (pvalue==NULL)
					{
						v_OicqPassWord.m_qqpwdmap[bufid].SetAt(qqnum, tp);
						qqcount++;
						qqmd5count++;
						continue;
					}
error=10;
					QQPWD* p = pvalue;
					while(p!=NULL)
					{
						if (0 == memcmp(p->pwd2md5, tp->pwd2md5, 16))
						{
							break;
						}
						p = p->pNext;
					}
					if (p == NULL)
					{
						tp->pNext = pvalue->pNext;
						pvalue->pNext = tp;
						qqmd5count++;
					}
					else
					{
error=11;
						// 重复的密码MD5
						v_OicqPassWord.ReleaseUnit(tp);
						continue;
					}
				}
			}
		}while(bGetOne);
		if(bReadOK)
		{
			InterlockedExchange(&v_OicqPassWord.m_readflg, bufid);
			v_limitcount[0][OICQID] = qqcount;
			v_limitcount[0][OICQPASS] = qqmd5count;
			return TRUE;
		}

		return bReadOK;
	}
	catch (...)
	{
		WriteLog("dce", "UpdateOicqPwd error =%d!",error);
	}
	return TRUE;
}

BOOL UpdateTaobaoPwd(LPCTSTR szBcp)
{
// 	{"TAOBAOID",		r4str, 20,  0},	// 淘宝ID
// 	{"REC_TIME",	r4num, 10, 0},	// 记录同步标识(同步时使用)(time_t数据的串)
// 	{"PASS",		r4str, 40, 0},	// 密码
// 	{"PASSTYPE",	r4str, 40, 0},	// md5
// 	{"SYSTYPE",		r4str, 20, 0},	// 系统类型
// 	{"SRCDEV",		r4str, 50, 0},	// 设备
// 	{"MARK",		r4str, 100, 0},	// 备注
// 	{"CAP_TIME",	r4str, 20, 0},	// 截获时间,格式: 2007-02-27 09:58:20
// 	{"LUSE_TIME",	r4str, 20, 0},	// 最后使用时间,格式: 2007-02-27 09:58:47
#define		bcp_taobao_count	9
#define		bcp_taobao_user		0
#define		bcp_taobao_pass		2
	ASSERT(v_TaobaoPassWord.m_readflg==0 || v_TaobaoPassWord.m_readflg==1);
	long	bufid = (v_TaobaoPassWord.m_readflg==0)? 1 : 0;
	v_TaobaoPassWord.m_taobaopwdmap[bufid].RemoveAll();
	memset(v_limitcount[0]+OICQID, 0, (HTTPUPSPLIT-OICQID)*4);
	int taobaocount = 0;
	int taobaomd5count = 0;
	CString taobaouser, taobaopwd;

	BOOL bReadOK = TRUE;

	CSyncPara synPara;
	DWORD dealPos = 0;
	if(FALSE == synPara.LoadPara(szBcp, dealPos, TRUE))
	{
		return TRUE;
	}
	CStringArray arrField;
	BOOL bGetOne;
	do{
		arrField.RemoveAll();
		bGetOne = synPara.GetOneRecord(dealPos, arrField);
		if(FALSE == bGetOne)
			break;
		ASSERT(arrField.GetSize() == bcp_taobao_count);
		if(arrField.GetSize() != bcp_taobao_count)
		{
			bReadOK = FALSE;
			break;
		}

		taobaouser.Format("%s",arrField[bcp_taobao_user]);
		taobaopwd.Format("%s",arrField[bcp_taobao_pass]);
		TAOBAOPWD* pvalue=NULL;
		v_TaobaoPassWord.m_taobaopwdmap[bufid].Lookup(taobaouser, pvalue);
		if (taobaopwd.GetLength()>0)
		{
			TAOBAOPWD* tp = v_TaobaoPassWord.GetNewUnit();
			memset(tp, 0, sizeof(TAOBAOPWD)-sizeof(CString));
			tp->longpwd = taobaopwd;
			taobaopwd.Format("18nnad7f1njdy7f23nadifu23djfdu%s",(CString)taobaopwd);
			MD5Tran(tp->pwd2md5, (void*)(LPCTSTR)taobaopwd, taobaopwd.GetLength());
			if (pvalue==NULL)
			{
				v_TaobaoPassWord.m_taobaopwdmap[bufid].SetAt(taobaouser, tp);
				taobaocount++;
				taobaomd5count++;
				continue;
			}
			TAOBAOPWD* p = pvalue;
			while(p!=NULL)
			{
				if (0 == memcmp(p->pwd2md5, tp->pwd2md5, 16))
				{
					if (p->password[0]==0)
						strcpy(p->password, tp->password);
					break;
				}
				p = p->pNext;
			}
			if (p == NULL)
			{
				tp->pNext = pvalue->pNext;
				pvalue->pNext = tp;
				taobaomd5count++;
			}
			else
			{
				// 重复的密码MD5
				v_TaobaoPassWord.ReleaseUnit(tp);
				continue;
			}
		}
	}while(bGetOne);
	
	if(bReadOK)
	{
		InterlockedExchange(&v_TaobaoPassWord.m_readflg, bufid);
		v_limitcount[0][TAOBAOID] = taobaocount;
		v_limitcount[0][TAOBAOPASS] = taobaomd5count;
	}

	return bReadOK;
}

BOOL UpdateFyyOneClimit(LPCTSTR szBcp, AllDicMap& alldicmap, int bufid)
{
// 	{"CASEID",		r4str, 50, 0},	// 案件ID
// 	{"OBJECTTYPE",	r4str, 16, 0},	// 对象类型
// 	{"OBJECTNAME",	r4str, 50, 0},	// 对象名称
// 	{"ADDRNAME",	r4str, 200,0},	// 地址数据
// 	{"ADDRTYPE",	r4str, 2,  0},	// 地址类型
// 	{"STATE",		r4str, 1,  0},	// 启用状态
// 	{"IFW",			r4str, 1,  0},	// Web告警
// 	{"IFHANDSET",	r4str, 1,  0},	// 短信告警
// 	{"BUKONG",		r4str, 1,  0},	// 是否布控
// 	{"ADDMAN",		r4str, 50, 0},	// 增加人员
// 	{"ADDTM",		r4str, 20, 0},	// 增加时间									
// 	{"REMARK",		r4str, 100, 0},	// 增加时间									
// 	{"PRO",			r4str, 50, 0},	// 不起作用协议     httpdown,unip,media		
// 	{"WEBPRO",		r4str, 400, 0},	// 页面告警协议     httpdown,unip,media等	
// 	{"HANDPRO",		r4str, 400, 0},	// 手机告警协议     httpdown,unip,media等	
// 	{"CODING",		r4str, 100, 0},	// 关键字类线索的编码类型 GB BIG UTF8默认为GB
#define		fyybcp_climit_count		16
#define		fyybcp_climit_caseid		0
#define		fyybcp_climit_objecttype	2
#define		fyybcp_climit_objectname	1
#define		fyybcp_climit_addrname		1
#define		fyybcp_climit_addrtype		2
#define		fyybcp_climit_state		3
#define		fyybcp_climit_ifw			4
#define		fyybcp_climit_ifhandset	5
#define		fyybcp_climit_nocappro	9
#define		fyybcp_climit_webalarmpro	10
#define		fyybcp_climit_handalarmpro	11
#define		fyybcp_climit_addrid	13
int error=0;
	BOOL bReadOK = TRUE;
	BOOL Isprint=TRUE;
	try
	{
		LIMITDATA buf;
		CString straddrname;

		CSyncPara synPara;
		DWORD dealPos = 0;
		if(FALSE == synPara.LoadPara(szBcp, dealPos, TRUE))
		{
			return FALSE;
		}
		CStringArray arrField;
		BOOL bGetOne = synPara.GetOneRecord(dealPos, arrField);
error=1;
		while(bGetOne)
		{
	// 		ASSERT(arrField.GetSize() == bcp_climit_count);
	// 		if(arrField.GetSize() != bcp_climit_count)
	// 		{
	// 			bReadOK = FALSE;
	// 			break;
	// 		}
	// 		static LPCTSTR FIELDSNAME[9] =
	// 		{
	// 			_T("CASEID"), _T("OBJECTTYPE"), _T("OBJECTNAME"), 
	// 			_T("ADDRNAME"), _T("ADDRTYPE"), _T("STATE"),
	// 			_T("IFW"), _T("IFHANDSET"), _T("BUKONG")
	// 		};
	//2审批通过  3审批未通过 1未审批
			if ((arrField[fyybcp_climit_state].GetLength()<=0) || (arrField[fyybcp_climit_state].GetAt(0) != '2'))
			{
				arrField.RemoveAll();
				bGetOne = synPara.GetOneRecord(dealPos, arrField);
				continue;
			}
			if (arrField.GetSize()<8)
			{
				if (Isprint)
				{
					WriteLog("dce", "UpdateFyyOneClimit() climit 字段少于8个 =%d",arrField.GetSize());
					Isprint=FALSE;
				}
				arrField.RemoveAll();
				bGetOne = synPara.GetOneRecord(dealPos, arrField);
				continue;
			}
error=2;
			memset(&buf, 0, sizeof(buf));
			buf.ltype= atol(arrField[fyybcp_climit_addrtype]);		// 地址类型

			if (v_programtype == SYSTEM_SLEUTH)
			{
				if (buf.ltype==18 || buf.ltype==14)
				{
					buf.ifexpand= atol(arrField[fyybcp_climit_ifw]);	// 是否扩展
				}
			}
			// 增加地址参数
			buf.caseid= arrField[fyybcp_climit_caseid];		// 案件ID
			buf.objtype= arrField[fyybcp_climit_objecttype];		// 对象类型
			buf.objname= arrField[fyybcp_climit_objectname];		// 对象名称
			buf.limit= arrField[fyybcp_climit_addrname];// 地址数据
			buf.nocappro= arrField[fyybcp_climit_addrname];
			if (arrField.GetSize()>13)
			{
				buf.limitid=arrField[fyybcp_climit_addrid];
			}
error=3;
			if (arrField[fyybcp_climit_ifw].CompareNoCase("1") == 0)
			{
				buf.AlarmState |= 0x10;
			}
error=31;

			if (arrField[fyybcp_climit_ifhandset].CompareNoCase("1") == 0)
			{
				buf.AlarmState |= 0x20;
			}
error=32;

			straddrname = arrField[fyybcp_climit_addrname];
error=33;

			switch (buf.ltype)
			{
			case 18:		// 关键字
				{
					if (v_workmode!=1)
					{
error=4;
						CaseInfo caseinfo(&buf);
						v_limitcount[0][LIMITTYPE_CASEKEYWORD]++;
						//gb23型线索要转换为utf-8和big5型线索，匹配三种编码的线索
						for (int ii=0;ii<3;ii++)
						{
							if (ii == 1)
							{
								CChineseCodeLib cchinesecodelib;
								cchinesecodelib.GB2312ToUTF_8(straddrname,(char*)(LPCSTR)caseinfo.LimitAddr,caseinfo.LimitAddr.GetLength());
								if (straddrname.CompareNoCase(caseinfo.LimitAddr) == 0)//编码后与gb2312一样就不再向匹配线索字典写入
								{
									continue ;
								}
							}
							else if (ii == 2)
							{
								straddrname.Format("%s",caseinfo.LimitAddr);
								CChineseCodeLib cchinesecodelib;
								cchinesecodelib.GB2GBK((char*)(LPCSTR)straddrname);
								if (straddrname.CompareNoCase(caseinfo.LimitAddr) == 0)//编码后与gb2312一样就不再向匹配线索字典写入
								{
									continue ;
								}
							}
							AllDicInfo* lpalldicinfo = NULL;
							if (alldicmap.Lookup(straddrname,(void*&)lpalldicinfo))
							{
								lpalldicinfo->m_caseinfoarry.Add(caseinfo);
							}
							else
							{
								lpalldicinfo = new AllDicInfo;
								lpalldicinfo->m_caseinfoarry.Add(caseinfo);
								alldicmap.SetAt(straddrname,(void*&)lpalldicinfo);
							}
							v_CaseLimitPara.m_ridreallimit[bufid].SetAt(straddrname, caseinfo.CaseID);
							
						}
					}
				}
				break;
			case 19:		// 技侦关键字
				{
error=5;
					if (v_workmode!=1)
					{
						CaseInfo caseinfo(&buf);
						v_limitcount[0][LIMITTYPE_CASEJZWORD]++;
						AllDicInfo* lpalldicinfo = NULL;
						if (alldicmap.Lookup(straddrname,(void*&)lpalldicinfo))
						{
							lpalldicinfo->m_caseinfoarry.Add(caseinfo);
						}
						else
						{
							lpalldicinfo = new AllDicInfo;
							lpalldicinfo->m_caseinfoarry.Add(caseinfo);
							alldicmap.SetAt(straddrname,(void*&)lpalldicinfo);
						}
					}
				}
				break;
			case 14:		// URL
				{
error=6;
					if (v_workmode!=1)
					{
						CaseInfo caseinfo(&buf);
 						v_limitcount[0][LIMITTYPE_CASEURL]++;
						AllDicInfo* lpalldicinfo = NULL;
						straddrname.MakeLower();		// URL要转换成小写字母
						if (alldicmap.Lookup(straddrname,(void*&)lpalldicinfo))
						{
							lpalldicinfo->m_caseinfoarry.Add(caseinfo);
						}
						else
						{
							lpalldicinfo = new AllDicInfo;
							lpalldicinfo->m_caseinfoarry.Add(caseinfo);
							alldicmap.SetAt(straddrname,(void*&)lpalldicinfo);
						}
						v_CaseLimitPara.m_ridreallimit[bufid].SetAt(straddrname, caseinfo.CaseID);
					}
				}
				break;
			case 8:			// IP
				{
error=7;
					char tmpipbuf[220];
					if ((strnicmp(buf.limit, "sip", 3)==0) ||
						(strnicmp(buf.limit, "dip", 3)==0))
					{
						// 去除ip的前缀(sip / dip)
						strcpy(tmpipbuf, buf.limit+3);
						buf.limit = tmpipbuf;
					}
					FormatIp(buf.limit,tmpipbuf);
					buf.limit=tmpipbuf;
					CString key = buf.limit;
					key.TrimRight();
					key.TrimLeft();

					CaseInfoArray *tmpcaseinfoarr = NULL;
					CaseInfo *ptmpcs = NULL;
					ptmpcs = new CaseInfo(&buf);
					if (!v_CaseLimitPara.m_ipmap[bufid].Lookup(key, tmpcaseinfoarr))
					{
						tmpcaseinfoarr = new CaseInfoArray;
						tmpcaseinfoarr->Add(*ptmpcs);
						v_CaseLimitPara.m_ipmap[bufid].SetAt(key, tmpcaseinfoarr);
					}
					else
					{
						tmpcaseinfoarr->Add(*ptmpcs);
					}
					v_limitcount[0][LIMITTYPE_CASEIP]++;

				}
				break;
			case 9:		// MAC
				{
error=8;
					CString key = buf.limit;
					key.TrimRight();
					key.TrimLeft();
					CaseInfoArray *tmpcaseinfoarr = NULL;
					CaseInfo *ptmpcs = NULL;
					ptmpcs = new CaseInfo(&buf);
					key.MakeLower();
					if (!v_CaseLimitPara.m_macmap[bufid].Lookup(key, tmpcaseinfoarr))
					{
						tmpcaseinfoarr = new CaseInfoArray;
						tmpcaseinfoarr->Add(*ptmpcs);
						v_CaseLimitPara.m_macmap[bufid].SetAt(key, tmpcaseinfoarr);
					}
					else
					{
						tmpcaseinfoarr->Add(*ptmpcs);
					}
					v_limitcount[0][LIMITTYPE_CASEMAC]++;

				}
				break;
			case 16:	// 账号
				{
error=9;
					CaseInfoArray *tmpcaseinfoarr = NULL;
					CaseInfo *ptmpcs = NULL;
					ptmpcs = new CaseInfo(&buf);
					CString key = buf.limit;
					key.TrimRight();
					key.TrimLeft();
					key.MakeLower();
					if (!v_CaseLimitPara.m_phoneaccountmap[bufid].Lookup(key, tmpcaseinfoarr))
					{
						tmpcaseinfoarr = new CaseInfoArray;
						tmpcaseinfoarr->Add(*ptmpcs);
						v_CaseLimitPara.m_phoneaccountmap[bufid].SetAt(key, tmpcaseinfoarr);
					}
					else
					{
						tmpcaseinfoarr->Add(*ptmpcs);
					}
						
					v_limitcount[0][LIMITTYPE_CASEACCOUNT]++;
					
				}
				break;
			case 17:	// 电话
				{
error=10;
					CaseInfoArray *tmpcaseinfoarr = NULL;
					CaseInfo *ptmpcs = NULL;
					ptmpcs = new CaseInfo(&buf);
					CString key = buf.limit;
					key.TrimRight();
					key.TrimLeft();
					key.MakeLower();
					if (!v_CaseLimitPara.m_phoneaccountmap[bufid].Lookup(key, tmpcaseinfoarr))
					{
						tmpcaseinfoarr = new CaseInfoArray;
						tmpcaseinfoarr->Add(*ptmpcs);
						v_CaseLimitPara.m_phoneaccountmap[bufid].SetAt(key, tmpcaseinfoarr);
					}
					else
					{
						tmpcaseinfoarr->Add(*ptmpcs);
					}
				}
				break;
			case 34:		// IP+URL
				{
error=12;
					CString strlimit=buf.limit;
					strlimit.TrimRight();
					strlimit.TrimLeft();
					if(strlen(strlimit)<=16 || strlimit[15]!='_')
						break;
					CString key=strlimit.Mid(16);
 					CaseInfo caseinfo(&buf);

					AllDicInfo* lpalldicinfo = NULL;
					key.MakeLower();		// URL要转成小写字母
					if (alldicmap.Lookup(key, (void*&)lpalldicinfo))
					{

						lpalldicinfo->m_caseinfoarry.Add(caseinfo);
					}
					else
					{
						lpalldicinfo = new AllDicInfo;
						lpalldicinfo->m_caseinfoarry.Add(caseinfo);
						alldicmap.SetAt(key,(void*&)lpalldicinfo);
					}

				}
				break;
			case 35:		// IP+PORT
				{
error=14;
					CString strlimit=buf.limit;
					strlimit.TrimRight();
					strlimit.TrimLeft();
					if(strlen(strlimit)<=16 || strlimit[15]!='_')
						break;
	//					strlimit.Replace('_',':');
					CString key=strlimit.Mid(16);

					CaseInfoArray *tmpcaseinfoarr = NULL;
					CaseInfo *ptmpcs = NULL;
					ptmpcs = new CaseInfo(&buf);
					if (!v_CaseLimitPara.m_ipportmap[bufid].Lookup(strlimit, tmpcaseinfoarr))
					{
						tmpcaseinfoarr = new CaseInfoArray;
						tmpcaseinfoarr->Add(*ptmpcs);
						v_CaseLimitPara.m_ipportmap[bufid].SetAt(strlimit, tmpcaseinfoarr);
					}
					else
					{
						tmpcaseinfoarr->Add(*ptmpcs);
					}
				}

				break;
			case 36:		// email
				{
error=17;
					CString strlimit=buf.limit;
					strlimit.TrimRight();
					strlimit.TrimLeft();

					CaseInfoArray *tmpcaseinfoarr = NULL;
					CaseInfo *ptmpcs = NULL;
					ptmpcs = new CaseInfo(&buf);
					if (!v_CaseLimitPara.m_emailmap[bufid].Lookup(strlimit, tmpcaseinfoarr))
					{
						tmpcaseinfoarr = new CaseInfoArray;
						tmpcaseinfoarr->Add(*ptmpcs);
						v_CaseLimitPara.m_emailmap[bufid].SetAt(strlimit, tmpcaseinfoarr);
					}
					else
					{
						tmpcaseinfoarr->Add(*ptmpcs);
					}
				}

				break;
			case 37:		// 即时消息
				{
error=18;
					CString strlimit=buf.limit;
					strlimit.TrimRight();
					strlimit.TrimLeft();

					CaseInfoArray *tmpcaseinfoarr = NULL;
					CaseInfo *ptmpcs = NULL;
					ptmpcs = new CaseInfo(&buf);
					if (!v_CaseLimitPara.m_immap[bufid].Lookup(strlimit, tmpcaseinfoarr))
					{
						tmpcaseinfoarr = new CaseInfoArray;
						tmpcaseinfoarr->Add(*ptmpcs);
						v_CaseLimitPara.m_immap[bufid].SetAt(strlimit, tmpcaseinfoarr);
					}
					else
					{
						tmpcaseinfoarr->Add(*ptmpcs);
					}
				}

				break;
			default:
				break;
			}
error=19;
			arrField.RemoveAll();
			bGetOne = synPara.GetOneRecord(dealPos, arrField);
		}
	}
	catch (...)
	{
		WriteLog("dce", "UpdateFyyOneClimit() Error!! %d",error);
	}
	return bReadOK;
}
BOOL UpdateOneClimit(LPCTSTR szBcp, AllDicMap& alldicmap, int bufid)
{
// 	{"CASEID",		r4str, 50, 0},	// 案件ID
// 	{"OBJECTTYPE",	r4str, 16, 0},	// 对象类型
// 	{"OBJECTNAME",	r4str, 50, 0},	// 对象名称
// 	{"ADDRNAME",	r4str, 200,0},	// 地址数据
// 	{"ADDRTYPE",	r4str, 2,  0},	// 地址类型
// 	{"STATE",		r4str, 1,  0},	// 启用状态
// 	{"IFW",			r4str, 1,  0},	// Web告警
// 	{"IFHANDSET",	r4str, 1,  0},	// 短信告警
// 	{"BUKONG",		r4str, 1,  0},	// 是否布控
// 	{"ADDMAN",		r4str, 50, 0},	// 增加人员
// 	{"ADDTM",		r4str, 20, 0},	// 增加时间									
// 	{"REMARK",		r4str, 100, 0},	// 增加时间									
// 	{"PRO",			r4str, 50, 0},	// 不起作用协议     httpdown,unip,media		
// 	{"WEBPRO",		r4str, 400, 0},	// 页面告警协议     httpdown,unip,media等	
// 	{"HANDPRO",		r4str, 400, 0},	// 手机告警协议     httpdown,unip,media等	
// 	{"CODING",		r4str, 100, 0},	// 关键字类线索的编码类型 GB BIG UTF8默认为GB
#define		bcp_climit_count		16
#define		bcp_climit_caseid		0
#define		bcp_climit_objecttype	1
#define		bcp_climit_objectname	2
#define		bcp_climit_addrname		3
#define		bcp_climit_addrtype		4
#define		bcp_climit_state		5
#define		bcp_climit_ifw			6
#define		bcp_climit_ifhandset	7

	LIMITDATA buf;
	CString straddrname;

	BOOL bReadOK = TRUE;
	CSyncPara synPara;
	DWORD dealPos = 0;
	if(FALSE == synPara.LoadPara(szBcp, dealPos, TRUE))
	{
		return FALSE;
	}
	CStringArray arrField;
	BOOL bGetOne = synPara.GetOneRecord(dealPos, arrField);
	while(bGetOne)
	{
//		ASSERT(arrField.GetSize() == bcp_climit_count);
		if(arrField.GetSize() != bcp_climit_count)
		{
			bReadOK = FALSE;
			break;
		}
// 		static LPCTSTR FIELDSNAME[9] =
// 		{
// 			_T("CASEID"), _T("OBJECTTYPE"), _T("OBJECTNAME"), 
// 			_T("ADDRNAME"), _T("ADDRTYPE"), _T("STATE"),
// 			_T("IFW"), _T("IFHANDSET"), _T("BUKONG")
// 		};

		if ((arrField[bcp_climit_state].GetLength()<=0) || (arrField[bcp_climit_state].GetAt(0) != '1'))
		{
			arrField.RemoveAll();
			bGetOne = synPara.GetOneRecord(dealPos, arrField);
			continue;
		}

		memset(&buf, 0, sizeof(buf));
		buf.ltype= atol(arrField[bcp_climit_addrtype]);		// 地址类型

		if (v_programtype == SYSTEM_SLEUTH)
		{
			if (buf.ltype==18 || buf.ltype==14)
			{
				buf.ifexpand= atol(arrField[bcp_climit_ifw]);	// 是否扩展
			}
		}
		// 增加地址参数
		buf.caseid= arrField[bcp_climit_caseid];		// 案件ID
		buf.objtype= arrField[bcp_climit_objecttype];		// 对象类型
		buf.objname= arrField[bcp_climit_objectname];		// 对象名称
		buf.limit= arrField[bcp_climit_addrname];// 地址数据
		if (arrField[bcp_climit_ifw].CompareNoCase("1") == 0)
		{
			buf.AlarmState |= 0x10;
		}
		if (arrField[bcp_climit_ifhandset].CompareNoCase("1") == 0)
		{
			buf.AlarmState |= 0x20;
		}
		straddrname = arrField[bcp_climit_addrname];
		switch (buf.ltype)
		{
		case 18:		// 关键字
			{
				if (v_workmode!=1)
				{
					CaseInfo caseinfo(&buf);
					v_limitcount[0][LIMITTYPE_CASEKEYWORD]++;
					//gb23型线索要转换为utf-8和big5型线索，匹配三种编码的线索
					for (int ii=0;ii<3;ii++)
					{
						if (ii == 1)
						{
							CChineseCodeLib cchinesecodelib;
							cchinesecodelib.GB2312ToUTF_8(straddrname,(char*)(LPCSTR)caseinfo.LimitAddr,caseinfo.LimitAddr.GetLength());
							if (straddrname.CompareNoCase(caseinfo.LimitAddr) == 0)//编码后与gb2312一样就不再向匹配线索字典写入
							{
								continue ;
							}
						}
						else if (ii == 2)
						{
							straddrname.Format("%s",caseinfo.LimitAddr);
							CChineseCodeLib cchinesecodelib;
							cchinesecodelib.GB2GBK((char*)(LPCSTR)straddrname);
							if (straddrname.CompareNoCase(caseinfo.LimitAddr) == 0)//编码后与gb2312一样就不再向匹配线索字典写入
							{
								continue ;
							}
						}
						AllDicInfo* lpalldicinfo = NULL;
						if (alldicmap.Lookup(straddrname,(void*&)lpalldicinfo))
						{
							lpalldicinfo->m_caseinfoarry.Add(caseinfo);
						}
						else
						{
							lpalldicinfo = new AllDicInfo;
							lpalldicinfo->m_caseinfoarry.Add(caseinfo);
							alldicmap.SetAt(straddrname,(void*&)lpalldicinfo);
						}
						v_CaseLimitPara.m_ridreallimit[bufid].SetAt(straddrname, caseinfo.CaseID);
						
					}
				}
			}
			break;
		case 19:		// 技侦关键字
			{
				if (v_workmode!=1)
				{
					CaseInfo caseinfo(&buf);
					v_limitcount[0][LIMITTYPE_CASEJZWORD]++;
					AllDicInfo* lpalldicinfo = NULL;
					if (alldicmap.Lookup(straddrname,(void*&)lpalldicinfo))
					{
						lpalldicinfo->m_caseinfoarry.Add(caseinfo);
					}
					else
					{
						lpalldicinfo = new AllDicInfo;
						lpalldicinfo->m_caseinfoarry.Add(caseinfo);
						alldicmap.SetAt(straddrname,(void*&)lpalldicinfo);
					}
				}
			}
			break;
		case 14:		// URL
			{
				if (v_workmode!=1)
				{
					CaseInfo caseinfo(&buf);
 					v_limitcount[0][LIMITTYPE_CASEURL]++;
					AllDicInfo* lpalldicinfo = NULL;
					straddrname.MakeLower();		// URL要转换成小写字母
					if (alldicmap.Lookup(straddrname,(void*&)lpalldicinfo))
					{
						lpalldicinfo->m_caseinfoarry.Add(caseinfo);
					}
					else
					{
						lpalldicinfo = new AllDicInfo;
						lpalldicinfo->m_caseinfoarry.Add(caseinfo);
						alldicmap.SetAt(straddrname,(void*&)lpalldicinfo);
					}
					v_CaseLimitPara.m_ridreallimit[bufid].SetAt(straddrname, caseinfo.CaseID);
				}
			}
			break;
		case 8:			// IP
			{
				
				char tmpipbuf[220];
				if ((strnicmp(buf.limit, "sip", 3)==0) ||
					(strnicmp(buf.limit, "dip", 3)==0))
				{
					// 去除ip的前缀(sip / dip)
					strcpy(tmpipbuf, buf.limit+3);
					buf.limit = tmpipbuf;
				}
				FormatIp(buf.limit,tmpipbuf);
				buf.limit=tmpipbuf;
				CString key = buf.limit;
				key.TrimRight();
				key.TrimLeft();

				CaseInfoArray *tmpcaseinfoarr = NULL;
				CaseInfo *ptmpcs = NULL;
				ptmpcs = new CaseInfo(&buf);
				if (!v_CaseLimitPara.m_ipmap[bufid].Lookup(key, tmpcaseinfoarr))
				{
					tmpcaseinfoarr = new CaseInfoArray;
					tmpcaseinfoarr->Add(*ptmpcs);
					v_CaseLimitPara.m_ipmap[bufid].SetAt(key, tmpcaseinfoarr);
				}
				else
				{
					tmpcaseinfoarr->Add(*ptmpcs);
				}
				v_limitcount[0][LIMITTYPE_CASEIP]++;

			}
			break;
		case 9:		// MAC
			{
				CString key = buf.limit;
				key.TrimRight();
				key.TrimLeft();
				CaseInfoArray *tmpcaseinfoarr = NULL;
				CaseInfo *ptmpcs = NULL;
				ptmpcs = new CaseInfo(&buf);
				key.MakeLower();
				if (!v_CaseLimitPara.m_macmap[bufid].Lookup(key, tmpcaseinfoarr))
				{
					tmpcaseinfoarr = new CaseInfoArray;
					tmpcaseinfoarr->Add(*ptmpcs);
					v_CaseLimitPara.m_macmap[bufid].SetAt(key, tmpcaseinfoarr);
				}
				else
				{
					tmpcaseinfoarr->Add(*ptmpcs);
				}
				v_limitcount[0][LIMITTYPE_CASEMAC]++;

			}
			break;
		case 16:	// 账号
			{
				CaseInfoArray *tmpcaseinfoarr = NULL;
				CaseInfo *ptmpcs = NULL;
				ptmpcs = new CaseInfo(&buf);
				CString key = buf.limit;
				key.TrimRight();
				key.TrimLeft();
				key.MakeLower();
				if (!v_CaseLimitPara.m_phoneaccountmap[bufid].Lookup(key, tmpcaseinfoarr))
				{
					tmpcaseinfoarr = new CaseInfoArray;
					tmpcaseinfoarr->Add(*ptmpcs);
					v_CaseLimitPara.m_phoneaccountmap[bufid].SetAt(key, tmpcaseinfoarr);
				}
				else
				{
					tmpcaseinfoarr->Add(*ptmpcs);
				}
					
				v_limitcount[0][LIMITTYPE_CASEACCOUNT]++;
				
			}
			break;
		case 17:	// 电话
			{
				CaseInfoArray *tmpcaseinfoarr = NULL;
				CaseInfo *ptmpcs = NULL;
				ptmpcs = new CaseInfo(&buf);
				CString key = buf.limit;
				key.TrimRight();
				key.TrimLeft();
				key.MakeLower();
				if (!v_CaseLimitPara.m_phoneaccountmap[bufid].Lookup(key, tmpcaseinfoarr))
				{
					tmpcaseinfoarr = new CaseInfoArray;
					tmpcaseinfoarr->Add(*ptmpcs);
					v_CaseLimitPara.m_phoneaccountmap[bufid].SetAt(key, tmpcaseinfoarr);
				}
				else
				{
					tmpcaseinfoarr->Add(*ptmpcs);
				}
			}
			break;
		case 34:		// IP+URL
			{
				CString strlimit=buf.limit;
				strlimit.TrimRight();
				strlimit.TrimLeft();
				if(strlen(strlimit)<=16 || strlimit[15]!='_')
					break;
				CString key=strlimit.Mid(16);
 				CaseInfo caseinfo(&buf);

				AllDicInfo* lpalldicinfo = NULL;
				key.MakeLower();		// URL要转成小写字母
				if (alldicmap.Lookup(key, (void*&)lpalldicinfo))
				{

					lpalldicinfo->m_caseinfoarry.Add(caseinfo);
				}
				else
				{
					lpalldicinfo = new AllDicInfo;
					lpalldicinfo->m_caseinfoarry.Add(caseinfo);
					alldicmap.SetAt(key,(void*&)lpalldicinfo);
				}

			}
			break;
		case 35:		// IP+PORT
			{
				CString strlimit=buf.limit;
				strlimit.TrimRight();
				strlimit.TrimLeft();
				if(strlen(strlimit)<=16 || strlimit[15]!='_')
					break;
//					strlimit.Replace('_',':');
				CString key=strlimit.Mid(16);

				CaseInfoArray *tmpcaseinfoarr = NULL;
				CaseInfo *ptmpcs = NULL;
				ptmpcs = new CaseInfo(&buf);
				if (!v_CaseLimitPara.m_ipportmap[bufid].Lookup(strlimit, tmpcaseinfoarr))
				{
					tmpcaseinfoarr = new CaseInfoArray;
					tmpcaseinfoarr->Add(*ptmpcs);
					v_CaseLimitPara.m_ipportmap[bufid].SetAt(strlimit, tmpcaseinfoarr);
				}
				else
				{
					tmpcaseinfoarr->Add(*ptmpcs);
				}
			}

			break;
		case 36:		// email
			{
				CString strlimit=buf.limit;
				strlimit.TrimRight();
				strlimit.TrimLeft();

				CaseInfoArray *tmpcaseinfoarr = NULL;
				CaseInfo *ptmpcs = NULL;
				ptmpcs = new CaseInfo(&buf);
				if (!v_CaseLimitPara.m_emailmap[bufid].Lookup(strlimit, tmpcaseinfoarr))
				{
					tmpcaseinfoarr = new CaseInfoArray;
					tmpcaseinfoarr->Add(*ptmpcs);
					v_CaseLimitPara.m_emailmap[bufid].SetAt(strlimit, tmpcaseinfoarr);
				}
				else
				{
					tmpcaseinfoarr->Add(*ptmpcs);
				}
			}

			break;
		case 37:		// 即时消息
			{
				CString strlimit=buf.limit;
				strlimit.TrimRight();
				strlimit.TrimLeft();

				CaseInfoArray *tmpcaseinfoarr = NULL;
				CaseInfo *ptmpcs = NULL;
				ptmpcs = new CaseInfo(&buf);
				if (!v_CaseLimitPara.m_immap[bufid].Lookup(strlimit, tmpcaseinfoarr))
				{
					tmpcaseinfoarr = new CaseInfoArray;
					tmpcaseinfoarr->Add(*ptmpcs);
					v_CaseLimitPara.m_immap[bufid].SetAt(strlimit, tmpcaseinfoarr);
				}
				else
				{
					tmpcaseinfoarr->Add(*ptmpcs);
				}
			}

			break;
		default:
			break;
		}

		arrField.RemoveAll();
		bGetOne = synPara.GetOneRecord(dealPos, arrField);
	}
	

	return bReadOK;
}
#include "rbstringarray.h"

BOOL UpdateDictionary(AllDicMap* alldicmap,int bufid)
{
int error=0;
	try
	{
		AllDicInfo* lpalldicinfo;
		POSITION pos = alldicmap->GetStartPosition();
		CString mapkey;
		BOOL HaveKey = FALSE;
		int listnum=0;
		CRbStringArray sorarry;
		while (pos != NULL)
		{
			alldicmap->GetNextAssoc(pos,mapkey,(void*&)lpalldicinfo);
			sorarry.Add(mapkey);
			listnum++;
		}
error=1;
		sorarry.Sort();
		for (int mm=0;mm<sorarry.GetSize();mm++)
		{
			if (alldicmap->Lookup(sorarry[mm],(void*&)lpalldicinfo))
			{
				Vocable temp;
				temp.m_Vocable.Format("%s",sorarry[mm]);
				temp.m_pUserData = (VOID*)lpalldicinfo;
				v_CaseLimitPara.m_keyworddic[bufid].m_VocableList.push_front(temp);
				HaveKey = TRUE;
			}
		}
error=2;
		if (HaveKey)
		{
			v_CaseLimitPara.m_keyworddic[bufid].IniKeyBTree(200000);
		}
		
		return TRUE;
	}
	catch(...)
	{
		WriteLog("dce", "UpdateDictionary() Error!! %d",error);
		return FALSE;
	}

}

BOOL ClearCaseMap(CaseIpMacMap *caseipmacmap)
{
	POSITION pos=caseipmacmap->GetStartPosition();
	CaseInfoArray *p;
	
	CString key;
	while (pos!=NULL)
	{
		caseipmacmap->GetNextAssoc(pos,key,p);
		delete p;
		p = NULL;
	}
	caseipmacmap->RemoveAll();
	return TRUE;
}

BOOL UpdateCLimitPara(LPCTSTR szBcp)
{
	int error=0;
	try
	{
		ASSERT(v_CaseLimitPara.m_readflg==0 || v_CaseLimitPara.m_readflg==1);

		//只要有一个需要更新案件线索的地方，后续调用updateclimitpara的都不再调用。案件重点人员一块更新
		for (int n=0; n<g_tablenum; n++)
		{
			if (g_parainfo[n].pUpdateFunc == UpdateCLimitPara)
			{
				g_parainfo[n].updateflg = FALSE;
			}
		}
error=1;
		long	bufid = (v_CaseLimitPara.m_readflg==0)? 1 : 0;
		memset(v_limitcount[0], 0, (LIMITTYPE_HTTPWORD-LIMITTYPE_CASEIP)*4);
		memset(v_limitcount[0], 0, (LIMITTYPE_CASEJZWORD+1)*4);

		ClearCaseMap(&v_CaseLimitPara.m_ipmap[bufid]);
		ClearCaseMap(&v_CaseLimitPara.m_macmap[bufid]);
		ClearCaseMap(&v_CaseLimitPara.m_phoneaccountmap[bufid]);
error=2;
		v_CaseLimitPara.m_ridreallimit[bufid].RemoveAll();
	//		v_CaseLimitPara.m_vlanidMap[bufid].RemoveAll();
		v_CaseLimitPara.m_immap[bufid].RemoveAll();
		v_CaseLimitPara.m_emailmap[bufid].RemoveAll();
		v_CaseLimitPara.m_urllist[bufid].clear();
		v_CaseLimitPara.m_keyworddic[bufid].ClearData();

error=3;
		AllDicMap alldicmap;
		alldicmap.InitHashTable(50051);
		if(v_programtype != SYSTEM_SLEUTH)
		{
			// 读案件线索表
			UpdateFyyOneClimit("e:\\netspy\\para\\dce\\climit.bcp", alldicmap, bufid);

		}
		else
		{
			// 读案件线索表
			UpdateOneClimit(szBcp, alldicmap, bufid);

		}
error=4;
	// 	for (int i=0; i<climitarr.GetSize(); i++)
	// 	{
	// 		ReadCaseLimitDbf(climitarr[i], alldicmap, bufid);
	// 	}

		//更新预警的关键字字典，跟案件关键字放到一个字典中
	//	UpdateIndexRule(&alldicmap, bufid);
		UpdateWarnLimit(&alldicmap, bufid);
error=5;
		UpdateDictionary(&alldicmap, bufid);
error=55;
		
		InterlockedExchange(&v_CaseLimitPara.m_readflg, bufid);
		bufid=(v_CaseLimitPara.m_readflg==0)? 1 : 0;
// 		if(v_warnlimitPara.m_mempool[bufid]!=NULL)
// 		{
// 			delete[] v_warnlimitPara.m_mempool[bufid];
// 			v_warnlimitPara.m_mempool[bufid]=NULL;
// 		}
error=7;
// 		if (v_warnlimitPara.m_lpalldicinfo[bufid]!=NULL)
// 		{
// 			delete[] v_warnlimitPara.m_lpalldicinfo[bufid];
// 			v_warnlimitPara.m_lpalldicinfo[bufid]=NULL;
// 		}
error=6;
		return TRUE;
	}
	catch(...)
	{
		WriteLog("dce", "UpdateCLimitPara() Error!! %d",error);
		return FALSE;
	}	// 清空原缓冲区


}

BOOL UpdateNewSystemPara(LPCTSTR szBcp)
{
#define		OPEN_MEDIA_QQ		0
#define		OPEN_MEDIA_BLUESKY	1
#define		OPEN_MEDIA_EHT		2
#define		OPEN_MEDIA_MSN		3
#define		OPEN_MEDIA_POPO		4
#define		OPEN_MEDIA_FLASH	5
#define		OPEN_MEDIA_SOQ		6
#define		OPEN_MEDIA_UC		7
#define		OPEN_MEDIA_YAHOO	8
#define		OPEN_MEDIA_NETM		9
#define		OPEN_MEDIA_VOIP		10
#define		OPEN_MEDIA_BDC		11
#define		OPEN_MEDIA_ASF		12
#define		OPEN_MEDIA_VP		13
#define		OPEN_MEDIA_UUTong	14

	
#define		OPEN_INDEX_SMTP		0
#define		OPEN_INDEX_POP3		1
#define		OPEN_INDEX_HTTPUP	2
#define		OPEN_INDEX_HTTPDOWN	3
#define		OPEN_INDEX_FTP		4
#define		OPEN_INDEX_TELNET	5
#define		OPEN_INDEX_OICQ		6
#define		OPEN_INDEX_YAHOO	7
#define		OPEN_INDEX_MSN		8
#define		OPEN_INDEX_ICQAOL	9
#define		OPEN_INDEX_MIR		10
#define		OPEN_INDEX_OURGAME	11
#define		OPEN_INDEX_NOG		12
#define		OPEN_INDEX_VOIPG	13
#define		OPEN_INDEX_BF		14
#define		OPEN_INDEX_SOQ		15
#define		OPEN_INDEX_EHTCHAT	16
#define		OPEN_INDEX_VPCHAT	17
#define		OPEN_INDEX_EMULE	18
#define		OPEN_INDEX_VNC		19
#define		OPEN_INDEX_RTSP		20
#define		OPEN_INDEX_HF		21
#define		OPEN_INDEX_FGUP		22
#define		OPEN_INDEX_FGDN		23
#define		OPEN_INDEX_FGURL	24
#define		OPEN_INDEX_DYNAUP	25
#define		OPEN_INDEX_DYNADN	26
#define		OPEN_INDEX_DYNAURL	27
#define		OPEN_INDEX_WUJIE	28
#define		OPEN_INDEX_UNIP		29
#define		OPEN_INDEX_TAOBAO	30
#define		OPEN_INDEX_FETION	31
#define		OPEN_INDEX_POPO		32
#define		OPEN_INDEX_UC		33



	int bcp_system_paraid;
	int bcp_system_paraval;
	int bcp_system_count = 7;
	if(v_programtype != SYSTEM_SLEUTH)
	{
		bcp_system_count = 4;
		bcp_system_paraid=0;
		bcp_system_paraval=2;		
	}
	else
	{
		bcp_system_paraid=0;
		bcp_system_paraval=3;		
	}
	//int Indexidx, Caseidx, Mediaidx;
	int openindex[200];
// 	int opencase[60];
// 	int openwarn[60];
// 	int openmedia[60];
// 	int casemedia[60];
// 	int warnmedia[60];
	for (int n=0; n<200; n++)
	{
		openindex[n] = 1;
// 		opencase[n] = 1;
// 		openmedia[n] = 1;
// 		casemedia[n] = 1;
// 		openwarn[n] = 1;
// 		warnmedia[n] = 1;
	}
//	opencase[OPEN_CASE_IMAGE] = 0;

	BOOL bReadOK = TRUE;
	CString paraval;
	DWORD paraid, dwparaval;

	CSyncPara synPara;
	DWORD dealPos = 0;
	if(FALSE == synPara.LoadPara(szBcp, dealPos, TRUE))
	{
		return FALSE;
	}
	CStringArray arrField;
	BOOL bGetOne = synPara.GetOneRecord(dealPos, arrField);
	while(bGetOne)
	{
		ASSERT(arrField.GetSize() == bcp_system_count);
		if(arrField.GetSize() != bcp_system_count)
		{
			bReadOK = FALSE;
			break;
		}
//		CString tmp = arrField[2];
		paraid = atoi(arrField[bcp_system_paraid]);
		paraval = CString(arrField[bcp_system_paraval]);
		dwparaval = atoi(paraval);

		if(paraid == 17102)	
		{
			// IDC管理类统计
			v_statIdc = dwparaval;
		}
		
		if(paraid == 28101)	
		{
			// 音视频轮换时长 (10 - 60分钟)
			int val = dwparaval;
			if (val < 10)
				val = 10;
			if (val > 60)
				val = 60;
			InterlockedExchange(&v_mediainterval, val * 60);		// 换算成秒
		}
		if(paraid == 28104)	// 视频数据的抽样比例
		{
			int val = dwparaval;
			if (val <= 256)
				InterlockedExchange(&v_mediasample, 1);		// 换算成秒
			else
				InterlockedExchange(&v_mediasample, val / 256);		// 换算成秒
		}

		if(paraid >= 0 && paraid <= 1000)
		{
			switch (paraid)
			{
			case 1:		
				InterlockedExchange((long *)&v_openflagsmtp,	dwparaval);break;
			case 2:		
				InterlockedExchange((long *)&v_openflagpop3,	dwparaval);break;
			case 3:		
				InterlockedExchange((long *)&v_openflagwebmail,	dwparaval);break;	
			case 4:		
				InterlockedExchange((long *)&v_openflaghttpup,	dwparaval);break;
			case 5:		
				InterlockedExchange((long *)&v_openflaghttpdown,	dwparaval);break;
// 			case 6:		
// 				InterlockedExchange((long *)&v_openflaghttpsup,	dwparaval);break;				
// 			case 7:		
// 				InterlockedExchange((long *)&v_openflaghttpsdown,	dwparaval);break;
			case 8:		
				InterlockedExchange((long *)&v_openflagunip,	dwparaval);break;
			case 9:		
				InterlockedExchange((long *)&v_openflaghttpurl,	dwparaval);break;				
			case 10:		
				InterlockedExchange((long *)&v_openflagvgate,	dwparaval);break;
			case 11:		
				InterlockedExchange((long *)&v_openflagvnc,	dwparaval);break;
			case 12:		
				InterlockedExchange((long *)&v_openflagrtsp,	dwparaval);break;	
			case 13:		
				InterlockedExchange((long *)&v_openflagemule,	dwparaval);break;
			case 14:		
				InterlockedExchange((long *)&v_openflagremote,	dwparaval);break;
			case 15:		
				InterlockedExchange((long *)&v_openflagterminal,	dwparaval);break;				
			case 16:		
				InterlockedExchange((long *)&v_openflaghttps,	dwparaval);break;
			case 51:		
				InterlockedExchange((long *)&v_openflagoicq,	dwparaval);break;
			case 52:		
				InterlockedExchange((long *)&v_openflagoicqcontent,	dwparaval);break;
			case 53:		
				InterlockedExchange((long *)&v_openflagoicqraw,	dwparaval);break;
			case 54:		
				InterlockedExchange((long *)&v_openflagmsn,	dwparaval);break;
			case 55:		
				InterlockedExchange((long *)&v_openflagyahoo,	dwparaval);break;
			case 56:		
				InterlockedExchange((long *)&v_openflagaol,	dwparaval);break;
			case 57:		
				InterlockedExchange((long *)&v_openflagEht,	dwparaval);break;
			case 58:		
				InterlockedExchange((long *)&v_openflagSoQ,	dwparaval);break;
			case 59:		
				InterlockedExchange((long *)&v_openflagtaobao,	dwparaval);break;
			case 60:		
				InterlockedExchange((long *)&v_openflagfetion,	dwparaval);break;
			case 61:		
				InterlockedExchange((long *)&v_openflagpopo,	dwparaval);break;
			case 62:		
				InterlockedExchange((long *)&v_openflaguc,	dwparaval);break;
			case 63:		
				InterlockedExchange((long *)&v_openflagVp,	dwparaval);break;
			case 101:		
				InterlockedExchange((long *)&v_openflagchuanqi,	dwparaval);break;
			case 102:		
				InterlockedExchange((long *)&v_openflaglzgame,	dwparaval);break;
			case 103:		
				InterlockedExchange((long *)&v_openflagbf,	dwparaval);break;
			case 104:		
				InterlockedExchange((long *)&v_openflaghf,	dwparaval);break;
			case 105:		
				InterlockedExchange((long *)&v_openflagqqgame,	dwparaval);break;
			case 106:		
				InterlockedExchange((long *)&v_openflaggameperfect,	dwparaval);break;												
			case 151:		
				InterlockedExchange((long *)&v_openflagFlashMedia,	dwparaval);break;
			case 152:		
				InterlockedExchange((long *)&v_openflagUCMedia,	dwparaval);break;
			case 153:		
				InterlockedExchange((long *)&v_openflagQQMedia,	dwparaval);break;
			case 154:		
				InterlockedExchange((long *)&v_openflagPoPoMedia,	dwparaval);break;
			case 155:		
				InterlockedExchange((long *)&v_openflagSoQMedia,	dwparaval);break;
			case 156:		
				InterlockedExchange((long *)&v_openflagMSNMedia,	dwparaval);break;
			case 157:		
				InterlockedExchange((long *)&v_openflagBLMedia,	dwparaval);break;
			case 158:		
				InterlockedExchange((long *)&v_openflagYahooMedia,	dwparaval);break;
			case 159:		
				InterlockedExchange((long *)&v_openUUTongMedia,	dwparaval);break;
			case 160:		
				InterlockedExchange((long *)&v_openflagVPMedia,	dwparaval);break;
			case 161:		
				InterlockedExchange((long *)&v_openflagVoip,	dwparaval);break;
			case 162:		
				InterlockedExchange((long *)&v_openflagXunTongMedia,	dwparaval);break;
			case 163:		
				InterlockedExchange((long *)&v_openflagEPHMedia,	dwparaval);break;
			case 201:		
				InterlockedExchange((long *)&v_openflagwujieup,	dwparaval);break;
			case 202:		
				InterlockedExchange((long *)&v_openflagwujiedn,	dwparaval);break;
			case 203:		
				InterlockedExchange((long *)&v_openflagwujie,	dwparaval);break;
			case 204:		
				InterlockedExchange((long *)&v_openflagfgup,	dwparaval);break;
			case 205:		
				InterlockedExchange((long *)&v_openflagfgdn,	dwparaval);break;
			case 206:		
				InterlockedExchange((long *)&v_openflagfgurl,	dwparaval);break;
			case 207:		
				InterlockedExchange((long *)&v_openflagdynaup,	dwparaval);break;
			case 208:		
				InterlockedExchange((long *)&v_openflagdynadn,	dwparaval);break;
			case 209:		
				InterlockedExchange((long *)&v_openflagdynaurl,	dwparaval);break;
			case 210:		
				InterlockedExchange((long *)&v_openflagfirephoenix,	dwparaval);break;
			case 211:		
				InterlockedExchange((long *)&v_openflagftp,	dwparaval);break;
			case 212:		
				InterlockedExchange((long *)&v_openflagtelnet,	dwparaval);break;
			case 229:		
				InterlockedExchange((long *)&v_openflagwow,	dwparaval);break;
			case 241:		
				InterlockedExchange((long *)&v_openflagimsg51,	dwparaval);break;
			case 243:		
				InterlockedExchange((long *)&v_openflagdiicall,	dwparaval);break;
			case 244:		
				InterlockedExchange((long *)&v_openflagwcall,	dwparaval);break;
			case 245:		
				InterlockedExchange((long *)&v_openflagstarnet,	dwparaval);break;
			case 246:		
				InterlockedExchange((long *)&v_openflageyebeam,	dwparaval);break;
			case 247:		
				InterlockedExchange((long *)&v_openflaggizmo,	dwparaval);break;
			case 248:		
				InterlockedExchange((long *)&v_openflagmtalk,	dwparaval);break;
			case 249:		
				InterlockedExchange((long *)&v_openflagmrtalk,	dwparaval);break;
			case 250:		
				InterlockedExchange((long *)&v_openflagheyoo,	dwparaval);break;
			case 251:		
				InterlockedExchange((long *)&v_openflagteltel,	dwparaval);break;
			case 252:		
				InterlockedExchange((long *)&v_openflag66call,	dwparaval);break;
			case 253:		
				InterlockedExchange((long *)&v_openflagqqtranfile,	dwparaval);break;
			case 254:		
				InterlockedExchange((long *)&v_openflagdayingjia,	dwparaval);break;
			case 64:		
				InterlockedExchange((long *)&v_openflagbaiduhi,	dwparaval);break;
																																												
			default:
				InterlockedExchange((long *)&v_openflagtrojan,	dwparaval);
				break;
			}
		}


// 		if(paraid >= 11101 && paraid <= 11134)
// 		{
// 			Indexidx = paraid - 11101;
// 			openindex[Indexidx] = dwparaval;	
// 		}
// 
// 		if(paraid >= 15101 && paraid <= 15134)
// 		{
// 			Indexidx = paraid - 15101;
// 			openwarn[Indexidx] = dwparaval;
// 		}
// 
// 		if(paraid >= 13101 && paraid <= 13135)
// 		{
// 			Caseidx = paraid - 13101;
// 			opencase[Caseidx] = dwparaval;
// 		}
// 
// 		if(paraid >= 11161 && paraid <= 11175)
// 		{
// 			Mediaidx = paraid - 11161;
// 			openmedia[Mediaidx] = dwparaval;
// 		}
// 
// 		if(paraid >= 13161 && paraid <= 13175)
// 		{
// 			Mediaidx = paraid - 13161;
// 			casemedia[Mediaidx] = dwparaval;
// 		}
// 		
// 		if(paraid >= 15161 && paraid <= 15175)
// 		{
// 			Mediaidx = paraid - 15161;
// 			warnmedia[Mediaidx] = dwparaval;
// 		}

		arrField.RemoveAll();
		bGetOne = synPara.GetOneRecord(dealPos, arrField);
	}

	// 预警类开关
	InterlockedExchange((long *)&v_openflagindeximage, 0);
// 	InterlockedExchange((long *)&v_openflagsmtp,	openindex[OPEN_INDEX_SMTP]);
// 	InterlockedExchange((long *)&v_openflagpop3,	openindex[OPEN_INDEX_POP3]);
// 	InterlockedExchange((long *)&v_openflaghttpup,	openindex[OPEN_INDEX_HTTPUP]);
// 	InterlockedExchange((long *)&v_openflaghttpdown,openindex[OPEN_INDEX_HTTPDOWN]);
// 	InterlockedExchange((long *)&v_openflagftp,		openindex[OPEN_INDEX_FTP]);
// 	InterlockedExchange((long *)&v_openflagtelnet,	openindex[OPEN_INDEX_TELNET]);
// 	InterlockedExchange((long *)&v_openflagoicq,	openindex[OPEN_INDEX_OICQ]);
// 	InterlockedExchange((long *)&v_openflagyahoo,	openindex[OPEN_INDEX_YAHOO]);
// 	InterlockedExchange((long *)&v_openflagmsn,		openindex[OPEN_INDEX_MSN]);
// 	InterlockedExchange((long *)&v_openflagaol,		openindex[OPEN_INDEX_ICQAOL]);
// 	InterlockedExchange((long *)&v_openflagchuanqi, openindex[OPEN_INDEX_MIR]);
// 	InterlockedExchange((long *)&v_openflaglzgame,	openindex[OPEN_INDEX_OURGAME]);
// 	InterlockedExchange((long *)&v_openflagnog,		openindex[OPEN_INDEX_NOG]);
// 	InterlockedExchange((long *)&v_openflagvgate,	openindex[OPEN_INDEX_VOIPG]);
// 	InterlockedExchange((long *)&v_openflagbf,		openindex[OPEN_INDEX_BF]);
// 	InterlockedExchange((long *)&v_openflagSoQ,		openindex[OPEN_INDEX_SOQ]);
// 	InterlockedExchange((long *)&v_openflagEht,		openindex[OPEN_INDEX_EHTCHAT]);
// 	InterlockedExchange((long *)&v_openflagVp,		openindex[OPEN_INDEX_VPCHAT]);
// 	InterlockedExchange((long *)&v_openflagemule,	openindex[OPEN_INDEX_EMULE]);
// 	InterlockedExchange((long *)&v_openflagvnc,		openindex[OPEN_INDEX_VNC]);
// 	InterlockedExchange((long *)&v_openflagrtsp,	openindex[OPEN_INDEX_RTSP]);
// 	InterlockedExchange((long *)&v_openflaghf,		openindex[OPEN_INDEX_HF]);
// 	InterlockedExchange((long *)&v_openflagunip,	openindex[OPEN_INDEX_UNIP]);
// 	// 全文NOG
// 	InterlockedExchange((long *)&v_openflagfgup,	openindex[OPEN_INDEX_FGUP]);		
// 	InterlockedExchange((long *)&v_openflagfgdn,	openindex[OPEN_INDEX_FGDN]);		
// 	InterlockedExchange((long *)&v_openflagfgurl,	openindex[OPEN_INDEX_FGURL]);	
// 	InterlockedExchange((long *)&v_openflagdynaup,	openindex[OPEN_INDEX_DYNAUP]);
// 	InterlockedExchange((long *)&v_openflagdynadn,	openindex[OPEN_INDEX_DYNADN]);	
// 	InterlockedExchange((long *)&v_openflagdynaurl,	openindex[OPEN_INDEX_DYNAURL]);	
// 	InterlockedExchange((long *)&v_openflagwujie,	openindex[OPEN_INDEX_WUJIE]);	
// 
// 	// 预警视频类开关
// 	InterlockedExchange((long *)&v_openflagQQMedia,		openmedia[OPEN_MEDIA_QQ]);
// 	InterlockedExchange((long *)&v_openflagBLMedia,		openmedia[OPEN_MEDIA_BLUESKY]);
// 	InterlockedExchange((long *)&v_openflagEPHMedia,	openmedia[OPEN_MEDIA_EHT]);
// 	InterlockedExchange((long *)&v_openflagMSNMedia,	openmedia[OPEN_MEDIA_MSN]);
// 	InterlockedExchange((long *)&v_openflagPoPoMedia,	openmedia[OPEN_MEDIA_POPO]);
// 	InterlockedExchange((long *)&v_openflagFlashMedia,	openmedia[OPEN_MEDIA_FLASH]);
// 	InterlockedExchange((long *)&v_openflagSoQMedia,	openmedia[OPEN_MEDIA_SOQ]);
// 	InterlockedExchange((long *)&v_openflagUCMedia,		openmedia[OPEN_MEDIA_UC]);
// 	InterlockedExchange((long *)&v_openflagYahooMedia,	openmedia[OPEN_MEDIA_YAHOO]);
// 	InterlockedExchange((long *)&v_openflagNetMetMedia,	openmedia[OPEN_MEDIA_NETM]);
// 	InterlockedExchange((long *)&v_openflagVoip,		openmedia[OPEN_MEDIA_VOIP]);
// 	InterlockedExchange((long *)&v_openflagBDCMedia,	openmedia[OPEN_MEDIA_BDC]);
// 	InterlockedExchange((long *)&v_openflagASFMedia,	openmedia[OPEN_MEDIA_ASF]);
// //	InterlockedExchange((long *)&v_openflagOYesCamMedia,openmedia[OPEN_MEDIA_OYESCAM]);
// 	InterlockedExchange((long *)&v_openflagVPMedia,		openmedia[OPEN_MEDIA_VP]);
// 	InterlockedExchange((long *)&v_openUUTongMedia,		openmedia[OPEN_MEDIA_UUTong]);

	// 预警类开关
	InterlockedExchange((long *)&v_openwarnindeximage, 0);
// 	InterlockedExchange((long *)&v_openwarnsmtp,	openwarn[OPEN_INDEX_SMTP]);
// 	InterlockedExchange((long *)&v_openwarnpop3,	openwarn[OPEN_INDEX_POP3]);
// 	InterlockedExchange((long *)&v_openwarnhttpup,	openwarn[OPEN_INDEX_HTTPUP]);
// 	InterlockedExchange((long *)&v_openwarnhttpdown,openwarn[OPEN_INDEX_HTTPDOWN]);
// 	InterlockedExchange((long *)&v_openwarnftp,		openwarn[OPEN_INDEX_FTP]);
// 	InterlockedExchange((long *)&v_openwarntelnet,	openwarn[OPEN_INDEX_TELNET]);
// 	InterlockedExchange((long *)&v_openwarnoicq,	openwarn[OPEN_INDEX_OICQ]);
// 	InterlockedExchange((long *)&v_openwarnyahoo,	openwarn[OPEN_INDEX_YAHOO]);
// 	InterlockedExchange((long *)&v_openwarnmsn,		openwarn[OPEN_INDEX_MSN]);
// 	InterlockedExchange((long *)&v_openwarnaol,		openwarn[OPEN_INDEX_ICQAOL]);
// 	InterlockedExchange((long *)&v_openwarnchuanqi, openwarn[OPEN_INDEX_MIR]);
// 	InterlockedExchange((long *)&v_openwarnlzgame,	openwarn[OPEN_INDEX_OURGAME]);
// 	InterlockedExchange((long *)&v_openwarnnog,		openwarn[OPEN_INDEX_NOG]);
// 	InterlockedExchange((long *)&v_openwarnvgate,	openwarn[OPEN_INDEX_VOIPG]);
// 	InterlockedExchange((long *)&v_openwarnbf,		openwarn[OPEN_INDEX_BF]);
// 	InterlockedExchange((long *)&v_openwarnSoQ,		openwarn[OPEN_INDEX_SOQ]);
// 	InterlockedExchange((long *)&v_openwarnEht,		openwarn[OPEN_INDEX_EHTCHAT]);
// 	InterlockedExchange((long *)&v_openwarnVp,		openwarn[OPEN_INDEX_VPCHAT]);
// 	InterlockedExchange((long *)&v_openwarnemule,	openwarn[OPEN_INDEX_EMULE]);
// 	InterlockedExchange((long *)&v_openwarnvnc,		openwarn[OPEN_INDEX_VNC]);
// 	InterlockedExchange((long *)&v_openwarnrtsp,	openwarn[OPEN_INDEX_RTSP]);
// 	InterlockedExchange((long *)&v_openwarnhf,		openwarn[OPEN_INDEX_HF]);
// 	InterlockedExchange((long *)&v_openwarnunip,	openwarn[OPEN_INDEX_UNIP]);
// 	InterlockedExchange((long *)&v_openwarntaobao,	openwarn[OPEN_INDEX_TAOBAO]);
// 	InterlockedExchange((long *)&v_openwarnfetion,	openwarn[OPEN_INDEX_FETION]);
// 	InterlockedExchange((long *)&v_openwarnpopo,	openwarn[OPEN_INDEX_POPO]);
// 	InterlockedExchange((long *)&v_openwarnuc,		openwarn[OPEN_INDEX_UC]);
// 
// 	// 全文NOG
// 	InterlockedExchange((long *)&v_openwarnfgup,	openwarn[OPEN_INDEX_FGUP]);		
// 	InterlockedExchange((long *)&v_openwarnfgdn,	openwarn[OPEN_INDEX_FGDN]);		
// 	InterlockedExchange((long *)&v_openwarnfgurl,	openwarn[OPEN_INDEX_FGURL]);	
// 	InterlockedExchange((long *)&v_openwarndynaup,	openwarn[OPEN_INDEX_DYNAUP]);
// 	InterlockedExchange((long *)&v_openwarndynadn,	openwarn[OPEN_INDEX_DYNADN]);	
// 	InterlockedExchange((long *)&v_openwarndynaurl,	openwarn[OPEN_INDEX_DYNAURL]);	
// 	InterlockedExchange((long *)&v_openwarnwujie,	openwarn[OPEN_INDEX_WUJIE]);	
// 	
// 	// 预警视频类开关
// 	InterlockedExchange((long *)&v_openwarnQQMedia,		warnmedia[OPEN_MEDIA_QQ]);
// 	InterlockedExchange((long *)&v_openwarnBLMedia,		warnmedia[OPEN_MEDIA_BLUESKY]);
// 	InterlockedExchange((long *)&v_openwarnEPHMedia,	warnmedia[OPEN_MEDIA_EHT]);
// 	InterlockedExchange((long *)&v_openwarnMSNMedia,	warnmedia[OPEN_MEDIA_MSN]);
// 	InterlockedExchange((long *)&v_openwarnPoPoMedia,	warnmedia[OPEN_MEDIA_POPO]);
// 	InterlockedExchange((long *)&v_openwarnFlashMedia,	warnmedia[OPEN_MEDIA_FLASH]);
// 	InterlockedExchange((long *)&v_openwarnSoQMedia,	warnmedia[OPEN_MEDIA_SOQ]);
// 	InterlockedExchange((long *)&v_openwarnUCMedia,		warnmedia[OPEN_MEDIA_UC]);
// 	InterlockedExchange((long *)&v_openwarnYahooMedia,	warnmedia[OPEN_MEDIA_YAHOO]);
// 	InterlockedExchange((long *)&v_openwarnNetMetMedia,	warnmedia[OPEN_MEDIA_NETM]);
// 	InterlockedExchange((long *)&v_openwarnVoip,		warnmedia[OPEN_MEDIA_VOIP]);
// 	InterlockedExchange((long *)&v_openwarnBDCMedia,	warnmedia[OPEN_MEDIA_BDC]);
// 	InterlockedExchange((long *)&v_openwarnASFMedia,	warnmedia[OPEN_MEDIA_ASF]);
// 	//	InterlockedExchange((long *)&v_openwarnOYesCamMedia,warnmedia[OPEN_MEDIA_OYESCAM]);
// 	InterlockedExchange((long *)&v_openwarnVPMedia,		warnmedia[OPEN_MEDIA_VP]);
// 	InterlockedExchange((long *)&v_openwarnUUTongMedia,	warnmedia[OPEN_MEDIA_UUTong]);
	

	return bReadOK;
}

BOOL UpdateSystemPara(LPCTSTR szBcp)
{
	if(v_programtype != SYSTEM_SLEUTH)
	{
		UpdateNewSystemPara(szBcp);
		return TRUE;
	}
#define		OPEN_MEDIA_QQ		0
#define		OPEN_MEDIA_BLUESKY	1
#define		OPEN_MEDIA_EHT		2
#define		OPEN_MEDIA_MSN		3
#define		OPEN_MEDIA_POPO		4
#define		OPEN_MEDIA_FLASH	5
#define		OPEN_MEDIA_SOQ		6
#define		OPEN_MEDIA_UC		7
#define		OPEN_MEDIA_YAHOO	8
#define		OPEN_MEDIA_NETM		9
#define		OPEN_MEDIA_VOIP		10
#define		OPEN_MEDIA_BDC		11
#define		OPEN_MEDIA_ASF		12
#define		OPEN_MEDIA_VP		13
#define		OPEN_MEDIA_UUTong	14


#define		OPEN_CASE_SMTP		0
#define		OPEN_CASE_POP3		1
#define		OPEN_CASE_HTTPUP	2
#define		OPEN_CASE_HTTPDOWN	3
#define		OPEN_CASE_FTP		4
#define		OPEN_CASE_TELNET	5
#define		OPEN_CASE_OICQ		6
#define		OPEN_CASE_YAHOO		7
#define		OPEN_CASE_MSN		8
#define		OPEN_CASE_ICQAOL	9
#define		OPEN_CASE_MIR		10
#define		OPEN_CASE_OURGAME	11
#define		OPEN_CASE_NOG		12
#define		OPEN_CASE_VOIPG		13
#define		OPEN_CASE_BF		14
#define		OPEN_CASE_SOQ		15
#define		OPEN_CASE_EHTCHAT	16
#define		OPEN_CASE_VPCHAT	17
#define		OPEN_CASE_IMAGE		18
#define		OPEN_CASE_EMULE		19
#define		OPEN_CASE_VNC		20
#define		OPEN_CASE_RTSP		21
#define		OPEN_CASE_HF		22
#define		OPEN_CASE_FGUP		23
#define		OPEN_CASE_FGDN		24
#define		OPEN_CASE_FGURL		25
#define		OPEN_CASE_DYNAUP	26
#define		OPEN_CASE_DYNADN	27
#define		OPEN_CASE_DYNAURL	28
#define		OPEN_CASE_WUJIE		29
#define		OPEN_CASE_UNIP		30
#define		OPEN_CASE_TAOBAO	31
#define		OPEN_CASE_FETION	32
#define		OPEN_CASE_POPO		33
#define		OPEN_CASE_UC		34
#define		OPEN_CASE_TROJAN	35
	
#define		OPEN_INDEX_SMTP		0
#define		OPEN_INDEX_POP3		1
#define		OPEN_INDEX_HTTPUP	2
#define		OPEN_INDEX_HTTPDOWN	3
#define		OPEN_INDEX_FTP		4
#define		OPEN_INDEX_TELNET	5
#define		OPEN_INDEX_OICQ		6
#define		OPEN_INDEX_YAHOO	7
#define		OPEN_INDEX_MSN		8
#define		OPEN_INDEX_ICQAOL	9
#define		OPEN_INDEX_MIR		10
#define		OPEN_INDEX_OURGAME	11
#define		OPEN_INDEX_NOG		12
#define		OPEN_INDEX_VOIPG	13
#define		OPEN_INDEX_BF		14
#define		OPEN_INDEX_SOQ		15
#define		OPEN_INDEX_EHTCHAT	16
#define		OPEN_INDEX_VPCHAT	17
#define		OPEN_INDEX_EMULE	18
#define		OPEN_INDEX_VNC		19
#define		OPEN_INDEX_RTSP		20
#define		OPEN_INDEX_HF		21
#define		OPEN_INDEX_FGUP		22
#define		OPEN_INDEX_FGDN		23
#define		OPEN_INDEX_FGURL	24
#define		OPEN_INDEX_DYNAUP	25
#define		OPEN_INDEX_DYNADN	26
#define		OPEN_INDEX_DYNAURL	27
#define		OPEN_INDEX_WUJIE	28
#define		OPEN_INDEX_UNIP		29
#define		OPEN_INDEX_TAOBAO	30
#define		OPEN_INDEX_FETION	31
#define		OPEN_INDEX_POPO		32
#define		OPEN_INDEX_UC		33

#define		OPEN_INDEX_TROJAN	34

//预警数据开关
#define		OPEN_WARN_SMTP		0
#define		OPEN_WARN_POP3		1
#define		OPEN_WARN_HTTPUP	2
#define		OPEN_WARN_HTTPDOWN	3
#define		OPEN_WARN_FTP		4
#define		OPEN_WARN_TELNET	5
#define		OPEN_WARN_OICQ		6
#define		OPEN_WARN_YAHOO		7
#define		OPEN_WARN_MSN		8
#define		OPEN_WARN_ICQAOL	9
#define		OPEN_WARN_MIR		10
#define		OPEN_WARN_OURGAME	11
#define		OPEN_WARN_NOG		12
#define		OPEN_WARN_VOIPG		13
#define		OPEN_WARN_BF		14
#define		OPEN_WARN_SOQ		15
#define		OPEN_WARN_EHTCHAT	16
#define		OPEN_WARN_VPCHAT	17
#define		OPEN_WARN_EMULE		18
#define		OPEN_WARN_VNC		19
#define		OPEN_WARN_RTSP		20
#define		OPEN_WARN_HF		21
#define		OPEN_WARN_FGUP		22
#define		OPEN_WARN_FGDN		23
#define		OPEN_WARN_FGURL		24
#define		OPEN_WARN_DYNAUP	25
#define		OPEN_WARN_DYNADN	26
#define		OPEN_WARN_DYNAURL	27
#define		OPEN_WARN_WUJIE		28
#define		OPEN_WARN_UNIP		29
#define		OPEN_WARN_TAOBAO	30
#define		OPEN_WARN_FETION	31
#define		OPEN_WARN_POPO		32
#define		OPEN_WARN_UC		33

// 	{"REC_TIME",	r4num, 10, 0},	// 记录同步标识(同步时使用)(time_t数据的串)
// 	{"PARAID",		r4str, 6,  0},	// 系统参数编号
// 	{"PARANAME",	r4str, 40, 0},	// 系统参数名称
// 	{"PARAVALUE",	r4str, 100, 0},	// 参数的值,值的具体类型应该根据参数的名称确定
// 	{"REMARK",		r4str, 200, 0},	// 备注
// 	{"SUBS",		r4str, 50, 0},	// 子系统
// 	{"DEVICE",		r4str, 100, 0},	// 设备   local 和 具体设备名
// #define		bcp_system_count	7
// #define		bcp_system_paraid	0
// #define		bcp_system_paraval	2
	int bcp_system_paraid;
	int bcp_system_paraval;
	int bcp_system_count = 7;
	if(v_programtype != SYSTEM_SLEUTH)
	{
		bcp_system_count = 4;
		bcp_system_paraid=0;
		bcp_system_paraval=2;		
	}
	else
	{
		bcp_system_paraid=0;
		bcp_system_paraval=3;		
	}
	int Indexidx, Caseidx, Mediaidx;
	int openindex[60];
	int opencase[60];
	int openwarn[60];
	int openmedia[60];
	int casemedia[60];
	int warnmedia[60];
	for (int n=0; n<60; n++)
	{
		openindex[n] = 1;
		opencase[n] = 1;
		openmedia[n] = 1;
		casemedia[n] = 1;
		openwarn[n] = 1;
		warnmedia[n] = 1;
	}
	opencase[OPEN_CASE_IMAGE] = 0;

	BOOL bReadOK = TRUE;
	CString paraval;
	DWORD paraid, dwparaval;

	CSyncPara synPara;
	DWORD dealPos = 0;
	if(FALSE == synPara.LoadPara(szBcp, dealPos, TRUE))
	{
		return FALSE;
	}
	CStringArray arrField;
	BOOL bGetOne = synPara.GetOneRecord(dealPos, arrField);
	while(bGetOne)
	{
//		ASSERT(arrField.GetSize() == bcp_system_count);
		if(arrField.GetSize() != bcp_system_count)
		{
			bReadOK = FALSE;
			break;
		}
//		CString tmp = arrField[2];
		paraid = atoi(arrField[bcp_system_paraid]);
		paraval = CString(arrField[bcp_system_paraval]);
		dwparaval = atoi(paraval);
		if(paraid == 26101)
		{
			int PrevMod = v_workmode;
			if(strncmp(paraval, "回溯", 4) == 0)
				v_workmode = 3;
			else if(strncmp(paraval, "案件", 4) == 0)
				v_workmode = 2;
			else if(strncmp(paraval, "重点", 4) == 0)
				v_workmode = 1;
			if(PrevMod != v_workmode)
			{
				WriteLog("dce", "截获模式变化, 原来:%s, 改后:%s",
					((PrevMod==1)?	 "重点":((PrevMod==2)   ?"案件":"回溯")),
					((v_workmode==1)?"重点":((v_workmode==2)?"案件":"回溯")) );
				g_parainfo[PARAMTABLECLIMIT].update = 1;	// 重新读CLimit表
			}
		}
		if(paraid == 17102)	
		{
			// IDC管理类统计
			v_statIdc = dwparaval;
		}
		
		if(paraid == 28101)	
		{
			// 音视频轮换时长 (10 - 60分钟)
			int val = dwparaval;
			if (val < 10)
				val = 10;
			if (val > 60)
				val = 60;
			InterlockedExchange(&v_mediainterval, val * 60);		// 换算成秒
		}
		if(paraid == 28104)	// 视频数据的抽样比例
		{
			int val = dwparaval;
			if (val <= 256)
				InterlockedExchange(&v_mediasample, 1);		// 换算成秒
			else
				InterlockedExchange(&v_mediasample, val / 256);		// 换算成秒
		}
		if(paraid >= 19101 && paraid <= 19107)	
		{
			// 网络活动历史
			switch(paraid)
			{
			case 19101:	// Httpup统计开关
				InterlockedExchange((long *)&v_statHttpup, dwparaval); 
				break;
			case 19102:	// 即时消息QQ
				InterlockedExchange((long *)&v_statImsgQQ, dwparaval);
				break;
			case 19103:	// 即时消息其它
				InterlockedExchange((long *)&v_statImsgOther, dwparaval);
				break;
			case 19104:	// 邮件
				InterlockedExchange((long *)&v_statMail,  dwparaval); 
				break;
			case 19105:	// 音视频
				InterlockedExchange((long *)&v_statMedia, dwparaval);
				break;
			case 19106:	// 游戏
				InterlockedExchange((long *)&v_statGame,  dwparaval);
				break;
			case 19107:	// 其它(除以上)
				InterlockedExchange((long *)&v_statOther, dwparaval);
				break;
			}
		}

		if(paraid >= 11101 && paraid <= 11134)
		{
			Indexidx = paraid - 11101;
			openindex[Indexidx] = dwparaval;	
		}

		if(paraid >= 15101 && paraid <= 15134)
		{
			Indexidx = paraid - 15101;
			openwarn[Indexidx] = dwparaval;
		}

		if(paraid >= 13101 && paraid <= 13135)
		{
			Caseidx = paraid - 13101;
			opencase[Caseidx] = dwparaval;
		}

		if(paraid >= 11161 && paraid <= 11175)
		{
			Mediaidx = paraid - 11161;
			openmedia[Mediaidx] = dwparaval;
		}

		if(paraid >= 13161 && paraid <= 13175)
		{
			Mediaidx = paraid - 13161;
			casemedia[Mediaidx] = dwparaval;
		}
		
		if(paraid >= 15161 && paraid <= 15175)
		{
			Mediaidx = paraid - 15161;
			warnmedia[Mediaidx] = dwparaval;
		}

		arrField.RemoveAll();
		bGetOne = synPara.GetOneRecord(dealPos, arrField);
	}
	if(v_workmode != 3)
	{
		// 全文类
		InterlockedExchange((long *)&v_openflagindeximage, 0);
		InterlockedExchange((long *)&v_openflagsmtp,	0);
		InterlockedExchange((long *)&v_openflagpop3,	0);
		InterlockedExchange((long *)&v_openflaghttpup,	0);
		InterlockedExchange((long *)&v_openflaghttpdown,0);
		InterlockedExchange((long *)&v_openflagftp,		0);
		InterlockedExchange((long *)&v_openflagtelnet,	0);
		InterlockedExchange((long *)&v_openflagoicq,	0);
		InterlockedExchange((long *)&v_openflagyahoo,	0);
		InterlockedExchange((long *)&v_openflagmsn,		0);
		InterlockedExchange((long *)&v_openflagaol,		0);
		InterlockedExchange((long *)&v_openflagchuanqi, 0);
		InterlockedExchange((long *)&v_openflaglzgame,	0);
		InterlockedExchange((long *)&v_openflagnog,		0);
		InterlockedExchange((long *)&v_openflagvgate,	0);
		InterlockedExchange((long *)&v_openflagbf,		0);
		InterlockedExchange((long *)&v_openflagSoQ,		0);
		InterlockedExchange((long *)&v_openflagEht,		0);
		InterlockedExchange((long *)&v_openflagVp,		0);
		InterlockedExchange((long *)&v_openflagemule,	0);
		InterlockedExchange((long *)&v_openflagvnc,		0);
		InterlockedExchange((long *)&v_openflagrtsp,	0);
		InterlockedExchange((long *)&v_openflaghf,		0);
		InterlockedExchange((long *)&v_openflagtrojan,	0);

		// 全文 NOG
		InterlockedExchange((long *)&v_openflagfgup,	0);
		InterlockedExchange((long *)&v_openflagfgdn,	0);
		InterlockedExchange((long *)&v_openflagfgurl,	0);
		InterlockedExchange((long *)&v_openflagdynaup,	0);
		InterlockedExchange((long *)&v_openflagdynadn,	0);
		InterlockedExchange((long *)&v_openflagdynaurl,	0);
		InterlockedExchange((long *)&v_openflagwujie,	0);
		InterlockedExchange((long *)&v_openflagunip,	0);
		
		// 全文视频类
		InterlockedExchange((long *)&v_openflagQQMedia,		0);
		InterlockedExchange((long *)&v_openflagBLMedia,		0);
		InterlockedExchange((long *)&v_openflagEPHMedia,	0);
		InterlockedExchange((long *)&v_openflagMSNMedia,	0);
		InterlockedExchange((long *)&v_openflagPoPoMedia,	0);
		InterlockedExchange((long *)&v_openflagFlashMedia,	0);
		InterlockedExchange((long *)&v_openflagSoQMedia,	0);
		InterlockedExchange((long *)&v_openflagUCMedia,		0);
		InterlockedExchange((long *)&v_openflagYahooMedia,	0);
		InterlockedExchange((long *)&v_openflagNetMetMedia,	0);
		InterlockedExchange((long *)&v_openflagVoip,		0);
		InterlockedExchange((long *)&v_openflagBDCMedia,	0);
		InterlockedExchange((long *)&v_openflagASFMedia,	0);
		InterlockedExchange((long *)&v_openflagOYesCamMedia,0);
		InterlockedExchange((long *)&v_openflagVPMedia,		0);
		InterlockedExchange((long *)&v_openUUTongMedia,		0);


		// 预警类
		InterlockedExchange((long *)&v_openwarnindeximage, 0);
		InterlockedExchange((long *)&v_openwarnsmtp,	0);
		InterlockedExchange((long *)&v_openwarnpop3,	0);
		InterlockedExchange((long *)&v_openwarnhttpup,	0);
		InterlockedExchange((long *)&v_openwarnhttpdown,0);
		InterlockedExchange((long *)&v_openwarnftp,		0);
		InterlockedExchange((long *)&v_openwarntelnet,	0);
		InterlockedExchange((long *)&v_openwarnoicq,	0);
		InterlockedExchange((long *)&v_openwarnyahoo,	0);
		InterlockedExchange((long *)&v_openwarnmsn,		0);
		InterlockedExchange((long *)&v_openwarnaol,		0);
		InterlockedExchange((long *)&v_openwarnchuanqi, 0);
		InterlockedExchange((long *)&v_openwarnlzgame,	0);
		InterlockedExchange((long *)&v_openwarnnog,		0);
		InterlockedExchange((long *)&v_openwarnvgate,	0);
		InterlockedExchange((long *)&v_openwarnbf,		0);
		InterlockedExchange((long *)&v_openwarnSoQ,		0);
		InterlockedExchange((long *)&v_openwarnEht,		0);
		InterlockedExchange((long *)&v_openwarnVp,		0);
		InterlockedExchange((long *)&v_openwarnemule,	0);
		InterlockedExchange((long *)&v_openwarnvnc,		0);
		InterlockedExchange((long *)&v_openwarnrtsp,	0);
		InterlockedExchange((long *)&v_openwarnhf,		0);
		InterlockedExchange((long *)&v_openwarntaobao,	0);
		InterlockedExchange((long *)&v_openwarnfetion,	0);
		InterlockedExchange((long *)&v_openwarnpopo,	0);
		InterlockedExchange((long *)&v_openwarnuc,		0);

		InterlockedExchange((long *)&v_openwarntrojan,	0);

		// 预警NOG
		InterlockedExchange((long *)&v_openwarnfgup,	0);
		InterlockedExchange((long *)&v_openwarnfgdn,	0);
		InterlockedExchange((long *)&v_openwarnfgurl,	0);
		InterlockedExchange((long *)&v_openwarndynaup,	0);
		InterlockedExchange((long *)&v_openwarndynadn,	0);
		InterlockedExchange((long *)&v_openwarndynaurl,	0);
		InterlockedExchange((long *)&v_openwarnwujie,	0);
		InterlockedExchange((long *)&v_openwarnunip,	0);
		
		// 预警视频类
		InterlockedExchange((long *)&v_openwarnQQMedia,		0);
		InterlockedExchange((long *)&v_openwarnBLMedia,		0);
		InterlockedExchange((long *)&v_openwarnEPHMedia,	0);
		InterlockedExchange((long *)&v_openwarnMSNMedia,	0);
		InterlockedExchange((long *)&v_openwarnPoPoMedia,	0);
		InterlockedExchange((long *)&v_openwarnFlashMedia,	0);
		InterlockedExchange((long *)&v_openwarnSoQMedia,	0);
		InterlockedExchange((long *)&v_openwarnUCMedia,		0);
		InterlockedExchange((long *)&v_openwarnYahooMedia,	0);
		InterlockedExchange((long *)&v_openwarnNetMetMedia,	0);
		InterlockedExchange((long *)&v_openwarnVoip,		0);
		InterlockedExchange((long *)&v_openwarnBDCMedia,	0);
		InterlockedExchange((long *)&v_openwarnASFMedia,	0);
		InterlockedExchange((long *)&v_openwarnOYesCamMedia,0);
		InterlockedExchange((long *)&v_openwarnVPMedia,		0);
		InterlockedExchange((long *)&v_openwarnUUTongMedia,		0);

	}
	else
	{
		//if(v_openflagsmtp != openindex[OPEN_INDEX_SMTP])
		//{
		//	WriteLog("dce", "Smtp预警开关改变:%s", (openindex[OPEN_INDEX_SMTP])?"打开":"关闭");
		//	InterlockedExchange((long *)&v_openflagsmtp, openindex[OPEN_INDEX_SMTP]);
		//}
		//if(v_openflagpop3 != openindex[OPEN_INDEX_POP3])
		//{
		//	WriteLog("dce", "Smtp预警开关改变:%s", (openindex[OPEN_INDEX_POP3])?"打开":"关闭");
		//	InterlockedExchange((long *)&v_openflagsmtp, openindex[OPEN_INDEX_POP3]);
		//}
		// 预警类开关
		InterlockedExchange((long *)&v_openflagindeximage, 0);
		InterlockedExchange((long *)&v_openflagsmtp,	openindex[OPEN_INDEX_SMTP]);
		InterlockedExchange((long *)&v_openflagpop3,	openindex[OPEN_INDEX_POP3]);
		InterlockedExchange((long *)&v_openflaghttpup,	openindex[OPEN_INDEX_HTTPUP]);
		InterlockedExchange((long *)&v_openflaghttpdown,openindex[OPEN_INDEX_HTTPDOWN]);
		InterlockedExchange((long *)&v_openflagftp,		openindex[OPEN_INDEX_FTP]);
		InterlockedExchange((long *)&v_openflagtelnet,	openindex[OPEN_INDEX_TELNET]);
		InterlockedExchange((long *)&v_openflagoicq,	openindex[OPEN_INDEX_OICQ]);
		InterlockedExchange((long *)&v_openflagyahoo,	openindex[OPEN_INDEX_YAHOO]);
		InterlockedExchange((long *)&v_openflagmsn,		openindex[OPEN_INDEX_MSN]);
		InterlockedExchange((long *)&v_openflagaol,		openindex[OPEN_INDEX_ICQAOL]);
		InterlockedExchange((long *)&v_openflagchuanqi, openindex[OPEN_INDEX_MIR]);
		InterlockedExchange((long *)&v_openflaglzgame,	openindex[OPEN_INDEX_OURGAME]);
		InterlockedExchange((long *)&v_openflagnog,		openindex[OPEN_INDEX_NOG]);
		InterlockedExchange((long *)&v_openflagvgate,	openindex[OPEN_INDEX_VOIPG]);
		InterlockedExchange((long *)&v_openflagbf,		openindex[OPEN_INDEX_BF]);
		InterlockedExchange((long *)&v_openflagSoQ,		openindex[OPEN_INDEX_SOQ]);
		InterlockedExchange((long *)&v_openflagEht,		openindex[OPEN_INDEX_EHTCHAT]);
		InterlockedExchange((long *)&v_openflagVp,		openindex[OPEN_INDEX_VPCHAT]);
		InterlockedExchange((long *)&v_openflagemule,	openindex[OPEN_INDEX_EMULE]);
		InterlockedExchange((long *)&v_openflagvnc,		openindex[OPEN_INDEX_VNC]);
		InterlockedExchange((long *)&v_openflagrtsp,	openindex[OPEN_INDEX_RTSP]);
		InterlockedExchange((long *)&v_openflaghf,		openindex[OPEN_INDEX_HF]);
		InterlockedExchange((long *)&v_openflagunip,	openindex[OPEN_INDEX_UNIP]);

		InterlockedExchange((long *)&v_openflagtrojan,	openindex[OPEN_INDEX_TROJAN]);


		// 全文NOG
		InterlockedExchange((long *)&v_openflagfgup,	openindex[OPEN_INDEX_FGUP]);		
		InterlockedExchange((long *)&v_openflagfgdn,	openindex[OPEN_INDEX_FGDN]);		
		InterlockedExchange((long *)&v_openflagfgurl,	openindex[OPEN_INDEX_FGURL]);	
		InterlockedExchange((long *)&v_openflagdynaup,	openindex[OPEN_INDEX_DYNAUP]);
		InterlockedExchange((long *)&v_openflagdynadn,	openindex[OPEN_INDEX_DYNADN]);	
		InterlockedExchange((long *)&v_openflagdynaurl,	openindex[OPEN_INDEX_DYNAURL]);	
		InterlockedExchange((long *)&v_openflagwujie,	openindex[OPEN_INDEX_WUJIE]);	

		// 预警视频类开关
		InterlockedExchange((long *)&v_openflagQQMedia,		openmedia[OPEN_MEDIA_QQ]);
		InterlockedExchange((long *)&v_openflagBLMedia,		openmedia[OPEN_MEDIA_BLUESKY]);
		InterlockedExchange((long *)&v_openflagEPHMedia,	openmedia[OPEN_MEDIA_EHT]);
		InterlockedExchange((long *)&v_openflagMSNMedia,	openmedia[OPEN_MEDIA_MSN]);
		InterlockedExchange((long *)&v_openflagPoPoMedia,	openmedia[OPEN_MEDIA_POPO]);
		InterlockedExchange((long *)&v_openflagFlashMedia,	openmedia[OPEN_MEDIA_FLASH]);
		InterlockedExchange((long *)&v_openflagSoQMedia,	openmedia[OPEN_MEDIA_SOQ]);
		InterlockedExchange((long *)&v_openflagUCMedia,		openmedia[OPEN_MEDIA_UC]);
		InterlockedExchange((long *)&v_openflagYahooMedia,	openmedia[OPEN_MEDIA_YAHOO]);
		InterlockedExchange((long *)&v_openflagNetMetMedia,	openmedia[OPEN_MEDIA_NETM]);
		InterlockedExchange((long *)&v_openflagVoip,		openmedia[OPEN_MEDIA_VOIP]);
		InterlockedExchange((long *)&v_openflagBDCMedia,	openmedia[OPEN_MEDIA_BDC]);
		InterlockedExchange((long *)&v_openflagASFMedia,	openmedia[OPEN_MEDIA_ASF]);
	//	InterlockedExchange((long *)&v_openflagOYesCamMedia,openmedia[OPEN_MEDIA_OYESCAM]);
		InterlockedExchange((long *)&v_openflagVPMedia,		openmedia[OPEN_MEDIA_VP]);
		InterlockedExchange((long *)&v_openUUTongMedia,		openmedia[OPEN_MEDIA_UUTong]);

		// 预警类开关
		InterlockedExchange((long *)&v_openwarnindeximage, 0);
		InterlockedExchange((long *)&v_openwarnsmtp,	openwarn[OPEN_INDEX_SMTP]);
		InterlockedExchange((long *)&v_openwarnpop3,	openwarn[OPEN_INDEX_POP3]);
		InterlockedExchange((long *)&v_openwarnhttpup,	openwarn[OPEN_INDEX_HTTPUP]);
		InterlockedExchange((long *)&v_openwarnhttpdown,openwarn[OPEN_INDEX_HTTPDOWN]);
		InterlockedExchange((long *)&v_openwarnftp,		openwarn[OPEN_INDEX_FTP]);
		InterlockedExchange((long *)&v_openwarntelnet,	openwarn[OPEN_INDEX_TELNET]);
		InterlockedExchange((long *)&v_openwarnoicq,	openwarn[OPEN_INDEX_OICQ]);
		InterlockedExchange((long *)&v_openwarnyahoo,	openwarn[OPEN_INDEX_YAHOO]);
		InterlockedExchange((long *)&v_openwarnmsn,		openwarn[OPEN_INDEX_MSN]);
		InterlockedExchange((long *)&v_openwarnaol,		openwarn[OPEN_INDEX_ICQAOL]);
		InterlockedExchange((long *)&v_openwarnchuanqi, openwarn[OPEN_INDEX_MIR]);
		InterlockedExchange((long *)&v_openwarnlzgame,	openwarn[OPEN_INDEX_OURGAME]);
		InterlockedExchange((long *)&v_openwarnnog,		openwarn[OPEN_INDEX_NOG]);
		InterlockedExchange((long *)&v_openwarnvgate,	openwarn[OPEN_INDEX_VOIPG]);
		InterlockedExchange((long *)&v_openwarnbf,		openwarn[OPEN_INDEX_BF]);
		InterlockedExchange((long *)&v_openwarnSoQ,		openwarn[OPEN_INDEX_SOQ]);
		InterlockedExchange((long *)&v_openwarnEht,		openwarn[OPEN_INDEX_EHTCHAT]);
		InterlockedExchange((long *)&v_openwarnVp,		openwarn[OPEN_INDEX_VPCHAT]);
		InterlockedExchange((long *)&v_openwarnemule,	openwarn[OPEN_INDEX_EMULE]);
		InterlockedExchange((long *)&v_openwarnvnc,		openwarn[OPEN_INDEX_VNC]);
		InterlockedExchange((long *)&v_openwarnrtsp,	openwarn[OPEN_INDEX_RTSP]);
		InterlockedExchange((long *)&v_openwarnhf,		openwarn[OPEN_INDEX_HF]);
		InterlockedExchange((long *)&v_openwarnunip,	openwarn[OPEN_INDEX_UNIP]);
		InterlockedExchange((long *)&v_openwarntaobao,	openwarn[OPEN_INDEX_TAOBAO]);
		InterlockedExchange((long *)&v_openwarnfetion,	openwarn[OPEN_INDEX_FETION]);
		InterlockedExchange((long *)&v_openwarnpopo,	openwarn[OPEN_INDEX_POPO]);
		InterlockedExchange((long *)&v_openwarnuc,		openwarn[OPEN_INDEX_UC]);
		InterlockedExchange((long *)&v_openwarntrojan,	openwarn[OPEN_INDEX_TROJAN]);

		// 全文NOG
		InterlockedExchange((long *)&v_openwarnfgup,	openwarn[OPEN_INDEX_FGUP]);		
		InterlockedExchange((long *)&v_openwarnfgdn,	openwarn[OPEN_INDEX_FGDN]);		
		InterlockedExchange((long *)&v_openwarnfgurl,	openwarn[OPEN_INDEX_FGURL]);	
		InterlockedExchange((long *)&v_openwarndynaup,	openwarn[OPEN_INDEX_DYNAUP]);
		InterlockedExchange((long *)&v_openwarndynadn,	openwarn[OPEN_INDEX_DYNADN]);	
		InterlockedExchange((long *)&v_openwarndynaurl,	openwarn[OPEN_INDEX_DYNAURL]);	
		InterlockedExchange((long *)&v_openwarnwujie,	openwarn[OPEN_INDEX_WUJIE]);	
		
		// 预警视频类开关
		InterlockedExchange((long *)&v_openwarnQQMedia,		warnmedia[OPEN_MEDIA_QQ]);
		InterlockedExchange((long *)&v_openwarnBLMedia,		warnmedia[OPEN_MEDIA_BLUESKY]);
		InterlockedExchange((long *)&v_openwarnEPHMedia,	warnmedia[OPEN_MEDIA_EHT]);
		InterlockedExchange((long *)&v_openwarnMSNMedia,	warnmedia[OPEN_MEDIA_MSN]);
		InterlockedExchange((long *)&v_openwarnPoPoMedia,	warnmedia[OPEN_MEDIA_POPO]);
		InterlockedExchange((long *)&v_openwarnFlashMedia,	warnmedia[OPEN_MEDIA_FLASH]);
		InterlockedExchange((long *)&v_openwarnSoQMedia,	warnmedia[OPEN_MEDIA_SOQ]);
		InterlockedExchange((long *)&v_openwarnUCMedia,		warnmedia[OPEN_MEDIA_UC]);
		InterlockedExchange((long *)&v_openwarnYahooMedia,	warnmedia[OPEN_MEDIA_YAHOO]);
		InterlockedExchange((long *)&v_openwarnNetMetMedia,	warnmedia[OPEN_MEDIA_NETM]);
		InterlockedExchange((long *)&v_openwarnVoip,		warnmedia[OPEN_MEDIA_VOIP]);
		InterlockedExchange((long *)&v_openwarnBDCMedia,	warnmedia[OPEN_MEDIA_BDC]);
		InterlockedExchange((long *)&v_openwarnASFMedia,	warnmedia[OPEN_MEDIA_ASF]);
		//	InterlockedExchange((long *)&v_openwarnOYesCamMedia,warnmedia[OPEN_MEDIA_OYESCAM]);
		InterlockedExchange((long *)&v_openwarnVPMedia,		warnmedia[OPEN_MEDIA_VP]);
		InterlockedExchange((long *)&v_openwarnUUTongMedia,	warnmedia[OPEN_MEDIA_UUTong]);
	}
	
	// 案件类开关
	InterlockedExchange((long *)&v_opencasesmtp,		opencase[OPEN_CASE_SMTP]);
	InterlockedExchange((long *)&v_opencasepop3,		opencase[OPEN_CASE_POP3]);
	InterlockedExchange((long *)&v_opencasehttpup,		opencase[OPEN_CASE_HTTPUP]);
	InterlockedExchange((long *)&v_opencasehttpdown,	opencase[OPEN_CASE_HTTPDOWN]);
	InterlockedExchange((long *)&v_opencaseftp,			opencase[OPEN_CASE_FTP]);
	InterlockedExchange((long *)&v_opencasetelnet,		opencase[OPEN_CASE_TELNET]);
	InterlockedExchange((long *)&v_opencaseoicq,		opencase[OPEN_CASE_OICQ]);
	InterlockedExchange((long *)&v_opencaseyahoo,		opencase[OPEN_CASE_YAHOO]);
	InterlockedExchange((long *)&v_opencasemsn,			opencase[OPEN_CASE_MSN]);
	InterlockedExchange((long *)&v_opencaseaol,			opencase[OPEN_CASE_ICQAOL]);
	InterlockedExchange((long *)&v_opencasechuanqi,		opencase[OPEN_CASE_MIR]);
	InterlockedExchange((long *)&v_opencaselzgame,		opencase[OPEN_CASE_OURGAME]);
	InterlockedExchange((long *)&v_opencasenog,			opencase[OPEN_CASE_NOG]);
	InterlockedExchange((long *)&v_opencasevgate,		opencase[OPEN_CASE_VOIPG]);
	InterlockedExchange((long *)&v_opencasebf,			opencase[OPEN_CASE_BF]);
	InterlockedExchange((long *)&v_opencaseSoQ,			opencase[OPEN_CASE_SOQ]);
	InterlockedExchange((long *)&v_opencaseEht,			opencase[OPEN_CASE_EHTCHAT]);
	InterlockedExchange((long *)&v_opencaseVp,			opencase[OPEN_CASE_VPCHAT]);
	InterlockedExchange((long *)&v_opencasehttpimage,	opencase[OPEN_CASE_IMAGE]);
	InterlockedExchange((long *)&v_opencaseemule,		opencase[OPEN_CASE_EMULE]);
	InterlockedExchange((long *)&v_opencasevnc,			opencase[OPEN_CASE_VNC]);
	InterlockedExchange((long *)&v_opencasertsp,		opencase[OPEN_CASE_RTSP]);
	InterlockedExchange((long *)&v_opencasehf,			opencase[OPEN_CASE_HF]);
	InterlockedExchange((long *)&v_opencaseunip,		opencase[OPEN_CASE_UNIP]);
	InterlockedExchange((long *)&v_opencasetaobao,		opencase[OPEN_CASE_TAOBAO]);
	InterlockedExchange((long *)&v_opencasefetion,		opencase[OPEN_CASE_FETION]);
	InterlockedExchange((long *)&v_opencasepopo,		opencase[OPEN_CASE_POPO]);
	InterlockedExchange((long *)&v_opencaseuc,			opencase[OPEN_CASE_UC]);

	InterlockedExchange((long *)&v_opencasetrojan,		opencase[OPEN_CASE_TROJAN]);

	// 案件 NOG
	InterlockedExchange((long *)&v_opencasefgup,		opencase[OPEN_CASE_FGUP]);		
	InterlockedExchange((long *)&v_opencasefgdn,		opencase[OPEN_CASE_FGDN]);		
	InterlockedExchange((long *)&v_opencasefgurl,		opencase[OPEN_CASE_FGURL]);	
	InterlockedExchange((long *)&v_opencasedynaup,		opencase[OPEN_CASE_DYNAUP]);
	InterlockedExchange((long *)&v_opencasedynadn,		opencase[OPEN_CASE_DYNADN]);	
	InterlockedExchange((long *)&v_opencasedynaurl,		opencase[OPEN_CASE_DYNAURL]);	
	InterlockedExchange((long *)&v_opencasewujie,		opencase[OPEN_CASE_WUJIE]);	

	// 案件视频类开关
	InterlockedExchange((long *)&v_opencaseQQMedia,		casemedia[OPEN_MEDIA_QQ]);
	InterlockedExchange((long *)&v_opencaseBLMedia,		casemedia[OPEN_MEDIA_BLUESKY]);
	InterlockedExchange((long *)&v_opencaseEPHMedia,	casemedia[OPEN_MEDIA_EHT]);
	InterlockedExchange((long *)&v_opencaseMSNMedia,	casemedia[OPEN_MEDIA_MSN]);
	InterlockedExchange((long *)&v_opencasePoPoMedia,	casemedia[OPEN_MEDIA_POPO]);
	InterlockedExchange((long *)&v_opencaseFlashMedia,	casemedia[OPEN_MEDIA_FLASH]);
	InterlockedExchange((long *)&v_opencaseSoQMedia,	casemedia[OPEN_MEDIA_SOQ]);
	InterlockedExchange((long *)&v_opencaseUCMedia,		casemedia[OPEN_MEDIA_UC]);
	InterlockedExchange((long *)&v_opencaseYahooMedia,	casemedia[OPEN_MEDIA_YAHOO]);
	InterlockedExchange((long *)&v_opencaseNetMetMedia,	casemedia[OPEN_MEDIA_NETM]);
	InterlockedExchange((long *)&v_opencaseVoip,		casemedia[OPEN_MEDIA_VOIP]);
	InterlockedExchange((long *)&v_opencaseBDCMedia,	casemedia[OPEN_MEDIA_BDC]);
	InterlockedExchange((long *)&v_opencaseASFMedia,	casemedia[OPEN_MEDIA_ASF]);
//	InterlockedExchange((long *)&v_opencaseOYesCamMedia,casemedia[OPEN_MEDIA_OYESCAM]);
	InterlockedExchange((long *)&v_opencaseVPMedia,		casemedia[OPEN_MEDIA_VP]);
	InterlockedExchange((long *)&v_opencaseUUTongMedia,	casemedia[OPEN_MEDIA_UUTong]);

	return bReadOK;
}
BOOL FormatIp(CString ip,char* returnip)
{
	int num;
	int ipc1, ipc2, ipc3, ipc4; 

	// 构造IP单元对象
	num = sscanf(ip,"%d.%d.%d.%d",&ipc1,&ipc2,&ipc3,&ipc4);
	if ((num != 4) || (ipc1 < 0) || (ipc1 > 255) ||
		(ipc2 < 0) || (ipc2 > 255) || (ipc3 < 0) ||
		(ipc3 > 255) || (ipc4 < 0) || (ipc4 > 255))
		return FALSE;

	sprintf(returnip,"%03d.%03d.%03d.%03d",ipc1,ipc2,ipc3,ipc4);
	return TRUE;
}

BOOL UpdateLocalIpAddr(LPCTSTR filename)
{
	k_IpQueryTable.LoadLocalIPFromDbf(filename);
	return TRUE;
}

BOOL UpdateAllIpAddr(LPCTSTR filename)
{
	k_IpQueryTable.LoadAllIPFromDbf(filename);
	return TRUE;
}
BOOL UpdateyysIpAddr(LPCTSTR filename)
{
	k_IpQueryTable.LoadyysIPFromDbf(filename);
	return TRUE;
}
BOOL UpdateMoniterWeb(LPCTSTR filename)
{
	g_moniterweb.UpdateMap();
	return true;
}

BOOL UpdateIndexKeyWord(LPCTSTR filename)
{
	#define  bcp_keyword_count 5
	if (_access(filename,0) != 0)
	{
		WriteLog("dce","文件 %s 不存在!");
		return TRUE;
	}
	CString importsql;

int error =0;	
// 	Code4	table_code;
// 	Data4	table_data;
	ASSERT(v_IndexLimitPara.m_keywordreadflg==0 || v_IndexLimitPara.m_keywordreadflg==1);
	long	bufid = (v_IndexLimitPara.m_keywordreadflg==0)? 1 : 0;

// 	ZWSQL_RES *result;
// 	ZWSQL zwsql; //数据库连接句柄

	BOOL   bhavekeyword = FALSE;
	try
	{
error =1;
		// 清空原缓冲区
		v_IndexLimitPara.m_keywordlist[bufid].clear();
		v_IndexLimitPara.m_keyworddic[bufid].ClearData();
error =2;
		memset(v_limitcount[0]+LIMITTYPE_HTTPWORD, 0, (LIMITTYPE_INDEXIPURL-LIMITTYPE_HTTPWORD)*4);
		BOOL bReadOK = TRUE;
		CSyncPara synPara;
		DWORD dealPos = 0;
		if(FALSE == synPara.LoadPara(filename, dealPos, TRUE))
		{
			return TRUE;
		}
		CStringArray arrField;
		BOOL bGetOne;

		LIMITDATA buf;
		buf.AlarmState = 0;
		CString   str, strtmp, strifkz;
		CString   strcaseid, strcaseobjtype, strcaseobjname, straddrtype, straddrname;
		BOOL      bhavekeyword = FALSE;
		BOOL	  bhavejzkeyword = FALSE;
		int ii=0;
error =5;
		CMapStringToPtr		filter_map;
		filter_map.InitHashTable(1123);
		void *p = 0;
//		遍历数据库
// 		table_data.select();
// 		int reccount = table_data.recCount();
		SYSTEMTIME nowtime;
		CString strtime;
		GetLocalTime(&nowtime);
		strtime.Format("%04d-%02d-%02d",nowtime.wYear, nowtime.wMonth, nowtime.wDay);
		CString begtime, endtime, keyword;
		v_limitcount[0][LIMITTYPE_HTTPWORD]=0;
error =6;
	do{
			arrField.RemoveAll();
			bGetOne = synPara.GetOneRecord(dealPos, arrField);
			if(FALSE == bGetOne)
				break;
//			ASSERT(arrField.GetSize() == bcp_keyword_count);
// 			if(arrField.GetSize() != bcp_keyword_count)
// 			{
// 				bReadOK = FALSE;
// 				break;
// 			}
			// 启用状态
//			begtime = arrField[3];
//			endtime = arrField[4];
//			begtime.TrimRight();
//			endtime.TrimRight();
			if(arrField.GetSize() < 3)
				continue;

//			if (strcmp(strtime,begtime)>=0 && strcmp(endtime,strtime)>=0)
			{
				keyword = arrField[2];
				keyword.TrimRight();
error =7;
				keyword.TrimRight();
				if (!keyword.IsEmpty())
				{
					if (!filter_map.Lookup(keyword, p))
					{
						filter_map.SetAt(keyword, p);
//							v_IndexLimitPara.m_keywordlist[bufid].push_back((LPCTSTR)keyword);
						Vocable temp;// = new Vocable;
						temp.m_Vocable.Format("%s",keyword);
error =8;
						AllDicInfo* lpalldicinfo = new AllDicInfo;
						temp.m_pUserData = (VOID*)lpalldicinfo;
//							temp.Vhandle=2;
						v_IndexLimitPara.m_keyworddic[bufid].m_VocableList.push_front(temp);
						
						v_limitcount[0][LIMITTYPE_HTTPWORD]++;

						bhavekeyword = TRUE;
					}
				}
error =9;
// 				keyword = arrField[1];
// 				keyword.TrimRight();
// //					crypt((char *)(LPCTSTR)keyword,keyword.GetLength());					
// 				if (!keyword.IsEmpty())
// 				{
// error =10;
// 					if (!filter_map.Lookup(keyword, p))
// 					{
// 						filter_map.SetAt(keyword, p);
// //							v_IndexLimitPara.m_keywordlist[bufid].push_back((LPCTSTR)keyword);
// 						Vocable temp;
// 						temp.m_Vocable.Format("%s",keyword);
// 						AllDicInfo* lpalldicinfo = new AllDicInfo;
// 						temp.m_pUserData = (VOID*)lpalldicinfo;
// 
// //							temp.Vhandle=2;
// 						v_IndexLimitPara.m_keyworddic[bufid].m_VocableList.push_front(temp);
// 						
// 						v_limitcount[0][LIMITTYPE_HTTPWORD]++;
// 
// 						bhavekeyword = TRUE;
// 					}
// 				}
			}
		}while(bGetOne);
error =11;
		if(bhavekeyword)
		{
			v_IndexLimitPara.m_keyworddic[bufid].m_VocableList.sort();
			v_IndexLimitPara.m_keyworddic[bufid].IniKeyBTree(200000);
		}
		InterlockedExchange(&v_IndexLimitPara.m_keywordreadflg, bufid);
		return TRUE;
	}
	catch(...)
	{

// 		table_code.closeAll();
// 		table_code.initUndo();
		WriteLog("dce", "UpdateIndexKeyWord() Error!! =%d",error);
		CopyFile(filename,"d:\\netspy\\log\\keyword.bcp",false);
	}
	return FALSE;
}
BOOL UpdateHttpupSplit(LPCTSTR filename)
{
	memset(v_limitcount[0]+HTTPUPSPLIT, 0, 4);
	long	bufid = (v_CustUrlAccID==0)? 1 : 0;
	v_CustUrlList[bufid].clear();
	
	BOOL bReadOK = TRUE;
	CSyncPara synPara;
	DWORD dealPos = 0;
	if(FALSE == synPara.LoadPara(filename, dealPos, TRUE))
	{
		return TRUE;
	}
	CStringArray arrField;
	BOOL bGetOne;
	BOOL bInvalid;
	int cntRecord = 0;
	CString strV, strUrl, strSplit, strDesc, strQqinfo;
	int InfoType, FieldType;

	do{

		arrField.RemoveAll();
		bGetOne = synPara.GetOneRecord(dealPos, arrField);
		if(FALSE == bGetOne)
			break;
// 			ASSERT(arrField.GetSize() == bcp_keyword_count);
// 			if(arrField.GetSize() != bcp_keyword_count)
// 			{
// 				bReadOK = FALSE;
// 				break;
// 			}
		if(arrField.GetSize() == 6)
		{
			/// Url SplitId SplitPass ServiceName Mark
			strUrl = arrField[1];
			strUrl.TrimRight();
			strUrl.TrimLeft("http: ");
			strUrl.TrimLeft("/");		// 去掉http:// 头, 如果有的话

			strDesc = arrField[0];
			//strDesc += "用户名";
			CustomUrlField tmp(1, strDesc, strUrl, arrField[2], arrField[3]); // 1, 1 表示, 登陆,ID
			v_CustUrlList[bufid].push_back(tmp);
// 				}
// 				if(false == arrField[2].IsEmpty())
// 				{
// 					cntRecord ++;
// 					strDesc = arrField[3];
// 					//strDesc += "密码";
// 					CustomUrlField tmp(1, 2, arrField[2], strDesc, strUrl);	// 1, 2 表示, 登陆,密码
// 					v_CustUrlList[bufid].push_back(tmp);
// 				}
		}
		else
		{
			bInvalid = TRUE;
			strV = arrField[0];
			strV.TrimLeft();
			strV.TrimRight();
			if(strV.CompareNoCase("登录") == 0)
				InfoType = 1;
			else if(strV.CompareNoCase("注册") == 0)
				InfoType = 2;
			else if(strV.CompareNoCase("腾讯QQ登录") == 0 ||
					strV.CompareNoCase("腾讯QQ登陆") == 0 ||
					strV.CompareNoCase("腾讯QQ") == 0)
				InfoType = 3;
			else
			{
				WriteLog("dce", "Up拆解发现不可识别信息类型:%s", strV);
				bInvalid = FALSE;
			}
			strV = arrField[3];
			strV.TrimLeft();
			strV.TrimRight();
			if(strV.CompareNoCase("id") == 0)
				FieldType = 1;
			else if(strV.CompareNoCase("密码") == 0)
				FieldType = 2;
			else if(strV.CompareNoCase("其他") == 0)
				FieldType = 3;
			else
			{
				bInvalid = FALSE;
				WriteLog("dce", "Up拆解发现不可识别字段类型:%s", strV);
			}
			if(bInvalid)
			{
				cntRecord ++;

				strUrl = arrField[1];;
				strUrl.TrimRight();
				strUrl.TrimLeft("http: ");
				strUrl.TrimLeft("/");		// 去掉http:// 头, 如果有的话
				
				strSplit = arrField[2];
				strSplit.TrimRight();
				strSplit.TrimLeft();
				strSplit.TrimRight("= ");	// 去掉拆解符后的=号, 如果有的话
				
				strDesc = arrField[4];
				strDesc.TrimLeft();
				strDesc.TrimRight();
				CustomUrlField tmp(InfoType, FieldType, strSplit, strDesc, strUrl);
				v_CustUrlList[bufid].push_back(tmp);
			}
		}
	}while(bGetOne);

	CustomUrlField tmp(1, "imwqqcrcak", "www.baidu.com/aajjkk/qq", "a1", "a2"); // 1, 1 表示, 登陆,ID
	v_CustUrlList[bufid].push_back(tmp);

	InterlockedExchange(&v_CustUrlAccID, bufid);
	v_limitcount[0][HTTPUPSPLIT] = cntRecord;
	return TRUE;
}
BOOL Updatehttpregist(LPCTSTR filename)
{
	memset(v_limitcount[0]+HTTPUPSPLIT, 0, 4);
	long	bufid = (v_HttpregistAccID==0)? 1 : 0;
	v_HttpregistList[bufid].clear();
	
	BOOL bReadOK = TRUE;
	CSyncPara synPara;
	DWORD dealPos = 0;
	if(FALSE == synPara.LoadPara(filename, dealPos, TRUE))
	{
		return TRUE;
	}
	CStringArray arrField;
	BOOL bGetOne;
	int cntRecord = 0;
	CString strV, strUrl, strSplit, strDesc, strQqinfo;

	do{
		arrField.RemoveAll();
		bGetOne = synPara.GetOneRecord(dealPos, arrField);
		if(FALSE == bGetOne)
			break;
		if(arrField.GetSize() > 8)
		{
			/// Url SplitId SplitPass ServiceName Mark
			strUrl = arrField[1];
			strUrl.TrimRight();
			strUrl.TrimLeft("http: ");
			strUrl.TrimLeft("/");		// 去掉http:// 头, 如果有的话
// 			CustomUrlField(
// 			int infotype, CString httpname,CString domain,CString userid, CString password,
// 			CString email="",CString qq="",CString addres="",CString mobile="",
// 			CString strcardnum="",CString cardtype="")
//13		bbs.dvbbs.net//reg.asp	动网注册	username_	psw_	e_mail	OICQ	address	userphone		
//			strDesc = arrField[0];
//			strDesc += "用户名";
			CustomUrlField tmp(2, arrField[0],strUrl, arrField[3], arrField[4],
				arrField[5], arrField[6],arrField[7], arrField[8],arrField[9], arrField[10]); 
			v_HttpregistList[bufid].push_back(tmp);
		}

	}while(bGetOne);
	
	InterlockedExchange(&v_HttpregistAccID, bufid);
	v_limitcount[0][HTTPUPSPLIT] = cntRecord;
	return TRUE;
}
BOOL UpdateIndexLimit(LPCTSTR filename)
{
	long	bufid = (v_IdexlimitID==0)? 1 : 0;
	v_Indexlimtmap[bufid].RemoveAll();
	DWORD paratype, dwparaval;
	CString paraval,paraip,paraport,parano;
	
	BOOL bInvalid ;
	int		staPort[65536];
	memset(staPort, 0, sizeof(staPort));

	// 全文线索类型
	#define			IndexPort				49
	#define			IndexIp					8
	#define			IndexIpPort				52	// 2008.3.13
	#define			bcp_idxlimit_count		10
	#define			bcp_idxlimit_type		2
	#define			bcp_idxlimit_ip			3
	#define			bcp_idxlimit_port		5
	#define			bcp_idxlimit_no			1

	BOOL bReadOK = TRUE;
	CSyncPara synPara;
	DWORD dealPos = 0;
	if(FALSE == synPara.LoadPara(filename, dealPos, TRUE))
	{
		return TRUE;
	}
	CStringArray arrField;
	BOOL bGetOne;
	int cntRecord = 0;
	CString strV, strUrl, strSplit, strDesc, strQqinfo;

	do{
		arrField.RemoveAll();
		bGetOne = synPara.GetOneRecord(dealPos, arrField);
		if(FALSE == bGetOne)
			break;
		paratype = atoi(arrField[bcp_idxlimit_type]);
		paraip = CString(arrField[bcp_idxlimit_ip]);
		paraport = CString(arrField[bcp_idxlimit_port]);
		parano = CString(arrField[bcp_idxlimit_no]);
		dwparaval = atoi(paraport);
		bInvalid = FALSE;
		switch(paratype)
		{
		case IndexPort:
			if(dwparaval > 0 && dwparaval < 65536)
			{
				sindexlimit tindexlimit;
				tindexlimit.typeinfo=IndexPort;
				tindexlimit.no=parano;
				v_Indexlimtmap[bufid].SetAt(paraport,tindexlimit);
			}
			break;
		case IndexIpPort:
			{				
				sindexlimit tindexlimit;
				tindexlimit.typeinfo=IndexIpPort;
				tindexlimit.no=parano;
				tindexlimit.m_port=paraport;
				v_Indexlimtmap[bufid].SetAt(paraip,tindexlimit);
			}
			break;
		case IndexIp:
			{				
				sindexlimit tindexlimit;
				tindexlimit.typeinfo=IndexIp;
				tindexlimit.no=parano;
				v_Indexlimtmap[bufid].SetAt(paraip,tindexlimit);
			}
			break;

		default:
			ASSERT(FALSE);
			break;
		}

	}while(bGetOne);
	
	InterlockedExchange(&v_IdexlimitID, bufid);
	return TRUE;
}
inline void AddDataToMap(int rubtype, CString rubdata, int bufid)
{
	ASSERT(bufid==0 || bufid==1);
	ASSERT(!rubdata.IsEmpty());
	if (rubdata.IsEmpty())
		return;
	switch(rubtype)
	{
	case 8:		//	固定IP
		v_RubbishPara.m_rubbipmap[bufid].SetAt(rubdata, (void*)rubtype);
		v_limitcount[0][LIMITTYPE_RUBBISHIP]++;
		break;
	case 14:	//	URL
		rubdata.MakeLower();
		v_RubbishPara.m_rubburllist[bufid].push_back((LPCTSTR)rubdata);
		v_limitcount[0][LIMITTYPE_RUBBISHURL]++;
		break;
	case 16:	//	上网帐号
	case 17:	//	电话号码
		rubdata.MakeLower();
		v_RubbishPara.m_rubbaaamap[bufid].SetAt(rubdata, (void*)rubtype);
		v_limitcount[0][LIMITTYPE_RUBBISHAAA]++;
		break;
	case 41:	//	邮箱地址
	case 42:	//	邮件主题
	case 43:	//	附件名称
		v_RubbishPara.m_rubbmaillist[bufid].push_back((LPCTSTR)rubdata);
		v_limitcount[0][LIMITTYPE_RUBBISHMAIL]++;
		break;
	default:
		// 非法的数据类型
		ASSERT(FALSE);
		break;
	}
}

BOOL UpdateFullRule(LPCTSTR filename)
{
	UpdateIndexLimit(filename);
	v_IndexLimitPara.UpdateRule(filename);
	return TRUE;
}

BOOL UpdateRubbish(LPCTSTR filename)
{
	ASSERT(v_RubbishPara.m_readflg==0 || v_RubbishPara.m_readflg==1);
	long	bufid = (v_RubbishPara.m_readflg==0)? 1 : 0;
	try
	{
		// 清空原缓冲区
		v_RubbishPara.m_rubbipmap[bufid].RemoveAll();
		v_RubbishPara.m_rubbaaamap[bufid].RemoveAll();
		v_RubbishPara.m_rubbmaillist[bufid].clear();
		v_RubbishPara.m_rubburllist[bufid].clear();
		memset(v_limitcount[0]+LIMITTYPE_RUBBISHIP, 0, (OICQID-LIMITTYPE_RUBBISHIP)*4);	
		v_limitcount[0][LIMITTYPE_RUBBISHIP]=0;
		v_limitcount[0][LIMITTYPE_RUBBISHURL]=0;
		v_limitcount[0][LIMITTYPE_RUBBISHAAA]=0;
		v_limitcount[0][LIMITTYPE_RUBBISHMAIL]=0;
		// 增加缺省的垃圾信息
		AddDataToMap(14, "codecs.microsoft.com/isapi/ocget.dll", bufid);
		AddDataToMap(14, "activex.microsoft.com/objects/ocget.dll", bufid);

		BOOL bReadOK = TRUE;
		CSyncPara synPara;
		DWORD dealPos = 0;
		if(FALSE == synPara.LoadPara(filename, dealPos, TRUE))
		{
			return TRUE;
		}
		CStringArray arrField;
		BOOL bGetOne;
		int cntRecord = 0;
		CString strV, strUrl, strSplit, strDesc, strQqinfo;
		do{

			arrField.RemoveAll();
			bGetOne = synPara.GetOneRecord(dealPos, arrField);
			if(FALSE == bGetOne)
				break;

			if(arrField.GetSize() < 2)
				continue;
// 			ASSERT(arrField.GetSize() == bcp_keyword_count);
// 			if(arrField.GetSize() != bcp_keyword_count)
// 			{
// 				bReadOK = FALSE;
// 				break;
// 			}
			CString tmpstr = arrField[1];
			tmpstr.TrimRight();
			if (!tmpstr.IsEmpty())
			{
				AddDataToMap(atol(arrField[0]), tmpstr, bufid);
			}
		}while(bGetOne);	
		InterlockedExchange(&v_RubbishPara.m_readflg, bufid);
		return TRUE;
	}
	catch(...)
	{
		WriteLog("dce", "LoadRubbishTable() Error!!");
		return FALSE;
	}
}

BOOL ReleaseMap(WarnlimitMap *r_WarnlimitMap)
{
	POSITION pos=r_WarnlimitMap->GetStartPosition();
	Lpwarnkeyinfo p;
	
	CString key;
	while (pos!=NULL)
	{
		r_WarnlimitMap->GetNextAssoc(pos,key,p);
		delete p;
		p = NULL;
	}
	r_WarnlimitMap->RemoveAll();
	return TRUE;
}

BOOL ReleaseCCicMap(ccicMap *r_ccicMap)
{
	POSITION pos=r_ccicMap->GetStartPosition();
	Lpccicinfo p;
	
	CString key;
	while (pos!=NULL)
	{
		r_ccicMap->GetNextAssoc(pos,key,p);
		delete p;
		p = NULL;
	}
	r_ccicMap->RemoveAll();
	return TRUE;
}

//以下为更新重点人员相关信息
BOOL updatekprule(int bufid)
{
int error=0;
	try
	{
		BOOL bReadOK = TRUE;
		CSyncPara synPara;
		DWORD dealPos = 0;
	//	CMapStringToString mapclassrule;
		CStringArray arrField;
		BOOL bGetOne;
		int cntRecord = 0;
		CString strfirstclass,strsecclass,strdetail,strclassvalue;
error=1;
		//规则map用来初始化重点人员分类每一项的分值
		if(TRUE == synPara.LoadPara("e:\\netspy\\para\\dce\\kprule.bcp", dealPos, TRUE))
		{
			do{
error=2;
				arrField.RemoveAll();
				bGetOne = synPara.GetOneRecord(dealPos, arrField);
				if(FALSE == bGetOne)
					break;
				if (arrField.GetSize()<4)
				{
					continue;
				}
error=3;
				strfirstclass=arrField[1];
				strsecclass=arrField[2];
				strdetail=arrField[0];
error=4;
				strclassvalue=arrField[3];
				if (!strclassvalue.IsEmpty())
				{
error=5;
					v_warnlimitPara.m_warnruleMap[bufid].SetAt(strfirstclass+strsecclass+strdetail,strclassvalue);
				}
				
			}while(bGetOne);
		}
		return TRUE;
	}
	catch(...)
	{
		WriteLog("dce", "updatekprule Error!! %d",error);
		return FALSE;
	}


}

BOOL updatekpkeyword(int bufid,AllDicMap* alldicmap)
{
int error=0;
	try
	{
		v_limitcount[0][KEYPEOPLE]=0;

		DWORD dealPos = 0;
		BOOL bReadOK = TRUE;
		CSyncPara synPara;
	//	CMapStringToString mapclassrule;
		CStringArray arrField;
		BOOL bGetOne;
		int cntRecord = 0;
		CString strfirstclass,strsecclass,strdetail,strclassvalue;
error=1;
		Lpwarnkeyinfo lpwarnlimit = NULL;
		CString strkey;
		if(TRUE == synPara.LoadPara("e:\\netspy\\para\\dce\\kpkeyword.bcp", dealPos, TRUE))
		{
			do{

				arrField.RemoveAll();
				bGetOne = synPara.GetOneRecord(dealPos, arrField);
				if(FALSE == bGetOne)
					break;
				if (arrField.GetSize()<4)
				{
					continue;
				}
error=2;
				strfirstclass=arrField[1];
				strsecclass=arrField[2];
				strkey=arrField[3];
				CString strtmp;
				//首先判断重点人员的类对应规则中的是否需要匹配关键字，否则不初始化到字典中
				if (v_warnlimitPara.m_warnruleMap[bufid].Lookup(strfirstclass+"_"+strsecclass+"7",strtmp))
				{
					lpwarnlimit = new warnkeyinfo;
					sprintf(lpwarnlimit->m_firstclass,"%s",strfirstclass);
					sprintf(lpwarnlimit->m_secclass ,"%s", strsecclass);
					sprintf(lpwarnlimit->m_key ,"%s",strkey);
					lpwarnlimit->m_keyquanzhi = atoi(strtmp);
error=3;
					AllDicInfo* lpalldicinfo = NULL;
					if (alldicmap->Lookup(strkey,(void*&)lpalldicinfo))
					{
						lpalldicinfo->m_warninfo->m_last->m_next = lpwarnlimit;
						lpalldicinfo->m_warninfo->m_last = lpwarnlimit;
					}
					else
					{
						lpalldicinfo = new AllDicInfo;
error=4;						
						if(lpalldicinfo->m_warninfo == NULL)
						{
							lpalldicinfo->m_warninfo = lpwarnlimit;
						}
						alldicmap->SetAt(strkey,(void*&)lpalldicinfo);
					}
				}
				v_limitcount[0][KEYPEOPLE]++;

				
			}while(bGetOne);
		}
		return TRUE;
	}
	catch(...)
	{
		WriteLog("dce", "updatekpkeyword() Error!! %d",error);
		return FALSE;
	}


}
BOOL updatecardinfo(int bufid,CMapStringToString &idclassinfomap)
{
int error=0;
	try
	{
		BOOL bReadOK = TRUE;
		CSyncPara synPara;
		DWORD dealPos = 0;
	//	CMapStringToString mapclassrule;
		CStringArray arrField;
		BOOL bGetOne;
		int cntRecord = 0;
		CString strfirstclass,strsecclass,strid,strclassvalue,straccount;
		Lpwarnkeyinfo lpwarnlimit = NULL;
error=1;
		if(TRUE == synPara.LoadPara("e:\\netspy\\para\\dce\\cardinfo.bcp", dealPos, TRUE))
		{
			do{

				arrField.RemoveAll();
				bGetOne = synPara.GetOneRecord(dealPos, arrField);
				if(FALSE == bGetOne)
					break;
				if (arrField.GetSize()<3)
				{
					continue;
				}
				strid.Format("%011s",arrField[0]);
error=2;
				if (idclassinfomap.Lookup(strid,strclassvalue))
				{
					CStringArray arrclass;
					Splitstr(strclassvalue,arrclass,'\t');
					if (arrclass.GetSize()<2)
					{
						continue;
					}
					strfirstclass=arrclass[0];
					strsecclass=arrclass[1];
				}
				else
				{
					continue;
				}
error=3;
				straccount=arrField[2];
				if (!straccount.IsEmpty())
				{
					lpwarnlimit = new warnkeyinfo;
					sprintf(lpwarnlimit->m_id ,"%s", strid);
					sprintf(lpwarnlimit->m_firstclass ,"%s",strfirstclass);
					sprintf(lpwarnlimit->m_secclass ,"%s", strsecclass);
					warnkeyinfo* lpwarnkeyinfo = NULL;
					if (v_warnlimitPara.m_warnlimitMap[bufid].Lookup(straccount,lpwarnkeyinfo))
					{
						lpwarnkeyinfo->m_last->m_next = lpwarnlimit;
						lpwarnkeyinfo->m_last = lpwarnlimit;
					}
					else
					{
						v_warnlimitPara.m_warnlimitMap[bufid].SetAt(straccount,lpwarnlimit);
					}
				}
				
			}while(bGetOne);
		}
		return TRUE;
	}
	catch(...)
	{
		WriteLog("dce", "updatecardinfo() Error!! %d",error);
		return FALSE;
	}

}

BOOL updatekeypeople(int bufid,CMapStringToString &peopleclassmap)
{
int error=0;
	try
	{
		CMapStringToString idclassinfomap;
		BOOL bReadOK = TRUE;
		CSyncPara synPara;
		DWORD dealPos = 0;
	//	CMapStringToString mapclassrule;
		CStringArray arrField;
		BOOL bGetOne;
		int cntRecord = 0;
		CString strfirstclass,strsecclass,strdetail,strclassvalue;
		Lpwarnkeyinfo lpwarnlimit = NULL;
error=1;
		CString strid,straccount,strip,strmac,strclass;
		if(TRUE == synPara.LoadPara("e:\\netspy\\para\\dce\\keypeople.bcp", dealPos, TRUE))
		{
			do{

				arrField.RemoveAll();
				bGetOne = synPara.GetOneRecord(dealPos, arrField);
				if(FALSE == bGetOne)
					break;
				if (arrField.GetSize()<5)
				{
					continue;
				}
				strid=arrField[0];
				strclass=arrField[4];
				CStringArray arraccount,arrip,arrmac,arrclass;
				Splitstr(strclass,arrclass,',');
				if (arrclass.GetSize()<2)
				{
					continue;
				}
				strfirstclass=arrclass[0];
				strsecclass=arrclass[1];
				idclassinfomap.SetAt(strid,strfirstclass+'\t'+strsecclass);
error=2;
				Splitstr(arrField[1],arraccount,',');
				Splitstr(arrField[2],arrip,',');
				Splitstr(arrField[3],arrmac,',');

				peopleclassmap.SetAt(strid,strclass);
				for (int i=0;i<arraccount.GetSize();i++)
				{
					lpwarnlimit = new warnkeyinfo;
					sprintf(lpwarnlimit->m_id ,"%s", strid);
					sprintf(lpwarnlimit->m_firstclass ,"%s",strfirstclass);
					sprintf(lpwarnlimit->m_secclass ,"%s", strsecclass);
					warnkeyinfo* lpwarnkeyinfo = NULL;
					if (v_warnlimitPara.m_warnlimitMap[bufid].Lookup(arraccount.GetAt(i),lpwarnkeyinfo))
					{
						lpwarnkeyinfo->m_last->m_next = lpwarnlimit;
						lpwarnkeyinfo->m_last = lpwarnlimit;
					}
					else
					{
						v_warnlimitPara.m_warnlimitMap[bufid].SetAt(arraccount.GetAt(i),lpwarnlimit);
					}
				}
error=3;
				for (i=0;i<arrip.GetSize();i++)
				{
					lpwarnlimit = new warnkeyinfo;
					_snprintf(lpwarnlimit->m_id ,21,"%s", strid);
					_snprintf(lpwarnlimit->m_firstclass,20 ,"%s", strfirstclass);
					_snprintf(lpwarnlimit->m_secclass ,20,"%s", strsecclass);
					char tmpipbuf[16];
					if(!FormatIp(arrip.GetAt(i),tmpipbuf))
					{
						continue;
					}
					warnkeyinfo* lpwarnkeyinfo = NULL;
					if (v_warnlimitPara.m_warnlimitMap[bufid].Lookup(tmpipbuf,lpwarnkeyinfo))
					{
						lpwarnkeyinfo->m_last->m_next = lpwarnlimit;
						lpwarnkeyinfo->m_last = lpwarnlimit;
					}
					else
					{
						v_warnlimitPara.m_warnlimitMap[bufid].SetAt(tmpipbuf,lpwarnlimit);
					}
				}
error=4;
				for (i=0;i<arrmac.GetSize();i++)
				{
					lpwarnlimit = new warnkeyinfo;
					sprintf(lpwarnlimit->m_id ,"%s", strid);
					sprintf(lpwarnlimit->m_firstclass ,"%s", strfirstclass);
					sprintf(lpwarnlimit->m_secclass ,"%s", strsecclass);
					warnkeyinfo* lpwarnkeyinfo = NULL;
					if (v_warnlimitPara.m_warnlimitMap[bufid].Lookup(arrmac.GetAt(i),lpwarnkeyinfo))
					{
						lpwarnkeyinfo->m_last->m_next = lpwarnlimit;
						lpwarnkeyinfo->m_last = lpwarnlimit;
					}
					else
					{
						v_warnlimitPara.m_warnlimitMap[bufid].SetAt(arrmac.GetAt(i),lpwarnlimit);
					}
				}

				
			}while(bGetOne);
			updatecardinfo(bufid,idclassinfomap);
		}
		return TRUE;
	}
	catch(...)
	{
		WriteLog("dce", "updatekeypeople() Error!! %d",error);
		return FALSE;
	}

}

BOOL updatenetclimit(int bufid,CMapStringToString &peopleclassmap)
{
	int error=0;
	try
	{
		BOOL bReadOK = TRUE;
		CSyncPara synPara;
		DWORD dealPos = 0;
	//	CMapStringToString mapclassrule;
		CStringArray arrField;
		BOOL bGetOne;
		int cntRecord = 0;
		CString strfirstclass,strsecclass,strdetail,strclassvalue;
		Lpwarnkeyinfo lpwarnlimit = NULL;
		CString strid,straccount,strip,strmac,strclass;
error=1;
		CString stridstail;
		if(TRUE == synPara.LoadPara("e:\\netspy\\para\\dce\\netclimit.bcp", dealPos, TRUE))
		{
			do{

				arrField.RemoveAll();
				bGetOne = synPara.GetOneRecord(dealPos, arrField);
				if(FALSE == bGetOne)
					break;
				if (arrField.GetSize()<3)
				{
					continue;
				}
				strid=arrField[0];
				stridstail=arrField[2];
error=2;
				CString strtmp;
				if(peopleclassmap.Lookup(strid,strclass))
				{
					CStringArray arrclass;
					Splitstr(strclass,arrclass,',');
					strfirstclass=arrclass[0];
					strsecclass=arrclass[1];
error=3;
					lpwarnlimit = new warnkeyinfo;
					sprintf(lpwarnlimit->m_id ,"%s", strid);
					sprintf(lpwarnlimit->m_firstclass ,"%s", strfirstclass);
					sprintf(lpwarnlimit->m_secclass ,"%s", strsecclass);
					warnkeyinfo* lpwarnkeyinfo = NULL;
					if (v_warnlimitPara.m_warnlimitMap[bufid].Lookup(stridstail,lpwarnkeyinfo))
					{
						lpwarnkeyinfo->m_last->m_next = lpwarnlimit;
						lpwarnkeyinfo->m_last = lpwarnlimit;
					}
					else
					{
						v_warnlimitPara.m_warnlimitMap[bufid].SetAt(stridstail,lpwarnlimit);
					}

				}

				
			}while(bGetOne);
		}
		return TRUE;
	}
	catch(...)
	{
		WriteLog("dce", "updatenetclimit() Error!! %d",error);
		return FALSE;
	}


}
BOOL UpdateCcicDictionary(AllDicMap* alldicmap,int bufid)
{
	return TRUE;
int error=0;
// 	try
// 	{
// 
// 
// 		AllDicInfo* lpalldicinfo;
// 		POSITION pos = alldicmap->GetStartPosition();
// 		CString mapkey;
// 		BOOL HaveKey = FALSE;
// 		int listnum=0;
// 		CRbStringArray sorarry;
// 		while (pos != NULL)
// 		{
// 			alldicmap->GetNextAssoc(pos,mapkey,(void*&)lpalldicinfo);
// 			sorarry.Add(mapkey);
// 			listnum++;
// 		}
// error=1;
// 		sorarry.Sort();
// 		for (int mm=0;mm<sorarry.GetSize();mm++)
// 		{
// 			if (alldicmap->Lookup(sorarry[mm],(void*&)lpalldicinfo))
// 			{
// 				Vocable temp;
// 				temp.m_Vocable.Format("%s",sorarry[mm]);
// 				temp.m_pUserData = (VOID*)lpalldicinfo;
// 				v_ccicpara.m_keyworddic[bufid].m_VocableList.push_front(temp);
// 				HaveKey = TRUE;
// 			}
// 		}
// error=2;
// 		if (HaveKey)
// 		{
// 			v_ccicpara.m_keyworddic[bufid].IniKeyBTree(200000*20);
// 		}
// 		
// 		return TRUE;
// 	}
// 	catch(...)
// 	{
// 		WriteLog("dce", "UpdateCcicDictionary() Error!! %d",error);
// 		return FALSE;
// 	}

}
BOOL UpdateCcicPara(LPCTSTR filename)
{
#define CCICFILENAME "e:\\netspy\\para\\dce\\ccic.bcp"
int error=0;
	try
	{
		int flen = GetFileLength(CCICFILENAME);
		//ccic因更新太慢，只要大小不变时间变了也不更新
		if (v_ccicpara.m_filelen==flen)
		{
			return TRUE;
		}
		v_limitcount[0][CCIC]=0;
		long	bufid = (v_ccicpara.m_readflg==0)? 1 : 0;
//		v_ccicpara.m_warnlimitmap[bufid].ClearData();

		BOOL bReadOK = TRUE;
		CSyncPara synPara;
		DWORD dealPos = 0;
		CStringArray arrField;
		BOOL bGetOne;
		int cntRecord = 0;
		CString strfirstclass,strsecclass,strdetail,strclassvalue;
		Lpwarnkeyinfo lpwarnlimit = NULL;
		CString strid,straccount,strip,strmac,strclass;
error=1;
		//读取ccic
		int ccicnum=0;
		int ridnum=0;
		int memsize=0;
		CString strname,strnick,strdate;
		if(TRUE == synPara.LoadPara(CCICFILENAME, dealPos, TRUE))
		{
error=111;

			v_ccicpara.m_mempool[bufid]=new char[flen+1000];
			do{

				arrField.RemoveAll();
				bGetOne = synPara.GetOneRecord(dealPos, arrField);
				if(FALSE == bGetOne)
					break;
				if (arrField.GetSize()<=4)
				{
					continue;
				}
				strid=arrField[4];
				strname=arrField[0];
				strnick=arrField[1];
				strdate=arrField[3];
				strid.TrimRight();
				if (strid.IsEmpty())
				{
					continue;
				}
error=2;
				CString strtmp;
				Lpccicinfo lpccicinfo;
				if (!v_ccicpara.m_ccicmap[bufid].Lookup(strid,lpccicinfo))
				{
					lpccicinfo=new ccicinfo;
 					int ccicbuflen=strname.GetLength()+strnick.GetLength()+strdate.GetLength()+strid.GetLength()+3;
					lpccicinfo->m_ccicbuf=v_ccicpara.m_mempool[bufid]+memsize;
					memsize+=ccicbuflen+1;
					if (memsize>flen)
					{
						break;
					}
error=3;
					v_ccicpara.m_mempool[bufid][memsize]='\0';
					memsize+=1;
					_snprintf(lpccicinfo->m_ccicbuf,ccicbuflen,"%s\t%s\t%s\t%s",strname,strnick,strid,strdate);
						ridnum++;
					_snprintf(lpccicinfo->m_id,20,"%s",strid);
	ridnum++;
					
					v_ccicpara.m_ccicmap[bufid].SetAt(strid,lpccicinfo);
				}
				v_limitcount[0][CCIC]++;

			}while(bGetOne);
		}

		InterlockedExchange(&v_ccicpara.m_readflg, bufid);

 		bufid=(v_ccicpara.m_readflg==0)? 1 : 0;
		ReleaseCCicMap(&v_ccicpara.m_ccicmap[bufid]);
		if(v_ccicpara.m_mempool[bufid]!=NULL)
		{
			delete[] v_ccicpara.m_mempool[bufid];
			v_ccicpara.m_mempool[bufid]=NULL;
		}
 		v_ccicpara.m_filelen=flen;

		return TRUE;
	}
	catch(...)
	{
		WriteLog("dce", "updateccic() Error!! %d",error);
		return FALSE;
	}

}

BOOL UpdateWarnLimit(AllDicMap* alldicmap,long bufid)
{
	WriteLog("dce", "更新 UpdateWarnLimit");

int error=0;
	try
	{


// 		BOOL bReadOK = TRUE;
// 		CSyncPara synPara;
// 		DWORD dealPos = 0;
// 	//	CMapStringToString mapclassrule;
// 		CStringArray arrField;
// 		BOOL bGetOne;
// 		int cntRecord = 0;
		CString strfirstclass,strsecclass,strdetail,strclassvalue;

		ReleaseMap(&v_warnlimitPara.m_warnlimitMap[bufid]);
		v_warnlimitPara.m_warnruleMap[bufid].RemoveAll();
		CMapStringToString peopleclassmap;//重点人员id对应的分类，用来初始化重点人员虚拟身份

	//规则map用来初始化重点人员分类每一项的分值

		updatekprule(bufid);
	//读取关键字类，初始化关键字类字典

	    updatekpkeyword(bufid,alldicmap);
	// 读取重点人员表，初始线索map

		updatekeypeople(bufid,peopleclassmap);
		//读取重点人员线索表

		updatenetclimit(bufid,peopleclassmap);
			//读取ccic

//		updateccic(bufid,alldicmap);

	//规则map用来初始化重点人员分类每一项的分值
/*		if(TRUE == synPara.LoadPara("e:\\netspy\\para\\dce\\kprule.bcp", dealPos, TRUE))
		{
			do{

				arrField.RemoveAll();
				bGetOne = synPara.GetOneRecord(dealPos, arrField);
				if(FALSE == bGetOne)
					break;
				strfirstclass=arrField[1];
				strsecclass=arrField[2];
				strdetail=arrField[0];
				strclassvalue=arrField[3];
				if (!strclassvalue.IsEmpty())
				{
					v_warnlimitPara.m_warnruleMap[bufid].SetAt(strfirstclass+strsecclass+strdetail,strclassvalue);
				}
				
			}while(bGetOne);
		}


	//读取关键字类，初始化关键字类字典
		Lpwarnkeyinfo lpwarnlimit = NULL;
error=1;
		CString strkey;
		if(TRUE == synPara.LoadPara("e:\\netspy\\para\\dce\\kpkeyword.bcp", dealPos, TRUE))
		{
			do{

				arrField.RemoveAll();
				bGetOne = synPara.GetOneRecord(dealPos, arrField);
				if(FALSE == bGetOne)
					break;
				strfirstclass=arrField[1];
				strsecclass=arrField[2];
				strkey=arrField[3];
				CString strtmp;
				//首先判断重点人员的类对应规则中的是否需要匹配关键字，否则不初始化到字典中
				if (v_warnlimitPara.m_warnruleMap[bufid].Lookup(strfirstclass+"_"+strsecclass+"7",strtmp))
				{
					lpwarnlimit = new warnkeyinfo;
					sprintf(lpwarnlimit->m_firstclass,"%s",strfirstclass);
					sprintf(lpwarnlimit->m_secclass ,"%s", strsecclass);
					sprintf(lpwarnlimit->m_key ,"%s",strkey);
					lpwarnlimit->m_keyquanzhi = atoi(strtmp);

					AllDicInfo* lpalldicinfo = NULL;
					if (alldicmap->Lookup(strkey,(void*&)lpalldicinfo))
					{
						lpalldicinfo->m_warninfo->m_last->m_next = lpwarnlimit;
						lpalldicinfo->m_warninfo->m_last = lpwarnlimit;
					}
					else
					{
						lpalldicinfo = new AllDicInfo;
						
						if(lpalldicinfo->m_warninfo == NULL)
						{
							lpalldicinfo->m_warninfo = lpwarnlimit;
						}
						alldicmap->SetAt(strkey,(void*&)lpalldicinfo);
					}
				}
				
			}while(bGetOne);
		}


error=2;

	// 读取重点人员表，初始线索map
		CString strid,straccount,strip,strmac,strclass;
		if(TRUE == synPara.LoadPara("e:\\netspy\\para\\dce\\keypeople.bcp", dealPos, TRUE))
		{
			do{

				arrField.RemoveAll();
				bGetOne = synPara.GetOneRecord(dealPos, arrField);
				if(FALSE == bGetOne)
					break;
				if (arrField.GetSize()<=4)
				{
					continue;
				}
				strid=arrField[0];
				strclass=arrField[4];
				CStringArray arraccount,arrip,arrmac,arrclass;
				Splitstr(strclass,arrclass,',');
				if (arrclass.GetSize()<2)
				{
					continue;
				}
				strfirstclass=arrclass[0];
				strsecclass=arrclass[1];
				Splitstr(arrField[1],arraccount,',');
				Splitstr(arrField[2],arrip,',');
				Splitstr(arrField[3],arrmac,',');

				peopleclassmap.SetAt(strid,strclass);
				for (int i=0;i<arraccount.GetSize();i++)
				{
					lpwarnlimit = new warnkeyinfo;
					sprintf(lpwarnlimit->m_id ,"%s", strid);
					sprintf(lpwarnlimit->m_firstclass ,"%s",strfirstclass);
					sprintf(lpwarnlimit->m_secclass ,"%s", strsecclass);
					warnkeyinfo* lpwarnkeyinfo = NULL;
					if (v_warnlimitPara.m_warnlimitMap[bufid].Lookup(arraccount.GetAt(i),lpwarnkeyinfo))
					{
						lpwarnkeyinfo->m_last->m_next = lpwarnlimit;
						lpwarnkeyinfo->m_last = lpwarnlimit;
					}
					else
					{
						v_warnlimitPara.m_warnlimitMap[bufid].SetAt(arraccount.GetAt(i),lpwarnlimit);
					}
				}
				for (i=0;i<arrip.GetSize();i++)
				{
					lpwarnlimit = new warnkeyinfo;
					_snprintf(lpwarnlimit->m_id ,21,"%s", strid);
					_snprintf(lpwarnlimit->m_firstclass,20 ,"%s", strfirstclass);
					_snprintf(lpwarnlimit->m_secclass ,20,"%s", strsecclass);
					char tmpipbuf[16];
					if(!FormatIp(arrip.GetAt(i),tmpipbuf))
					{
						continue;
					}
					warnkeyinfo* lpwarnkeyinfo = NULL;
					if (v_warnlimitPara.m_warnlimitMap[bufid].Lookup(tmpipbuf,lpwarnkeyinfo))
					{
						lpwarnkeyinfo->m_last->m_next = lpwarnlimit;
						lpwarnkeyinfo->m_last = lpwarnlimit;
					}
					else
					{
						v_warnlimitPara.m_warnlimitMap[bufid].SetAt(tmpipbuf,lpwarnlimit);
					}
				}
				for (i=0;i<arrmac.GetSize();i++)
				{
					lpwarnlimit = new warnkeyinfo;
					sprintf(lpwarnlimit->m_id ,"%s", strid);
					sprintf(lpwarnlimit->m_firstclass ,"%s", strfirstclass);
					sprintf(lpwarnlimit->m_secclass ,"%s", strsecclass);
					warnkeyinfo* lpwarnkeyinfo = NULL;
					if (v_warnlimitPara.m_warnlimitMap[bufid].Lookup(arrmac.GetAt(i),lpwarnkeyinfo))
					{
						lpwarnkeyinfo->m_last->m_next = lpwarnlimit;
						lpwarnkeyinfo->m_last = lpwarnlimit;
					}
					else
					{
						v_warnlimitPara.m_warnlimitMap[bufid].SetAt(arrmac.GetAt(i),lpwarnlimit);
					}
				}

				
			}while(bGetOne);
		}

			//读取重点人员线索表
error=1;
		CString stridstail;
		if(TRUE == synPara.LoadPara("e:\\netspy\\para\\dce\\netclimit.bcp", dealPos, TRUE))
		{
			do{

				arrField.RemoveAll();
				bGetOne = synPara.GetOneRecord(dealPos, arrField);
				if(FALSE == bGetOne)
					break;
				strid=arrField[0];
				stridstail=arrField[2];

				CString strtmp;
				if(peopleclassmap.Lookup(strid,strclass))
				{
					CStringArray arrclass;
					Splitstr(strclass,arrclass,',');
					strfirstclass=arrclass[0];
					strsecclass=arrclass[1];

					lpwarnlimit = new warnkeyinfo;
					sprintf(lpwarnlimit->m_id ,"%s", strid);
					sprintf(lpwarnlimit->m_firstclass ,"%s", strfirstclass);
					sprintf(lpwarnlimit->m_secclass ,"%s", strsecclass);
					warnkeyinfo* lpwarnkeyinfo = NULL;
					if (v_warnlimitPara.m_warnlimitMap[bufid].Lookup(stridstail,lpwarnkeyinfo))
					{
						lpwarnkeyinfo->m_last->m_next = lpwarnlimit;
						lpwarnkeyinfo->m_last = lpwarnlimit;
					}
					else
					{
						v_warnlimitPara.m_warnlimitMap[bufid].SetAt(stridstail,lpwarnlimit);
					}

				}

				
			}while(bGetOne);
		}

		
error=3;
	//读取ccic
		int ccicnum=0;
		int ridnum=0;
		int memsize=0;
		int lpalldicinfonum=0;
		CString strname,strnick,strdate;
		BaseAllDicInfo* lpalldicinfo = NULL;
		if(TRUE == synPara.LoadPara("e:\\netspy\\para\\dce\\ccic.bcp", dealPos, TRUE))
		{
			v_warnlimitPara.m_mempool[bufid]=new char[20*1024*1024];
			v_warnlimitPara.m_lpalldicinfo[bufid]=new BaseAllDicInfo[50*10000];

			do{

				arrField.RemoveAll();
				bGetOne = synPara.GetOneRecord(dealPos, arrField);
				if(FALSE == bGetOne)
					break;
				if (arrField.GetSize()<=4)
				{
					continue;
				}
				strid=arrField[4];
				strname=arrField[0];
				strnick=arrField[1];
				strdate=arrField[3];
				strid.TrimRight();
				if (strid.IsEmpty())
				{
					continue;
				}
				ccicnum++;
				if (ccicnum>400000)
				{
					break;
				}
				CString strtmp;
error=4;
				if (alldicmap->Lookup(strid,(void*&)lpalldicinfo))
				{
					;
				}
				else
				{
					int ccicbuflen=strname.GetLength()+strnick.GetLength()+strdate.GetLength()+strid.GetLength()+3;
					lpalldicinfo = v_warnlimitPara.m_lpalldicinfo[bufid]+lpalldicinfonum;//new BaseAllDicInfo;
					lpalldicinfonum++;
					lpalldicinfo->m_ccicbuf=v_warnlimitPara.m_mempool[bufid]+memsize;
					memsize+=ccicbuflen+1;
					if (memsize>20*1024*1024-100)
					{
						break;
					}
					v_warnlimitPara.m_mempool[bufid][memsize]='\0';
					memsize+=1;

					lpalldicinfo->m_isccic=TRUE;
					_snprintf(lpalldicinfo->m_ccicbuf,ccicbuflen,"%s\t%s\t%s\t%s",strname,strnick,strid,strdate);
ridnum++;
error=5;
					
					alldicmap->SetAt(strid,(void*&)lpalldicinfo);
				}
				
			}while(bGetOne);
		}
*/


		return TRUE;
	}
	catch (...)
	{
		WriteLog("dce","UpdateWarnLimit error");
	}
	return FALSE;
}

