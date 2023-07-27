//***************************************************************
//	Copyright (C) 2007 - ɽ������Ƽ���չ���޹�˾��Ȩ����
//	�ļ���: 	Mir
//	�汾:		1.0
//	��������:	2007-7-12
//	����:		��ǿ
//	��Ҫ����:	�����Э�黹ԭ
//	ģ������:	
//	��Ҫ����:	RecoverChuanQi
//				MirSession::OutputFile
//**************************************************************** 

#include "stdafx.h"
#include "imsg.h"
#include "Mir.h"

/************************* ���� **************************/
#define		CHUANQI_3		1		// ����3
#define		CHUANQI_WORLD	2		// ��������

BOOL RecoverChuanQi(LPCHUANQIPAC pChuanQiStruc, DATEHEADER& packh, char* pPacket);

BOOL DecodeChanQi3(char *ptxt, int& len);

BOOL DecodeChanQiWorld(char *ptxt, int& len);

MirSession::MirSession(DATEHEADER& packh, int aT)
:SocketSession(packh, aT)
{
	m_closetm = packh.m_tm + 120;
	g_GameSion++;
}

MirSession::~MirSession()
{
	g_GameSion--;
}

//*************************************************
//	��������:		MirSession::OutputFile
//	����:			����ļ�
//	�������:		optm �ػ�ʱ��
//					opinfo ��������Ϣ
//	�������:		
//	����ֵ:			
//	����:			
//**************************************************
void MirSession::OutputFile(time_t optm, LPCHUANQIPAC opinfo)
{
	// ƥ��IDC����Դ
	CString tmpisp;
	if (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_cdip), tmpisp))
	{
		if (m_dataSrc == DATASRC_OTHER)
		{
			m_dataSrc = DATASRC_IDC;
			m_strISP = tmpisp;
		}
		if ((v_statIdc) && (opinfo->m_PacketType == 1))
		{
			//��½��дIDCServer
			CString ttstr;
			ttstr.Format("%s\1%s\1hf\1��Ϸ\1%s\1����(��������)\1%s\1\1", 
				m_dip.Left(15), m_strISP, m_dip.Right(5), TYPESTATE_DECIDED);
			v_IdcServerStat[IMSG_PROTOCOL].AddServer(ttstr);
		}
	}
	else if ((m_dataSrc == DATASRC_OTHER)
		&& (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_csip), m_strISP)))
	{
		m_dataSrc = DATASRC_IDC;
	}

	if (m_userid.IsEmpty())
	{
		m_userid = opinfo->m_UserName;
	}
	// 
	CString state;
	switch (opinfo->m_PacketType)
	{
	case 1:		//��½��client --->Server
		m_password = opinfo->m_PWD ;
		state = "��¼";
		// ��¼�İ�Ҫд���ʷ
		if (v_statGame)
		{
			HistoryRecord hisRec;
 			hisRec.SetRecord(IDX_FORWTYPE,	"1");
 			hisRec.SetRecord(IDX_ID,		m_userid);
			hisRec.SetRecord(IDX_USER,		m_userid);

			hisRec.SetRecord(IDX_PASS,		m_password);
			hisRec.SetRecord(IDX_STATE,		"��¼");
			if (opinfo->m_Type == 1)
			{
				//imsg_����3
				WriteHisData("imsg", "imsg_mir", &hisRec, g_pImsgMissObj,IMSG_PROTOCOL);//, HISTORY_DATA);
			}
			else
			{
				//imsg_��������
				WriteHisData("imsg", "imsg_mir", &hisRec, g_pImsgMissObj,IMSG_PROTOCOL);//, HISTORY_DATA);
			}
		}
		break;
	case 2:		//��½ʧ�ܰ� Server-->Client
		m_password.Empty();
		state = "��¼ʧ��";
		break;
	case 4:		//4.Server-->Client ��ʾ ��Ϸ�������б�
		state.Format("�����Ϸ�������б�: %s", opinfo->m_Info);
		break;
	case 5:		//Client-->Server ��ʾ �û�ѡ�����Ϸ��������
		state.Format("ѡ�����Ϸ��������: %s", opinfo->m_Info);
		break;
	case 6:		//Server-->Client ��ʾ ��Ϸ��������� IP
		state.Format("�����Ϸ���������IP: %s", opinfo->m_Info);
		break;
	default:
		return;
	}


	// ƥ�䰸����Ԥ����������Ϣ
	CString casestr;		// ��Ҫƥ��ؼ��ֵĴ�
	casestr.Format("%s\r\n\r\n%s\r\n\r\n%s", m_userid,m_password, opinfo->m_Info);
	DWORD dwstate = 0;
	SearchCaseOrWarn(this, casestr, casestr.GetLength(), dwstate);
	SearchCaseEmailIm(this, m_userid, m_userid.GetLength(), 0);

	m_OpenCaseFlg = v_opencasechuanqi;
	m_OpenWarnFlg = v_openwarnchuanqi;
	m_OpenIndxFlg = v_openflagchuanqi;

	// ����ļ�
	CString text;
	if (!m_userid.IsEmpty())
	{
		text.Format("user= %s\r\n", m_userid);
	}
	if (!m_password.IsEmpty())
	{
		text.Format("%spass= %s\r\n", (CString)text, m_password);
	}
	text.Format("%sstat= %s\r\n", (CString)text, state);
	m_indexfname = CreateFilename(m_sip, m_dip, "imsg_mir.indexfile.txt");
	ASSERT(!m_indexfname.IsEmpty());

	if (opinfo->m_Type == 1)
	{
		WriteIndexFile("imsg_mir", "imsg_mir", text, g_pImsgMissObj);
	}
	else
	{
		WriteIndexFile("imsg_mir", "imsg_mir", text, g_pImsgMissObj);
	}
	m_CaseArr.RemoveAll();
	m_WarnArr.RemoveAll();
}

