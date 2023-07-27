
#include "stdafx.h"
#include "resource.h"
#include "DCE.h"
#include <afxtempl.h>
#include <list>
#include <fstream>
#include <Shlwapi.h>
#include "CheckHard.h"
#include "BaseStruct.h"
#include "ReadDDeFile.h"
#include "SocketSession.h"
#include "DceMission.h"
// #include "zwsql.h"
#include "zwsqldb.h"
#include "IpQueryLib.h"
#include "dbgswitch.h"
#include "NatAuth.h"
#include "http/Installed/Installed.h"
#include "http/Shopping/Shopping.h"
#include "PhoneNumberLocation.h"
#include "http/Forum/ForumBaiduTieBa.h"

extern BOOL testtaobaopass();
Basicinfomap m_basicinfomap;

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern int printThreadFile(BOOL &bJiya);

extern long m_ReadDdeFileNum;
extern long m_TotalPacketNum;
extern long m_TotalByteNum;
//extern HANDLE _crtheap;

#ifdef	PACKETUNM
extern long m_TotalQueueUnit1;	// 入队列的数
extern long m_TotalQueueUnit2;	// 出队列的数
#endif


#ifdef		_DEBUG
#pragma message("连接静态库文件: imwDogLibD.lib")
#pragma comment( lib, "imwDogLibD.lib" )
#else
#pragma message("连接静态库文件: imwDogLib.lib")
#pragma comment( lib, "imwDogLib.lib" )
#endif

// #pragma comment( lib, "Lib\\c4dll.lib")
#pragma message("连接库文件: openssl")
#pragma comment(lib, "Lib\\ssleay32.lib")
#pragma comment(lib, "Lib\\libeay32.lib")

BOOL v_DogFlag = 0;
//int		m_dcegroup=0;			// dce所在的组
//int		m_dceposition=0;		// dce号
int	m_ddecount=0;					// dde的个数
HANDLE MYHEAP=0;
char	v_localip[20];				// 本机IP
CStringArray v_ParaServer;			// 参数服务器数组(新的CS)

void RunMainLoop();					// 主循环
void CreateLogPath(CTime now);		// 创建日志路径
void CreateServerFile();			// 检查服务器文件的积压状态
void CheckDDeFile();
void OutPutTmpStrramFile();
int RunDceMission(DceMission* pDceMission, int size);
extern int getallmac();

DWORD WINAPI DelDDeBakFile(LPVOID p);
DWORD WINAPI RecSpreadPacket(LPVOID);
DWORD WINAPI MacFileServiceThread(LPVOID n);

R_RSA_PRIVATE_KEY  g_privateKey;  /* new RSA private key */

