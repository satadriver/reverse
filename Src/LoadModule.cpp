#include "stdafx.h"
#include "Dce.h"
#include "ReadDdeFile.h"
#include "SocketSession.h"
#include "DceMission.h"
#include "aaa.h"				// 认证处理模块
#include "unip.h"				// UNIP处理模块
#include "smtp\\smtp.h"			// SMTP处理模块
#include "Pop3\\pop3.h"			// POP3处理模块
#include "ftp\\ftp.h"			// FTP, Telnet, p2p处理模块
#include "imsg\\imsg.h"
#include "media\\media.h"
#include "http\\http.h"

typedef DceProcotol* (WINAPI *CreateObjectFunction)(void);
typedef void (WINAPI *InitImsgProxyFunction)(void);
extern DWORD WINAPI DealQQinfo(LPVOID p);
//SubDceMission* m_pMission = NULL;
//typedef CMap<CString, LPCTSTR, SubDceMission*, SubDceMission*&> CDceMissionMap;

DWORD WINAPI DeleteDDEFile(LPVOID p);
DWORD WINAPI DelDDeBakFile(LPVOID p);
DWORD WINAPI ColletTotal(LPVOID p);
DWORD WINAPI DDeFileReadPorc(LPVOID p);


static LPCTSTR pDdefiletype[PRONUM] =\
{
	_T("aaa"),
	_T("http"),
	_T("pop3"),
	_T("media"),
	_T("ftp"),
	_T("imsg"),
	_T("smtp"),
	_T("unip"),
	_T("allnet"),
	_T("http2"),

};

UINT GetIdFormProtocol(LPCTSTR strprotocol)
{
	for (UINT i=1; i<=PRONUM; i++)
	{
		if (stricmp(strprotocol, pDdefiletype[i-1])==0)
			return i;
	}
	return 0;
}