int MirSession::InsertPacket(DATEHEADER& packh,const char *pPacket)
{
	try
	{
		m_lastpacktm = packh.m_tm;
 		if (SearchSeq(m_seq, packh.m_sequence))	// �򵥲����ظ��İ�
		{
			return 0;
		}
		
		if (packh.m_type==ProxyTCP)
		{
			m_ProxyType = 1;
		}

		CHUANQIPAC ChuanqiInfo;
		ChuanqiInfo.m_PacketType = 0;
		ChuanqiInfo.m_Type = 0;
		ChuanqiInfo.m_direction = 0;
		CString text;
		if (RecoverChuanQi(&ChuanqiInfo, packh, (char*)pPacket))
		{
			ASSERT((ChuanqiInfo.m_direction==1) || (ChuanqiInfo.m_direction==2));
			int direction = 0;
			if (memcmp(packh.m_sip, m_csip, 4) == 0)
			{
				direction = ChuanqiInfo.m_direction;
			}
			else
			{
				if (ChuanqiInfo.m_direction == 1)
				{
					direction = 2;
				}
				else
				{
					direction = 1;
				}
			}
			
			if (direction == 2)
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

			// ���Mir�ļ�
			OutputFile(packh.m_tm, &ChuanqiInfo);
 		}
		return 0;
	}
	catch (...)
	{
		WriteLog(IMSGPRO, "MirSession::InsertPacket() Error!!");
		return -3;
	}
}

