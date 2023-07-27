#include "stdafx.h"
#include "..\\include\\d4all.hpp"
#include "..\\include\\zwsql.h"

#include "Dce.h"
#include <sys\\stat.h>
#include "IpQueryLib.h"

//#include <iostream>
//#include <wininet.h>
//#pragma comment(lib, "wininet.lib")
#include <winhttp.h>
#pragma comment(lib, "winhttp.lib")

extern BOOL sIPtouIP(const char *ip, unsigned int &uIP);
static DWORD v_printpara = 0;
static DWORD v_limitcount[2][25] = 
{
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};
//static DWORD v_tmplimitcn[15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
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

BOOL UpdateSystemPara(LPCTSTR filename);
BOOL UpdateCaseLimit(LPCTSTR filename);
BOOL UpdateWarnLimit(LPCTSTR filename);
BOOL UpdateIndexLimit(LPCTSTR filename);
BOOL UpdateIndexKeyWord(LPCTSTR filename);
BOOL UpdateRubbish(LPCTSTR filename);
BOOL UpdateLegalIP(LPCTSTR filename);
BOOL UpdateIdcIpAddr(LPCTSTR filename);
BOOL UpdateOicqPwd(LPCTSTR filename);
BOOL UpdateLocalIpAddr(LPCTSTR filename);
BOOL UpdateAllIpAddr(LPCTSTR filename);
BOOL UpdateHttpupSplit(LPCTSTR filename);
BOOL UpdateIDCHttpKey(LPCTSTR filename);
BOOL UpdateIDCServerPort(LPCTSTR filename);
// BOOL UpdatePeopleKey(LPCTSTR filename,int bufid);
// BOOL UpdateWarnKey(LPCTSTR filename,int bufid);
BOOL UpdateIndexRule(AllDicMap* alldicmap,int bufid);
BOOL ClearCaseMap(CaseIpMacMap *caseipmacmap);
BOOL UpdateTaobaoPwd(LPCTSTR filename);

//初始化或者更新字典
BOOL UpdateDictionary(AllDicMap* alldicmap,int bufid);


void crypt(char *sour,int   src_len);
int str_trim_crypt_cpy(char*pdtr, char *pstr);

typedef BOOL (*LIMITFUNCTION)(LPCTSTR);	// 更新参数函数指针

struct ParaFileInfo
{
	LPCTSTR			fname;				// dbf文件名(不包含路径)
	time_t			filetime;			// 本地的文件时间
	time_t			updatetm;			// 最后更新时间
	BOOL			updateflg;			// 更新标志
	int				update;				// 强行更新时间间隔
	LIMITFUNCTION	pUpdateFunc;		// 更新参数函数指针
	UINT			systemtype;			// 适用系统
};

#define		FIXTABLENUM	17	//固定表名的表个数
#define		PARAMTABLECLIMIT 1
int			tablenum = 17;					// 表的总个数
CStringArray tablefilearr;				


static ParaFileInfo g_parainfo[32] = \
{
/*0*/	{_T("system.sql"),		0,  0,	TRUE,	600,		UpdateSystemPara,		SYSTEM_ALL},
		{_T("climit.sql"),		0,  0,	TRUE,	600,		UpdateCaseLimit,		SYSTEM_ALL},
		{_T("indexlimit.sql"),	0,  0,	TRUE,	3600,		UpdateIndexLimit,		SYSTEM_ALL-SYSTEM_SLEUTH},
		{_T("keyword.sql"),		0,  0,	TRUE,	3600,		UpdateIndexKeyWord,		SYSTEM_ALL-SYSTEM_SLEUTH},
		{_T("exception.sql"),	0,  0,	TRUE,	3600,		UpdateRubbish,			SYSTEM_ALL-SYSTEM_SLEUTH},
/*5*/	{_T("legalip.sql"),		0,  0,	TRUE,	24*60*60,	UpdateLegalIP,			SYSTEM_SZFJ},
		{_T("idc_ipinfo.sql"),	0,  0,	TRUE,	48*60*60,	UpdateIdcIpAddr,		SYSTEM_002},
		{_T("Ipaddr.rar"),		0,  0,	TRUE,	72*60*60,	UpdateLocalIpAddr,		SYSTEM_ALL-SYSTEM_SLEUTH},
		{_T("allIpaddr.rar"),	0,  0,	TRUE,	72*60*60,	UpdateAllIpAddr,		SYSTEM_ALL-SYSTEM_SLEUTH},
		{_T("httpupmatch.sql"),	0,  0,	TRUE,	7200,		UpdateHttpupSplit,		SYSTEM_ALL-SYSTEM_SLEUTH},
/*10*/	{_T("idchttpkey.sql"),	0,  0,	TRUE,	3600,		UpdateIDCHttpKey,		SYSTEM_002},
		{_T("idcsvrport.sql"),	0,  0,	TRUE,	3600,		UpdateIDCServerPort,	SYSTEM_002},
		{_T("taobaopass.rar"),	0,  0,	TRUE,	48*60*60,	UpdateTaobaoPwd,		SYSTEM_ALL},
		{_T("qqpas.rar"),		0,	0,	TRUE,	48*60*60,	UpdateOicqPwd,			SYSTEM_ALL},
		{_T("ImportantMan.sql"),0,  0,	TRUE,	3600,		UpdateCaseLimit,		SYSTEM_ALL-SYSTEM_SLEUTH},
		{_T("warnlevel.sql"),	0,  0,	TRUE,	3600,		UpdateCaseLimit,		SYSTEM_ALL-SYSTEM_SLEUTH},
		{_T("warnrule.sql"),	0,  0,	TRUE,	3600,		UpdateCaseLimit,		SYSTEM_ALL-SYSTEM_SLEUTH},
/*17*/	{_T(""),				0,  0,			FALSE,	0,			NULL,					0},
		{_T(""),				0,  0,			FALSE,	0,			NULL,					0},
		{_T(""),				0,  0,			FALSE,	0,			NULL,					0},
		{_T(""),				0,  0,			FALSE,	0,			NULL,					0},
		{_T(""),				0,  0,			FALSE,	0,			NULL,					0},
		{_T(""),				0,  0,			FALSE,	0,			NULL,					0},
		{_T(""),				0,  0,			FALSE,	0,			NULL,					0},
		{_T(""),				0,  0,			FALSE,	0,			NULL,					0},
/*25*/	{_T(""),				0,  0,			FALSE,	0,			NULL,					0},
		{_T(""),				0,  0,			FALSE,	0,			NULL,					0},
		{_T(""),				0,  0,			FALSE,	0,			NULL,					0},
		{_T(""),				0,  0,			FALSE,	0,			NULL,					0},
		{_T(""),				0,  0,			FALSE,	0,			NULL,					0},
		{_T(""),				0,  0,			FALSE,	0,			NULL,					0},
/*31*/	{_T(""),				0,  0,			FALSE,	0,			NULL,					0},
};
static ParaFileInfo g_sleuthparainfo[32] = \
{
/*0*/	{_T("system.sql"),		0,  0,	TRUE,	600,		UpdateSystemPara,		SYSTEM_ALL},
		{_T("climit.sql"),		0,  0,	TRUE,	600,		UpdateCaseLimit,		SYSTEM_ALL},
		{_T("indexlimit.sql"),	0,  0,	TRUE,	3600,		UpdateIndexLimit,		SYSTEM_ALL-SYSTEM_SLEUTH},
		{_T("keyword.sql"),		0,  0,	TRUE,	3600,		UpdateIndexKeyWord,		SYSTEM_ALL-SYSTEM_SLEUTH},
		{_T("exception.sql"),	0,  0,	TRUE,	3600,		UpdateRubbish,			SYSTEM_ALL-SYSTEM_SLEUTH},
/*5*/	{_T("legalip.sql"),		0,  0,	TRUE,	24*60*60,	UpdateLegalIP,			SYSTEM_SZFJ},
		{_T("idc_ipinfo.sql"),	0,  0,	TRUE,	48*60*60,	UpdateIdcIpAddr,		SYSTEM_002},
		{_T("Ipaddr.sql"),		0,  0,	TRUE,	72*60*60,	UpdateLocalIpAddr,		SYSTEM_ALL-SYSTEM_SLEUTH},
		{_T("allIpaddr.sql"),	0,  0,	TRUE,	72*60*60,	UpdateAllIpAddr,		SYSTEM_ALL-SYSTEM_SLEUTH},
		{_T("httpupmatch.sql"),	0,  0,	TRUE,	7200,		UpdateHttpupSplit,		SYSTEM_ALL-SYSTEM_SLEUTH},
/*10*/	{_T("idchttpkey.sql"),	0,  0,	TRUE,	3600,		UpdateIDCHttpKey,		SYSTEM_002},
		{_T("idcsvrport.sql"),	0,  0,	TRUE,	3600,		UpdateIDCServerPort,	SYSTEM_002},
		{_T("taobaopass.sql"),	0,  0,	TRUE,	48*60*60,	UpdateTaobaoPwd,		SYSTEM_ALL},
		{_T("qqpas.sql"),		0,	0,	TRUE,	48*60*60,	UpdateOicqPwd,			SYSTEM_ALL},
		{_T("ImportantMan.sql"),0,  0,	TRUE,	3600,		UpdateWarnLimit,		SYSTEM_ALL-SYSTEM_SLEUTH},
		{_T("warnlevel.sql"),	0,  0,	TRUE,	3600,		UpdateWarnLimit,		SYSTEM_ALL-SYSTEM_SLEUTH},
		{_T("warnrule.sql"),	0,  0,	TRUE,	3600,		UpdateWarnLimit,		SYSTEM_ALL-SYSTEM_SLEUTH},
/*17*/	{_T(""),				0,  0,			FALSE,	0,			NULL,					0},
		{_T(""),				0,  0,			FALSE,	0,			NULL,					0},
		{_T(""),				0,  0,			FALSE,	0,			NULL,					0},
		{_T(""),				0,  0,			FALSE,	0,			NULL,					0},
		{_T(""),				0,  0,			FALSE,	0,			NULL,					0},
		{_T(""),				0,  0,			FALSE,	0,			NULL,					0},
		{_T(""),				0,  0,			FALSE,	0,			NULL,					0},
		{_T(""),				0,  0,			FALSE,	0,			NULL,					0},
/*25*/	{_T(""),				0,  0,			FALSE,	0,			NULL,					0},
		{_T(""),				0,  0,			FALSE,	0,			NULL,					0},
		{_T(""),				0,  0,			FALSE,	0,			NULL,					0},
		{_T(""),				0,  0,			FALSE,	0,			NULL,					0},
		{_T(""),				0,  0,			FALSE,	0,			NULL,					0},
		{_T(""),				0,  0,			FALSE,	0,			NULL,					0},
/*31*/	{_T(""),				0,  0,			FALSE,	0,			NULL,					0},
};
ParaFileInfo* g_pclimitinfo = NULL;		// 多个Climit表的信息数组

CString   __stdcall  GetAsDataInfo (
				const char *  IP ,
				const char *virtruldir , 
				CString strContent,
				unsigned int intPort,
				BOOL bIsEncrypt)
{
	// open up the Internet
	//_bstr_t bstr_IP(IP, TRUE); 
	CString strvirtruldir=virtruldir;
	CString strReturn="";
	//char * strReturn;
	CString ContentType="Content-Type: text/html";
	//
	WCHAR BSTRIP[30];
	WCHAR BSTRvirtruldir[600];
	memset(BSTRIP,0,30*2);
	memset(BSTRvirtruldir,0,600*2);

	MultiByteToWideChar(CP_ACP,0,IP,strlen(IP)+1,BSTRIP,strlen(IP)+1);
	MultiByteToWideChar(CP_ACP,0,virtruldir,strlen(virtruldir)+1,BSTRvirtruldir,strlen(virtruldir)+1);

	DWORD dwTotalSize	= 0;
	DWORD dwTotalMatch	= 0;
	DWORD dwSize		= 0;
	DWORD dwDownloaded	= 0;
	LPVOID lpOutBuffer	= NULL;
	DWORD dwBytesWritten= 0;
	LPSTR pszOutBuffer	= NULL;

	CString strSeq;
	BOOL bOK = TRUE;
	
	HINTERNET  hSession = NULL, 
		hConnect = NULL,
		hRequest = NULL;
	
	// Use WinHttpOpen to obtain a session handle.
	if(bIsEncrypt)
	{
		strSeq.Format("WinHttpOpen 01");
		hSession = WinHttpOpen( L"IMW WinHTTP AS",  
			WINHTTP_ACCESS_TYPE_NO_PROXY,
			WINHTTP_NO_PROXY_NAME, 
			WINHTTP_NO_PROXY_BYPASS, 0);//WINHTTP_FLAG_ASYNC
	}
	else
	{
		strSeq.Format("WinHttpOpen 02");
		hSession = WinHttpOpen( L"Mozilla/4.0(Compatible win32; MSIE)",  
			WINHTTP_ACCESS_TYPE_NO_PROXY,
			WINHTTP_NO_PROXY_NAME, 
			WINHTTP_NO_PROXY_BYPASS, 0);//WINHTTP_FLAG_ASYNC
	}
	
	WinHttpSetTimeouts(hSession,500,1000,2000,30000);
	// Specify an HTTP server.
	WORD port=intPort;
	if (hSession)
	{
		strSeq.Format("WinHttpConnect");
		hConnect = WinHttpConnect( 
			hSession, BSTRIP  ,//L"www.microsoft.com",
			port, 0);//INTERNET_DEFAULT_HTTPS_PORT
	}

	// Create an HTTP request handle.
	if (hConnect)
	{
		strSeq.Format("WinHttpOpenRequest");
		hRequest = WinHttpOpenRequest( hConnect, L"POST", BSTRvirtruldir,
			NULL,WINHTTP_NO_REFERER, 
			WINHTTP_DEFAULT_ACCEPT_TYPES, 
			0);
	}
	
	if( hRequest)
	{
		strSeq.Format("WinHttpAddRequestHeaders 01");
		bOK = WinHttpAddRequestHeaders( hRequest, 
			L"Content-Type: text/html;charset=GB2312",
			-1,
			WINHTTP_ADDREQ_FLAG_ADD );
	}
	
	if( bOK )
	{
		strSeq.Format("WinHttpAddRequestHeaders 02");
		bOK = WinHttpAddRequestHeaders( hRequest, 
			L"Accept-Language: zh-cn",
			-1,
			WINHTTP_ADDREQ_FLAG_ADD );	
	}
	
	if( bOK )
	{
		strSeq.Format("WinHttpAddRequestHeaders 03");
		bOK = WinHttpAddRequestHeaders( hRequest, 
			L"Connection: Close",
			-1,
			WINHTTP_ADDREQ_FLAG_ADD );	
	}
	
	// Send a request.
	char *pInBuf = NULL;
	int inLen = 0;
	unsigned char *outBuf = NULL;
	int lPwdLen2 = 0;
	long m_key[4];
	m_key[0] = 0x8d2834de;
	m_key[1] = 0xf7af2b31;
	m_key[2] = 0x516c0e14;
	m_key[3] = 0x0c48bdbf;
	//////////////////////////////////////////////////////////////////////////
	// 加密发出的数据
	if(bIsEncrypt)
	{
		pInBuf = NULL;
		inLen = strContent.GetLength();//strlen(strContent);
		pInBuf = new char[inLen+10];
		memcpy(pInBuf, strContent, inLen);
		
		outBuf = new unsigned char[inLen+1024];
		lPwdLen2 = inLen+1024;
		
		memset((char*)outBuf, 0, lPwdLen2);
		int aTst = qq_crypt(ENCRYPT,
			(unsigned char *)pInBuf,
			inLen, 
			(unsigned char *)m_key, 
			outBuf,
			&lPwdLen2);
		
		if ( bOK ) 
		{
			if(lPwdLen2 > 8192)
			{
				strSeq.Format("WinHttpSendRequest 01");
				bOK = WinHttpSendRequest( hRequest, 
					WINHTTP_NO_ADDITIONAL_HEADERS, 0,
					(void *)(outBuf),
					8192,//WINHTTP_NO_REQUEST_DATA, 0, 
					lPwdLen2, 0);

				DWORD aaabbb;
				int pos = 8192;
				while(pos < lPwdLen2 && bOK)
				{
					strSeq.Format("WinHttpWriteData");
					bOK = WinHttpWriteData( hRequest, 
						(void *)(outBuf+pos),
						//8192,//WINHTTP_NO_REQUEST_DATA, 0, 
						min(8192,(lPwdLen2-pos)),
						&aaabbb);
					if (bOK)
						pos += aaabbb;
				}
			}
			else
			{
				strSeq.Format("WinHttpSendRequest 02");
				bOK = WinHttpSendRequest( hRequest, 
					WINHTTP_NO_ADDITIONAL_HEADERS, 0,
					(void *)(outBuf),
					lPwdLen2,//WINHTTP_NO_REQUEST_DATA, 0, 
					lPwdLen2, 0);
			}
		}
		
		if(pInBuf != NULL)
			delete pInBuf;
		if(outBuf != NULL)
			delete outBuf;
	}
	else
	{
		if ( bOK ) 
		{
			strSeq.Format("WinHttpSendRequest 03");
			bOK = WinHttpSendRequest( hRequest, 
				WINHTTP_NO_ADDITIONAL_HEADERS, 0,
				(void *)(LPCTSTR)strContent,
				strlen(strContent),//WINHTTP_NO_REQUEST_DATA, 0, 
				strlen(strContent), 0);
		}
	}
	//////////////////////////////////////////////////////////////////////////

	if ( bOK )
	{
		strSeq.Format("WinHttpReceiveResponse");
		bOK = WinHttpReceiveResponse( hRequest, NULL);
	}
	
	//////////////////////////////////////////////////////////////////////////
	if ( bOK )
	{
		dwSize=0;
		char Length[100];
		memset(Length,0,100);
		
		strSeq.Format("WinHttpQueryHeaders");
		bOK = WinHttpQueryHeaders( hRequest, 
			WINHTTP_QUERY_CONTENT_LENGTH	,
			WINHTTP_HEADER_NAME_BY_INDEX, 
			NULL, &dwSize, 
			WINHTTP_NO_HEADER_INDEX);
//dwSize = 96;
		
		// Allocate memory for the buffer.
		//lpOutBuffer = new WCHAR[dwSize/sizeof(WCHAR)];
		bOK = WinHttpQueryHeaders( hRequest, 
			WINHTTP_QUERY_CONTENT_LENGTH	,
			WINHTTP_HEADER_NAME_BY_INDEX, 
			(LPVOID)Length, &dwSize, 
			WINHTTP_NO_HEADER_INDEX);
		
		CString value=CString((LPCWSTR)Length);
		dwTotalSize = atol((LPCTSTR)value);
		//DbgPrintLog("WinHttpQueryHeaders Get Len:%d", dwTotalSize);
	}
	
	if ( bOK )
	{
		strSeq.Format("new char[dwTotalSize+1]");
		pszOutBuffer = new char[dwTotalSize+1];
		if(pszOutBuffer == NULL)
		{
			strReturn = "分配内存失败";
//			DbgPrintLog("分配内存失败");
			bOK = FALSE;
		}
		else
		{
			ZeroMemory(pszOutBuffer, dwTotalSize+1);
			do 
			{
				// Check for available data.
				dwSize = 0;
				strSeq.Format("WinHttpQueryDataAvailable");
				bOK = WinHttpQueryDataAvailable( hRequest, &dwSize);
				if ( !bOK )
				{
//					DbgPrintLog("Error %u in WinHttpQueryDataAvailable.\n",GetLastError());
					break;
				}
				else
				{
					// Read the Data.
					strSeq.Format("WinHttpReadData");
					bOK = WinHttpReadData(hRequest, 
						(LPVOID)(pszOutBuffer+dwTotalMatch), 
						dwSize, &dwDownloaded);
					if ( !bOK )
					{
//						DbgPrintLog("Error %u in WinHttpReadData.\n", GetLastError());
						break;
					}
					else
					{
						//printf("match data %d\r\n", dwDownloaded);
						dwTotalMatch += dwDownloaded;
					}
				}
			} while (dwSize>0);
			//DbgPrintLog("WinHttpReadData Total Match:%d\r\n", dwTotalMatch);
		}
	}
	
	//////////////////////////////////////////////////////////////////////////
	// 解密返回的数据
	if(dwTotalMatch == dwTotalSize && dwTotalMatch > 0)
	{
		if(bIsEncrypt)
		{
			pInBuf = NULL;
			inLen = dwTotalSize;
			pInBuf = new char[inLen+10];
			memcpy(pInBuf, pszOutBuffer, inLen);
			
			outBuf = new unsigned char[inLen+1024];
			lPwdLen2 = inLen+1024;
			
			memset((char*)outBuf, 0, lPwdLen2);
			int aTst = qq_crypt(DECRYPT,
				(unsigned char *)pInBuf,
				inLen, 
				(unsigned char *)m_key, 
				outBuf,
				&lPwdLen2);
			if(aTst == 1)
			{
				char *pBuf = strReturn.GetBufferSetLength(lPwdLen2);
				memcpy(pBuf, outBuf, lPwdLen2);
				//strReturn = CString(outBuf);
			}
			else
				strReturn = "解密失败";
			
			if(pInBuf != NULL) delete pInBuf;
			if(outBuf != NULL) delete outBuf;
		}
		else
		{
			char *pBuf = strReturn.GetBufferSetLength(dwTotalMatch);
			memcpy(pBuf, pszOutBuffer, dwTotalMatch);
//			strReturn=pszOutBuffer;
		}
	}
	else
	{
		if(bOK == FALSE)
		{
			strReturn.Format("命令失败, 当前操作:%s, 错误号:%d", strSeq, GetLastError());
//			DbgPrintLog("命令失败, 当前操作:%s, 错误号:%d", strSeq, GetLastError());
		}
		else if(dwTotalMatch == 0)
		{
			strReturn = "命令返回, 但结果为空";
//			DbgPrintLog("未接收到数据");
		}
		else
		{
			strReturn.Format("数据接收不完整, %d/%d", dwTotalMatch, dwTotalSize);
//			DbgPrintLog("数据接收不完整, %d/%d", dwTotalMatch, dwTotalSize);
		}

	}

	if(pszOutBuffer)	delete []pszOutBuffer;
	// Close any open handles.
	if (hRequest) WinHttpCloseHandle(hRequest);
	if (hConnect) WinHttpCloseHandle(hConnect);
	if (hSession) WinHttpCloseHandle(hSession);

	return strReturn;
}