/*****************  开辟还原线程  ******************/
int RunDceMission(DceMission* pDceMission, int size)
{
	ASSERT(pDceMission != NULL);
	int MissionNum = 0;
	DceMission *tpMiss = pDceMission;
	// 初始化任务
	// 认证
	tpMiss->m_pSubPro = (DceProcotol*)new AAAProtocol;
	tpMiss->m_ddefiletype = tpMiss->m_pSubPro->m_ddefiletype;
	MissionNum++;
	tpMiss++;

	// Http
	tpMiss->m_pSubPro = (DceProcotol*)new HttpProtocol(1);
	tpMiss->m_ddefiletype = tpMiss->m_pSubPro->m_ddefiletype;
	MissionNum++;
	tpMiss++;
	
	char httpstate[200];
	memset(httpstate, 0, 200);
	IMWGetPrivateProfileString("para","http","false",httpstate,sizeof(httpstate),CONFIGFILE);
	if(stricmp(httpstate, "double")==0)
	{
		// Http
		tpMiss->m_pSubPro = (DceProcotol*)new HttpProtocol(2);
		tpMiss->m_pSubPro->m_ddefiletype="http2";
		tpMiss->m_ddefiletype = tpMiss->m_pSubPro->m_ddefiletype;
		MissionNum++;
		tpMiss++;
	}

	// 即时消息
	tpMiss->m_pSubPro = (DceProcotol*)new ImsgProtocol;
	tpMiss->m_ddefiletype = tpMiss->m_pSubPro->m_ddefiletype;
	MissionNum++;
	tpMiss++;

	// 音视频
	tpMiss->m_pSubPro = (DceProcotol*)new MediaProtocol;
	tpMiss->m_ddefiletype = tpMiss->m_pSubPro->m_ddefiletype;
	MissionNum++;
	tpMiss++;

	// 未知协议IP
	tpMiss->m_pSubPro = (DceProcotol*)new UnipProtocol;
	tpMiss->m_ddefiletype = tpMiss->m_pSubPro->m_ddefiletype;
	MissionNum++;
	tpMiss++;

	// SMTP
	tpMiss->m_pSubPro = (DceProcotol*)new SmtpProtocol;
	tpMiss->m_ddefiletype = tpMiss->m_pSubPro->m_ddefiletype;
	MissionNum++;
	tpMiss++;

	// POP3
	tpMiss->m_pSubPro = (DceProcotol*)new Pop3Protocol;
	tpMiss->m_ddefiletype = tpMiss->m_pSubPro->m_ddefiletype;
	MissionNum++;
	tpMiss++;

	// FTP
	tpMiss->m_pSubPro = (DceProcotol*)new FtpProtocol;
	tpMiss->m_ddefiletype = tpMiss->m_pSubPro->m_ddefiletype;
	MissionNum++;
	tpMiss++;

	if((MissionNum <= 0) || (MissionNum > size))
	{
		return MissionNum;
	}

	// 为读DDE文件的线程分配DDE数据队列并开启读文件的线程
	DDEFILEINFO* pDdeFileInfo = new DDEFILEINFO[MissionNum*v_ddethreadnum];
	DDEFILEINFO* ptmp = pDdeFileInfo;
	DDELIST *tpdde = &v_ddelist;
	ASSERT(v_ddethreadnum > 0);
	for (int l = 0; l < v_ddethreadnum; l++)
	{
		ptmp = pDdeFileInfo+l;
		DDeInfo *tmpDDeinfo = new DDeInfo;
 		tmpDDeinfo->ddeip = tpdde->ip;
 		tmpDDeinfo->spyaddr = tpdde->spyaddr;
 		tmpDDeinfo->ddethnum = 1;
		tmpDDeinfo->pDdeFile = ptmp;
		tpMiss = pDceMission;
		for (int s = 0; s < MissionNum; s++)
		{
			// 初始化一个单元
			ptmp->m_fileState = 0;	
			ptmp->m_filetm = 0xFFFFFFFF;
			ptmp->m_ddefiletype = tpMiss->m_ddefiletype;
			ptmp->m_proid = GetIdFormProtocol(tpMiss->m_ddefiletype);
			if (ptmp->m_proid > 0)
			{
				ptmp->m_proid--;
			}
			if (s < MissionNum-1)
			{
				ptmp->m_pNext = ptmp+v_ddethreadnum;
			}
			else
			{
				ptmp->m_pNext = NULL;
			}
			tpMiss++;
			ptmp = ptmp->m_pNext;
		}
		// 创建DDE文件读取线程
		HANDLE H = CreateThread(NULL, 0, DDeFileReadPorc, tmpDDeinfo, 0, NULL);
		CloseHandle(H);
		tpdde = tpdde->pNext;
		if (l < 1)
		{
			::Sleep(1000);
		}
	}
	// 启动各个协议还原线程
	tpMiss = pDceMission;
	ptmp = pDdeFileInfo;
	for (int n = 0; n < MissionNum; n++)
	{
		HANDLE H = CreateThread(NULL, 0, DceMission::MissRunProc, tpMiss, 0, NULL);
		CloseHandle(H);
		tpMiss->m_pDdeFileInfo = ptmp;
		ptmp += v_ddethreadnum;
		tpMiss++;
	}

	// 开辟其他线程
	HANDLE TH = CreateThread(NULL, 0, DeleteDDEFile, 0, 0, NULL);
	CloseHandle(TH);
	
//	if ((v_programtype!=SYSTEM_SLEUTH) && v_stat)
	{
		//收集各个线程缓冲区数据，写入dbf中
		HANDLE STH = CreateThread(NULL, 0, ColletTotal, 0, 0, NULL);
		CloseHandle(STH);
	}

	//开启定时清理WEIBOMAP线程
	/*
	HANDLE STH = CreateThread(NULL, 0, CleanWeiboMap, 0, 0, NULL);
	CloseHandle(STH);
*/
 	HANDLE QQTH = CreateThread(NULL, 0, DealQQinfo, 0, 0, NULL);
 	CloseHandle(QQTH);
	HANDLE DTH = CreateThread(NULL, 0, DelDDeBakFile, 0, 0, NULL);
	CloseHandle(DTH);

	return MissionNum;

// 	memset(v_bddefilebak, 0, 4*16);
// 	CDceMissionMap missmap;
// 	CreateObjectFunction	fun = NULL;
// 	InitImsgProxyFunction	proxyfun = NULL;
// 	CString path;
// 	HANDLE hfindfile;
// 	WIN32_FIND_DATA finddata;
// #ifdef	_DEBUG
// //	char p[300];
// //	GetModuleFileName(NULL, p, sizeof(p));
// //	path = p;
// //	path.ReleaseBuffer(path.ReverseFind('\\'));
// 	hfindfile = FindFirstFile("*d.px", &finddata);
// #else
// 	path = v_RootPath+"\\bin\\";
// 	hfindfile = FindFirstFile(path+"dce*.px", &finddata);
// #endif
// 	int allprobak = 0;
// 	int procount = 0;
// 	if (v_programtype == SYSTEM_SLEUTH)
// 	{
// 		allprobak = (_access("x:\\xxx\\flg\\allpro.flg", 0) == 0)? 1 : 0;
// 	}
// 	if (hfindfile != INVALID_HANDLE_VALUE)
// 	{
// 		do
// 		{
// 			HMODULE H = LoadLibrary(path+finddata.cFileName);
// 
// 			if (H)
// 			{
// 				fun = (CreateObjectFunction)GetProcAddress(H, "CreateProcotolObjectv2");
// 				if (fun)
// 				{
// 					DceProcotol* p = fun();
// 					if (p != NULL)
// 					{
// 						if ((p->m_ddefiletype != NULL) || ((p->m_ddefiletype[0] != 0)))
// 						{
// 							SubDceMission *pMiss=NULL;
// 							if (!missmap.Lookup(p->m_ddefiletype, pMiss))
// 							{
// 								pMiss = new SubDceMission;
// 								pMiss->m_pSubPro = NULL;
// 								pMiss->m_ddefiletype = p->m_ddefiletype;
// 								if ((m_pMission==NULL) || (stricmp(pMiss->m_ddefiletype, "aaa")==0))
// 								{
// 									pMiss->m_pNextMiss = m_pMission;
// 									m_pMission = pMiss;
// 								}
// 								else
// 								{
// 									pMiss->m_pNextMiss = m_pMission->m_pNextMiss;
// 									m_pMission->m_pNextMiss = pMiss;
// 								}
// 								missmap.SetAt(p->m_ddefiletype, (SubDceMission*)pMiss);
// 							}
// //							p->m_pNext = pMiss->m_pSubPro;
// 							pMiss->m_pSubPro = p;
// 							if (stricmp(pMiss->m_ddefiletype, "imsg") == 0)
// 							{
// 								// 初始化IMSG代理函数
// 								proxyfun = (InitImsgProxyFunction)GetProcAddress(H, "InitImsgProxyFunction");
// 								if (proxyfun!=NULL)
// 									proxyfun();
// 							}
// 							WriteLog("dce", "Load %s OK!!", finddata.cFileName);
// 						}
// 						else
// 						{
// 							delete p;
// 							FreeLibrary(H);
// 						}
// 					}
// 					else
// 					{
// 						FreeLibrary(H);
// 					}
// 				}
// 				else
// 				{
// 					FreeLibrary(H);
// 				}
// 			}
// 		}while(FindNextFile(hfindfile, &finddata));
// 		FindClose(hfindfile);
// 	}

// 为读DDE文件的线程分配DDE数据队列并开启读文件的线程
// 	DDEFILEINFO* pDdeFileInfo = new DDEFILEINFO[missmap.GetCount()*v_ddethreadnum];
// 	DDEFILEINFO* ptmp = pDdeFileInfo;
// 	DDELIST *tpdde = &v_ddelist;
// 	ASSERT(v_ddethreadnum > 0);
// 	for (int l = 0; l < v_ddethreadnum; l++)
// 	{
// 		ptmp = pDdeFileInfo+l;
// 		DDeInfo *tmpDDeinfo = new DDeInfo;
//  		tmpDDeinfo->ddeip = tpdde->ip;
//  		tmpDDeinfo->spyaddr = tpdde->spyaddr;
//  		tmpDDeinfo->ddethnum = 1;
// 		tmpDDeinfo->pDdeFile = ptmp;
// 		SubDceMission *tpMiss = m_pMission;
// 		for (int s = 0; s < missmap.GetCount(); s++)
// 		{
// 			// 初始化一个单元
// //			ptmp->datatime = 0xFFFFFFFF;
// 			ptmp->m_fileState = 0;	
// 			ptmp->m_filetm = 0xFFFFFFFF;
// //			ptmp->nextfiletm =  0xFFFFFFF;
// 			ptmp->m_ddefiletype = tpMiss->m_ddefiletype;
// 			ptmp->m_proid = GetIdFormProtocol(tpMiss->m_ddefiletype);
// 			if (ptmp->m_proid>0)
// 				ptmp->m_proid--;
// 			if (s < missmap.GetCount()-1)
// 				ptmp->m_pNext = ptmp+v_ddethreadnum;
// 			else
// 				ptmp->m_pNext = NULL;
// 			tpMiss = (SubDceMission*)tpMiss->m_pNextMiss;
// 			ptmp = ptmp->m_pNext;
// 		}
// 		// 创建DDE文件读取线程
// 		HANDLE H = CreateThread(NULL, 0, DDeFileReadPorc, tmpDDeinfo, 0, NULL);
// 		CloseHandle(H);
// 		tpdde = tpdde->pNext;
// 		if (l<1)
// 			::Sleep(1000);
// 	}
// 	
// 	ptmp = pDdeFileInfo;
// 	// 启动各个协议还原线程
// 	SubDceMission *pMiss = m_pMission;
// 	while (pMiss!=NULL)
// 	{
// 		HANDLE H = CreateThread(NULL, 0, SubDceMission::MissRunProc, pMiss, 0, NULL);
// 		CloseHandle(H);
// //		if (stricmp(pMiss->m_ddefiletype, "unip") == 0)
// //			DceMission::MissRunProc(pMiss);
// 		pMiss->m_pDdeFileInfo = ptmp;
// 		ptmp += v_ddethreadnum;
// 		pMiss = (SubDceMission*)pMiss->m_pNextMiss;
// 	}
// 
// 
// #ifndef		_DEBUG
// //	CLEAR_END		// 代码加密结束
// //	PATCH_END	
// //	ENCODE_END
// #endif
// 	return missmap.GetCount();	// 返回成功调入的插件的个数
}

