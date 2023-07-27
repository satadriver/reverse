#include "stdafx.h"
#include <winsock.h>
#include "Dce.h"

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <Nb30.h>

BOOL sIPtouIP(const char *ip, unsigned int &uIP);
int getallmac();
//DCELIST* CheckLocalIP(DWORD pip, DCELIST* pdcelist);
//**********************************************************
//	处理命令行参数
//**********************************************************
void ProcessCmdLine(int argc, char **argv)
{

	BOOL isPass = FALSE;
	CString spyaddr;
	for (int i = 1; i < argc; i++)
	{
#ifdef	_DEBUG
		printf("参数%d、%s\n", i, argv[i]);
#endif
		if (_stricmp(argv[i], "-inet!!!!") == 0)	// 大系统启动密码
		{
			v_programtype = SYSTEM_002;
			v_DceRunFlg = TRUE;	
			return;
		}
		else if (_stricmp(argv[i], "-sleuth2005") == 0)	// 7启动密码
		{
			v_programtype = SYSTEM_SLEUTH;
		}
		else if (_stricmp(argv[i], "-netspystart") == 0)	// 7启动密码
		{
			v_programtype = SYSTEM_SLEUTH;
		}
// 		else if (memcmp(argv[i], "-P", 2) == 0)	// 截获位置
// 		{
// 			//MQ时读取E:\netspy\para\ Intercept.ini文件中的截获点
// 			char keystr[30];
// 			GetPrivateProfileString(
// 				"Intercept", "arrPos",	"NO",
// 				keystr, 20,	POSCONFIGINI);
// 			if(stricmp(keystr, "NO")!=0)
// 			{
// 				v_ddelist.spyaddr.Format("%s",keystr);
// 			}
// 			else
// 			{
// 				v_ddelist.spyaddr.Format("接口1");
// 			}
// 		}
	}
	if (v_programtype == SYSTEM_SLEUTH)
	{
		char keystr[40];
		memset(keystr, 0, sizeof(keystr));
		//读取配置文件 
		GetPrivateProfileString("intercept","arrpos","NO",keystr,36,POSCONFIGINI);
		if(stricmp(keystr, "NO")!=0)
			v_ddelist.spyaddr.Format("%s",keystr);
		else
			v_ddelist.spyaddr.Format("接口1");
		if (v_ddelist.spyaddr.IsEmpty())
		{
			WriteLog("dce", "soft init error4!!");
			ExitDce(4);
		}
// #ifndef		_DEBUG
// 		if (!CheckMicroDog_4())
// 		{
// 			WriteLog("dce", "soft init error6!!");
// 			v_workmode = 1;
// 		}
// #endif
	}
	else if (v_programtype != SYSTEM_002)
	{
		WriteLog("dce", "soft init error3!!");
		ExitDce(3);
	}
	v_DceRunFlg = TRUE;	
	return;
}

DCELIST* CheckLocalIP(DWORD pip, DCELIST* pdcelist)
{
	ASSERT(pip != 0);
	while (pdcelist != NULL)
	{
		if (pdcelist->uip == pip)
		{
			return pdcelist;
		}
		pdcelist = pdcelist->pNext;
	}
	return NULL;
}

int	ReadDceHost(DCELIST& dcelist)
{
	DCELIST *pdcehost = &dcelist;
	char Key[20];
	char Value[20];
	int dcecount = 0;
	for(int group=1; ; group++)
	{
		for(int machine=1; ; machine++)
		{
			sprintf(Key,"IP%02d%02d",group,machine);
			IMWGetPrivateProfileString("DCE",Key,"NO",Value,sizeof(Value),CONFIGFILE);
			if(lstrcmp("NO",Value) != 0)
			{
				unsigned int  uip;
				if (sIPtouIP(Value, uip))
				{
					if (pdcehost->uip != 0)
					{
						ASSERT(pdcehost->pNext == NULL);
						pdcehost->pNext = new DCELIST;
						pdcehost = pdcehost->pNext;
					}
					sprintf(Key,"USERAMDISK%02d%02d",group,machine);
					IMWGetPrivateProfileString("DCE",Key,"NO",Value,sizeof(Value),CONFIGFILE);
					if(stricmp("false", Value) == 0)
						pdcehost->useramdisk = FALSE;
					pdcehost->uip = uip;
					pdcehost->group = group;
					pdcehost->machine = machine;
					dcecount++;
				}
			}
			else
			{
				if(machine == 1)
					goto ENDREAD;
				break;
			}
		}
	}
	
ENDREAD:
	return dcecount;
}

