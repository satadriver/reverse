// imsg.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "imsg.h"
#include "Eph.h"
#include "bf.h"
#include "Hf.h"
#include "vp.h"
#include "oicq.h"
#include "Mir.h"
#include "Soqq.h"
#include "OurGame.h"
#include "Ymsg.h"
#include "msn.h"
#include "IcqAol.h"
#include "Fetion.h"
#include "TaoBao1.h"
#include "Uc.h"
#include "QQGame.h"
#include "PerfectGame.h"
#include "wow.h"
#include "wuyiguagua.h"
#include "QQTranFile.h"
#include "daying.h"
#include "SqlTDS.h"
#include "mysql.h"
#include "Dzh.h"
#include "MaiQuan.h"
#include "Xintiandi.h"
#include "GTalk.h"
#include "mobileqq.h"
#include "VoipImsg.h"
#include "doom.h"
#include "WeMeet.h"
#include "QQHeartbeat.h"
#include "..\\LimitPara.h"

// typedef BOOL* CHECKIMSGPROXY (IMSGPROXY&, CString)
//extern "C" PROCOTOL_EXTEND BOOL CheckImsgProxy(IMSGPROXY* imsgproxy, CString Content_type);

CString		g_ImsgTmpPath;
DceMission*	g_pImsgMissObj	= NULL;
char*		g_pMsgBuffer	= NULL;
int			g_Msglen		= 0;

DWORD		g_OicqSion	= 0;
DWORD		g_MsnSion	= 0;
DWORD		g_YmsgSion	= 0;
DWORD		g_ImsgSion	= 0;
DWORD		g_GameSion	= 0;
DWORD		g_qqTry[6]  = {0,0,0,0,0,0};

extern OicqSocketMap g_OicqBuddyMap;
LPVOID g_pDecodeMsn = NULL;
BOOL InitDeoceFunc()
{
	HMODULE hMoudle = LoadLibrary("d:\\netspy\\bin\\MsgAlgorModule.dll");
	if(NULL == hMoudle)
	{
		WriteLog(IMSGPRO, "MsgAlgorModule 不存在");
		return FALSE;
	}
	
	g_pDecodeMsn = (LPVOID) GetProcAddress(hMoudle , "MyDecryptCal");
	if(0 == g_pDecodeMsn)
	{
		WriteLog(IMSGPRO, "MsgAlgorModule MyDecryptCal 寻址失败");
		return FALSE;
	}
	
	return TRUE;
}

// LPCTSTR		g_pImsgSubDir[] =\
// {
// 	_T("imsg_oicq"),
// 	_T("imsg_msn"),
// 	_T("imsg_ymsg"),
// 	_T("imsg_lianzhong"),
// 	_T("imsg_mir"),
// 	_T("imsg_bf"),
// 	_T("imsg_soqq"),
// 	_T("imsg_icqaol"),
// 	_T("imsg_eht"),
// 	_T("imsg_vp"),
// 	_T("imsg_hf"),
// 	NULL
// };

// void* ::operator new (unsigned int sz)
// {
// 	ASSERT(g_ImsgHeap!=NULL);
// 	return HeapAlloc(g_ImsgHeap, HEAP_NO_SERIALIZE, (sz+15)&~15);
// }
// 
// void  ::operator delete (void* p)
// {
// 	HeapFree(g_ImsgHeap, HEAP_NO_SERIALIZE, p);
// }

// This is an example of an exported function.

// Imsg的Http代理处理
// 注意该函数被HTTP线程调用 不能使用new，delete 和 本插件内的所有全局变量(只读的变量除外)
// extern BOOL ProcessYmsgPacket(ImsgSession *s, time_t packtm, char* databuf, int packlen,
// 							  int direction, char* msgbuf, int& msgbuflen,CStringArray* temstr);
// extern int	ProcessMsnPacket(ImsgSession* s, LPMSNMSG pMsg, char* buf, int len, int direction,CStringArray* tempstr);

