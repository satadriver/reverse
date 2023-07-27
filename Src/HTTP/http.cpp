#include "stdafx.h"
#include "http.h"
#include "TeSpread.h"
#include "httpSession.h"
#include "publicfun.h"
#include "FreeGateDec.h"
#include "Https.h"
#include "NogSession.h"
#include "mobilemomo.h"
#include "pcanywhere.h"
#include "ssh.h"
#include "AQWS360.H"
#include "../imsg/QQTranFile.h"
#include "YiXinHttpSession.h"
#include "Chating/Zello.h"
#include "Chating/Telegram.h"
#include "qqpim/QQSyncAsist.h"
#include "..\\src\\IMSG\\mobileqq.h"

typedef CMap<CString, LPCSTR, mobileqq*, mobileqq *&> HttpMobileqqMap;
HttpMobileqqMap	g_httpmobileqqmap;

// extern "C" PROCOTOL_EXTEND DceProcotol* CreateProcotolObjectv2(void);
// HANDLE g_httpHeap = HeapCreate(HEAP_NO_SERIALIZE, 4096*1024, 0);
//CString g_HttpTmpPath;

CString	g_HttpRarPath;
HttpSocketMap g_HttpMap(8192);
IPHostMap g_IpHostMap(8192);

DceMission*	g_httpMissObj = NULL;
char*		g_HttpFileBuffer=NULL;
char*		g_CHUNKBUFFER=NULL;
LPCTSTR		g_DDEPOS="";
int			g_httpFrom;

// DWORD	g_GetPacketCount=0;			// �Ե�����ʽ�����Get���ܸ���
// DWORD	g_HttpurlCount=0;			// �Ե�����ʽ�����Get����дURL�ļ��İ�����
// DWORD	g_GetSessCount=0;			// �Ե�����ʽ�����Get���н����Ự�İ���
// DWORD	g_IdcHttpCount=0;			// ֱ�ӽ����Ự��Get��
// DWORD	g_PostPacketCount=0;		// �Ե�����ʽ�����Post���ܸ���
// DWORD	g_HttpupCount=0;			// �Ե�����ʽ�����Get����дHttpup�ļ��İ�����
// DWORD	g_PostSessCount=0;			// �Ե�����ʽ�����Post���н����Ự�İ���
// DWORD	g_HttpSessCount=0;			// �����Ự�ĺ����İ���
// DWORD	g_HttpokCount=0;			// Http ok �İ���
// DWORD	g_HttpsPacketCount=0;		// Https�İ���
// DWORD	g_RubbishPacketCount=0;		// ����û��ֱ�Ӵ���İ���
// DWORD	g_freegatePacketCount=0;	// �޽�8�İ���
// DWORD	g_UltrasurfPacketCount=0;	// �޽�8�İ���
//mapTeSpread g_mapFreeGate63;
DWORD		g_httpFileNum = 0;
BOOL		g_DumpPacketFlg = FALSE;	// 
extern void	BuildDeHttpUpInfo();

Search g_ExplorerType1("windows");
Search g_ExplorerType2("linux");

map<CString,time_t> g_uniqmid;			// Ψһֵȥ�ع�ϣ, ��:Ψһֵ, ֵ:������ʱ��
CMap<CString,LPCSTR,YiXinHttpSession *,YiXinHttpSession *&>cmapYiXin;

