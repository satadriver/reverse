//***************************************************************
//	Copyright (C) 2007 - 山东兆物科技发展有限公司版权所有
//	文件名: 	OicqBuddy
//	版本:		1.0
//	创建日期:	2007-7-13
//	作者:		荣强
//	主要功能:	处理OICQ的联系人的包
//	模块描述:	
//	主要函数:	ProcessOicqBuddyPacket 处理入口函数
//				WriteOicqBuddyFile 输出QQ联系人的数据
//**************************************************************** 

#include "StdAfx.h"
#include "imsg.h"
#include "oicq.h"

//********************** Oicq点对点协议处理 **********************************//

OicqSocketMap g_OicqBuddyMap(8192);


//*************************************************
//	函数名称:		ProcessOicqBuddyPacket
//	描述:			处理OICQ的联系人的包的入口函数
//	输入参数:		packh 是包头结构
//					pPacket 是包数据
//					sypaddr 包的截获位置
//					PacketFrom 包的原始类型
//	输出参数:		
//	返回值:			如果这个包是OICQ的联系人的包就返回TRUE, 否则返回FALSE
//	其它:			
//**************************************************
BOOL ProcessOicqBuddyPacket(
				DATEHEADER& packh,
				const char* pPacket, 
				LPCTSTR spyaddr, 
				int PacketFrom)
{
	DWORD id[3];
	int type = 1;
	try
	{
		if (packh.m_type==UDPPacket || packh.m_type==ProxyUDP)
		{
			if (packh.m_dport==0)
				return FALSE;
			if ((*pPacket==0) && (pPacket[1]>4) && (pPacket[1]<0x10) && (pPacket[3]!=0) &&
				(pPacket[3]!=0xFF) && (packh.m_datalen>52) && (packh.m_datalen<120) &&
				((packh.m_datalen&3)==0))
			{
				DWORD idrand;
				memset(&idrand, pPacket[3], 4);
				memcpy(id, pPacket+4, 8);
				id[0] = ~(id[0]^idrand);
				id[1] = ~(id[1]^idrand);
				id[0] = Swap4Byte(id[0]);
				id[1] = Swap4Byte(id[1]);
				if ((id[0]<10000 || id[1]<10000) ||
					(id[0]>700000000 || id[1]>700000000))
				{
					return FALSE;
				}
			}
			else if (packh.m_datalen>34 &&
				(memcmp(pPacket+18, "SIP/", 4)==0) &&
				(memcmp(pPacket+26, "200 OK\r\n",8)==0))
			{
				CString str = CString(pPacket+34, packh.m_datalen-34);
				str.MakeLower();
				if (str.Find("tencent=tencentsipstack")<0)
					return FALSE;
				DWORD idrand;
				memset(&idrand, pPacket[3], 4);
				memcpy(id, pPacket+4, 8);
				id[0] = ~(id[0]^idrand);
				id[1] = ~(id[1]^idrand);
				id[0] = Swap4Byte(id[0]);
				id[1] = Swap4Byte(id[1]);
				type = 2;
				if ((id[0]<10000 || id[1]<10000) ||
					(id[0]>700000000 || id[1]>700000000))
				{
					return FALSE;
				}
			}
			else
				return FALSE;
		}
		else
		{
			CString str;
			if ((packh.m_datalen>0xb+14) &&
				strnicmp(pPacket+0xb, "SIP/2.0 200 OK", 14)==0)
			{
				str = CString(pPacket+0xB, packh.m_datalen-0xB);
			}
			else if ((packh.m_datalen>0xb+31+14) && 
				strnicmp(pPacket+0xb+31, "SIP/2.0 200 OK", 14)==0)
			{
				str = CString(pPacket+0xB+31, packh.m_datalen-0xB-31);
			}
			else
				return FALSE;
				
			str.MakeLower();
			CString tempid;
			int pos = str.Find("tencent=tencentsipstack");
			if(pos<0)
				return FALSE;

			pos = str.Find("from: <sip:", 0);
			if (pos<0)
				return FALSE;
			int pos1 = str.Find("@", pos);
			if (pos1<pos || pos1>pos+22)
				return FALSE;
			tempid = str.Mid(pos+11, pos1-pos-11);
			id[2] = atoi(tempid);
			if (id[2] < 10000)
				return FALSE;

			pos = str.Find("to: <sip:", pos);
			if (pos<0)
				return FALSE;
			pos1 = str.Find("@", pos);
			if (pos1<pos || pos1>pos+20)
				return FALSE;
			tempid = str.Mid(pos+9, pos1-pos-9);
			id[1] = atoi(tempid);
			if (id[1] < 10000)
				return FALSE;

			pos = str.Find("contact: <sip:", pos);
			if (pos<0)
			{
				if (id[2] != id[1])
					id[0] = id[2];
				else
					return FALSE;
			}
			pos1 = str.Find("@", pos);
			if (pos1<pos || pos1>pos+25)
				return FALSE;
			tempid = str.Mid(pos+14, pos1-pos-14);
			id[0] = atoi(tempid);
			if (id[0] < 10000)
				return FALSE;
			if (id[0] == id[1])
			{
				if (id[0] == id[2])
					return FALSE;
				else
					id[1] = id[2];
			}
			type = 2;
		}
		
		SOCKETKEY skey1;
		memcpy(&skey1.sip, &packh.m_sip, 4);
		memcpy(&skey1.dip, &packh.m_dip, 4);
		skey1.sport = packh.m_sport;
		skey1.dport = packh.m_dport;
		OicqBuddy* sion = 0;
		if (g_OicqBuddyMap.Lookup(skey1, sion))
		{
			ASSERT(sion!=NULL);
			if ((id[0] == sion->m_QQid1) && (id[1] == sion->m_QQid2))
			{
				sion->m_PitchonPacket++;
			}
		}
		else
		{
			SOCKETKEY skey2;
			memcpy(&skey2.dip, &packh.m_sip, 4);
			memcpy(&skey2.sip, &packh.m_dip, 4);
			skey2.dport = packh.m_sport;
			skey2.sport = packh.m_dport;
			sion = 0;
			if (g_OicqBuddyMap.Lookup(skey2, sion))
			{
				if ((id[0] == sion->m_QQid2) && (id[1] == sion->m_QQid1))
				{
					sion->m_PitchonPacket++;
				}
				memcpy(&skey1, &skey2, sizeof(SOCKETKEY));
			}
		}
		if (sion != NULL)
		{
			sion->m_TotalPacket++;
			if (packh.m_type >= ProxyTCP)
			{
				sion->m_proxytype = 1;
			}
			if (sion->m_linktype == 1)
			{
				if (type == 2)
				{
					sion->m_linktype = 2;
				}
			}
		}
		else
		{
			if (id[0] != id[1])
			{
				sion = new OicqBuddy;
				if (packh.m_type>=ProxyTCP)
				{
					sion->m_proxytype = 1;
				}
				else
				{
					sion->m_proxytype = 0;
				}
				sion->m_QQid1 = id[0];
				sion->m_QQid2 = id[1];
				sion->m_FirstPackettm = packh.m_tm;
				sion->m_PitchonPacket = sion->m_TotalPacket = 1;
				memcpy(sion->m_mac, packh.m_mac, 12);
				sion->m_linktype = type;
				sion->m_spyaddr = spyaddr;
				BOOL isCDMA = FALSE;
				if ((PacketFrom == CDMATCP) || (PacketFrom == CDMAUDP))
				{
					isCDMA = TRUE;
					sion->m_DataSrc = DATASRC_CDMA;
				}
				else if ((PacketFrom == GPRSTCP) || (PacketFrom == GPRSUDP))
				{
					sion->m_DataSrc = DATASRC_GPRS;
				}
				else
				{
					sion->m_DataSrc = DATASRC_OTHER;
				}
				sion->m_authinfo.GetAuthInfo(packh, isCDMA);
				g_OicqBuddyMap.SetAt(skey1, sion);
			}
		}
		return TRUE;
	}
	catch(...)
	{
		WriteLog(IMSGPRO, "ProcessOicqBuddyPacket() Error!!");
	}
	return FALSE;
}