BOOL WINAPI CheckImsgProxy(IMSGPROXY* pImsgProxy)
{
	if(memcmp(pImsgProxy->ImsgType, "msn", 3)==0)
	{ // MSN 
// typedef struct IMSGPARA
// {
// 	time_t			m_packtm;			// 数据包的时间
// 	int				m_direction;		// 数据包方向 1: 表示sip-->dip,
// 										// 2: 表示dip-->sip
// 	char*			m_packetbuf;		// 包数据缓冲区
// 	int				m_packetbufsize;	// 包数据缓冲区大小
// 	int				m_packetlen;		// 包数据长度
// 	char*			m_msgbuf;			// 还原信息缓冲区
// 	int				m_msgbufsize;		// 还原信息缓冲区大小
// 	int				m_msglen;			// 还原信息的长度
// 
// 	CStringArray	m_hisstrarr;		// 活动历史的内容
// 	BOOL			m_bIdcSvr;			// 是否写IDCServer
// }IMSGPARA;
		MsnSession TmpSession;
		TmpSession.m_direction = 1;
		TmpSession.m_sip = pImsgProxy->sip;
		TmpSession.m_dip = pImsgProxy->dip;
		memcpy(TmpSession.m_csip, pImsgProxy->csip, 4);
		memcpy(TmpSession.m_cdip, pImsgProxy->cdip, 4);
		TmpSession.m_userid = pImsgProxy->userid;
		TmpSession.m_Colloquist = pImsgProxy->linkman;
		TmpSession.m_dataSrc = pImsgProxy->m_dataSrc;
		TmpSession.m_bRecord = pImsgProxy->bReaord;
		
		IMSGPARA MsnMsgPara;
		MsnMsgPara.m_direction = 1;
		MsnMsgPara.m_packtm = pImsgProxy->packettm;
		MsnMsgPara.m_packetbuf = pImsgProxy->pPacketData;
		MsnMsgPara.m_packetbufsize = MsnMsgPara.m_packetlen = pImsgProxy->datalen;
		MsnMsgPara.m_msgbuf = pImsgProxy->pmsgbuf;
		MsnMsgPara.m_msgbufsize = FILEBUFFERSIZE;
		MsnMsgPara.m_msglen = 0;

		ProcessMsnData(&TmpSession, &MsnMsgPara);
// 		if (MsnMsgPara.m_hisstrarr.GetSize()>0)
// 		{
// 			WriteArrToData(IMSG_PROTOCOL, HISTORY_DATA, "", &MsnMsgPara.m_hisstrarr);
// 		}
// 		// 写虚拟身份
// 		if (MsnMsgPara.m_webfigrarr.GetSize()>0)
// 		{
// 			WriteArrToData(IMSG_PROTOCOL, WEBFIG_DATA, "", &MsnMsgPara.m_webfigrarr);
// 		}
		if (MsnMsgPara.m_webidarr.GetSize()>0)
		{
			WriteArrToData(IMSG_PROTOCOL, WEBID_DATA, "", &MsnMsgPara.m_webidarr);
		}
		
// 		pImsgProxy->msgtext = MsnMsg.Msg;
		if (pImsgProxy->userid.GetLength() < TmpSession.m_userid.GetLength())
		{	
			pImsgProxy->userid = TmpSession.m_userid;
		}
		if (pImsgProxy->linkman.GetLength() < TmpSession.m_Colloquist.GetLength() && TmpSession.m_Colloquist!="服务器")
		{
			pImsgProxy->linkman = TmpSession.m_Colloquist;
		}
		if (MsnMsgPara.m_msglen > 0)
		{
			pImsgProxy->msglen = MsnMsgPara.m_msglen;
		}
		return TRUE;
	}
	else if	(memcmp(pImsgProxy->ImsgType, "ymsg", 4)==0)
	{
		// Yahoo
		YmsgSession TmpSession;
		TmpSession.m_bRecord = pImsgProxy->bReaord;
		TmpSession.m_direction = 1;//pImsgProxy->Direction;
		TmpSession.m_sip = pImsgProxy->sip;
		TmpSession.m_dip = pImsgProxy->dip;
		TmpSession.m_userid = pImsgProxy->userid;
 		TmpSession.m_dataSrc = pImsgProxy->m_dataSrc;
		memcpy(TmpSession.m_csip, pImsgProxy->csip, 4);
		memcpy(TmpSession.m_cdip, pImsgProxy->cdip, 4);
//		TmpSession.m_Colloquist = pImsgProxy->linkman;

		IMSGPARA YmsgMsgPara;
		YmsgMsgPara.m_direction = 1;
		YmsgMsgPara.m_packtm = pImsgProxy->packettm;
		YmsgMsgPara.m_packetbuf = pImsgProxy->pPacketData;
		YmsgMsgPara.m_packetbufsize = YmsgMsgPara.m_packetlen = pImsgProxy->datalen;
		YmsgMsgPara.m_msgbuf = pImsgProxy->pmsgbuf;
		YmsgMsgPara.m_msgbufsize = FILEBUFFERSIZE;
		YmsgMsgPara.m_msglen = 0;

		TmpSession.ProcessYmsgPacket(&YmsgMsgPara);
// 		if (YmsgMsgPara.m_hisstrarr.GetSize()>0)
// 		{
// 			WriteArrToData(IMSG_PROTOCOL, HISTORY_DATA, "", &YmsgMsgPara.m_hisstrarr);
// 		}
// 		// 写虚拟身份
// 		if (YmsgMsgPara.m_webfigrarr.GetSize()>0)
// 		{
// 			WriteArrToData(IMSG_PROTOCOL, WEBFIG_DATA, "", &YmsgMsgPara.m_webfigrarr);
// 		}
		if (YmsgMsgPara.m_webidarr.GetSize()>0)
		{
			WriteArrToData(IMSG_PROTOCOL, WEBID_DATA, "", &YmsgMsgPara.m_webidarr);
		}

		if (pImsgProxy->userid.GetLength() < TmpSession.m_userid.GetLength())
		{	
			pImsgProxy->userid = TmpSession.m_userid;
		}
// 		if (pImsgProxy->linkman.GetLength() < TmpSession.m_Colloquist.GetLength() && TmpSession.m_Colloquist!="服务器")
// 		{
// 			pImsgProxy->linkman = TmpSession.m_Colloquist;
// 		}
		if (YmsgMsgPara.m_msglen > 0)
		{
			pImsgProxy->msglen = YmsgMsgPara.m_msglen;
		}
		return TRUE;
	}
	return FALSE;
}