HttpProtocol::HttpProtocol()
{
	m_ddefiletype = "http";
	m_HashSize = 201119;
	g_IpHostMap.InitHashTable(201119);

	m_GetPacketCount=0;			// �Ե�����ʽ�����Get���ܸ���
	m_HttpurlCount=0;			// �Ե�����ʽ�����Get����дURL�ļ��İ�����
	m_GetSessCount=0;			// �Ե�����ʽ�����Get���н����Ự�İ���
	m_IdcHttpCount=0;			// ֱ�ӽ����Ự��Get��
	m_PostPacketCount=0;		// �Ե�����ʽ�����Post���ܸ���
	m_HttpupCount=0;			// �Ե�����ʽ�����Get����дHttpup�ļ��İ�����
	m_PostSessCount=0;			// �Ե�����ʽ�����Post���н����Ự�İ���
	m_HttpSessCount=0;			// �����Ự�ĺ����İ���
	m_HttpokCount=0;			// Http ok �İ���
	m_HttpsPacketCount=0;		// Https�İ���
	m_RubbishPacketCount=0;		// ����û��ֱ�Ӵ���İ���
	m_freegatePacketCount=0;	// �޽�8�İ���
	m_UltrasurfPacketCount=0;	// �޽�8�İ���
	m_httpFileNum=0;
	char*	m_CHUNKBUFFER=NULL;
	char*	m_HttpFileBuffer=NULL;
}
HttpProtocol::HttpProtocol(int httpfilenum)
{
	m_ddefiletype = "http";
	m_HashSize = 201119;
	g_IpHostMap.InitHashTable(201119);

	m_GetPacketCount=0;			// �Ե�����ʽ�����Get���ܸ���
	m_HttpurlCount=0;			// �Ե�����ʽ�����Get����дURL�ļ��İ�����
	m_GetSessCount=0;			// �Ե�����ʽ�����Get���н����Ự�İ���
	m_IdcHttpCount=0;			// ֱ�ӽ����Ự��Get��
	m_PostPacketCount=0;		// �Ե�����ʽ�����Post���ܸ���
	m_HttpupCount=0;			// �Ե�����ʽ�����Get����дHttpup�ļ��İ�����
	m_PostSessCount=0;			// �Ե�����ʽ�����Post���н����Ự�İ���
	m_HttpSessCount=0;			// �����Ự�ĺ����İ���
	m_HttpokCount=0;			// Http ok �İ���
	m_HttpsPacketCount=0;		// Https�İ���
	m_RubbishPacketCount=0;		// ����û��ֱ�Ӵ���İ���
	m_freegatePacketCount=0;	// �޽�8�İ���
	m_UltrasurfPacketCount=0;	// �޽�8�İ���
	m_httpFileNum=0;
	char*	m_CHUNKBUFFER=NULL;
	char*	m_HttpFileBuffer=NULL;
	m_httpFileNum=httpfilenum;
}

HttpProtocol::~HttpProtocol()
{
}

void HttpProtocol::RenameTmpDir()
{
	// 1���Ӽ��һ���Ƿ���д��Dump�İ�
	if(_access(v_WorkPath+"\\flg\\DumpPacket.flg", 0) == 0)
	{
		if(!g_DumpPacketFlg)
		{
			g_DumpPacketFlg = TRUE;
			WriteLog(HTTPPRO, "�����־����");
			v_HttpDumpHttpPacket.InitDumpBuffer();
		}
		v_HttpDumpHttpPacket.SaveBuffer();
// #ifdef		DUMPHTTPPACKET
// 		v_HttpDumpHttpPacket.SaveBuffer();
// #endif
//		remove("x:\\xxx\\flg\\DumpPacket.flg");
	}
	else
	{
		if(g_DumpPacketFlg)
		{
			g_DumpPacketFlg = FALSE;
			WriteLog(HTTPPRO, "�����־�ر�");
			v_HttpDumpHttpPacket.SaveBuffer();
			v_HttpDumpHttpPacket.~DumpPacket();
		}
	}
	DceProcotol::RenameTmpDir();
	// ����IDC��ϣ��
	g_HttpMap.ClearHttpSocketMap(time(0));
}

