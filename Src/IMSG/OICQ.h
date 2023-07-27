//***************************************************************
//	Copyright (C) 2007 - ɽ������Ƽ���չ���޹�˾��Ȩ����
//	�ļ���: 	OICQ
//	�汾:		1.0
//	��������:	2007-7-13
//	����:		��ǿ
//	��Ҫ����:	OICQЭ��Ļ�ԭ
//**************************************************************** 
#ifndef INCLUDE_AFX_OICQ_H_20070713_INCLUDED
#define INCLUDE_AFX_OICQ_H_20070713_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif

#include <afxtempl.h>
// QQ2009(TM) qqver >= 141c
#define TM2008						0x141b	// �ð汾��İ���Э�鴦��
#define QQ2009P4					0x1525	// QQ2009P4 QQ2009Э��
#define QQ2009SP5					0x1957	// qq2009SP5
#define QQ_0xdd						0x00dd	// c->s ���Verify_Pass2, �����
											//		Verify_Pass2����Token, PassKey��
											// s->c ���Info_Key1, Verify_Pass2��
#define QQ_0xe5						0x00e5	// s->c ���Info_Key2, Info_Key1��
#define QQ_0x30						0x0030	// s->c ���Session_Key, 2009Э��Info_Key2��
											//		2008Э��ֱ��PassKey��
#define QQ_CMD_LOGIN                0x0022	// login, old
#define TM_CMD_LOGIN                0x0030	// login, 2008Э���
#define TM_CMD_2013LOGIN            0x00aa	// login, 2008Э���

#define TM_CMD_RECV_IM              0x00ce
#define TM_CMD_SEND_IM              0x00cd
#define TM_CMD_LOGOUT               0x0062
#define QQ2013_CMD_LOGIN            0x0173
#define QQ2014_CMD_LOGIN            0x01bb
#define QQ_0825_PACK				0X0825
#define QQ_0836_PACK				0X0836
#define QQ_0828_PACK				0X0828

// qqver < 141c
#define LEN_TEA_KEY      16        //length of key used in TEA-16 Algorithm.
#define MAX_QQ_PASS		 16

// #define QQ_CMD_KEEP_ALIVE           0x0002 // get onlines from tencent
#define QQ_CMD_QUN					0x0002 // Ⱥ����, �汾���ڵ���QQ2009P4, ����֮ǰҲ��, ����ȷ��
#define QQ_CMD_REG_NEW_ID_2         0x0003 // register new id2
#define QQ_CMD_UPDATE_INFO          0x0004 // update information
#define QQ_CMD_SEARCH_USER          0x0005 // serach for user
#define QQ_CMD_GET_USER_INFO        0x0006 // get user information
#define QQ_CMD_ADD_FRIEND_WO_AUTH   0x0009 // add friend without auth
#define QQ_CMD_DEL_FRIEND           0x000a // delete a friend 
#define QQ_CMD_BUDDY_AUTH           0x000b // buddy authentication
#define QQ_CMD_CHANGE_ONLINE_STATUS 0x000d // change online status
#define QQ_CMD_REG_NEW_ID_1         0x0011 // register new id1
#define QQ_CMD_ACK_SYS_MSG          0x0012 // ack system service message
#define QQ_CMD_SEND_IM              0x0016 // send message
#define QQ_CMD_RECV_IM              0x0017 // receive message
#define QQ_CMD_REMOVE_SELF          0x001c // remove self 
#define QQ_CMD_CELL_PHONE_1         0x0021 // cell phone 1
#define QQ_CMD_GET_FRIENDS_LIST     0x0026 // retrieve my freinds list
#define QQ_CMD_GET_BUDDY_LIST		0x0126 // retrieve my freinds list
#define QQ_CMD_GET_FRIENDS_ONLINE   0x0027 // get online friends list
#define QQ_CMD_CELL_PHONE_2         0x0029 // cell phone 2
#define QQ_CMD_SEND_SMS				0x002D // ���ֻ�����
#define QQ_CMD_PRE_LOGIN            0x0062 // prelogin packet, in some higher version
#define QQ_CMD_RECV_MSG_SYS         0x0080 // receive a system message
#define QQ_CMD_RECV_MSG_FRIEND_CHANGE_STATUS  0x0081 // friends change status
#define	QQ_CMD_GET_LEVEL			0x005C