void OicqSocketMap::ClearOicqSocketMap(time_t now)
{
	POSITION pos = GetStartPosition();
	SOCKETKEY  key;
	OicqBuddy* s = NULL;
	while (pos)
	{
		GetNextAssoc(pos, key, s);
		if (abs(now - s->m_FirstPackettm) > 90)
		{
			if ((s->m_TotalPacket > 4) && (s->m_linktype != 3)
				&& (s->m_TotalPacket >= s->m_PitchonPacket)
				&& (s->m_TotalPacket < s->m_PitchonPacket + 2))
			{
				WriteOicqBuddyFile(s, key);
			}
			s->m_linktype = 3;
			if (abs(now - s->m_FirstPackettm) > 180)
			{
				RemoveKey(key);
			}
		}
	}
}

//*************************************************
//	函数名称:		WriteOicqBuddyFile
//	描述:			输出QQ联系人的数据(对照文件和活动历史)
//	输入参数:		pSion QQ联系人的会话指针
//					key 包的IP和端口
//					
//	输出参数:		
//	返回值:			
//	其它:			
//**************************************************
void WriteOicqBuddyFile(OicqBuddy *pSion, SOCKETKEY& key)
{
	// 先临时构造一个OicqSession对象
	OicqSession tmpsession(pSion, key);

	// 匹配案件、预警和屏蔽信息
	DWORD dwstate = 0;
	SearchCaseOrWarn(&tmpsession, tmpsession.m_userid,
		tmpsession.m_userid.GetLength(), dwstate);
	SearchCaseEmailIm(&tmpsession, tmpsession.m_userid,
		tmpsession.m_userid.GetLength(), 0);

	// 匹配数据源不输出IDCServer
	if (tmpsession.m_dataSrc == DATASRC_OTHER)
	{
		if ((v_IdcIpLib.QueryIdcIP(Swap4Byte(key.dip), tmpsession.m_strISP))
			|| (v_IdcIpLib.QueryIdcIP(Swap4Byte(key.sip), tmpsession.m_strISP)))
		{
			tmpsession.m_dataSrc = DATASRC_IDC;
		}
	}

	// 输出对照文件
	tmpsession.m_indexfname = CreateFilename(tmpsession.m_sip, 
		tmpsession.m_dip, "imsg_oicq.indexfile.txt");
	CString stetxt;
	stetxt.Format("user= %s\r\nentity= 0\r\nstat= 通讯\r\n", tmpsession.m_userid);
	tmpsession.WriteIndexFile("imsg_oicq", "imsg_oicq", stetxt, g_pImsgMissObj);

	// 输出活动历史(两个方向各输出一遍)
 	if (v_statImsgQQ)
	{
		HistoryRecord hisRec;
		hisRec.SetRecord(IDX_FORWTYPE,	"2");
		CString strID1, strID2;
		strID1.Format("%d", pSion->m_QQid1);
		hisRec.SetRecord(IDX_ID,		strID1);
		hisRec.SetRecord(IDX_USER,		strID1);

		strID2.Format("%d", pSion->m_QQid2);
		hisRec.SetRecord(IDX_CONTACT,	strID2);
		hisRec.SetRecord(IDX_STATE,		"通讯");
		tmpsession.WriteHisData("imsg_oicq", "imsg_oicq", &hisRec, g_pImsgMissObj,IMSG_PROTOCOL);//, HISTORY_DATA);

		// 交换方向
		CString tstr = tmpsession.m_sip;
		tmpsession.m_sip = tmpsession.m_dip;
		tmpsession.m_dip = tstr;
		tstr = tmpsession.m_dmac;
		tmpsession.m_dmac = tmpsession.m_smac;
		tmpsession.m_smac = tstr;
		DWORD ip;
		memcpy(&ip, tmpsession.m_csip, 4);
		memcpy(tmpsession.m_csip, tmpsession.m_cdip, 4);
		memcpy(tmpsession.m_cdip, &ip, 4);
		tmpsession.m_AuthInfo.SwapAuthInfo();
		HistoryRecord hisRec2;
		hisRec2.SetRecord(IDX_FORWTYPE,	"2");
		strID1.Format("%d", pSion->m_QQid2);
		hisRec2.SetRecord(IDX_ID,		strID1);
		hisRec.SetRecord(IDX_USER,		strID1);

		strID2.Format("%d", pSion->m_QQid1);
		hisRec2.SetRecord(IDX_CONTACT,	strID2);
		hisRec2.SetRecord(IDX_STATE,		"通讯");
		tmpsession.WriteHisData("imsg_oicq", "imsg_oicq", &hisRec2, g_pImsgMissObj,	IMSG_PROTOCOL);//, HISTORY_DATA);
		
	}

}