// void InitImsgProxyFunction()
// {
// 	SetImsgProxyFunction(CheckImsgProxy);
// }

ImsgProtocol::ImsgProtocol()
{
	m_ddefiletype = "imsg";
	m_HashSize = 201119;
}

ImsgProtocol::~ImsgProtocol()
{
	
}

BOOL ImsgProtocol::InitObject(DceMission* pMiss)
{
	WriteLog(IMSGPRO, "**************** DCE Module-IMSG(ver:%s %s) Start Run!! ****************",
		__DATE__, __TIME__);
	g_pImsgMissObj = pMiss;
	g_pMsgBuffer = new char[MSGBUFFERLEN];
// 	if (pMiss->m_MissHeap == NULL)
// 	{
// 		pMiss->m_MissHeap = g_ImsgHeap;
// 	}

	if (pMiss->m_logfilename[0] == 0)
	{
		pMiss->m_logfilename = IMSGPRO;
	}

	m_pDseFileInfo = new IpDataStruct[3];
	m_pDseFileInfo[0].m_filenum = 0;
	m_pDseFileInfo[1].m_filenum = 0;
	m_pDseFileInfo[2].m_filenum = 0;
	m_pDseFileInfo[0].m_realflen = 0;
	m_pDseFileInfo[1].m_realflen = 0;
	m_pDseFileInfo[2].m_realflen = 0;
	m_pDseFileInfo[0].m_pindexfile = NULL;
	m_pDseFileInfo[1].m_pindexfile = NULL;
	m_pDseFileInfo[2].m_pindexfile = NULL;
	m_pDseFileInfo[0].m_prealfile = NULL;
	m_pDseFileInfo[1].m_prealfile = NULL;
	m_pDseFileInfo[2].m_prealfile = NULL;
	m_DseInfoCount = 3;

	InitDeoceFunc();
	InitMobileQQKey();

	return TRUE;
}