BOOL UpdateLocalIpAddr(LPCTSTR filename)
{
	return k_IpQueryTable.LoadLocalIPFromDbf(filename);
}

BOOL UpdateAllIpAddr(LPCTSTR filename)
{
	return k_IpQueryTable.LoadAllIPFromDbf(filename);
}

void ExitDCE(LPCTSTR filename)
{
	if (_access(filename, 0) == 0)
	{
		WriteLog("dce", "Error 文件%s删除失败!!", filename);
		InterlockedExchange((long*)&v_DceRunFlg, 0);
		FILE *pf = fopen(v_RootPath+"\\flg\\deldcepara.flg", "wb");
		if (pf == NULL)
			WriteLog("dce", "创建标志文件deldcepara.flg失败!");
		else
			fclose(pf);
		::Sleep(2000);
		exit(-2);
	}
}

//*************************************************
//	函数名称:		RbCopyFile
//	描述:			拷贝文件
//	输入参数:		RemoteHost 远程主机的IP
//					srcfile 源文件名(包含全路径)
//					dstfile 目的文件名(包含全路径)
//					srcftm	源文件的时间
//					bFailIfExists 如果目的文件存在时操作是否失败(含义等同于CopyFile中的同名参数)
//	输出参数:		
//	返回值:			拷贝成功返回TRUE, 否则返回FALSE
//	其它:			
//*************************************************
BOOL RbCopyFile(LPCTSTR RemoteHost, LPCTSTR srcfile, LPCTSTR dstfile, CString strContent, FILETIME srcftm)
{
	CString Res = GetAsDataInfo(
	RemoteHost,
	"RB_Build_AS",
	strContent,
	9000,		// 管理AS的端口
	strContent.GetLength());
	if (Res.IsEmpty())
	{
		WriteLog("dce", "从CS<%s>更新参数<%s>时返回空", RemoteHost, srcfile);
	}
	else if (Res.GetLength() <= 6)
	{
		WriteLog("dce", "从CS<%s>更新参数<%s>时返回数据太少<%s>", RemoteHost, srcfile, Res);
	}
	else if (memicmp((LPCTSTR)Res, "Error!", 6) == 0)
	{
		WriteLog("dce", "从CS<%s>更新参数<%s>时返回错误: %s", RemoteHost, srcfile, Res.Mid(6));
	}
	else if (memicmp((LPCTSTR)Res, "OK\r\n", 4) != 0)
	{
		WriteLog("dce", "从CS<%s>更新参数<%s>时返回格式错误: %s", RemoteHost, srcfile, Res.Left(40));
	}
	else
	{
		int ste = 1;
		BOOL isSucc = FALSE;
		int datalen = Res.GetLength();
		LPCTSTR tp = Res;
		tp += 4;
		if (memicmp(tp, "filelen=", 8) == 0)
		{
			ste = 2;
			int flen = atoi(tp+8);
			char* p = strstr(tp+8, "\r\nfiledata=");
			if (p != NULL)
			{
				ste = 3;
				if (p-tp+4+11+flen == datalen)
				{
					ste = 4;
					for (int i=0; i<3; i++)
					{
						if(_access(dstfile, 0) == 0)
						{
							ste = 5;
							if (!DeleteFile(dstfile))
							{
								::Sleep(1000);
								continue;
							}
						}
						ste = 6;
						CFile file;
						if (file.Open(dstfile, CFile::modeCreate|CFile::modeReadWrite))
						{
							ste = 7;
							file.Write(p+11, flen);
							ste = 8;
							// convert system time to local file time
//  							FILETIME ftm, utcftm;
// 							SYSTEMTIME sysTime;
// 							sysTime.wYear = (WORD)srcftm.GetYear();
// 							sysTime.wMonth = (WORD)srcftm.GetMonth();
// 							sysTime.wDay = (WORD)srcftm.GetDay();
// 							sysTime.wHour = (WORD)srcftm.GetHour();
// 							sysTime.wMinute = (WORD)srcftm.GetMinute();
// 							sysTime.wSecond = (WORD)srcftm.GetSecond();
// 							sysTime.wMilliseconds = 0;
// 
// 							if (SystemTimeToFileTime((LPSYSTEMTIME)&sysTime, &ftm))
// 							{
// 								ste = 9;
// 								// convert local file time to UTC file time
// 								if (LocalFileTimeToFileTime(&ftm, &utcftm))
// 								{
//									ste = 10;
									if (SetFileTime((HANDLE)file.m_hFile, &srcftm, &srcftm, &srcftm))
									{
										ste = 11;
										file.Close();
										return TRUE;
									}
// 								}
// 							}
							file.Close();
						}
						break;
					}
				}
			}
		}

		WriteLog("dce", "从CS<%s>更新参数<%s>失败: %d", RemoteHost, srcfile, ste);
	}
	return FALSE;
}

//*************************************************
//	函数名称:		RbCopyFile
//	描述:			拷贝文件,因一个表拷贝是需要拷贝两个文件，dbf，和表头文件frm，所以提取出，拷贝两次
//	输入参数:		RemoteHost 远程主机的IP
//					srcfile 源文件名(包含全路径)
//					dstfile 目的文件名(包含全路径)
//					srcftm	源文件的时间
//					bFailIfExists 如果目的文件存在时操作是否失败(含义等同于CopyFile中的同名参数)
//	输出参数:		
//	返回值:			拷贝成功返回TRUE, 否则返回FALSE
//	其它:			
//*************************************************
BOOL RbCopyFile(LPCTSTR RemoteHost, LPCTSTR srcfile, LPCTSTR dstfile, FILETIME srcftm, BOOL bFailIfExists = FALSE)
{
	if(RemoteHost[0] == 0)	// 本地路径
	{
		return CopyFile(srcfile, dstfile, bFailIfExists);
	}
	CString strContent;
	strContent.Format("funName=Getfile\r\nfilename=%s\r\n", srcfile);

	RbCopyFile(RemoteHost,srcfile,dstfile,strContent,srcftm);
// 	CString tmpsrcfile(srcfile);
// 	tmpsrcfile.Replace(".dbf",".frm");
// 	CString tmpdstfile(dstfile);
// 	tmpdstfile.Replace(".dbf",".frm");
// 	RbCopyFile(RemoteHost,tmpsrcfile,tmpdstfile,strContent,srcftm);

	return FALSE;
}

// 更新参数(大系统,IDC,SZFJ,BJWB系统)
// 检查、拷贝DBF文件
BOOL SearchRemoteDbf(LPCTSTR svrip)
{
	BOOL ret = FALSE;
	BOOL copysucceed=FALSE;
	CString RemoteParaPath;
	CString cdxname;
	time_t now = time(0);
	HANDLE hfindfile;
	WIN32_FIND_DATA finddata;
	CString ParaPath(v_WorkPath+"\\para\\");
	int error = 0;
// 	if ((strcmp(svrip, v_localip) == 0) || (v_programtype == SYSTEM_SLEUTH))
 	if (v_programtype == SYSTEM_SLEUTH)
	{
		RemoteParaPath = ParaPath;
		svrip = "";
	}
	else
	{
		RemoteParaPath.Format("\\\\%s\\d$\\netspy\\para\\", svrip);
	}

	CString LocalParaPath(v_WorkPath+"\\para\\dce\\");

	DWORD begtm = GetTickCount();
	hfindfile = FindFirstFile(RemoteParaPath+"*.*", &finddata);
	RemoteParaPath = ParaPath;
error = 1;
	if (hfindfile != INVALID_HANDLE_VALUE)
	{
		ret = TRUE;
		try
		{
			do
			{
				for (int i = 0; i < tablenum; i++)
				{
					if ((v_programtype & g_parainfo[i].systemtype)==0)
						continue;
error = 2;
					if (stricmp(finddata.cFileName, g_parainfo[i].fname) == 0)
					{
						FILETIME ft;
						memcpy(&ft, &finddata.ftLastWriteTime, sizeof(FILETIME));
						time_t tt = CTime(ft).GetTime();
						if ((g_parainfo[i].filetime<tt) ||
							((g_parainfo[i].filetime==tt) &&
							(abs(now-g_parainfo[i].updatetm)>=g_parainfo[i].update)))
						{
error =3;
							if (RbCopyFile(svrip, RemoteParaPath+finddata.cFileName, LocalParaPath+finddata.cFileName, finddata.ftLastWriteTime, FALSE))
							{
//								OutputDebugString(RemoteParaPath+finddata.cFileName);
								g_parainfo[i].updateflg = TRUE;
								g_parainfo[i].updatetm = time(0);
								copysucceed = TRUE;
							}
							else
							{
error =4;
								g_parainfo[i].filetime=0;
								WriteLog("dce", "Copy Parafile %s\\%s Failel!!", svrip, finddata.cFileName);
								::Sleep(100);
								DeleteFile(LocalParaPath+finddata.cFileName);
								copysucceed=FALSE;
							}
							DWORD nowtm = GetTickCount();
							if ((nowtm - begtm)>60000)
							{
								CreateFlgFile();
								nowtm = begtm;
							}
						}
						break;
					}
				}
error =5;
				if ((i == tablenum) && (tablenum < 32))
				{
					// 没有找到表的对应信息
					int len = strlen(finddata.cFileName);
					if ((len > 10) // "climit.dbf"
						&& (stricmp(finddata.cFileName+len-10, "climit.sql") == 0))
					{
error =6;
						// 新的案件线索表
						g_parainfo[tablenum].fname = tablefilearr.GetAt(tablefilearr.Add(finddata.cFileName));
// 						g_parainfo[tablenum].filetime;
 						g_parainfo[tablenum].updatetm = now;
						g_parainfo[tablenum].updateflg = TRUE;
						g_parainfo[tablenum].update = g_parainfo[PARAMTABLECLIMIT].update;
						g_parainfo[tablenum].pUpdateFunc = g_parainfo[PARAMTABLECLIMIT].pUpdateFunc;
						g_parainfo[tablenum].systemtype = v_programtype;
						tablenum++;
						WriteLog("dce", "发现了第%d个新的案件线索表<%s>", tablenum-FIXTABLENUM, finddata.cFileName);
						FILETIME ft;
						memcpy(&ft, &finddata.ftLastWriteTime, sizeof(FILETIME));
						time_t tt = CTime(ft).GetTime();
error = 7;
						if (RbCopyFile(svrip, RemoteParaPath+finddata.cFileName, LocalParaPath+finddata.cFileName, finddata.ftLastWriteTime, FALSE))
						{
//							OutputDebugString(RemoteParaPath+finddata.cFileName);
							g_parainfo[tablenum].filetime = tt+1;
							g_parainfo[tablenum].updateflg = TRUE;
							g_parainfo[tablenum].updatetm = time(0);
							copysucceed = TRUE;
						}
						else
						{
							g_parainfo[tablenum].filetime=0;
							WriteLog("dce", "Copy Parafile %s\\%s Failel!!", svrip, finddata.cFileName);
							::Sleep(100);
							DeleteFile(LocalParaPath+finddata.cFileName);
							copysucceed=FALSE;
						}
error =8;
						DWORD nowtm = GetTickCount();
						if ((nowtm - begtm)>60000)
						{
							CreateFlgFile();
							nowtm = begtm;
						}
					}
				}
			}while(FindNextFile(hfindfile, &finddata));
		}
		catch (...)
		{
			WriteLog("dce", "SearchRemoteDbf(%s) Error!! = %d", svrip,error);
			ret = FALSE;
		}
		FindClose(hfindfile);
	}
	else
	{
		OutputDebugString("搜索文件失败!!");
		if ((GetTickCount()-begtm) >= 10000)
			WriteLog("dce", "目录<%s>访问失败!!", RemoteParaPath);
	}
	return (ret&&copysucceed);
}
// 更新参数(SLEUTH系统)
// 检查、拷贝DBF文件
BOOL SearchSleuthRemoteDbf(LPCTSTR svrip)
{
	BOOL ret = FALSE;
	BOOL copysucceed=FALSE;
	CString RemoteParaPath;
	CString cdxname;
	time_t now = time(0);
	HANDLE hfindfile;
	WIN32_FIND_DATA finddata;
	CString ParaPath(v_WorkPath+"\\para\\");
// 	if ((strcmp(svrip, v_localip) == 0) || (v_programtype == SYSTEM_SLEUTH))
 	if (v_programtype == SYSTEM_SLEUTH)
	{
		RemoteParaPath = ParaPath;
		svrip = "";
	}
	else
	{
		RemoteParaPath.Format("\\\\%s\\d$\\netspy\\para\\", svrip);
	}

	CString LocalParaPath(v_WorkPath+"\\para\\dce\\");

	DWORD begtm = GetTickCount();
	hfindfile = FindFirstFile(RemoteParaPath+"*.*", &finddata);
	RemoteParaPath = ParaPath;
	if (hfindfile != INVALID_HANDLE_VALUE)
	{
		ret = TRUE;
		try
		{
			do
			{
				for (int i = 0; i < tablenum; i++)
				{
					if ((v_programtype & g_sleuthparainfo[i].systemtype)==0)
						continue;
					if (stricmp(finddata.cFileName, g_sleuthparainfo[i].fname) == 0)
					{

						FILETIME ft;
						memcpy(&ft, &finddata.ftLastWriteTime, sizeof(FILETIME));
						time_t tt = CTime(ft).GetTime();
						if ((g_sleuthparainfo[i].filetime<tt) ||
							((g_sleuthparainfo[i].filetime==tt) &&
							(abs(now-g_sleuthparainfo[i].updatetm)>=g_sleuthparainfo[i].update)))
						{

							if (RbCopyFile(svrip, RemoteParaPath+finddata.cFileName, LocalParaPath+finddata.cFileName, finddata.ftLastWriteTime, FALSE))
							{

//								OutputDebugString(RemoteParaPath+finddata.cFileName);
								g_sleuthparainfo[i].updateflg = TRUE;
								g_sleuthparainfo[i].updatetm = time(0);
								copysucceed = TRUE;
							}
							else
							{
								g_sleuthparainfo[i].filetime=0;
								WriteLog("dce", "Copy Parafile %s\\%s Failel!!", svrip, finddata.cFileName);
								::Sleep(100);
								DeleteFile(LocalParaPath+finddata.cFileName);
								copysucceed=FALSE;
							}
							DWORD nowtm = GetTickCount();
							if ((nowtm - begtm)>60000)
							{
								CreateFlgFile();
								nowtm = begtm;
							}
						}
						break;
					}
				}
				if ((i == tablenum) && (tablenum < 32))
				{
					// 没有找到表的对应信息
					int len = strlen(finddata.cFileName);
					if ((len > 10) // "climit.dbf"
						&& (stricmp(finddata.cFileName+len-10, "climit.sql") == 0))
					{
						// 新的案件线索表
						g_sleuthparainfo[tablenum].fname = tablefilearr.GetAt(tablefilearr.Add(finddata.cFileName));
// 						g_sleuthparainfo[tablenum].filetime;
 						g_sleuthparainfo[tablenum].updatetm = now;
						g_sleuthparainfo[tablenum].updateflg = TRUE;
						g_sleuthparainfo[tablenum].update = g_sleuthparainfo[PARAMTABLECLIMIT].update;
						g_sleuthparainfo[tablenum].pUpdateFunc = g_sleuthparainfo[PARAMTABLECLIMIT].pUpdateFunc;
						g_sleuthparainfo[tablenum].systemtype = v_programtype;
						tablenum++;
						WriteLog("dce", "发现了第%d个新的案件线索表<%s>", tablenum-FIXTABLENUM, finddata.cFileName);
						FILETIME ft;
						memcpy(&ft, &finddata.ftLastWriteTime, sizeof(FILETIME));
						time_t tt = CTime(ft).GetTime();

						if (RbCopyFile(svrip, RemoteParaPath+finddata.cFileName, LocalParaPath+finddata.cFileName, finddata.ftLastWriteTime, FALSE))
						{
//							OutputDebugString(RemoteParaPath+finddata.cFileName);
							g_sleuthparainfo[tablenum].filetime = tt+1;
							g_sleuthparainfo[tablenum].updateflg = TRUE;
							g_sleuthparainfo[tablenum].updatetm = time(0);
							copysucceed = TRUE;
						}
						else
						{
							g_sleuthparainfo[tablenum].filetime=0;
							WriteLog("dce", "Copy Parafile %s\\%s Failel!!", svrip, finddata.cFileName);
							::Sleep(100);
							DeleteFile(LocalParaPath+finddata.cFileName);
							copysucceed=FALSE;
						}
						DWORD nowtm = GetTickCount();
						if ((nowtm - begtm)>60000)
						{
							CreateFlgFile();
							nowtm = begtm;
						}
					}
				}
			}while(FindNextFile(hfindfile, &finddata));
		}
		catch (...)
		{
			WriteLog("dce", "SearchRemoteDbf(%s) Error!!", svrip);
			ret = FALSE;
		}
		FindClose(hfindfile);
	}
	else
	{
		OutputDebugString("搜索文件失败!!");
		if ((GetTickCount()-begtm) >= 10000)
			WriteLog("dce", "目录<%s>访问失败!!", RemoteParaPath);
	}
	return (ret&&copysucceed);
}

