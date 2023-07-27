
#include "StdAfx.h"

#include "imsg.h"
#include "doom.h"

#define DOOM_GHOST_PACKET 1

int isDoomGhostPacket(DATEHEADER& packh, const char* pPacket)
{
	if (packh.m_type == TCPPacket)
	{
		if (  (memcmp(pPacket, "gh0st", 5) == 0) )
		{
			return DOOM_GHOST_PACKET;
		}
	}
	return 0;
}

TrojanSession::TrojanSession(DATEHEADER& packh, int aT) : SocketSession(packh,aT)
{
	m_begfiletm = packh.m_tm;
	m_closetm = packh.m_tm + 120;
}

TrojanSession::~TrojanSession()
{

}

void TrojanSession::WriteTrojanIndexFile()
{
	m_OpenCaseFlg = v_opencasetrojan;
	m_OpenWarnFlg = v_openwarntrojan;
	m_OpenIndxFlg = v_openflagtrojan;

	DWORD state = 0;
	SearchCaseOrWarn(this, m_strWarn, m_strWarn.GetLength(), state);

	CString strSuffix = m_strPro + ".indexfile.txt";
	m_indexfname = CreateFilename(m_sip, m_dip, strSuffix);
	m_strTxt.Format("webname=%s\r\nstate=%s\r\nmsg=%s\r\n",m_strWarn,m_strAction,m_strContent);
	WriteIndexFile(m_strPro, m_strPro, m_strTxt, g_pImsgMissObj);
	m_CaseArr.RemoveAll();
	m_WarnArr.RemoveAll();
	m_closetm += 10;
}

int TrojanSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{
	return 0;
}