//*************************************************
//	��������:		RecoverChuanQi
//	����:			��ԭ��������ݰ�
//	�������:		packh �ǰ�ͷ�ṹ
//					pPacket �ǰ�����
//	�������:		pChuanQiStruc ��ԭ�������
//	����ֵ:			�����ԭ����Ч�������򷵻�TRUE
//	����:			
//**************************************************
BOOL RecoverChuanQi(LPCHUANQIPAC pChuanQiStruc, DATEHEADER& packh, char* pPacket)
{
	CString Encode;
	CString UserPass;
	CString ChuanqiInfo;
	if(packh.m_datalen>17 && pPacket[0]=='#' && pPacket[packh.m_datalen-1]=='!')
	{				
		if (packh.m_dport==7000)
		{
			if (packh.m_datalen < 20)
				return FALSE;
			if (pPacket[1]>'0' && pPacket[1]<='9')
			{
				if (memcmp(pPacket+2,"<<<<<I@C<<<<<<<<",16)==0)
				{	
					//1. //logon 1 mir3
					//#2<<<<<I@C<<<<<<<<YrAjXRajUsI`Go@mHO@mHL!
					pChuanQiStruc->m_Type = 1; //"imsg_����3";
					pChuanQiStruc->m_PacketType = 1;
					pChuanQiStruc->m_direction = 1;

					// ����
					int txtlen = packh.m_datalen - 19;
					char *ptext = pPacket + 18;
					if (!DecodeChanQi3(ptext, txtlen))
						return FALSE;
					// ��ȡ�û���������
					ptext[txtlen] = 0;
					char *tp = strstr(ptext, "/");
					if (tp != NULL)
						*tp++ = 0;
					pChuanQiStruc->m_PWD = tp;	// ����
					pChuanQiStruc->m_UserName = ptext;	// �û���
				}
				else if (memcmp(pPacket+2,"<<<<<B\\<<<<<<<<<",16)==0)
				{
					//5. mir3 Client-->Server ��ʾ �û�ѡ�����Ϸ��������
					//����	#3<<<<<B\<<<<<<<<<oJoKl_@!
					pChuanQiStruc->m_Type = 1; //"imsg_����3";
					pChuanQiStruc->m_PacketType = 5;
					pChuanQiStruc->m_direction = 1;
					// ����
					int txtlen = packh.m_datalen - 19;
					char *ptext = pPacket + 18;
					if (!DecodeChanQi3(ptext, txtlen))
						return FALSE;
					ptext[txtlen] = 0;
					pChuanQiStruc->m_Info = ptext;
				}
			}
			else if(memcmp(pPacket+2,"vvfuvIguvvfuvvfu",16)==0)
			{
				//1. logon mirworld
				//#2vvfuvIguvvfuvvfuqqUuqqUuk]Ea]]Ee]=!	
				//#9vvfuvIguvvfuvvfuqqUuqqUuk]Ea]]Ee]=!
				pChuanQiStruc->m_Type=2; //"imsg_��������";
				pChuanQiStruc->m_PacketType = 1;
				pChuanQiStruc->m_direction = 1;
				// ����
				int txtlen = packh.m_datalen - 19;
				char *ptext = pPacket + 18;
				if (!DecodeChanQiWorld(ptext, txtlen))
					return FALSE;
				ptext[txtlen] = 0;
				// ��ȡ�û���������
				char *tp = strstr(ptext, "/");
				if (tp != NULL)
					*tp++ = 0;
				pChuanQiStruc->m_PWD = tp;	// ����
				pChuanQiStruc->m_UserName = ptext;	// �û���
			}
			else if(memcmp(pPacket+2,"vvfuv^fsvvfuvvfu",16) == 0)
			{
				//5. mir3 Client-->Server ��ʾ �û�ѡ�����Ϸ��������
				//#2vvfuv^fsvvfuvvfuF>_@>;!
				pChuanQiStruc->m_Type=2; //"imsg_��������";
				pChuanQiStruc->m_PacketType =5;
				pChuanQiStruc->m_direction = 1;
				// ����
				int txtlen = packh.m_datalen - 19;
				char *ptext = pPacket + 18;
				if (!DecodeChanQiWorld(ptext, txtlen))
					return FALSE;
				ptext[txtlen] = 0;
				pChuanQiStruc->m_Info = ptext;
			}
		}
		else if (packh.m_sport==7000)
		{
			if(packh.m_datalen > 18)
			{
				if(memcmp(pPacket+1,"<<<<<=@><<<<<<<<",16)==0 )
				{
					//4. mir3 Server-->Client ��ʾ ��Ϸ�������б� ���� 18 ���ֽ�	
					//����	#<<<<<=@><<<<<<<<oJoKl_@dHN`woJoKl_DdHN`woJoKl_HdHN`woJoKl_LdHN`w!
					//		#<<<<<=@><<<<<<<<jggJu>\nFOkHmi[VF?DeJxkJqzTdH^`wmy^wn^\nFOh!
					pChuanQiStruc->m_Type = 1;	//"imsg_����3";
					pChuanQiStruc->m_PacketType = 4;
					pChuanQiStruc->m_direction = 2;

					// ����
					int txtlen = packh.m_datalen - 18;
					char *ptext = pPacket + 17;
					if (!DecodeChanQi3(ptext, txtlen))
//					if (!DecodeChanQiWorld(ptext, txtlen))
						return FALSE;
					ptext[txtlen] = 0;
					pChuanQiStruc->m_Info = ptext;
				}
				else if(memcmp(pPacket+6,"=D><<<<<<<<",11)==0 )
				{
					//6. mir3 Server-->Client ��ʾ ��Ϸ��������� IP
					//	����	#SIH><=D><<<<<<<<H_@lG_DnG_@uI>tnH?DkIo@lH>xmJ?PmJ?<!
					//	����	#QL`<<=D><<<<<<<<H_@tG_@qG_DmG_@pI^xsHO<uGoDoJ?`!
					pChuanQiStruc->m_Type=1;	//"imsg_����3";
					pChuanQiStruc->m_PacketType = 6;
					pChuanQiStruc->m_direction = 2;
					// ����
					int txtlen = packh.m_datalen - 18;
					char *ptext = pPacket + 17;
					if (!DecodeChanQi3(ptext, txtlen))
						return FALSE;
					ptext[txtlen] = 0;
					pChuanQiStruc->m_Info = ptext;
				}
			}
			else	// len == 18
			if (memcmp(pPacket+3, "{{{{", 4)==0 && 
				memcmp(pPacket+8, "=<<<<<<<<!", 10)==0 )
			{	
				//2. logonFail 2 mir3
				//����	#zk{{{{X=<<<<<<<<!
				//����	#{{{{{{X=<<<<<<<<!
				pChuanQiStruc->m_Type = 1;	//"imsg_����3";
				pChuanQiStruc->m_PacketType = 2;
				pChuanQiStruc->m_direction = 2;
			}
		}
	}
	return (pChuanQiStruc->m_PacketType != 0);
}