int GetLocalHostIP(int& group, int& machine)
{
	group = machine = 0;
	memset(v_localip, 0, 16);
	char szHostName[128];
	int n=128;
	WSADATA wsaData;
	DWORD uip = 0;

	// 读DCE段
	DCELIST dcelist;
	if (ReadDceHost(dcelist)<=0)
	{
		WriteLog("dce", "没有发现dce的参数配置!!");
		return 0;
	}
	// 获取本机IP
	memset(szHostName, 0, 4);
	if (WSAStartup(0x202,&wsaData) != SOCKET_ERROR)
	{
		if( gethostname(szHostName, n) == 0 ) 
		{
			struct hostent * pHost;
			if (NULL != (pHost = gethostbyname(szHostName)))
			{
				if (pHost->h_length == 4)
				{
					BYTE* tp;
					for (int i = 0; i < 16; i++)
					{
						if (NULL != (tp = (BYTE*)pHost->h_addr_list[i]))
						{
							ASSERT(pHost->h_length == 4);
							//pip[i] = (*tp << 24) | (*(tp+1) << 16) | (*(tp+2) << 8) | *(tp+3);
							uip = (*tp << 24) | (*(tp+1) << 16) | (*(tp+2) << 8) | *(tp+3);
							DCELIST* pcurdce = CheckLocalIP(uip, &dcelist);
							if (pcurdce!=NULL)
							{
								sprintf(v_localip, "%u.%u.%u.%u", *tp, *(tp+1), *(tp+2), *(tp+3));
								group = pcurdce->group;
								machine = pcurdce->machine;
								V_WriteZ = pcurdce->useramdisk;
								WriteLog("dce", "取得合法的本机IP: %s 所在组:%d和号%d", v_localip, pcurdce->group, pcurdce->machine);
								return pcurdce->group;
							}
							
						}
						else
						{
							break;
						}
					}
				}
			}
		}
	}
	else
	{
		fprintf(stderr,"WSAStartup failed with error %d\n",WSAGetLastError());
	}
	// 尝试127.0.0.1
	uip = 0x7F000001;	// 127.0.0.1
	DCELIST* pcurdce = CheckLocalIP(uip, &dcelist);
	if (pcurdce!=NULL)
	{
		strcpy(v_localip, "127.0.0.1");
		group = pcurdce->group;
		machine = pcurdce->machine;
		WriteLog("dce", "取得合法的本机IP: %s 所在组:%d和号%d", v_localip, pcurdce->group, pcurdce->machine);
		return pcurdce->group;
	}
	
	WSACleanup();
	memset(v_localip, 0, 4);
	WriteLog("dce", "Error!! 没有取得合法的本机IP");
	return 0;
}

BOOL sIPtouIP(const char *ip, unsigned int &uIP)
{
	int num;
	int ipc1=256, ipc2=256, ipc3=256, ipc4=256;
	
	uIP = 0;	// IP串非法
	num = sscanf(ip,"%d.%d.%d.%d",&ipc1,&ipc2,&ipc3,&ipc4);
	
	if(num != 4)
		return FALSE;
	else
	{
		if(ipc1 < 0 || ipc1 > 255)
			return FALSE;
		if(ipc2 < 0 || ipc2 > 255)
			return FALSE;
		if(ipc3 < 0 || ipc3 > 255)
			return FALSE;
		if(ipc4 < 0 || ipc4 > 255)
			return FALSE;
		
		uIP = (ipc1 << 24) + (ipc2 << 16) + (ipc3 << 8) + ipc4;
		return uIP>0;
	}
}

