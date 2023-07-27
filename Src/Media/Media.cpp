#include "stdafx.h"
#include "MediaSession.h"
#include "Media.h"
#include "Flashvideo.h"
#include "ucvideo.h"
#include "qqvideo.h"
#include "msnvideo.h"
#include "yahoovideo.h"
#include "voip.h"
#include "popovideo.h"
#include "sqqvideo.h"
#include "blueskyvideo.h"
#include "bdcvideo.h"
#include "vgate.h"		// 语音网关分析
#include "VpVideo.h"
#include "UUTong.h"
#include "StreamRTSP.h"
#include "DoShowVideo.h"
#include "YYAudio.h"

#include "VoipQQ.h"
#include "Voip97CallSession.h"
#include "VoipALICallSession.h"
#include "VoipDYTSession.h"
#include "VoipHHTSession.h"
#include "VoipKCSession.h"
#include "VoipKBCALLSession.h"
#include "VoipKubaoSession.h"
#include "VoipYMCallSession.h"
#include "VoipUXINSession.h"
#include "VoipHHCallSession.h"
#include "VoipCLOUDCALLSession.h"

#include "VoipMobileKBCALLSession.h"
#include "VoipMobileAILIAOSession.h"
#include "VoipMobileG723Session.h"
#include "VoipMobileHHCALLSession.h"
#include "VoipMobileTTSession.h"
#include "VoipTalkBox.h"
#include "VoipETSession.h"
#include "VoipMedia.h"

#include "ZelloAudio.h"

#include "FreeGateFinder.h"

extern BOOL IsFlvVideo (UCHAR * p,USHORT len);
//flv 下行和没有特征的上行
extern BOOL ParserFlashCommand1 (UCHAR * p,USHORT len);

extern int isEphVideo(DATEHEADER& packh, WORD* p);
extern int isYAHOOVideo(DATEHEADER& packh, BYTE* p);
extern int isVOIPVideo(DATEHEADER& packh, BYTE* p);
extern int isVPVideo(DATEHEADER& dateHeader, BYTE* tp);
extern int isUUTong(DATEHEADER& dateHeader, BYTE* tp);
extern int isUUTong(DATEHEADER& dateHeader, BYTE* tp);
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

DceMission *g_MediaMissObj = NULL;
DWORD	g_MediaFileNum = 0;
DATEHEADER DataHeader;
char	g_Packet[MAXLEN];
CString	MediaTmpPath;



BOOL g_bProcessVoip;
// static LPCTSTR subproname[] = 
// {
// 	_T("media"),
// 	_T("vgate"),
// //	_T("oyescam"),
// 	_T("rtsp"),
// 	NULL
// };
// 
// void* ::operator new (unsigned int sz)
// {
// 	ASSERT(g_mediaHeap!=NULL);
// 	return HeapAlloc(g_mediaHeap, HEAP_NO_SERIALIZE, (sz+15)&~15);
// }
// 
// void  ::operator delete (void* p)
// {
// 	HeapFree(g_mediaHeap, HEAP_NO_SERIALIZE, p);
// }


MediaProtocol::MediaProtocol()
{
	m_ddefiletype = "media";
//	m_outdirlist = subproname;
}

MediaProtocol::~MediaProtocol()
{

}

inline LPCTSTR GetKey(DATEHEADER& packh, char* strkey)
{
	sprintf(strkey, "%03d.%03d.%03d.%03d_%05d_%03d.%03d.%03d.%03d_%05d",
		packh.m_sip[0], packh.m_sip[1], packh.m_sip[2], packh.m_sip[3], packh.m_sport,
		packh.m_dip[0], packh.m_dip[1], packh.m_dip[2], packh.m_dip[3], packh.m_dport);
	return strkey;
}

inline LPCTSTR GetAckKey(DATEHEADER& packh, char* strkey)
{
	sprintf(strkey, "%03d.%03d.%03d.%03d_%05d_%03d.%03d.%03d.%03d_%05d",
		packh.m_dip[0], packh.m_dip[1], packh.m_dip[2], packh.m_dip[3], packh.m_dport,
		packh.m_sip[0], packh.m_sip[1], packh.m_sip[2], packh.m_sip[3], packh.m_sport);
	return strkey;
}