BOOL ImsgProtocol::ProcessPacket(DATEHEADER& packh, const char* pPacket, LPCTSTR spyaddr)
{
	if (packh.m_datalen<=4)
	{
		return FALSE;
	}

#ifdef  _DEBUG		// 增加数据的过滤(仅用于协议调试)
// 	if (packh.sport != 5190)
// 		return TRUE;
#endif

int error=0;
int itype = 0;
	SocketSession* pSion = NULL;
	try
	{

		int tmphb = QQHeartbeat::IsQQHeartbeat(packh, pPacket);
		if (tmphb) // QQ心跳
		{
			itype= 53;
			QQHeartbeat *hb = new QQHeartbeat(packh, m_PacketFrom);
			hb->ProcessData(pPacket,tmphb);
			delete hb;
		}

		// 首先处理Oicq联系人
		if(ProcessOicqBuddyPacket(packh, pPacket, spyaddr, m_PacketFrom))
		{
			return TRUE;
		}
		
		// 判断是否是OICQ
		WORD qqcmd = 0;
		WORD qqver = 0;
		DWORD dwQQID = isOicqPacket(packh, pPacket, qqcmd, qqver);
		if (dwQQID != 0)
		{
			if(g_dbgSwitch.m_outqqcmd)
			{
				if(dwQQID > 1000)
				{
					printf("QQID:%-10d, QQCmd:0x%02x, QQVer:0x%04x\r\n", 
						dwQQID, qqcmd, qqver);
				}
			}
			// 可以创建会话的包 只有命令号为: 0x62, 0x22, 0x27, 0xA1, 0x01
			if (//(qqcmd==0x27) || (qqcmd==0x22) || 仅依上下线命令建会话, 避免会话关闭后,建会话导致的重复上线
				((qqver<TM2008) && (qqcmd==0x91)) || (qqcmd==0xdd) || (qqcmd==0xba) ||
				((qqver<TM2008) && (qqcmd==0xa1)) || ((qqver<TM2008) && (qqcmd==0x01)) ||
				(qqver > TM2008 && (qqcmd == TM_CMD_LOGOUT ||qqcmd==TM_CMD_LOGIN || qqcmd==TM_CMD_2013LOGIN 
				|| qqcmd==QQ2013_CMD_LOGIN || qqcmd==QQ2014_CMD_LOGIN || 
				qqcmd == QQ_0825_PACK ||qqcmd == QQ_0836_PACK || qqcmd == QQ_0828_PACK)) )
			{				
itype = 1;
				pSion = (SocketSession*)new OicqSession(packh, m_PacketFrom, dwQQID);
			}
			else
			{
				return TRUE;
			}
		}
		//判断是否是手机QQ的包
		//////////////////////////////////////////////////////////////////////////
		else if(isMobileQQPacket(packh, pPacket) == 1)
		{
			pSion = (SocketSession*)new mobileqq(packh, m_PacketFrom ,2);
// 			pSion->InsertPacket(packh, pPacket);
// 			return TRUE;
		}
		else if(IsGTalkPacket(packh,pPacket))
		{
itype = 2;
			pSion = (SocketSession*)new GTalkSession(packh, m_PacketFrom);
		}
		// 判断是否是YMSG
		else if (memcmp(pPacket, "YMSG", 4)==0)
		{
error=1;
itype = 3;
			pSion = (SocketSession*)new YmsgSession(packh, m_PacketFrom);
		}
		//判断是否是E话通
		else if (isEPHPacket(packh,pPacket))
		{
error=2;
itype = 4;
			pSion = (SocketSession*)new EphSession(packh, m_PacketFrom);
		}
		else if(isEPHImsgPacket(packh,pPacket))
		{
error=3;
itype = 5;
			pSion = (SocketSession*)new ImsgEphSession(packh, m_PacketFrom);
		}
		//判断是否是边锋
		else if(IsBfPacket(packh, pPacket))
		{
error=4;
itype = 6;
			pSion = (SocketSession*)new BfSession(packh, m_PacketFrom);
		}
		else if(IsMQPacket(packh, pPacket))
		{
itype = 7;
			pSion = (SocketSession*)new CMaiQuan(packh, m_PacketFrom);
		}
		//判断是否是浩方
		else if(IsHfPacket(packh, pPacket))
		{
error=5;
itype = 8;
			pSion = (SocketSession*)new HfSession(packh, m_PacketFrom);
		}
		else if(IsSoQQPacket(packh, pPacket))
		{
error=6;
itype = 9;
			pSion = (SocketSession*)new SoqqSession(packh, m_PacketFrom);
		}
		//判断是否vp
		else if(IsVpPacket(packh, pPacket))
		{
error=7;
itype = 10;
			pSion = (SocketSession*)new VPIMSession(packh, m_PacketFrom);
		}
		else if(isFetion(packh, pPacket))
		{
error=8;
itype = 11;
			pSion = (SocketSession*)new FetionSession(packh, m_PacketFrom);
		}
		else if(isMobileFethion(packh, pPacket))
		{
			pSion = (SocketSession*)new MebileFetionSession(packh, m_PacketFrom);
		}
		else if(isQQGame(packh,pPacket))
		{
error=9;
itype = 12;
			pSion = (SocketSession*)new QQGameSession(packh, m_PacketFrom);
		}
		else if(isPerfectGame(packh,pPacket))
		{
error=10;
itype = 13;
			pSion = (SocketSession*)new PerfectGameSession(packh, m_PacketFrom);
		}
		else if(IsUcPacket(packh, pPacket))
		{
error=11;
itype = 14;
			pSion = (SocketSession*)new UCIMSession(packh, m_PacketFrom);
		}
		else if(IsTaoBaoPacket(packh, pPacket))
		{
error=12;
itype = 15;
			pSion = (SocketSession*)new TaoBaoSession(packh, m_PacketFrom);
		}
		else if((memcmp(pPacket,"\x2a\x01",2)==0 &&packh.m_datalen>50 && packh.m_datalen<150)||
			(memcmp(pPacket,"\x2a\x02",2)==0 &&
			(memcmp(pPacket+6,"\x0\x4\x0\x6",4)==0||memcmp(pPacket+6,"\x0\x1\x0\xf",4)==0||
			memcmp(pPacket+6,"\x0\x4\x0\x7",4)==0)))
		{
error=13;
itype = 16;
			pSion = (SocketSession*)new IcqAolSession(packh, m_PacketFrom);
		}
		else if (IsWoWPacket(packh, pPacket)>0)		// 判断是否是魔兽世界的包
		{
error=14;
itype = 17;
			pSion = (SocketSession*)new WowSession(packh, m_PacketFrom);
		}
		else if(IsQQTranFilePacket(packh,pPacket))
		{
itype = 18;
			pSion = (SocketSession*)new QQTranFileSession(packh, m_PacketFrom);
		}
	
		else if(IsDayingPacket(packh,pPacket))
		{
itype = 19;
			pSion = (SocketSession*)new DayingSession(packh, m_PacketFrom);
		}
		// 判断是否是MSN
		else if ((((packh.m_sport == 1863) || (packh.m_dport == 1863) || (pPacket[3] == ' '))
			&& IsMsnPacket(packh, pPacket) )
			|| IsMsnTranfilePacket(packh, pPacket))
		{
itype = 20;
			//创建session
			pSion = (SocketSession*)new MsnSession(packh, m_PacketFrom);
		}
// 		else if ((packh.m_sport == 1863) || (packh.m_dport == 1863) || (pPacket[3] == ' ') )
// // 			|| (memcmp(pPacket+0x10, "INVITE MSNMSGR", 14) == 0)
// // 			|| (memcmp(pPacket+0x20, "INVITE MSNMSGR", 14) == 0))
// 		{
// error=15;
// 
// 			pSion = (SocketSession*)new MsnSession(packh, m_PacketFrom);
// 		}
		// 判断是否是OURGAME
		else if (IsOurgamePacket(packh, pPacket))
		{
error=16;
itype = 21;
			pSion = (SocketSession*)new OurGameSession(packh, m_PacketFrom);
		}
		// 判断是否是MIR
		else if ((packh.m_sport==7000 || packh.m_dport==7000) &&
			(packh.m_datalen>17 && pPacket[0]=='#' && pPacket[packh.m_datalen-1]=='!'))
		{
error=17;
itype = 22;
			pSion = (SocketSession*)new MirSession(packh, m_PacketFrom);
		}
		else if(IsWyggPacket(packh, pPacket))
		{
error=18;
itype = 23;
			pSion = (SocketSession*)new WuyiguaguaSession(packh, m_PacketFrom);
		}
		else if(IsSqlTDSPacket(packh, pPacket))
		{
error=19;
itype = 24;
			pSion = (SocketSession*)new CSqlTDSSession(packh, m_PacketFrom);
		}
		else if(IsMySqlPacket(packh, pPacket))
		{
itype = 25;
			pSion = (SocketSession*)new MysqlSession(packh, m_PacketFrom);
		}
		else if(IsDzh60(packh, pPacket))
		{
itype = 26;
			pSion = (SocketSession*)new CDzh60Session(packh, m_PacketFrom);
		}
		else if(IsThsPacket(packh, pPacket))
		{
itype = 27;
			pSion = (SocketSession*)new CThsSession(packh, m_PacketFrom);
		}
		else if(IsxtdPacket(packh, pPacket))
		{
itype = 28;
			pSion = (SocketSession*)new CXintiandi(packh, m_PacketFrom);
		}
		/*
		else if (TrojanBoerSession::IsBoerPacket(packh, pPacket))
		{
			itype= 29;
			pSion = (SocketSession *)new TrojanBoerSession(packh, m_PacketFrom);
		}
		else if (TrojanZxshellSession::IsZxshellPacket(packh, pPacket))
		{
			itype= 30;
			pSion = (SocketSession*)new TrojanZxshellSession(packh, m_PacketFrom);
		}
		
		else if (TrojanXengineSession::IsXenginePacket(packh, pPacket))
		{
			itype= 31;
			pSion = (SocketSession*)new TrojanXengineSession(packh, m_PacketFrom);
		}
		
		else if (TrojanSunShadowSession::IsSunShadowPacket(packh, pPacket))
		{
			itype= 32;
			pSion = (SocketSession*)new TrojanSunShadowSession(packh, m_PacketFrom);
		}
		
		else if (TrojanHavratSession::IsHavratPacket(packh, pPacket))
		{
			itype= 33;
			pSion = (SocketSession*)new TrojanHavratSession(packh, m_PacketFrom);
		}
		
		else if (TrojanRainbowBridgeSession::IsRainbowBridgePacket(packh, pPacket))
		{
			itype= 34;
			pSion = (SocketSession*)new TrojanRainbowBridgeSession(packh, m_PacketFrom);
		}
		
		else if (TrojanEvilLotusSession::IsEvilLotusPacket(packh, pPacket))
		{
			itype= 35;
			pSion = (SocketSession*)new TrojanEvilLotusSession(packh, m_PacketFrom);
		}
		
		else if (TrojanRenWoSession::IsRenWoPacket(packh, pPacket))
		{
			itype= 36;
			pSion = (SocketSession*)new TrojanRenWoSession(packh, m_PacketFrom);
		}
		
		else if (TrojanNSSession::IsNSPacket(packh, pPacket))
		{
			itype= 37;
			pSion = (SocketSession*)new TrojanNSSession(packh, m_PacketFrom);
		}
		else if (TrojanXDoorSession::IsXDoorPacket(packh, pPacket))
		{
			itype= 38;
			pSion = (SocketSession*)new TrojanXDoorSession(packh, m_PacketFrom);
		}
		else if (TrojanWangLing1Session::IsWangLing1Packet(packh, pPacket))
		{
			itype= 39;
			pSion = (SocketSession*)new TrojanWangLing1Session(packh, m_PacketFrom);
		}
		else if (TrojanMoJianChuanShuoSession::IsMoJianChuanShuoPacket(packh, pPacket))
		{
			itype= 40;
			pSion = (SocketSession*)new TrojanMoJianChuanShuoSession(packh, m_PacketFrom);
		}
		else if (TrojanBlackHoleSession::IsBlackHole(packh, pPacket))
		{
			itype= 41;
			pSion = (SocketSession*)new TrojanBlackHoleSession(packh, m_PacketFrom);
		}
		else if (TrojanGhostSession::IsGhostPacket(packh, pPacket))
		{
			itype= 42;
			pSion = (SocketSession*)new TrojanGhostSession(packh, m_PacketFrom);
		}
		else if (TrojanLanHuDieSession::IsLanHuDiePacket(packh, pPacket))
		{
			itype= 43;
			pSion = (SocketSession*)new TrojanLanHuDieSession(packh, m_PacketFrom);
		}*/
		else if (ETphoneNumSession::IsETphoneNumPacket(packh, pPacket))
		{
			itype= 44;
			pSion = (SocketSession*)new ETphoneNumSession(packh, m_PacketFrom);
		}
		
		else if (VoipFEIYINImsgSession::IsVoipFEIYINImsg(packh, pPacket))
		{
			itype= 45;
			pSion = (SocketSession*)new VoipFEIYINImsgSession(packh, m_PacketFrom);
		}		
		else if (VoipTUXINImsgSession::IsVoipTUXINImsg(packh, pPacket))
		{
			itype= 46;
			pSion = (SocketSession*)new VoipTUXINImsgSession(packh, m_PacketFrom);
		}
		else if (VoipSKYImsgSession::IsVoipSKYImsg(packh, pPacket))
		{
			itype= 47;
			pSion = (SocketSession*)new VoipSKYImsgSession(packh, m_PacketFrom);
		}
		else if (VoipKCImsgSession::IsVoipKCImsg(packh, pPacket))
		{
			itype= 48;
			pSion = (SocketSession*)new VoipKCImsgSession(packh, m_PacketFrom);
		}
		else if (VoipYLTImsgSession::IsVoipYLTImsg(packh, pPacket))
		{
			itype= 49;
			pSion = (SocketSession*)new VoipYLTImsgSession(packh, m_PacketFrom);
		}
		else if (VoipWEIWEIImsgSession::IsVoipWEIWEIImsg(packh, pPacket))
		{
			itype= 50;
			pSion = (SocketSession*)new VoipWEIWEIImsgSession(packh, m_PacketFrom);
		}
		else if (VoipUxinImsgSession::IsVoipUxinImsg(packh, pPacket))
		{
			itype= 51;
			pSion = (SocketSession*)new VoipUxinImsgSession(packh, m_PacketFrom);
		}		
		else if (VoipAdlImsgSession::IsVoipAdlImsg(packh, pPacket))
		{
			itype= 52;
			pSion = (SocketSession*)new VoipAdlImsgSession(packh, m_PacketFrom);
		}

		else if (WeMeet::IsWeMeet(packh, pPacket))
		{
			itype= 53;
			pSion = (SocketSession*)new WeMeet(packh, m_PacketFrom);
		}
		else
		{
			return FALSE;
		}

error=100;

		if(pSion != 0)
		{
			g_pImsgMissObj->m_SocketMap.SetAt(pSion->m_sip + "_" + pSion->m_dip,
				(SocketSession*&)pSion);
error = 102;
			pSion->m_spyaddr = spyaddr;
			pSion->InsertPacket(packh, pPacket);
		}
	}
	catch (...)
	{
		WriteLog(IMSGPRO, "ProcessPacket() Error!!=%d, type:%d",error, itype);
	}
	return TRUE;
}