const BYTE Inject_RSA_PrivateKey[] = 
{
0x85, 0xD5, 0x67, 0x98, 0x33, 0x69, 0xC6, 0xE7, 0x10, 0x00, 0x00, 0x00, 0xC2, 0x02, 0x00, 0x00, 
0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0xE1, 0xDC, 0x08, 0x00, 0x01, 0x10, 
0x28, 0x37, 0x25, 0x42, 0x9D, 0x18, 0x06, 0x8C, 0x9C, 0xA0, 0xB4, 0xA3, 0x57, 0xA0, 0x23, 0x01, 
0xD4, 0x19, 0x7A, 0xB0, 0x0C, 0x9C, 0x1E, 0x1C, 0xBB, 0xCA, 0x44, 0xEB, 0x31, 0x6B, 0x18, 0xB4, 
0x67, 0x34, 0x66, 0x69, 0x82, 0x53, 0x27, 0x98, 0x20, 0x17, 0x17, 0x18, 0xB9, 0x2B, 0x52, 0xA4, 
0x42, 0x85, 0x57, 0xF6, 0xC4, 0xC9, 0xF8, 0xA1, 0x00, 0x54, 0x38, 0x03, 0x25, 0xBC, 0x18, 0xE2, 
0x47, 0xC2, 0xC5, 0x23, 0x14, 0x57, 0x56, 0x5D, 0xE2, 0xB6, 0x01, 0x83, 0x3D, 0x10, 0xED, 0x72, 
0x35, 0x7B, 0x02, 0xAC, 0x46, 0x03, 0x65, 0x23, 0x94, 0x3C, 0x3B, 0xC5, 0xCE, 0xD4, 0x90, 0x3E, 
0xA8, 0x0A, 0x0D, 0x51, 0xD7, 0x6E, 0x55, 0x80, 0x3E, 0xAC, 0xAC, 0x80, 0x98, 0x12, 0xE2, 0x0D, 
0x22, 0x72, 0x8D, 0x4E, 0xAC, 0x29, 0x84, 0x68, 0x4F, 0x2A, 0x5C, 0x44, 0x68, 0xC0, 0xF2, 0x51, 
0x41, 0x9C, 0x0D, 0x09, 0xD3, 0x90, 0x7C, 0x13, 0x93, 0x41, 0xC6, 0xAD, 0x02, 0x35, 0xC8, 0x15, 
0x02, 0x40, 0xB8, 0xB0, 0xE1, 0xC3, 0x88, 0x13, 0x2B, 0x5E, 0xCC, 0xB8, 0xB1, 0xE3, 0xC7, 0x90, 
0x23, 0x1F, 0x0E, 0x00, 0x20, 0x00, 0x1E, 0x23, 0xD2, 0x80, 0xE4, 0xF8, 0x57, 0x44, 0x8D, 0x96, 
0x6B, 0xDE, 0xE6, 0x80, 0x7B, 0x34, 0xE4, 0x96, 0x87, 0x57, 0xDA, 0x86, 0x7D, 0xFA, 0x16, 0xCC, 
0x5D, 0x8D, 0x61, 0x84, 0x5E, 0x3C, 0x6A, 0xF4, 0x2D, 0xD5, 0x17, 0x6A, 0x93, 0x64, 0xE4, 0x22, 
0x70, 0x20, 0x43, 0xAB, 0x30, 0x27, 0x8C, 0x84, 0x63, 0x65, 0x8C, 0x88, 0x1F, 0x23, 0x0F, 0x9A, 
0x59, 0x88, 0x10, 0x82, 0x0F, 0x00, 0x70, 0xE5, 0x7C, 0x58, 0xAA, 0xC3, 0x82, 0x5F, 0xA4, 0x79, 
0x02, 0xC4, 0xE5, 0xC8, 0xB1, 0x2F, 0x5F, 0x37, 0x21, 0x61, 0xA8, 0x3D, 0x2B, 0x13, 0x2E, 0x8B, 
0x14, 0x12, 0x5B, 0x48, 0x2C, 0x1A, 0xB5, 0xEE, 0xD5, 0x85, 0x4C, 0x4E, 0x64, 0xF8, 0x90, 0xB2, 
0x67, 0x9D, 0x8A, 0x08, 0x66, 0x70, 0xA0, 0xC9, 0xA3, 0xA5, 0xC2, 0x0C, 0x4B, 0x69, 0x44, 0x91, 
0xC2, 0x14, 0x34, 0xA4, 0x32, 0xCD, 0x10, 0x96, 0xF0, 0xB0, 0x81, 0x15, 0x21, 0x9C, 0x50, 0xC6, 
0x14, 0x65, 0x98, 0x72, 0x46, 0x1C, 0xC3, 0xF4, 0x60, 0x8F, 0x34, 0x51, 0x5C, 0xD2, 0x03, 0x0C, 
0x5D, 0xD8, 0xE3, 0x04, 0x37, 0x0C, 0x8C, 0x14, 0x09, 0x3C, 0x3B, 0xFC, 0x21, 0x44, 0x39, 0x0C, 
0x80, 0xC1, 0x0A, 0x1D, 0x24, 0x24, 0x73, 0x09, 0x24, 0x73, 0xF4, 0xA0, 0x88, 0x01, 0xE2, 0xC4, 
0x81, 0x06, 0x0B, 0x2E, 0x78, 0x81, 0x06, 0x1F, 0x02, 0xBC, 0x90, 0x8C, 0x05, 0xB0, 0x38, 0xD1, 
0x0C, 0x2E, 0xFE, 0x51, 0x03, 0x09, 0x09, 0x13, 0x18, 0xC2, 0x44, 0x0F, 0x0A, 0x14, 0x40, 0x0F, 
0x25, 0x7C, 0x70, 0x72, 0xCC, 0x2F, 0xED, 0x68, 0xE3, 0x42, 0x1F, 0x66, 0xB0, 0xA3, 0x80, 0x06, 
0x37, 0x7C, 0x22, 0x46, 0x27, 0xA0, 0xE0, 0x33, 0xCA, 0x3D, 0x89, 0x44, 0xF2, 0xC4, 0x0A, 0x57, 
0x2C, 0x10, 0x02, 0x25, 0xB8, 0x38, 0xA1, 0x04, 0x3A, 0xD7, 0xB8, 0x70, 0x0D, 0x27, 0xA1, 0xB4, 
0x92, 0xC3, 0x2D, 0xBB, 0xF0, 0x73, 0x80, 0x37, 0xE0, 0x24, 0xC2, 0x0D, 0x1B, 0x7B, 0x3C, 0x01, 
0x0F, 0x00, 0x09, 0xE0, 0x53, 0x41, 0x19, 0x68, 0x18, 0xE1, 0x83, 0x0D, 0xF9, 0xB8, 0x71, 0xC6, 
0x07, 0x00, 0xBC, 0x42, 0xC0, 0x2E, 0x7F, 0xE8, 0xB3, 0x82, 0x17, 0x0C, 0x5C, 0x03, 0x41, 0x14, 
0x4F, 0xC0, 0xF0, 0x05, 0x31, 0x90, 0x2C, 0xB1, 0x4C, 0x14, 0x3E, 0xE8, 0x40, 0xC3, 0x10, 0x47, 
0xE4, 0x32, 0xC0, 0x1E, 0xB3, 0x7C, 0x72, 0x44, 0x30, 0xC9, 0x34, 0x10, 0x8D, 0x19, 0x06, 0x04, 
0xA1, 0xC6, 0x1B, 0x1C, 0xE8, 0x61, 0x87, 0x26, 0x37, 0x98, 0xF2, 0x4B, 0x20, 0xE9, 0x28, 0x71, 
0x45, 0x20, 0x7E, 0x7C, 0xF3, 0x45, 0x18, 0xA5, 0x48, 0x41, 0xC0, 0x08, 0xBE, 0x5C, 0xA0, 0x8B, 
0x1A, 0x67, 0xD8, 0xF1, 0xC6, 0x11, 0x2F, 0x14, 0xA1, 0x47, 0x24, 0xDF, 0xFC, 0xF2, 0x8A, 0x2B, 
0x50, 0xE4, 0x40, 0x8B, 0x07, 0xC1, 0x70, 0x83, 0x84, 0x2C, 0x2C, 0xF0, 0xD2, 0x01, 0x1C, 0xEE, 
0x98, 0xE2, 0x0B, 0x2C, 0xD1, 0x10, 0x60, 0x07, 0x1B, 0xDA, 0xFC, 0xC0, 0x06, 0x02, 0xAC, 0xD4, 
0x20, 0x04, 0x14, 0x5A, 0x8C, 0x73, 0x41, 0x0A, 0x5B, 0x50, 0xA2, 0xCE, 0x28, 0x00, 0x28, 0xD3, 
0x8F, 0x15, 0xF4, 0x90, 0x22, 0x04, 0x25, 0x59, 0xA8, 0x33, 0x40, 0x3E, 0x67, 0x3C, 0x30, 0x46, 
0x07, 0x4D, 0x10, 0xB1, 0xC7, 0x1E, 0xB0, 0x40, 0xB1, 0x04, 0x01, 0xF2, 0x80, 0x31, 0x49, 0x04, 
0xBC, 0x74, 0x81, 0xC2, 0x32, 0x9A, 0x98, 0x70, 0x09, 0x3B, 0xE4, 0x78, 0x03, 0xC7, 0x29, 0x83, 
0x44, 0x90, 0x4A, 0x23, 0xDC, 0xD8, 0x63, 0x82, 0x2A, 0x63, 0x64, 0xD3, 0xC9, 0x07, 0x88, 0x0C, 
0xB1, 0xC8, 0x19, 0x47, 0x24, 0x60, 0x41, 0x23, 0x14, 0xA8, 0xF0, 0xCB, 0x3F, 0x4F, 0xD4, 0x81, 
0x4C, 0x05, 0xC3, 0x48, 0xA1, 0x48, 0x29, 0xBD, 0xB0, 0xC2, 0x0E, 0x34, 0xF0, 0x24, 0x11, 0x4B, 
0x17, 0x70, 0xA8, 0x42, 0x4D, 0x11, 0x01, 0x01 	

};



void LodeKeys(DWORD Bin_Data_Index ,R_RSA_PRIVATE_KEY * privateKey)
{

	unsigned char* pSrc=(unsigned char*)Inject_RSA_PrivateKey;
	DWORD	dwOldSize = * (DWORD *) &pSrc[12] ;
	BYTE	* pDest = new BYTE[dwOldSize];
	ZeroMemory (pDest, dwOldSize) ;
	if (pDest == NULL)
	{
		return ;
	}
	//////////////////////////////////////////////////////////////////////////
	//	解码
	if (* (WORD *) &pSrc[10] == 0)
	{
		pSrc += 26 + (* (WORD *) &pSrc[24]) ;
		FCLzw	lzw ;
		lzw.LZW_Decode (pSrc, pDest) ;
	}
	else
	{
		delete[] pDest;
		return;
	}

	memcpy(privateKey,pDest,sizeof(R_RSA_PRIVATE_KEY));
	delete[] pDest;



// 	HGLOBAL hRes;
// 	HRSRC hResInfo;
// 	HMODULE insApp = GetModuleHandle(NULL);
// 	hResInfo = FindResource(insApp,MAKEINTRESOURCE(Bin_Data_Index),"BIN_DATA");
// 	hRes = LoadResource(insApp,hResInfo );	// Load it
// 	DWORD dFileLength = SizeofResource( insApp, hResInfo );  //计算EXE文件大小
// 	BOOL isSucceed=FALSE;
// 	memcpy(privateKey,hRes,sizeof(R_RSA_PRIVATE_KEY));
}
BOOL CreateValue(char *buf )
{
	for (int i=0; i<16; i++)
	{
		switch(rand() % 5)
		{
		case 0:		// '0' - '9'
			buf[i] = '0' + (rand() % 10);
			break;
		case 1:
		case 2:		// 'A' - 'Z'
			buf[i] = 'A' + (rand() % 26);
			break;
		case 3:
		case 4:		// 'a' - 'z'
			buf[i] = 'a' + (rand() % 26);
			break;
		}
	}
	return TRUE;
}
extern DWORD ConvertIP(CString strip);