BOOL HttpProtocol::WriteObjectState(time_t now, CString& strtxt)
{
//	CString strKey;
// 	int iDelay, iItem;
// 	TeSpread pTeSpread, pTeSpreadAuth;
// 	POSITION pos = g_mapFreeGate63.GetStartPosition();
// 
// 	iItem = 0;		// ��չȷ����IP����
// 	while(pos != NULL)
// 	{
// 		iDelay = 5 * 60;
// 		g_mapFreeGate63.GetNextAssoc(pos, strKey, pTeSpread);
// 		if(pTeSpread.cntTimes > 0)
// 			iDelay = 30 * 60;	// ��ȷ����IP, 30�������κζ�λ����
// 		else
// 		{
// 			ASSERT(strKey.GetLength() == 31);
// 			if(g_mapFreeGate63.Lookup(strKey.Left(15), pTeSpreadAuth))
// 			{
// 				// ��ȷ��IP, ���IPʹ�õĶ�λSocket30������
// 				if(pTeSpreadAuth.cntTimes >= TESPREAD_FREEGATE63)
// 					iDelay = 30 * 60;	
// 			}
// 		}
// 		if( abs(now - pTeSpread.tmLast) > iDelay)
// 			g_mapFreeGate63.RemoveKey(strKey);
// 		else
// 		{
// 			if(pTeSpread.cntTimes >= TESPREAD_FREEGATE63)
// 				iItem ++;
// 		}
// 	}

// 	DWORD	g_GetPacketCount;		// �Ե�����ʽ�����Get���ܸ���
// 	DWORD	g_HttpurlCount;			// �Ե�����ʽ�����Get����дURL�ļ��İ�����
// 	DWORD	g_GetSessCount;			// �Ե�����ʽ�����Get���н����Ự�İ���
// 	DWORD	g_IdcHttpCount;			// ֱ�ӽ����Ự��Get��
// 	DWORD	g_PostPacketCount;		// �Ե�����ʽ�����Post���ܸ���
// 	DWORD	g_HttpupCount;			// �Ե�����ʽ�����Post����дHttpup�ļ��İ�����
// 	DWORD	g_PostSessCount;		// �Ե�����ʽ�����Post���н����Ự�İ���
// 	DWORD	g_HttpSessCount;		// �����Ự�ĺ����İ���
// 	DWORD	g_HttpokCount;			// Http ok �İ���
//	DWORD	g_HttpsPacketCount;		// Https�İ���
//	DWORD	g_UltrasurfPacketCount;	// �޽�8�İ���
// 	DWORD	g_RubbishPacketCount;	// ����û��ֱ�Ӵ���İ���
// 	strtxt.Format("FreeGate63��չ��%d��, ��ǰ��Socket%d��\r\nIDC Socket%d",
// 		iItem, g_mapFreeGate63.GetCount()-iItem, g_HttpMap.GetCount());
	strtxt.Format("IDC Socket: %d", g_HttpMap.GetCount());
	strtxt.Format("%s\r\nGetPacket: ��Get��%u��, Httpurl%u��, IDC%u��, Sion%u��\r\n"
			"PostPacket: ��Post��%u��, ����Post��%u��, PostSion%u��\r\n"
			"OtherPacket: Sion��%u��, IDCOK��%u��, Https��%u��, ֮�Ű�%u��, �޽�8��%u��, ������%u��, "
			"IPHostMap %d\r\n",
			CString(strtxt), m_GetPacketCount, m_HttpurlCount, m_IdcHttpCount, m_GetSessCount, 
			m_PostPacketCount, m_HttpupCount, m_PostSessCount,
			m_HttpSessCount, m_HttpokCount, m_HttpsPacketCount, m_freegatePacketCount, m_UltrasurfPacketCount, m_RubbishPacketCount,
			g_IpHostMap.GetCount());

	m_GetPacketCount=0;	
	m_HttpurlCount=0;			
	m_GetSessCount=0;			
	m_IdcHttpCount=0;			
	m_PostPacketCount=0;		
	m_HttpupCount=0;			
	m_PostSessCount=0;		
	m_HttpSessCount=0;		
	m_HttpokCount=0;
	m_HttpsPacketCount=0;
	m_RubbishPacketCount=0;
	m_UltrasurfPacketCount=0;
	return FALSE;
}

BOOL HttpProtocol::InitObject(DceMission* pMiss)
{
	::CoInitialize(NULL);
	WriteLog(HTTPPRO, "**************** DCE Module-HTTP(ver:%s %s) Start Run!! ****************", __DATE__, __TIME__);
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
//	g_HttpFileBuffer = new char [FILEBUFFERSIZE];
//	g_CHUNKBUFFER= new char [FILEBUFFERSIZE];
	g_httpMissObj = pMiss;
	m_httpMissObj = pMiss;
	m_CHUNKBUFFER= new char [FILEBUFFERSIZE];
	m_HttpFileBuffer = new char [FILEBUFFERSIZE];
// 	if (pMiss->m_MissHeap == NULL)
// 	{
// 		pMiss->m_MissHeap = g_httpHeap;
// 	}
	if (pMiss->m_logfilename[0] == 0)
	{
		pMiss->m_logfilename = HTTPPRO;
	}
	// �������ò�����
	BuildDeHttpUpInfo();
//	g_mapFreeGate63.InitHashTable();
	g_HttpMap.InitHashTable(101111, FALSE);
	return TRUE;
}

