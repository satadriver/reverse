//***************************************************************
//	Copyright (C) 2007 - ɽ������Ƽ��Ƽ���չ���޹�˾��Ȩ����
//	�ļ���: 	MediaSession
//	�汾:		1.0
//	��������:	2007-8-2
//	����:		��ǿ
//	��Ҫ����:	��Media��ĻỰ�������һ�εķ�װ
//	ģ������:
//	��Ҫ����:	
//  ע������:	
//
//**************************************************************** 

#if !defined(AFX_MEDIASESSION_H__0F16E02D_D584_4909_A23B_D7122086E8F4__INCLUDED_)
#define AFX_MEDIASESSION_H__0F16E02D_D584_4909_A23B_D7122086E8F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#include "..\\ReadDDeFile.h"
#include "..\\SocketSession.h"
//*************************************************
//	������:			MediaSession	
//	��̳�:			SocketSession
//	����:			
//	��Ҫ����:		
//	��Ҫ����:		
//	ʹ������:		ÿһ������Ƶ�ĻỰ�඼Ҫ�ӱ���̳�
//					�������ػ����ListToFile����
//					����Ƶ�ļ����ύ��ͨ�������CloseSession�������
//					CloseSession����һ�㲻��Ҫ��������
//*************************************************
class MediaSession : public SocketSession
{
public:
	MediaSession(DATEHEADER& packh, int aT);
	MediaSession();

	virtual ~MediaSession() { };

	virtual BOOL CloseSession(time_t now);

	// �ú�������
	virtual int ListToFile() = NULL;		// �ú�����������

	// �ûỰ�Ƿ�����Socket��
	virtual BOOL NeedAckSocket() {return TRUE;}

	LPCTSTR	m_MediaPro;			// Э������(�������ļ��е�Э���ֶ�ֵ)
	LPCTSTR m_MediaDirection;	// ����Ƶ����(�������ļ��еķ����ֶ�ֵ)
	int		m_AvType;			// ʵ���ļ�����(1:��Ƶ / 2:��Ƶ / 3:����Ƶ)
	CString	m_LinkMan;			// dip ����Ӧ��ID(sip ����Ӧ��ID����m_userid��)
	LPCTSTR	m_IDCSvrType;		// IDC����������(Ӣ�ı�ʾ) Ϊ�ջ�NULL��ʾ����Ҫ���IDCServer
	LPCTSTR m_IDCSvrSubType;	// IDC����������(���ı�ʾ) Ϊ�ջ�NULL��ʾ����Ҫ���IDCServer
	time_t	m_processtm;

	CString m_type;
	CString m_strText;			//ר������VOIP
protected:
	inline BOOL WriteIdcStat(LPCTSTR szType, LPCTSTR subClass)
	{

		if(v_stat && v_statIdc && szType != NULL && subClass != NULL
			&& szType[0] != 0 && subClass[0] != 0)
		{
			CString strISP;
			if (v_IdcIpLib.QueryIdcIP(Swap4Byte(*((DWORD*)m_cdip)), strISP))
			{
				CString idcstr;
				idcstr.Format("%03u.%03u.%03u.%03u\1%s\1%s\1"
					"����Ƶ����\1%s\1%s\1%s\1\1",
					m_cdip[0], m_cdip[1], m_cdip[2], m_cdip[3], strISP,
					szType, m_dip.Right(5), subClass, TYPESTATE_DECIDED);
				v_IdcServerStat[MEDIA_PROTOCOL].AddServer(idcstr);
			}
			else if (v_IdcIpLib.QueryIdcIP(Swap4Byte(*((DWORD*)m_csip)), strISP))
			{
				CString idcstr;
				idcstr.Format("%03u.%03u.%03u.%03u\1%s\1%s\1"
					"����Ƶ����\1%s\1%s\1%s\1\1",
					m_csip[0], m_csip[1], m_csip[2], m_csip[3], strISP,
					szType, m_sip.Right(5), subClass, TYPESTATE_DECIDED);
				v_IdcServerStat[MEDIA_PROTOCOL].AddServer(idcstr);
			}
		}
		return TRUE;
	}
};