// 如果某些协议的插件没有调入则用这个线程删除dde上这些协议的文件
DWORD WINAPI DeleteDDEFile(LPVOID p)
{
// 	time_t checktime = time(0);
// 	DceMission* pMiss = m_pMission;
// 	UINT promark = (1<<PROTOCOLCOUNT)-1;
// 	while (pMiss)
// 	{
// 		UINT id = GetIdFormProtocol(pMiss->m_ddefiletype);
// 		if (id > 0)
// 		{
// 			promark &= ~(1<<(id-1));
// 		}
// 		pMiss = pMiss->m_pNextMiss;
// 	}
// 	if (v_programtype != SYSTEM_SZFJ)
// 		promark &= ~(1<<(PROTOCOLCOUNT-1));	// 系统不是数字福建就不处理Allnet数据
// 	
// 	if (promark == 0)	// 插件全部被调入
// 		return 0;
// 
// 	char path[300];
// 	while (v_DceRunFlg)
// 	{
// 		HANDLE hfindfile=INVALID_HANDLE_VALUE;
// 		WIN32_FIND_DATA finddata;
// 		try
// 		{
// 			time_t now = time(0);
// 			if (abs(now-checktime)<5)
// 			{
// 				::Sleep(2000);
// 				continue;
// 			}
// 			checktime = now;
// 			DDELIST* tmpddelst=&v_ddelist;
// 			while(tmpddelst!=NULL)
// 			{
// 				DWORD m = 1;
// 				int stlen;
// 				if (stricmp(tmpddelst->ip, v_localip) == 0)
// 				{
// 					stlen = sprintf(path, "v:\\netspy\\%s\\1\\", v_localip);
// 				}
// 				else
// 				{
// 					if (_access("x:\\xxx\\flg\\localdde.flg", 0) == 0)
// 					{
// 						stlen = sprintf(path, "v:\\netspy\\%s\\1\\", tmpddelst->ip);
// 					}
// 					else
// 					{
// 						stlen = sprintf(path, "\\\\%s\\v$\\netspy\\%s\\1\\",
// 							tmpddelst->ip, v_localip);
// 					}
// 				}
// 				for (int i = 0; i < PROTOCOLCOUNT; i++)
// 				{
// 					if (promark & m)
// 					{
// 						sprintf(path+stlen, "20*.%s", pDdefiletype[i]);
// 						DWORD ctm = GetTickCount();
// 						hfindfile = FindFirstFile(path, &finddata);
// 						if (hfindfile != INVALID_HANDLE_VALUE)
// 						{
// 							memset(path+stlen, 0, 4);
// 							do
// 							{
// 								strcpy(path+stlen, finddata.cFileName);
// 								DeleteFile(path);
// 							}while(FindNextFile(hfindfile, &finddata));
// 							FindClose(hfindfile);
// 							hfindfile = INVALID_HANDLE_VALUE;
// 						}
// 						else
// 						{
// 							if (GetTickCount()-ctm>5000)
// 								break;		// 这个dde不通
// 						}
// 					}
// 					m *= 2;
// 				}
// 				tmpddelst = tmpddelst->pNext;
// 			}
// 		}
// 		catch(...)
// 		{
// 			WriteLog("dce", "DeleteDDEFile() Error!!");
// 			if(hfindfile!=INVALID_HANDLE_VALUE)
// 				FindClose(hfindfile);
// 			hfindfile=INVALID_HANDLE_VALUE;
// 		}
// 	}
	return 0;
}