int	ReadDdeHost(int ddegroup, int dcemachine, DDELIST& ddelist)
{
	char Key[20];
	char Key2[20];
	char Value2[20];
	char Value[20];
	DDELIST* pDdeHost = &ddelist;
	CString spyaddr = v_ddelist.spyaddr;
	for(int machine=1; ;++machine)
	{
		sprintf(Key,"IP%02d%02d",ddegroup, machine);
		sprintf(Key2, "dde%02d%02d_%02d", ddegroup, dcemachine, machine);
		IMWGetPrivateProfileString("DCE", Key2, "NO", Value2, sizeof(Value2), CONFIGFILE);
		IMWGetPrivateProfileString("DDE",Key,"NO",Value,sizeof(Value),CONFIGFILE);
		unsigned int  uip;
		if(/*(lstrcmp("NO", Value)!=0) && sIPtouIP(Value, uip) &&*/
			(lstrcmp("NO", Value2) != 0) && sIPtouIP(Value2, uip))
		{
			if (pDdeHost->ip[0] != 0)
			{
				pDdeHost->pNext = new DDELIST;
				pDdeHost = pDdeHost->pNext;
			}
			strcpy(pDdeHost->ip, Value2);
			// 读线程数(一个dde只有一套文件)
			pDdeHost->threadnum = 1;

			v_ddethreadnum++;
		}
		else
			break;
	}
	return machine-1;
}

int ReadParaServerHost()
{
	v_ParaServer.RemoveAll();
	char Key[20];
	char ServerIP[40];
	int count = 1;
	UINT uip = 0;
	for(;;++count)
	{
		sprintf(Key,"ip%d",count);
		IMWGetPrivateProfileString("CaseServer",Key,"NO",ServerIP,sizeof(ServerIP)-1,CONFIGFILE);
		if(lstrcmp("NO",ServerIP) != 0)
		{
			if (sIPtouIP(ServerIP, uip))
			{
				v_ParaServer.Add(ServerIP);
			}
		}
		else
			break;
	}
	int cscount = v_ParaServer.GetSize();
 	WriteLog("dce", "CS的总个数为:%d", cscount);
	return cscount;
}
/////////////////////////////////////////////
//id与名称对照文件名根格式如下
// x:\\xxx\\para\\pos.ini
//探针接受卡的名称
// [card]
// receivercard=接口1
// dip=2.3.4.5
// [ADDRID]
// ADDRID=1
// [POS]
// id1=1
// posname1=南京a酒店
// id2=2
// posname2=南京b酒店
/////////////////////////////////////////////
// int Read_tz_pos()
// {
// 	char Key[20],Key1[20];
// 
// 	char posname[40],id[40];
// 	int count = 1;
// 	UINT uip = 0;
// 	for(;;++count)
// 	{
// 		sprintf(Key,"posname%d",count);
// 		::GetPrivateProfileString("POS",Key,"NO",posname,sizeof(posname)-1,"x:\\xxx\\para\\pos.ini");
// 		sprintf(Key1,"id%d",count);
// 		::GetPrivateProfileString("POS",Key1,"NO",id,sizeof(id)-1,"x:\\xxx\\para\\pos.ini");
// 
// 		if(lstrcmp("NO",posname) != 0 && lstrcmp("NO",id) != 0 )
// 		{
// 			map_ddepos.SetAt(id,posname);
// 		}
// 		else
// 			break;
// 	}
// 	int cscount = map_ddepos.GetCount();
//  	WriteLog("dce", "pos总个数为:%d", cscount);
// 	
// 	GetPrivateProfileString(
// 	"tz",
// 	"tztype",
// 	"**",
// 	posname,
// 	100,
// 	CONFIGFILE);
// 
// 	if(lstrcmp("tz",posname) == 0 || lstrcmp("tz",id) == 0 )
// 		v_Istz = TRUE;
// 	else
// 		v_Istz = FALSE;
// 	return cscount;
// //	return 1;
// }

void ReadHttpurl()
{
/*	char Key[20];*/
	char Value[40];
	int count = 1;
	UINT uip = 0;
/*	int power;*/

	IMWGetPrivateProfileString("http", "httpurl",
		"true", Value, sizeof(Value)-1, CONFIGFILE);
	if(lstrcmp("false", Value) == 0)
	{
		v_httpurl		= FALSE;		// 回溯httpurl是否写开关
	}
 	WriteLog("dce", "不输出回溯httpurl数据");
}