BOOL HttpProtocol::ProcessPacket(DATEHEADER& packh, const char* pPacket, LPCTSTR spyaddr) // http ����
{
//��ӥ׹��666
#ifdef _DEBUG
// 	string src_data(pPacket);
// 	AfxMessageBox(pPacket);
// 	if (src_data.find("/richlifeApp/extIntf/IMessage?") != -1)
// 	{
// 		AfxMessageBox(pPacket);
// 	}
#endif

#ifdef  _DEBUG		// �������ݵĹ���(������Э�����)
// 	if ((packh.m_sport != 1866)/* || (packh.m_sip[3] != 16)*/)
// 		return TRUE;
#endif
	g_httpFrom = m_PacketFrom;
	if (packh.m_type == HTTP200OK)
	{
		m_HttpokCount++;
		g_HttpMap.AddHttpResponse(packh);
		return TRUE;
	}
	
	g_DDEPOS = spyaddr;
	int re=0;
// 	if(stricmp(spyaddr, "@REC&C") == 0)
// 	{
// 		if( pPacket[3+15] == ',' &&
// 			strlen(pPacket) == 34 && 
// 			memcmp(pPacket, "14 ", 3)==0 )
// 		{
// 			char ipsrc[20], ipdst[20];
// 			memset(ipsrc, 0, sizeof(ipsrc));
// 			memset(ipdst, 0, sizeof(ipdst));
// 			memcpy(ipsrc, pPacket+ 3, 15);
// 			memcpy(ipdst, pPacket+19, 15);
// 
// 			CreateFreeGate63(ipsrc, ipdst, FALSE);
// 		}
// 		return TRUE;
// 	}

	if(_access("e://netspy//flg/dumpteibaid.flg", 0) == 0)
	{
		remove("e://netspy//flg/dumpteibaid.flg");
	}
	try
	{
		BOOL bDeal = FALSE;		// �Ѵ���
		char tmppack[16];
		memset(tmppack+12, 0, 4);
		memcpy(tmppack, pPacket, 12);
		strupr(tmppack);
		////���� GET POST ��////������ �Ϳ��//////////////////////////////////
		if(memcmp(tmppack,"GET ",4) == 0)/****GETҪ��� IP_URL URL***********/	
		{
			if(( packh.m_type== IdcHttpPacket || packh.m_type==ProxyTCP ||
				packh.m_type== CdmaHttpPacket || packh.m_type== GprsHttpPacket) &&
				m_httpMissObj->m_SocketMap.GetCount() < 7000)
				// �Ự������7000�� �����ʹ�������ݾͲ�ֱ�ӽ����Ự
			{
				HttpSession *s=0;
				m_IdcHttpCount++;
				if(packh.m_type == CdmaHttpPacket)
				{
					s = new HttpSession(packh, CDMATCP);
					s->m_dceprocotol=this;
				}
				else if(packh.m_type == GprsHttpPacket)
				{
					s = new HttpSession(packh, GPRSTCP);
					s->m_dceprocotol=this;
				}
				else
				{
					s = new HttpSession(packh, g_httpFrom);
					s->m_dceprocotol=this;
				}
				if(s != NULL)
				{
					s->m_PacketType=7;
					if (-2 >= s->InsertPacket(packh, pPacket))
					{
						delete s;
					}
					else
					{
						/*
						char key[64] = {0};
						sprintf(key, "%03d.%03d.%03d.%03d_%05d_%03d.%03d.%03d.%03d_%05d_%d",
							packh.m_sip[0], packh.m_sip[1], packh.m_sip[2], packh.m_sip[3], packh.m_sport,
							packh.m_dip[0], packh.m_dip[1], packh.m_dip[2], packh.m_dip[3], packh.m_dport,packh.m_sequence);
						m_httpMissObj->m_SocketMap.SetAt(key, (SocketSession*&)s);*/
						m_httpMissObj->m_SocketMap.SetAt(s->m_sip+"_"+s->m_dip, (SocketSession*&)s);
					}
				}
			}
			else
			{
#ifdef		DUMPHTTPPACKET
//				v_HttpDumpHttpPacket.DumpOnePacket(packh, pPacket);
#endif
				m_GetPacketCount++;
				ProcessHttpGet(packh, pPacket,this);
			}
			bDeal = TRUE;
			return TRUE;
		}
		else if(strncmp(tmppack,"POST ",5) == 0 || memcmp(tmppack,"PUT ",4) == 0)
		{
#ifdef		DUMPHTTPPACKET
//			v_HttpDumpHttpPacket.DumpOnePacket(packh, pPacket);
#endif
			if (!v_openflaghttpup)
			{
				return FALSE;
			}
			m_PostPacketCount++;

 			ProcessHttpPost(packh, pPacket,this);

			bDeal = TRUE;
			return TRUE;
		}
		//�Ƿ���İİ��λ���ݰ�
		if(FALSE == bDeal)
		{
			if (IsMomoPacket(pPacket, packh))
			{
				if (ProcessMomoPacket(pPacket, packh, this))
				{
					bDeal = TRUE;
					return TRUE;
				}
			
			}
		}		
		if (FALSE == bDeal)
		{
			bDeal = ProcQQPimPacketStream(packh, pPacket,this);
			if (bDeal == TRUE)
			{
				return TRUE;
			}
		}
		if (FALSE == bDeal)
		{
		
			if(Zello::IsZelloPacket(packh,pPacket))
			{
				Zello::ProcessZelloPacket(packh,pPacket,this);
				bDeal = TRUE;
			}
		}
		if (FALSE == bDeal)
		{
			
			if(IsTelegramPacket(packh,pPacket))
			{
				ProcessTelegramPacket(packh,pPacket,this);
				bDeal = TRUE;
			}
			
		}				
	if (FALSE == bDeal)
		{
			if(isMobileQQPacket(packh,pPacket))
			{
				mobileqq *mqq = 0;
				char socketkey[256];
				sprintf(socketkey,"%08x_%08x_%04x_%04x",*(unsigned int*)packh.m_sip,*(unsigned int*)packh.m_dip, packh.m_sport, packh.m_dport);
				int result = g_httpmobileqqmap.Lookup(socketkey,mqq);
				if (result == 0)
				{
					sprintf(socketkey,"%08x_%08x_%04x_%04x",*(unsigned int*)packh.m_dip,*(unsigned int*)packh.m_sip,packh.m_dport, packh.m_sport);
					result = g_httpmobileqqmap.Lookup(socketkey,mqq);
					if(result == 0)
					{
						mqq = new mobileqq(packh,TCPPacket,MOBILEQQ_PROTOCOLHTTP);
						g_httpmobileqqmap.SetAt(socketkey,mqq);
					}
				}
				
				try
				{
					analyse(packh,pPacket,mqq);
				}
				catch (...)
				{
					WriteLog(HTTPPRO,"process mobile qq http packet error\r\n");
				}
				
				bDeal = TRUE;
			}
		}

		//�Ƿ���pcanywhere���ݰ�
		if(FALSE == bDeal)
		{
			if(packh.m_dport == 5631 && memcmp(pPacket+1,"\x49\x00\x4c",3)==0)
			{
				if(Pcanywhere_DealPacket(pPacket, packh,this))
				{
					bDeal = TRUE;
					return TRUE;
				}
			}
		}
		
		//�Ƿ���ssh���ݰ�
		if(FALSE == bDeal)
		{
			if(packh.m_dport == 22)
			{
				CString sshs;
				CString sshmapip;
				sshmapip.Format("%d_%d_%d_%d",packh.m_sip[0],packh.m_sip[1],packh.m_sip[2],packh.m_sip[3]);

				if(v_sshMap.Lookup(sshmapip,sshs))
				{
					//��IP�Ѿ���¼��
					bDeal = TRUE;
					return TRUE;
				}
				else
				{
					if(ssh_DealPacket(pPacket, packh,this))
					{
						SYSTEMTIME st;
						GetLocalTime(&st);
						CString tm;
						tm.Format("%04u-%02u-%02u %02u:%02u:%02u",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);

						v_sshMap.SetAt(sshmapip,tm);

						bDeal = TRUE;
						return TRUE;
					}
					
				}
			}
		}
	
		if(FALSE == bDeal)
		{
			if(isUltrasurf8(pPacket, packh,this))
			{
				bDeal = TRUE;
				return TRUE;
			}
		}

		//modify by hzs  ,��dns ��λ
// 		if(FALSE == bDeal && packh.m_dport == 443)
// 		{
// 			if(isFreegate686p((const unsigned char*)pPacket, packh))
// 			{
// 				bDeal = TRUE;
// 				return TRUE;
// 			}
// 		}


		// ��7.05��Ϊdns  ��ʽ��λ
		if(FALSE == bDeal && packh.m_dport == 53)
		{
			if(isFreegate689b3((const unsigned char*)pPacket, packh,this))
			{
				bDeal = TRUE;
				return TRUE;
			}
		}


		/*
		SSLV3
			content type: handshake (22)
			verstion: SSL 3.1 ��3.0 (0x0301 | 0x0300)
			Length: short 
			handleshake type : server hello (2)
			length:
			version:  SSL 3.1 ��3.0 (0x0301 | 0x0300)

		*/
		//ssl ���� 16 03 01 ��ͷ ֻ����server hello�İ�, ��ȥ��
		if(FALSE == bDeal &&
			packh.m_type == TCPPacket
			&&packh.m_datalen>0x200 &&
			memcmp(pPacket,"\x16\x03\x01\x00\x4a\x02",6)==0  )
		{
			if(isFreegate7((const unsigned char*)pPacket, packh,this))
			{
				bDeal = TRUE;
				return TRUE;
			}
		}


		/*
		SSLV3
			content type: handshake (22)
			verstion: SSL 3.1 ��3.0 (0x0301 | 0x0300)
			Length: short 
			handleshake type : server hello (2)
			length:
			version:  TLS 1.0 (0x0301 | 0x0300)

		*/
		//ssl ���� 16 03 01 ��ͷ ͬʱ����client hello �� server hello�İ�,����ͬʱ����������
		//��ȥ��
		if(FALSE == bDeal &&
			packh.m_type == TCPPacket
			&&packh.m_datalen>0x40 &&
			(memcmp(pPacket,"\x16\x03\x01\x00\x48\x01",6)==0 ||   //client hello
			memcmp(pPacket,"\x16\x03\x01\x00\x51\x02",6)==0	))    //server
		{
			if(isFreegate740((const unsigned char*)pPacket, packh,this))
			{
				bDeal = TRUE;
				return TRUE;
			}
		}
		if(FALSE == bDeal)
		{
			DWORD errfree = 0;
			try{

			unsigned char rKey[16];
			errfree = 1;
			DWORD rCheck = isFreeGate(pPacket, packh, rKey,this);
			errfree = 11;
			if (rCheck)
			{
				errfree = 2;
				if(rCheck == FG_675HELLO || rCheck == FG_676HELLO)
				{
					CString dIP;
					CFgServer *pServer = NULL;
					dIP.Format("%u.%u.%u.%u", packh.m_dip[0], packh.m_dip[1], packh.m_dip[2], packh.m_dip[3]);
					if(FALSE == g_MapFgServer.Lookup(dIP, (CObject*&)pServer))
					{
						pServer = new CFgServer;
						if(pServer)
						{
							pServer->SetKey(pServer->m_cntKey, rKey);
							g_MapFgServer.SetAt(dIP, (CObject*&)pServer);
						}
					}
				}
				errfree = 3;
				if(rCheck == FG_MAYRESPONSES)
				{
					// �õ�һ��ȷ�Ϸ����������е�17 03 00
					CString sIP;
					CFgServer *pServer = NULL;
					sIP.Format("%u.%u.%u.%u", packh.m_sip[0], packh.m_sip[1], packh.m_sip[2], packh.m_sip[3]);
					if(g_MapFgServer.Lookup(sIP, (CObject*&)pServer))
					{
						// ȷ��Ϊ����������
						unsigned char tmpPacket[2000];
						DWORD xorlen = (UCHAR)pPacket[3]*256+(UCHAR)pPacket[4];
						// ASSERT(xorlen < packh.m_datalen);
						if(packh.m_datalen < 2000 && xorlen+5 == packh.m_datalen)
						{	
							memset(tmpPacket, 0, 2000);
							Search	SearchXPower("Content-Length");//TROJDATAID);

							BYTE xorstring[300];
							unsigned char rKey[16];
							for (int iK = 0; iK < pServer->m_cntKey ; iK++)
							{
								memcpy(tmpPacket, pPacket, packh.m_datalen);

								memcpy(rKey, pServer->m_Key+iK*16, 16);
								sub_makekey_40EB56(rKey, 16, xorstring);	// ����xorstring
								sub_xorcode_40EBC7((unsigned char*)(tmpPacket+5), xorlen, xorstring);

								int bm2CRLF = Search2CRLF.find((const char*)tmpPacket+5, xorlen);
								int bmFlag = SearchXPower.find((const char*)tmpPacket+5, xorlen);
								if(bm2CRLF != -1 && bmFlag==-1) // ��˫�س� �� Content-length
								{
									// ���ַ����������л�Ӧ
									sub_makekey_40EB56(rKey, 16, xorstring);	// ��������xorstring
									sub_xorcode_40EBC7(tmpPacket+5+bm2CRLF+4, xorlen-bm2CRLF, xorstring);

									unsigned char *psessid = tmpPacket+5+bm2CRLF+4; // clienthello��sessionid��λ��
									if(psessid[6]==0x10 && psessid[0x17]==0x20)
									{
										psessid += 0x18;
										unsigned char cKey[20];
										unsigned char cc1,cc2;
										for (int ih = 0; ih < 32 ; ih+=2)
										{
											cc1 = psessid[ih];
											cc2 = psessid[ih+1];
											if(cc1 >= '0' && cc1 <= '9')
												cKey[ih/2] = cc1-'0';
											else
												cKey[ih/2] = cc1-0x57;
											
											if(cc2 >= '0' && cc2 <= '9')
												cKey[ih/2] = cKey[ih/2]*16+(cc2-'0');
											else
												cKey[ih/2] = cKey[ih/2]*16+(cc2-0x57);
										}

										memcpy(pServer->m_Key+iK*16, cKey, 0x10);	// ���ǲ�ѯKey
										pServer->m_bHave = TRUE;
										g_MapFgServer.SetAt(sIP, pServer);
										WriteLog(HTTPPRO, "ȷ�������� %s", sIP);
									}
								}
							}
						}	// end ����С��2000
					}	// ��ϣ����

					// �巢�ֻỰ
					CheckFreegateServer();
				}
				errfree = 4;
				if(rCheck == FG_675COMMUNICATION || rCheck == FG_676COMMUNICATION)
				{
					CNogSession *pfg = new CNogSession(packh, g_httpFrom);
					pfg->m_dceprocotol=this;
					if(pfg != NULL)
					{
						pfg->SetOriKey(rCheck, rKey, (const unsigned char*)pPacket);	// ���ݽ��ܻỰ, ���ý�Կ
						m_httpMissObj->m_SocketMap.SetAt(pfg->m_sip+"_"+pfg->m_dip, (SocketSession*&)pfg);
					}
				}

				bDeal = TRUE;
				return TRUE;
			}
			}catch (...) {
				WriteLog(HTTPPRO, "ProcessHttpPacket isFreegate() Error, no:%u!!", errfree);
			}
		}
		if (FALSE == bDeal)
		{
			if (memcmp(pPacket,"\x57\x41\x01\x04",4) == 0)
			{
				if (memcmp(pPacket + 87,"\x0c\xb5\xfc\x0d",4) == 0)
				{
					const char *userbegin = pPacket + 91;
					const char *userend = strstr((pPacket + 91),"\xec\x6c");
					if (userend)
					{
						char tmp[32] = {0};
						memcpy(&tmp,userbegin,userend - userbegin);
						CString username(tmp);
						HttpSession tmpsession(packh, g_httpFrom);
						tmpsession.m_dceprocotol=this;					
						CString txt;
						txt.Format("user=%s\r\nwebname=WhatsApp\r\nmsgtype=\r\ntype=�ֻ�\r\nstate=ʹ��\r\n",username);
		//				tmpsession.WriteIndexFile("software_running","software_running",txt,tmpsession.m_dceprocotol->m_httpMissObj);
						bDeal = TRUE;
					}
				}
			}
		}
		if(FALSE == bDeal)
		{
			try{

			DWORD rCheck = isHttps(pPacket, packh);
			if (rCheck)
			{
				if(rCheck > 0)
				{
					HttpsSession *pfg = new HttpsSession(packh, g_httpFrom, (rCheck==1)?TRUE:FALSE);
					pfg->m_dceprocotol=this;
					if(pfg != NULL)
					{
						pfg->InsertPacket(packh, pPacket);	// �װ�
						m_httpMissObj->m_SocketMap.SetAt(pfg->m_sip+"_"+pfg->m_dip, (SocketSession*&)pfg);
					}
				}

				bDeal = TRUE;
				return TRUE;
			}

			}catch (...) {
				WriteLog(HTTPPRO, "ProcessHttpPacket isHttps() Error!!");
			}
		}
		
		if (FALSE == bDeal)
		{
			if( (packh.m_sport == 8080) || (packh.m_sport == 80) || (packh.m_sport == 443) ||
				(packh.m_dport == 8080) || (packh.m_dport == 80) || (packh.m_dport == 443) )
			{
				char strYiXinKey[32] = {0};	
				sprintf(strYiXinKey,"%08x_%08x_%04x_%04x",*(unsigned int*)packh.m_sip,*(unsigned int*)packh.m_dip,packh.m_sport,packh.m_dport);
				YiXinHttpSession * clsYiXinSession = 0;
				int Result = cmapYiXin.Lookup(strYiXinKey,clsYiXinSession);
				if (Result)
				{
					clsYiXinSession->ProcessYiXinPacket(packh,pPacket,this);
				}
				else 
				{
					Result = YiXinHttpSession::IsYiXinStartPacket(packh,pPacket);
					if(Result ) 
					{
						clsYiXinSession = new YiXinHttpSession(packh, pPacket);
						clsYiXinSession->m_uiMode = Result;
						cmapYiXin.SetAt(strYiXinKey,clsYiXinSession);
						Result = clsYiXinSession->ProcessYiXinPacket(packh,pPacket,this);
						bDeal = TRUE;
						return TRUE;
					}
				}
			}
		}

		if(FALSE == bDeal)
		{
			if (v_stat)
			{
				if (packh.m_sport == 443)
				{
					CString strISP;
					if (v_statIdc && v_IdcIpLib.QueryIdcIP(Swap4Byte(*((DWORD*)(packh.m_sip))), strISP))
					{
						m_HttpsPacketCount++;	// Https�İ���
						int r = IsHttpsPacket(packh, pPacket);
						if (r > 0)
						{
							// ��HTTPS�İ� Server --> Client
							CString ttstr;
							ttstr.Format("%03u.%03u.%03u.%03u\1%s\1https\1HTTPS����\1%05u\1HTTPS\1%s\1\1", 
								packh.m_sip[0], packh.m_sip[1], packh.m_sip[2], packh.m_sip[3],
								strISP, packh.m_sport, TYPESTATE_DECIDED);
							v_IdcServerStat[IMSG_PROTOCOL].AddServer(ttstr);
						}
					}
					bDeal = TRUE;
				}
				
			}
		}
		if (FALSE == bDeal)
		{
			if (IsAQWS360DevProcess(pPacket))
			{
				if (AQWS360DevProcess(pPacket,packh,this))
				{
					bDeal = TRUE;
				}
			}
		}
		if(FALSE == bDeal)
		{
			// δ�����ҷ�ͳ���ð�
			m_RubbishPacketCount++;
		}
	}
	catch (...)
	{
		WriteLog(HTTPPRO, "ProcessHttpPacket() Error!!");
	}
	return TRUE;
}