// 程序入口
int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	// add by cxl
	char szFullPath[MAX_PATH] = {0};

	GetModuleFileName(NULL, szFullPath, MAX_PATH);
	PathRemoveFileSpec(szFullPath);
	PathAppend(szFullPath, "config.ini");

	if (PathFileExists(szFullPath))
	{
		char sz3G[10] = {0};
		char szAAA[10] = {0};
		::GetPrivateProfileString("Usage", "3G", "0", sz3G, 9, szFullPath);
		::GetPrivateProfileString("Usage", "AAA", "0", szAAA, 9, szFullPath);

		g_b3g = atoi(sz3G);
		g_bAAA = atoi(szAAA);
		if (!g_b3g && !g_bAAA)
		{
			g_bNormal = true;
		}

	}
	else
	{
		::WritePrivateProfileString("Usage", "3G", "0", szFullPath);
		::WritePrivateProfileString("Usage", "AAA", "0", szFullPath);
	}
	// end


// 	CString tta;
// 	CFile fp;
// 	fp.Open("v:\\httphtml.txt", CFile::modeRead);
// 	int flen = fp.GetLength();
// 	char *ptt = tta.GetBufferSetLength(flen+2);
// 	memset(ptt+flen, 0, 2);
// 	fp.ReadHuge(ptt, flen);
// 	fp.Close();
// 	CString tt = ConverHttp2Html(tta);
//k_IpQueryTable.LoadAllIPFromDbf("v:\\netipaddr.bcp");
//	system("CatchScreen.exe");
//	testtaobaopass();
//UpdatePara();
// 	time_t tm = CTime(2000,1,1,0,0,0).GetTime();
// 	char str[100];
// 	GetTimeString(tm, "%Y-%m-%d %H:%M:%S", str, 100);
//	char *p = new char[20];
	CreateLogPath(0);
	
	CString strHardId;
	GetRegeditKey(strHardId);

	WriteLog("dce", "***************DCE.EXE Bulid: %s %s\n%s****************",
		__DATE__, __TIME__, __FILE__);
//	v_pWriteLogFun = ;
	// 处理命令行参数
	ProcessCmdLine(argc, argv);

// 	if (v_programtype==SYSTEM_SLEUTH)
// 		v_stat = FALSE;
				
	if(_setmaxstdio(2048)!=2048)
	{
		WriteLog("dce", "Error!! 设置最大文件数错误");
		return -10;
	}

	HANDLE v_AppMutexH = CreateMutex(NULL, FALSE, "Dce 2008-03-01 00:00:00");
	if (v_AppMutexH != NULL)
	{
		if (::GetLastError() == ERROR_ALREADY_EXISTS)
		{
			WriteLog("dce", "Error!! Mutex is Exist!!");
			CloseHandle(v_AppMutexH);
			return -11;
		}
	}
	else
	{
		WriteLog("dce", "Error!! Mutex Create Failed!!");
		return -12;
	}
	
	// 判断删除para\dce目录
	if (_access(g_DelDceParaFlg, 0) == 0)
	{
		WriteLog("dce", "删除para\\dce目录!");
		system(CStringEx::GetStringEx("rd /s /q \"%s\"", g_strRootPara));
		DeleteFile(g_DelDceParaFlg);
	}

	//检查迅雷收集MAC工作目录是否存在
	CString threadpath;
	threadpath.Format("e:\\netspy\\basedata");
	if(access(threadpath,0)!=0)
	{
		mkdir(threadpath);

		CString threadmacpath;
		threadmacpath.Format("%s\\mac",threadpath);
		if(access(threadmacpath,0)!=0)
		{
			mkdir(threadmacpath);
		}

		threadmacpath.Format("%s\\tmpmac",threadpath);
		if(access(threadmacpath,0)!=0)
		{
			mkdir(threadmacpath);
		}
	}

	/// dump switch list and state
	g_dbgSwitch.Update();

// RBPBEGIN
#ifndef		_DEBUG
	if (CheckDebug())
	{
		WriteLog("dce", "soft init error8");
		Goto_Ring0_2();
	}
#endif
	
	// 获得本机IP
	memset(v_localip, 0, 16);
	char szHostName[128];
	// 获取系统类型
	memset(szHostName, 0, 4);
	IMWGetPrivateProfileString("para","systype","NO",szHostName,sizeof(szHostName),CONFIGFILE);
	if (stricmp(szHostName, "NO") == 0)
	{
		IMWGetPrivateProfileString("system","system","NO",szHostName,sizeof(szHostName),CONFIGFILE);
	}
	_strlwr(szHostName);
	if (v_programtype == SYSTEM_002)
	{
		if (stricmp(szHostName, "zk") == 0)
			v_programtype = SYSTEM_002;
		else if (stricmp(szHostName, "szfj") == 0)
			v_programtype = SYSTEM_SZFJ;
		else if (stricmp(szHostName, "fyy") == 0
			||stricmp(szHostName, "nobusiness") == 0)
		{
			v_programtype = SYSTEM_FYY;
		}
		else if (stricmp(szHostName, "zone") == 0)
		{
			v_programtype = SYSTEM_ZONE;
		}
		else if (stricmp(szHostName, "department") == 0)
		{
			v_programtype = SYSTEM_DEPARTMENT;
		}
		else if (stricmp(szHostName, "hotel") == 0)
		{
			v_programtype = SYSTEM_HOTEL;
		}		
		else if (stricmp(szHostName, "telewireless") == 0)
		{
			v_programtype = SYSTEM_TELEWIRELESS;
		}
		else if (stricmp(szHostName, "unicomwireless") == 0)
		{
			v_programtype = SYSTEM_UNICOMWIRELESS;
		}
		else if (stricmp(szHostName, "zk") == 0)
		{
			v_programtype = SYSTEM_ZK;
		}
		// system_jc modified to system_002
// 		else if (stricmp(szHostName, "jc") == 0)
// 			v_programtype = SYSTEM_002;
// 		else if (stricmp(szHostName, "sj") == 0)
// 			v_programtype = SYSTEM_SJ;
// 		else if (stricmp(szHostName, "zk") == 0)
// 			v_programtype = SYSTEM_002;
		else
		{
			// 错误的系统标识
			WriteLog("dce", "Error!! 系统标识(%s)错误",szHostName);
			return 0;
		}
	}