class MEDIAPACKET
{
public:
	MEDIAPACKET(DATEHEADER& packh, const char* pbuf)
	{
		m_tm = packh.m_tm;
		m_vtm = packh.m_tm1;
		m_datalen = packh.m_datalen;
		m_buf = new char[m_datalen];
		memcpy(m_buf, pbuf, m_datalen);
		memcpy(m_sip, packh.m_sip, 4);
		memcpy(m_dip, packh.m_dip, 4);
		m_serial = packh.m_ipid;	
		if(packh.m_type == TCPPacket)
		{
			m_TCPSeq = packh.m_sequence;
			m_TCPAck = packh.m_ack;
			m_vtype = 1;
		}
		else if(packh.m_type == UDPPacket) 
		{
			m_vtype =0;
		}
		m_packettype = packh.m_type;
	}

	~MEDIAPACKET()	
	{
		if (m_buf)
		{
			delete[] m_buf;
		}
	}
	BYTE	m_vtype;		// ����(3.δ֪ 1.��Ƶ  2.��Ƶ)
	BYTE	m_packettype;	// ����(2 tcp 3 udp
	WORD	m_serial;		// �����к�udp/audio udp��3 4�ֽ�
	DWORD	m_TCPSeq;		// TCP�����к�
	DWORD	m_TCPAck;		// TCP�����к�
	time_t	m_tm;			// 
	time_t	m_vtm;
	BYTE	m_sip[4];		// ԴIP
	BYTE	m_dip[4];		// Ŀ��IP
	int		m_datalen;		// ���ݳ���
	char*	m_buf;			// ��������ַ
};


inline BOOL AFXAPI operator< (const MEDIAPACKET& s1, const MEDIAPACKET& s2)
{
	if(s1.m_packettype == TCPPacket)
	{
		if(memcmp(s1.m_sip,s2.m_sip,4)==0)
		{
			LONG dif = 0;
			dif = (LONG)(s1.m_TCPSeq - s2.m_TCPSeq);
			//�ֲ�ͬ�������Ƚ����д�С
			if(dif >= 0)
			{
				return FALSE;
			}
			else 
			{
				return TRUE;
			}
		}
		else
		{
			LONG dif=0;
			dif = (LONG)(s1.m_TCPAck - s2.m_TCPSeq);
			//�ֲ�ͬ�������Ƚ����д�С
			if(dif > 0)
			{
				return FALSE;
			}
			else
			{
				return TRUE;
			}
		}
	}
	else
	{
		ASSERT (s1.m_packettype == UDPPacket);
		if ((s2.m_vtype == s1.m_vtype) && (abs(s2.m_tm - s1.m_tm) <= 2))
		{
			if((s1.m_serial == 0) && (s2.m_serial == 0))
			{
				if (s1.m_tm == s2.m_tm)
				{
					return s1.m_vtm < s2.m_vtm;
				}
				else
				{
					return s1.m_tm < s2.m_tm;				
				}
			}
			else if((WORD)(s1.m_serial - s2.m_serial) < 0x8000)
			{
				return FALSE;
			}
			else
			{
				return TRUE;
			}
		}
		else
		{
			if (s1.m_tm == s2.m_tm)
			{
				return s1.m_vtm < s2.m_vtm;
			}
			else
			{
				return s1.m_tm < s2.m_tm;
			}
		}
	}
	return FALSE;
}


typedef list<MEDIAPACKET, allocator<MEDIAPACKET> > MediaPacketList;
typedef MediaPacketList::iterator MediaPacketListIt ;

//#include "..\\QQ_Crypt.h"

inline int WriteMediaFile(CFile& file, OLDDATEHEADER& packh, MEDIAPACKET& packet)
{
	if(file.GetLength()==0) //�ļ��״�д��,�մ������״�д��RSA���ܵ�key
	{
		file.Write("sdzw",4);
		file.Write((char*)&g_encodedlen,4);
		file.Write(g_encodedkeybuf,g_encodedlen);
	}

	ASSERT(file.m_hFile != CFile::hFileNull);
//VMPBEGIN
	UCHAR OUTBUF[2000];
//	UCHAR KEY[]="\x2\x9\xa\xf\x8\x4\xe\x7\x9\x0\x6\x3\x1\xc\xd\xb";
//	UCHAR KEY[]="\x22\x19\x4A\xFD\x87\x40\x4E\x87\xF9\x6A\xA6\xD3\xC1\x0A\xDD\xCB";		// New Key
	INT outbuflen = 0;
	packh.m_tm = packet.m_tm;
	packh.m_tm1 = packet.m_vtm;
	packh.m_datalen = packet.m_datalen;
	packh.m_sequence = packet.m_TCPSeq;
	packh.m_ack = packet.m_TCPAck;
	packh.m_ipid = packet.m_serial;
	qq_encrypt((UCHAR *)packet.m_buf, packh.m_datalen, g_test, OUTBUF, &outbuflen);
	packh.m_datalen = outbuflen;
//	memcpy(packh.mac, "\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1", 12);
	memcpy(packh.m_mac, "\x3\x3\x3\x3\x3\x3\x3\x3\x3\x3\x3\x3", 12);
	file.Write(&packh, sizeof(OLDDATEHEADER));
	file.Write(OUTBUF, outbuflen);
//VMPEND
	return 0;
}