int	ReadDseServerHost()
{
	ReadHttpurl();
	ASSERT(g_DseServer.m_count == 0);
	char Key[20];
	char ServerIP[40];
	char Value[40];
	int count = 1;
	UINT uip = 0;
	int power;
	
	int idxPower = 0;
	for(;;++count)
	{
		sprintf(Key, "ip%d", count);
		IMWGetPrivateProfileString("IndexServer", Key,
			"NO", ServerIP, sizeof(ServerIP)-1, CONFIGFILE);
		if(lstrcmp("NO", ServerIP) != 0)
		{
			if (sIPtouIP(ServerIP, uip))
			{
				sprintf(Key, "power%d", count);
				IMWGetPrivateProfileString("IndexServer", Key,
					"NO", Value,sizeof(Value)-1, CONFIGFILE);
				power = atoi(Value);
				if (power <= 0)
					continue;
				g_DseServer.AddDse(ServerIP, power);
			}
		}
		else
			break;
	}
 	WriteLog("dce", "dse的总个数为:%d   TotalPower:%d",
		g_DseServer.m_count, g_DseServer.m_totalpower);
	return g_DseServer.m_count;
}
BOOL Inipubvar()
{
	v_datapath.Format("%s:\\netspy",g_dcepath);
	v_StatPath.Format("%s:\\netspy\\statdata",g_dcepath);
// 	CString		v_RootPath("d:\\netspy");
// 	CString		v_WorkPath("e:\\netspy");
// 	CString		v_StatPath("e:\\netspy\\statdata");
// 	CString		v_DdeBakPath("e:\\ddebak");
// 	CString		g_strRootPara	= v_WorkPath+"\\para\\dce";		// 参数根目录
// 	CString		g_DelDceParaFlg = v_WorkPath+"\\flg\\deldcepara.flg";
// 	CString		CONFIGFILE		= v_RootPath+"\\ini\\local.ini";
// 	CString		POSCONFIGINI		= v_WorkPath+"\\para\\Intercept.ini";
	return TRUE;
}
using namespace std;
#define bzero(thing,sz) memset(thing,0,sz)

bool GetAdapterInfo(int adapter_num, CString &mac_addr)
{
    // 重启网络适配器以使我们能开始对它查询
  NCB Ncb;
  memset(&Ncb, 0, sizeof(Ncb));
  Ncb.ncb_command = NCBRESET;
  Ncb.ncb_lana_num = adapter_num;
  if (Netbios(&Ncb) != NRC_GOODRET) {
    mac_addr = "bad (NCBRESET): ";
    mac_addr += CString(Ncb.ncb_retcode);
    return false;
  }

  // 准备得到适配器状态快
  bzero(&Ncb,sizeof(Ncb));
  Ncb.ncb_command = NCBASTAT;
  Ncb.ncb_lana_num = adapter_num;
  strcpy((char *) Ncb.ncb_callname, "*");
  struct ASTAT
  {
    ADAPTER_STATUS adapt;
    NAME_BUFFER NameBuff[30];
  } Adapter;
  bzero(&Adapter,sizeof(Adapter));
  Ncb.ncb_buffer = (unsigned char *)&Adapter;
  Ncb.ncb_length = sizeof(Adapter);

  // 取适配器信息，如果成功按标准colon-delimited格式返回它
  if (Netbios(&Ncb) == 0)
  {
    char acMAC[18];
    sprintf(acMAC, "%02X:%02X:%02X:%02X:%02X:%02X",
            int (Adapter.adapt.adapter_address[0]),
            int (Adapter.adapt.adapter_address[1]),
            int (Adapter.adapt.adapter_address[2]),
            int (Adapter.adapt.adapter_address[3]),
            int (Adapter.adapt.adapter_address[4]),
            int (Adapter.adapt.adapter_address[5]));
    mac_addr = acMAC;
	mac_addr.MakeLower();
	v_localmac.SetAt(mac_addr,mac_addr);
    return true;
  }
  else
  {
    mac_addr = "bad (NCBASTAT): ";
    mac_addr += CString(Ncb.ncb_retcode);
    return false;
  }
}

int getallmac()
{
  // 得到适配器列表
  LANA_ENUM AdapterList;
  NCB Ncb;
  memset(&Ncb, 0, sizeof(NCB));
  Ncb.ncb_command = NCBENUM;
  Ncb.ncb_buffer = (unsigned char *)&AdapterList;
  Ncb.ncb_length = sizeof(AdapterList);
  Netbios(&Ncb);

  // 得到所有的本地以太网地址
  CString mac_addr;
  for (int i = 0; i < AdapterList.length; i++)
  {
    if (GetAdapterInfo(AdapterList.lana[i], mac_addr))
    {
      cout << "Adapter " << int (AdapterList.lana[i]) <<
              "'s MAC is " << mac_addr << endl;
    }
    else
    {
      cerr << "Failed to get MAC address! Do you" << endl;
      cerr << "have the NetBIOS protocol installed?" << endl;
      break;
    }
  }

  return 0;
}