BOOL CopyParaFile()
{
	// 检查本地的参数表
	CString LocalParaPath(v_WorkPath+"\\para\\dce\\");
	if (_access(LocalParaPath, 0) != 0)
	{
		CreateDir(LocalParaPath);
	}
	else
	{
		for (int i = 0; i < tablenum; i++)
		{
			if (g_parainfo[i].systemtype & v_programtype)
			{
				struct _stat fsta;
				if (!_stat(LocalParaPath+g_parainfo[i].fname, &fsta))
				{
					g_parainfo[i].filetime = fsta.st_mtime;
				}
				else
				{
					g_parainfo[i].filetime = 0;
					g_parainfo[i].updateflg = FALSE;
				}
			}
		}
	}


	// 检查远程的参数表
	for (int j=0; j<v_ParaServer.GetSize(); j++)
	{
// 		SearchRemoteDbf(v_ParaServer[j]);
	 	if (v_programtype == SYSTEM_SLEUTH)
		{
			if (SearchSleuthRemoteDbf(v_ParaServer[j]))
			{
				// 如果主CS存活就不检查辅CS
				break;
			}

		}
		else
		{
			if (SearchRemoteDbf(v_ParaServer[j]))
			{
				// 如果主CS存活就不检查辅CS
				break;
			}
		}
	}
	return TRUE;
}

BOOL UpdatePara()
{
// 	k_IpQueryTable.LoadLocalIPFromDbf("D:\\netspy\\database\\ipaddr.dbf");
// 	k_IpQueryTable.LoadAllIPFromDbf("D:\\netspy\\database\\allipaddr.dbf");
// 
// //	k_IpQueryTable.LoadAllIPFromDbf("");
// 	CString cssipaddr = k_IpQueryTable.QueryIpAddr("192.168.0.2");

//	OutputDebugString("开始更新参数!!");
 	if (v_programtype == SYSTEM_SLEUTH)
	{
		LPCTSTR flgname = v_WorkPath+"\\flg\\deldcepara.flg";
		CreateFlgFile(flgname);
		static LPCTSTR plocalparapath=v_WorkPath+"\\para\\dce\\";
		BOOL IsUpdataWarnAlarm = FALSE;
 		if (CopyParaFile())
		{
	//		OutputDebugString("开始读本地表!!");
			char dbfname[300];
			try
			{
				for (int i=0; i<tablenum; i++)
				{

					if ((g_sleuthparainfo[i].updateflg && (g_sleuthparainfo[i].systemtype & v_programtype)))
					{
	//					OutputDebugString(CString("开始读表")+g_parainfo[i].fname);

						sprintf(dbfname, "%s%s", plocalparapath, g_sleuthparainfo[i].fname);

						if (_access(dbfname, 0) == 0)
						{
	//						OutputDebugString(CString("开始读表")+g_parainfo[i].fname);
							CreateFlgFile();	// 创建心跳文件

							if (!g_sleuthparainfo[i].pUpdateFunc(dbfname))
							{
								WriteLog("dce", "更新表%s失败!", g_sleuthparainfo[i].fname);
								DeleteFile(dbfname);
								ExitDCE(dbfname);
							}
						}
						g_sleuthparainfo[i].updateflg = FALSE;
					}
				}

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

					str.Format("%sQQID(%d), QQPASS(%d),TAOBAOID(%d), TAOBAOPASS(%d), HttpupSplit(%d)", (CString)str,
						v_limitcount[0][OICQID], v_limitcount[0][OICQPASS], v_limitcount[0][TAOBAOID], v_limitcount[0][TAOBAOPASS],v_limitcount[0][HTTPUPSPLIT]);
					WriteLog("dce", str);
	//				memset(v_limitcount, 0, sizeof(v_limitcount))
					memcpy(v_limitcount[1], v_limitcount[0], sizeof(v_limitcount)/2);
				}
			}
			catch(...)
			{
				WriteLog("dce", "更新系统参数失败!!");
				return FALSE;
			}
		}
		DeleteFile(flgname);
		OutputDebugString("结束更新参数!!");

	}
	else
	{
		LPCTSTR flgname = g_DelDceParaFlg;
		CreateFlgFile(flgname);
		static LPCTSTR plocalparapath=v_WorkPath+\\para\\dce\\";
		BOOL IsUpdataWarnAlarm = FALSE;
 		if (CopyParaFile())
		{
	//		OutputDebugString("开始读本地表!!");
			char dbfname[300];
			try
			{
				for (int i=0; i<tablenum; i++)
				{
					if ((g_parainfo[i].updateflg && (g_parainfo[i].systemtype & v_programtype)))
					{
	//					OutputDebugString(CString("开始读表")+g_parainfo[i].fname);
						sprintf(dbfname, "%s%s", plocalparapath, g_parainfo[i].fname);
						if (_access(dbfname, 0) == 0)
						{
	//						OutputDebugString(CString("开始读表")+g_parainfo[i].fname);
							CreateFlgFile();	// 创建心跳文件
							if (!g_parainfo[i].pUpdateFunc(dbfname))
							{
								WriteLog("dce", "更新表%s失败!", g_parainfo[i].fname);
								DeleteFile(dbfname);
								ExitDCE(dbfname);
							}
						}
						g_parainfo[i].updateflg = FALSE;
					}
				}

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

					str.Format("%sQQID(%d), QQPASS(%d), TAOBAOID(%d), TAOBAOPASS(%d),HttpupSplit(%d)", (CString)str,
						v_limitcount[0][OICQID], v_limitcount[0][OICQPASS], v_limitcount[0][TAOBAOID], v_limitcount[0][TAOBAOPASS],v_limitcount[0][HTTPUPSPLIT]);
					WriteLog("dce", str);
	//				memset(v_limitcount, 0, sizeof(v_limitcount))
					memcpy(v_limitcount[1], v_limitcount[0], sizeof(v_limitcount)/2);
				}
			}
			catch(...)
			{
				WriteLog("dce", "更新系统参数失败!!");
				return FALSE;
			}
		}
		DeleteFile(flgname);
		OutputDebugString("结束更新参数!!");
	}

	return TRUE;
}