inline int WriteMediaFile_old(CFile& file, OLDDATEHEADER& packh, UCHAR* packetbuf)
{
	if(file.GetLength()==0) //�ļ��״�д��,�մ������״�д��RSA���ܵ�key
	{
		file.Write("sdzw",4);
		file.Write((char*)&g_encodedlen,4);
		file.Write(g_encodedkeybuf,g_encodedlen);
	}

	ASSERT(file.m_hFile != CFile::hFileNull);
//VMPBEGIN
	UCHAR OUTBUF[2000];
//	UCHAR KEY[]="\x2\x9\xa\xf\x8\x4\xe\x7\x9\x0\x6\x3\x1\xc\xd\xb";
//	UCHAR KEY[]="\x22\x19\x4A\xFD\x87\x40\x4E\x87\xF9\x6A\xA6\xD3\xC1\x0A\xDD\xCB";		// New Key
	INT outbuflen = 0;
//	packh.m_tm = packet.m_tm;
//	packh.m_tm1 = packet.m_vtm;
//	packh.m_datalen = packet.m_datalen;
//	packh.m_sequence = packet.m_TCPSeq;
//	packh.m_ack = packet.m_TCPAck;
//	packh.m_ipid = packet.m_serial;
	qq_encrypt((UCHAR *)packetbuf, packh.m_datalen, g_test, OUTBUF, &outbuflen);
	packh.m_datalen = outbuflen;
//	memcpy(packh.mac, "\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1", 12);
	memcpy(packh.m_mac, "\x3\x3\x3\x3\x3\x3\x3\x3\x3\x3\x3\x3", 12);
	file.Write(&packh, sizeof(OLDDATEHEADER));
	file.Write(OUTBUF, outbuflen);
//VMPEND
	return 0;
}




inline int WriteMediaFile1(CFile& file, OLDDATEHEADER& packh, MEDIAPACKET& packet)
{
	ASSERT(file.m_hFile != CFile::hFileNull);
//VMPBEGIN
	UCHAR OUTBUF[2000];
//	UCHAR KEY[]="\x2\x9\xa\xf\x8\x4\xe\x7\x9\x0\x6\x3\x1\xc\xd\xb";
//	UCHAR KEY[]="\x22\x19\x4A\xFD\x87\x40\x4E\x87\xF9\x6A\xA6\xD3\xC1\x0A\xDD\xCB";		// New Key
	INT outbuflen = 0;
	packh.m_tm = packet.m_tm;
	packh.m_tm1 = packet.m_vtm;
	packh.m_datalen = packet.m_datalen;
	packh.m_sequence = packet.m_TCPSeq;
	packh.m_ack = packet.m_TCPAck;
	packh.m_ipid = packet.m_serial;

	char* key="\x22\x19\x4A\xFD\x87\x40\x4E\x87\xF9\x6A\xA6\xD3\xC1\x0A\xDD\xCB";
	qq_encrypt((UCHAR *)packet.m_buf, packh.m_datalen,(UCHAR*)key , OUTBUF, &outbuflen);
	packh.m_datalen = outbuflen;
//	memcpy(packh.mac, "\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1", 12);
	memcpy(packh.m_mac, "\x2\x2\x2\x2\x2\x2\x2\x2\x2\x2\x2\x2", 12);
	file.Write(&packh, sizeof(OLDDATEHEADER));
	file.Write(OUTBUF, outbuflen);
//VMPEND
	return 0;
}





BOOL CheckFlvMainFrame(CString realname);

#endif