// 	//////////////////////////////////////////////////////////////////////////
// 	//使用时间限制
// 	char rcstr[1024];
// 	IMWGetPrivateProfileString("systemenddate", "enddate", "**", rcstr, 1020, CONFIGFILE);
// 	if(rcstr[0] != 0 && strlen(rcstr) > 2)
// 	{
// 		COleDateTime CurrTime = COleDateTime::GetCurrentTime();  
// 		COleDateTime tempTime;  
// 		tempTime.ParseDateTime(rcstr);  
// 
// 		COleDateTimeSpan result = CurrTime - tempTime;
// 		if(result.GetDays()>0)
// 			return 1;
// 		
// 	}

	if (v_programtype != SYSTEM_SLEUTH)
	{
		char keystr[40];
		memset(keystr, 0, sizeof(keystr));
		IMWGetPrivateProfileString(
			"intercept", "arrpos",	"NO",
			keystr, 36,	ZONEPOSINI);
		if(stricmp(keystr, "NO")!=0)
			v_ddelist.spyaddr.Format("%s",keystr);
		else
		{
			memset(keystr, 0, sizeof(keystr));
			GetPrivateProfileString(
				"intercept", "arrpos",	"NO",
				keystr, 36,	POSCONFIGINI);
			if(stricmp(keystr, "NO")!=0)
				v_ddelist.spyaddr.Format("%s",keystr);
			else
				v_ddelist.spyaddr.Format("接口1");
		}
	}
	WriteLog("dce", "system=%s, v_programtype=0x%x", szHostName, v_programtype);
	char workpath[128];
	// 获取工作目录
	memset(workpath, 0, 4);
	IMWGetPrivateProfileString("WORKDIR","dde","z",workpath,sizeof(workpath),CONFIGFILE);
	g_ddepath.Format("%s",workpath);
	memset(workpath, 0, 4);
	IMWGetPrivateProfileString("WORKDIR","dce","e",workpath,sizeof(workpath),CONFIGFILE);
	g_dcepath.Format("%s",workpath);
	Inipubvar();
// 	else if (v_programtype==SYSTEM_SLEUTH && stricmp(szHostName, "sleuth")!=0)
// 	{
// 		WriteLog("dce", "Error!! 系统标识错误");
// 		return 0;
// 	}
	int group, machine;
	GetLocalHostIP(group, machine);
	if((group <= 0) || (machine <= 0))
	{
		CloseHandle(v_AppMutexH);
		return -13;
	}	
	// 读取DDE信息
	//int group, machine;
	if(v_programtype != SYSTEM_SLEUTH)
		m_ddecount = ReadDdeHost(group, machine, v_ddelist);
	else
	{
		DDELIST* pDdeHost = &v_ddelist;
		if (pDdeHost->ip[0] != 0)
		{
			pDdeHost->pNext = new DDELIST;
			pDdeHost = pDdeHost->pNext;
		}
		strcpy(pDdeHost->ip, "127.0.0.1");
		// 读线程数(一个dde只有一套文件)
		pDdeHost->threadnum = 1;

		m_ddecount ++;
		v_ddethreadnum++;
#ifndef _DEBUG
	getallmac();
#endif

	}
	if (m_ddecount <= 0)
	{
		WriteLog("dce", "没有取到DDE的配置参数!!");
		CloseHandle(v_AppMutexH);
		return -16;
	}
	if (v_programtype == SYSTEM_SLEUTH)
	{
		if (v_ddelist.spyaddr.IsEmpty())
		{
			WriteLog("dce", "soft init error4!!");
			ExitDce(4);
		}
	}

//	WriteLog("dce","CHECK-HARD -0 \n");
//	printf("--------------------------------\n");
//#ifndef		_DEBUG
//	CheckHard(__DATE__, NULL, 0, NULL);
//#endif

	//读取基站对照表，入hash