#define QQ_STATUS_HIDDEN	0x28	// ����
#define QQ_STATUS_AWAY		0x1E	// �뿪
#define QQ_STATUS_OFFLINE	0x14	// ����
#define QQ_STATUS_ONLINE	0x0A	// ����


struct OicqBuddy;

//********************** Oicq��Ե�Э�鴦�� **********************************//

typedef struct SOCKETKEY
{
	DWORD	sip;
	DWORD	sport;
	DWORD	dip;
	DWORD	dport;
}SOCKETKEY;


inline UINT AFXAPI HashKey(SOCKETKEY key)
{
	return (((key.sip+key.dip)<<5)+(key.sport+key.dport));
}

inline BOOL AFXAPI operator==(const SOCKETKEY& s1, const SOCKETKEY& s2)
{
	if (memcmp(&s1, &s2, sizeof(SOCKETKEY)) == 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

typedef struct OicqBuddy
{
	WORD		m_TotalPacket;
	WORD		m_PitchonPacket;
	DWORD		m_QQid1;
	DWORD		m_QQid2;
	time_t		m_FirstPackettm;
	BYTE		m_mac[12];
	WORD		m_proxytype;
	WORD		m_linktype;		// 1��ʾ��ϵ  2��ʾ��Ƶ����
	AuthInfo	m_authinfo;
	LPCTSTR		m_spyaddr;
	DWORD		m_DataSrc;		// ����Դ
}OicqBuddy;

inline void AFXAPI DestructElements(OicqBuddy** ppElements, int nCount)
{
	delete *ppElements;
}

class OicqSocketMap : public CMap<SOCKETKEY, SOCKETKEY, OicqBuddy*, OicqBuddy*&>
{
public:
	OicqSocketMap(int nBlockSize)
	{
		ASSERT(nBlockSize > 0);
		m_pHashTable = NULL;
		m_nHashTableSize = 101111;  // default size
		m_nCount = 0;
		m_pFreeList = NULL;
		m_pBlocks = NULL;
		m_nBlockSize = nBlockSize;
		InitHashTable(101111);
	}
	//	HANDLE m_memheap;
// 	void InitHashTable(UINT nHashSize)
// 	{
// 		if (m_nBlockSize<256)
// 			m_nBlockSize=256;
// 		m_nHashTableSize = nHashSize;
// 		ASSERT(m_pHashTable==NULL);
// 		ASSERT(g_ImsgHeap!=NULL);
// 		m_pHashTable = (CAssoc**)HeapAlloc(g_ImsgHeap, 0, sizeof(CAssoc*)*nHashSize);
// 		memset(m_pHashTable, 0, sizeof(CAssoc*) * nHashSize);
// 	}
	void RemoveAll()
	{
		delete[] m_pHashTable;
	}
	
	BOOL RemoveKey(SOCKETKEY key)
	{
		ASSERT_VALID(this);
		
		if (m_pHashTable == NULL)
			return FALSE;  // nothing in the table
		
		CAssoc** ppAssocPrev;
		ppAssocPrev = &m_pHashTable[HashKey(key) % m_nHashTableSize];
		
		CAssoc* pAssoc;
		for (pAssoc = *ppAssocPrev; pAssoc != NULL; pAssoc = pAssoc->pNext)
		{
			if (CompareElements(&pAssoc->key, &key))
			{
				// remove it
				*ppAssocPrev = pAssoc->pNext;  // remove from list
				//				FreeAssoc(pAssoc);
				DestructElements(&pAssoc->value, 1);
				DestructElements(&pAssoc->key, 1);
				pAssoc->pNext = m_pFreeList;
				m_pFreeList = pAssoc;
				m_nCount--;
				ASSERT(m_nCount >= 0);  // make sure we don't underflow
				
				return TRUE;
			}
			ppAssocPrev = &pAssoc->pNext;
		}
		return FALSE;  // not found
	}
	void ClearOicqSocketMap(time_t);
};

// ����QQ��ϵ�˵����
BOOL ProcessOicqBuddyPacket(
				DATEHEADER& packh,
				const char* pPacket, 
				LPCTSTR spyaddr, 
				int PacketFrom);

// ����ļ�
void WriteOicqBuddyFile(OicqBuddy *pSion, SOCKETKEY& key);

//**********************  Oicq����Э�鴦��  **********************************//

#define LEN_TEA_KEY      16        //length of key used in TEA-16 Algorithm.
#define MAX_QQ_PASS		 16

#define	KEY_EMPTY		0
#define	KEY_INIT		1
#define	KEY_SESSION		2
#define KEY_PASS		3

typedef struct OicqPacket
{
	WORD			m_base;		/// �������ݿ�ʼλ��
	BOOL			m_bTCP;		/// ��������
	BOOL			m_bC2S;		/// true �ͻ��˷���, false ����������
	WORD			m_qqcmd;
	WORD			m_seq;
	OLDDATEHEADER	m_packetheader;
	BYTE*			m_pPacketData;
	
	OicqPacket(){memset(this, 0, sizeof(OicqPacket));}
	OicqPacket(DATEHEADER& packh, const char* pPacket, int direction, WORD qqcmd)
	{
		InitPacket(packh, pPacket, direction, qqcmd);
	}
	~OicqPacket()
	{
		if (m_pPacketData!=NULL)
		{
			delete[] m_pPacketData;
			m_pPacketData = NULL;
		}
	}
	int InitPacket(DATEHEADER& packh, const char* pPacket, int direction, WORD qqcmd);
}OicqPacket;

inline BOOL AFXAPI operator<(const OicqPacket& s1, const OicqPacket& s2)
{
//	DWORD dseq = (s2.m_seq - s1.m_seq);	// ����, �����İ�����ǰ��
	DWORD dseq = (s1.m_seq - s2.m_seq);	// ����, ��ʱ����
	return dseq > 0x80000000;		// (s1.Ssequence < s2.Ssequence)
	//	return (s1.Ssequence < s2.Ssequence);
}
typedef list<OicqPacket, allocator<OicqPacket> > OicqPacketList;
typedef OicqPacketList::iterator OicqPacketListIt ;

//*************************************************
//	��������:		isOicqPacket
//	����:			�ж�һ�����Ƿ���QQ�İ�
//	�������:		packh �ǰ�ͷ�ṹ
//					pPacket �ǰ�����
//
//	�������:		cmdid �������
//	����ֵ:			�������QQ�İ��ͷ���0, 
//					��������ǿͻ��˷����İ��ͷ���QQ��(>10000)
//						����Ƿ��������صİ��ͷ���1
//	����:			
//**************************************************
//#define _DEBUG_QQCMD		// �˺��������QQ������
inline DWORD isOicqPacket(DATEHEADER& packh, const char* pPacket, WORD& cmdid, WORD& qqver)
{
// 	const char* tp;
// 	int len;
// 	if (pPacket[packh.datalen-1] != 3)
// 		return FALSE;
// 	if ((packh.type == UDPPacket) || (packh.type == ProxyUDP))
// 	{
// 		len = packh.datalen-7-1;
// 		tp = pPacket;
// 	}
// 	else
// 	{
// 		len = Swap2Byte(*(unsigned short*)pPacket)-2-7-1;//packh.datalen-2-7-1;
// 		tp = pPacket+2;
// 	}
// 	
// 	// tp[0] ͷ, tp[1:2] �汾, tp[3:2] ����
// 	if ((tp[0] == 2) && (tp[3] == 0) && (((len&7)==4) || (tp[4]==0x62 && len==5)))
// 	{
// 		DWORD dwQQ = Swap4Byte(*((DWORD*)(tp+7)));
// 		cmdid = tp[4];
// 		return (dwQQ>10000)? dwQQ : 0;
// 	}
// 	else if((tp[0] == 2) &&	(tp[3] == 0) && (((len&3)==0) || tp[4]==0x62) )
// 	{
// 		return 1;	// ��������Ӧ��
// 	}
// 	return 0;
	const BYTE* tp;
	int len;
	if (pPacket[packh.m_datalen-1] != 3)
		return FALSE;
	tp = (const BYTE*)pPacket;
	if ((packh.m_type == UDPPacket) || (packh.m_type == ProxyUDP))
	{
		qqver = tp[1]*256+tp[2];
		if(qqver >= QQ2009SP5 && packh.m_datalen >= 32)
		{
			//1957 Ϊ qq2009sp5
			//1a35 Ϊ qq2009sp6
			if(tp[11]==2)
				len = packh.m_datalen-7-1-11;
			else
				len = packh.m_datalen-7-1-7;
		}
		else
		{
			len = packh.m_datalen-7-1;
		}

		// tp[0] ͷ, tp[1:2] �汾, tp[3:2] ����	0x126ΪGetBuddyList
		if ((tp[0] == 2) && (tp[3] == 0 || tp[3] == 1))
		{
			if (4 == (len & 7))
			{
				// Client --> Server
				cmdid = tp[3]*256+tp[4];
				qqver = tp[1]*256+tp[2];
				DWORD dwQQ = Swap4Byte(*((DWORD*)(tp+7)));
				return (dwQQ>10000)? dwQQ : 0;
			}
			else if (6 == (len & 7))
			{
				// Client --> Server
				cmdid = tp[3]*256+tp[4];
				qqver = tp[1]*256+tp[2];
				DWORD dwQQ = Swap4Byte(*((DWORD*)(tp+7)));
				return (dwQQ>10000)? dwQQ : 0;
			}
			else if (0 == (len & 7))
			{
				// Server --> Client
// 				if (tp[4] == QQ_CMD_LOGIN || tp[4]==TM_CMD_LOGIN)
// 				{
					cmdid = tp[3]*256+tp[4];
					qqver = tp[1]*256+tp[2];
// 				}
				return 1;	// ��������Ӧ��
			}
			else if ((tp[4] == QQ_CMD_LOGIN || tp[4]==TM_CMD_LOGIN || tp[4] == QQ_0xe5)
				&& (6 == (len & 7)))
			{
				WORD hlen = 0;
				if(qqver >= QQ2009SP5)
					hlen = Swap2Byte(*((WORD*)(tp+7+4+11)));
				else
					hlen = Swap2Byte(*((WORD*)(tp+7+4)));
				if (((len - hlen) > 40) && (0 == (hlen & 7)))
				{
					// Client --> Server �µ�0x22��
					cmdid = tp[3]*256+tp[4];
					qqver = tp[1]*256+tp[2];
					DWORD dwQQ = Swap4Byte(*((DWORD*)(tp+7)));
					return (dwQQ>10000)? dwQQ : 0;
				}
			}
		}
	}
	else
	{
		// TCP�ǿ��Զ�������Ϸ��͵�
		DWORD dwQQ = 0;
		int totallen = packh.m_datalen;
		while (totallen > 10)
		{
			len = Swap2Byte(*((WORD*)tp));
			if ((len > 10) && (len <= totallen))
			{
				tp += 2;
				totallen -= 2;
				len -= 2;

				qqver = tp[1]*256+tp[2];
				if(qqver >= QQ2009SP5 && packh.m_datalen >= 32)
				{
					//1957 Ϊ qq2009sp5
					/// 1a35 Ϊqq2009sp6
					if(tp[11]==2)
					{
						totallen -= 11;
						len -= 11;
					}
					else
					{
						totallen -= 11;
						len -= 7;
					}
				}

				// tp[0] ͷ, tp[1:2] �汾, tp[3:2] ����
				if ((tp[0] == 2) && (tp[3] == 0 || tp[3] == 1))
				{
					if (4 == (len & 7))
					{
						// Client --> Server
						if(!(cmdid == QQ_CMD_LOGIN || cmdid ==TM_CMD_LOGIN))
						{
							qqver = tp[1]*256+tp[2];
							cmdid = tp[3]*256+tp[4];	// ��ϰ�, �ǵ�½����cmdidΪ��������
						}
						DWORD tmpdwQQ = Swap4Byte(*((DWORD*)(tp+7)));
						if (dwQQ == 0)
						{
							dwQQ = tmpdwQQ;
						}
						else if (dwQQ != tmpdwQQ)
						{
							return 0;	// ������
						}
					}
					else if (0 == (len & 7))
					{
						// Server --> Client
						if(cmdid != 0x22)
						{
							qqver = tp[1]*256+tp[2];
							cmdid = tp[3]*256+tp[4];
						}
						return 1;	// ��������Ӧ��
					}
					else if ((tp[4] == QQ_CMD_LOGIN || tp[4] == TM_CMD_LOGIN || tp[4] == QQ_0xe5)
						&& (6 == (len & 7)))
					{
						WORD hlen = Swap2Byte(*((WORD*)(tp+7+4)));
						if (((len - hlen) > 40) && (0 == (hlen & 7)))
						{
							// Client --> Server �µ�0x22��
							if(!(cmdid == QQ_CMD_LOGIN || cmdid ==TM_CMD_LOGIN))
							{
								qqver = tp[1]*256+tp[2];
								cmdid = tp[3]*256+tp[4];
							}
							DWORD tmpdwQQ = Swap4Byte(*((DWORD*)(tp+7)));
							if (dwQQ == 0)
							{
								dwQQ = tmpdwQQ;
							}
							else if (dwQQ != tmpdwQQ)
							{
								return 0;	// ������
							}
						}
					}
				}
				else
				{
					return 0;
				}
			}
			else
			{
				return 0;
			}
			if (cmdid == 0)
			{
				break;
			}
			tp += len;
			totallen -= len;
		}
		return (dwQQ>10000)? dwQQ : 0;
	}
	return 0;

}

/*! m_sontype BITλ����
/// xxxx xxxx xxxx xxxx
      || |||| |||| ||||
      || |||| |||| |||0x1�Ƿ�ƥ�䵽����
      || |||| |||| ||0x2�Ƿ���ƥ�������
      || |||| |||| |0x4�Ƿ�õ�C->S0x30��
      || |||| |||| 0x8�Ƿ�õ�S->C0x30��
      || |||| |||0x10�Ƿ��Լ�������
      || |||| ||0x20����֤����, �ɹ�
      || |||| |0x40�Ƿ�õ�C->S0xdd��
      || |||| 0x80�Ƿ�õ�S->C0xdd��
      || |||0x100�Ƿ�õ�S->C0xe5��
      || ||0x200�Ƿ�õ�S->C0x1d��
    |||| |0x400�Ƿ�õ�C->S0x1d��
    |||| 0x800�Ƿ������qqcrack
    |||0x1000�Ƿ����������
    ||0x2000�Ƿ����������
    |0x4000�Ƿ��������ú����б�
    0x8000 ...
���ڱ�־λ������:
 1���ڿ��Ʋ��ظ�ƥ�䰸�����Ƿ�ɹ�ƥ�䵽������Ϣ.
 2, ����ȷ���Ƿ�õ���ָ������İ�
 3, ���ڿؼ����ظ������/���߼�qqcrack����.
*/
#define BIT_ISCASE		0x1
#define BIT_MATCHCASE	0x2
#define BIT_C2S_0x30	0x4
#define BIT_S2C_0x30	0x8
#define BIT_CHKPASS		0x10
#define BIT_OKSESSION	0x20
#define BIT_C2S_0xdd	0x40
#define BIT_S2C_0xdd	0x80
#define BIT_S2C_0xe5	0x100
#define BIT_S2C_0x1d	0x200
#define BIT_C2S_0x1d	0x400
#define BIT_QQCRACK		0x800
#define BIT_OUTUP		0x1000
#define BIT_OUTDOWN		0x2000
#define BIT_OUTLIST		0x4000

#define BIT_NOTOUTUP	0xFEFFF//����֮��and���ֵ��ʹ����״̬Ϊ��
#define BIT_NOTOUTDOWN	0xFDFFF//����֮��and���ֵ��ʹ����״̬Ϊ��

#define GO_QQ2009	(BIT_S2C_0xdd | BIT_C2S_0xdd | BIT_S2C_0xe5 | BIT_S2C_0x30)	/// �ﵽ�ƽ�����
// #define CRK_QQ1		(BIT_C2S_0xdd | BIT_C2S_0x30 )	/// �㹻ȷ����½�ɹ������qqcrack
#define CRK_QQ2		(BIT_C2S_0xdd | BIT_S2C_0x30 )	/// �㹻ȷ����½�ɹ������qqcrack
#define CRK_QQ3		(BIT_C2S_0xdd | BIT_C2S_0x1d)	/// �㹻ȷ����½�ɹ������qqcrack
#define CRK_QQ4		(BIT_C2S_0xdd | BIT_S2C_0x1d )	/// �㹻ȷ����½�ɹ������qqcrack
#define OCP_SAVE	(GO_QQ2009 | BIT_ISCASE)		/// �����Ҵﵽ�ƽ�����
#define UNIT_OUT	(BIT_ISCASE | BIT_MATCHCASE| BIT_CHKPASS| BIT_OKSESSION|\
 BIT_QQCRACK| BIT_OUTUP| BIT_OUTDOWN| BIT_OUTLIST)	/// ���зǻ���ض����ı�־λ
class CQunINfo :public CObject
{
public:
	DWORD ownQQ;
	DWORD lastQQ;
	DWORD dwQNo;
	CString strQun;
	CStringArray strQunNo;
	CQunINfo(){
		dwQNo = 0;
		ownQQ = 0;
		lastQQ = 0;
	}
};

class OicqSession : public SocketSession
{
public:
	OicqSession(DATEHEADER& packh, int aT, DWORD QQID);
	OicqSession(OicqBuddy *pSion, SOCKETKEY& key);
	virtual ~OicqSession();
	virtual int InsertPacket(DATEHEADER& dataheader,const char *pPacket);
	virtual BOOL CloseSession(time_t nowtm);
public:
// 	WORD		m_cmdtype;
	DWORD		m_sontype;

	time_t		m_filetm;
	CString		m_filename;
	CString		m_password;
	char*		m_pMsgBuf;
	int			m_MsgBufLen;
	CString		m_qqgoupid;
	SimpleSeq	m_SendmsgID;	
	SimpleSeq	m_RecvmsgID;
	CMapStringToOb  m_mapQun;	/// Ⱥ�ڲ��� ��Ӧ Ⱥ�ⲿ��(Ⱥ��)
	CMap<DWORD,DWORD,CString,LPCTSTR> m_mapQQFriendIP; //qq ���ѵ�ip 
public:
	WORD				m_qqver;		///< �˻ỰQQ�汾
	void WriteRealFile(const char* pData, int datalen);

// 	void WriteOicqIndexFile(LPCTSTR stattext, int entity);
	void FormatOicqPacket(WORD QQVer, WORD QQCmd, time_t packtm);
	void FormatQQUid();
	BOOL InitQQParse(DWORD dwQQ, const char *qqPass);
	BOOL SetQQKey(int keyType, const unsigned char *key);
	BOOL LookQQKey(int keyType, unsigned unsigned char *key);
	int  ParseQQPacketList();
	int  SaveOcpFile(BOOL bOutput);
	void  CheckOicqPassword(DWORD qqid);
	void OutData(const char* pData, int datalen);

private:
	unsigned long		m_cntPacket;
	DWORD				m_qqNO;			///< QQ��
	unsigned char		m_sessionkey[LEN_TEA_KEY];
	unsigned char		m_de0x30key[LEN_TEA_KEY];

	OicqPacketList		m_packetlist;
// 	OicqPacket			m_LoginReq;
// 	OicqPacket			m_LoginRes;
	BOOL	CheckHaveGo();	// �Ƿ����㹻���SessionKey

	BOOL	CheckHaveCrack();	// �Ƿ��ѵõ�VerifyToken����½�ɹ�ȷ��

	BOOL	FindPacket(WORD cmdid, BYTE **pPacket, int &lenPacket, BOOL bC2S, int seq);	// �õ���½�����е�һ����
	void	CopyLoginSeg(OicqSession *pLogin);
	int				m_loginTokenLen;
	unsigned char	m_loginToken[0x100];

public:
	BOOL				m_bloginsort;
	OicqPacketList		m_loginList;
};

BOOL DecryptQQPacket(BYTE* data, int& len, BYTE* key, BYTE* outbuf);



#endif