int TrojanBoerSession::IsBoerPacket(DATEHEADER & packh, const char * pPacket)
{
	if (packh.m_type == TCPPacket)
	{
		if( memcmp(pPacket,"EMSG", strlen("EMSG")) ==0 )
		{ 
			if (memcmp(pPacket + 21,"Msg",strlen("Msg")) == 0)
			{
				if (memcmp(pPacket + 33,"Msg",strlen("Msg")) == 0)
				{
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}


TrojanBoerSession::TrojanBoerSession(DATEHEADER & packh, int aT) : TrojanSession(packh, aT)
{
	m_strPro = "doom_trojan_boer";
	m_strWarn = "波尔";
}


TrojanBoerSession::~TrojanBoerSession()
{

}


int TrojanBoerSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{
	if(packh.m_type == TCPPacket)
	{
		if (memcmp(pPacket + 21,"Msg",strlen("Msg")) == 0)
		{
			if (memcmp(pPacket + 33,"Msg",strlen("Msg")) == 0)
			{
				if( memcmp(pPacket,"EMSG0005", strlen("EMSG0005")) ==0 )
				{ 
					
					int ContentLen = *(int*)(pPacket + 8);
					
					char * BoerFlag = "@#$@#$";
					if (memcmp(pPacket + 45,BoerFlag,6) != 0 )
					{
						return FALSE;
					}
					
					char * ContentHead =(char*)(pPacket + 51);
					char * ContentEnd = strstr(ContentHead,BoerFlag);
					if (ContentEnd == 0)
					{
						return FALSE;
					}
					
					if (ContentEnd == ContentHead)
					{
						return FALSE;
					}
					
					if (ContentEnd - ContentHead >= 256)
					{
						return FALSE;
					}
					/*
					m_Content = ContentHead;
					m_Content.Left(ContentEnd - ContentHead);
					m_Action = "cmd";
					*/
					m_strAction = "CMD命令";
					m_strContent = "";
					WriteTrojanIndexFile();
					return TRUE;
				}

				if( memcmp(pPacket,"EMSG000", strlen("EMSG000")) ==0 )
				{
					
					int ContentLen = *(int*)(pPacket + 8);
					
					char * BoerFlag = "@#$@#$";
					if (memcmp(pPacket + 45,BoerFlag,6) != 0 )
					{
						return FALSE;
					}
					
					char * ContentHead = (char*)(pPacket + 51);
					char * ContentEnd = strstr(ContentHead,BoerFlag);
					if (ContentEnd == 0)
					{
						return FALSE;
					}
					
					if (ContentEnd == ContentHead)
					{
						return FALSE;
					}
					
					if (ContentEnd - ContentHead >= 256)
					{
						return FALSE;
					}
					/*
					m_Content = ContentHead;
					m_Content.Left(ContentEnd - ContentHead);
					m_Action = "msg";
					*/
					m_strAction = "发送消息";
					m_strContent = "";
					WriteTrojanIndexFile();
					return TRUE;
				}
				m_strAction = "BOER";
				m_strContent = "";
				WriteTrojanIndexFile();
				return TRUE;

			}
		}
	}
	return FALSE;
}



int TrojanZxshellSession::IsZxshellPacket(DATEHEADER & packh, const char * pPacket)
{
	if (packh.m_type == TCPPacket)
	{
		if( (packh.m_sport == 8000) && (packh.m_datalen == 100) )
		{ 
			if (*pPacket == 0x06)
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}


TrojanZxshellSession::TrojanZxshellSession(DATEHEADER & packh,int aT) : TrojanSession(packh, aT)
{
	m_strPro = "doom_trojan_zxshell";
	m_strWarn = "zxshell";
}


TrojanZxshellSession::~TrojanZxshellSession()
{
	
}


int TrojanZxshellSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{
	if (packh.m_type == TCPPacket)
	{
		if( (packh.m_sport == 8000) && (packh.m_datalen == 100) )
		{ 
			if (*pPacket == 0x06)
			{
				m_strAction = "下载";
				m_strContent = pPacket + 5;
				WriteTrojanIndexFile();	 
				return TRUE;
				
			}
		}
	}
	
	return TRUE;
}

int TrojanXengineSession::IsXenginePacket(DATEHEADER & packh, const char * pPacket)
{
	if (packh.m_type == TCPPacket)
	{
		if( memcmp(pPacket,"\x5f\x0d\x01\x00\xec\x03\x00\x00", 8) ==0 )
		{ 
			if (memcmp(pPacket + 16,"dream2fly",strlen("dream2fly")) == 0)
			{
				if (*(unsigned short*)(pPacket + 12) == 0x0434)
				{
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}


TrojanXengineSession::TrojanXengineSession(DATEHEADER & packh,int aT) : TrojanSession(packh, aT)
{
	m_strPro = "doom_trojan_xengine";
	m_strWarn = "xengine";
}


TrojanXengineSession::~TrojanXengineSession()
{

}


int TrojanXengineSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{
	if(packh.m_type == TCPPacket)
	{
		if( memcmp(pPacket,"\x5f\x0d\x01\x00\xec\x03\x00\x00", 8) ==0 )
		{ 
			if (memcmp(pPacket + 16,"dream2fly",strlen("dream2fly")) == 0)
			{
			
				m_strContent.Empty();
				m_strAction = "登陆";
				
				WriteTrojanIndexFile();	 
				return TRUE;

			}
		}
		if( memcmp(pPacket,"\x5f\x0d\x01\x00\xee\x03\x00\x00", 8) ==0 )
		{ 			
			m_strContent.Empty();
			m_strAction = "查看进程";
			
			WriteTrojanIndexFile();	 
			return TRUE;
		}
		if( memcmp(pPacket,"\x5f\x0d\x01\x00\xfb\x03\x00\x00", 8) ==0 )
		{ 			
			m_strContent= pPacket + 16;
			m_strAction = "下载文件";
			
			WriteTrojanIndexFile();	 
			return TRUE;
		}
		if( memcmp(pPacket,"\x5f\x0d\x01\x00\xf9\x03\x00\x00", 8) ==0 )
		{ 			
			m_strContent= pPacket + 16;
			m_strAction = "上传文件";
			
			WriteTrojanIndexFile();	 
			return TRUE;
		}

		if( memcmp(pPacket,"\x5f\x0d\x01\x00\xf9\x03\x00\x00", 8) ==0 )
		{ 			
			m_strContent= pPacket + 16;
			m_strAction = "上传文件";
			
			WriteTrojanIndexFile();	 
			return TRUE;
		}
		/*
		if( (memcmp(pPacket,"\x5f\x0d\x01\x00\", 4) ==0 ) && 
			(memcmp(pPacket + 5,"\x03\x00\x00\", 3) ==0 ) )
		{ 			
			m_strContent= "";
			m_strAction = "Xengine";
			
			WriteTrojanIndexFile();	 
			return TRUE;
		}*/
	}
	
	return FALSE;
}



int TrojanSunShadowSession::IsSunShadowPacket(DATEHEADER & packh, const char * pPacket)
{
	if (packh.m_type == TCPPacket)
	{
		if( memcmp(pPacket,"\xff\x01",2) == 0)
		{ 
			return TRUE;
		}
	}
	return FALSE;
}


TrojanSunShadowSession::TrojanSunShadowSession(DATEHEADER & packh,int aT) : TrojanSession(packh, aT)
{
	m_strPro = "doom_trojan_sunshadow";
	m_strWarn = "sunshadow";
}


TrojanSunShadowSession::~TrojanSunShadowSession()
{

}


int TrojanSunShadowSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{
	if (packh.m_type == TCPPacket)
	{
		if( memcmp(pPacket,"\xff\x01\x0d\x01\x13\x00\x00\x00\xe0\xfc\x00\x00",12) == 0)
		{ 
			char  CmdName[256] = {0};
			memcpy(CmdName,pPacket + 12,packh.m_datalen - 12);
			m_strContent = CmdName;
			m_strAction = "CMD命令";
			
			WriteTrojanIndexFile();	 
			return TRUE;
		}

		if (memcmp(pPacket,"\xff\x01\x06\x02\x00\x00\x00\x00\xfa\xfb\x00\x00",12) == 0)
		{
			m_strContent.Empty();
			m_strAction = "截屏";
			
			WriteTrojanIndexFile();	 
			return TRUE;
		}
		if (memcmp(pPacket,"\xff\x01\x05\x02\x2a\x00\x00\x00\xfa\xfb\xd1\xfb",12) == 0)
		{
			m_strContent = pPacket + 12;
			char NarrowChar[256] = {0};
			if (packh.m_datalen >= 256)
			{
				return FALSE;
			}
			for (int Cnt = 0;Cnt < (packh.m_datalen - 12)/2; Cnt++)
			{
				WideCharToMultiByte(CP_ACP,NULL,(const unsigned short*)(pPacket + 12 + Cnt*2),
					1,NarrowChar + Cnt,sizeof(WCHAR),NULL,NULL);
			}
			
			m_strContent = NarrowChar;

			m_strAction = "下载文件";
			
			WriteTrojanIndexFile();	 
			return TRUE;
		}
		if (memcmp(pPacket,"\xff\x01\x0c\x02\x32\x00\x00\x00\xc2\xfb\x00\x00",12) == 0)
		{
			m_strContent = pPacket + 12;
			char NarrowChar[256] = {0};
			if (packh.m_datalen >= 256)
			{
				return FALSE;
			}
			for (int Cnt = 0;Cnt < (packh.m_datalen - 12)/2; Cnt++)
			{
				WideCharToMultiByte(CP_ACP,NULL,(const unsigned short*)(pPacket + 12 + Cnt*2),
					1,NarrowChar + Cnt,sizeof(WCHAR),NULL,NULL);
			}
			
			m_strContent = NarrowChar;
			
			m_strAction = "执行文件";
			
			WriteTrojanIndexFile();	 
			return TRUE;
		}

	}
	
	return FALSE;
}


int TrojanHavratSession::IsHavratPacket(DATEHEADER& packh, const char *pPacket)
{
	if (packh.m_type == TCPPacket)
	{
		if( memcmp(pPacket,"CMDC: ",strlen("CMDC: ")) == 0)
		{ 
			return TRUE;
		}
		else if (memcmp(pPacket,"downlo",strlen("downlo")) == 0)
		{
			return TRUE;
		}
		else if (memcmp(pPacket,"upload",strlen("upload")) == 0)
		{
			return TRUE;
		}
		
	}
	
	return FALSE;
}

TrojanHavratSession::TrojanHavratSession(DATEHEADER & packh,int aT) : TrojanSession(packh, aT)
{
	
	m_strPro = "doom_trojan_havrat";
	m_strWarn = "havrat";

}


TrojanHavratSession::~TrojanHavratSession()
{
	
}

int TrojanHavratSession::InsertPacket(DATEHEADER & packh, const char * pPacket)
{
	if (packh.m_type == TCPPacket)
	{
		if( memcmp(pPacket,"CMDC: ",strlen("CMDC: ")) == 0)
		{ 
			m_strContent.Empty();
			m_strAction = "CMD命令";
			
			WriteTrojanIndexFile();	 
			return TRUE;
		}
		if (memcmp(pPacket,"downlo",strlen("downlo")) == 0)
		{
			if (packh.m_datalen >= 256)
			{
				return FALSE;
			}
			char  CmdName[256] = {0};
			memmove(CmdName,pPacket + 6,packh.m_datalen - 6);
			m_strContent = CmdName;
			m_strAction = "下载文件";
			
			WriteTrojanIndexFile();	 
			return TRUE;
		}
		if (memcmp(pPacket,"upload",strlen("upload")) == 0)
		{
			if (packh.m_datalen >= 256)
			{
				return FALSE;
			}
			char  CmdName[256] = {0};
			memcpy(CmdName,pPacket + 6,packh.m_datalen - 6);
			m_strContent = CmdName;
			m_strAction = "上传文件";
			
			WriteTrojanIndexFile();	 
			return TRUE;
		}
	}
	return FALSE;
}



int TrojanRainbowBridgeSession::IsRainbowBridgePacket(DATEHEADER & packh, const char * pPacket)
{
	if (packh.m_type == TCPPacket)
	{
		if( memcmp(pPacket + 4,"\xbc\x73\x8a\x4b\xc6\x78\x90\x02",8) == 0)
		{ 
			return TRUE;
		}
	}

	return FALSE;
}


TrojanRainbowBridgeSession::TrojanRainbowBridgeSession(DATEHEADER & packh,int aT) : TrojanSession(packh, aT)
{
	m_strPro = "doom_trojan_chq";
	m_strWarn = "彩虹桥";
}


TrojanRainbowBridgeSession::~TrojanRainbowBridgeSession(){ }


int TrojanRainbowBridgeSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{
	if (packh.m_type == TCPPacket)
	{
		if( memcmp(pPacket + 4,"\xbc\x73\x8a\x4b\xc6\x78\x90\x02",8) == 0)
		{ 
			m_strContent.Empty();
			m_strAction = "";
			
			WriteTrojanIndexFile();	 
			return TRUE;
		}
	}	
	return FALSE;
}


int TrojanEvilLotusSession::IsEvilLotusPacket(DATEHEADER & packh, const char * pPacket)
{
	if (packh.m_type == TCPPacket)
	{
		if (packh.m_datalen < 12)
		{
			return FALSE;
		}
		if( memcmp(pPacket,"\x00\x00\x00\x00\x6a\x80\x00\x00\x64\xc8\x00\x00",12) == 0)
		{ 
			return TRUE;
		}
	}
	return FALSE;
}


TrojanEvilLotusSession::TrojanEvilLotusSession(DATEHEADER & packh,int aT) : TrojanSession(packh, aT)
{
	m_strPro = "doom_trojan_evilotus";
	m_strWarn = "evilotus";
}


TrojanEvilLotusSession::~TrojanEvilLotusSession()
{

}


int TrojanEvilLotusSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{
	if (packh.m_type == TCPPacket)
	{
		if (packh.m_datalen < 12)
		{
			return FALSE;
		}
		if( (*(unsigned int*)pPacket == 0) &&(*(unsigned int*)(pPacket + 8) == 0x0000c864) )
		{ 
			m_strContent.Empty();
			m_strAction = "";
			
			WriteTrojanIndexFile();	 
			return TRUE;
		}
	}	
	return FALSE;
}

int TrojanRenWoSession::IsRenWoPacket(DATEHEADER & packh, const char * pPacket)
{
	if (packh.m_type == TCPPacket)
	{
		if (memcmp(pPacket,"HTTP/1.0 200 Connected OK\r\n",strlen("HTTP/1.0 200 Connected OK\r\n") == 0) )
		{
			if (packh.m_datalen != strlen("HTTP/1.0 200 Connected OK\r\n"))
			{
				return FALSE;
			}
			
			return TRUE;
		}
		if (memcmp(pPacket,"MTIz\r\n",6) == 0) 
		{
			if (packh.m_datalen != 6)
			{
				return FALSE;
			}
			return TRUE;
		}
	}
	
	return FALSE;
}


TrojanRenWoSession::TrojanRenWoSession(DATEHEADER & packh,int aT) : TrojanSession(packh, aT)
{
	m_strPro = "doom_trojan_rw";
	m_strWarn = "任我";
}


TrojanRenWoSession::~TrojanRenWoSession()
{

}


int TrojanRenWoSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{
	if (packh.m_type == TCPPacket)
	{
		if (memcmp(pPacket,"HTTP/1.0 200 Connected OK\r\n",strlen("HTTP/1.0 200 Connected OK\r\n") == 0) )
		{
			if (packh.m_datalen != strlen("HTTP/1.0 200 Connected OK\r\n"))
			{
				return FALSE;
			}
			m_strContent.Empty();
			m_strAction = "登陆";
			
			WriteTrojanIndexFile();
			return TRUE;
		}
		if (memcmp(pPacket,"MTIz",strlen("MTIz")) == 0 )
		{
		
			m_strContent.Empty();
			m_strAction = "";
			
			WriteTrojanIndexFile();
			return TRUE;
		}
	}	
	return FALSE;
}


int TrojanNSSession::IsNSPacket(DATEHEADER & packh, const char * pPacket)
{
	if (packh.m_type == TCPPacket)
	{

		if (memcmp(pPacket,"HTTP OK\r\n",strlen("HTTP OK\r\n")) == 0 )
		{
			if (packh.m_datalen != strlen("HTTP OK\r\n"))
			{
				return FALSE;
			}
			return TRUE;
		}
		if (memcmp(pPacket,"TVlCUg0K",strlen("TVlCUg0K")) == 0)	//file transfer,process,cmd,kbd
		{
			return TRUE;
		}
	}
	
	return FALSE;
}


TrojanNSSession::TrojanNSSession(DATEHEADER & packh,int aT) : TrojanSession(packh, aT)
{
	m_strPro = "doom_trojan_ns";
	m_strWarn = "ns";
}


TrojanNSSession::~TrojanNSSession()
{

}


int TrojanNSSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{
	if (packh.m_type == TCPPacket)
	{
		if (memcmp(pPacket,"HTTP OK\r\n",strlen("HTTP OK\r\n")) == 0 )
		{
			if (packh.m_datalen != strlen("HTTP OK\r\n"))
			{
				return FALSE;
			}
			m_strContent.Empty();
			m_strAction = "登陆";
			
			WriteTrojanIndexFile();	 
			return TRUE;
		}
		if (memcmp(pPacket,"TVlCUg0K",strlen("TVlCUg0K")) == 0)	//file transfer,process,cmd,kbd
		{
			;
		}
	}	
	return TRUE;
}

int TrojanGhostSession::IsGhostPacket(DATEHEADER & packh, const char * pPacket)
{
	if (packh.m_type == TCPPacket)
	{
		if( memcmp(pPacket,"Gh0st",5) == 0)
		{ 
			return TRUE;
		}
	}
	return FALSE;
}


TrojanGhostSession::TrojanGhostSession(DATEHEADER & packh,int aT) : TrojanSession(packh, aT)
{
	m_strPro = "doom_trojan_ghost";
	m_strWarn = "ghost0";
}


TrojanGhostSession::~TrojanGhostSession()
{
	
}


int TrojanGhostSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{
	if (packh.m_type == TCPPacket)
	{
		if( memcmp(pPacket,"Gh0st",5) == 0)
		{ 
			m_strContent.Empty();
			m_strAction = "";
			
			WriteTrojanIndexFile();	 
			return TRUE;
		}
	}
	return 0;
}

int TrojanXDoorSession::IsXDoorPacket(DATEHEADER & packh, const char * pPacket)
{
	if (packh.m_type == TCPPacket)
	{
		if( memcmp(pPacket,"\nstartx",7) == 0)
		{ 
			return TRUE;
		}
	}
	return FALSE;
}


TrojanXDoorSession::TrojanXDoorSession(DATEHEADER & packh,int aT) : TrojanSession(packh, aT)
{
	m_strPro = "doom_trojan_xdoor";
	m_strWarn = "xdoor";
}


TrojanXDoorSession::~TrojanXDoorSession()
{
	
}


int TrojanXDoorSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{
	if (packh.m_type == TCPPacket)
	{
		if (  (memcmp(pPacket, "\nstartxfile\n", 12) == 0) )
		{
			m_strContent.Empty();
			m_strAction = "查看文件";
			
			WriteTrojanIndexFile();	 
			return TRUE;
		}
		if (  (memcmp(pPacket, "\nstartxprocess\n", 15) == 0) )
		{
			m_strContent.Empty();
			m_strAction = "查看进程";
			
			WriteTrojanIndexFile();	 
			return TRUE;
		}
		if (  (memcmp(pPacket, "\nstartxservices\n", 16) == 0) )
		{
			m_strContent.Empty();
			m_strAction = "查看服务";
			
			WriteTrojanIndexFile();	 
			return TRUE;
		}
		if (  (memcmp(pPacket, "\nstartxsound\n", 13) == 0) )
		{
			m_strContent.Empty();
			m_strAction = "监听声音";
			
			WriteTrojanIndexFile();	 
			return TRUE;
		}
		if (  (memcmp(pPacket, "\nstartxvideo\n", 13) == 0) )
		{
			m_strContent.Empty();
			m_strAction = "查看视频";
			
			WriteTrojanIndexFile();	 
			return TRUE;
		}	
		if (  (memcmp(pPacket, "\nstartxreg\n", 11) == 0) )
		{
			m_strContent.Empty();
			m_strAction = "查看注册表";
			
			WriteTrojanIndexFile();	 
			return TRUE;
		}	
		if (  (memcmp(pPacket, "\nstartxscreen\n", 14) == 0) )
		{
			m_strContent.Empty();
			m_strAction = "屏幕监控";
			
			WriteTrojanIndexFile();	 
			return TRUE;
		}	
		if (  (memcmp(pPacket, "\nstartxcmd\n", 11) == 0) )
		{
			m_strContent.Empty();
			m_strAction = "CMD命令";
			
			WriteTrojanIndexFile();	 
			return TRUE;
		}	
	}	
	return FALSE;
}


int TrojanHuiGeZiSession::IsHuiGeZiPacket(DATEHEADER & packh, const char * pPacket)
{
	if (packh.m_type == TCPPacket)
	{
		if( (memcmp(pPacket + 4, "\x38\x38\x34\x35\x64\x36\x61\x33\x64\x38\x61\x66\x31\x66\x39\x61", 16) == 0))
		{ 
			return TRUE;
		}
	}
	return FALSE;
}


TrojanHuiGeZiSession::TrojanHuiGeZiSession(DATEHEADER & packh,int aT) : TrojanSession(packh, aT)
{
	m_strPro = "doom_trojan_hgz";
	m_strWarn = "灰鸽子";
}


TrojanHuiGeZiSession::~TrojanHuiGeZiSession()
{
	
}

int TrojanHuiGeZiSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{
	if (packh.m_type == TCPPacket)
	{
		if (  (memcmp(pPacket + 4, "\x38\x38\x34\x35\x64\x36\x61\x33\x64\x38\x61\x66\x31\x66\x39\x61", 16) == 0) )
		{
			if (memcmp( pPacket ,"\x27\x0\x0\x0",4) == 0)
			{
				m_strContent.Empty();
				m_strAction = "CMD命令";
				
				WriteTrojanIndexFile();	 
				return TRUE;
			}
			if (memcmp( pPacket ,"\x0b\x0\x0\x0",4) == 0)
			{
				m_strContent.Empty();
				m_strAction = "屏幕监控";
				
				WriteTrojanIndexFile();	 
				return TRUE;
			}
			m_strContent.Empty();
			m_strAction = "";
			
			WriteTrojanIndexFile();	 
			return TRUE;
		}
	}
	return 0;
}

int TrojanWangLing1Session::IsWangLing1Packet(DATEHEADER & packh, const char * pPacket)
{
	
	if( memcmp(pPacket,"\x80\x01\x00\x00\xc0\x00\x00\xb5",8) == 0)
	{ 
		return TRUE;
	}
	if (memcmp(pPacket,"\x77\xc3\xef\x8a",4) == 0)
	{
		return TRUE;
	}
	
	return FALSE;
}


TrojanWangLing1Session::TrojanWangLing1Session(DATEHEADER & packh,int aT) : TrojanSession(packh, aT)
{
	m_strPro = "doom_trojan_wl1";
	m_strWarn = "网灵一号";
}


TrojanWangLing1Session::~TrojanWangLing1Session()
{
	
}

int TrojanWangLing1Session::InsertPacket(DATEHEADER& packh, const char *pPacket)
{
	
	if( memcmp(pPacket,"\x80\x01\x00\x00\xc0\x00\x00\xb5",8) == 0)
	{ 
		m_strContent.Empty();
		m_strAction = "";
		
		WriteTrojanIndexFile();	 
		return TRUE;
	}
	if (memcmp(pPacket,"\x77\xc3\xef\x8a",4) == 0)
	{
		
		if (memcmp(pPacket + 4,"\xc0\x00\x00\x04",4) == 0)
		{
			
			m_strContent.Empty();
			m_strAction = "屏幕监控";
			
			WriteTrojanIndexFile();	 
			return TRUE;
		}
		if (memcmp(pPacket + 4,"\xc0\x00\x00\x03",4) == 0)
		{
			
			m_strContent.Empty();
			m_strAction = "文件传输";
			
			WriteTrojanIndexFile();	 
			return TRUE;
		}
	}
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////
int TrojanHeiKeFangXianSession::IsHeiKeFangXianPacket(DATEHEADER & packh, const char * pPacket)
{
	if (packh.m_type == TCPPacket)
	{
		if( memcmp(pPacket + 5,"\x38\x62\x34\x63\x61\x35\x38\x31\x37\x32\x38\x38\x30\x62\x62\x62",16) == 0)
		{ 
			return TRUE;
		}
	}
	return FALSE;
}


TrojanHeiKeFangXianSession::TrojanHeiKeFangXianSession(DATEHEADER & packh,int aT) : TrojanSession(packh, aT)
{
	m_strPro = "doom_trojan_hkfx";
	m_strWarn = "黑客防线";
}


TrojanHeiKeFangXianSession::~TrojanHeiKeFangXianSession()
{
	
}

int TrojanHeiKeFangXianSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{
	if (packh.m_type == TCPPacket)
	{
		if( memcmp(pPacket + 5,"\x38\x62\x34\x63\x61\x35\x38\x31\x37\x32\x38\x38\x30\x62\x62\x62",16) == 0)
		{ 
			if (memcmp(pPacket + 4,"\x27\x00\x00\x00",4) == 0)
			{
				
				m_strContent.Empty();
				m_strAction = "CMD命令";
				
				WriteTrojanIndexFile();	 
				return TRUE;
			}
			if (memcmp(pPacket + 4,"\x0b\x00\x00\x00",4) == 0)
			{
				
				m_strContent.Empty();
				m_strAction = "文件传输";
				
				WriteTrojanIndexFile();	 
				return TRUE;
			}

			m_strContent.Empty();
			m_strAction = "";
			
			WriteTrojanIndexFile();	 
			return TRUE;
		}
	}
	return 0;
}


int TrojanMoJianChuanShuoSession::IsMoJianChuanShuoPacket(DATEHEADER & packh, const char * pPacket)
{
	if ( packh.m_type == TCPPacket)
	{
		if( memcmp(pPacket,"\x78\xda\x63\x60",4) == 0)
		{ 
			return TRUE;
		}
	}
	return FALSE;
}


TrojanMoJianChuanShuoSession::TrojanMoJianChuanShuoSession(DATEHEADER & packh,int aT) : TrojanSession(packh, aT)
{
	m_strPro = "doom_trojan_mjcs";
	m_strWarn = "魔剑传说";
}


TrojanMoJianChuanShuoSession::~TrojanMoJianChuanShuoSession()
{
	
}


int TrojanMoJianChuanShuoSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{
	if ( packh.m_type == TCPPacket)
	{
		if( memcmp(pPacket,"\x78\xda\x63\x60",4) == 0)
		{ 
			
			if ( (memcmp(pPacket + 5,"\x80\xff\x40\x70\x06\x08\x00\x21\xee\x07\x2d",10) == 0) ||
				 (memcmp(pPacket + 5,"\x80\x63\x40\x7c\x06\x08\x00\x0e\x38\x03\xf7",10) == 0) )
			{
				m_strContent.Empty();
				m_strAction = "";
				
				WriteTrojanIndexFile();
				return TRUE;
			}
		}
	}
	
		
	return 0;
}



int TrojanBlackHoleSession::IsBlackHole(DATEHEADER & packh, const char * pPacket)
{
	if (packh.m_type == TCPPacket)
	{
		if( memcmp(pPacket + 2 ,"000000",6) == 0)
		{ 
			return TRUE;
		}
	}
	return FALSE;
} 
TrojanBlackHoleSession::TrojanBlackHoleSession(DATEHEADER & packh,int aT) : TrojanSession(packh, aT)
{
	m_strPro = "doom_trojan_hd";
	m_strWarn = "黑洞";
} 

TrojanBlackHoleSession::~TrojanBlackHoleSession()
{
	
} 

int TrojanBlackHoleSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{
	if (packh.m_type == TCPPacket)
	{
		if( memcmp(pPacket + 2 ,"000000",6) == 0)
		{ 
			m_strContent.Empty();
			m_strAction.Empty();
			WriteTrojanIndexFile();
			return TRUE;
		}
	}
	return 0;
}


int TrojanLanHuDieSession::IsLanHuDiePacket(DATEHEADER & packh, const char * pPacket)
{
	if (packh.m_type == TCPPacket)
	{
		if( memcmp(pPacket + 8 ,"\xf7\x34\xba\x42\xaa\xaa\xaa\xaa",8) == 0)
		{ 
			return TRUE;
		}
	}
	return FALSE;
} 

TrojanLanHuDieSession::TrojanLanHuDieSession(DATEHEADER & packh,int aT) : TrojanSession(packh, aT)
{
	m_strPro = "doom_trojan_lhd";
	m_strWarn = "蓝蝴蝶";
} 

TrojanLanHuDieSession::~TrojanLanHuDieSession()
{
	
} 

int TrojanLanHuDieSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{
	if (packh.m_type == TCPPacket)
	{
		if( memcmp(pPacket + 8 ,"\xf7\x34\xba\x42\xaa\xaa\xaa\xaa",8) == 0)
		{  
			m_strContent.Empty();
			m_strAction.Empty();
			WriteTrojanIndexFile();
			return TRUE;
		}
	}
	return 0;
}