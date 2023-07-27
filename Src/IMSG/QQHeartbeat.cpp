
#include "StdAfx.h"

#include "imsg.h"
#include "QQHeartbeat.h"


QQHeartbeat::QQHeartbeat(DATEHEADER& packh, int aT) : SocketSession(packh,aT)
{
	m_begfiletm = packh.m_tm;
	m_closetm = packh.m_tm + 120;
}

QQHeartbeat::~QQHeartbeat()
{
	
}

BOOL QQHeartbeat::CloseSession(time_t nowtm)
{
	return FALSE;
}

int QQHeartbeat::ProcessData(const char*pPacket,int n)
{
	if (1 == n)
	{
		u_long qq = ntohl(*(u_long *)(pPacket + 7));
// 		string tmp(pPacket + 7, 4);
// 		string tmp2 = get_string_from_hex_string((unsigned char *)tmp.c_str(), tmp.length());
// 		u_long tt = atol(tmp2.c_str());
// 		printf("tmp2=%s\n", tmp2.c_str());
		
		if (qq < 0)
		{
			return FALSE;
		}
		CString strSuffix = "imsg_oicq.indexfile.txt";
		m_indexfname = CreateFilename(m_sip, m_dip, strSuffix);
		CString txt;
		txt.Format("user=%u\r\nentity= 0\r\nstat=心跳(0)\r\nplatform=电脑\r\n",qq);
		WriteIndexFile("imsg_oicq", "imsg_oicq", txt, g_pImsgMissObj);
		
		return TRUE;
	}

	if (2 == n)
	{
		char qq[16] = {0};
		int len = 0;
		int qqlen = ntohl(*(int *)(pPacket + 14));
		qqlen -= 4;
		if (qqlen > 16)
		{
			return 0;
		}
		memcpy(qq,pPacket + 18,qqlen);
		
		
		for (int i = 0; i < 16; i++)
		{
			if ( ('0' <= qq[i]) && (qq[i] <= '9') )
			{
				len++;
			}
			else
			{
				break;
			}
		}
	
		if (len == qqlen)
		{
			CString strSuffix = "imsg_oicq.indexfile.txt";
			m_indexfname = CreateFilename(m_sip, m_dip, strSuffix);
			CString txt;
			txt.Format("user=%s\r\nentity= 0\r\nstat=心跳(1)\r\nplatform=手机(1)\r\n",qq);
			WriteIndexFile("imsg_oicq", "imsg_oicq", txt, g_pImsgMissObj);
			return 1;
		}


	}

	if ( n==3 )
	{
		/*
		char qq[16] = {0};
		
		int qqlen = ntohl(*(int *)(pPacket + 16));
		qqlen -= 4;
		if (qqlen > 16)
		{
			return 0;
		}
		memcpy(qq,pPacket + 20,qqlen);
		
		int len = 0;
		for (int i = 0; i < 16; i++)
		{
			if ( ('0' <= qq[i]) && (qq[i] <= '9') )
			{
				len++;
			}
			else
			{
				break;
			}
		}
		
		if (len == qqlen)
		{
			CString strSuffix = "imsg_oicq.indexfile.txt";
			m_indexfname = CreateFilename(m_sip, m_dip, strSuffix);
			CString txt;
			txt.Format("user=%s\r\nentity= 0\r\nstat=心跳(2)\r\nplatform=手机(2)\r\n",qq);
			WriteIndexFile("imsg_oicq", "imsg_oicq", txt, g_pImsgMissObj);
			return 1;
		}*/
	}
	return 0;
}

int QQHeartbeat::InsertPacket(DATEHEADER& packh,const char *pPacket)
{
	return 0;
}