/*
	v_imsiphone.InitHashTable(200000);
	

	CString imsiphonepath;
	imsiphonepath.Format("e:\\netspy\\para\\imsiphone.csv");
	if(access(imsiphonepath,0)==0)
	{
		WriteLog("dce","正在读入手机号IMSI号对应关系表");
		
		long fieldnum=0;
		ifstream fin(imsiphonepath);  
		string s;  
		while(getline(fin,s))
		{   
			CString lines;
			CStringArray field;
			lines.Format("%s",s.c_str());
			
			Splitstr(lines,field,',');
			
			if(field.GetSize()==2)
			{
				v_imsiphone.SetAt(field[1],field[0]);
			}
			
			fieldnum++;
		}
		fin.close();
		WriteLog("dce","共读入%d条",v_imsiphone.GetCount());
	}
*/
	char *pathwifi = "e:\\appserv";
	if(access(pathwifi,0) !=0 )
	{
		mkdir(pathwifi);
		pathwifi = "e:\\appserv\\www";
		if(access(pathwifi,0) !=0 )
		{
			mkdir(pathwifi);
			pathwifi = "e:\\appserv\\www\\wifimac";
			if(access(pathwifi,0) !=0 )
			{
				mkdir(pathwifi);
			}
		}
	}

	PhoneNumberLocation::Instance();
	installed::Installed::InitApkAppMap();               //安装包程序 包名 初始化
	shopping::Shopping::InitIdentityCardAddressCodeMap();//地址码 地址名 初始化
	HistoryRecord::LoadWiFiRouter();

	forum::BaiDuTieBa::ReadBDUSS();
	CStdioFile cfile;
	
	m_basicinfomap.InitHashTable(201119);
	if(cfile.Open(GPRSBASICINFO,CFile::modeRead|CFile::shareDenyWrite))
	{
		CString buf;
		int icount =0;
		CString infokey;
		BasicInfoStruct m_lpstructinfo;
		while(cfile.ReadString(buf))
		{
			if (icount>0)
			{
				CStringArray basicinfoarr;
				Splitstr(buf,basicinfoarr,',');
				int len = basicinfoarr.GetSize();
				//只需要第1,2,3,4,5,6字段
				if (len > 6)
				{
					infokey.Format("%s%s",basicinfoarr[0],basicinfoarr[1]);
					m_lpstructinfo.name = basicinfoarr[2];
					m_lpstructinfo.address = basicinfoarr[3];
					m_lpstructinfo.longitude = basicinfoarr[4];
					m_lpstructinfo.latitude = basicinfoarr[5];

					m_basicinfomap.SetAt(infokey,m_lpstructinfo);
				}
				
			}
			icount++;
		}
		WriteLog("dce","WCDMA ----- 共读入%d条",m_basicinfomap.GetCount());
		cfile.Close();
	}
	

	NatAuth *na = NatAuth::GetInstance();
	na->LoadAllMap();

	if (_access("v:\\netspy\\statdata\\emlfrdshp\\",0) != 0)
	{
		::mkdir("v:\\netspy\\statdata\\emlfrdshp\\");
	}

	if (_access("v:\\netspy\\statdata\\esq\\",0) != 0)
	{
		::mkdir("v:\\netspy\\statdata\\esq\\");
	}


	if(g_dbgSwitch.m_bkfirstz)
	{
		/// \\\\ddeip\\v$\\netspy\\dceip\\200*.idx 200*.file
		/// e:\\netspy\bkdde\\errno{n}\\ddeip\\localip\\200*.idx 200*.file
		/// e:\\netspy\\bkdde\\err.ini
		//[err]
		//next=错误序号(default 1)
		//max=最大记录数量(default 10)
		//rectm%d=YYYY-mm-dd HH:MM:SS
		//recnum%d=记录的文件数量

		int errmax = GetPrivateProfileInt("err", "max", 10, "e:\\netspy\\bkzdisk\\err.ini");
		int erridx = GetPrivateProfileInt("err", "next", 1, "e:\\netspy\\bkzdisk\\err.ini");
		WriteLog("error", "已记录错误%d次, 最大记录%d次.", erridx, errmax);
		WritePrivateProfileString("err", "next", CStringEx::GetStringEx("%d", erridx), "e:\\netspy\\bkzdisk\\err.ini");
		if(erridx < errmax)
		{
			int totalrec = 0;
			CString srcDir, dstDir, str;
			DDELIST* pDdeHost = &v_ddelist;
			while(pDdeHost->ip[0] != 0)
			{
				srcDir.Format("\\\\%s\\v$\\netspy\\%s\\1\\", pDdeHost->ip, v_localip);
				dstDir.Format("e:\\netspy\\bkzdisk\\idx_%02d\\%s\\", erridx, pDdeHost->ip);
				CreateDir(dstDir);
				
				CFileFind pathfind;
				BOOL isfind = pathfind.FindFile(srcDir+"20*");
				while (isfind)
				{
					isfind = pathfind.FindNextFile();
					str = pathfind.GetFileName();
					if(CopyFile(srcDir+str, dstDir+str, FALSE))
						totalrec ++;
				}
				pathfind.Close();

			
				if(pDdeHost->pNext)
					pDdeHost = pDdeHost->pNext;
				else
					break;
			}
			str.Format("recnum%d", erridx);
			WritePrivateProfileString("err", str, CStringEx::GetStringEx("%d", totalrec), "e:\\netspy\\bkzdisk\\err.ini");
			str.Format("rectm%d", erridx);
			WritePrivateProfileString("err", str, CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"), "e:\\netspy\\bkzdisk\\err.ini");
			WritePrivateProfileString("err", "next", CStringEx::GetStringEx("%d", erridx+1), "e:\\netspy\\bkzdisk\\err.ini");
		}
	}

	// 读取DCE列表
// 	DCELIST dcelist;
// 	ReadDceHost(dcelist);
// 	
// 	WSADATA wsaData;
// 	if (WSAStartup(0x202,&wsaData) == SOCKET_ERROR)
// 	{
// 		WriteLog("dce", "WSAStartup failed with error %d",WSAGetLastError());
// 		WSACleanup();
// 	}
// 	int iDCE = 0;
// 	DCELIST* pDceHost = &dcelist;
// 	while(pDceHost != NULL)
// 	{
// 		if(Swap4Byte(pDceHost->uip) != inet_addr(v_localip))
// 		{
// 			v_DceTeSpread.dceTe[iDCE].dceradius = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
// 			struct sockaddr_in* saServer = (struct sockaddr_in*)&v_DceTeSpread.dceTe[iDCE].dce_addr;
// 			saServer->sin_port = htons(8018);
// 			saServer->sin_family = AF_INET;
// 			saServer->sin_addr.s_addr = Swap4Byte(pDceHost->uip);
// 
// 			iDCE++;		// 计数
// 		}
// 		pDceHost = pDceHost->pNext;
// 	}
// 	WriteLog("dce", "需通讯的DCE机器%d个", iDCE);
// 	v_DceTeSpread.count = iDCE;

	// 读取IndexServer服务器信息
	if (ReadDseServerHost()<=0)
	{
		WriteLog("dce", "没有取到IndexServer的配置参数!!");
		CloseHandle(v_AppMutexH);
		return -17;
	}
	
	// 读取参数服务器信息(CS)
	if (0 >= ReadParaServerHost())
		return -18;
	WriteLog("dce", "读取参数服务器信息(CS)");
	// 更新参数
	UpdatePara();

	//读取探针的id与名称对照表，放到文件para\\pos.ini
//	Read_tz_pos();

// RBPEND
	extern IpQueryLib	k_IpQueryTable;	// IP区域表
//	CString sipaddr = k_IpQueryTable.QueryIpAddr("116.052.009.221");
	DWORD converip=ConvertIP("116.052.009.221");
	CString sipaddr = k_IpQueryTable.QueryIpAddr(converip);
	WriteLog("dce", "ConvertIP 116.052.009.221 =%s",sipaddr);

//WriteLog("dce", "116.052.009.221ip所属区为：%s!!",sipaddr);
	memset(szHostName, 0, sizeof(szHostName));
	IMWGetPrivateProfileString("Language","Language","",szHostName,sizeof(szHostName),CONFIGFILE);
	if (stricmp(szHostName, "true") == 0)
		v_bLanguage = TRUE;
	else
		v_bLanguage = FALSE;
	if (v_bLanguage)
	{
		int eWordRatio;
		int sWordRatio;
		eWordRatio = GetPrivateProfileInt("Language", "eWordRatio", 6, CONFIGFILE);
		if (eWordRatio<6)
			eWordRatio=6;
		sWordRatio = GetPrivateProfileInt("Language", "sWordRatio", 3, CONFIGFILE);
		if (sWordRatio<3)
			sWordRatio=3;

		memset(szHostName, 0, sizeof(szHostName));
		IMWGetPrivateProfileString("Language","WordFlag","",szHostName,sizeof(szHostName),CONFIGFILE);
		// 读取语种词库
		WriteLog("dce", "eWordRatio=%d sWordRatio=%d WordFlag=%s", eWordRatio, sWordRatio, szHostName);
		if (szHostName[0] != 0)
			InitLanguageDice(szHostName, eWordRatio, sWordRatio);
		else
			InitLanguageDice(NULL, eWordRatio, sWordRatio);

		// TEST
//		FILE * fs=0;
//		fs=fopen("x:\\xxx\\a.httpup.txt", "rb");
//		if(fs > 0)
//		{
//			UINT readsize= _filelength(fs->_file);
//			char* ptext = new char[readsize+50];
//			readsize=fread(ptext,1,readsize,fs);
//			fclose(fs);
//			
//			CString ttstr;
//			char *p = ttstr.GetBufferSetLength(readsize);
//			if (p != NULL)
//			{
//				memcpy(p, ptext, readsize);
//				//ttstr.ReleaseBuffer(readsize);
//			}
//			DWORD begin = GetTickCount();
//			LPCTSTR L = CheckSPL(ptext, readsize);
//			DWORD end = GetTickCount();
//			delete[] ptext;
//		}
		
	}
	WriteLog("dce", "更新参数完成");
	// 检查服务器的积压状态
	g_DseServer.CheckServerJiyaState();
	WriteLog("dce", "检查服务器的积压状态");

	//先创建运行标志，然后加载程序退出时的认证数据
	CreateFlgFile();
	
//	WriteLog("dce", "dddddddddddd完成");

	// 删除临时的流文件
	OutPutTmpStrramFile();

	//检查是否需要些认证
	if(_access("e:\\netspy\\flg\\noWriteAAA.flg",0)==0)
	{
		g_processAAA = 0; //不写认证
		WriteLog("dce","本探针不输出认证数据");
	}else{
		g_processAAA = 1; //写认证
		WriteLog("dce","本探针输出认证数据");
	}

	//播放器文件的加密用的密钥用RSA加密放在播放器文件首部
	LodeKeys(IDR_BIN_DATA1,&g_privateKey);
	char teakey[16];
	CreateValue(teakey);

	memcpy(g_test, teakey, 16);

	unsigned int aEnLen = 0;
	PrivateEncrypt(NULL, &aEnLen, teakey, 16, &g_privateKey);
	char *paEnBuf = new char[aEnLen+4];
	PrivateEncrypt(paEnBuf, &aEnLen, teakey, 16, &g_privateKey);
		
	g_encodedkeybuf=paEnBuf;
	g_encodedlen=aEnLen;


	// 初始化播放文件的密钥
	//memcpy(g_test, "\x22\x19\x4A\xFD\x87\x40\x4E\x87\xF9\x6A\xA6\xD3\xC1\x0A\xDD\xCB", 16);




	// 调入插件
	DceMission		DceMissionArr[PRONUM];
	int				DceMissionNum = RunDceMission(DceMissionArr, PRONUM);
	if ((DceMissionNum<=0) || (DceMissionNum>PRONUM))
	{
		WriteLog("dce", "还原线程失败(DceMissionNum = %d)!!", DceMissionNum);
		CloseHandle(v_AppMutexH);
		return -19;
	}

	// 开启收包线程
	HANDLE	hRecv = CreateThread(NULL, 0, RecSpreadPacket, NULL, FALSE, 0);
	CloseHandle(hRecv);

	HANDLE hTzUnit = CreateThread(NULL, 0, UpdateTzUnit, NULL, FALSE, 0);
	CloseHandle(hTzUnit);

	//MAC地址服务线程
	HANDLE hTMac = CreateThread(NULL, 0, MacFileServiceThread, NULL, FALSE, 0);
	CloseHandle(hTMac);
	
//	WriteLog("dce", "eeeeeeeeeeee完成");
	// 进入主循环
	RunMainLoop();

	InterlockedExchange((long*)&v_DceRunFlg, 0);
	WSACleanup();
	CloseHandle(v_AppMutexH);
	Sleep(3000);
	return 0;
}