// 删除备份的dde文件
DWORD WINAPI DelDDeBakFile(LPVOID p)
{
	CString bakddeflg;
	bakddeflg.Format("%s\\flg\\bakdde.flg", v_WorkPath);
	if (_access(bakddeflg, 0) == 0)
	{
		DeleteFile(bakddeflg);
		if(_access(v_datapath+"\\ddebak\\", 0) == 0)
		{
			char cmd[200];
			system(CStringEx::GetStringEx("rd %s /s /q", v_DdeBakPath));
			rename(v_datapath+"\\ddebak", v_DdeBakPath);
			char rarfile[50];
			memset(cmd, 0, 4);
			for (int i = 1; i <= 9; i++)
			{
				sprintf(rarfile, "%s%d.rar", v_DdeBakPath, i);
				if (_access(rarfile, 0) != 0)
				{
					sprintf(cmd, "%s\\bin\\rar.exe a -m5 -hpinetsifter20021001 -r -y -df %s %s",
						v_RootPath, rarfile, v_DdeBakPath);
					break;
				}
			}
			if (cmd[0] == 0)
			{
				sprintf(rarfile, "%s%d.rar", v_DdeBakPath, (time(0)%9)+1);
				DeleteFile(rarfile);
				sprintf(cmd, "%s\\bin\\rar.exe a -m5 -hpinetsifter20021001 -r -y -df %s %s",
					v_RootPath, rarfile, v_DdeBakPath);
			}
			system(cmd);
			WriteLog("dce", "打包ddebak目录成功目标文件%s", rarfile);
		}
	}
	bakddeflg.Empty();

	char baktm[PRONUM][20];
	const int	 deltm[PRONUM] = 
	{
		30*60/*aaa*/,		60*60/*http*/,	1*60*60/*pop3*/,	3*60*60/*media*/,
		24*60*60/*ftp telnet p2p*/,			30*60/*imsg*/,		1*60*60/*smtp*/,	
		30*60/*unip*/,		30*60/*allnet*/,60*60/*http*/
	};
	memset(baktm, 0, sizeof(baktm));
	time_t checktm = 0;
	time_t deletetm = 0;
	time_t chktmpdirtm = time(0);
	int procount = (v_programtype==SYSTEM_SZFJ)? PRONUM : PRONUM-1;
	int bakcount = 0;
	HANDLE hfindfile=INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA finddata;
	DWORD d = 0;
	while(v_DceRunFlg)
	{
		try
		{
			time_t now = time(0);
			// 检查各个协议的临时目录的垃圾文件
			if ((now - chktmpdirtm)>20*60)
			{
				d++;
				// 删除6个小时前的临时文件
				char tmpfile[200];
				for (int i=0; i<procount; i++)
				{
					time_t rubbfiletm = now-deltm[i];
					CString tmpstr;
					tmpstr.Format("%s:\\dce",g_ddepath);
					int len = sprintf(tmpfile, "%s\\tmp\\dce%d\\*.*",
						(d&1)? tmpstr : v_datapath, i+1);
					hfindfile = FindFirstFile(tmpfile, &finddata);
					if (hfindfile != INVALID_HANDLE_VALUE)
					{
						len -= 3;
						do
						{
							if (!(finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
							{
								CTime ftm(finddata.ftCreationTime);
								if (ftm.GetTime() < rubbfiletm)
								{
									memset(tmpfile+len, 0, 4);
									strcpy(tmpfile+len, finddata.cFileName);
									DeleteFile(tmpfile);
									WriteLog("dce", "删除垃圾文件%s", tmpfile);
								}
							}
						}while(FindNextFile(hfindfile, &finddata));
						FindClose(hfindfile);
						hfindfile = INVALID_HANDLE_VALUE;
					}	
				}
				chktmpdirtm = now;
			}
			if (abs(now - checktm)>=60)
			{
				// 检查是否备份Http
//				v_HttpDumpHttpPacket.InitDumpBuffer();
				checktm = now;
				bakcount = 0;
				// 读取各个协议的备份开关
				for (int i = 0; i < procount; i++)
				{
					int bakval = ::GetPrivateProfileInt(pDdefiletype[i], "bakfile", 0, "d:\\netspy\\para\\ddebakfile.ini");
					if (bakval<0)
						bakval = 0;
					else if (bakval>120)
						bakval = 120;
					if (bakval!=0)
						bakcount++;
					else if (baktm[i][0] != 'z')
					{
						memset(baktm[i], 'z', 16);
						memset(baktm[i]+16, 0, 4);
					}
					v_bddefilebak[i] = bakval;
				}
			}
			::Sleep(1000);
			if (_access("e:\\netspy\\flg\\nodelddebak.flg", 0) != 0)
			{
				if (bakcount==0)
				{
					if (_access(v_datapath+"\\ddebak", 0) == 0)
						system(CStringEx::GetStringEx("rd %s\\ddebak /s /q", v_datapath));
				}
				else
				{
					if (abs(now-deletetm)>3*60)
					{
						int filecount = 0;
						char path[300];
						deletetm = now;
						DDELIST *tmpDde = &v_ddelist;
						for (int i = 0; i<procount; i++)
						{
							if (v_bddefilebak[i]!=0)
							{
								GetTimeString(now-(v_bddefilebak[i]*5*60), "%Y%m%d%H%M", baktm[i], 20);
							}
							else
							{
								ASSERT(baktm[i][0] == 'z');
							}
						}
					
						while (tmpDde!=NULL)
						{
							int slen = sprintf(path, "%s\\ddebak\\%s\\1\\20*.*",
								v_datapath, tmpDde->ip);
							hfindfile = FindFirstFile(path, &finddata);
							if (hfindfile != INVALID_HANDLE_VALUE)
							{
								slen -= 6;
								memset(path+slen, 0, 4);
								do
								{
									filecount++;
									BOOL isDel = TRUE;
									UINT id = GetIdFormProtocol(finddata.cFileName+17);
									if (id>0)
									{
										if (strcmp(finddata.cFileName, baktm[id-1])>0)
											isDel = FALSE;
									}
									if (isDel)
									{
										strcpy(path+slen, finddata.cFileName);
										DeleteFile(path);
									}
								}while(FindNextFile(hfindfile, &finddata));
								FindClose(hfindfile);
								hfindfile = INVALID_HANDLE_VALUE;
							}
							tmpDde = tmpDde->pNext;
						}
						if (filecount==0)
						{
							system(CStringEx::GetStringEx("rd %s /s /q", v_DdeBakPath));
						}
					}
				}
			}
			::Sleep(29000);
		}
		catch(...)
		{
			WriteLog("dce", "DelDDeBakFile() Error!!");
			if (hfindfile!=INVALID_HANDLE_VALUE)
				FindClose(hfindfile);
			hfindfile = INVALID_HANDLE_VALUE;
		}
	}
	return 0;
}

//
DWORD WINAPI RecSpreadPacket(LPVOID)
{
	SOCKET Gsocket=socket(AF_INET,SOCK_DGRAM,0);
	int timeout = 1000;
	setsockopt(Gsocket,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(timeout) );	// 设置1秒超时
	struct sockaddr_in Glocal;
	ZeroMemory(&Glocal, sizeof(struct sockaddr));
	Glocal.sin_family=AF_INET;
	Glocal.sin_port=htons(8018);			//监听端口
	Glocal.sin_addr.s_addr=INADDR_ANY;		//本机
	int bok=bind(Gsocket,(struct sockaddr*)&Glocal,sizeof Glocal);
	DWORD buffer[4096];
	int len =sizeof(struct sockaddr);

	while (v_DceRunFlg)
	{
		//接收命令包
		len =sizeof(struct sockaddr);
		ZeroMemory(&Glocal, sizeof(struct sockaddr));
		ZeroMemory(buffer, sizeof(buffer));
		int reclen=recvfrom(Gsocket,
			(char*)buffer,
			sizeof(buffer),
			0,
			(struct sockaddr*)&Glocal,
			&len);
		if (reclen==SOCKET_ERROR)
		{
			Sleep(5);
		}
		else
		{
			buffer[reclen] = 0;
			if (memcmp(buffer,"externlimit",11)==0)//收到其他dce发来的重点人员非帐号扩线信息
			{
				AddExtenIp((char*)buffer,reclen);
			}
			else
			{
				DATABUFFER oneBuf;
				oneBuf.packetnum = 1;
				oneBuf.bufsize = reclen+1;
				oneBuf.pBuffer = (char*)buffer;
				//分析并返回数据
				AddHttpSpread(oneBuf);
			}
		}
	}
	return 0;
}