BOOL ReadCaseLimitDbf(LPCTSTR dbfname, 	AllDicMap& alldicmap, int bufid)
{
	// 打开数据库
// 	Code4	table_code;
// 	Data4	table_data;
// 	try
// 	{
// 		// 打开数据表
// 		table_code.autoOpen = 0;
// 		table_code.errOff = 1;
// 		table_code.errOpen = 0;
// 		table_code.readOnly = 1;
// 		table_data.open(table_code, dbfname);
// 		if (table_data.data == NULL)
// 		{
// 			table_code.closeAll();
// 			table_code.initUndo();
// 			WriteLog("dce", "文件%s打开失败!", dbfname);
// 			DeleteFile(dbfname);
// 			ExitDCE(dbfname);
// 			return FALSE;
// 		}
// 		
// 		// 关联字段
// 		Field4	fields[9];
// 		static LPCTSTR FIELDSNAME[9] =
// 		{
// 			_T("CASEID"), _T("OBJECTTYPE"), _T("OBJECTNAME"), 
// 			_T("ADDRNAME"), _T("ADDRTYPE"), _T("STATE"),
// 			_T("IFW"), _T("IFHANDSET"), _T("BUKONG")
// 		};
// 		// , , , , , , ifw, head, IFBUKONG
// 		for (int i = 0; i < 9; i++)
// 		{
// 			if (r4success != fields[i].init(table_data, FIELDSNAME[i]))
// 			{
// 				table_code.closeAll();
// 				table_code.initUndo();
// 				WriteLog("dce", "关联字段%s失败!", FIELDSNAME[i]);
// 				DeleteFile(dbfname);
// 				ExitDCE(dbfname);
// 				return FALSE;
// 			}
// 		}
// 
// 				// 遍历数据库
// 		LIMITDATA buf;
// 		buf.AlarmState = 0;
// 		CString   str, strtmp, strifkz;
// 		CString   strcaseid, strcaseobjtype, strcaseobjname, straddrtype, straddrname;
// 		BOOL      bhavekeyword = FALSE;
// 		BOOL	  bhavejzkeyword = FALSE;
// 
// 		int reccount = table_data.recCount();
// 		for (int rc = table_data.top(); rc == r4success; reccount--, rc=table_data.skip())
// 		{
// 			CString m,n;
// 			m = fields[5].str();
// 			n = fields[6].str();
// 
// 			crypt((char *)(LPCTSTR)m,m.GetLength());
// 			crypt((char *)(LPCTSTR)n,n.GetLength());
// 			m.TrimRight();
// 			n.TrimRight();
// 			
// #ifdef		_DEBUG
// 			
// #endif
// 			if (table_data.deleted() || (m.GetLength()<=0) || (m.GetAt(0) != '1'))
// 				continue;
// 			memset(&buf, 0, sizeof(buf));
// 
// 			straddrtype = fields[4].str();
// 			straddrtype.TrimRight();
// 			crypt((char *)(LPCTSTR)straddrtype,straddrtype.GetLength());
// 			buf.ltype= atol(straddrtype);		// 地址类型
// 
// 			if (v_programtype == SYSTEM_SLEUTH)
// 			{
// 				if (buf.ltype==18 || buf.ltype==14)
// 				{
// 					strifkz = fields[6].str();
// 					strifkz.TrimRight();
// 					crypt((char *)(LPCTSTR)strifkz,strifkz.GetLength());
// 					buf.ifexpand= atol(strifkz);	// 是否扩展
// 				}
// 			}
// 			
// 			// 增加地址参数
// 			strcaseid = fields[0].str();
// 			strcaseid.TrimRight();
// 			crypt((char *)(LPCTSTR)strcaseid,strcaseid.GetLength());
// 			buf.caseid= strcaseid;		// 案件ID
// 
// 			strcaseobjtype = fields[1].str();
// 			strcaseobjtype.TrimRight();
// 			crypt((char *)(LPCTSTR)strcaseobjtype,strcaseobjtype.GetLength());			
// 			buf.objtype= strcaseobjtype;		// 对象类型
// 
// 			strcaseobjname = fields[2].str();
// 			strcaseobjname.TrimRight();
// 			crypt((char *)(LPCTSTR)strcaseobjname,strcaseobjname.GetLength());			
// 			buf.objname= strcaseobjname;		// 对象名称
// 
// 			straddrname = fields[3].str();
// 			straddrname.TrimRight();
// 			crypt((char *)(LPCTSTR)straddrname,straddrname.GetLength());			
// 			buf.limit= straddrname;// 地址数据
// 			
// 			CString TmpAlarmState;
// 			TmpAlarmState = fields[6].str();		//页面告警
// 			TmpAlarmState.TrimRight();
// 			if (TmpAlarmState.CompareNoCase("1") == 0)
// 			{
// 				buf.AlarmState |= 0x10;
// 			}
// 
// 			TmpAlarmState = fields[7].str();		//手机告警
// 			TmpAlarmState.TrimRight();
// 			if (TmpAlarmState.CompareNoCase("1") == 0)
// 			{
// 				buf.AlarmState |= 0x20;
// 			}
// 
// 
// 			switch (buf.ltype)
// 			{
// 			case 18:		// 关键字
// 				{
// 					if (v_workmode!=1)
// 					{
// 						CaseInfo caseinfo(&buf);
// 						v_limitcount[0][LIMITTYPE_CASEKEYWORD]++;
// 
// 						AllDicInfo* lpalldicinfo = NULL;
// 						if (alldicmap.Lookup(straddrname,lpalldicinfo))
// 						{
// 							lpalldicinfo->m_caseinfoarry.Add(caseinfo);
// 						}
// 						else
// 						{
// 							lpalldicinfo = new AllDicInfo;
// 							lpalldicinfo->m_caseinfoarry.Add(caseinfo);
// 							alldicmap.SetAt(straddrname,lpalldicinfo);
// 						}
// 						v_CaseLimitPara.m_ridreallimit[bufid].SetAt(straddrname, caseinfo.CaseID);
// 					}
// 				}
// 				break;
// 			case 19:		// 技侦关键字
// 				{
// 					if (v_workmode!=1)
// 					{
// 						CaseInfo caseinfo(&buf);
// 						v_limitcount[0][LIMITTYPE_CASEJZWORD]++;
// 						AllDicInfo* lpalldicinfo = NULL;
// 						if (alldicmap.Lookup(straddrname,lpalldicinfo))
// 						{
// 							lpalldicinfo->m_caseinfoarry.Add(caseinfo);
// 						}
// 						else
// 						{
// 							lpalldicinfo = new AllDicInfo;
// 							lpalldicinfo->m_caseinfoarry.Add(caseinfo);
// 							alldicmap.SetAt(straddrname,lpalldicinfo);
// 						}
// 					}
// 				}
// 				break;
// 			case 14:		// URL
// 				{
// 					if (v_workmode!=1)
// 					{
// 						CaseInfo caseinfo(&buf);
//  						v_limitcount[0][LIMITTYPE_CASEURL]++;
// 						AllDicInfo* lpalldicinfo = NULL;
// 						straddrname.MakeLower();		// URL要转换成小写字母
// 						if (alldicmap.Lookup(straddrname,lpalldicinfo))
// 						{
// 							lpalldicinfo->m_caseinfoarry.Add(caseinfo);
// 						}
// 						else
// 						{
// 							lpalldicinfo = new AllDicInfo;
// 							lpalldicinfo->m_caseinfoarry.Add(caseinfo);
// 							alldicmap.SetAt(straddrname,lpalldicinfo);
// 						}
// 						v_CaseLimitPara.m_ridreallimit[bufid].SetAt(straddrname, caseinfo.CaseID);
// 					}
// 				}
// 				break;
// 			case 8:			// IP
// 				{
// 					char tmpipbuf[220];
// 					if ((strnicmp(buf.limit, "sip", 3)==0) ||
// 						(strnicmp(buf.limit, "dip", 3)==0))
// 					{
// 						// 去除ip的前缀(sip / dip)
// 						strcpy(tmpipbuf, buf.limit+3);
// 						buf.limit = tmpipbuf;
// 					}
// 					CString key = buf.limit;
// 					key.TrimRight();
// 					key.TrimLeft();
// 
// 					CaseInfoArray *tmpcaseinfoarr = NULL;
// 					CaseInfo *ptmpcs = NULL;
// 					ptmpcs = new CaseInfo(&buf);
// 					if (!v_CaseLimitPara.m_ipmap[bufid].Lookup(key, tmpcaseinfoarr))
// 					{
// 						tmpcaseinfoarr = new CaseInfoArray;
// 						tmpcaseinfoarr->Add(*ptmpcs);
// 						v_CaseLimitPara.m_ipmap[bufid].SetAt(key, tmpcaseinfoarr);
// 					}
// 					else
// 					{
// 						tmpcaseinfoarr->Add(*ptmpcs);
// 					}
// 					v_limitcount[0][LIMITTYPE_CASEIP]++;
// 
// 				}
// 				break;
// 			case 9:		// MAC
// 				{
// 					CString key = buf.limit;
// 					key.TrimRight();
// 					key.TrimLeft();
// 					CaseInfoArray *tmpcaseinfoarr = NULL;
// 					CaseInfo *ptmpcs = NULL;
// 					ptmpcs = new CaseInfo(&buf);
// 					key.MakeLower();
// 					if (!v_CaseLimitPara.m_macmap[bufid].Lookup(key, tmpcaseinfoarr))
// 					{
// 						tmpcaseinfoarr = new CaseInfoArray;
// 						tmpcaseinfoarr->Add(*ptmpcs);
// 						v_CaseLimitPara.m_macmap[bufid].SetAt(key, tmpcaseinfoarr);
// 					}
// 					else
// 					{
// 						tmpcaseinfoarr->Add(*ptmpcs);
// 					}
// 					v_limitcount[0][LIMITTYPE_CASEMAC]++;
// 
// 				}
// 				break;
// 			case 16:	// 账号
// 				{
// 					CaseInfoArray *tmpcaseinfoarr = NULL;
// 					CaseInfo *ptmpcs = NULL;
// 					ptmpcs = new CaseInfo(&buf);
// 					CString key = buf.limit;
// 					key.TrimRight();
// 					key.TrimLeft();
// 					key.MakeLower();
// 					if (!v_CaseLimitPara.m_phoneaccountmap[bufid].Lookup(key, tmpcaseinfoarr))
// 					{
// 						tmpcaseinfoarr = new CaseInfoArray;
// 						tmpcaseinfoarr->Add(*ptmpcs);
// 						v_CaseLimitPara.m_phoneaccountmap[bufid].SetAt(key, tmpcaseinfoarr);
// 					}
// 					else
// 					{
// 						tmpcaseinfoarr->Add(*ptmpcs);
// 					}
// 						
// 					v_limitcount[0][LIMITTYPE_CASEACCOUNT]++;
// 					
// 				}
// 				break;
// 			case 17:	// 电话
// 				{
// 					CaseInfoArray *tmpcaseinfoarr = NULL;
// 					CaseInfo *ptmpcs = NULL;
// 					ptmpcs = new CaseInfo(&buf);
// 					CString key = buf.limit;
// 					key.TrimRight();
// 					key.TrimLeft();
// 					key.MakeLower();
// 					if (!v_CaseLimitPara.m_phoneaccountmap[bufid].Lookup(key, tmpcaseinfoarr))
// 					{
// 						tmpcaseinfoarr = new CaseInfoArray;
// 						tmpcaseinfoarr->Add(*ptmpcs);
// 						v_CaseLimitPara.m_phoneaccountmap[bufid].SetAt(key, tmpcaseinfoarr);
// 					}
// 					else
// 					{
// 						tmpcaseinfoarr->Add(*ptmpcs);
// 					}
// 				}
// 				break;
// 			case 34:		// IP+URL
// 				{
// 					CString strlimit=buf.limit;
// 					strlimit.TrimRight();
// 					strlimit.TrimLeft();
// 					if(strlen(strlimit)<=16 || strlimit[15]!='_')
// 						break;
// 					CString key=strlimit.Mid(16);
//  					CaseInfo caseinfo(&buf);
// 
// 					AllDicInfo* lpalldicinfo = NULL;
// 					key.MakeLower();		// URL要转成小写字母
// 					if (alldicmap.Lookup(key, lpalldicinfo))
// 					{
// 
// 						lpalldicinfo->m_caseinfoarry.Add(caseinfo);
// 					}
// 					else
// 					{
// 						lpalldicinfo = new AllDicInfo;
// 						lpalldicinfo->m_caseinfoarry.Add(caseinfo);
// 						alldicmap.SetAt(key,lpalldicinfo);
// 					}
// 
// 				}
// 				break;
// 			case 35:		// IP+PORT
// 				{
// 					CString strlimit=buf.limit;
// 					strlimit.TrimRight();
// 					strlimit.TrimLeft();
// 					if(strlen(strlimit)<=16 || strlimit[15]!='_')
// 						break;
// //					strlimit.Replace('_',':');
// 					CString key=strlimit.Mid(16);
//  
// 					CaseInfoArray *tmpcaseinfoarr = NULL;
// 					CaseInfo *ptmpcs = NULL;
// 					ptmpcs = new CaseInfo(&buf);
// 					if (!v_CaseLimitPara.m_ipportmap[bufid].Lookup(strlimit, tmpcaseinfoarr))
// 					{
// 						tmpcaseinfoarr = new CaseInfoArray;
// 						tmpcaseinfoarr->Add(*ptmpcs);
// 						v_CaseLimitPara.m_ipportmap[bufid].SetAt(strlimit, tmpcaseinfoarr);
// 					}
// 					else
// 					{
// 						tmpcaseinfoarr->Add(*ptmpcs);
// 					}
// 				}
// 
// 				break;
// 			default:
// 				break;
// 			}
// 		}
// 
// 		table_code.closeAll();
// 		table_code.initUndo();
// 		if (reccount==0)
// 		{
// 			return TRUE;
// 		}
// 	}
// 	catch(...)
// 	{
// 		WriteLog("dce", "ReadCaseLimitDbf() Error!!");
// 		table_code.closeAll();
// 		table_code.initUndo();
// 		DeleteFile(dbfname);
// 		ExitDCE(dbfname);
// 	}
	CString importsql;
	importsql.Format("d:\\netspy\\dbclient -ujncfwlkjyxgongs dce <%s",dbfname);
	system(importsql);
 	ZWSQL_RES *result;
	ZWSQL zwsql; //数据库连接句柄

	try
	{
			// 打开数据表

		zwsql_init (&zwsql);
		if(!zwsql_real_connect(&zwsql,"localhost","jncfwlkjyxgongs",NULL,NULL,SQLPORT,NULL,0))
		{ 
			AfxMessageBox("数据库连接失败"); 
			return false;
		}
		char ch_query[200];
		int i = 1;
		int len = 0;

		// 		static LPCTSTR FIELDSNAME[9] =
	// 		{
	// 			_T("CASEID"), _T("OBJECTTYPE"), _T("OBJECTNAME"), 
	// 			_T("ADDRNAME"), _T("ADDRTYPE"), _T("STATE"),
	// 			_T("IFW"), _T("IFHANDSET"), _T("BUKONG")
	// 		};

		CString tablename;
		tablename.Format("%s",dbfname);
		int pos1,pos2;
		pos1 = tablename.ReverseFind('\\');
		pos2 = tablename.ReverseFind('.');
		tablename.Format("%s",tablename.Mid(pos1,pos2-pos1));
		len=sprintf(ch_query,"(select decode(CASEID,'inetsifter20021001'),decode(OBJECTTYPE,'inetsifter20021001'),\
			decode(OBJECTNAME,'inetsifter20021001'),decode(ADDRNAME,'inetsifter20021001'),decode(ADDRTYPE,'inetsifter20021001'),\
			decode(STATE,'inetsifter20021001'),decode(IFW,'inetsifter20021001'),decode(IFHANDSET,'inetsifter20021001'),\
			decode(BUKONG,'inetsifter20021001') from dce.%s)",tablename);
		if(zwsql_real_query(&zwsql,ch_query,len)!=0)
		{ 
//			zwsql_free_result(result);
			zwsql_close(&zwsql);
			return FALSE;
		}
		ZWSQL_ROW row;
		if(!(result=zwsql_store_result(&zwsql)))
		{ 
			zwsql_free_result(result);
			zwsql_close(&zwsql);
			return FALSE;
	//		AfxMessageBox("读取数据集失败"); 		
		}
		LIMITDATA buf;
		buf.AlarmState = 0;
		CString   str, strtmp, strifkz;
		CString   strcaseid, strcaseobjtype, strcaseobjname, straddrtype, straddrname;
		BOOL      bhavekeyword = FALSE;
		BOOL	  bhavejzkeyword = FALSE;
		int ii=0;
		while(row=zwsql_fetch_row(result))
		{
 			str.Format("%s",row[0]);
 			
			str.Format("%s",row[1]);
 			
 			str.Format("%s",row[2]);
			ii++;
			CString m,n;
			m.Format("%s",row[5]);
			n.Format("%s",row[6]);

// 			crypt((char *)(LPCTSTR)m,m.GetLength());
// 			crypt((char *)(LPCTSTR)n,n.GetLength());
// 			m.TrimRight();
// 			n.TrimRight();
			
#ifdef		_DEBUG
			
#endif
			if ((m.GetLength()<=0) || (m.GetAt(0) != '1'))
				continue;
			memset(&buf, 0, sizeof(buf));

			straddrtype.Format("%s",row[4]);
			straddrtype.TrimRight();
//			crypt((char *)(LPCTSTR)straddrtype,straddrtype.GetLength());
			buf.ltype= atol(straddrtype);		// 地址类型

			if (v_programtype == SYSTEM_SLEUTH)
			{
				if (buf.ltype==18 || buf.ltype==14)
				{
					strifkz.Format("%s",row[6]);
//					strifkz.TrimRight();
//					crypt((char *)(LPCTSTR)strifkz,strifkz.GetLength());
					buf.ifexpand= atol(strifkz);	// 是否扩展
				}
			}
			
			// 增加地址参数
			strcaseid.Format("%s",row[0]);
// 			strcaseid.TrimRight();
// 			crypt((char *)(LPCTSTR)strcaseid,strcaseid.GetLength());
			buf.caseid= strcaseid;		// 案件ID

			strcaseobjtype.Format("%s",row[1]);
// 			strcaseobjtype.TrimRight();
// 			crypt((char *)(LPCTSTR)strcaseobjtype,strcaseobjtype.GetLength());			
			buf.objtype= strcaseobjtype;		// 对象类型

			strcaseobjname.Format("%s",row[2]);
// 			strcaseobjname.TrimRight();
// 			crypt((char *)(LPCTSTR)strcaseobjname,strcaseobjname.GetLength());			
			buf.objname= strcaseobjname;		// 对象名称

			straddrname.Format("%s",row[3]);
// 			straddrname.TrimRight();
// 			crypt((char *)(LPCTSTR)straddrname,straddrname.GetLength());			
			buf.limit= straddrname;// 地址数据
			
			CString TmpAlarmState;
			TmpAlarmState.Format("%s",row[6]);		//页面告警
			TmpAlarmState.TrimRight();
			if (TmpAlarmState.CompareNoCase("1") == 0)
			{
				buf.AlarmState |= 0x10;
			}

			TmpAlarmState.Format("%s",row[7]);		//手机告警
			TmpAlarmState.TrimRight();
			if (TmpAlarmState.CompareNoCase("1") == 0)
			{
				buf.AlarmState |= 0x20;
			}


			switch (buf.ltype)
			{
			case 18:		// 关键字
				{
					if (v_workmode!=1)
					{
						CaseInfo caseinfo(&buf);
						v_limitcount[0][LIMITTYPE_CASEKEYWORD]++;

						AllDicInfo* lpalldicinfo = NULL;
						if (alldicmap.Lookup(straddrname,lpalldicinfo))
						{
							lpalldicinfo->m_caseinfoarry.Add(caseinfo);
						}
						else
						{
							lpalldicinfo = new AllDicInfo;
							lpalldicinfo->m_caseinfoarry.Add(caseinfo);
							alldicmap.SetAt(straddrname,lpalldicinfo);
						}
						v_CaseLimitPara.m_ridreallimit[bufid].SetAt(straddrname, caseinfo.CaseID);
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
						if (alldicmap.Lookup(straddrname,lpalldicinfo))
						{
							lpalldicinfo->m_caseinfoarry.Add(caseinfo);
						}
						else
						{
							lpalldicinfo = new AllDicInfo;
							lpalldicinfo->m_caseinfoarry.Add(caseinfo);
							alldicmap.SetAt(straddrname,lpalldicinfo);
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
						if (alldicmap.Lookup(straddrname,lpalldicinfo))
						{
							lpalldicinfo->m_caseinfoarry.Add(caseinfo);
						}
						else
						{
							lpalldicinfo = new AllDicInfo;
							lpalldicinfo->m_caseinfoarry.Add(caseinfo);
							alldicmap.SetAt(straddrname,lpalldicinfo);
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
					if (alldicmap.Lookup(key, lpalldicinfo))
					{

						lpalldicinfo->m_caseinfoarry.Add(caseinfo);
					}
					else
					{
						lpalldicinfo = new AllDicInfo;
						lpalldicinfo->m_caseinfoarry.Add(caseinfo);
						alldicmap.SetAt(key,lpalldicinfo);
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
			default:
				break;
			}
		}
		zwsql_free_result(result);
		zwsql_close(&zwsql);
		return TRUE;
// 		if (reccount==0)
// 		{
// 			return TRUE;
// 		}
	}
	catch(...)
	{
		WriteLog("dce", "ReadCaseLimitDbf() Error!!");
// 		table_code.closeAll();
// 		table_code.initUndo();
		zwsql_free_result(result);
		zwsql_close(&zwsql);
		DeleteFile(dbfname);
		ExitDCE(dbfname);
	}
	return FALSE;
}

BOOL UpdateWarnLimit(LPCTSTR filename)
{
	// 案件线索表和预警线索表要一起更新
	return UpdateCaseLimit(filename);
}

BOOL UpdateCaseLimit(LPCTSTR filename)
{
	if (_access(filename,0) != 0)
	{
		WriteLog("dce","文件 %s 不存在!");
		return FALSE;
	}

	// 案件线索表和预警线索表要一起更新
	// 清除更新表信息的更新标志
	try
	{
		CStringArray climitarr;		// 案件线索表的数组
		for (int n=0; n<tablenum; n++)
		{
			if (g_parainfo[n].pUpdateFunc == UpdateCaseLimit)
			{
				g_parainfo[n].updateflg = FALSE;
				CString str;
				str.Format("d:\\netspy\\para\\dce\\%s", g_parainfo[n].fname);
				if (_access(str, 0) == 0)
				{
					// DBF文件存在就放到数组中
					climitarr.Add(str);
				}
			}
			else if (g_parainfo[n].pUpdateFunc == UpdateWarnLimit)
			{
				g_parainfo[n].updateflg = FALSE;
			}
		}
	
		// 清空原缓冲区
		ASSERT(v_CaseLimitPara.m_readflg==0 || v_CaseLimitPara.m_readflg==1);
		long	bufid = (v_CaseLimitPara.m_readflg==0)? 1 : 0;
		memset(v_limitcount[0], 0, (LIMITTYPE_HTTPWORD-LIMITTYPE_CASEIP)*4);
		memset(v_limitcount[0], 0, (LIMITTYPE_CASEJZWORD+1)*4);

		ClearCaseMap(&v_CaseLimitPara.m_ipmap[bufid]);
		ClearCaseMap(&v_CaseLimitPara.m_macmap[bufid]);
		ClearCaseMap(&v_CaseLimitPara.m_phoneaccountmap[bufid]);

		v_CaseLimitPara.m_ridreallimit[bufid].RemoveAll();
//		v_CaseLimitPara.m_vlanidMap[bufid].RemoveAll();

		v_CaseLimitPara.m_urllist[bufid].clear();
		v_CaseLimitPara.m_keyworddic[bufid].ClearData();


		AllDicMap alldicmap;

		// 读案件线索表
		for (int i=0; i<climitarr.GetSize(); i++)
		{
			ReadCaseLimitDbf(climitarr[i], alldicmap, bufid);
		}

		//更新预警的关键字字典，跟案件关键字放到一个字典中
		UpdateIndexRule(&alldicmap, bufid);
		UpdateDictionary(&alldicmap, bufid);
		
		InterlockedExchange(&v_CaseLimitPara.m_readflg, bufid);

		return TRUE;
	}
	catch(...)
	{
		WriteLog("dce", "UpdateCaseLimit() Error!!");
	}
	return FALSE;
}

BOOL UpdateDictionary(AllDicMap* alldicmap,int bufid)
{
	AllDicInfo* lpalldicinfo;
	POSITION pos = alldicmap->GetStartPosition();
	CString mapkey;
	BOOL HaveKey = FALSE;
	while (pos != NULL)
	{
		alldicmap->GetNextAssoc(pos,mapkey,lpalldicinfo);
		Vocable temp;
		temp.m_Vocable.Format("%s",mapkey);
		temp.m_pUserData = (VOID*)lpalldicinfo;
		v_CaseLimitPara.m_keyworddic[bufid].m_VocableList.push_front(temp);
		HaveKey = TRUE;
	}
	if (HaveKey)
	{
		v_CaseLimitPara.m_keyworddic[bufid].IniKeyBTree();
	}
	
	return TRUE;
}


BOOL UpdateIDCHttpKey(LPCTSTR filename)
{
	CString importsql;
	importsql.Format("d:\\netspy\\dbclient -ujncfwlkjyxgongs dce <%s",filename);
	system(importsql);

// 	Code4	table_code;
// 	Data4	table_data;
	ASSERT(v_IDCHttpKeyflag==0 || v_IDCHttpKeyflag==1);
	long	bufid = (v_IDCHttpKeyflag==0)? 1 : 0;
	ZWSQL_RES *result;
	ZWSQL zwsql; //数据库连接句柄

	try{

		// 清空原缓冲区
		v_IDCHttpKeyLilt[bufid].clear();

// 		table_code.autoOpen = 0;
// 		table_code.errOff = 1;
// 		table_code.errOpen = 0;
// 		table_code.readOnly = 1;
// 		table_data.open(table_code, filename);
// 		if (table_data.data == NULL)
// 		{
// 			table_code.closeAll();
// 			table_code.initUndo();
// 			WriteLog("dce", "文件%s打开失败!\n", filename);
// 			return FALSE;
// 		}
// 		
// 		// 关联字段
// 		Field4	fields[3];
// 		static LPCTSTR FIELDSNAME[3] =
// 		{
// 			_T("KEYWORD1"),	// 关键字值1
// 			_T("KEYWORD2"),	// 关键字值2
// 			_T("WEBTYPE"),	// 网站类型
// 		};
// 		for (int i = 0; i < 3; i++)
// 		{
// 			if (r4success != fields[i].init(table_data, FIELDSNAME[i]))
// 			{
// 				table_code.closeAll();
// 				table_code.initUndo();
// 				WriteLog("dce", "关联字段%s失败!\n", FIELDSNAME[i]);
// 				return FALSE;
// 			}
// 		}
		zwsql_init (&zwsql);
		if(!zwsql_real_connect(&zwsql,"localhost","jncfwlkjyxgongs",NULL,NULL,SQLPORT,NULL,0))
		{ 
			AfxMessageBox("数据库连接失败"); 
			return false;
		}
		char ch_query[200];
		int i = 1;
		int len = 0;


		len=sprintf(ch_query,"(select decode(KEYWORD1,'inetsifter20021001'),decode(KEYWORD2,'inetsifter20021001'),\
			decode(WEBTYPE,'inetsifter20021001') from dce.idchttpkey)");
		if(zwsql_real_query(&zwsql,ch_query,len)!=0)
		{ 
	//		AfxMessageBox("数据库中表格出错"); 
			zwsql_free_result(result);
			zwsql_close(&zwsql);
			return FALSE;
		}
		ZWSQL_ROW row;
		if(!(result=zwsql_store_result(&zwsql)))
		{ 
			zwsql_free_result(result);
			zwsql_close(&zwsql);
			return FALSE;
	//		AfxMessageBox("读取数据集失败"); 		
		}
		LIMITDATA buf;
		buf.AlarmState = 0;
		CString   str, strtmp, strifkz;
		CString   strcaseid, strcaseobjtype, strcaseobjname, straddrtype, straddrname;
		BOOL      bhavekeyword = FALSE;
		BOOL	  bhavejzkeyword = FALSE;
		int ii=0;

		CMapStringToPtr		filter_map;
		filter_map.InitHashTable(1123);
		void *p = 0;
		// 遍历数据库
//		table_data.select();
//		int reccount = table_data.recCount();
		CString keyword1, keyword2, webtype;
		v_limitcount[0][LIMITTYPE_IDCHTTPKEY]=0;

		while(row=zwsql_fetch_row(result))
		{


// 		}
// 		
// 		for (int rc=table_data.top(); rc==r4success; rc=table_data.skip(), reccount--)
// 		{
//			if (!table_data.deleted())
			{
				keyword1 = row[0];
				keyword1.TrimRight();
//				crypt((char*)(LPCTSTR)keyword1,keyword1.GetLength());
				keyword2 = row[1];
				keyword2.TrimRight();
//				crypt((char*)(LPCTSTR)keyword2,keyword2.GetLength());
				if (!keyword1.IsEmpty())
				{
					CString tmpkey;
					if (!keyword2.IsEmpty())
						tmpkey.Format("%s_%s", keyword1, keyword2);
					if (!filter_map.Lookup(tmpkey, p))
					{
						// 此关键字首次出现
						filter_map.SetAt(tmpkey, p);
						webtype = row[2];
						webtype.TrimRight();
//						crypt((char*)(LPCTSTR)webtype,webtype.GetLength());
						if(!webtype.IsEmpty())
						{
							IDCHttpKey zPushKey;
							zPushKey.keyword1.InitSearch(keyword1);
							if (keyword2.IsEmpty())
							{
								zPushKey.num = 1;
							}
							else
							{
								zPushKey.keyword2.InitSearch(keyword2);
								zPushKey.num = 2;
							}
							zPushKey.servertype = webtype;
							v_IDCHttpKeyLilt[bufid].push_back(zPushKey);
							v_limitcount[0][LIMITTYPE_IDCHTTPKEY]++;
						}
					}	// end if 该关键字出现过
				}
			}	// end if 记录是删除状态
		}

// 		table_code.closeAll();
// 		table_code.initUndo();
//		if (reccount==0)
		{
			zwsql_free_result(result);
			zwsql_close(&zwsql);

			InterlockedExchange(&v_IDCHttpKeyflag, bufid);
			return TRUE;
		}
	}catch(...){
		WriteLog("dce","UpdateIDCHttpKey Catch Error!");
		zwsql_free_result(result);
		zwsql_close(&zwsql);
	}

	return FALSE;
}

BOOL UpdateIndexKeyWord(LPCTSTR filename)
{
	if (_access(filename,0) != 0)
	{
		WriteLog("dce","文件 %s 不存在!");
		return FALSE;
	}
	CString importsql;
	importsql.Format("d:\\netspy\\dbclient -ujncfwlkjyxgongs dce <%s",filename);
	system(importsql);

int error =0;	
// 	Code4	table_code;
// 	Data4	table_data;
	ASSERT(v_IndexLimitPara.m_keywordreadflg==0 || v_IndexLimitPara.m_keywordreadflg==1);
	long	bufid = (v_IndexLimitPara.m_keywordreadflg==0)? 1 : 0;

	ZWSQL_RES *result;
	ZWSQL zwsql; //数据库连接句柄

	BOOL   bhavekeyword = FALSE;
	try
	{
error =1;
		// 清空原缓冲区
		v_IndexLimitPara.m_keywordlist[bufid].clear();
		v_IndexLimitPara.m_keyworddic[bufid].ClearData();
error =2;
		memset(v_limitcount[0]+LIMITTYPE_HTTPWORD, 0, (LIMITTYPE_INDEXIPURL-LIMITTYPE_HTTPWORD)*4);
// 		// 打开数据表
// 		table_code.autoOpen = 0;
// 		table_code.errOff = 1;
// 		table_code.errOpen = 0;
// 		table_code.readOnly = 1;
// 		table_data.open(table_code, filename);
// error =3;
// 		if (table_data.data == NULL)
// 		{
// 			table_code.closeAll();
// 			table_code.initUndo();
// 			WriteLog("dce", "文件%s打开失败!\n", filename);
// 			return FALSE;
// 		}
// 		
// 		// 关联字段
// 		Field4	fields[5];
// 		static LPCTSTR FIELDSNAME[5] =
// 		{
// 			_T("KEYWORDS1"),	// 关键字值1
// 			_T("KEYWORDS2"),	// 关键字值2
// 			_T("A_STATUS"),		//此关键字是否在预警中启用
// 			_T("BTIME"),
// 			_T("ETIME"),
// 		};
// error =4;		
// 		// , , , , , , ifw, head, IFBUKONG
// 		for (int i = 0; i < 5; i++)
// 		{
// 			if (r4success != fields[i].init(table_data, FIELDSNAME[i]))
// 			{
// 				table_code.closeAll();
// 				table_code.initUndo();
// 				WriteLog("dce", "关联字段%s失败!\n", FIELDSNAME[i]);
// 				return FALSE;
// 			}
// 		}
		zwsql_init (&zwsql);
		if(!zwsql_real_connect(&zwsql,"localhost","jncfwlkjyxgongs",NULL,NULL,SQLPORT,NULL,0))
		{ 
			AfxMessageBox("数据库连接失败"); 
			return false;
		}
		char ch_query[200];
		int i = 1;
		int len = 0;


		len=sprintf(ch_query,"(select decode(KEYWORDS1,'inetsifter20021001'),decode(KEYWORDS2,'inetsifter20021001'),\
			decode(A_STATUS,'inetsifter20021001'),decode(BTIME,'inetsifter20021001'),decode(ETIME,'inetsifter20021001')\
			from dce.idchttpkey)");
		if(zwsql_real_query(&zwsql,ch_query,len)!=0)
		{ 
	//		AfxMessageBox("数据库中表格出错"); 
			zwsql_free_result(result);
			zwsql_close(&zwsql);
			return FALSE;
		}
		ZWSQL_ROW row;
		if(!(result=zwsql_store_result(&zwsql)))
		{ 
			zwsql_free_result(result);
			zwsql_close(&zwsql);
			return FALSE;
	//		AfxMessageBox("读取数据集失败"); 		
		}
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
		// 遍历数据库
// 		table_data.select();
// 		int reccount = table_data.recCount();

		SYSTEMTIME nowtime;
		CString strtime;
		GetLocalTime(&nowtime);
		strtime.Format("%04d-%02d-%02d",nowtime.wYear, nowtime.wMonth, nowtime.wDay);
		CString begtime, endtime, keyword;
		v_limitcount[0][LIMITTYPE_HTTPWORD]=0;
error =6;
		while(row=zwsql_fetch_row(result))
		{
// 		for (int rc=table_data.top(); rc==r4success; rc=table_data.skip(), reccount--)
// 		{
// 			if (!table_data.deleted())
			{
				// 启用状态
				begtime = row[3];
				endtime = row[4];
				begtime.TrimRight();
				endtime.TrimRight();

// 				crypt((char *)(LPCTSTR)begtime,begtime.GetLength());
// 				crypt((char *)(LPCTSTR)endtime,endtime.GetLength());

//				begtime.TrimLeft();
//				begtime.TrimRight();
//				endtime.TrimLeft();
//				endtime.TrimRight();
				if (strcmp(strtime,begtime)>=0 && strcmp(endtime,strtime)>=0)
				{
					keyword = row[0];
					keyword.TrimRight();
//			    	crypt((char *)(LPCTSTR)keyword,keyword.GetLength());
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
					keyword = row[1];
					keyword.TrimRight();
//					crypt((char *)(LPCTSTR)keyword,keyword.GetLength());					
					if (!keyword.IsEmpty())
					{
error =10;
						if (!filter_map.Lookup(keyword, p))
						{
							filter_map.SetAt(keyword, p);
//							v_IndexLimitPara.m_keywordlist[bufid].push_back((LPCTSTR)keyword);
							Vocable temp;
							temp.m_Vocable.Format("%s",keyword);
							AllDicInfo* lpalldicinfo = new AllDicInfo;
							temp.m_pUserData = (VOID*)lpalldicinfo;

//							temp.Vhandle=2;
							v_IndexLimitPara.m_keyworddic[bufid].m_VocableList.push_front(temp);
							
							v_limitcount[0][LIMITTYPE_HTTPWORD]++;

							bhavekeyword = TRUE;
							WriteLog("dce","KeyWord:%s",keyword);
						}
					}
				}
			}
		}
error =11;
		if(bhavekeyword){
	//		v_IndexLimitPara.m_keyworddic[bufid].ClearData();
			v_IndexLimitPara.m_keyworddic[bufid].IniKeyBTree();
		}
error =12;		

		zwsql_free_result(result);
		zwsql_close(&zwsql);

// 		table_code.closeAll();
// 		table_code.initUndo();
// 		if (reccount==0)
		{
			InterlockedExchange(&v_IndexLimitPara.m_keywordreadflg, bufid);
			return TRUE;
		}
	}
	catch(...)
	{
		zwsql_free_result(result);
		zwsql_close(&zwsql);

// 		table_code.closeAll();
// 		table_code.initUndo();
		WriteLog("dce", "UpdateIndexKeyWord() Error!! =%d",error);
		CopyFile(filename,"d:\\netspy\\log\\keyword.dbf",false);
	}
	return FALSE;
}

BOOL UpdateIndexLimit(LPCTSTR filename)
{
	CString importsql;
	importsql.Format("d:\\netspy\\dbclient -ujncfwlkjyxgongs dce <%s",filename);
	system(importsql);

	ZWSQL_RES *result;
	ZWSQL zwsql; //数据库连接句柄

	// 打开数据库
// 	Code4	table_code;
// 	Data4	table_data;
	ASSERT(v_IndexLimitPara.m_limitreadflg==0 || v_IndexLimitPara.m_limitreadflg==1);
	long	bufid = (v_IndexLimitPara.m_limitreadflg==0)? 1 : 0;
	try
	{
		// 清空原缓冲区
		v_IndexLimitPara.m_ipurlmap[bufid].RemoveAll();
		memset(v_limitcount[0]+LIMITTYPE_INDEXIPURL, 0, (LIMITTYPE_RUBBISHIP-LIMITTYPE_INDEXIPURL)*4);
		memset(v_IndexLimitPara.m_port[bufid], 0, 65536);
		// 打开数据表
// 		table_code.autoOpen = 0;
// 		table_code.errOff = 1;
// 		table_code.errOpen = 0;
// 		table_code.readOnly = 1;
// 		table_data.open(table_code, filename);
// 		if (table_data.data == NULL)
// 		{
// 			table_code.closeAll();
// 			table_code.initUndo();
// 			WriteLog("dce", "文件%s打开失败!\n", filename);
// 			return FALSE;
// 		}
// 		
// 		// 关联字段
// 		Field4 type(table_data, "TYPE");
// 		Field4 addr(table_data, "ADDR");
// 		// 关联字段
// 		if (!type.isValid())
// 		{
// 			printf("关联字段%s失败!\n", "TYPE");
// 			table_code.closeAll();
// 			table_code.initUndo();
// 			return FALSE;
// 		}
// 		if (!addr.isValid())
// 		{
// 			printf("关联字段%s失败!\n", "ADDR");
// 			table_code.closeAll();
// 			table_code.initUndo();
// 			return FALSE;
// 		}
		
		// 遍历数据库
		v_limitcount[0][LIMITTYPE_INDEXIPURL]=0;
		v_limitcount[0][LIMITTYPE_INDEXPORT]=0;
		zwsql_init (&zwsql);
		if(!zwsql_real_connect(&zwsql,"localhost","jncfwlkjyxgongs",NULL,NULL,SQLPORT,NULL,0))
		{ 
			AfxMessageBox("数据库连接失败"); 
			return false;
		}
		char ch_query[200];
		int i = 1;
		int len = 0;


		len=sprintf(ch_query,"(select decode(TYPE,'inetsifter20021001'),decode(ADDR,'inetsifter20021001') from dce.indexlimit)");
		if(zwsql_real_query(&zwsql,ch_query,len)!=0)
		{ 
	//		AfxMessageBox("数据库中表格出错"); 
			zwsql_free_result(result);
			zwsql_close(&zwsql);
			return FALSE;
		}
		ZWSQL_ROW row;
		if(!(result=zwsql_store_result(&zwsql)))
		{ 
			zwsql_free_result(result);
			zwsql_close(&zwsql);
			return FALSE;
	//		AfxMessageBox("读取数据集失败"); 		
		}
		while(row=zwsql_fetch_row(result))
		{
// 		int reccount = table_data.recCount();
// 		for (int rc=table_data.top(); rc == r4success; rc=table_data.skip())
//		{
// 			reccount--;
// 			if (!table_data.deleted())
			{
//				long iType = (long)type;
				char tmp_is_stop[10];
//				str_trim_crypt_cpy(tmp_is_stop,(char*)type.str());
				sprintf(tmp_is_stop,row[0]);
				long iType = atol(tmp_is_stop);
				if(iType == 49)
				{
//					int iPort = (long)addr;
//					str_trim_crypt_cpy(tmp_is_stop,(char*)addr.str());
					sprintf(tmp_is_stop,row[1]);
					int iPort = atoi(tmp_is_stop);
					
					if (iPort<=0 || iPort>0xFFFF)
						continue;
					v_IndexLimitPara.m_port[bufid][iPort] = 0x55;
					v_limitcount[0][LIMITTYPE_INDEXPORT]++;
				}
				else if(iType == 50)
				{
					LIMITDATA buf;
					memset(&buf, 0, sizeof(LIMITDATA));
					buf.ltype = 50;
//					buf.limit = addr.str();
					
					CString strlimit;// = buf.limit;
// 					strlimit.TrimRight();
// 					strlimit.TrimLeft();
					strlimit.Format("%s",row[1]);
//					crypt((char*)(LPCTSTR)strlimit,strlimit.GetLength());
					if(strlimit.GetLength()<=16 || strlimit[15]!='\x80')
						continue;
					CString key=strlimit.Left(15);
					CString url=strlimit.Mid(16);
					IndexKeyWordList *purllist=NULL;
					if (!v_IndexLimitPara.m_ipurlmap[bufid].Lookup(key, purllist))
					{
						purllist = new IndexKeyWordList;
						v_IndexLimitPara.m_ipurlmap[bufid].SetAt(key, purllist);
					}
					ASSERT(purllist!=NULL);
					url.MakeLower();
					purllist->push_back((LPCTSTR)url);
					v_limitcount[0][LIMITTYPE_INDEXIPURL]++;
				}
				else if(iType == 52)
				{
					LIMITDATA buf;
					memset(&buf, 0, sizeof(LIMITDATA));
					buf.ltype = 52;
//					buf.limit = addr.str();
					
					CString strlimit ;//= buf.limit;
// 					strlimit.TrimRight();
// 					strlimit.TrimLeft();
					strlimit.Format("%s",row[1]);
//					crypt((char*)(LPCTSTR)strlimit,strlimit.GetLength());
					if(strlimit.GetLength()<=16 || strlimit[15]!='\x80')
						continue;
					strlimit.Replace('\x80',':');
					CString key=strlimit.Left(15);
					CString url=strlimit.Mid(16);
					IndexKeyWordList *purllist=NULL;
					CString tmpstr;
					if (!v_IndexLimitPara.m_ipportmap[bufid].Lookup(strlimit, tmpstr))
					{
						purllist = new IndexKeyWordList;
						v_IndexLimitPara.m_ipportmap[bufid].SetAt(key, tmpstr);
					}
//					v_limitcount[0][LIMITTYPE_INDEXIPURL]++;
				}
			}
		}
		zwsql_free_result(result);
		zwsql_close(&zwsql);

// 		table_code.closeAll();
// 		table_code.initUndo();
// 		if (reccount==0)
		{
			InterlockedExchange(&v_IndexLimitPara.m_limitreadflg, bufid);
			return TRUE;
		}
	}
	catch(...)
	{
		zwsql_free_result(result);
		zwsql_close(&zwsql);

// 		table_code.closeAll();
// 		table_code.initUndo();
		WriteLog("dce", "UpdateIndexPara() Error!!");
		return FALSE;
	}
	return FALSE;
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

BOOL UpdateRubbish(LPCTSTR filename)
{
	CString importsql;
	importsql.Format("d:\\netspy\\dbclient -ujncfwlkjyxgongs dce <%s",filename);
	system(importsql);

	// 打开数据库
	ZWSQL_RES *result;
	ZWSQL zwsql; //数据库连接句柄

// 	Code4	table_code;
// 	Data4	table_data;
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
		// 打开数据表
// 		table_code.autoOpen = 0;
// 		table_code.errOff = 1;
// 		table_code.errOpen = 0;
// 		table_code.readOnly = 1;
// 		table_data.open(table_code, filename);
// 		if (table_data.data == NULL)
// 		{
// 			table_code.closeAll();
// 			table_code.initUndo();
// 			WriteLog("dce", "文件%s打开失败!", filename);
// 			return FALSE;
// 		}
// 		
// 		// 关联字段
// 		Field4	fields[2];
// 		// SHIELDTYPE, SHIELDNAME
// 		if (r4success != fields[0].init(table_data, "SHIELDTYPE"))
// 		{
// 			table_code.closeAll();
// 			table_code.initUndo();
// 			WriteLog("dce", "关联字段(SHIELDTYPE)失败!");
// 			return FALSE;
// 		}
// 		if (r4success != fields[1].init(table_data, "SHIELDADDR"))
// 		{
// 			table_code.closeAll();
// 			table_code.initUndo();
// 			WriteLog("dce", "关联字段(SHIELDADDR)失败!");
// 			return FALSE;
// 		}
		
		v_limitcount[0][LIMITTYPE_RUBBISHIP]=0;
		v_limitcount[0][LIMITTYPE_RUBBISHURL]=0;
		v_limitcount[0][LIMITTYPE_RUBBISHAAA]=0;
		v_limitcount[0][LIMITTYPE_RUBBISHMAIL]=0;
		// 增加缺省的垃圾信息
		AddDataToMap(14, "codecs.microsoft.com/isapi/ocget.dll", bufid);
		AddDataToMap(14, "activex.microsoft.com/objects/ocget.dll", bufid);

		zwsql_init (&zwsql);
		if(!zwsql_real_connect(&zwsql,"localhost","jncfwlkjyxgongs",NULL,NULL,SQLPORT,NULL,0))
		{ 
			AfxMessageBox("数据库连接失败"); 
			return false;
		}
		char ch_query[200];
		int i = 1;
		int len = 0;


		len=sprintf(ch_query,"(select decode(TYPE,'inetsifter20021001'),decode(ADDR,'inetsifter20021001') from dce.indexlimit)");
		if(zwsql_real_query(&zwsql,ch_query,len)!=0)
		{ 
	//		AfxMessageBox("数据库中表格出错"); 
			zwsql_free_result(result);
			zwsql_close(&zwsql);
			return FALSE;
		}
		ZWSQL_ROW row;
		if(!(result=zwsql_store_result(&zwsql)))
		{ 
			zwsql_free_result(result);
			zwsql_close(&zwsql);
			return FALSE;
	//		AfxMessageBox("读取数据集失败"); 		
		}
		while(row=zwsql_fetch_row(result))
		{
		// 遍历数据库
// 		table_data.select();
// 		int rc = table_data.top();
// 		int reccount = table_data.recCount();
// 		while (rc == r4success)
// 		{
// 			if (!table_data.deleted())
			{
				CString tmpstr = row[1];
				tmpstr.TrimRight();
//				crypt((char*)(LPCTSTR)tmpstr,tmpstr.GetLength());
				if (!tmpstr.IsEmpty())
				{
					AddDataToMap((long)row[0], tmpstr, bufid);
				}
			}
// 			rc = table_data.skip();
// 			reccount--;
		}
		
		zwsql_free_result(result);
		zwsql_close(&zwsql);

// 		table_code.closeAll();
// 		table_code.initUndo();
// 		if (reccount == 0)
		{
			InterlockedExchange(&v_RubbishPara.m_readflg, bufid);
			return TRUE;
		}
// 		else
// 		{
// 			WriteLog("dce", "表%s记录数不对%d", filename, reccount);
// 			return FALSE;
// 		}
	}
	catch(...)
	{
		zwsql_free_result(result);
		zwsql_close(&zwsql);

// 		table_code.closeAll();
// 		table_code.initUndo();
		WriteLog("dce", "LoadRubbishTable() Error!!");
		return FALSE;
	}
}

BOOL UpdateSystemPara(LPCTSTR filename)
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
	
	// 打开数据库
// 	Code4	table_code;
// 	Data4	table_data;
	CString importsql;
	importsql.Format("d:\\netspy\\dbclient -ujncfwlkjyxgongs dce <%s",filename);
	system(importsql);

	ZWSQL_RES *result;
	ZWSQL zwsql; //数据库连接句柄

	try
	{
// 		table_code.autoOpen = 0;
// 		table_code.errOff = 1;
// 		table_code.errOpen = 0;
// 		table_code.readOnly = 1;
// 		table_data.open(table_code, filename);
// 		if (table_data.data == NULL)
// 		{
// 			WriteLog("dce", "文件%s打开失败!", filename);
// 			table_code.closeAll();
// 			table_code.initUndo();
// 			return FALSE;
// 		}
// 		
// 		// 关联字段
// 		Field4	fields[2];
// 		// PARAID, PARAVALUE
// 		if (r4success != fields[0].init(table_data, "PARAID"))
// 		{
// 			WriteLog("dce", "关联字段(PARAID)失败!");
// 			table_code.closeAll();
// 			table_code.initUndo();
// 			return FALSE;
// 		}
// 		if (r4success != fields[1].init(table_data, "PARAVALUE"))
// 		{
// 			WriteLog("dce", "关联字段(PARAVALUE)失败!");
// 			table_code.closeAll();
// 			table_code.initUndo();
// 			return FALSE;
// 		}

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
		

		// 遍历数据库
// 		table_data.select();
// 		int rc = table_data.top();
		int paraid, Statidx, Indexidx, Caseidx, Mediaidx;
		char tmp_char[50];
		
		zwsql_init (&zwsql);
		if(!zwsql_real_connect(&zwsql,"localhost","jncfwlkjyxgongs",NULL,NULL,SQLPORT,NULL,0))
		{ 
			AfxMessageBox("数据库连接失败"); 
			return false;
		}
		char ch_query[200];
		int i = 1;
		int len = 0;


		len=sprintf(ch_query,"(select decode(PARAID,'inetsifter20021001'),decode(PARAVALUE,'inetsifter20021001') from dce.system)");
		if(zwsql_real_query(&zwsql,ch_query,len)!=0)
		{ 
	//		AfxMessageBox("数据库中表格出错"); 
//			zwsql_free_result(result);
			zwsql_close(&zwsql);
			return FALSE;
		}
		ZWSQL_ROW row;
		if(!(result=zwsql_store_result(&zwsql)))
		{ 
			zwsql_free_result(result);
			zwsql_close(&zwsql);
			return FALSE;
	//		AfxMessageBox("读取数据集失败"); 		
		}
		while(row=zwsql_fetch_row(result))
		{
// 		if(rc != r4success)
// 		{
// 			WriteLog("dce","UpdateSystemPara -->system.dbf is null,exit!!");			
// 			exit(-1);
// 		}
// 
// 		while (rc == r4success)
//		{   //			paraid = (long)fields[0];
//			str_trim_crypt_cpy(tmp_char,(char*)fields[0].str());
			sprintf(tmp_char,row[0]);
			paraid = atol(tmp_char);
			memset(tmp_char,0,50);
//			str_trim_crypt_cpy(tmp_char,(char*)fields[1].str());
			sprintf(tmp_char,row[1]);
			if(paraid == 26101)
			{
				int PrevMod = v_workmode;
				if(strncmp(tmp_char/*(char*)fields[1].str()*/, "回溯", 4) == 0)
					v_workmode = 3;
				else if(strncmp(tmp_char, "案件", 4) == 0)
					v_workmode = 2;
				else if(strncmp(tmp_char, "重点", 4) == 0)
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
				v_statIdc = atol(tmp_char);
			}
			
			if(paraid == 28101)	
			{
				// 音视频轮换时长 (10 - 60分钟)
				int val = atol(tmp_char);
				if (val < 10)
				{
					val = 10;
				}
				if (val > 60)
				{
					val = 60;
				}
				InterlockedExchange(&v_mediainterval, val * 60);		// 换算成秒
			}
			if(paraid == 28104)	// 视频数据的抽样比例
			{
				int val = atol(tmp_char);
				if (val <= 256)
				{
					InterlockedExchange(&v_mediasample, 1);		// 换算成秒
				}
				else
				{
					InterlockedExchange(&v_mediasample, val / 256);		// 换算成秒
				}
			}


			if(paraid >= 19101 && paraid <= 19107)	
			{
				// 网络活动历史
				Statidx = atol(tmp_char);
				switch(paraid)
				{
				case 19101:	// Httpup统计开关
					InterlockedExchange((long *)&v_statHttpup, Statidx); 
					break;
				case 19102:	// 即时消息QQ
					InterlockedExchange((long *)&v_statImsgQQ, Statidx);
					break;
				case 19103:	// 即时消息其它
					InterlockedExchange((long *)&v_statImsgOther, Statidx);
					break;
				case 19104:	// 邮件
					InterlockedExchange((long *)&v_statMail,  Statidx); 
					break;
				case 19105:	// 音视频
					InterlockedExchange((long *)&v_statMedia, Statidx);
					break;
				case 19106:	// 游戏
					InterlockedExchange((long *)&v_statGame,  Statidx);
					break;
				case 19107:	// 其它(除以上)
					InterlockedExchange((long *)&v_statOther, Statidx);
					break;
				}
			}

			if(paraid >= 11101 && paraid <= 11134)
			{
				Indexidx = paraid - 11101;
//				openindex[Indexidx] = (long)fields[1];
//				char tmp_openflg[20];
				CString tmp_openflg;
				tmp_openflg.Format("%s",row[1]);	
//				str_trim_crypt_cpy(tmp_openflg,(char*)fields[1].str());
				openindex[Indexidx] = atol(tmp_openflg);	
			}

			if(paraid >= 15101 && paraid <= 15134)
			{
				Indexidx = paraid - 15101;
//				openindex[Indexidx] = (long)fields[1];
//				char tmp_openflg[20];
				CString tmp_openflg;
				tmp_openflg.Format("%s",row[1]);	
//				str_trim_crypt_cpy(tmp_openflg,(char*)fields[1].str());
				openwarn[Indexidx] = atol(tmp_openflg);
				
			}

			if(paraid >= 13101 && paraid <= 13135)
			{
				Caseidx = paraid - 13101;
//				openindex[Indexidx] = (long)fields[1];
//				char tmp_openflg[20];
				CString tmp_openflg;
				tmp_openflg.Format("%s",row[1]);	
//				str_trim_crypt_cpy(tmp_openflg,(char*)fields[1].str());
				opencase[Caseidx] = atol(tmp_openflg);
				
			}

			if(paraid >= 11161 && paraid <= 11175)
			{
				Mediaidx = paraid - 11161;
//				openindex[Indexidx] = (long)fields[1];
//				char tmp_openflg[20];
				CString tmp_openflg;
				tmp_openflg.Format("%s",row[1]);	
//				str_trim_crypt_cpy(tmp_openflg,(char*)fields[1].str());
				openmedia[Mediaidx] = atol(tmp_openflg);
				
			}

			if(paraid >= 13161 && paraid <= 13175)
			{
				Mediaidx = paraid - 13161;
//				openindex[Indexidx] = (long)fields[1];
//				char tmp_openflg[20];
				CString tmp_openflg;
				tmp_openflg.Format("%s",row[1]);	
//				str_trim_crypt_cpy(tmp_openflg,(char*)fields[1].str());
				casemedia[Mediaidx] = atol(tmp_openflg);
				
			}
			
			if(paraid >= 15161 && paraid <= 15175)
			{
				Mediaidx = paraid - 15161;
//				openindex[Indexidx] = (long)fields[1];
//				char tmp_openflg[20];
				CString tmp_openflg;
				tmp_openflg.Format("%s",row[1]);	
//				str_trim_crypt_cpy(tmp_openflg,(char*)fields[1].str());
				warnmedia[Mediaidx] = atol(tmp_openflg);
				
			}

			
//			rc = table_data.skip();
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

// 		table_code.closeAll();
// 		table_code.initUndo();
		zwsql_free_result(result);
		zwsql_close(&zwsql);

		return TRUE;
	}
	catch(...)
	{
		WriteLog("dce", "更新系统参数表错误!!");
// 		table_code.closeAll();
// 		table_code.initUndo();
		zwsql_free_result(result);
		zwsql_close(&zwsql);

		return FALSE;
	}
}

BOOL UpdateLegalIP(LPCTSTR filename)
{
	CString importsql;
	importsql.Format("d:\\netspy\\dbclient -ujncfwlkjyxgongs dce <%s",filename);
	system(importsql);

	// 打开数据库
	ZWSQL_RES *result;
	ZWSQL zwsql; //数据库连接句柄

// 	Code4	table_code;
// 	Data4	table_data;
	try
	{
// 		table_code.autoOpen = 0;
// 		table_code.errOff = 1;
// 		table_code.errOpen = 0;
// 		table_code.readOnly = 1;
// 		table_data.open(table_code, filename);
// 		if (table_data.data == NULL)
// 		{
// 			WriteLog("dce", "文件%s打开失败!", filename);
// 			table_code.closeAll();
// 			table_code.initUndo();
// 			return FALSE;
// 		}
// 		
// 		// 关联字段
// 		Field4	fields[2];
// 		// PARAID, PARAVALUE
// 		if (r4success != fields[0].init(table_data, "BEGINIP"))
// 		{
// 			WriteLog("dce", "关联字段(BEGINIP)失败!");
// 			table_code.closeAll();
// 			table_code.initUndo();
// 			return FALSE;
// 		}
// 		if (r4success != fields[1].init(table_data, "ENDIP"))
// 		{
// 			WriteLog("dce", "关联字段(ENDIP)失败!");
// 			table_code.closeAll();
// 			table_code.initUndo();
// 			return FALSE;
// 		}
		
		// 遍历数据库
		CString s;
		unsigned char *tp = (unsigned char*)s.GetBufferSetLength(65536);
		memset(tp, 0, 65536);
		int ip_Ipcount=0;
		zwsql_init (&zwsql);
		if(!zwsql_real_connect(&zwsql,"localhost","jncfwlkjyxgongs",NULL,NULL,SQLPORT,NULL,0))
		{ 
			AfxMessageBox("数据库连接失败"); 
			return false;
		}
		char ch_query[200];
		int i = 1;
		int len = 0;


		len=sprintf(ch_query,"(select PARAID,PARAVALUE from dce.system)");
		if(zwsql_real_query(&zwsql,ch_query,len)!=0)
		{ 
	//		AfxMessageBox("数据库中表格出错"); 
//			zwsql_free_result(result);
			zwsql_close(&zwsql);
			return FALSE;
		}
		ZWSQL_ROW row;
		if(!(result=zwsql_store_result(&zwsql)))
		{ 
			zwsql_free_result(result);
			zwsql_close(&zwsql);
			return FALSE;
	//		AfxMessageBox("读取数据集失败"); 		
		}
		while(row=zwsql_fetch_row(result))
		{
// 		table_data.select();
// 		int rc = table_data.top();
// 		
// 		while (rc == r4success)
//		{
//			if (!table_data.deleted())
			{
				UINT begip, endip;
				if (sIPtouIP(row[0], begip) &&
					sIPtouIP(row[1], endip))
				{
					begip >>= 16;
					endip >>= 16;
					ASSERT(begip < 65536);
					ASSERT(endip < 65536);
					if (endip >= begip)
						memset(tp+begip, 1, endip-begip+1);
					ip_Ipcount++;
				}
//				rc = table_data.skip();
			}
		}
		zwsql_free_result(result);
		zwsql_close(&zwsql);

// 		table_code.closeAll();
// 		table_code.initUndo();
		if(ip_Ipcount==0)
		{
			::Sleep(0);
			memset(v_pLeaglIP,1,65536);
			return TRUE;
		}
		::Sleep(0);
		memcpy(v_pLeaglIP, tp, 65536);
		return TRUE;
	}
	catch(...)
	{
		zwsql_free_result(result);
		zwsql_close(&zwsql);

// 		table_code.closeAll();
// 		table_code.initUndo();
		return FALSE;
	}
}

BOOL UpdateIdcIpAddr(LPCTSTR filename)
{
	CString importsql;
	importsql.Format("d:\\netspy\\dbclient -ujncfwlkjyxgongs dce <%s",filename);
	system(importsql);

	// 打开数据库
	ZWSQL_RES *result;
	ZWSQL zwsql; //数据库连接句柄

// 	Code4	table_code;
// 	Data4	table_data;
	try
	{
// 		table_code.autoOpen = 0;
// 		table_code.errOff = 1;
// 		table_code.errOpen = 0;
// 		table_code.readOnly = 1;
// 		table_data.open(table_code, filename);
// 		if (table_data.data == NULL)
// 		{
// 			WriteLog("dce", "文件%s打开失败!", filename);
// 			table_code.closeAll();
// 			table_code.initUndo();
// 			return FALSE;
// 		}
// 		
// 		// 关联字段
// 		Field4	fields[3];
// 		// PARAID, PARAVALUE
// 		if (r4success != fields[0].init(table_data, "S_IPINT"))
// 		{
// 			WriteLog("dce", "关联字段(S_IPINT)失败!");
// 			table_code.closeAll();
// 			table_code.initUndo();
// 			return FALSE;
// 		}
// 		if (r4success != fields[1].init(table_data, "E_IPINT"))
// 		{
// 			WriteLog("dce", "关联字段(E_IPINT)失败!");
// 			table_code.closeAll();
// 			table_code.initUndo();
// 			return FALSE;
// 		}
// 		if (r4success != fields[2].init(table_data, "ISP"))
// 		{
// 			WriteLog("dce", "关联字段(ISP)失败!");
// 			table_code.closeAll();
// 			table_code.initUndo();
// 			return FALSE;
// 		}
// 		
// 		// 遍历数据库
// 		table_data.select();
// 		int rc = table_data.top();
// 		int ip_Ipcount=table_data.recCount();
		v_IdcIpLib.ClearAllIp();
		zwsql_init (&zwsql);
		if(!zwsql_real_connect(&zwsql,"localhost","jncfwlkjyxgongs",NULL,NULL,SQLPORT,NULL,0))
		{ 
//			AfxMessageBox("数据库连接失败"); 
			return false;
		}
		char ch_query[200];
		int i = 1;
		int len = 0;


		len=sprintf(ch_query,"(select PARAID,PARAVALUE from dce.system)");
		if(zwsql_real_query(&zwsql,ch_query,len)!=0)
		{ 
	//		AfxMessageBox("数据库中表格出错"); 
//			zwsql_free_result(result);
			zwsql_close(&zwsql);
			return FALSE;
		}
		ZWSQL_ROW row;
		if(!(result=zwsql_store_result(&zwsql)))
		{ 
			zwsql_free_result(result);
			zwsql_close(&zwsql);
			return FALSE;
	//		AfxMessageBox("读取数据集失败"); 		
		}
		while(row=zwsql_fetch_row(result))
		{
//		while (rc == r4success)
//		{
//			if (!table_data.deleted())
			{
				UINT begip, endip;
//				if (sIPtouIP(fields[0].str(), begip) &&
//					sIPtouIP(fields[1].str(), endip))
				begip = (long)row[0];
				endip = (long)row[1];
				{
					if (endip < begip)
					{
						DWORD t = endip;
						endip = begip;
						begip = t;
					}
					v_IdcIpLib.AddIdcIp(begip, endip, row[2]);
				}
			}
// 			rc = table_data.skip();
// 			ip_Ipcount--;
		}
		zwsql_free_result(result);
		zwsql_close(&zwsql);

// 		table_code.closeAll();
// 		table_code.initUndo();
// 		if(ip_Ipcount==0)
//		{
			v_IdcIpLib.Optimize();
			WriteLog("dce", "读入IDC IP Addr %d", v_IdcIpLib.GetIpCount());
			return TRUE;
// 		}
// 		else
// 		{
// 			v_IdcIpLib.ClearAllIp();
// 		}
	}
	catch(...)
	{
		zwsql_free_result(result);
		zwsql_close(&zwsql);

// 		table_code.closeAll();
// 		table_code.initUndo();
	}
	return FALSE;
}

BOOL UpdateIDCServerPort(LPCTSTR filename)
{
	CString importsql;
	importsql.Format("d:\\netspy\\dbclient -ujncfwlkjyxgongs dce <%s",filename);
	system(importsql);

	// 打开数据库
// 	Code4	table_code;
// 	Data4	table_data;
	ZWSQL_RES *result;
	ZWSQL zwsql; //数据库连接句柄

	long	bufid = (v_IDCSvrPort.readflg==0)? 1 : 0;
	try
	{
		// 清空原缓冲区
		v_IDCSvrPort.Clear(bufid);
		memset(v_limitcount[0]+LIMITTYPE_IDCSVRPORT, 0, 4);
		// 打开数据表
// 		table_code.autoOpen = 0;
// 		table_code.errOff = 1;
// 		table_code.errOpen = 0;
// 		table_code.readOnly = 1;
// 		table_data.open(table_code, "d:\\netspy\\Dictionary\\idcsvrport.dbf");
// 		if (table_data.data == NULL)
// 		{
// 			table_code.closeAll();
// 			table_code.initUndo();
// 			WriteLog("dce", "文件d:\\netspy\\Dictionary\\idcsvrport.dbf打开失败!\n");
// 			return FALSE;
// 		}
// 		
// 		// 关联字段
// 		// 关联字段
// 		Field4	fields[4];
// 		static LPCTSTR FIELDSNAME[4] =
// 		{
// 			_T("SVRPORT"),	// 开放端口
// 			_T("PRO"),		// 协议 TCP / UDP
// 			_T("SVRTYPE"),	// 网站类型
// 			_T("SUBTYPE"),	// 网站子类型
// 		};
// 
// 		for (int i = 0; i < 4; i++)
// 		{
// 			if (r4success != fields[i].init(table_data, FIELDSNAME[i]))
// 			{
// 				table_code.closeAll();
// 				table_code.initUndo();
// 				WriteLog("dce", "关联字段%s失败!\n", FIELDSNAME[i]);
// 				return FALSE;
// 			}
// 		}
		
		// 遍历数据库
//		int reccount = table_data.recCount();
		CString svrtype,  protype, subtype;
		zwsql_init (&zwsql);
		if(!zwsql_real_connect(&zwsql,"localhost","jncfwlkjyxgongs",NULL,NULL,SQLPORT,NULL,0))
		{ 
			AfxMessageBox("数据库连接失败"); 
			return false;
		}
		char ch_query[200];
		int i = 1;
		int len = 0;


		len=sprintf(ch_query,"(select SVRPORT,PRO,SVRTYPE,SUBTYPE from dce.system)");
		if(zwsql_real_query(&zwsql,ch_query,len)!=0)
		{ 
	//		AfxMessageBox("数据库中表格出错"); 
//			zwsql_free_result(result);
			zwsql_close(&zwsql);
			return FALSE;
		}
		ZWSQL_ROW row;
		if(!(result=zwsql_store_result(&zwsql)))
		{ 
			zwsql_free_result(result);
			zwsql_close(&zwsql);
			return FALSE;
	//		AfxMessageBox("读取数据集失败"); 		
		}
		while(row=zwsql_fetch_row(result))
		{
//		for (int rc=table_data.top(); rc == r4success; rc=table_data.skip())
//		{
// 			reccount--;
// 			if (!table_data.deleted())
			{
				long port = (long)row[0];		// svrport
				if (port >= 0 && port < 65536)
				{
					svrtype = row[2];			// 网站类型
					subtype = row[3];			// 网站子类型
					protype = row[1];			// 协议
					subtype.TrimRight();
					svrtype.TrimRight();
					protype.TrimRight();
					// SUBPRO\11PRO\1PORT\1ClassType
					CString tmpstr;
					tmpstr.Format("%s\1%s\1%05u\1%s", subtype, svrtype, port, subtype);
					if (protype.CompareNoCase("tcp") == 0)
					{
						v_IDCSvrPort.AddPort(port, TRUE, tmpstr);
						v_limitcount[0][LIMITTYPE_IDCSVRPORT]++;
					}
					else if(protype.CompareNoCase("udp") == 0)
					{
						v_IDCSvrPort.AddPort(port, FALSE, tmpstr);
						v_limitcount[0][LIMITTYPE_IDCSVRPORT]++;
					}
				}
			}
		}
		zwsql_free_result(result);
		zwsql_close(&zwsql);

// 		table_code.closeAll();
// 		table_code.initUndo();
// 		if (reccount==0)
		{
			InterlockedExchange(&v_IDCSvrPort.readflg, bufid);
			return TRUE;
		}
	}
	catch(...)
	{
		zwsql_free_result(result);
		zwsql_close(&zwsql);

// 		table_code.closeAll();
// 		table_code.initUndo();
		WriteLog("dce", "UpdateIDCSvrPort() Error!!");
		return FALSE;
	}
	return FALSE;
}

BOOL UpdateOicqPwd(LPCTSTR filename)
{
	
	// 判断并解压RAR文件
	if (stricmp(filename, "d:\\netspy\\para\\dce\\qqpas.rar") == 0)
	{
		if (!UnRarDbf(filename, "d:\\netspy\\para\\dce\\rar", 300))
		{
			return FALSE;
		}
		filename = "e:\\netspy\\para\\dce\\rar\\qqpas.sql";
		
	}
	CString importsql;
	importsql.Format("d:\\netspy\\dbclient -ujncfwlkjyxgongs dce <%s",filename);
	system(importsql);

	ZWSQL_RES *result;
	ZWSQL zwsql; //数据库连接句柄

// 	Code4	table_code;
// 	Data4	table_data;
	ASSERT(v_OicqPassWord.m_readflg==0 || v_OicqPassWord.m_readflg==1);
	long	bufid = (v_OicqPassWord.m_readflg==0)? 1 : 0;
	try
	{
		// 清空原缓冲区
		v_OicqPassWord.m_qqpwdmap[bufid].RemoveAll();
		memset(v_limitcount[0]+OICQID, 0, (HTTPUPSPLIT-OICQID)*4);
		// 打开数据表
// 		table_code.autoOpen = 0;
// 		table_code.errOff = 1;
// 		table_code.errOpen = 0;
// 		table_code.readOnly = 1;
// 		table_data.open(table_code, filename);
// 		if (table_data.data == NULL)
// 		{
// 			table_code.closeAll();
// 			table_code.initUndo();
// 			WriteLog("dce", "文件%s打开失败!", filename);
// 			return FALSE;
// 		}
// 		
// 		// 关联字段
// 		Field4	fields[3];
// 		// SHIELDTYPE, SHIELDNAME
// 		if (r4success != fields[0].init(table_data, "QQID"))
// 		{
// 			table_code.closeAll();
// 			table_code.initUndo();
// 			WriteLog("dce", "关联字段(QQID)失败!");
// 			return FALSE;
// 		}
// 		if (r4success != fields[1].init(table_data, "PASS"))
// 		{
// 			table_code.closeAll();
// 			table_code.initUndo();
// 			WriteLog("dce", "关联字段(PASS)失败!");
// 			return FALSE;
// 		}
// 
// 		if (r4success != fields[2].init(table_data, "PASSTYPE"))
// 		{
// 			table_code.closeAll();
// 			table_code.initUndo();
// 			WriteLog("dce", "关联字段(PASSTYPE)失败!");
// 			return FALSE;
// 		}
		
		// 遍历数据库
		
//		int reccount = table_data.recCount();
		int qqcount = 0;
		int qqmd5count = 0;

				zwsql_init (&zwsql);
		if(!zwsql_real_connect(&zwsql,"localhost","jncfwlkjyxgongs",NULL,NULL,SQLPORT,NULL,0))
		{ 
			AfxMessageBox("数据库连接失败"); 
			return false;
		}
		char ch_query[200];
		int i = 1;
		int len = 0;


		len=sprintf(ch_query,"(select QQID,PASS,PASSTYPE from dce.qqpass)");
		if(zwsql_real_query(&zwsql,ch_query,len)!=0)
		{ 
	//		AfxMessageBox("数据库中表格出错"); 
			zwsql_free_result(result);
			zwsql_close(&zwsql);
			return FALSE;
		}
		ZWSQL_ROW row;
		if(!(result=zwsql_store_result(&zwsql)))
		{ 
			zwsql_free_result(result);
			zwsql_close(&zwsql);
			return FALSE;
	//		AfxMessageBox("读取数据集失败"); 		
		}
		while(row=zwsql_fetch_row(result))
		{
// 		for (int rc = table_data.top(); rc == r4success; rc=table_data.skip(),reccount--)
// 		{
// 			if (!table_data.deleted())
			{
				DWORD qqnum = atol(row[0]);
				if (qqnum<10000)
				{
///					WriteLog("dce", "QQNum: %d 错误!", qqnum);
					continue;
				}
				CString passtype = row[2];
				passtype.TrimRight();

				QQPWD* pvalue=NULL;
				if (passtype.CompareNoCase("0")==0 ||
					passtype.IsEmpty())
				{
					CString qqpwd = row[1];
					qqpwd.TrimLeft();
					qqpwd.TrimRight();
					v_OicqPassWord.m_qqpwdmap[bufid].Lookup(qqnum, pvalue);
					if (qqpwd.GetLength()>0)
					{
						QQPWD* tp = v_OicqPassWord.GetNewUnit();
						memset(tp, 0, sizeof(QQPWD)-sizeof(CString));
// 						if (qqpwd.GetLength()<20)
// 							strcpy(tp->password, qqpwd);
// 						else
							tp->longpwd = qqpwd;
						char md51[16];
						MD5Tran(md51, (void*)(LPCTSTR)qqpwd, qqpwd.GetLength());
						MD5Tran(tp->pwd2md5, md51, 16);
						if (pvalue==NULL)
						{
							v_OicqPassWord.m_qqpwdmap[bufid].SetAt(qqnum, tp);
							qqcount++;
							qqmd5count++;
							continue;
						}
						QQPWD* p = pvalue;
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
							qqmd5count++;
						}
						else
						{
							// 重复的密码MD5
							v_OicqPassWord.ReleaseUnit(tp);
							continue;
						}
					}
				}
				else
				{
					CString qqpwdmd5;
					qqpwdmd5.Format("%s",row[1]);
					qqpwdmd5.TrimLeft();
					qqpwdmd5.TrimRight();
					if (qqpwdmd5.GetLength()==0)
						continue;
					if (qqpwdmd5.GetLength()!=32)
					{
//						WriteLog("dce", "QQPwdMd5: %s 长度不合法", qqpwdmd5);
						qqpwdmd5.Empty();
					}
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
						QQPWD* tp = v_OicqPassWord.GetNewUnit();
						memset(tp, 0, sizeof(QQPWD)-sizeof(CString));
						memcpy(tp->pwd2md5, hash, 16);
						if (pvalue==NULL)
						{
							v_OicqPassWord.m_qqpwdmap[bufid].SetAt(qqnum, tp);
							qqcount++;
							qqmd5count++;
							continue;
						}
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
							// 重复的密码MD5
							v_OicqPassWord.ReleaseUnit(tp);
							continue;
						}
					}
				}
				

			}
		}
		
		zwsql_free_result(result);
		zwsql_close(&zwsql);

// 		table_code.closeAll();
// 		table_code.initUndo();
//		if (reccount == 0)
		{
			InterlockedExchange(&v_OicqPassWord.m_readflg, bufid);
			v_limitcount[0][OICQID] = qqcount;
			v_limitcount[0][OICQPASS] = qqmd5count;
//			WriteLog("dce", "Read QQNum:%d    Password:%d", qqcount, qqmd5count);
			return TRUE;
		}
// 		else
// 		{
// 			WriteLog("dce", "表%s记录数不对%d", filename, reccount);
// 			return FALSE;
// 		}
	}
	catch(...)
	{
		zwsql_free_result(result);
		zwsql_close(&zwsql);
// 		table_code.closeAll();
// 		table_code.initUndo();
		WriteLog("dce", "LoadOicqPwdTable() Error!!");
		return FALSE;
	}
}

BOOL UpdateTaobaoPwd(LPCTSTR filename)
{
	// 判断并解压RAR文件
	if (stricmp(filename, "d:\\netspy\\para\\dce\\taobaopass.rar") == 0)
	{
		if (!UnRarDbf(filename, "d:\\netspy\\para\\dce\\rar", 300))
		{
			return FALSE;
		}
		filename = "d:\\netspy\\para\\dce\\rar\\taobaopass.dbf";
	}
	CString importsql;
	importsql.Format("d:\\netspy\\dbclient -ujncfwlkjyxgongs dce <%s",filename);
	system(importsql);

	ZWSQL_RES *result;
	ZWSQL zwsql; //数据库连接句柄

// 	Code4	table_code;
// 	Data4	table_data;
	ASSERT(v_TaobaoPassWord.m_readflg==0 || v_TaobaoPassWord.m_readflg==1);
	long	bufid = (v_TaobaoPassWord.m_readflg==0)? 1 : 0;
	try
	{
		// 清空原缓冲区
		v_TaobaoPassWord.m_taobaopwdmap[bufid].RemoveAll();
		memset(v_limitcount[0]+OICQID, 0, (HTTPUPSPLIT-OICQID)*4);
		// 打开数据表
// 		table_code.autoOpen = 0;
// 		table_code.errOff = 1;
// 		table_code.errOpen = 0;
// 		table_code.readOnly = 1;
// 		table_data.open(table_code, filename);
// 		if (table_data.data == NULL)
// 		{
// 			table_code.closeAll();
// 			table_code.initUndo();
// 			WriteLog("dce", "文件%s打开失败!", filename);
// 			return FALSE;
// 		}
// 		
// 		// 关联字段
// 		Field4	fields[3];
// 		// SHIELDTYPE, SHIELDNAME
// 		if (r4success != fields[0].init(table_data, "TAOBAOID"))
// 		{
// 			table_code.closeAll();
// 			table_code.initUndo();
// 			WriteLog("dce", "关联字段(QQID)失败!");
// 			return FALSE;
// 		}
// 		if (r4success != fields[1].init(table_data, "PASS"))
// 		{
// 			table_code.closeAll();
// 			table_code.initUndo();
// 			WriteLog("dce", "关联字段(PASS)失败!");
// 			return FALSE;
// 		}

// 		if (r4success != fields[2].init(table_data, "MD5"))
// 		{
// 			table_code.closeAll();
// 			table_code.initUndo();
// 			WriteLog("dce", "关联字段(MD5)失败!");
// 			return FALSE;
// 		}
		
		// 遍历数据库
		
//		int reccount = table_data.recCount();
		int taobaocount = 0;
		int taobaomd5count = 0;
				zwsql_init (&zwsql);
		if(!zwsql_real_connect(&zwsql,"localhost","jncfwlkjyxgongs",NULL,NULL,SQLPORT,NULL,0))
		{ 
			AfxMessageBox("数据库连接失败"); 
			return false;
		}
		char ch_query[200];
		int i = 1;
		int len = 0;


		len=sprintf(ch_query,"(select TAOBAOID,PASS,MD5 from dce.taobaopass)");
		if(zwsql_real_query(&zwsql,ch_query,len)!=0)
		{ 
	//		AfxMessageBox("数据库中表格出错"); 
			zwsql_free_result(result);
			zwsql_close(&zwsql);
			return FALSE;
		}
		ZWSQL_ROW row;
		if(!(result=zwsql_store_result(&zwsql)))
		{ 
			zwsql_free_result(result);
			zwsql_close(&zwsql);
			return FALSE;
	//		AfxMessageBox("读取数据集失败"); 		
		}
		while(row=zwsql_fetch_row(result))
		{
//		for (int rc = table_data.top(); rc == r4success; rc=table_data.skip(),reccount--)
//		{
/*			if (!table_data.deleted())
			{
				CString taobaouser = fields[0].str();
				taobaouser.TrimRight();

				TAOBAOPWD* pvalue=NULL;
				v_TaobaoPassWord.m_taobaopwdmap[bufid].Lookup(taobaouser, pvalue);

				CString taobaopwdmd5 = fields[2].str();
				taobaopwdmd5.TrimLeft();
				taobaopwdmd5.TrimRight();
				if (taobaopwdmd5.GetLength()==0)
					continue;
// 				if (taobaopwdmd5.GetLength()!=32)
// 				{
// 					WriteLog("dce", "taobaoPwdMd5: %s 长度不合法", taobaopwdmd5);
// 					taobaopwdmd5.Empty();
// 				}
				taobaopwdmd5.MakeUpper();
				DWORD hash[4];
				memset(hash, 0, sizeof(hash));
				if (4 == sscanf(taobaopwdmd5, "%08X%08X%08X%08X",
					hash, hash+1, hash+2, hash+3))
				{
					hash[0] = Swap4Byte(hash[0]);
					hash[1] = Swap4Byte(hash[1]);
					hash[2] = Swap4Byte(hash[2]);
					hash[3] = Swap4Byte(hash[3]);
					TAOBAOPWD* tp = v_TaobaoPassWord.GetNewUnit();
					memset(tp, 0, sizeof(TAOBAOPWD)-sizeof(CString));
					memcpy(tp->pwd2md5, hash, 16);
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
			}*/
//			if (!table_data.deleted())
			{
				CString taobaouser;
				taobaouser.Format("%s",row[0]);
				taobaouser.TrimLeft();
				taobaouser.TrimRight();
				
				CString taobaopwd;
				taobaopwd.Format("%s",row[1]);
				taobaopwd.TrimLeft();
				taobaopwd.TrimRight();
				TAOBAOPWD* pvalue=NULL;
				v_TaobaoPassWord.m_taobaopwdmap[bufid].Lookup(taobaouser, pvalue);
				if (taobaopwd.GetLength()>0)
				{
					TAOBAOPWD* tp = v_TaobaoPassWord.GetNewUnit();
					memset(tp, 0, sizeof(TAOBAOPWD)-sizeof(CString));
// 					if (taobaopwd.GetLength()<20)
// 						strcpy(tp->password, taobaopwd);
// 					else
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
				
/*				CString taobaopwdmd5 = fields[2].str();
				taobaopwdmd5.TrimLeft();
				taobaopwdmd5.TrimRight();
				if (taobaopwdmd5.GetLength()==0)
					continue;
				if (taobaopwdmd5.GetLength()!=32)
				{
					WriteLog("dce", "taobaoPwdMd5: %s 长度不合法", taobaopwdmd5);
					taobaopwdmd5.Empty();
				}
				taobaopwdmd5.MakeUpper();
				DWORD hash[4];
				memset(hash, 0, sizeof(hash));
				if (4 == sscanf(taobaopwdmd5, "%08X%08X%08X%08X",
					hash, hash+1, hash+2, hash+3))
				{
					hash[0] = Swap4Byte(hash[0]);
					hash[1] = Swap4Byte(hash[1]);
					hash[2] = Swap4Byte(hash[2]);
					hash[3] = Swap4Byte(hash[3]);
					TAOBAOPWD* tp = v_TaobaoPassWord.GetNewUnit();
					memset(tp, 0, sizeof(TAOBAOPWD)-sizeof(CString));
					memcpy(tp->pwd2md5, hash, 16);
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
				}*/
			}
		}

		zwsql_free_result(result);
		zwsql_close(&zwsql);

// 		table_code.closeAll();
// 		table_code.initUndo();
//		if (reccount == 0)
		{
			InterlockedExchange(&v_TaobaoPassWord.m_readflg, bufid);
			v_limitcount[0][TAOBAOID] = taobaocount;
			v_limitcount[0][TAOBAOPASS] = taobaomd5count;
//			WriteLog("dce", "Read QQNum:%d    Password:%d", qqcount, qqmd5count);
			return TRUE;
		}
// 		else
// 		{
// //			WriteLog("dce", "表%s记录数不对%d", filename, reccount);
// 			return FALSE;
// 		}
	}
	catch(...)
	{
		zwsql_free_result(result);
		zwsql_close(&zwsql);

// 		table_code.closeAll();
// 		table_code.initUndo();
		WriteLog("dce", "LoadTaobaoPwdTable() Error!!");
		return FALSE;
	}
}
// 	{"REC_TIME",	r4num, 10, 0},// 记录同步标识(同步时使用)(time_t数据的串)
// 	{"INFOTYPE",	r4str, 20,0},	// type
// 	{"URL",			r4str, 200,0},	// url
// 	{"FIELD1",		r4str, 50, 0},	// 标识1
// 	{"FIELD2",		r4str, 50, 0},	// 标识2
// 	{"FIELD1TYPE",	r4str, 50, 0},	// 标识1类型
// 	{"FIELD2TYPE",	r4str, 50, 0},	// 标识2类型
// 	{"PRI",			r4str, 2,  0},	// 优先级
// 	{"MATCHTYPE",	r4str, 2,  0},	// 匹配方式
// 	{"SRCDEV",		r4str, 50, 0},	// 来源
// 	{"SYSTYPE",		r4str, 50, 0},	// 业务类型
// 	{"MARK1",		r4str, 50, 0},	// 备注1
// 	{"MARK2",		r4str, 100,0},	// 备注2
BOOL UpdateHttpupSplit(LPCTSTR filename)
{
	
	CString importsql;
	importsql.Format("d:\\netspy\\dbclient -ujncfwlkjyxgongs dce <%s",filename);
	system(importsql);

	// 打开数据库

	CStringArray urllist,infotypelist,splitlist, memolist;
	ZWSQL_RES *result;
	ZWSQL zwsql; //数据库连接句柄

// 	Code4	table_code;
// 	Data4	table_data;
	try
	{
// 		table_code.autoOpen = 0;
// 		table_code.errOff = 1;
// 		table_code.errOpen = 0;
// 		table_code.readOnly = 1;
// 		table_data.open(table_code, filename);
// 		if (table_data.data == NULL)
// 		{
// 			WriteLog("dce", "文件%s打开失败!", filename);
// 			table_code.closeAll();
// 			table_code.initUndo();
// 			return FALSE;
// 		}
// 		Field4	fields[5];
// 		static LPCTSTR FIELDSNAME[5] =
// 		{
// 			_T("URL"),	
// 			_T("INFOTYPE"),	
// 			_T("FIELD"),
// 			_T("FIELDTYPE"),
// 			_T("DESCRIP"),
// 		};
// 		
// 		for (int i = 0; i < 5; i++)
// 		{
// 			if (r4success != fields[i].init(table_data, FIELDSNAME[i]))
// 			{
// 				table_code.closeAll();
// 				table_code.initUndo();
// 				WriteLog("dce", "httpupmatch 关联%s失败!", FIELDSNAME[i]);
// 				return FALSE;
// 			}
// 		}

		memset(v_limitcount[0]+HTTPUPSPLIT, 0, 4);
		long	bufid = (v_CustUrlAccID==0)? 1 : 0;
		v_CustUrlList[bufid].clear();
		
		// 遍历数据库
		CString strV, strUrl, strSplit, strDesc, strQqinfo;
		int InfoType, FieldType;
		BOOL bInvalid;
//		int rc = table_data.top();
		int cntRecord = 0;
		zwsql_init (&zwsql);
		if(!zwsql_real_connect(&zwsql,"localhost","jncfwlkjyxgongs",NULL,NULL,SQLPORT,NULL,0))
		{ 
			return false;
		}
		char ch_query[200];
		int i = 1;
		int len = 0;


		len=sprintf(ch_query,"(select decode(URL,'inetsifter20021001'),decode(INFOTYPE,'inetsifter20021001'),\
			decode(FIELD,'inetsifter20021001'),decode(FIELDTYPE,'inetsifter20021001'),\
			decode(DESCRIP,'inetsifter20021001') from dce.httpupmatch)");
		if(zwsql_real_query(&zwsql,ch_query,len)!=0)
		{ 
			zwsql_close(&zwsql);
			return FALSE;
		}
		ZWSQL_ROW row;
		if(!(result=zwsql_store_result(&zwsql)))
		{ 
			zwsql_free_result(result);
			zwsql_close(&zwsql);
			return FALSE;
		}
		while(row=zwsql_fetch_row(result))
		{
// 		while (rc == r4success)
// 		{
			bInvalid = TRUE;
			strV = row[1];
			strV.TrimLeft();
			strV.TrimRight();
//			crypt((char*)(LPCTSTR)strV,strV.GetLength());
			if(strV.CompareNoCase("登录") == 0)
				InfoType = 1;
			else if(strV.CompareNoCase("注册") == 0)
				InfoType = 2;
			else if(strV.CompareNoCase("腾讯QQ登录") == 0 ||
					strV.CompareNoCase("腾讯QQ登陆") == 0)
				InfoType = 3;
			else
			{
				WriteLog("dce", "Up拆解发现不可识别信息类型:%s", strV);
				bInvalid = FALSE;
			}

			strV = row[3];
			strV.TrimLeft();
			strV.TrimRight();
//			crypt((char*)(LPCTSTR)strV,strV.GetLength());
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

				strUrl = row[0];
				strUrl.TrimRight();
//				crypt((char*)(LPCTSTR)strUrl,strUrl.GetLength());
				strUrl.TrimLeft("http: ");
				strUrl.TrimLeft("/");		// 去掉http:// 头, 如果有的话
//				strUrl.TrimRight();
				
				
				strSplit = row[2];
				strSplit.TrimRight();
//				crypt((char*)(LPCTSTR)strSplit,strSplit.GetLength());
				strSplit.TrimLeft();
				strSplit.TrimRight("= ");	// 去掉拆解符后的=号, 如果有的话
				
				strDesc = row[4];
				strDesc.TrimLeft();
				strDesc.TrimRight();

//				crypt((char*)(LPCTSTR)strDesc,strDesc.GetLength());
				CustomUrlField tmp(InfoType, FieldType, strSplit, strDesc, strUrl);
				v_CustUrlList[bufid].push_back(tmp);
			}
//			rc = table_data.skip();
		}
		
// 		table_code.closeAll();
// 		table_code.initUndo();
		zwsql_free_result(result);
		zwsql_close(&zwsql);

		InterlockedExchange(&v_CustUrlAccID, bufid);
		v_limitcount[0][HTTPUPSPLIT] = cntRecord;
		return TRUE;
	}
	catch(...)
	{
		WriteLog("dce","UpdateHttpupSplit Catch Error!");
// 		table_code.closeAll();
// 		table_code.initUndo();
		zwsql_free_result(result);
		zwsql_close(&zwsql);
		return FALSE;
	}
}

void crypt(char *sour,int src_len)   
{   
	char   *key = "fasfasf@#$@$^#$5646846asfcv,>?<>#$@!";
	int des_len = 36;
	int   i,j;   
	char	tmp_char[1];
	for   (i=0,j=0;i<src_len;i++,j++)
	{
		if (j==des_len)
		{
			j=0;
		}
		tmp_char[0] = sour[i];
		sour[i]=sour[i]^key[j]; 

		if ((((i+2) == src_len)&&
			sour[i+1] == 0x20))
		{
			sour[i] = tmp_char[0];
			sour[i+1] = 0x00;
			break;
		}

		if (((i+1) == src_len) ||
			sour[i] == 0x00 ||
			sour[i] == 0x0D ||
			sour[i] == 0x0A)
		{
			sour[i] = tmp_char[0];
		}
	}
}
int str_trim_crypt_cpy(char*pdtr, char *pstr)
{
	static char space[265] = 
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// 0
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
		crypt(pdtr,l);
		return l;
	}
	else
	{
		int l = tep-tp+1;
		memmove(pstr, pstr+bpos, l);
		pstr[l] = 0;
		crypt(pdtr,l);
		return l;
	}
}

TAG4INFO Rule_tag[]=			// 规则索引
{
	{"KEY_TYPE",		"KEY_TYPE", 0, 0, 0},	//)
	{"IM_TYPE",		"IM_TYPE", 0, 0, 0},	//)
	{0, 0, 0, 0, 0}
};
TAG4INFO Key_tag[]=				// 预设关键字索引
{
	{"KEY_TYPE",	"CLASS",			0, 0, 0},			//第一层关键字类
	{"SECKEYINFO",	"CLASS+SUBCLASS",	0, 0, 0},			//第一层加第二层关键字类
	{"THRKEYINFO",	"CLASS+SUBCLASS+SUBCLASS2", 0, 0, 0},	//第一层加第二层加第三层关键字类
	{0, 0, 0, 0, 0}
};
TAG4INFO People_tag[]=				// 重点人员索引
{
	{"IM_TYPE",		"IM_TYPE", 0, 0, 0},	//
	{0, 0, 0, 0, 0}
};
BOOL ReleaseMap(WarnlimitMap *r_WarnlimitMap);

/*****************************************************************************************
  函数名称:        UpdateIndexRule
  描述:            更新预警相关的表
  输入参数:        
                   
  输出参数:		      
  返回值:          
  其它:
  更新注意事项：

  1.以规则表为主线
  2.读出一条规则后，规则如果是重点人员+关键字的：
		2.1.1.读出一个重点人员，循环读出所有的关键字，进行初始化
	规则如果只是重点人员的：
		2.1.2.读出各个重点人员，进行初始化
	规则如果只是关键字的：
		2.1.3.循环读出所有的关键字，进行初始化
  3.循环读取所有规则
****************************************************************************************/
BOOL UpdateIndexRule(AllDicMap* alldicmap,int bufid)
{
	CString warnkeyfilename("d:\\netspy\\para\\dce\\keyword.dbf");
	CString ImportantManfilename("d:\\netspy\\para\\dce\\ImportantMan.dbf");
	CString warnrulefilename("d:\\netspy\\para\\dce\\warnrule.dbf");
	CString warnlevelfilename("d:\\netspy\\para\\dce\\warnlevel.dbf");
	
	CString warnkeyfilenamecdx("d:\\netspy\\para\\dce\\keyword.cdx");
	CString ImportantManfilenamecdx("d:\\netspy\\para\\dce\\ImportantMan.cdx");
	CString warnrulefilenamecdx("d:\\netspy\\para\\dce\\warnrule.cdx");
	CString warnlevelfilenamecdx("d:\\netspy\\para\\dce\\warnlevel.cdx");
	
	try
	{
		if((_access(ImportantManfilename, 0) != 0)
			|| (_access(warnlevelfilename, 0) != 0)
			|| (_access(warnrulefilename, 0) != 0)
			|| (_access(warnkeyfilename, 0) != 0))
		{
			return 0;
		}
		CString dbfname,cdxfname;
		
		//清空hash
		ReleaseMap(&v_warnlimitPara.m_warnlimitMap[bufid]);
		
		//***************初始化并打开4个表*********************************//
		
		Code4 code_rule,code_key,code_people,code_fz;
		CTableAccess ac_table_rule,ac_table_key,ac_table_people,ac_table_fz;
		code_rule.autoOpen = 0;
		code_rule.lockAttempts = 7;
		code_key.autoOpen = 0;
		code_key.lockAttempts = 7;
		code_people.autoOpen = 0;
		code_people.lockAttempts = 7;
		code_fz.autoOpen = 0;
		code_fz.lockAttempts = 7;
		
		Data4 data_rule,data_key,data_people,data_fz;
		ac_table_rule.SetCode4(&code_rule, &data_rule);
		ac_table_key.SetCode4(&code_key, &data_key);
		ac_table_people.SetCode4(&code_people, &data_people);
		ac_table_fz.SetCode4(&code_fz, &data_fz);
		
		// 打开预警规则表
		if (!OpenDbfTable(code_rule, data_rule, warnrulefilename))
			return 0;
		
		// 打开预设关键字表
		if (!OpenDbfTable(code_key, data_key, warnkeyfilename))
			return 0;
		
		// 打开重点人员表
		if (!OpenDbfTable(code_people, data_people, ImportantManfilename))
			return 0;	
		
		// 打开告警阀值表
		if (!OpenDbfTable(code_fz, data_fz, warnlevelfilename))
			return 0;
		
		Index4 index_rule,index_key,index_people;
		if (!OpenIndex(data_rule, index_rule, Rule_tag, warnrulefilenamecdx))
			return 0;
		
		if (!OpenIndex(data_key, index_key, Key_tag, warnkeyfilenamecdx))
			return 0;
		
		if (!OpenIndex(data_people, index_people, People_tag, ImportantManfilenamecdx))
			return 0;
		
		ac_table_rule.SetIndex4(&index_rule);
		ac_table_key.SetIndex4(&index_key);
		ac_table_people.SetIndex4(&index_people);
		
		// 选择索引
		Tag4 Tag_key(data_key, "KEY_TYPE");
		if (!Tag_key.isValid())
		{
			WriteLog("dce", "索引<REC_TIME>无效!");
			return 0;
		}
		data_key.select(Tag_key);
		
		// 选择索引
		Tag4 Tag_people(data_people, "IM_TYPE");
		if (!Tag_people.isValid())
		{
			WriteLog("dce", "索引<REC_TIME>无效!");
			return 0;
		}
		data_people.select(Tag_people);
		
		Field4	fields_rule[9];
		static LPCTSTR FIELDSNAME_RULE[9] =
		{
			_T("warn_class"),	_T("rule_id"),
			_T("im_type"),		_T("key_type"),
			_T("extend"),		_T("warnlevel"),
			_T("state"),		_T("SCLASS"),
			_T("SCLASS2")
		};
		for (int j = 0; j < 9; j++)
		{
			if (r4success != fields_rule[j].init(data_rule, FIELDSNAME_RULE[j]))
			{
				WriteLog("dce", "关联预警规则表字段%s失败!", FIELDSNAME_RULE[j]);
				break;
			}
		}
		if (j < 9)
			return 0;	// 关联字段失败
		
		// 关联字段
		Field4	fields_key[6];
		static LPCTSTR FIELDSNAME_KEY[6] =
		{
			_T("CLASS"),	_T("KEYWORDS1"),	_T("WARN_VALUE"),
			_T("SUBCLASS"), _T("SUBCLASS2"),    _T("WARN_ADDUP"),
		};
		for (j = 0; j < 6; j++)
		{
			if (r4success != fields_key[j].init(data_key, FIELDSNAME_KEY[j]))
			{
				WriteLog("dce", "关联本地IP表字段%s失败!", FIELDSNAME_KEY[j]);
				break;
			}
		}
		if (j < 6)
			return 0;	// 关联字段失败
		
		// 关联字段
		Field4	fields_people[5];
		static LPCTSTR FIELDSNAME_PEOPLE[5] =
		{
			_T("im_type"),	_T("dummy_type") ,	
			_T("im_key"),	_T("real_name"),
			_T("card_id")
		};
		for (j = 0; j < 5; j++)
		{
			if (r4success != fields_people[j].init(data_people, FIELDSNAME_PEOPLE[j]))
			{
				WriteLog("dce", "关联本地IP表字段%s失败!", FIELDSNAME_PEOPLE[j]);
				break;
			}
		}
		if (j < 5)
			return 0;	// 关联字段失败
		
		// 关联字段
		Field4	fields_fz[3];
		static LPCTSTR FIELDSNAME_FZ[3] =
		{
			_T("wl_red"),	_T("wl_orange") ,	
				_T("wl_yellow")
		};
		for (j = 0; j < 3; j++)
		{
			if (r4success != fields_fz[j].init(data_fz, FIELDSNAME_FZ[j]))
			{
				WriteLog("dce", "关联本地IP表字段%s失败!", FIELDSNAME_FZ[j]);
				break;
			}
		}
		if (j < 3)
			return 0;	// 关联字段失败
		
		//***************初始化并打开4个表完毕*********************************//
		
		//读取总阀值
		data_fz.top();
		CString red,yellow,orange;
		red = fields_fz[0].str();
		red.TrimRight();
		yellow = fields_fz[1].str();
		yellow.TrimRight();
		orange = fields_fz[2].str();
		orange.TrimRight();
		v_warnlimitPara.g_warnlimit[bufid].red = atoi(red);
		v_warnlimitPara.g_warnlimit[bufid].yellow = atoi(yellow);
		v_warnlimitPara.g_warnlimit[bufid].orange = atoi(orange);
		
		Lpwarnkeyinfo lpwarnlimit = NULL;
//		Warnkeylist *m_warnkeylist = NULL;
		Lpwarnkeyinfo *currentkeylist = NULL;
		
		CString quanzhi, account, key;
		CString emphasestype, dummytype, name, card, im_name, addup;
		char  seekBuf[152];
		
		//以规则表为主线，初始化结构		
		for (int rc = data_rule.top(); rc == r4success; rc = data_rule.skip())
		{
			CString state, classtype, ruleid, emphasestype, keytype, extend, level;
			CString secclass, thrclass;
			
			//读取规则表的信息
			state = fields_rule[6].str();
			state.TrimRight();
			
			if(state.CompareNoCase("0") ==0 )
				continue;
			
			classtype = fields_rule[0].str();
			classtype.TrimRight();
			
			ruleid = fields_rule[1].str();
			ruleid.TrimRight();
			
			emphasestype = fields_rule[2].str();
			emphasestype.TrimRight();
			
			keytype = fields_rule[3].str();
			keytype.TrimRight();
			
			extend = fields_rule[4].str();
			extend.TrimRight();
			
			level = fields_rule[5].str();
			level.TrimRight();		
			
			secclass = fields_rule[7].str();
			secclass.TrimRight();
			
			thrclass = fields_rule[8].str();
			thrclass.TrimRight();
			//读取规则表的信息结束
			
			//有重点人员的规则
			if(!emphasestype.IsEmpty())
			{
				//查找此规则人员类的所有人员
				int subrc = data_people.seek(emphasestype);
				while (subrc == r4success)
				{
					//读出某一条重点人员信息
					emphasestype = fields_people[0].str();
					emphasestype.TrimRight();
					
					dummytype = fields_people[1].str();
					dummytype.TrimRight();
					
					im_name = fields_people[2].str();
					im_name.TrimRight();
					
					name = fields_people[3].str();
					name.TrimRight();
					
					card = fields_people[4].str();
					card.TrimRight();
					
					//为重点人员+关键字的规则
					if(!keytype.IsEmpty())
					{
						//读取关键字信息
						CString tmpkeytype, tmpsecclass, tmpthrclass;
						tmpkeytype.Format("%s",keytype);
						crypt((char*)(LPCSTR)tmpkeytype,tmpkeytype.GetLength());
						
						tmpsecclass.Format("%s",secclass);
						crypt((char*)(LPCSTR)tmpsecclass,tmpsecclass.GetLength());
						
						tmpthrclass.Format("%s",thrclass);
						crypt((char*)(LPCSTR)tmpthrclass,tmpthrclass.GetLength());
						
						Tag4 Tag_rule;
						if(secclass.IsEmpty())
						{
							Tag_rule.init(data_rule, "KEY_TYPE");
							sprintf(seekBuf,"%-50s",tmpkeytype);
						}
						else if(thrclass.IsEmpty())
						{
							Tag_key.init(data_key, "SECKEYINFO");
							sprintf(seekBuf,"%-50s%-50s",tmpkeytype,tmpsecclass);
						}
						else
						{
							Tag_key.init(data_key, "THRKEYINFO");
							sprintf(seekBuf,"%-50s%-50s%-50s",tmpkeytype,tmpsecclass,tmpthrclass);
						}
						
						if (!Tag_key.isValid())
						{
							WriteLog("dce", "索引 Tag_rule 无效!");
							return 0;
						}
						data_key.select(Tag_key);
						
						int subrc = data_key.seek(seekBuf);
						while (subrc == r4success)
						{
							key = fields_key[1].str();
							key.TrimRight();
							crypt((char*)(LPCSTR)key,key.GetLength());
							
							quanzhi = fields_key[2].str();
							quanzhi.TrimRight();
							crypt((char*)(LPCSTR)quanzhi,quanzhi.GetLength());
							
							addup = fields_key[5].str();
							addup.TrimRight();
							crypt((char*)(LPCSTR)addup,addup.GetLength());
							
							lpwarnlimit = new warnkeyinfo;
							lpwarnlimit->m_fenlei = classtype;
							lpwarnlimit->m_ruleid = ruleid;
							lpwarnlimit->m_level = level;
							lpwarnlimit->m_name = name;
							lpwarnlimit->m_card = card;
							lpwarnlimit->m_imname = im_name;
							lpwarnlimit->m_peopletype = emphasestype;
							lpwarnlimit->m_keytypename = keytype;
							lpwarnlimit->m_secclass = secclass;
							lpwarnlimit->m_thrclass = thrclass;
							lpwarnlimit->m_key = key;
							lpwarnlimit->m_keyquanzhi = atoi(quanzhi);
							lpwarnlimit->m_IsAddup = atoi(addup);
							
							AllDicInfo* lpalldicinfo = NULL;
							if (alldicmap->Lookup(key,lpalldicinfo))
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
								
								alldicmap->SetAt(key,lpalldicinfo);
							}
							
							subrc = data_key.seekNext(seekBuf);
						}
					}
					//为只有关键字的规则
					else
					{
						
					}
					
					lpwarnlimit = new warnkeyinfo;
					lpwarnlimit->m_fenlei = classtype;
					lpwarnlimit->m_ruleid = ruleid;
					lpwarnlimit->m_level = level;
					lpwarnlimit->m_name = name;
					lpwarnlimit->m_card = card;
					lpwarnlimit->m_imname = key;
					lpwarnlimit->m_peopletype = emphasestype;
					lpwarnlimit->m_keytypename = keytype;
					lpwarnlimit->m_secclass = secclass;
					lpwarnlimit->m_thrclass = thrclass;
					lpwarnlimit->m_keyquanzhi = atoi(quanzhi);
					lpwarnlimit->m_IsAddup = atoi(addup);
					lpwarnlimit->m_havesearchinpeople = TRUE;
					
					//重点人员中上网帐号、IP地址、MAC地址类的虚拟身份值
					if (dummytype.CompareNoCase("上网帐号") == 0||
						dummytype.CompareNoCase("ip地址") == 0||
						dummytype.CompareNoCase("mac地址") == 0)
					{
						
						Lpwarnkeyinfo tmplpwarnlimit;
						if (v_warnlimitPara.m_warnlimitMap[bufid].Lookup(key,tmplpwarnlimit))
						{
							tmplpwarnlimit->m_last->m_next = lpwarnlimit;
							tmplpwarnlimit->m_last = lpwarnlimit;
						}
						else
						{
							lpwarnlimit->m_last = lpwarnlimit;
							v_warnlimitPara.m_warnlimitMap[bufid].SetAt(key,lpwarnlimit);
						}
						
					}
					//重点人员关键字类型虚拟身份值
					else
					{
						AllDicInfo* lpalldicinfo = NULL;
						if (alldicmap->Lookup(key,lpalldicinfo))
						{
							lpalldicinfo->m_warninfo->m_last->m_next = lpwarnlimit;
							lpalldicinfo->m_warninfo->m_last = lpwarnlimit;
						}
						else
						{
							lpalldicinfo = new AllDicInfo;
							
							lpalldicinfo->m_warninfo = lpwarnlimit;
							
							alldicmap->SetAt(key,lpalldicinfo);
						}
					}
					subrc = data_people.seekNext(emphasestype);
				}
			}
			//为只是关键字的规则
			else
			{
				
			}
			
			
			InterlockedExchange(&v_warnlimitPara.m_readflg, bufid);
			
			CloseTable(&code_rule, &data_rule, &index_rule, &ac_table_rule);
			CloseTable(&code_people, &data_people, &index_people, &ac_table_people);
			CloseTable(&code_key, &data_key, &index_key, &ac_table_key);
			CloseTable(&code_fz, &data_fz, NULL, &ac_table_fz);
		}
			
	}		
	catch (...)
	{
		WriteLog("dce","UpdateIndexRule error");
	}
	return 1;
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