void RunMainLoop() // 主循环
{
// 	InterlockedExchange((long*)&v_flag,
// 		Swap4Byte(GetTickCount()) ^ 0x5A5A5A5A);

	time_t runtm = time(0);			// DCE程序的运行时间
	time_t updateparatm = runtm;	// DCE更新参数的时间
	time_t crtlogtm = runtm;		// DCE创建日志路径的时间
	time_t Chkrdftm = runtm;			// DCE检查DDE积压的时间
	time_t natauthtm = runtm;

	time_t ChkDsetm = (runtm/60) * 60;			// DCE检查各DSE是否积压
//	time_t ExportAAAtm = 0;			// DCE导出认证的时间

	while(v_DceRunFlg)
	{
int error=0;
		try
		{
			time_t now = time(0);
#ifndef _DEBUG
	//		if(CheckRunState())
#endif
			{
				CreateFlgFile();
			}
error=1;			
			if(_access("e:\\netspy\\flg\\reverse.flg",0)!=0)
			{
				WriteLog("dce", "%s, dce file no find!!", "e:\\netspy\\flg\\reverse.flg");
				return;
			}

			::Sleep(6000);
			if (_access("e:\\netspy\\flg\\checkip.flg",0)==0)
			{
				FILE* fp;
				fp=fopen("e:\\netspy\\flg\\checkip.flg","rb");

				if (fp!=NULL)
				{
					UINT readsize= _filelength(fp->_file);
					char* ptext = new char[readsize+50];
					memset(ptext,0,readsize+50);
					readsize=fread(ptext,1,readsize,fp);
					fclose(fp);
					extern IpQueryLib	k_IpQueryTable;	// IP区域表
					CString sipaddr = k_IpQueryTable.QueryIpAddr(ptext);
					WriteLog("dce", "ip %s 所属区为：%s!!",ptext,sipaddr);
					delete[] ptext;
				}
				DeleteFile("e:\\netspy\\flg\\checkip.flg");
			}

			if (abs(now-natauthtm) >= 60)
			{
				NatAuth *na = NatAuth::GetInstance();
				na->LoadAddMap();
				na->OutputAllMap();
				natauthtm = now;
			}
			if (abs(now-Chkrdftm)>=5*60)
			{
// 				DWORD heapsize=0;
// 				DWORD emptysize=0;
// 				int   blockcount=0;
// 				HANDLE Heap = GetProcessHeap();
// 				if (Heap!=NULL)
// 				{
// 					if(!GetHeapInfo(Heap, heapsize, emptysize, blockcount))
// 					{
// 						heapsize = emptysize = blockcount = 0;
// 					}
// 					WriteLog("dce", "当前内存状态: heapco:%d  heapsize:%u(%u)",
// 						blockcount, heapsize, emptysize);
// 				}
				CheckDDeFile();
				Chkrdftm = now;
// VMPBEGIN
// #ifndef		_DEBUG
// 				InterlockedExchange((long*)&v_DogFlag, CheckMicroDog_5());
// #else
// 				v_DogFlag = 1;
// #endif
// 				if (v_DogFlag == 0)
// 				{
// 					WriteLog("dce", "flg = %d", v_DogFlag);
// 					InterlockedExchange((long*)&v_flag,
// 						Swap4Byte(GetTickCount() - 1000*1000) ^ 0x5A5A5A5A);
// 				}
// 				else
// 				{
// 					InterlockedExchange((long*)&v_flag,
// 						Swap4Byte(GetTickCount()) ^ 0x5A5A5A5A);
// 				}
// VMPEND		
			}
			if (abs(now-crtlogtm)>=10*60)
			{
				long dt = (now-runtm)/3600;
				WriteLog("dce", "DCE已经运行了%d小时", dt);
error=2;
				CreateLogPath(now);
				crtlogtm = now;
#ifndef		_DEBUG
				if (CheckDebug())
				{
					WriteLog("dce", "soft init error7");
					Goto_Ring0_2();
				}
#endif
error=3;
// 				if (v_programtype == SYSTEM_SLEUTH)
// 				{
// 					char ddepath[200];
// 					for (int threadnum = 2; threadnum<40; threadnum++)
// 					{
// 						sprintf(ddepath, "v:\\netspy\\127.0.0.1\\%d\\", threadnum);
// 						if (_access(ddepath, 0) != 0)
// 						{
// 							threadnum--;
// 							break;
// 						}
// 					}
// 					if (threadnum > v_ddethreadnum)
// 					{
// 						WriteLog("dce", "Find new dde thread(%d)!!", threadnum);
// 						InterlockedExchange((long*)&v_DceRunFlg, 0);
// 						break;
// 					}
// 				}
				
			}
error=4;
			if (abs(now-ChkDsetm) >= 60)
			{
				/// 更新调试开关
				g_dbgSwitch.Update();

				char keystr[40];
				memset(keystr, 0, sizeof(keystr));
				if (v_programtype == SYSTEM_SLEUTH)
				{
					memset(keystr, 0, sizeof(keystr));
					GetPrivateProfileString(
						"intercept", "arrpos",	"NO",
						keystr, 36,	POSCONFIGINI);
					if(stricmp(keystr, "NO")!=0)
					{
						if(v_ddelist.spyaddr.Compare(keystr) != 0)
						{
							v_ddelist.spyaddr.Format("%s",keystr);
							WriteLog("dce","截获点变化, 新截获点%s", keystr);
						}
					}
					else
						v_ddelist.spyaddr.Format("接口1");

				}
				else
				{
					IMWGetPrivateProfileString(
						"intercept", "arrpos",	"NO",
						keystr, 36,	ZONEPOSINI);
					if(stricmp(keystr, "NO")!=0)
					{
						if(v_ddelist.spyaddr.Compare(keystr) != 0)
						{
							v_ddelist.spyaddr.Format("%s",keystr);
							WriteLog("dce","截获点变化, 新截获点%s", keystr);
						}
					}
				}

// 				if (v_programtype == SYSTEM_ZONE)
// 				{
// 					char keystr[40];
// 					memset(keystr, 0, sizeof(keystr));
// 					IMWGetPrivateProfileString(
// 						"intercept", "arrpos",	"NO",
// 						keystr, 36,	ZONEPOSINI);
// 					if(stricmp(keystr, "NO")!=0)
// 					{
// 						if(v_ddelist.spyaddr.Compare(keystr) != 0)
// 						{
// 							v_ddelist.spyaddr.Format("%s",keystr);
// 							WriteLog("dce","截获点变化, 新截获点%s", keystr);
// 						}
// 					}
// 				}
// 				else
// 				{
// 					char keystr[40];
// 					memset(keystr, 0, sizeof(keystr));
// 					GetPrivateProfileString(
// 						"intercept", "arrpos",	"NO",
// 						keystr, 36,	POSCONFIGINI);
// 					if(stricmp(keystr, "NO")!=0)
// 					{
// 						if(v_ddelist.spyaddr.Compare(keystr) != 0)
// 						{
// 							v_ddelist.spyaddr.Format("%s",keystr);
// 							WriteLog("dce","截获点变化, 新截获点%s", keystr);
// 						}
// 					}
// 				}
error=5;
				ChkDsetm = (now / 60) * 60;
				g_DseServer.CheckServerJiyaState();
			}
			if (v_programtype != SYSTEM_SLEUTH)
			{
//				if (abs(now-updateparatm)>=1*60)
				{
error=6;
#ifndef		_DEBUG
					if (UpdatePara())
						updateparatm = now;
#else
					if (UpdatePara())
						updateparatm=now;
#endif
				}
			}
			else
			{
error=7;
//#ifndef		_DEBUG
				if (UpdatePara())
					updateparatm = now;
//#else
//				updateparatm = now;
//#endif
			}
		}
		catch(...)
		{
			WriteLog("dce", "RunMainLoop() Error!!error=%d",error);
		}
	}
}