CString GetDomain(CString url)
{
	CString domain;
	
	if(url.GetLength()<=7)
		domain = url;
	else
	{
		if(memcmp(url,"http://",7)==0)
		{
			int pos=0;
			CString tmp;
			tmp=url.Mid(7);
			pos=tmp.Find('/');
			if(pos>0)
			{
				CString tmpstr;
				tmpstr=tmp.Left(pos);
				domain=tmpstr;
			}
			else
			{
				domain=tmp;
			}
		}
		else
		{
			domain=url;
		}
	}
	if (domain.GetLength() > 4)
	{
		if (memcmp(domain,"www.",4)==0)
		{
			domain.Format("%s",domain.Mid(4));
		}
	}

	return domain;
}  

// LPCTSTR GetDataSrc(DATEHEADER& packh, int PacketFrom, CString &strISP)
// {
// 	LPCTSTR rDataSrc = _T("");
// 	if(PacketFrom > 0)
// 	{
// 		if ((PacketFrom == CDMATCP) || (PacketFrom == CDMAUDP))
// 		{
// 			rDataSrc = DATASRC_CDMA;
// 		}
// 		else if ((PacketFrom == GPRSTCP) || (PacketFrom == GPRSUDP))
// 		{
// 			rDataSrc = DATASRC_GPRS;
// 		}
// 	}
// 	
// 	if(*rDataSrc==0)
// 	{
// 		DWORD dwIP = Swap4Byte(*(DWORD*)packh.dip);
// 		if(v_IdcIpLib.QueryIdcIP(dwIP, strISP))
// 			rDataSrc = DATASRC_IDC;
// 		else
// 		{
// 			dwIP = Swap4Byte(*(DWORD*)packh.sip);
// 			if(v_IdcIpLib.QueryIdcIP(dwIP, strISP))
// 				rDataSrc = DATASRC_IDC;
// 		}
// 	}
// 	if(*rDataSrc==0)
// 		rDataSrc = DATASRC_OTHER;
// 
// 	return rDataSrc;
//}