void ImsgProtocol::RenameTmpDir()
{
	g_OicqBuddyMap.ClearOicqSocketMap(time(0));
	DceProcotol::RenameTmpDir();
}

BOOL ImsgProtocol::WriteObjectState(time_t packtm, CString& strtxt)
{
// 	DWORD heapsize=0;
// 	DWORD emptysize=0;
// 	int   blockcount=0;
// 	if (g_ImsgHeap!=NULL)
// 	{
// 		if(!GetHeapInfo(g_ImsgHeap, heapsize, emptysize, blockcount))
// 			heapsize = emptysize = blockcount = 0;
// 		//GetHeapInfo(ipub_Heap, heapsize, emptysize, blockcount);
// 	}
	char ptm[40];
	GetTimeString(packtm, "%Y-%m-%d %H:%M:%S", ptm, 40);
	WriteLog(IMSGPRO, "当前状态:packettime:%s  loop:%u  packnum:%u   mapsize:%u\r\n"
		"OicqSion:%-4u  BuddySion:%-6u  MsnSion:%-4u  YmsgSion:%-4u  GameSion:%-4u  ImsgSion:%-4u\r\n",
		ptm, g_pImsgMissObj->m_loopnum, g_pImsgMissObj->m_packetnum, g_pImsgMissObj->m_SocketMap.GetCount(),
		g_OicqSion, g_OicqBuddyMap.GetCount(), g_MsnSion, g_YmsgSion, g_GameSion, g_ImsgSion);
	if(g_dbgSwitch.m_dbgtrystat)
	{
		if(g_qqTry[0] == 0)
			WriteLog(IMSGPRO, "QQTry  Havn't Crack");
		else
		{
			WriteLog(IMSGPRO, "QQTry all %d, pass %d, ba %d, crack %d, fail1(have0x30) %d, fail2(lowver) %d,"
				"f1/all %d%%, f2/all %d%%",
				g_qqTry[0], g_qqTry[1], g_qqTry[2], g_qqTry[3], g_qqTry[4], g_qqTry[5], 
				(g_qqTry[4]*100)/g_qqTry[0], (g_qqTry[5]*100)/g_qqTry[0]);
			
			if(g_qqTry[0] > 1000)
			{
				for (int ii = 0; ii < 6 ; ii++)
					g_qqTry[ii] = 0;
			}
		}
	}
	return TRUE;
}