void CreateLogPath(CTime nowtime)
{
	BOOL DelOldLog;
	if (nowtime == 0)
	{
		DelOldLog = TRUE;
		nowtime = CTime::GetCurrentTime();
	}
	else
	{
		DelOldLog = FALSE;
	}
	// 检查并创建日志路径(\\log\\dce)
	char logpath[200];
	sprintf(logpath, "%s\\log\\dce\\%s", (LPCTSTR)v_WorkPath, nowtime.Format("%Y%m%d"));
	if (_access(logpath, 0) != 0)
	{
		CreateDir(logpath);
	}
	
	int hour = nowtime.GetHour();
	if (hour == 23)// || DelOldLog)
	{
		// 检查并创建两个小时后的路径日期路径(\\log\\dce\\yyyymmdd)
		nowtime += CTimeSpan(0, 2, 0, 0);
		sprintf(logpath, "%s\\log\\dce\\%s", v_WorkPath, nowtime.Format("%Y%m%d"));
		if (_access(logpath, 0) != 0)
		{
			mkdir(logpath);
			DelOldLog = TRUE;
		}
	}
	if (DelOldLog)
	{
		// 检查并删除以前的路径
		nowtime -= CTimeSpan(7, 2, 0, 0);
		strcpy(logpath, nowtime.Format("%Y%m%d"));
		CFileFind pathfind;
		BOOL isfind = pathfind.FindFile(v_WorkPath+"\\log\\dce\\20*");
		while (isfind)
		{
			isfind = pathfind.FindNextFile();
			CString str = pathfind.GetFileName();
			if (str.Compare(logpath) <= 0)
			{
				char dirname[300];
				sprintf(dirname, "rd \"%s\\log\\dce\\%s\" /s /q", v_WorkPath, str);
				system(dirname);
			}
		}
	}
}

#ifdef		ADD_RUNSTATE_INFO
void DumpRunState(DWORD pro)
{
	// pro
	// http		1
	// imsg		2
	// smtp		4
	// pop3		8
	if ((pro & 0xF) == 0)
		return;
	CString str;
	SYSTEMTIME nowtm;
	GetLocalTime(&nowtm);
	str.Format("%04d-%02d-%02d %02d:%02d:%02d\t",
			nowtm.wYear, nowtm.wMonth, nowtm.wDay,
			nowtm.wHour, nowtm.wMinute, nowtm.wSecond);
	char fname[MAX_PATH];
	sprintf(fname, "%s\\log\\dce\\%04d%02d%02d\\",
			v_WorkPath, nowtm.wYear, nowtm.wMonth, nowtm.wDay);
	if (_access(fname, 0) != 0)
		return ;
	strcat(fname, "runstate.log");
	CFile logfile;
	for (int i = 0; i < 3; i++)
	{
		logfile.Open(fname, CFile::modeWrite|CFile::modeCreate|CFile::modeNoTruncate);
		if (logfile.m_hFile != CFile::hFileNull)
			break;
		else if (i >= 2)
			return;
		else
			::Sleep(50);
	}
	// 判断文件大小
	// 写文件
	logfile.SeekToEnd();
	DWORD flen = logfile.GetLength();
	if (flen < 1024*1024)		// 500K
	{
		CString strste;
		// http
		if (pro & 1)
		{
			strste = v_httprunstate.DumpState();
			if (!strste.IsEmpty())
			{
				logfile.Write((LPCTSTR)str, str.GetLength());
				logfile.Write("http run state\r\n", sizeof("http run state\r\n")-1);
				logfile.Write((LPCTSTR)strste, strste.GetLength());
			}
		}

		// imsg
		if (pro & 2)
		{
			strste = v_imsgrunstate.DumpState();
			if (!strste.IsEmpty())
			{
				logfile.Write((LPCTSTR)str, str.GetLength());
				logfile.Write("imsg run state\r\n", sizeof("imsg run state\r\n")-1);
				logfile.Write((LPCTSTR)strste, strste.GetLength());
			}
		}

		logfile.Write("\r\n\r\n", 4);
	}
	logfile.Close();
	return;
}
#endif