inline MediaSession* LocateSession(DATEHEADER& packh, char* strkey)
{
	SocketSession *session=NULL;
	if (g_MediaMissObj->m_SocketMap.Lookup(GetKey(packh, strkey), session) >0)
	{
		return (MediaSession*)session;
	}
	if (g_MediaMissObj->m_SocketMap.Lookup(GetAckKey(packh, strkey), session) > 0)
	{
		if (session != NULL)
		{
			if (((MediaSession*)session)->NeedAckSocket())
			{
				return (MediaSession*)session;
			}
		}
	}
	return NULL;
}

void MediaProtocol::ProcessBuffer(DATABUFFER& packetbuf, time_t& packtm)
{
	int error=0;
	try
	{
		int datapos=0;
		char strkey[80];
		DataHeader.m_tm = packtm;
		for (int i = 0; i < packetbuf.packetnum; i++)
		{
			BOOL bCaseFlag = TRUE;
			char packettype = packetbuf.pBuffer[datapos];
			m_PacketFrom = packettype;
			switch(m_PacketFrom)
			{
			case CDMATCP:
			case GPRSTCP:
				packettype = TCPPacket;
				break;
			case CDMAUDP:
			case GPRSUDP:
				packettype = UDPPacket;
				break;
			}
error=1;
			if (packettype != AAAPacket) //(packettype == TCPPacket || packettype == UDPPacket)
			{
				ASSERT(packetbuf.bufsize-datapos >= NEWHEADERLEN);
				memcpy(&DataHeader, packetbuf.pBuffer+datapos, NEWHEADERLEN);
				ASSERT((packetbuf.bufsize-datapos-NEWHEADERLEN)>=DataHeader.m_datalen);
				if ((NEWHEADERLEN+DataHeader.m_datalen)>packetbuf.bufsize-datapos)
					break;
				if (DataHeader.m_datalen==0)
				{
					datapos += NEWHEADERLEN;
					continue;
				}
				ASSERT(DataHeader.m_datalen < MAXLEN);
				memcpy(g_Packet, packetbuf.pBuffer+datapos+NEWHEADERLEN, DataHeader.m_datalen);
				memset(g_Packet + DataHeader.m_datalen, 0, 16);

// 				DWORD TmpIpid;
// 				TmpIpid = v_crc32Calc.GetCRC(g_Packet, DataHeader.m_datalen);
	#ifdef _DEBUG
	//printf("ipid:%d,bhdatelen %d\r\n",TmpIpid,DataHeader.datalen);
	#endif
error=2;
				datapos += NEWHEADERLEN+DataHeader.m_datalen;
				g_MediaMissObj->m_packetnum++;
				
				sprintf(strkey, "%03d.%03d.%03d.%03d_%05d_%03d.%03d.%03d.%03d_%05d",
					DataHeader.m_sip[0], DataHeader.m_sip[1], DataHeader.m_sip[2], DataHeader.m_sip[3], DataHeader.m_sport,
					DataHeader.m_dip[0], DataHeader.m_dip[1], DataHeader.m_dip[2], DataHeader.m_dip[3], DataHeader.m_dport);
				
				
				
				
				// 数据包是语音网关处理下一个包
				if (ProcessVGatePacket(DataHeader, g_Packet, packetbuf.posaddr, m_PacketFrom))
				{
					continue;	
				}
error=3;
				MediaSession* s = LocateSession(DataHeader, strkey);


				if (s)
				{
					int re = s->InsertPacket(DataHeader, g_Packet);
					switch(re)
					{
					case -3:	//
						if (s->CloseSession(0))
							g_MediaMissObj->m_SocketMap.RemoveKey(strkey);
						break;
					case 8:
						if (s->CloseSession(0))
							g_MediaMissObj->m_SocketMap.RemoveKey(strkey);
						break;
					default:
						break;
					}
				}
				else
				{
					int ret=0;
					if ((packettype==UDPPacket) && (DataHeader.m_datalen>10))
					{
error=4;
						if ((DataHeader.m_datalen > 0x30)
							&& (isQQVideo(DataHeader, (BYTE*)g_Packet)))
						{
							s = (MediaSession*) new QQSession(DataHeader, m_PacketFrom);
						}
						else if ((DataHeader.m_datalen >= 24)
							&& isMSNVideo(DataHeader, (BYTE*)g_Packet))
						{
							s = (MediaSession*) new MSNSession(DataHeader, m_PacketFrom);
						}
						else if ((DataHeader.m_datalen > 24)
							&&  isYAHOOVideo(DataHeader,(BYTE *)g_Packet))
						{
							ret=isYAHOOVideo(DataHeader,(BYTE *)g_Packet);
							s = (MediaSession*) new YAHOOSession(DataHeader, m_PacketFrom);	
							if(ret==2)
							{
								s->m_AvType=2; //音频
							}
							else 
							{
								s->m_AvType=1; //视频
							}
						}
						else if ((DataHeader.m_datalen > 0x60)
							&& isSQQVideo(DataHeader,(BYTE *)g_Packet))
						{
							s = (MediaSession*) new SQQSession(DataHeader, m_PacketFrom);	
						}
						else if ((DataHeader.m_datalen > 0x100)
							&& isBLUESKYVideo(DataHeader,(BYTE *)g_Packet))
						{
							s = (MediaSession*) new BLUESKYSession(DataHeader, (BYTE*)g_Packet, m_PacketFrom);	
						}
//						else if ((DataHeader.m_datalen > 24)
//							&& isVOIPVideo(DataHeader,(BYTE *)g_Packet))
//						{
//							s = (MediaSession*) new VOIPSession(DataHeader, m_PacketFrom);	
//						}					
						else if ((DataHeader.m_datalen > 24)
							&& isPOPOVideo(DataHeader,(BYTE *)g_Packet))
						{
							s = (MediaSession*) new POPOSession(DataHeader, m_PacketFrom);	
						}
						else if (isUUTong(DataHeader,(BYTE *)g_Packet))
						{
							s = (MediaSession*) new UUTongSession(DataHeader, m_PacketFrom);					
						}		
						else if (isUcVideo1(DataHeader, g_Packet))
						{
							
							s = (MediaSession*) new UcSession(DataHeader, m_PacketFrom);
							ret=isUcVideo1(DataHeader, g_Packet);
							((UcSession*)s)->m_sinatype=ret;

						}
						else if(isEphVideo(DataHeader,(BYTE *)g_Packet))
						{
							s = (MediaSession*) new DoShowSession(DataHeader, m_PacketFrom);
						}
						else if(DataHeader.m_datalen >=198 && isYYAudio(DataHeader,(BYTE *)g_Packet))
						{
							s = (MediaSession*) new YYAudioSession(DataHeader, m_PacketFrom);	
						}
						else if (int ret = ZelloAudio::IsZelloAudio(DataHeader,g_Packet))
						{
							s = (MediaSession *) new ZelloAudio(DataHeader,m_PacketFrom,g_Packet,ret);
						}
						else if (FreeGateFinder::IsFreeGateFinder(DataHeader,g_Packet))
						{
							s = (MediaSession *)new FreeGateFinder(DataHeader,m_PacketFrom);
						}
						else if (g_bProcessVoip)
						{
							
							
							if (VoipALICallSession::IsVoipALICall(DataHeader,g_Packet))
							{
								s = (MediaSession *) new VoipALICallSession(DataHeader,m_PacketFrom,g_Packet);
							}
							else if (IsVoipQQPacket(DataHeader, g_Packet))
							{
								s = (MediaSession*)new VoipQQSession(DataHeader, m_PacketFrom);
							}
							else if (VoipDYTSession::IsVoipDYT(DataHeader,g_Packet))
							{
								s = (MediaSession *) new VoipDYTSession(DataHeader,m_PacketFrom,g_Packet);
							}
							else if (VoipHHTSession::IsVoipHHT(DataHeader,g_Packet))
							{
								s = (MediaSession *) new VoipHHTSession(DataHeader,m_PacketFrom,g_Packet);
							}
							else if (VoipKCSession::IsVoipKC(DataHeader,g_Packet))
							{
								s = (MediaSession *) new VoipKCSession(DataHeader,m_PacketFrom,g_Packet);
							}
							else if (VoipYMCallSession::IsVoipYMCall(DataHeader,g_Packet))
							{
								s = (MediaSession *) new VoipYMCallSession(DataHeader,m_PacketFrom,g_Packet);
							}
							else if (VoipKBCALLSession::IsVoipKBCALL(DataHeader,g_Packet))
							{
								s = (MediaSession *) new VoipKBCALLSession(DataHeader,m_PacketFrom,g_Packet);
							}
							else if (VoipKuBaoSession::IsVoipKuBao(DataHeader,g_Packet))
							{
								s = (MediaSession *) new VoipKuBaoSession(DataHeader,m_PacketFrom,g_Packet);
							}
							else if (VoipUXINSession::IsVoipUXIN(DataHeader,g_Packet))
							{
								s = (MediaSession *) new VoipUXINSession(DataHeader,m_PacketFrom,g_Packet);
							}
							else if (VoipHHCallSession::IsVoipHHCall(DataHeader,g_Packet))
							{
								s = (MediaSession *) new VoipHHCallSession(DataHeader,m_PacketFrom,g_Packet);
							}
							else if (Voip97CallSession::IsVoip97Call(DataHeader,g_Packet))
							{
								s = (MediaSession *) new Voip97CallSession(DataHeader,m_PacketFrom,g_Packet);
							}
							else if (VoipMobileAILIAOSession::IsVoipMobileAILIAO(DataHeader,g_Packet))
							{
								s = (MediaSession *) new VoipMobileAILIAOSession(DataHeader,m_PacketFrom,g_Packet);
							}
							else if (VoipMobileKBCALLSession::IsVoipMobileKBCALL(DataHeader,g_Packet))
							{
								s = (MediaSession *) new VoipMobileKBCALLSession(DataHeader,m_PacketFrom,g_Packet);
							}
							else if (VoipMobileHHCALLSession::IsVoipMobileHHCALL(DataHeader,g_Packet))
							{
								s = (MediaSession *) new VoipMobileHHCALLSession(DataHeader,m_PacketFrom,g_Packet);
							}							
							else if (VoipMobileG723Session::IsVoipMobileg723(DataHeader,g_Packet))
							{
								s = (MediaSession *) new VoipMobileG723Session(DataHeader,m_PacketFrom,g_Packet);
							}
							//可能与HHCALL有重合，放在最后面
							else if (VoipCLOUDCALLSession::IsVoipCloudCall(DataHeader,g_Packet))
							{
								s = (MediaSession *) new VoipCLOUDCALLSession(DataHeader,m_PacketFrom,g_Packet);
							}

							else if (VoipETSession::IsVoipET(DataHeader,g_Packet))
							{
								s = (MediaSession *) new VoipETSession(DataHeader,m_PacketFrom,g_Packet);
							}
							else if (VoipADLMediaSession::IsVoipADLMedia(DataHeader, g_Packet))
							{
								s = (MediaSession*)new VoipADLMediaSession(DataHeader, m_PacketFrom);
							}
							else if (VoipRYBMediaSession::IsVoipRYBMedia(DataHeader, g_Packet))
							{
								s = (MediaSession*)new VoipRYBMediaSession(DataHeader, m_PacketFrom);
							}
							else if (Voip97CallMediaSession::IsVoip97CallMedia(DataHeader, g_Packet))
							{
								s = (MediaSession*)new Voip97CallMediaSession(DataHeader, m_PacketFrom);
							}
							else if (VoipHHTMediaSession::IsVoipHHTMedia(DataHeader, g_Packet))
							{
								s = (MediaSession*)new VoipHHTMediaSession(DataHeader, m_PacketFrom);
							}
							else if (VoipYltMediaSession::IsVoipYltMedia(DataHeader, g_Packet))
							{
								s = (MediaSession*)new VoipYltMediaSession(DataHeader, m_PacketFrom);
							}
							else if (VoipMobileTTSession::IsVoipMobileTT(DataHeader, g_Packet))
							{
								s = (MediaSession*)new VoipMobileTTSession(DataHeader, m_PacketFrom,g_Packet);
							}

						}
						
						error=5;
					}
					else if(packettype == TCPPacket)
					{
error=6;
						if ((DataHeader.m_datalen > 8)
							&& IsFlvVideo((UCHAR *)g_Packet, DataHeader.m_datalen))
						{
							s = (MediaSession*) new FlashSession(DataHeader, m_PacketFrom);
							((FlashSession*)s)->m_havesep=TRUE;
						}	
						else if ((DataHeader.m_datalen > 8)
							&& ParserFlashCommand1((UCHAR *)g_Packet,DataHeader.m_datalen))
						{
							s = (MediaSession*) new FlashSession(DataHeader, m_PacketFrom);
							((FlashSession*)s)->m_havesep=FALSE;
						}	
						else if ((DataHeader.m_datalen > 20)
							&& IsRTSPPacket(DataHeader, g_Packet))
						{
							s = (MediaSession*) new RtspSession(DataHeader, m_PacketFrom);
						}
						else if ((DataHeader.m_datalen > 0x100)
							&& isBLUESKYVideo(DataHeader,(BYTE *)g_Packet))
						{
							s = (MediaSession*) new BLUESKYSession(DataHeader, (BYTE*)g_Packet, m_PacketFrom);	
						}
						else if ((DataHeader.m_datalen > 20)
							&& isVPVideo(DataHeader,(BYTE *)g_Packet))
						{
							s = (MediaSession*) new VPSession(DataHeader, m_PacketFrom);	
						}
						else if ((DataHeader.m_datalen > 20)
							&& isQQVideo(DataHeader,(BYTE *)g_Packet))
						{
							s = (MediaSession*) new QQSession(DataHeader, m_PacketFrom);
						}
						else if ((DataHeader.m_datalen > 24)
							&& isBDCVideo(DataHeader,(BYTE *)g_Packet))
						{
							s = (MediaSession*) new BDCSession(DataHeader, m_PacketFrom);	
						}
// 						else if ((DataHeader.m_datalen > 24)
// 							&& isMSNVideo(DataHeader,(BYTE *)g_Packet))
// 						{
// 							s = (MediaSession*) new MSNSession(DataHeader, m_PacketFrom);
// 						}
// 						else if ((DataHeader.m_datalen > 24)
// 							&& isYAHOOVideo(DataHeader,(BYTE *)g_Packet))
// 						{
// 							s = (MediaSession*) new YAHOOSession(DataHeader, m_PacketFrom);	
// 						}
						else if ((DataHeader.m_datalen > 0x60)
							&& isSQQVideo(DataHeader,(BYTE *)g_Packet))
						{
							s = (MediaSession*) new SQQSession(DataHeader, m_PacketFrom);	
						}
						else if ((DataHeader.m_datalen > 24)
							&& isPOPOVideo(DataHeader,(BYTE *)g_Packet))
						{
							s = (MediaSession*) new POPOSession(DataHeader, m_PacketFrom);	
						}
						else if (VoipTalkBox::IsVoipTalkBox(DataHeader,g_Packet))
						{
							s = (MediaSession *)new VoipTalkBox(DataHeader,m_PacketFrom,g_Packet);
						}
						else if ( int codec = ZelloAudio::IsZelloAudio(DataHeader,g_Packet))
						{
							s = (MediaSession *)new ZelloAudio(DataHeader,m_PacketFrom,g_Packet,codec);
						}

	
error=7;						
					}
					if(s>0)
					{
						GetKey(DataHeader, strkey);
error=8;
						g_MediaMissObj->m_SocketMap.SetAt(strkey, (SocketSession*&)s);
						s->m_spyaddr = packetbuf.posaddr;
 						s->InsertPacket(DataHeader, g_Packet);
					}
				}
			}
		}
		packtm = DataHeader.m_tm;
	}
	catch (...)
	{
		WriteLog(MEDIAPRO, "ProcessBuffer() catch no=%d",error);
	}
	return;
}

BOOL MediaProtocol::InitObject(DceMission* pMiss)
{
	WriteLog(MEDIAPRO, "**************** DCE Module-Media(ver:%s %s) Start Run!! ****************",
		__DATE__, __TIME__);
//	pMiss->m_SocketMap.m_memheap=g_mediaHeap;
// 	pMiss->m_SocketMap.InitHashTable(10003, TRUE);
// 	if (pMiss->m_MissHeap == NULL)
// 	{
// 		pMiss->m_MissHeap = g_mediaHeap;
// 	}
	if (pMiss->m_logfilename[0] == 0)
	{
		pMiss->m_logfilename = MEDIAPRO;
	}
	
	g_MediaMissObj = pMiss;
	MediaTmpPath = v_datapath+"\\tmp\\dce4\\";

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

	
	
//	int nRet = _access("e:\\netspy\\para\\voip.ini",0);
	if (_access("e:\\netspy\\para\\voip.ini",0) == -1)
	{

		g_bProcessVoip = TRUE;
	}
	else
	{
		g_bProcessVoip = FALSE;

	}
	

	
	return TRUE;
}