inline void DeOneBase64(char *psbuf, char *pdbuf)
{
	//******************************************************
	//	���ڴ��еı���psbuf����(00wwwwww 00xxxxxx 00yyyyyy 00zzzzzz)���
	//	����pdbuf(wwwwwwxx xxxxyyyy yyzzzzzz 00000000)
	//******************************************************
	__asm	mov	edx, psbuf
	__asm	mov	ecx, dword ptr [edx]
	__asm	mov eax, ecx
	__asm	and eax, 3FH
	__asm	mov ebx, eax
	__asm	shl ebx, 6
	__asm	mov eax, ecx
	__asm	shr eax, 8
	__asm	and eax, 3FH
	__asm	or	ebx, eax
	__asm	shl ebx, 6
	__asm	mov eax, ecx
	__asm	shr eax, 16
	__asm	and eax, 3FH
	__asm	or	ebx, eax
	__asm	shl ebx, 6
	__asm	mov eax, ecx
	__asm	shr eax, 24
	__asm	and eax, 3FH
	__asm	or	ebx, eax
	__asm	shl ebx, 8
	__asm	bswap	 ebx
	__asm	mov	edx, pdbuf
	__asm	mov	dword ptr[edx], ebx
}

//**********************************************************
//	���봫��3������
//**********************************************************
BOOL DecodeChanQi3(char *ptxt, int& len)
{
//	��׼
//	char cMap[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
//	����3
//	char cMap[]="<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_'abcdefghijklmnopqrstuvwxyz{"; 
//	��������
//	char cMap[]="[\\]^_`abcdefghijklmnopqrstuvwxyz;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	DWORD *pcode = (DWORD*)ptxt;
	int strlen = len;
	len = 0;
	DWORD data = 0;
	int i = 3;
	while (strlen >= 4)
	{
		data = *pcode - 0x3C3C3C3C;	// ÿһ���ֽڼ�һ��'<'
		DeOneBase64((char*)&data, ptxt);
		ptxt += 3;
		pcode++;
		strlen -= 4;
		len += 3;
	}
	if (strlen > 0)
	{
		data = *pcode - 0x3C3C3C3C;	// ÿһ���ֽڼ�һ��'<'
		DeOneBase64((char*)&data, (char*)&data);
		memcpy(ptxt, &data, --strlen);
		len += strlen;
	}
	return (len > 0);
}

//**********************************************************
//	���봫�����������
//**********************************************************
BOOL DecodeChanQiWorld(char *ptxt, int& len)
{
//	��׼
//	char cMap[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
//	����3
//	char cMap[]="<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_'abcdefghijklmnopqrstuvwxyz{"; 
//	��������
//	char cMap[]="[\\]^_`abcdefghijklmnopqrstuvwxyz;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	DWORD *pcode = (DWORD*)ptxt;
	int strlen = len;
	len = 0;
	DWORD data = 0;
	int i = 3;
	while (strlen > 0)
	{
		if (*ptxt <= 'z')
			data = (data << 6) + ((*ptxt - ';' + 32) & 0x3F);
		else
			data = (data << 6) + ((*ptxt - '[') & 0x3F);
		*ptxt++;
		strlen--;
		if (i == 0)
		{
			i = 3;
			__asm	mov		eax, data
			__asm	mov		edx, pcode
			__asm	shl		eax, 8
			__asm	bswap	eax
			__asm	mov		dword ptr[edx], eax
			__asm	add		edx, 3
			__asm	xor		eax, eax
			__asm	mov		pcode, edx
			__asm	mov		data, eax
			len += 3;
		}
		else
			i--;
	}
	if (data != 0)
	{
		__asm	mov		eax, data
		__asm	shl		eax, 8
		__asm	bswap	eax
		__asm	mov		dword ptr[pcode], eax
		len += 3;
	}
	return (len > 0);
}