void CheckDDeFile()
{

	int arith = m_ReadDdeFileNum;
	int fcount = min(v_ddethreadnum*10, 40);
//	WriteLog("dce", "arith=%d\tfcount=%d", arith, fcount);
	if (_access(v_WorkPath+"\\flg\\dump.flg", 0) == 0)
	{
#ifdef		ADD_RUNSTATE_INFO
		DumpRunState(0xF);
#endif
		DeleteFile(v_WorkPath+"\\flg\\dump.flg");
	}
	if (arith < fcount)
	{
		try
		{
			// 文件处理有问题了, 程序退出
			BOOL bJiya = FALSE;
			int ddefilecount = printThreadFile(bJiya);
			//if (ddefilecount > max(fcount, 15))
			if (bJiya)
			{
				WriteLog("dce", "Error!! Read DDE File arithmometer = %d(%d)", fcount, arith);
				FILE *fp;
				fp = fopen(v_WorkPath+"\\flg\\bakdde.flg", "ab");
				if (fp != NULL)
					fclose(fp);	

#ifdef		ADD_RUNSTATE_INFO
				DumpRunState(0xF);
#endif
				if (_access(v_WorkPath+"\\flg\\noreset.flg", 0) != 0)
					InterlockedExchange((long*)&v_DceRunFlg, 0);
			
				::Sleep(500);

			
			}
		}
		catch (...)
		{
			WriteLog("dce","CheckDDeFile catch error");
		}
	}
	// 清除计数器
#ifdef		PACKETUNM
	long readfnum = m_ReadDdeFileNum;
	long readpnum = m_TotalPacketNum;
	long readbnum = m_TotalByteNum;
	long readqnum = m_TotalQueueUnit2;
	long pushqnum = m_TotalQueueUnit1;
	WriteLog("dce", "read DDE File: %d     Packet: %d    Byte: %u, Push:%u, POP:%u",
		readfnum, readpnum, readbnum, pushqnum, readqnum);
#else
	long readfnum = InterlockedExchange(&m_ReadDdeFileNum, 0);
	long readpnum = InterlockedExchange(&m_TotalPacketNum, 0);
	long readbnum = InterlockedExchange(&m_TotalByteNum, 0);
	WriteLog("dce", "read DDE File: %d     Packet: %d    Byte: %u",
		readfnum, readpnum, readbnum);
#endif
}

void OutPutTmpStrramFile()
{
	SERVERHOST* pTmpServer = g_DseServer.m_pServer;
	char srcfname[MAX_PATH];
	char dstfname[MAX_PATH];
	int len;
	for(int i=0; i<g_DseServer.m_count; i++, pTmpServer++)
	{
		// 首先搜索并提交认证的临时文件
		HANDLE hfindfile;
		WIN32_FIND_DATA finddata;
		len = sprintf(srcfname, "%s\\CaseServer\\%s\\", v_datapath, pTmpServer->m_ip);
		memcpy(dstfname, srcfname, len);
		strcpy(srcfname+len, "tmp20*_aaa_*.file");
		hfindfile = FindFirstFile(srcfname, &finddata);
		if (hfindfile != INVALID_HANDLE_VALUE)
		{
			do
			{
				// *.file文件
				strcpy(srcfname+len, finddata.cFileName);
				strcpy(dstfname+len, finddata.cFileName+3);
				if (0 != rename(srcfname, dstfname))
				{
					Sleep(200);
					rename(srcfname, dstfname);
				}

				// *.idx文件
				int tlen = strlen(finddata.cFileName);
				memcpy(finddata.cFileName+tlen-4, "idx", 4);
				strcpy(srcfname+len, finddata.cFileName);
				strcpy(dstfname+len, finddata.cFileName+3);
				if (0 != rename(srcfname, dstfname))
				{
					Sleep(200);
					rename(srcfname, dstfname);
				}

			}while(FindNextFile(hfindfile, &finddata));
			FindClose(hfindfile);
		}

		// 最后清除剩余的Tmp文件
		sprintf(srcfname, "del %s\\IndexServer\\%s\\tmp*.*", v_datapath, pTmpServer->m_ip);
		system(srcfname);
		sprintf(srcfname, "del %s\\CaseServer\\%s\\tmp*.*", v_datapath, pTmpServer->m_ip);
		system(srcfname);
		sprintf(srcfname, "del %s\\WarnServer\\%s\\tmp*.*", v_datapath, pTmpServer->m_ip);
		system(srcfname);
	}
}


//MAC地址文件改名服务线程
DWORD WINAPI MacFileServiceThread(LPVOID n)
{
	while(1)
	{
		CTime t;
		CTimeSpan ts(0,0,0,1);

		CString precurrenttm;

		t=CTime::GetCurrentTime()-ts;
 		precurrenttm.Format("%04u%02u%02u%02u%02u%02u",t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute(),t.GetSecond());

		CString currenttm;
		SYSTEMTIME now;
		GetLocalTime(&now);

		currenttm.Format("%04d%02d%02d%02d%02d%02d",
			now.wYear, now.wMonth, now.wDay, now.wHour,
			now.wMinute, now.wSecond);

		CString tmppath, srcfile, dstfile;
		tmppath.Format("e:\\netspy\\basedata\\tmpmac\\");

		HANDLE hfindfile;
		WIN32_FIND_DATA finddata;
		hfindfile = FindFirstFile(tmppath+"*.tmp", &finddata);
		if (hfindfile != INVALID_HANDLE_VALUE)
		{
			try
			{
				do
				{
					srcfile.Format("%s%s", tmppath, finddata.cFileName);
					dstfile = srcfile;
					dstfile.Replace(".tmp",".bcp");

					if(srcfile.Find(currenttm)==-1 && srcfile.Find(precurrenttm)==-1)
					{
						rename(srcfile, dstfile);
					}

				}while(FindNextFile(hfindfile, &finddata));		
			}
			catch (...)
			{
				WriteLog("dce", "ColletTotal2() Error!!");
			}
			FindClose(hfindfile);
		}

		Sleep(1000*30);
	}

	return 1;
}


